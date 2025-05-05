// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_DEFAULTTABLE_H_
#define MUMBLE_TEST_TESTDATABASE_DEFAULTTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
}

namespace mumble {
namespace db {
	namespace test {

		class DefaultTable : public Table {
		public:
			static constexpr const char *NAME = "DefaultTable";
			// Use a default that is using spaces and different quotation marks (to make it more difficult)
			static constexpr const char *DEFAULT = "My sister's uncle is called \"Sam\"";

			DefaultTable(soci::session &sql, Backend backend);
			~DefaultTable() = default;

			void insert(const std::string &key);
			std::string select(const std::string &key);
		};
	} // namespace test
} // namespace db
} // namespace mumble

#endif // MUMBLE_TEST_TESTDATABASE_DEFAULTTABLE_H_
