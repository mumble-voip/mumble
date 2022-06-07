// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PrimaryKeyTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/DataType.h"
#include "database/PrimaryKey.h"
#include "database/Utils.h"

#include <vector>

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *PrimaryKeyTable::NAME;

		PrimaryKeyTable::PrimaryKeyTable(soci::session &sql, Backend backend) : Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(Column("pk_key", DataType(DataType::VarChar, 100), { Constraint(Constraint::NotNull) }));
			columns.push_back(Column("pk_value", DataType(DataType::VarChar, 100), { Constraint(Constraint::NotNull) }));

			setColumns(columns);

			PrimaryKey pk(columns[0]);
			setPrimaryKey(pk);
		}

		void PrimaryKeyTable::insert(const std::string &key, const std::string &value) {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (pk_key, pk_value) VALUES (:key, :value)", soci::use(key),
					soci::use(value);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting: ") + e.what());
			}
		}

		void PrimaryKeyTable::insertNull() {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (pk_key, pk_value) VALUES (NULL, 'dummy')";
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting NULL value: ") + e.what());
			}
		}

		std::string PrimaryKeyTable::select(const std::string &key) {
			std::string value;
			try {
				m_sql << "SELECT pk_value FROM \"" + getName() + "\" WHERE pk_key = :key", soci::use(key),
					soci::into(value);
			} catch (const soci::soci_error &e) {
				throw AccessException("Failed at querying value for key \"" + key + "\": " + e.what());
			}

			db::utils::verifyQueryResultedInData(m_sql);

			return value;
		}

		void PrimaryKeyTable::dropKey(const std::string &key) {
			try {
				m_sql << "DELETE FROM \"" + getName() + "\" WHERE pk_key = :key", soci::use(key);
			} catch (const soci::soci_error &e) {
				throw AccessException("Failed at dropping key \"" + key + "\": " + e.what());
			}
		}

	} // namespace test
} // namespace db
} // namespace mumble
