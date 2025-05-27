// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerTable.h"
#include "ACLTable.h"
#include "BanTable.h"
#include "ChannelLinkTable.h"
#include "ChannelListenerTable.h"
#include "ChannelPropertyTable.h"
#include "ChannelTable.h"
#include "ConfigTable.h"
#include "GroupMemberTable.h"
#include "GroupTable.h"
#include "LogTable.h"
#include "UserPropertyTable.h"
#include "UserTable.h"

#include "database/AccessException.h"
#include "database/Backend.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/MigrationException.h"
#include "database/PrimaryKey.h"
#include "database/TransactionHolder.h"
#include "database/Utils.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		constexpr const char *ServerTable::NAME;
		constexpr const char *ServerTable::column::server_id;


		ServerTable::ServerTable(soci::session &sql, ::mdb::Backend backend) : ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));

			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			setColumns({ serverCol });

			::mdb::PrimaryKey pk(serverCol);
			setPrimaryKey(pk);
		}

		void ServerTable::addServer(unsigned int id) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "INSERT INTO \"" << NAME << "\" (\"" << column::server_id << "\") VALUES (:id)", soci::use(id);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at adding server with ID " + std::to_string(id)));
			}
		}

		void ServerTable::removeServer(unsigned int id) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE \"" << column::server_id << "\" = :id", soci::use(id);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at removing server with ID " + std::to_string(id)));
			}
		}

		bool ServerTable::serverExists(unsigned int id) {
			try {
				int exists = 0;

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE \"" << column::server_id << "\" = :id LIMIT 1",
					soci::use(id), soci::into(exists);

				transaction.commit();

				assert(exists == 0 || exists == 1);

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed checking whether server with ID " + std::to_string(id) + " exists"));
			}
		}

		unsigned int ServerTable::getFreeServerID() {
			try {
				int freeID = 0;

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << ::mdb::utils::getLowestUnoccupiedIDStatement(m_backend, NAME, column::server_id),
					soci::into(freeID);

				transaction.commit();

				return static_cast< unsigned int >(freeID);
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at fetching next free server ID"));
			}
		}

		std::vector< unsigned int > ServerTable::getAllServerIDs() {
			try {
				std::vector< unsigned int > ids;
				soci::row row;

				::mdb::TransactionHolder transaction = ensureTransaction();

				soci::statement stmt =
					(m_sql.prepare << "SELECT \"" << column::server_id << "\" FROM \"" << NAME << "\"",
					 soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 1);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);

					ids.push_back(static_cast< unsigned int >(row.get< int >(0)));
				}

				transaction.commit();

				return ids;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at fetching all server IDs"));
			}
		}

		void ServerTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code old table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion <= toSchemeVersion);

			try {
				if (fromSchemeVersion < 10) {
					// In v10 we renamed this table from "servers" to "virtual_servers"
					// -> Import all data from the old table into the new one
					m_sql << "INSERT INTO \"" << getName() << "\" (\"" << column::server_id
						  << "\") SELECT \"server_id\" FROM \"servers" << mdb::Database::OLD_TABLE_SUFFIX << "\"";
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

		void ServerTable::postMigrationAction(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			assert(fromSchemeVersion < toSchemeVersion);
			(void) toSchemeVersion;

#define PROCESS_TABLE(Table)                                                                  \
	m_sql << "UPDATE \"" << Table::NAME << "\" SET \"" << Table::column::server_id << "\"=\"" \
		  << Table::column::server_id << "\" - 1"

			if (fromSchemeVersion < 10) {
				// Since v10 server IDs are expected to start at zero. If this is not yet the case (almost certainly),
				// shift all server IDs down by one.
				// Since the server ID has influence on the (default) port it listens to, matching this expectation will
				// ensure servers continuing to listen on the same port as before the migration.
				int minID = -1;
				m_sql << "SELECT MIN(\"" << column::server_id << "\") FROM \"" << getName() << "\"", soci::into(minID);

				if (minID > 0) {
					if (m_backend != ::mdb::Backend::MySQL) {
						PROCESS_TABLE(ServerTable);
					} else {
						// MySQL can't deal with multiple foreign keys sharing a column (as encountered for instance in
						// the access_control_list table) which is clearly a bug
						// (https://bugs.mysql.com/bug.php?id=11305) but it seems that they don't care about it and just
						// advice people to not do this. Hence, we have to do the change manually for all tables.
						m_sql << "SET FOREIGN_KEY_CHECKS=0";

						PROCESS_TABLE(ACLTable);
						PROCESS_TABLE(BanTable);
						PROCESS_TABLE(ChannelLinkTable);
						PROCESS_TABLE(ChannelListenerTable);
						PROCESS_TABLE(ChannelPropertyTable);
						PROCESS_TABLE(ChannelTable);
						PROCESS_TABLE(ConfigTable);
						PROCESS_TABLE(GroupMemberTable);
						PROCESS_TABLE(GroupTable);
						PROCESS_TABLE(LogTable);
						PROCESS_TABLE(ServerTable);
						PROCESS_TABLE(UserPropertyTable);
						PROCESS_TABLE(UserTable);

						m_sql << "SET FOREIGN_KEY_CHECKS=1";
					}
				}
			}
#undef PROCESS_TABLE
		}

	} // namespace db
} // namespace server
} // namespace mumble
