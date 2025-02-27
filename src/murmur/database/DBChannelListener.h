// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBCHANNELLISTENER_H_
#define MUMBLE_SERVER_DATABASE_DBCHANNELLISTENER_H_

namespace mumble {
namespace server {
	namespace db {

		/**
		 * A struct representing what the database needs to know / knows about a channel listener
		 */
		struct DBChannelListener {
			unsigned int serverID  = 0;
			unsigned int channelID = 0;
			unsigned int userID    = 0;
			float volumeAdjustment = 1;
			bool enabled           = true;

			DBChannelListener() = default;
			DBChannelListener(unsigned int serverID, unsigned int channelID, unsigned int userID);

			friend bool operator==(const DBChannelListener &lhs, const DBChannelListener &rhs);
			friend bool operator!=(const DBChannelListener &lhs, const DBChannelListener &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBCHANNELLISTENER_H_
