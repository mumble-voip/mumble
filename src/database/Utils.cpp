// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Utils.h"

#include <cassert>

namespace mumble {
namespace db {
	namespace utils {

		std::string triggerErrorStatement(Backend backend, const std::string &errorMsg) {
			switch (backend) {
				case Backend::SQLite:
					return "RAISE(ROLLBACK, '" + errorMsg + "')";
				case Backend::MySQL:
					return "SIGNAL SQLSTATE '45000' SET MESSAGE TEXT = '" + errorMsg + "'";
				case Backend::PostgreSQL:
					return "RAISE EXCEPTION '" + errorMsg + "'";
			}

			// Should be unreachable
			assert(false);
			return {};
		}

	} // namespace utils
} // namespace db
} // namespace mumble
