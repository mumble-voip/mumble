// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Constraint.h"
#include "Backend.h"
#include "Column.h"
#include "DataType.h"
#include "Table.h"
#include "UnsupportedOperationException.h"

#include <algorithm>
#include <cassert>

namespace mumble {
namespace db {

	Constraint::Constraint(Constraint::Type type, const std::string &name, const Table *foreignTable,
						   const Column *foreignColumn)
		: m_type(type), m_name(name) {
		// clang-format off
		assert(m_type != Constraint::ForeignKey || (
					!m_name.empty() 
					&& foreignTable
					&& foreignColumn
					// Assert that the foreign column is the primary key of the foreign table
					// (otherwise we can't create a foreign key on it)
					&& std::find_if(
					   foreignColumn->getConstraints().begin(), foreignColumn->getConstraints().end(),
					   [](const Constraint &constraint) { return constraint.getType() == Constraint::PrimaryKey; })
				   != foreignColumn->getConstraints().end()
				   // In theory we should also assert whether foreignColumn is part of foreignTable, but at the time
				   // this code runs, it is somewhat likely that the column has not yet been added to its table, so
				   // we can't assert on that.
		));
		// clang-format on

		if (m_type == Constraint::ForeignKey) {
			m_foreignColumnName = foreignColumn->getName();
			m_foreignTableName  = foreignTable->getName();
		}
	}

	Constraint::Type Constraint::getType() const { return m_type; }

	void Constraint::setType(Constraint::Type type) { m_type = type; }

	const std::string Constraint::getName() const { return m_name; }

	void Constraint::setName(const std::string &name) { m_name = name; }

	bool Constraint::canInline() const {
		// All constraints can be applied inline, but if we have defined a name for this constraint,
		// then we can't inline it, as inlined constraints can't have names.
		return m_name.empty() || m_type == Constraint::ForeignKey;
	}

	bool Constraint::canOutOfLine() const {
		// For out-of-line constraints, we require a name
		return !m_name.empty();
	}

	bool Constraint::prefersInline() const {
		// https://stackoverflow.com/a/54689613 - apparently applying NOT NULL inline, is (can be) more efficient
		return m_type == Constraint::NotNull || m_name.empty();
	}

	bool Constraint::canBeDropped(Backend backend) const {
		return backend != Backend::SQLite && (!m_name.empty() || m_type == Constraint::NotNull);
	}

	std::string Constraint::inlineSQL(Backend backend) const {
		assert(m_name.empty());

		switch (m_type) {
			case Constraint::NotNull:
				return "NOT NULL";
			case Constraint::Unique:
				return "UNIQUE";
			case Constraint::PrimaryKey: {
				switch (backend) {
					case Backend::MySQL:
						// Fallthrough
					case Backend::PostgreSQL:
						return "PRIMARY KEY";
					case Backend::SQLite:
						// In SQLite primary keys do not imply NOT NULL (in most cases) due to historic reasons, so in
						// order to make sure our table conforms to the SQL standard (which does dictate that a primary
						// key must not be NULL), we have to add the NOT NULL constraint ourselves.
						return "PRIMARY KEY NOT NULL";
				}

				// This code should be unreachable
				assert(false);
				return "PRIMARY KEY";
			}
			case Constraint::ForeignKey:
				assert(false);
				throw UnsupportedOperationException("Foreign keys can't be added inline");
		}

		// This code should never be reached
		assert(false);
		return "";
	}

