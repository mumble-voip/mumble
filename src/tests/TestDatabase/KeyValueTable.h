// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_KEYVALUE_TABLE_H_
#define MUMBLE_TEST_TESTDATABASE_KEYVALUE_TABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
}

namespace mumble {
namespace db {
	namespace test {
		class KeyValueTable : public Table {
		public:
			static constexpr const char *NAME = "keyValueTable";

			KeyValueTable(soci::session &sql, Backend backend);
			~KeyValueTable() = default;

			void insert(const std::string &key, const std::string &value);
			std::string query(const std::string &key, const std::string &defaultValue = {});
		};
	} // namespace test
} // namespace db
} // namespace mumble

#endif // MUMBLE_TEST_TESTDATABASE_KEYVALUE_TABLE_H_
