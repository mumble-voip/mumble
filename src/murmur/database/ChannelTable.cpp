// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ChannelTable.h"
#include "ServerTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
#include "database/MigrationException.h"
#include "database/NoDataException.h"
#include "database/TransactionHolder.h"
#include "database/Utils.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		constexpr const char *ChannelTable::NAME;
		constexpr const char *ChannelTable::column::server_id;
		constexpr const char *ChannelTable::column::channel_id;
		constexpr const char *ChannelTable::column::parent_id;
		constexpr const char *ChannelTable::column::name;
		constexpr const char *ChannelTable::column::inherit_acl;


		ChannelTable::ChannelTable(soci::session &sql, ::mdb::Backend backend, const ServerTable &serverTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column channelIDCol(column::channel_id, ::mdb::DataType(::mdb::DataType::Integer));
			channelIDCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column parentIDCol(column::parent_id, ::mdb::DataType(::mdb::DataType::Integer));
			parentIDCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column nameCol(column::name, ::mdb::DataType(::mdb::DataType::VarChar, 255));
			nameCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column inheritACLCol(column::inherit_acl, ::mdb::DataType(::mdb::DataType::SmallInteger));
			inheritACLCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));


			setColumns({ serverCol, channelIDCol, parentIDCol, nameCol, inheritACLCol });


			::mdb::PrimaryKey pk({ serverCol.getName(), channelIDCol.getName() });
			setPrimaryKey(pk);


			::mdb::ForeignKey fk1(serverTable, { serverCol });
			addForeignKey(fk1);

			::mdb::ForeignKey fk2(*this, { serverCol, parentIDCol });
			addForeignKey(fk2);
		}

		void ChannelTable::addChannel(const DBChannel &channel) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				short inheritACL = channel.inheritACL;

				m_sql << "INSERT INTO \"" << NAME << "\" (\"" << column::server_id << "\", \"" << column::channel_id
					  << "\", \"" << column::parent_id << "\", \"" << column::name << "\", \"" << column::inherit_acl
					  << "\") VALUES (:serverID, :channelID, :parentID, :name, :inheritACL)",
					soci::use(channel.serverID), soci::use(channel.channelID), soci::use(channel.parentID),
					soci::use(channel.name), soci::use(inheritACL);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at adding channel with ID " + std::to_string(channel.channelID)
										   + " on server with ID " + std::to_string(channel.serverID)));
			}
		}

		void ChannelTable::removeChannel(const DBChannel &channel) {
			removeChannel(channel.serverID, channel.channelID);
		}

		void ChannelTable::removeChannel(unsigned int serverID, unsigned int channelID) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE \"" << column::server_id << "\" = :serverID AND \""
					  << column::channel_id << "\" = :channelID",
					soci::use(serverID), soci::use(channelID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at removing channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		void ChannelTable::updateChannel(const DBChannel &channel) {
			assert(channelExists(channel));

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				short inheritACL = channel.inheritACL;

				m_sql << "UPDATE \"" << NAME << "\" SET \"" << column::parent_id << "\" = :parentID, \"" << column::name
					  << "\" = :name, \"" << column::inherit_acl << "\" = :inheritACL WHERE \"" << column::server_id
					  << "\" = :serverID AND \"" << column::channel_id << "\" = :channelID",
					soci::use(channel.parentID), soci::use(channel.name), soci::use(inheritACL),
					soci::use(channel.serverID), soci::use(channel.channelID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at updating channel with ID " + std::to_string(channel.channelID)
										   + " on server with ID " + std::to_string(channel.serverID)));
			}
		}

		bool ChannelTable::channelExists(const DBChannel &channel) {
			return channelExists(channel.serverID, channel.channelID);
		}

		bool ChannelTable::channelExists(unsigned int serverID, unsigned int channelID) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				int exists = 0;

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE \"" << column::server_id << "\" = :serverID AND \""
					  << column::channel_id << "\" = :channelID LIMIT 1",
					soci::use(serverID), soci::use(channelID), soci::into(exists);

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at checking for existence of channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		DBChannel ChannelTable::getChannelData(unsigned int serverID, unsigned int channelID) {
			DBChannel channel;
			channel.serverID  = serverID;
			channel.channelID = channelID;

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				int inherit = 0;

				m_sql << "SELECT \"" << column::parent_id << "\", \"" << column::name << "\", \"" << column::inherit_acl
					  << "\" FROM \"" << NAME << "\" WHERE \"" << column::server_id << "\" = :serverID AND \""
					  << column::channel_id << "\" = :channelID",
					soci::into(channel.parentID), soci::into(channel.name), soci::into(inherit), soci::use(serverID),
					soci::use(channelID);

				channel.inheritACL = inherit;

				transaction.commit();

				if (!m_sql.got_data()) {
					// Ensure that the looked for channel actually existed and we were able to fetch the corresponding
					// data
					throw ::mdb::NoDataException("No channel with ID " + std::to_string(channelID)
												 + " on server with ID " + std::to_string(serverID));
				}
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at checking for existence of channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}

			return channel;
		}

		unsigned int ChannelTable::getFreeChannelID(unsigned int serverID) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				unsigned int id = 0;

				m_sql << ::mdb::utils::getLowestUnoccupiedIDStatement(
					m_backend, NAME, column::channel_id, { ::mdb::utils::ColAlias(column::server_id, "serverID") }),
					soci::use(serverID, "serverID"), soci::into(id);

				transaction.commit();

				::mdb::utils::verifyQueryResultedInData(m_sql);

				return id;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at fetching a free channel ID for server with ID "
															  + std::to_string(serverID)));
			}
		}

		std::vector< unsigned int > ChannelTable::getChildrenOf(unsigned int serverID, unsigned int channelID) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				std::vector< unsigned int > children;
				soci::row row;

				soci::statement stmt =
					(m_sql.prepare << "SELECT \"" << column::channel_id << "\" FROM \"" << NAME << "\" WHERE \""
								   << column::server_id << "\" = :serverID AND \"" << column::parent_id
								   << "\" = :parentID AND NOT \"" << column::channel_id << "\" = :parentID",
					 soci::use(serverID, "serverID"), soci::use(channelID, "parentID"), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 1);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);

					children.push_back(static_cast< unsigned int >(row.get< int >(0)));
				}

				transaction.commit();

				return children;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at fetching children of channel "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}


		void ChannelTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code old table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion <= toSchemeVersion);

			try {
				if (fromSchemeVersion < 10) {
					// In v10 we renamed columns "name" -> "channel_name" and "inheritacl" -> "inherit_acl"
					// Furthermore, the following changes have been made:
					// - For the root channel parent_id is no longer NULL but instead equal to the channel's ID
					// - name can no longer be NULL (if there is an entry with a NULL name, we want the migration to
					// fail)
					// - inheritacl can no longer be NULL. Corresponding entries should instead use 0 (false) as the
					// default
					m_sql << "INSERT INTO \"" << NAME << "\" (\"" << column::server_id << "\", \"" << column::channel_id
						  << "\", \"" << column::parent_id << "\", \"" << column::name << "\", \""
						  << column::inherit_acl << "\") SELECT \"server_id\", \"channel_id\", "
						  << ::mdb::utils::nonNullOf("\"parent_id\"").otherwise("\"channel_id\"") << ", \"name\", "
						  << ::mdb::utils::nonNullOf("\"inheritacl\"").otherwise("0") << " FROM \"channels"
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
