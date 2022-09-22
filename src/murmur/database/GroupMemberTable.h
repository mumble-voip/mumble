// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_GROUPMEMBERTABLE_H_
#define MUMBLE_SERVER_DATABASE_GROUPMEMBERTABLE_H_

#include "DBGroupMember.h"

#include "database/Backend.h"
#include "database/Table.h"

#include <vector>

namespace soci {
class session;
};

namespace mumble {
namespace server {
	namespace db {

		class GroupTable;
		class UserTable;

		/**
		 * Table for storing group members
		 */
		class GroupMemberTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "group_members";

			struct column {
				column()                                  = delete;
				static constexpr const char *server_id    = "server_id";
				static constexpr const char *group_id     = "group_id";
				static constexpr const char *user_id      = "user_id";
				static constexpr const char *add_to_group = "add_to_group";
			};


			GroupMemberTable(soci::session &sql, ::mumble::db::Backend backend, const GroupTable &groupTable,
							 const UserTable &userTable);
			~GroupMemberTable() = default;

			void addEntry(const DBGroupMember &member);
			void addEntry(unsigned int serverID, unsigned int groupID, unsigned int userID, bool addToGroup);

			void removeEntry(const DBGroupMember &member);
			void removeEntry(unsigned int serverID, unsigned int groupID, unsigned int userID);

			bool entryExists(const DBGroupMember &member);
			bool entryExists(unsigned int serverID, unsigned int groupID, unsigned int userID);

			std::vector< DBGroupMember > getEntries(unsigned int serverID, unsigned int groupID);

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_GROUPMEMBERTABLE_H_
