// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBGroup.h"


namespace mumble {
namespace server {
	namespace db {

		bool operator==(const DBGroup &lhs, const DBGroup &rhs) {
			return lhs.serverID == rhs.serverID && lhs.groupID == rhs.groupID && lhs.channelID == rhs.channelID
				   && lhs.name == rhs.name && lhs.inherit == rhs.inherit && lhs.is_inheritable == rhs.is_inheritable;
		}

		bool operator!=(const DBGroup &lhs, const DBGroup &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
