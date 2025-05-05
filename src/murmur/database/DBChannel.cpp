// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBChannel.h"

namespace mumble {
namespace server {
	namespace db {

		bool operator==(const DBChannel &lhs, const DBChannel &rhs) {
			return lhs.serverID == rhs.serverID && lhs.channelID == rhs.channelID && lhs.parentID == rhs.parentID
				   && lhs.inheritACL == rhs.inheritACL && lhs.name == rhs.name;
		}

		bool operator!=(const DBChannel &lhs, const DBChannel &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
