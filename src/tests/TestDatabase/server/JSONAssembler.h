// Copyright 2023 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TEST_TESTDATABASE_SERVER_JSONASSEMBLER_H_
#define MUMBLE_TEST_TESTDATABASE_SERVER_JSONASSEMBLER_H_

#include "murmur/database/ServerDatabase.h"

#include <vector>

#include <nlohmann/json.hpp>

namespace mumble {
namespace db {
	namespace test {

		class JSONAssembler {
		public:
			struct DataPair {
				nlohmann::json inputData;
				nlohmann::json migratedData;
			};

			JSONAssembler();

			DataPair buildTestData(unsigned int fromSchemeVersion,
								   const ::mumble::server::db::ServerDatabase &serverDB);

		private:
			std::vector< DataPair > m_rawData;
		};

	} // namespace test
} // namespace db
} // namespace mumble

#endif // MUMBLE_TEST_TESTDATABASE_SERVER_JSONASSEMBLER_H_
