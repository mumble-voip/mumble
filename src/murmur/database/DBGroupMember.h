// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBGROUPMEMBER_H_
#define MUMBLE_SERVER_DATABASE_DBGROUPMEMBER_H_

#include <string>

namespace mumble {
namespace server {
	namespace db {

		struct DBGroupMember {
			unsigned int serverID = {};
			unsigned int groupID  = {};
			unsigned int userID   = {};
			bool addToGroup       = {};

			DBGroupMember() = default;
			DBGroupMember(unsigned int serverID, unsigned int groupID, unsigned int userID, bool addToGroup = {});

			friend bool operator==(const DBGroupMember &lhs, const DBGroupMember &rhs);
			friend bool operator!=(const DBGroupMember &lhs, const DBGroupMember &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBGROUPMEMBER_H_
