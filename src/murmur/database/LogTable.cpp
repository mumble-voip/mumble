// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "LogTable.h"
#include "ChronoUtils.h"
#include "ServerTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
#include "database/Index.h"
#include "database/MigrationException.h"
#include "database/TransactionHolder.h"
#include "database/Utils.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		constexpr const char *LogTable::NAME;
		constexpr const char *LogTable::column::server_id;
		constexpr const char *LogTable::column::message;
		constexpr const char *LogTable::column::date;


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

		void LogTable::logMessage(unsigned int serverID, const DBLogEntry &entry) {
			std::size_t timeSinceEpoch = toEpochSeconds(entry.timestamp);

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::message << ", "
					  << column::date << ") VALUES (:id, :msg, :date)",
					soci::use(serverID), soci::use(entry.message), soci::use(timeSinceEpoch);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at logging message for server with ID "
															  + std::to_string(serverID) + ": \"" + entry.message
															  + "\""));
			}
		}

		void LogTable::clearLog(unsigned int serverID) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID",
					soci::use(serverID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at clearing logs for server with ID " + std::to_string(serverID)));
			}
		}

		std::vector< DBLogEntry > LogTable::getLogs(unsigned int serverID, unsigned int maxEntries,
													unsigned int startOffset) {
			// We can't pass unsigned values to SOCI and therefore, we have to ensure that we don't exceed a signed
			// int's max value (as that'd end up being interpreted as a negative number)
			assert(maxEntries <= std::numeric_limits< int >::max());
			assert(startOffset <= std::numeric_limits< int >::max());

			try {
				std::vector< DBLogEntry > entries;
				soci::row row;

				::mdb::TransactionHolder transaction = ensureTransaction();

				soci::statement stmt =
					(m_sql.prepare << "SELECT " << column::date << ", " << column::message << " FROM \"" << NAME
								   << "\" WHERE " << column::server_id << " = :serverID ORDER BY " << column::date
								   << " DESC " << ::mdb::utils::limitOffset(m_backend, ":limit", ":offset"),
					 soci::use(serverID), soci::use(maxEntries), soci::use(startOffset), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 2);
					assert(row.get_properties(0).get_data_type() == soci::dt_long_long);
					assert(row.get_properties(1).get_data_type() == soci::dt_string);

					DBLogEntry entry;
					entry.message = row.get< std::string >(1);
					entry.timestamp =
						std::chrono::steady_clock::time_point(std::chrono::seconds(row.get< long long >(0)));

					entries.push_back(std::move(entry));
				}

				transaction.commit();

				return entries;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at getting logs for server with ID " + std::to_string(serverID)));
			}
		}


		void LogTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 10) {
					// In v10 we renamed this table from "slog" to "server_logs"
					// -> Import all data from the old table into the new one
					// Note that we also changed the type of the date column from DATE/TIMESTAMP to seconds since
					// epoch -> this requires conversion

					switch (m_backend) {
						case ::mdb::Backend::SQLite:
							m_sql << "INSERT INTO \"" << getName() << "\" (" << column::server_id << ", "
								  << column::message << ", " << column::date << ") "
								  << "SELECT server_id, msg, strftime('%s', msgtime) FROM \"slog"
								  << mdb::Database::OLD_TABLE_SUFFIX << "\"";
							break;
						case ::mdb::Backend::MySQL:
							m_sql << "INSERT INTO \"" << getName() << "\" (" << column::server_id << ", "
								  << column::message << ", " << column::date << ") "
								  << "SELECT server_id, msg, UNIX_TIMESTAMP(msgtime) FROM \"slog"
								  << mdb::Database::OLD_TABLE_SUFFIX << "\"";
							break;
						case ::mdb::Backend::PostgreSQL:
							m_sql << "INSERT INTO \"" << getName() << "\" (" << column::server_id << ", "
								  << column::message << ", " << column::date << ") "
								  << "SELECT server_id, msg, EXTRACT(EPOCH FROM msgtime) FROM \"slog"
								  << mdb::Database::OLD_TABLE_SUFFIX << "\"";
							break;
					}
				} else {
					// Use default implementation to handle migration without change of format
					mdb::Table::migrate(fromSchemeVersion, toSchemeVersion);
				}
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::MigrationException(
					std::string("Failed at migrating table \"") + NAME + "\" from scheme version "
					+ std::to_string(fromSchemeVersion) + " to " + std::to_string(toSchemeVersion)));
			}
		}

	} // namespace db
} // namespace server
} // namespace mumble
