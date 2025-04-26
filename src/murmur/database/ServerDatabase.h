// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_SERVERDATABASE_H_
#define MUMBLE_SERVER_DATABASE_SERVERDATABASE_H_

#include "database/Backend.h"
#include "database/Database.h"

namespace mumble {
namespace db {
	class MetaTable;
}
namespace server {
	namespace db {

		class ServerTable;
		class LogTable;
		class ConfigTable;
		class ChannelTable;
		class ChannelPropertyTable;
		class UserTable;
		class UserPropertyTable;
		class GroupTable;
		class GroupMemberTable;
		class ACLTable;
		class ChannelLinkTable;
		class BanTable;
		class ChannelListenerTable;

		class ServerDatabase : public ::mumble::db::Database {
		public:
			/**
			 * A version number keeping track of the scheme that is used for the database. Any change to the scheme
			 * has to be accompanied by increasing this number. A decrease is never allowed!
			 * Using a scheme version like this allows us to be able to create migration paths between scheme versions.
			 */
			static constexpr unsigned int DB_SCHEME_VERSION = 10;

			ServerDatabase(::mumble::db::Backend backend);
			~ServerDatabase() = default;

			unsigned int getSchemeVersion() const override;

			void migrateTables(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;

			::mumble::db::MetaTable &getMetaTable();
			ServerTable &getServerTable();
			LogTable &getLogTable();
			ConfigTable &getConfigTable();
			ChannelTable &getChannelTable();
			ChannelPropertyTable &getChannelPropertyTable();
			UserTable &getUserTable();
			UserPropertyTable &getUserPropertyTable();
			GroupTable &getGroupTable();
			GroupMemberTable &getGroupMemberTable();
			ACLTable &getACLTable();
			ChannelLinkTable &getChannelLinkTable();
			BanTable &getBanTable();
			ChannelListenerTable &getChannelListenerTable();

		protected:
			void setupStandardTables() override;
		};
	} // namespace db

} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_SERVERDATABASE_H_
