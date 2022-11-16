// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_CHANNELLISTENERTABLE_H_
#define MUMBLE_SERVER_DATABASE_CHANNELLISTENERTABLE_H_

#include "DBChannelListener.h"

#include "database/Backend.h"
#include "database/Table.h"

#include <vector>

namespace soci {
class session;
};

namespace mumble {
namespace server {
	namespace db {

		class ChannelTable;
		class UserTable;

		/**
		 * Table for storing channel listeners
		 */
		class ChannelListenerTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "channel_listeners";

			struct column {
				column()                                       = delete;
				static constexpr const char *server_id         = "server_id";
				static constexpr const char *user_id           = "user_id";
				static constexpr const char *channel_id        = "channel_id";
				static constexpr const char *volume_adjustment = "volume_adjustment";
				static constexpr const char *enabled           = "enabled";
			};

			ChannelListenerTable(soci::session &sql, ::mumble::db::Backend backend, const UserTable &userTable,
								 const ChannelTable &channelTable);
			~ChannelListenerTable() = default;

			void addListener(const DBChannelListener &listener);

			void removeListener(const DBChannelListener &listener);
			void removeListener(unsigned int serverID, unsigned int userID, unsigned int channelID);

			bool listenerExists(const DBChannelListener &listener);
			bool listenerExists(unsigned int serverID, unsigned int userID, unsigned int channelID);

			DBChannelListener getListenerDetails(const DBChannelListener &listener);
			DBChannelListener getListenerDetails(unsigned int serverID, unsigned int userID, unsigned int channelID);

			void updateListener(const DBChannelListener &listener);

			std::vector< DBChannelListener > getListenersForUser(unsigned int serverID, unsigned int userID);

			std::vector< DBChannelListener > getListenersForChannel(unsigned int serverID, unsigned int channelID);

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_CHANNELLISTENERTABLE_H_
