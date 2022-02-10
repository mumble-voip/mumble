// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DefaultTable.h"
#include "database/AccessException.h"
#include "database/Column.h"
#include "database/DataType.h"
#include "database/Utils.h"

#include <soci/soci.h>

namespace mumble {
namespace db {
	namespace test {

		constexpr const char *DefaultTable::NAME;
		constexpr const char *DefaultTable::DEFAULT;

		DefaultTable::DefaultTable(soci::session &sql, Backend backend) : Table(sql, backend, NAME) {
			std::vector< Column > columns;

			columns.push_back(Column("DefaultColKey", DataType(DataType::String, 100)));
			columns.push_back(Column("DefaultColValue", DataType(DataType::String, 100), {}, DEFAULT));

			setColumns(columns);
		}

		void DefaultTable::insert(const std::string &key) {
			try {
				m_sql << "INSERT INTO \"" + getName() + "\" (DefaultColKey) VALUES (:key)", soci::use(key);
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting default value: ") + e.what());
			}
		}

		std::string DefaultTable::select(const std::string &key) {
			try {
				std::string value;
				m_sql << "SELECT DefaultColValue FROM \"" + getName() + "\" WHERE DefaultColKey = :key", soci::use(key),
					soci::into(value);

				::mumble::db::utils::verifyQueryResultedInData(m_sql);

				return value;
			} catch (const soci::soci_error &e) {
				throw AccessException(std::string("Failed at inserting default value: ") + e.what());
			}
		}
	} // namespace test
} // namespace db
} // namespace mumble
