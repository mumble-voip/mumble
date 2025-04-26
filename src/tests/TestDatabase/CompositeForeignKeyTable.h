// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_COMPOSITEFOREIGNKEYTABLE_H_
#define MUMBLE_TEST_TESTDATABASE_COMPOSITEFOREIGNKEYTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
}

namespace mumble {
namespace db {
	namespace test {

		class CompositeForeignKeyTable : public Table {
		public:
			static constexpr const char *NAME = "compositeForeignKeyTable";

			CompositeForeignKeyTable(soci::session &sql, Backend backend, const Table &foreignTable);
			~CompositeForeignKeyTable() = default;

			void insert(const std::string &keyA, const std::string &keyB, const std::string &value);
			void insertNullA();
			void insertNullB();
			std::string select(const std::string &keyA, const std::string &keyB);
			void dropKey(const std::string &keyA, const std::string &keyB);
		};

	} // namespace test
} // namespace db
} // namespace mumble

#endif // MUMBLE_TEST_TESTDATABASE_COMPOSITEFOREIGNKEYTABLE_H_
