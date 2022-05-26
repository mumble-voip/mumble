// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "AutoIncrementTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/PrimaryKey.h"
#include "database/Utils.h"

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *AutoIncrementTable::NAME;

		AutoIncrementTable::AutoIncrementTable(soci::session &sql, Backend backend) : Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(Column("key_col", DataType(DataType::String, 100)));
			columns.push_back(Column("value_col", DataType(DataType::Integer), { Constraint(Constraint::NotNull) },
									 Column::Flag::AUTOINCREMENT));

			setColumns(columns);

			PrimaryKey pk(columns[1]);
			setPrimaryKey(pk);
		}

		void AutoIncrementTable::insert(const std::string &key) {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (key_col) VALUES (:key)", soci::use(key);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting autoincrement value: ") + e.what());
			}
		}

		int AutoIncrementTable::select(const std::string &key) {
			try {
				int value;
				m_sql << "SELECT value_col FROM \"" + getName() + "\" WHERE key_col = :key", soci::use(key),
					soci::into(value);

				::mumble::db::utils::verifyQueryResultedInData(m_sql);

				return value;
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at querying autoincrement value: ") + e.what());
			}
		}
	} // namespace test
} // namespace db
} // namespace mumble
