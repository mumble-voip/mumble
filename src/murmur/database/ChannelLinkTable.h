// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_CHANNELLINK_H_
#define MUMBLE_SERVER_DATABASE_CHANNELLINK_H_

#include "DBChannelLink.h"

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

		/**
		 * Table for storing channel links (aka: linked channels)
		 */
		class ChannelLinkTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "channel_links";

			struct column {
				column()                               = delete;
				static constexpr const char *server_id = "server_id";
				static constexpr const char *first_id  = "first_channel_id";
				static constexpr const char *second_id = "second_channel_id";
			};

			ChannelLinkTable(soci::session &sql, ::mumble::db::Backend backend, const ChannelTable &channelTable);
			~ChannelLinkTable() = default;

			void addLink(const DBChannelLink &link);

			void removeLink(const DBChannelLink &link);

			bool linkExists(const DBChannelLink &link);

			std::vector< DBChannelLink > getAllLinks(unsigned int serverID);

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_CHANNELLINK_H_