	std::string Constraint::outOfLineSQL(const Column &column, Backend backend) const {
		assert(!m_name.empty());

		std::string query = "CONSTRAINT " + m_name + " ";

		switch (m_type) {
			case Constraint::NotNull:
				// In order to have the NOT NULL constraint as a out-of-line constraint, we'll have to rewrite
				// it into a CHECK constraint
				query += "CHECK(" + column.getName() + " IS NOT NULL)";
				break;
			case Constraint::Unique:
				query += "UNIQUE(" + column.getName() + ")";
				break;
			case Constraint::PrimaryKey:
				query += "PRIMARY KEY (" + column.getName() + ")";
				switch (backend) {
					case Backend::MySQL:
						// Fallthrough
					case Backend::PostgreSQL:
						// Do nothing special
						break;
					case Backend::SQLite:
						// Explicitly also add NOT NULL as this is not implied for most cases in SQLite (see inlineSQL
						// for more info).
						// Note that since SQLite doesn't allow for dropping constraints anyway, we don't have to worry
						// about remembering the dummy name.
						query += ", CONSTRAINT " + m_name + "_sqlite_special_not_null " + "CHECK(" + column.getName()
								 + " IS NOT NULL)";
						break;
				}
				break;
			case Constraint::ForeignKey:
				assert(!m_foreignTableName.empty());
				assert(!m_foreignColumnName.empty());

				// We want to make sure that the referencing key is always staying synchronized with the referenced
				// (foreign) key. Thus, if the referenced value changes, we want the reference to change as well and if
				// the referenced value gets deleted, we also want to delete the row referencing that value.
				query += "FOREIGN KEY(" + column.getName() + ") REFERENCES \"" + m_foreignTableName + "\"("
						 + m_foreignColumnName + ") ON UPDATE CASCADE ON DELETE CASCADE";
				break;
		}

		assert(!query.empty());

		return query;
	}

	std::string Constraint::dropQuery(const Table &table, const Column &column, Backend backend) const {
		if (backend == Backend::SQLite) {
			throw UnsupportedOperationException("SQLite does not support dropping constraints from tables");
		} else if (backend == Backend::MySQL) {
			// MySQL is weird in that it doesn't allow for the standard "DROP CONSTRAINT <name>" syntax, that
			// virtually every other RDMS seems to support. Instead, we'll have to use a specific syntax for
			// dropping different kinds of constraints. In theory, MySQL 8.0.19 added support for the standard
			// DROP CONSTRAINT approach but based on preliminary tests, this support is still not quite
			// compatible with other RDMS, so for now we'll stick to using special code for MySQL...
			switch (m_type) {
				case Constraint::NotNull:
					if (m_name.empty()) {
						return "ALTER TABLE \"" + table.getName() + "\" MODIFY " + column.getName() + " "
							   + column.getType().sqlRepresentation() + " NULL";
					} else {
						// Named constraints for NOT NULL are implemented as a check
						return "ALTER TABLE \"" + table.getName() + "\" DROP CHECK " + m_name;
					}
				case Constraint::Unique:
					assert(!m_name.empty());
					return "ALTER TABLE \"" + table.getName() + "\" DROP INDEX " + m_name;
				case Constraint::PrimaryKey:
					return "ALTER TABLE \"" + table.getName() + "\" DROP PRIMARY KEY";
				case Constraint::ForeignKey:
					assert(!m_name.empty());
					return "ALTER TABLE \"" + table.getName() + "\" DROP FOREIGN KEY " + m_name;
			}
		}

		if (!m_name.empty()) {
			return "ALTER TABLE \"" + table.getName() + "\" DROP CONSTRAINT " + m_name;
		}

		switch (m_type) {
			case Constraint::NotNull:
				if (backend == Backend::PostgreSQL) {
					// Postgres is doing this differently than everyone else
					return "ALTER TABLE \"" + table.getName() + "\" ALTER COLUMN " + column.getName()
						   + " DROP NOT NULL";
				} else {
					return "ALTER TABLE \"" + table.getName() + "\" MODIFY " + column.getName() + " "
						   + column.getType().sqlRepresentation() + " NULL";
				}
			case Constraint::PrimaryKey:
			case Constraint::Unique:
			case Constraint::ForeignKey:
				// In all of these cases, we'd need a name to drop the constraint. But if we had one, we'd not reach
				// this code
				throw UnsupportedOperationException(
					"The only constraint that can be dropped without a name is NotNull");
				return "";
		}

		// This code should never be reached
		assert(false);
		return "";
	}

} // namespace db
} // namespace mumble
