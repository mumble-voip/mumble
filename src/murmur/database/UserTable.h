// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_USERTABLE_H_
#define MUMBLE_SERVER_DATABASE_USERTABLE_H_

#include "DBUser.h"
#include "DBUserData.h"

#include "database/Backend.h"
#include "database/Table.h"

#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include <gsl/span>

#include <boost/optional.hpp>

namespace soci {
class session;
};

class QByteArray;

namespace mumble {
namespace server {
	namespace db {

		class ServerTable;
		class ChannelTable;

		/**
		 * Table for storing the existing channels
		 */
		class UserTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "users";

			struct column {
				column()                                     = delete;
				static constexpr const char *server_id       = "server_id";
				static constexpr const char *user_id         = "user_id";
				static constexpr const char *user_name       = "user_name";
				static constexpr const char *password_hash   = "password_hash";
				static constexpr const char *salt            = "salt";
				static constexpr const char *kdf_iterations  = "kdf_iterations";
				static constexpr const char *last_channel_id = "last_channel_id";
				static constexpr const char *texture         = "texture";
				static constexpr const char *last_active     = "last_active";
				static constexpr const char *last_disconnect = "last_disconnect";
			};


			UserTable(soci::session &sql, ::mumble::db::Backend backend, const ServerTable &serverTable,
					  ChannelTable &channelTable);
			~UserTable() = default;


			void addUser(const DBUser &user, const std::string &name);

			void removeUser(const DBUser &user);

			bool userExists(const DBUser &user);

			void updateData(const DBUser &user, const DBUserData &data);
			DBUserData getData(const DBUser &user);

			void clearLastDisconnect(const DBUser &user);
			void setLastDisconnect(const DBUser &user, const std::chrono::steady_clock::time_point &timepoint =
														   std::chrono::steady_clock::now());
			std::chrono::steady_clock::time_point getLastDisconnect(const DBUser &user);

			void clearLastActive(const DBUser &user);
			void setLastActive(const DBUser &user, const std::chrono::steady_clock::time_point &timepoint =
													   std::chrono::steady_clock::now());
			std::chrono::steady_clock::time_point getLastActive(const DBUser &user);

			void clearLastChannelID(const DBUser &user);
			void setLastChannelID(const DBUser &user, unsigned int channelID);
			unsigned int getLastChannelID(const DBUser &user);

			void clearTexture(const DBUser &user);
			void setTexture(const DBUser &user, gsl::span< std::uint8_t > texture);
			std::vector< std::uint8_t > getTexture(const DBUser &user);

			void clearPassword(const DBUser &user);
			void setPassword(const DBUser &user, const DBUserData::PasswordData &password);
			DBUserData::PasswordData getPassword(const DBUser &user);

			void setName(const DBUser &user, const std::string &name);
			std::string getName(const DBUser &user);



			boost::optional< unsigned int > findUser(unsigned int serverID, const std::string &name,
													 bool caseSensitive = false);


			std::vector< DBUser > getRegisteredUsers(unsigned int serverID);


			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_USERTABLE_H_
