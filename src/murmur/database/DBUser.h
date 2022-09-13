// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBUSER_H_
#define MUMBLE_SERVER_DATABASE_DBUSER_H_

namespace mumble {
namespace server {
	namespace db {

		struct DBUser {
			unsigned int serverID         = {};
			unsigned int registeredUserID = {};

			DBUser() = default;
			DBUser(unsigned int serverID, unsigned int userID) : serverID(serverID), registeredUserID(userID) {}

			friend bool operator==(const DBUser &lhs, const DBUser &rhs);
			friend bool operator!=(const DBUser &lhs, const DBUser &rhs);
		};


	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBUSER_H_
