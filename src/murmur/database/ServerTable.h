// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_SERVERTABLE_H_
#define MUMBLE_SERVER_DATABASE_SERVERTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <string>
#include <unordered_set>

namespace soci {
class session;
};

namespace mumble {
namespace server {
	namespace db {

		/**
		 * Table for storing general information about the existing virtual servers
		 */
		class ServerTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "virtual_servers";

			ServerTable(soci::session &sql, ::mumble::db::Backend backend);
			~ServerTable() = default;

			void addServer(unsigned int id);
			void removeServer(unsigned int id);
			bool serverExists(unsigned int id) const;

			std::unordered_set< std::string > migrate(unsigned int fromSchemeVersion,
													  unsigned int toSchemeVersion) override;
			void create() override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_SERVERTABLE_H_
