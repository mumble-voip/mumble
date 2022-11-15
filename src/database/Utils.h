// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_UTILS_H_
#define MUMBLE_DATABASE_UTILS_H_

#include "Backend.h"
#include "NoDataException.h"

#include <soci/soci.h>

#include <string>
#include <vector>

namespace mumble {
namespace db {
	namespace utils {

		/**
		 * Tests the given SQL session to see if the last query resulted in any data having been fetched. If not,
		 * this function will throw an exception of the given type.
		 *
		 * @tparam Exception The type of the exception that is to be thrown
		 */
		template< typename Exception = NoDataException > void verifyQueryResultedInData(soci::session &sql) {
			if (!sql.got_data()) {
				throw Exception("Query did not result in any data having been fetched from the database: "
								+ sql.get_last_query());
			}
		}

		/**
		 * @returns A SQL statement that will raise an error that can be used inside a trigger to signal an error.
		 */
		std::string triggerErrorStatement(Backend backend, const std::string &errorMsg);

		struct ColAlias {
			std::string colName  = {};
			std::string colAlias = {};

			ColAlias(std::string colName);
			ColAlias(std::string colName, std::string colAlias);
		};

		std::string getLowestUnoccupiedIDStatement(Backend backend, const std::string &tableName,
												   const std::string &idColName,
												   const std::vector< ColAlias > &additionalCols = {});

		/**
		 * @returns The SQL statement for the respective backend to achieve limiting a query to a given
		 *  	size and optionally also starting at a specific offset.
		 */
		std::string limitOffset(Backend backend, const std::string &limit, const std::string &offset = "0");

	} // namespace utils
} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_UTILS_H_
