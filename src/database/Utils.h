// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_UTILS_H_
#define MUMBLE_DATABASE_UTILS_H_

#include "NoDataException.h"

#include <soci/soci.h>

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
	} // namespace utils
} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_UTILS_H_
