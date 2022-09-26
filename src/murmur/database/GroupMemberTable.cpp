// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "GroupMemberTable.h"
#include "GroupTable.h"
#include "UserTable.h"

#include "database/AccessException.h"
#include "database/Backend.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
#include "database/FormatException.h"
#include "database/Index.h"
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

		constexpr const char *GroupMemberTable::NAME;
		constexpr const char *GroupMemberTable::column::server_id;
		constexpr const char *GroupMemberTable::column::group_id;
		constexpr const char *GroupMemberTable::column::user_id;
		constexpr const char *GroupMemberTable::column::add_to_group;


		GroupMemberTable::GroupMemberTable(soci::session &sql, ::mdb::Backend backend, const GroupTable &groupTable,
										   const UserTable &userTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column groupIDCol(column::group_id, ::mdb::DataType(::mdb::DataType::Integer));
			groupIDCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column userIDCol(column::user_id, ::mdb::DataType(::mdb::DataType::Integer));
			userIDCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column addToGroupCol(column::add_to_group, ::mdb::DataType(::mdb::DataType::SmallInteger));
			addToGroupCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));
			addToGroupCol.setDefaultValue("1");


			setColumns({ serverCol, groupIDCol, userIDCol, addToGroupCol });


			::mdb::PrimaryKey pk(std::vector< std::string >{ column::server_id, column::group_id });
			setPrimaryKey(pk);

			::mdb::ForeignKey fk1(groupTable, { serverCol, groupIDCol });
			addForeignKey(fk1);

			::mdb::ForeignKey fk2(userTable, { serverCol, userIDCol });
			addForeignKey(fk2);
		}

		void GroupMemberTable::addEntry(const DBGroupMember &member) {
			addEntry(member.serverID, member.groupID, member.userID, member.addToGroup);
		}

		void GroupMemberTable::addEntry(unsigned int serverID, unsigned int groupID, unsigned int userID,
										bool addToGroup) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::group_id << ", "
					  << column::user_id << ", " << column::add_to_group
					  << ") VALUES (:serverID, :groupID, :userID, :add)",
					soci::use(serverID), soci::use(groupID), soci::use(userID),
					soci::use(static_cast< int >(addToGroup));

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at adding new group entry for user with ID " + std::to_string(userID) + " to group with ID "
					+ std::to_string(groupID) + " on server with ID " + std::to_string(serverID)));
			}
		}

		void GroupMemberTable::removeEntry(const DBGroupMember &member) {
			removeEntry(member.serverID, member.groupID, member.userID);
		}

		void GroupMemberTable::removeEntry(unsigned int serverID, unsigned int groupID, unsigned int userID) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::group_id << " = :groupID AND " << column::user_id << " = :userID",
					soci::use(serverID), soci::use(groupID), soci::use(userID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at removing group entry for user with ID " + std::to_string(userID) + " to group with ID "
					+ std::to_string(groupID) + " on server with ID " + std::to_string(serverID)));
			}
		}

		bool GroupMemberTable::entryExists(const DBGroupMember &member) {
			return entryExists(member.serverID, member.groupID, member.userID);
		}

		bool GroupMemberTable::entryExists(unsigned int serverID, unsigned int groupID, unsigned int userID) {
			try {
				int exists = false;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::group_id << " = :groupID AND " << column::user_id << " = :userID LIMIT 1",
					soci::use(serverID), soci::use(groupID), soci::use(userID), soci::into(exists);

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at removing group entry for user with ID " + std::to_string(userID) + " to group with ID "
					+ std::to_string(groupID) + " on server with ID " + std::to_string(serverID)));
			}
		}

		std::vector< DBGroupMember > GroupMemberTable::getEntries(unsigned int serverID, unsigned int groupID) {
			try {
				std::vector< DBGroupMember > groups;
				soci::row row;

				soci::transaction transaction(m_sql);

				soci::statement stmt = (m_sql.prepare << "SELECT " << column::user_id << ", " << column::add_to_group
													  << " FROM \"" << NAME << "\" WHERE " << column::server_id
													  << " = :serverID AND " << column::group_id << " = :groupID",
										soci::use(serverID), soci::use(groupID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 2);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);
					assert(row.get_properties(1).get_data_type() == soci::dt_integer);

					DBGroupMember member;
					member.serverID   = serverID;
					member.groupID    = groupID;
					member.userID     = static_cast< unsigned int >(row.get< int >(0));
					member.addToGroup = row.get< int >(1);

					groups.push_back(std::move(member));
				}

				transaction.commit();

				return groups;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting all group entries for group with ID "
															  + std::to_string(groupID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		void GroupMemberTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 4) {
					// Before v4, the column "user_id" was still called "player_id"
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::group_id << ", "
						  << column::user_id << ", " << column::add_to_group
						  << ") SELECT server_id, group_id, player_id, addit FROM \"group_members"
						  << ::mdb::Database::OLD_TABLE_SUFFIX << "\"";
				} else if (fromSchemeVersion < 10) {
					// In v10 we renamed the column "addit" -> "add_to_group"
					// -> Import all data from the old table into the new one
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::group_id << ", "
						  << column::user_id << ", " << column::add_to_group
						  << ") SELECT server_id, group_id, user_id, addit FROM \"group_members"
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
