// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TestUtils.h"
#include "database/MySQLConnectionParameter.h"
#include "database/PostgreSQLConnectionParameter.h"
#include "database/SQLiteConnectionParameter.h"

#include <nlohmann/json.hpp>

namespace mumble {
namespace db {
	namespace test {
		namespace utils {

			/**
			 * Aligns the column order of the two provided JSON representation of a DB's content, so that
			 * the columns appear in the same order (if they were only a permutation of each other to begin with).
			 * All related fields (column_types and all row entries) are permuted accordingly to match the new
			 * column order.
			 *
			 * This function has to be called, before checking reference and target for equality, as the column
			 * order in our DB JSON representation is to some degree arbitrary, but the JSON objects compare
			 * them in an order-aware fashion (as they are stored as arrays).
			 */
			void alignColumnOrder(const nlohmann::json &reference, nlohmann::json &target) {
				for (auto it = reference["tables"].begin(); it != reference["tables"].end(); ++it) {
					const nlohmann::json &referenceTable = it.value();
					nlohmann::json &targetTable          = target["tables"][it.key()];

					if (referenceTable["column_names"].size() != targetTable["column_names"].size()
						|| !std::is_permutation(referenceTable["column_names"].begin(),
												referenceTable["column_names"].end(),
												targetTable["column_names"].begin())) {
						// If the column names are not a simple permutation of each other, these tables are not equal
						// in any case, so we don't even try to align their ordering
						continue;
					}

					std::vector< std::size_t > permutation =
						test::utils::find_permutation(referenceTable["column_names"], targetTable["column_names"]);

					// First apply to column_names itself
					test::utils::apply_permutation(targetTable["column_names"], permutation);
					// Then in the same way to column_types
					test::utils::apply_permutation(targetTable["column_types"], permutation);

					for (nlohmann::json &currentRow : targetTable["rows"]) {
						test::utils::apply_permutation(currentRow, permutation);
					}
				}
			}


			const mumble::db::ConnectionParameter &getConnectionParamter(mumble::db::Backend backend) {
				switch (backend) {
					case mumble::db::Backend::SQLite: {
						static mumble::db::SQLiteConnectionParameter param("mumble_test_db");

						return param;
					}
					case mumble::db::Backend::MySQL: {
						static mumble::db::MySQLConnectionParameter param("mumble_test_db");

						param.userName = "mumble_test_user";
						param.password = "MumbleTestPassword";

						return param;
					}
					case mumble::db::Backend::PostgreSQL: {
						static mumble::db::PostgreSQLConnectionParameter param("mumble_test_db");

						param.userName = "mumble_test_user";
						param.password = "MumbleTestPassword";

						return param;
					}
				}

				throw std::runtime_error("Not all backend types covered in test!");
			}

		} // namespace utils
	}     // namespace test
} // namespace db
} // namespace mumble
