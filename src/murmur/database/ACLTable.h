// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_ACLTABLE_H_
#define MUMBLE_SERVER_DATABASE_ACLTABLE_H_

#include "DBAcl.h"

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
		class GroupTable;

		/**
		 * Table for storing Access Control Lists (ACLs)
		 */
		class ACLTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "access_control_lists";

			struct column {
				column()                                      = delete;
				static constexpr const char *server_id        = "server_id";
				static constexpr const char *channel_id       = "channel_id";
				static constexpr const char *priority         = "priority";
				static constexpr const char *aff_user_id      = "affected_user_id";
				static constexpr const char *aff_group_id     = "affected_group_id";
				static constexpr const char *apply_in_current = "apply_in_current_channel";
				static constexpr const char *apply_in_sub     = "apply_in_sub_channels";
				static constexpr const char *granted_flags    = "granted_privilege_flags";
				static constexpr const char *revoked_flags    = "revoked_privilege_flags";
			};


			ACLTable(soci::session &sql, ::mumble::db::Backend backend, const ChannelTable &channelTable,
					 const UserTable &userTable, const GroupTable &groupTable);
			~ACLTable() = default;

			void addACL(const DBAcl &acl);

			void removeACL(const DBAcl &acl);
			void removeACL(unsigned int serverID, unsigned int channelID, unsigned int priority);

			bool aclExists(const DBAcl &acl);
			bool aclExists(unsigned int serverID, unsigned int channelID, unsigned int priority);

			void clearACLs(unsigned int serverID, unsigned int channelID);

			std::vector< DBAcl > getAllACLs(unsigned int serverID, unsigned int channelID);

			std::size_t countOverallACLs(unsigned int serverID);

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_ACLTABLE_H_
