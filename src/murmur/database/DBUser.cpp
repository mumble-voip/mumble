// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBUser.h"

namespace mumble {
namespace server {
	namespace db {

		bool operator==(const DBUser &lhs, const DBUser &rhs) {
			return lhs.serverID == rhs.serverID && lhs.registeredUserID == rhs.registeredUserID;
		}

		bool operator!=(const DBUser &lhs, const DBUser &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
