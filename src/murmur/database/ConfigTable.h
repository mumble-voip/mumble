// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_CONFIGTABLE_H_
#define MUMBLE_SERVER_DATABASE_CONFIGTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>
#include <unordered_map>

namespace soci {
class session;
};

namespace mumble {
namespace server {
	namespace db {

		class ServerTable;

		/**
		 * Table for storing server config values.
		 */
		class ConfigTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "config";

			struct column {
				column()                               = delete;
				static constexpr const char *server_id = "server_id";
				static constexpr const char *key       = "config_name";
				static constexpr const char *value     = "config_value";
			};


			ConfigTable(soci::session &sql, ::mumble::db::Backend backend, const ServerTable &serverTable);
			~ConfigTable() = default;

			std::string getConfig(unsigned int serverID, const std::string &configName,
								  const std::string &defaultValue = {});
			void setConfig(unsigned int serverID, const std::string &configName, const std::string &value);
			void clearConfig(unsigned int serverID, const std::string &configName);

			std::unordered_map< std::string, std::string > getAllConfigs(unsigned int serverID);
			void clearAllConfigs(unsigned int serverID);


			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_CONFIGTABLE_H_
