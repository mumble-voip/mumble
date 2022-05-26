// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_CONTRAINTTABLE_H_
#define MUMBLE_TEST_TESTDATABASE_CONTRAINTTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
}

namespace mumble {
namespace db {
	namespace test {

		class ConstraintTable : public Table {
		public:
			static constexpr const char *NAME = "ConstraintTable";

			ConstraintTable(soci::session &sql, Backend backend);
			~ConstraintTable() = default;

#define DEFINE_TABLE(name)                                               \
	void insert##name(const std::string &key, const std::string &value); \
	std::string select##name(const std::string &key);

			DEFINE_TABLE(NotNull);
			DEFINE_TABLE(Unique);
#undef DEFINE_TABLE
			void insertNullInNotNullCol();

		protected:
			unsigned int dummyCounter = 0;
		};

	} // namespace test
} // namespace db
} // namespace mumble

#endif // MUMBLE_TEST_TESTDATABASE_CONTRAINTTABLE_H_
