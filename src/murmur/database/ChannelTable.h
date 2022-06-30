// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_CHANNELTABLE_H_
#define MUMBLE_SERVER_DATABASE_CHANNELTABLE_H_

#include "DBChannel.h"

#include "database/Backend.h"
#include "database/Table.h"

#include <string>

namespace soci {
class session;
};

namespace mumble {
namespace server {
	namespace db {

		class ServerTable;

		/**
		 * Table for storing the existing channels
		 */
		class ChannelTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME             = "channels";
			static constexpr unsigned int ROOT_CHANNEL_ID = 0;

			ChannelTable(soci::session &sql, ::mumble::db::Backend backend, const ServerTable &serverTable);
			~ChannelTable() = default;

			void addChannel(const DBChannel &channel);

			void removeChannel(const DBChannel &channel);
			void removeChannel(unsigned int serverID, unsigned int channelID);

			/**
			 * Updates the given channel. Note that the channel's serverID and channelID properties
			 * are used to identify the channel that is to be updated and these fields therefore can't
			 * be updated. They are immutable. If you want to change these, you'll have to remove the
			 * channel and add a new one instead.
			 */
			void updateChannel(const DBChannel &channel);

			bool channelExists(const DBChannel &channel);
			bool channelExists(unsigned int serverID, unsigned int channelID);

			DBChannel getChannelData(unsigned int serverID, unsigned int channelID);

			unsigned int getFreeChannelID(unsigned int serverID);
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_CHANNELTABLE_H_
