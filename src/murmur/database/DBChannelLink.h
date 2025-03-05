// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBCHANNELLINK_H_
#define MUMBLE_SERVER_DATABASE_DBCHANNELLINK_H_

namespace mumble {
namespace server {
	namespace db {

		struct DBChannelLink {
			unsigned int serverID        = {};
			unsigned int firstChannelID  = {};
			unsigned int secondChannelID = {};

			DBChannelLink() = default;
			DBChannelLink(unsigned int serverID, unsigned int firstChannelID, unsigned int secondChannelID);

			friend bool operator==(const DBChannelLink &lhs, const DBChannelLink &rhs);
			friend bool operator!=(const DBChannelLink &lhs, const DBChannelLink &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBCHANNELLINK_H_
