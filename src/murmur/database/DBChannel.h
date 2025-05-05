// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_DBCHANNEL_H_
#define MUMBLE_SERVER_DATABASE_DBCHANNEL_H_

#include <string>

namespace mumble {
namespace server {
	namespace db {

		/**
		 * A small struct representing what the database needs to know / knows about a channel
		 */
		struct DBChannel {
			unsigned int serverID  = 0;
			unsigned int channelID = 0;
			/**
			 * The ID of the channel's parent channel. If this channel has no parent, this is supposed
			 * to be set to the channel's own ID.
			 **/
			unsigned int parentID = 0;
			std::string name      = {};
			bool inheritACL       = false;

			friend bool operator==(const DBChannel &lhs, const DBChannel &rhs);
			friend bool operator!=(const DBChannel &lhs, const DBChannel &rhs);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_DBCHANNEL_H_
