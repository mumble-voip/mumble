// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_AUTOINCREMENTTTABLE_H_
#define MUMBLE_TEST_TESTDATABASE_AUTOINCREMENTTTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
}

namespace mumble {
namespace db {
	namespace test {

		class AutoIncrementTable : public Table {
		public:
			static constexpr const char *NAME = "AutoIncrementTable";

			AutoIncrementTable(soci::session &sql, Backend backend);
			~AutoIncrementTable() = default;

			void insert(const std::string &key);
			int select(const std::string &key);
		};
	} // namespace test
} // namespace db
} // namespace mumble

#endif // MUMBLE_TEST_TESTDATABASE_AUTOINCREMENTTTABLE_H_
