// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBChannelLink.h"

namespace mumble {
namespace server {
	namespace db {

		DBChannelLink::DBChannelLink(unsigned int serverID, unsigned int firstChannelID, unsigned int secondChannelID)
			: serverID(serverID), firstChannelID(firstChannelID), secondChannelID(secondChannelID) {}

		bool operator==(const DBChannelLink &lhs, const DBChannelLink &rhs) {
			return lhs.serverID == rhs.serverID
				   && ((lhs.firstChannelID == rhs.firstChannelID && lhs.secondChannelID == rhs.secondChannelID)
					   || (lhs.firstChannelID == rhs.secondChannelID && lhs.secondChannelID == rhs.firstChannelID));
		}

		bool operator!=(const DBChannelLink &lhs, const DBChannelLink &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
