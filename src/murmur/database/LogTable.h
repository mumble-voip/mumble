// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_LOGTABLE_H_
#define MUMBLE_SERVER_DATABASE_LOGTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include <chrono>

namespace soci {
class session;
};

namespace mumble {
namespace server {
	namespace db {

		class ServerTable;

		/**
		 * Table for storing server logs
		 */
		class LogTable : public ::mumble::db::Table {
		public:
			static constexpr const char *NAME = "server_logs";

			LogTable(soci::session &sql, ::mumble::db::Backend backend, const ServerTable &serverTable);
			~LogTable() = default;

			void logMessage(
				unsigned int serverID, const std::string &message,
				const std::chrono::time_point< std::chrono::steady_clock > &date = std::chrono::steady_clock::now());

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_LOGTABLE_H_
