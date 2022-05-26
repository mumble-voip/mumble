// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "KeyValueTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/DataType.h"
#include "database/Utils.h"

#include <vector>

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *KeyValueTable::NAME;

		KeyValueTable::KeyValueTable(soci::session &sql, Backend backend) : Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(Column("key_col", DataType(DataType::String, 150)));
			columns.push_back(Column("value_col", DataType(DataType::String, 150)));

			setColumns(columns);
		}

		void KeyValueTable::insert(const std::string &key, const std::string &value) {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (key_col, value_col) VALUES (:key, :value)", soci::use(key),
					soci::use(value);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting key-value-pair: ") + e.what());
			}
		}

		std::string KeyValueTable::query(const std::string &key) {
			std::string value;
			try {
				m_sql << "SELECT value_col FROM \"" + getName() + "\" WHERE key_col = :key", soci::use(key),
					soci::into(value);
			} catch (const soci::soci_error &e) {
				throw AccessException("Failed at querying value for key \"" + key + "\": " + e.what());
			}

			db::utils::verifyQueryResultedInData(m_sql);

			return value;
		}

	} // namespace test
} // namespace db
} // namespace mumble
