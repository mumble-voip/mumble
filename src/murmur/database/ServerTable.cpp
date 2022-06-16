// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/PrimaryKey.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		namespace column {
			constexpr const char *server_id = "server_id";
		}

		constexpr const char *ServerTable::NAME;

		ServerTable::ServerTable(soci::session &sql, ::mdb::Backend backend) : ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer),
									::mdb::Column::AUTOINCREMENT);

			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			setColumns({ serverCol });

			::mdb::PrimaryKey pk(serverCol);
			setPrimaryKey(pk);
		}

		void ServerTable::addServer(unsigned int id) {
			try {
				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ") VALUES (:id)", soci::use(id);
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at adding server with ID " + std::to_string(id)));
			}
		}

		void ServerTable::removeServer(unsigned int id) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :id", soci::use(id);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at removing server with ID " + std::to_string(id)));
			}
		}

		bool ServerTable::serverExists(unsigned int id) const {
			try {
				int exists = 0;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :id", soci::use(id),
					soci::into(exists);

				transaction.commit();

				assert(exists == 0 || exists == 1);

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed checking whether server with ID " + std::to_string(id) + " exists"));
			}
		}

		std::unordered_set< std::string > ServerTable::migrate(unsigned int fromSchemeVersion,
															   unsigned int toSchemeVersion) {
			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			std::unordered_set< std::string > tablesToDelete;

			try {
				if (fromSchemeVersion < 9) {
					// In v9 we renamed this table from "servers" to "virtual_servers"
					// -> Import all data from the old table into the new one
					m_sql << "INSERT INTO \"virtual_servers\" (server_id) SELECT \"servers\" (server_id)";

					tablesToDelete.insert("servers");
				}
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					std::string("Failed at migrating table \"") + NAME + "\" from scheme version "
					+ std::to_string(fromSchemeVersion) + " to " + std::to_string(toSchemeVersion)));
			}

			return tablesToDelete;
		}

		void ServerTable::create() {
			// First call the original create function to actually create the table
			::mdb::Table::create();

			// Now insert a valid entry. Since we just created this table from scratch, we know that
			// there does not exist another virtual server yet, so the ID of the first virtual server
			// to be created (the one that is currently initializing the DB) has to have ID 1.
			addServer(1);
			// TODO: Check if this is not better done by the init code of the server itself
		}

	} // namespace db
} // namespace server
} // namespace mumble
