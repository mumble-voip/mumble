// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserPropertyTable.h"
#include "UserTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
#include "database/MigrationException.h"
#include "database/PrimaryKey.h"
#include "database/Utils.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		constexpr const char *UserPropertyTable::NAME;
		constexpr const char *UserPropertyTable::column::server_id;
		constexpr const char *UserPropertyTable::column::user_id;
		constexpr const char *UserPropertyTable::column::key;
		constexpr const char *UserPropertyTable::column::value;


		UserPropertyTable::UserPropertyTable(soci::session &sql, ::mdb::Backend backend, const UserTable &userTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column channelCol(column::user_id, ::mdb::DataType(::mdb::DataType::Integer));
			channelCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column keyCol(column::key, ::mdb::DataType(::mdb::DataType::Integer));
			keyCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column valueCol(column::value, ::mdb::DataType(::mdb::DataType::Text));
			valueCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));


			setColumns({ serverCol, channelCol, keyCol, valueCol });


			::mdb::PrimaryKey pk({ column::server_id, column::user_id, column::key });
			setPrimaryKey(pk);


			::mdb::ForeignKey fk(userTable, { serverCol, channelCol });
			addForeignKey(fk);
		}

		std::string UserPropertyTable::doGetProperty(const DBUser &user, UserProperty property) {
			try {
				soci::transaction transaction(m_sql);

				std::string val;

				m_sql << "SELECT " << column::value << " FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID AND " << column::user_id << " = :userID AND " << column::key << " = :key",
					soci::into(val), soci::use(user.serverID), soci::use(user.registeredUserID),
					soci::use(static_cast< int >(property));

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return val;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at fetching property " + std::to_string(static_cast< int >(property)) + " for user with ID "
					+ std::to_string(user.registeredUserID) + " on server " + std::to_string(user.serverID)));
			}
		}

		bool UserPropertyTable::isPropertySet(const DBUser &user, UserProperty property) {
			try {
				soci::transaction transaction(m_sql);

				int exists = 0;
				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID AND " << column::key << " = :key",
					soci::into(exists), soci::use(user.serverID), soci::use(user.registeredUserID),
					soci::use(static_cast< int >(property));

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at checking whether property " + std::to_string(static_cast< int >(property))
					+ " is set for user with ID " + std::to_string(user.registeredUserID) + " on server "
					+ std::to_string(user.serverID)));
			}
		}

		void UserPropertyTable::setProperty(const DBUser &user, UserProperty property, const std::string &value) {
			bool propertyAlreadySet = isPropertySet(user, property);
			try {
				soci::transaction transaction(m_sql);

				if (propertyAlreadySet) {
					m_sql << "UPDATE \"" << NAME << "\" SET " << column::value << " = :value WHERE "
						  << column::server_id << " = :serverID AND " << column::user_id << " = :userID AND "
						  << column::key << " = :key",
						soci::use(value), soci::use(user.serverID), soci::use(user.registeredUserID),
						soci::use(static_cast< int >(property));
				} else {
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::user_id << ", "
						  << column::key << ", " << column::value << ") VALUES (:serverID, :userID, :key, :value)",
						soci::use(user.serverID), soci::use(user.registeredUserID),
						soci::use(static_cast< int >(property)), soci::use(value);
				}

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at setting or updating property " + std::to_string(static_cast< int >(property))
					+ " for user with ID " + std::to_string(user.registeredUserID) + " on server "
					+ std::to_string(user.serverID)));
			}
		}

		void UserPropertyTable::clearProperty(const DBUser &user, UserProperty property) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID AND " << column::key << " = :key",
					soci::use(user.serverID), soci::use(user.registeredUserID), soci::use(static_cast< int >(property));

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at clearing property " + std::to_string(static_cast< int >(property)) + " for user with ID "
					+ std::to_string(user.registeredUserID) + " on server " + std::to_string(user.serverID)));
			}
		}

		void UserPropertyTable::clearAllProperties(const DBUser &user) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID",
					soci::use(user.serverID), soci::use(user.registeredUserID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at clearing all properties for user with ID "
															  + std::to_string(user.registeredUserID) + " on server "
															  + std::to_string(user.serverID)));
			}
		}

		void UserPropertyTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code old table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 3) {
					// Nothing to do here
				} else if (fromSchemeVersion == 3) {
					// Before v4, this table didn't exist, so there is no need to do any migration from an older version
					// of this table. However, before v4 a users's email was stored in the "players" table and
					// thus, we'll have to move it out of there.
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::user_id << ", "
						  << column::key << ", " << column::value
						  << ") SELECT server_id, player_id, :key, email FROM \"users"
						  << ::mdb::Database::OLD_TABLE_SUFFIX << "\" WHERE email IS NOT NULL",
						soci::use(static_cast< int >(UserProperty::Email));
				} else if (fromSchemeVersion < 10) {
					// In v10 we renamed this table from "user_info" to "user_properties"
					// -> Import all data from the old table into the new one
					m_sql << "INSERT INTO \"" << getName() << "\" (" << column::server_id << ", " << column::user_id
						  << ", " << column::key << ", " << column::value
						  << ") SELECT server_id, user_id, key, value FROM \"user_info"
						  << mdb::Database::OLD_TABLE_SUFFIX << "\"";
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
