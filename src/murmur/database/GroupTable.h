// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_GROUPTABLE_H_
#define MUMBLE_SERVER_DATABASE_GROUPTABLE_H_

#include "DBGroup.h"

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
		 * Table for storing existing groups
		 */
		class GroupTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "groups";

			struct column {
				column()                                    = delete;
				static constexpr const char *server_id      = "server_id";
				static constexpr const char *group_id       = "group_id";
				static constexpr const char *group_name     = "group_name";
				static constexpr const char *channel_id     = "channel_id";
				static constexpr const char *inherit        = "inherit";
				static constexpr const char *is_inheritable = "is_inheritable";
			};


			GroupTable(soci::session &sql, ::mumble::db::Backend backend, const ChannelTable &channelTable);
			~GroupTable() = default;

			unsigned int getFreeGroupID(unsigned int serverID);

			unsigned int addGroup(const DBGroup &group);

			void updateGroup(const DBGroup &group);

			void removeGroup(const DBGroup &group);
			void removeGroup(unsigned int serverID, unsigned int groupID);

			bool groupExists(const DBGroup &group);
			bool groupExists(unsigned int serverID, unsigned int groupID);

			DBGroup getGroup(unsigned int serverID, unsigned int groupID);


			std::size_t countGroups(unsigned int serverID, unsigned int channelID);

			std::vector< DBGroup > getAllGroups(unsigned int serverID, unsigned int channelID);


			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_GROUPTABLE_H_
