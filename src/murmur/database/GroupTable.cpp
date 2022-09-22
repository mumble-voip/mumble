// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "GroupTable.h"
#include "ChannelTable.h"

#include "database/AccessException.h"
#include "database/Backend.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
#include "database/FormatException.h"
#include "database/Index.h"
#include "database/PrimaryKey.h"
#include "database/Utils.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		constexpr const char *GroupTable::NAME;
		constexpr const char *GroupTable::column::server_id;
		constexpr const char *GroupTable::column::group_id;
		constexpr const char *GroupTable::column::group_name;
		constexpr const char *GroupTable::column::channel_id;
		constexpr const char *GroupTable::column::inherit;
		constexpr const char *GroupTable::column::is_inheritable;


		GroupTable::GroupTable(soci::session &sql, ::mdb::Backend backend, const ChannelTable &channelTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column groupIDCol(column::group_id, ::mdb::DataType(::mdb::DataType::Integer));
			groupIDCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column groupNameCol(column::group_name, ::mdb::DataType(::mdb::DataType::VarChar, 255));
			groupNameCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column channelIDCol(column::channel_id, ::mdb::DataType(::mdb::DataType::Integer));
			channelIDCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column inheritCol(column::inherit, ::mdb::DataType(::mdb::DataType::SmallInteger));
			inheritCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));
			inheritCol.setDefaultValue("1");

			::mdb::Column isInheritableCol(column::is_inheritable, ::mdb::DataType(::mdb::DataType::SmallInteger));
			isInheritableCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));
			isInheritableCol.setDefaultValue("1");


			setColumns({ serverCol, groupIDCol, groupNameCol, channelIDCol, inheritCol, isInheritableCol });


			::mdb::PrimaryKey pk(std::vector< std::string >{ column::server_id, column::group_id });
			setPrimaryKey(pk);

			::mdb::ForeignKey fk(channelTable, { serverCol, channelIDCol });
			addForeignKey(fk);

			// Ensure group names are not duplicated within a given channel on a given server
			::mdb::Index ind(std::string(NAME) + "_unique_group_name_index",
							 { column::server_id, column::channel_id, column::group_name }, ::mdb::Index::UNIQUE);
			addIndex(ind, false);
		}

		unsigned int GroupTable::getFreeGroupID(unsigned int serverID) {
			try {
				int freeID;

				soci::transaction transaction(m_sql);

				m_sql << ::mdb::utils::getLowestUnoccupiedIDStatement(
					m_backend, NAME, column::group_id, { ::mdb::utils::ColAlias(column::server_id, "serverID") }),
					soci::use(serverID, "serverID"), soci::into(freeID);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return static_cast< unsigned int >(freeID);
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at fetching free group ID for server with ID "
															  + std::to_string(serverID)));
			}
		}

		unsigned int GroupTable::addGroup(const DBGroup &group) {
			if (group.name.empty()) {
				throw ::mdb::FormatException("A group's name must not be empty");
			}

			try {
				soci::transaction transaction(m_sql);

				long long group_id = 0;
				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::group_id << ", "
					  << column::channel_id << ", " << column::group_name << ", " << column::inherit << ", "
					  << column::is_inheritable
					  << ") VALUES (:serverID, :groupID, :channelID, :name, :inherit, :inheritable)",
					soci::use(group.serverID), soci::use(group.groupID), soci::use(group.channelID),
					soci::use(group.name), soci::use(static_cast< int >(group.inherit)),
					soci::use(static_cast< int >(group.is_inheritable)), soci::into(group_id);

				transaction.commit();

				return static_cast< unsigned int >(group_id);
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at adding group for channel with ID "
															  + std::to_string(group.channelID) + " on server with ID "
															  + std::to_string(group.serverID)));
			}
		}

		void GroupTable::updateGroup(const DBGroup &group) {
			assert(groupExists(group));

			if (group.name.empty()) {
				throw ::mdb::FormatException("A group's name must not be empty");
			}

			try {
				soci::transaction transaction(m_sql);

				m_sql << "UPDATE \"" << NAME << "\"SET " << column::group_name << " = :name, " << column::channel_id
					  << " = :channelID, " << column::inherit << " = :inherit, " << column::is_inheritable
					  << " = :inheritable WHERE " << column::server_id << " = :serverID AND " << column::group_id
					  << " = :groupID",
					soci::use(group.name), soci::use(group.channelID), soci::use(static_cast< int >(group.inherit)),
					soci::use(static_cast< int >(group.is_inheritable)), soci::use(group.serverID),
					soci::use(group.groupID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at updating group with ID "
															  + std::to_string(group.groupID) + " on server with ID "
															  + std::to_string(group.serverID)));
			}
		}

		void GroupTable::removeGroup(const DBGroup &group) { removeGroup(group.serverID, group.groupID); }

		void GroupTable::removeGroup(unsigned int serverID, unsigned int groupID) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::group_id << " = :groupID",
					soci::use(serverID), soci::use(groupID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at updating group with ID "
															  + std::to_string(groupID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		bool GroupTable::groupExists(const DBGroup &group) { return groupExists(group.serverID, group.groupID); }

		bool GroupTable::groupExists(unsigned int serverID, unsigned int groupID) {
			try {
				int exists = false;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::group_id << " = :groupID",
					soci::use(serverID), soci::use(groupID), soci::into(exists);

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at checking of existence of group with ID "
															  + std::to_string(groupID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		DBGroup GroupTable::getGroup(unsigned int serverID, unsigned int groupID) {
			assert(groupExists(serverID, groupID));

			try {
				DBGroup group;
				group.serverID = serverID;
				group.groupID  = groupID;

				int inherit        = false;
				int is_inheritable = false;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT " << column::channel_id << ", " << column::group_name << ", " << column::inherit
					  << ", " << column::is_inheritable << " FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID AND " << column::group_id << " = :groupID",
					soci::into(group.channelID), soci::into(group.name), soci::into(inherit),
					soci::into(is_inheritable), soci::use(serverID), soci::use(groupID);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				group.inherit        = inherit;
				group.is_inheritable = is_inheritable;

				return group;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting details for group with ID "
															  + std::to_string(groupID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		std::size_t GroupTable::countGroups(unsigned int serverID, unsigned int channelID) {
			try {
				long long nGroups = 0;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT COUNT(*) FROM (SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID AND " << column::channel_id << " = :channelID) AS dummy",
					soci::use(serverID), soci::use(channelID), soci::into(nGroups);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return static_cast< std::size_t >(nGroups);
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at counting groups for channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		std::vector< DBGroup > GroupTable::getAllGroups(unsigned int serverID, unsigned int channelID) {
			try {
				std::vector< DBGroup > groups;
				soci::row row;

				soci::transaction transaction(m_sql);

				soci::statement stmt = (m_sql.prepare << "SELECT " << column::group_id << ", " << column::group_name
													  << ", " << column::inherit << ", " << column::is_inheritable
													  << " FROM \"" << NAME << "\" WHERE " << column::server_id
													  << " = :serverID AND " << column::channel_id << " = :channelID",
										soci::use(serverID), soci::use(channelID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 4);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);
					assert(row.get_properties(1).get_data_type() == soci::dt_string);
					assert(row.get_properties(2).get_data_type() == soci::dt_integer);
					assert(row.get_properties(3).get_data_type() == soci::dt_integer);

					DBGroup group;
					group.serverID       = serverID;
					group.channelID      = channelID;
					group.groupID        = static_cast< unsigned int >(row.get< int >(0));
					group.name           = row.get< std::string >(1);
					group.inherit        = row.get< int >(2);
					group.is_inheritable = row.get< int >(3);

					groups.push_back(std::move(group));
				}

				transaction.commit();

				return groups;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting all groups for channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		void GroupTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 10) {
					// In v10 we renamed the columns "name" -> "group_name" and "inheritable" -> "is_inheritable"
					// -> Import all data from the old table into the new one
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::group_id << ", "
						  << column::group_name << ", " << column::channel_id << ", " << column::inherit << ", "
						  << column::is_inheritable
						  << ") SELECT server_id, group_id, name, channel_id, inherit, inheritable FROM \"groups"
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
