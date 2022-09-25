// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_BANTABLE_H_
#define MUMBLE_SERVER_DATABASE_BANTABLE_H_

#include "DBBan.h"

#include "database/Backend.h"
#include "database/Table.h"

#include <vector>

namespace soci {
class session;
};

namespace mumble {
namespace server {
	namespace db {

		class ServerTable;

		/**
		 * Table for storing banned users/addresses
		 */
		class BanTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "bans";

			struct column {
				column()                                   = delete;
				static constexpr const char *server_id     = "server_id";
				static constexpr const char *base_address  = "ipv6_base_address";
				static constexpr const char *prefix_length = "prefix_length";
				static constexpr const char *user_name     = "banned_user_name";
				static constexpr const char *cert_hash     = "banned_user_cert_hash";
				static constexpr const char *reason        = "reason";
				static constexpr const char *start_date    = "start_date";
				static constexpr const char *duration      = "duration";
			};


			BanTable(soci::session &sql, ::mumble::db::Backend backend, const ServerTable &serverTable);
			~BanTable() = default;

			void addBan(const DBBan &ban);

			void removeBan(const DBBan &ban);
			void removeBan(unsigned int serverID, const std::string &baseAddress, std::uint8_t prefixLength);

			bool banExists(const DBBan &ban);
			bool banExists(unsigned int serverID, const std::string &baseAddress, std::uint8_t prefixLength);

			DBBan getBanDetails(unsigned int serverID, const DBBan::ipv6_type &baseAddress, std::uint8_t prefixLength);
			DBBan getBanDetails(unsigned int serverID, const std::string &baseAddress, std::uint8_t prefixLength);

			std::vector< DBBan > getAllBans(unsigned int serverID);

			void clearBans(unsigned int serverID);

			void setBans(unsigned int serverID, const std::vector< DBBan > &bans);

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_BANTABLE_H_
