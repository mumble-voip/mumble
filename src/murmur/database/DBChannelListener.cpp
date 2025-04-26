// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBChannelListener.h"

#include <algorithm>
#include <cstdlib>
#include <limits>

namespace mumble {
namespace server {
	namespace db {

		DBChannelListener::DBChannelListener(unsigned int serverID, unsigned int channelID, unsigned int userID)
			: serverID(serverID), channelID(channelID), userID(userID) {}

		bool floatEquals(float a, float b) {
			float diff = std::abs(a - b);

			return diff <= std::max(std::numeric_limits< float >::round_error(),
									2 * std::numeric_limits< float >::epsilon());
		}

		bool operator==(const DBChannelListener &lhs, const DBChannelListener &rhs) {
			return lhs.serverID == rhs.serverID && lhs.channelID == rhs.channelID && lhs.userID == rhs.userID
				   && floatEquals(lhs.volumeAdjustment, rhs.volumeAdjustment) && lhs.enabled == rhs.enabled;
		}

	} // namespace db
} // namespace server
} // namespace mumble
