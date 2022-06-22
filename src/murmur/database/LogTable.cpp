// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LogTable.h"
#include "ServerTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
#include "database/Index.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		namespace column {
			constexpr const char *server_id = "server_id";
			constexpr const char *message   = "message";
			constexpr const char *date      = "message_date";
		} // namespace column

		constexpr const char *LogTable::NAME;

		LogTable::LogTable(soci::session &sql, ::mdb::Backend backend, const ServerTable &serverTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column msgCol(column::message, ::mdb::DataType(::mdb::DataType::Text));
			msgCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column msgTimeCol(column::date, ::mdb::DataType(::mdb::DataType::EpochTime));
			msgTimeCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));


			setColumns({ serverCol, msgCol, msgTimeCol });


			::mdb::ForeignKey fk(serverTable, { serverCol });
			addForeignKey(fk);


			::mdb::Index dateIndex(std::string(NAME) + "_" + column::date + "_index", { column::date });
			addIndex(dateIndex, false);
		}

		void LogTable::logMessage(unsigned int serverID, const std::string &message,
								  const std::chrono::time_point< std::chrono::steady_clock > &date) {
			std::size_t timeSinceEpoch =
				std::chrono::duration_cast< std::chrono::seconds >(date.time_since_epoch()).count();

			try {
				soci::transaction transaction(m_sql);

				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::message << ", "
					  << column::date << ") VALUES (:id, :msg, :date)",
					soci::use(serverID), soci::use(message), soci::use(timeSinceEpoch);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at logging message for server with ID "
															  + std::to_string(serverID) + ": \"" + message + "\""));
			}
		}


		void LogTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 9) {
					// In v9 we renamed this table from "slog" to "server_logs"
					// -> Import all data from the old table into the new one
					// Note that we also changed the type of the date column from DATE/TIMESTAMP to seconds since
					// epoch -> this requires conversion

					switch (m_backend) {
						case ::mdb::Backend::SQLite:
							m_sql << "INSERT INTO \"server_logs\" (server_id, message, message_date) "
								  << "SELECT server_id, msg, strftime('%s', msgtime) FROM \"slog"
								  << mdb::Database::OLD_TABLE_SUFFIX << "\"";
							break;
						case ::mdb::Backend::MySQL:
							m_sql << "INSERT INTO \"server_logs\" (server_id, message, message_date) "
								  << "SELECT server_id, msg, UNIX_TIMESTAMP(msgtime) FROM \"slog"
								  << mdb::Database::OLD_TABLE_SUFFIX << "\"";
							break;
						case ::mdb::Backend::PostgreSQL:
							m_sql << "INSERT INTO \"server_logs\" (server_id, message, message_date) "
								  << "SELECT server_id, msg, EXTRACT(EPOCH FROM msgtime) FROM \"slog"
								  << mdb::Database::OLD_TABLE_SUFFIX << "\"";
							break;
					}
				} else {
					// Use default implementation to handle migration without change of format
					mdb::Table::migrate(fromSchemeVersion, toSchemeVersion);
				}
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					std::string("Failed at migrating table \"") + NAME + "\" from scheme version "
					+ std::to_string(fromSchemeVersion) + " to " + std::to_string(toSchemeVersion)));
			}
		}

	} // namespace db
} // namespace server
} // namespace mumble
