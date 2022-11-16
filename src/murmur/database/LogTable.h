// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DATABASE_LOGTABLE_H_
#define MUMBLE_SERVER_DATABASE_LOGTABLE_H_

#include "database/Backend.h"
#include "database/Table.h"

#include "DBLogEntry.h"

#include <limits>

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

			struct column {
				column()                               = delete;
				static constexpr const char *server_id = "server_id";
				static constexpr const char *message   = "message";
				static constexpr const char *date      = "message_date";
			};


			LogTable(soci::session &sql, ::mumble::db::Backend backend, const ServerTable &serverTable);
			~LogTable() = default;

			void logMessage(unsigned int serverID, const DBLogEntry &entry);

			void clearLog(unsigned int serverID);

			std::vector< DBLogEntry > getLogs(unsigned int serverID,
											  unsigned int maxEntries  = std::numeric_limits< int >::max(),
											  unsigned int startOffset = 0);

			std::size_t getLogSize(unsigned int serverID);

			void migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) override;
		};

	} // namespace db
} // namespace server
} // namespace mumble

#endif // MUMBLE_SERVER_DATABASE_LOGTABLE_H_
