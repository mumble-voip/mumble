// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBGROUP_H_
#define MUMBLE_SERVER_DATABASE_DBGROUP_H_

#include <string>

namespace mumble {
namespace server {
	namespace db {

		struct DBGroup {
			unsigned int serverID  = {};
			unsigned int groupID   = {};
			unsigned int channelID = {};
			std::string name       = {};
			bool inherit           = true;
			bool is_inheritable    = true;

			DBGroup() = default;

			friend bool operator==(const DBGroup &lhs, const DBGroup &rhs);
			friend bool operator!=(const DBGroup &lhs, const DBGroup &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBGROUP_H_
