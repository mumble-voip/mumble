// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ConfigTable.h"
#include "ServerTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/ForeignKey.h"
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
			constexpr const char *key       = "config_name";
			constexpr const char *value     = "config_value";
		} // namespace column

		constexpr const char *ConfigTable::NAME;

		ConfigTable::ConfigTable(soci::session &sql, ::mdb::Backend backend, const ServerTable &serverTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column keyCol(column::key, ::mdb::DataType(::mdb::DataType::VarChar, 255));
			keyCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column valueCol(column::value, ::mdb::DataType(::mdb::DataType::Text));
			valueCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));


			setColumns({ serverCol, keyCol, valueCol });


			::mdb::PrimaryKey pk(std::vector< std::string >{ column::server_id, column::key });
			setPrimaryKey(pk);


			::mdb::ForeignKey fk(serverTable, { &serverCol });
			addForeignKey(fk);
		}

		std::unordered_set< std::string > ConfigTable::migrate(unsigned int fromSchemeVersion,
															   unsigned int toSchemeVersion) {
			// TODO: We probably have to rename old tables first to facilitate the migration process


			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			std::unordered_set< std::string > tablesToDelete;

			try {
				if (fromSchemeVersion < 9) {
					// In v9 we renamed this table from "servers" to "virtual_servers"
					// -> Import all data from the old table into the new one
					m_sql << "INSERT INTO \"virtual_servers\" (server_id) SELECT server_id FROM \"servers\"";

					tablesToDelete.insert("servers");
				}
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					std::string("Failed at migrating table \"") + NAME + "\" from scheme version "
					+ std::to_string(fromSchemeVersion) + " to " + std::to_string(toSchemeVersion)));
			}

			return tablesToDelete;
		}

	} // namespace db
} // namespace server
} // namespace mumble
