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
#include "database/Database.h"
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

		std::string ConfigTable::getConfig(unsigned int serverID, const std::string &configName,
										   const std::string &defaultValue) {
			try {
				std::string value = defaultValue;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT " << column::value << " FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :id AND " << column::key << " = :key",
					soci::use(serverID), soci::use(configName), soci::into(value);

				transaction.commit();

				return value;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting config \"" + configName
															  + "\" for server with ID " + std::to_string(serverID)));
			}
		}

		void ConfigTable::setConfig(unsigned int serverID, const std::string &configName, const std::string &value) {
			try {
				soci::transaction transaction(m_sql);

				// Perform an "upsert" operation - insert if it doesn't exist yet, insert otherwise
				int exists = 0;
				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :id AND " << column::key
					  << " = :key",
					soci::use(serverID), soci::use(configName), soci::into(exists);

				if (exists) {
					m_sql << "UPDATE \"" << NAME << "\" SET " << column::value << " = :value WHERE "
						  << column::server_id << " = :id AND " << column::key << " = :key",
						soci::use(value), soci::use(serverID), soci::use(configName);
				} else {
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::key << ", "
						  << column::value << ") VALUES (:id, :key, :value)",
						soci::use(serverID), soci::use(configName), soci::use(value);
				}

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at setting config \"" + configName
															  + "\" for server with ID " + std::to_string(serverID)));
			}
		}

		void ConfigTable::clearConfig(unsigned int serverID, const std::string &configName) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :id AND " << column::key
					  << " = :key",
					soci::use(serverID), soci::use(configName);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at clearing config \"" + configName
															  + "\" for server with ID " + std::to_string(serverID)));
			}
		}

		std::unordered_map< std::string, std::string > ConfigTable::getAllConfigs(unsigned int serverID) {
			std::unordered_map< std::string, std::string > configs;

			try {
				soci::row row;

				soci::transaction transaction(m_sql);

				soci::statement stmt = (m_sql.prepare << "SELECT " << column::key << ", " << column::value << " FROM \""
													  << NAME << "\" WHERE " << column::server_id << " = :id",
										soci::use(serverID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 2);
					assert(row.get_properties(0).get_data_type() == soci::dt_string);
					assert(row.get_properties(1).get_data_type() == soci::dt_string);

					configs.insert({ row.get< std::string >(0), row.get< std::string >(1) });
				}

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting all configs for server with ID "
															  + std::to_string(serverID)));
			}

			return configs;
		} // namespace db

		void ConfigTable::clearAllConfigs(unsigned int serverID) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :id", soci::use(serverID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at clearing all configs for server with ID "
															  + std::to_string(serverID)));
			}
		}


		void ConfigTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 4) {
					// In v4 we renamed the column "keystring" -> "key"
					m_sql << "INSERT INTO \"" << getName() << "\" (" << column::server_id << ", " << column::key << ", "
						  << column::value << ") SELECT server_id, keystring, value FROM \"config"
						  << mdb::Database::OLD_TABLE_SUFFIX << "\"";

				} else if (fromSchemeVersion < 9) {
					// In v9 we renamed the columns "key" -> "config_name" and "value" -> "config_value"
					// -> Import all data from the old table into the new one
					m_sql << "INSERT INTO \"" << getName() << "\" (" << column::server_id << ", " << column::key << ", "
						  << column::value << ") SELECT server_id, key, value FROM \"config"
						  << mdb::Database::OLD_TABLE_SUFFIX << "\"";
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
