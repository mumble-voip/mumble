// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBGroupMember.h"

namespace mumble {
namespace server {
	namespace db {

		DBGroupMember::DBGroupMember(unsigned int serverID, unsigned int groupID, unsigned int userID, bool addToGroup)
			: serverID(serverID), groupID(groupID), userID(userID), addToGroup(addToGroup) {}

		bool operator==(const DBGroupMember &lhs, const DBGroupMember &rhs) {
			return lhs.serverID == rhs.serverID && lhs.groupID == rhs.groupID && lhs.userID == rhs.userID
				   && lhs.addToGroup == rhs.addToGroup;
		}

		bool operator!=(const DBGroupMember &lhs, const DBGroupMember &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
