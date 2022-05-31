// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ForeignKey.h"
#include "Column.h"
#include "Table.h"

#include <cassert>

namespace mumble {
namespace db {

	ForeignKey::ForeignKey() {}

	ForeignKey::ForeignKey(const Table &foreignTable, const std::vector< Column > &columns) {
		setColumns(foreignTable, columns);
	}

	ForeignKey::ForeignKey(const Table &foreignTable, const std::vector< const Column * > &columns) {
		setColumns(foreignTable, columns);
	}

	const std::vector< std::string > &ForeignKey::getForeignColumnNames() const { return m_foreignColumnNames; }

	const std::vector< std::string > &ForeignKey::getColumnNames() const { return m_columnNames; }

	const std::string &ForeignKey::getForeignTableName() const { return m_foreignTableName; }

	void ForeignKey::setColumns(const Table &foreignTable, const std::vector< Column > &columns) {
		std::vector< const Column * > columnPtr;
		columnPtr.reserve(columns.size());

		for (const Column &current : columns) {
			columnPtr.push_back(&current);
		}

		setColumns(foreignTable, columnPtr);
	}

	void ForeignKey::setColumns(const Table &foreignTable, const std::vector< const Column * > &columns) {
		assert(foreignTable.hasPrimaryKey());
		assert(foreignTable.getPrimaryKey().getColumnNames().size() == columns.size());

#ifndef NDEBUG
		for (std::size_t i = 0; i < columns.size(); ++i) {
			const Column *foreignColumn = foreignTable.findColumn(foreignTable.getPrimaryKey().getColumnNames()[i]);

			// Assert that column exists
			assert(foreignColumn);

			// Assert that the column's type matches that of the foreign column
			assert(foreignColumn->getType() == columns[i]->getType());
		}
#endif

		m_foreignTableName = foreignTable.getName();

		m_foreignColumnNames.clear();
		m_foreignColumnNames = foreignTable.getPrimaryKey().getColumnNames();

		m_columnNames.clear();
		m_columnNames.reserve(columns.size());

		for (const Column *currentCol : columns) {
			m_columnNames.push_back(currentCol->getName());
		}
	}

	std::string ForeignKey::sql() const {
		assert(!m_foreignColumnNames.empty());
		assert(!m_foreignTableName.empty());
		assert(!m_columnNames.empty());
		assert(m_columnNames.size() == m_foreignColumnNames.size());

		std::string sql = "FOREIGN KEY (";

		for (std::size_t i = 0; i < m_columnNames.size(); ++i) {
			sql += m_columnNames[i];

			if (i + 1 < m_columnNames.size()) {
				sql += ", ";
			}
		}

		sql += ") REFERENCES \"" + m_foreignTableName + "\"(";

		for (std::size_t i = 0; i < m_foreignColumnNames.size(); ++i) {
			sql += m_foreignColumnNames[i];

			if (i + 1 < m_foreignColumnNames.size()) {
				sql += ", ";
			}
		}

		// We want to make sure that the referencing key is always staying synchronized with the referenced
		// (foreign) key. Thus, if the referenced value changes, we want the reference to change as well and if
		// the referenced value gets deleted, we also want to delete the row referencing that value
		sql += ") ON UPDATE CASCADE ON DELETE CASCADE";

		return sql;
	}

	bool operator==(const ForeignKey &lhs, const ForeignKey &rhs) {
		return lhs.m_foreignColumnNames == rhs.m_foreignColumnNames && lhs.m_columnNames == rhs.m_columnNames
			   && lhs.m_foreignTableName == rhs.m_foreignTableName;
	}

	bool operator!=(const ForeignKey &lhs, const ForeignKey &rhs) { return !(lhs == rhs); }

} // namespace db
} // namespace mumble
