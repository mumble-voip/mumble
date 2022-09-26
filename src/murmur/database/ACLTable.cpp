// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ACLTable.h"
#include "ChannelTable.h"
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

		constexpr const char *ACLTable::NAME;
		constexpr const char *ACLTable::column::server_id;
		constexpr const char *ACLTable::column::channel_id;
		constexpr const char *ACLTable::column::priority;
		constexpr const char *ACLTable::column::aff_user_id;
		constexpr const char *ACLTable::column::aff_group_id;
		constexpr const char *ACLTable::column::apply_in_current;
		constexpr const char *ACLTable::column::apply_in_sub;
		constexpr const char *ACLTable::column::granted_flags;
		constexpr const char *ACLTable::column::revoked_flags;


		ACLTable::ACLTable(soci::session &sql, ::mdb::Backend backend, const ChannelTable &channelTable,
						   const UserTable &userTable, const GroupTable &groupTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column channelIDCol(column::channel_id, ::mdb::DataType(::mdb::DataType::Integer));
			channelIDCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column priorityCol(column::priority, ::mdb::DataType(::mdb::DataType::Integer));
			priorityCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column affectedUserCol(column::aff_user_id, ::mdb::DataType(::mdb::DataType::Integer));
			affectedUserCol.setDefaultValue("NULL");

			::mdb::Column affectedGroupCol(column::aff_group_id, ::mdb::DataType(::mdb::DataType::Integer));
			affectedGroupCol.setDefaultValue("NULL");

			::mdb::Column applyCurCol(column::apply_in_current, ::mdb::DataType(::mdb::DataType::SmallInteger));
			applyCurCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column applySubCol(column::apply_in_sub, ::mdb::DataType(::mdb::DataType::SmallInteger));
			applySubCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column grantedCol(column::granted_flags, ::mdb::DataType(::mdb::DataType::Integer));
			grantedCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));
			grantedCol.setDefaultValue("0");

			::mdb::Column revokedCol(column::revoked_flags, ::mdb::DataType(::mdb::DataType::Integer));
			revokedCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));
			revokedCol.setDefaultValue("0");


			setColumns({ serverCol, channelIDCol, priorityCol, affectedUserCol, affectedGroupCol, applyCurCol,
						 applySubCol, grantedCol, revokedCol });


			::mdb::PrimaryKey pk(std::vector< std::string >{ column::server_id, column::channel_id, column::priority });
			setPrimaryKey(pk);

			::mdb::ForeignKey fk1(channelTable, { serverCol, channelIDCol });
			addForeignKey(fk1);

			::mdb::ForeignKey fk2(userTable, { serverCol, affectedUserCol });
			addForeignKey(fk2);

			::mdb::ForeignKey fk3(groupTable, { serverCol, affectedGroupCol });
			addForeignKey(fk3);
		}

		void ACLTable::addACL(const DBAcl &acl) {
			if (!acl.affectedGroupID && !acl.affectedUserID) {
				throw ::mdb::FormatException("Can't add ACL that doesn't affect anyone (neither group nor user)");
			}

			try {
				unsigned int userID      = 0;
				unsigned int groupID     = 0;
				soci::indicator userInd  = soci::i_null;
				soci::indicator groupInd = soci::i_null;

				if (acl.affectedUserID) {
					userInd = soci::i_ok;
					userID  = acl.affectedUserID.get();
				} else {
					groupInd = soci::i_ok;
					groupID  = acl.affectedGroupID.get();
				}

				soci::transaction transaction(m_sql);

				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::channel_id << ", "
					  << column::priority << ", " << column::aff_user_id << ", " << column::aff_group_id << ", "
					  << column::apply_in_current << ", " << column::apply_in_sub << ", " << column::granted_flags
					  << ", " << column::revoked_flags
					  << ") VALUES (:serverID, :channelID, :prio, :affectedUserID, :affectedGroupID, :applyCurrent, "
						 ":applySub, :granted, :revoked)",
					soci::use(acl.serverID), soci::use(acl.channelID), soci::use(acl.priority),
					soci::use(userID, userInd), soci::use(groupID, groupInd),
					soci::use(static_cast< int >(acl.applyInCurrentChannel)),
					soci::use(static_cast< int >(acl.applyInSubChannels)), soci::use(acl.grantedPrivilegeFlags),
					soci::use(acl.revokedPrivilegeFlags);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at adding new ACL with priority " + std::to_string(acl.priority) + " for channel with ID "
					+ std::to_string(acl.channelID) + " on server with ID " + std::to_string(acl.serverID)));
			}
		}

		void ACLTable::removeACL(const DBAcl &acl) { removeACL(acl.serverID, acl.channelID, acl.priority); }

		void ACLTable::removeACL(unsigned int serverID, unsigned int channelID, unsigned int priority) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::channel_id << " = :channelID AND " << column::priority << " = :prio",
					soci::use(serverID), soci::use(channelID), soci::use(priority);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at removing ACL with priority " + std::to_string(priority) + " for channel with ID "
					+ std::to_string(channelID) + " on server with ID " + std::to_string(serverID)));
			}
		}

		bool ACLTable::aclExists(const DBAcl &acl) { return aclExists(acl.serverID, acl.channelID, acl.priority); }

		bool ACLTable::aclExists(unsigned int serverID, unsigned int channelID, unsigned int priority) {
			try {
				int exists = false;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::channel_id << " = :channelID AND " << column::priority << " = :prio LIMIT 1",
					soci::use(serverID), soci::use(channelID), soci::use(priority), soci::into(exists);

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at checking for existence ACL with priority "
															  + std::to_string(priority) + " for channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		void ACLTable::clearACLs(unsigned int serverID, unsigned int channelID) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::channel_id << " = :channelID",
					soci::use(serverID), soci::use(channelID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at clearing ACLs for channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		std::vector< DBAcl > ACLTable::getAllACLs(unsigned int serverID, unsigned int channelID) {
			try {
				std::vector< DBAcl > acls;
				soci::row row;

				soci::transaction transaction(m_sql);

				soci::statement stmt =
					(m_sql.prepare << "SELECT " << column::priority << ", " << column::aff_user_id << ", "
								   << column::aff_group_id << ", " << column::apply_in_current << ", "
								   << column::apply_in_sub << ", " << column::granted_flags << ", "
								   << column::revoked_flags << " FROM \"" << NAME << "\" WHERE " << column::server_id
								   << " = :serverID AND " << column::channel_id << " = :channelID ORDER BY "
								   << column::priority,
					 soci::use(serverID), soci::use(channelID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 7);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);
					assert(row.get_properties(1).get_data_type() == soci::dt_integer);
					assert(row.get_properties(2).get_data_type() == soci::dt_integer);
					assert(row.get_properties(3).get_data_type() == soci::dt_integer);
					assert(row.get_properties(4).get_data_type() == soci::dt_integer);
					assert(row.get_properties(5).get_data_type() == soci::dt_integer);
					assert(row.get_properties(6).get_data_type() == soci::dt_integer);

					DBAcl acl;
					acl.serverID  = serverID;
					acl.channelID = channelID;
					acl.priority  = static_cast< unsigned int >(row.get< int >(0));
					if (row.get_indicator(1) == soci::i_ok) {
						acl.affectedUserID = static_cast< unsigned int >(row.get< int >(1));
					}
					if (row.get_indicator(2) == soci::i_ok) {
						acl.affectedGroupID = static_cast< unsigned int >(row.get< int >(2));
					}
					acl.applyInCurrentChannel = row.get< int >(3);
					acl.applyInSubChannels    = row.get< int >(4);
					acl.grantedPrivilegeFlags = static_cast< unsigned int >(row.get< int >(5));
					acl.revokedPrivilegeFlags = static_cast< unsigned int >(row.get< int >(6));

					acls.push_back(std::move(acl));
				}

				transaction.commit();

				return acls;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting ACLs for channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		std::size_t ACLTable::countOverallACLs(unsigned int serverID) {
			try {
				int count = 0;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT COUNT(*) FROM (SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID) AS dummy",
					soci::use(serverID), soci::into(count);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return static_cast< std::size_t >(count);
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at counting all ACLs on server with ID "
															  + std::to_string(serverID)));
			}
		}

		void ACLTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 10) {
					// Before v4, the column "user_id" was still called "player_id"
					// In v10 we renamed the table from "acl" to "access_control_lists" and the following columns have
					// been renamed as well:
					// "user_id" -> "affected_user_id"
					// "group_name" -> "affected_group_id" (Note that we also replaced using a group's name with
					// 		identifying groups by their ID instead)
					// 	"apply_here" -> "apply_in_current_channel"
					// 	"apply_sub" -> "apply_in_sub_channels"
					// 	"grantpriv" -> "granted_privilege_flags"
					// 	"revokepriv" -> "revoked_privilege_flags"
					std::string oldUserCol = fromSchemeVersion < 4 ? "player_id" : "user_id";

					// We have to look up the group ID corresponding to the group_name that was previously stored in the
					// acl table.
					// Our assumption here is that the GroupTable has already been migrated, so we can fetch the values
					// from the migrated table.
					// clang-format off
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::channel_id
						  << ", " << column::priority << ", " << column::aff_user_id << ", " << column::aff_group_id
						  << ", " << column::apply_in_current << ", " << column::apply_in_sub << ", "
						  << column::granted_flags << ", " << column::revoked_flags
						  << ") SELECT server_id, channel_id, priority, " << oldUserCol << ", "
						  << "("
						  <<	"SELECT group_id FROM \"" << GroupTable::NAME << "\" "
						  << 	"WHERE "
						  <<		GroupTable::column::server_id << " = outer_query.server_id "
						  <<		"AND " << GroupTable::column::group_name << " = outer_query.group_name"
						  << "), apply_here, apply_sub, grantpriv, revokepriv FROM \"acl"
						  << ::mdb::Database::OLD_TABLE_SUFFIX << "\" AS outer_query";
					// clang-format on

					// Check if the migration produced any useless ACLs (ones that apply to no one)
					int uselessACLs = 0;
					m_sql << "SELECT COUNT(*) FROM (SELECT 1 FROM \"" << NAME << "\" WHERE " << column::aff_user_id
						  << " = NULL AND " << column::aff_group_id << " = NULL) AS dummy",
						soci::into(uselessACLs);

					if (uselessACLs > 0) {
						// Immediately throwing on this might be a bit over-cautious but better safe than sorry.
						// By aborting the migration process by throwing an exception, we give the user to manually
						// inspect the database and perform the cleanup manually (in a way they see fit).
						throw ::mdb::MigrationException(
							std::string("Migration of table \"") + NAME + "\" from scheme version "
							+ std::to_string(fromSchemeVersion) + " to " + std::to_string(toSchemeVersion)
							+ " resulted in " + std::to_string(uselessACLs)
							+ " invalid ACLs (ones that apply neither to a user nor to a group.\n"
							+ "This could be the result of having ACLs in the database that reference non-existing "
							+ "group names.");
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
