// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBAcl.h"

namespace mumble {
namespace server {
	namespace db {

		bool operator==(const DBAcl &lhs, const DBAcl &rhs) {
			return lhs.serverID == rhs.serverID && lhs.channelID == rhs.channelID && lhs.priority == rhs.priority
				   && lhs.affectedUserID == rhs.affectedUserID && lhs.affectedGroupID == rhs.affectedGroupID
				   && lhs.applyInCurrentChannel == rhs.applyInCurrentChannel
				   && lhs.applyInSubChannels == rhs.applyInSubChannels
				   && lhs.grantedPrivilegeFlags == rhs.grantedPrivilegeFlags
				   && lhs.revokedPrivilegeFlags == rhs.revokedPrivilegeFlags;
		}

		bool operator!=(const DBAcl &lhs, const DBAcl &rhs) { return !(lhs == rhs); }

	} // namespace db
} // namespace server
} // namespace mumble
