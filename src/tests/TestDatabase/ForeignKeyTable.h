// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_FOREIGNKEYTABLE_H_
#define MUMBLE_TEST_TESTDATABASE_FOREIGNKEYTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
}

namespace mumble {
namespace db {
	namespace test {

		class ForeignKeyTable : public Table {
		public:
			static constexpr const char *NAME = "foreignKeyTable";

			ForeignKeyTable(soci::session &sql, Backend backend, const Table &foreignTable);
			~ForeignKeyTable() = default;

			void insert(const std::string &key, const std::string &value);
			void insertNull();
			std::string select(const std::string &key);
			void dropKey(const std::string &key);
		};

	} // namespace test
} // namespace db
} // namespace mumble

#endif // MUMBLE_TEST_TESTDATABASE_FOREIGNKEYTABLE_H_
