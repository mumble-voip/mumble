// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ForeignKeyTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/DataType.h"
#include "database/ForeignKey.h"
#include "database/Utils.h"

#include <vector>

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *ForeignKeyTable::NAME;

		ForeignKeyTable::ForeignKeyTable(soci::session &sql, Backend backend, const Table &foreignTable)
			: Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(Column("fk_key", DataType(DataType::String, 100), { Constraint(Constraint::NotNull) }));
			columns.push_back(Column("fk_value", DataType(DataType::String, 100), { Constraint(Constraint::NotNull) }));

			setColumns(columns);

			ForeignKey foreignKey(foreignTable, { columns[0] });
			addForeignKey(foreignKey);
		}

		void ForeignKeyTable::insert(const std::string &key, const std::string &value) {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (fk_key, fk_value) VALUES (:key, :value)", soci::use(key),
					soci::use(value);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting: ") + e.what());
			}
		}

		void ForeignKeyTable::insertNull() {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (fk_key, fk_value) VALUES (NULL, 'dummy')";
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting NULL value: ") + e.what());
			}
		}

		std::string ForeignKeyTable::select(const std::string &key) {
			std::string value;
			try {
				m_sql << "SELECT fk_value FROM \"" + getName() + "\" WHERE fk_key = :key", soci::use(key),
					soci::into(value);
			} catch (const soci::soci_error &e) {
				throw AccessException("Failed at querying value for key \"" + key + "\": " + e.what());
			}

			db::utils::verifyQueryResultedInData(m_sql);

			return value;
		}

		void ForeignKeyTable::dropKey(const std::string &key) {
			try {
				m_sql << "DELETE FROM \"" + getName() + "\" WHERE fk_key = :key", soci::use(key);
			} catch (const soci::soci_error &e) {
				throw AccessException("Failed at dropping key \"" + key + "\": " + e.what());
			}
		}

	} // namespace test
} // namespace db
} // namespace mumble
