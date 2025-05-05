// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_INTEGERTABLE_H_
#define MUMBLE_TEST_TESTDATABASE_INTEGERTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
}

namespace mumble {
namespace db {
	namespace test {

		class IntegerTable : public Table {
		public:
			static constexpr const char *NAME = "IntegerTable";

			struct column {
				column()                                = delete;
				static constexpr const char *int_col    = "int_col";
				static constexpr const char *second_col = "second_col";
			};

			static constexpr int lowestID_unconstrained = 7;
			static constexpr int lowestID_second_empty  = 0;
			static constexpr int lowestID_second_bla    = 3;

			IntegerTable(soci::session &sql, Backend backend);
			~IntegerTable() = default;

			void init();

			void insert(int value, const std::string &secondVal = {});

			int selectMinFreeID(const std::string &sqlStatement, const std::string &secondColAlias,
								const std::string *secondVal);
		};
	} // namespace test
} // namespace db
} // namespace mumble

#endif // MUMBLE_TEST_TESTDATABASE_INTEGERTABLE_H_
