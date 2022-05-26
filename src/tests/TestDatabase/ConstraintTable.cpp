// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ConstraintTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/Utils.h"

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *ConstraintTable::NAME;

		/*
		 * Note that since we are adding columns with NOT NULL, UNIQUE and PRIMARY KEY constraints, we have to make sure
		 * that upon every insert that we are performing, we also add explicit values for these columns as otherwise the
		 * respective constraints might easily get violated (e.g. because NULL is inserted as a default value or the
		 * same value (NULL/EMPTY) is inserted multiple times.
		 */

		ConstraintTable::ConstraintTable(soci::session &sql, Backend backend) : Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(Column("not_null_key", DataType(DataType::String, 100),
									 { Constraint(Constraint::NotNull) }, "myDefaultValue"));
			columns.push_back(Column("not_null_value", DataType(DataType::String, 100)));

			columns.push_back(
				Column("unique_key", DataType(DataType::String, 100), { Constraint(Constraint::Unique) }));
			columns.push_back(Column("unique_value", DataType(DataType::String, 100)));

			setColumns(columns);
		}

		void ConstraintTable::insertNotNull(const std::string &key, const std::string &value) {
			std::string dummy = "dummy" + std::to_string(dummyCounter++);
			try {
				m_sql << "INSERT INTO \"" + getName()
							 + "\" (not_null_key, not_null_value, unique_key) VALUES (:key, :value, "
							   ":dummy1)",
					soci::use(key), soci::use(value), soci::use(dummy);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting not-null value: ") + e.what());
			}
		}

		void ConstraintTable::insertUnique(const std::string &key, const std::string &value) {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (unique_key, unique_value) VALUES (:key, :value)",
					soci::use(key), soci::use(value);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting unique value: ") + e.what());
			}
		}

		std::string ConstraintTable::selectNotNull(const std::string &key) {
			try {
				std::string value;
				m_sql << "SELECT not_null_value FROM \"" + getName() + "\" WHERE not_null_key = :key", soci::use(key),
					soci::into(value);

				::mumble::db::utils::verifyQueryResultedInData(m_sql);

				return value;
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at fetching not-null value: ") + e.what());
			}
		}

		std::string ConstraintTable::selectUnique(const std::string &key) {
			try {
				std::string value;
				m_sql << "SELECT unique_value FROM \"" + getName() + "\" WHERE unique_key = :key", soci::use(key),
					soci::into(value);

				::mumble::db::utils::verifyQueryResultedInData(m_sql);

				return value;
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at fetching unique value: ") + e.what());
			}
		}

		void ConstraintTable::insertNullInNotNullCol() {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (not_null_key, not_null_value) VALUES (NULL, 'NullValue')";
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting NULL value in not-NULL col: ") + e.what());
			}
		}

	} // namespace test
} // namespace db
} // namespace mumble
