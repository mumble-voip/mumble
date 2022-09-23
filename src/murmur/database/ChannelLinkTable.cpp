// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ChannelLinkTable.h"
#include "ChannelTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
#include "database/FormatException.h"
#include "database/PrimaryKey.h"
#include "database/Trigger.h"
#include "database/Utils.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		constexpr const char *ChannelLinkTable::NAME;
		constexpr const char *ChannelLinkTable::column::server_id;
		constexpr const char *ChannelLinkTable::column::first_id;
		constexpr const char *ChannelLinkTable::column::second_id;


		ChannelLinkTable::ChannelLinkTable(soci::session &sql, ::mdb::Backend backend, const ChannelTable &channelTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column firstChanCol(column::first_id, ::mdb::DataType(::mdb::DataType::Integer));
			firstChanCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column secondChanCol(column::second_id, ::mdb::DataType(::mdb::DataType::Integer));
			secondChanCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));


			setColumns({ serverCol, firstChanCol, secondChanCol });


			::mdb::PrimaryKey pk({ column::server_id, column::first_id, column::second_id });
			setPrimaryKey(pk);


			::mdb::ForeignKey fk1(channelTable, { serverCol, firstChanCol });
			addForeignKey(fk1);

			::mdb::ForeignKey fk2(channelTable, { serverCol, secondChanCol });
			addForeignKey(fk2);

			// Add a trigger that causes an error on update or insert, if second_id <= first_id. This is to ensure
			// that we always maintain second_id > first_id in the DB and thus, the unique constraint introduced by
			// the primary key is enough to avoid duplicate entries in the DB (where we consider (a,b) and (b,a) to
			// be duplicates of each other).
			std::string triggerBody = ::mdb::utils::triggerErrorStatement(
				m_backend, std::string("Violated requirement ") + column::second_id + " > " + column::first_id);
			std::string triggerCondition = std::string("NEW.") + column::second_id + " <= NEW." + column::first_id;
			::mdb::Trigger insertTrigger(std::string(NAME) + "_ensure_id_order_insert_trigger",
										 ::mdb::Trigger::Timing::Before, ::mdb::Trigger::Event::Insert, triggerBody,
										 triggerCondition);
			::mdb::Trigger updateTrigger(std::string(NAME) + "_ensure_id_order_update_trigger",
										 ::mdb::Trigger::Timing::Before, ::mdb::Trigger::Event::Update, triggerBody,
										 triggerCondition);
			addTrigger(insertTrigger, false);
			addTrigger(updateTrigger, false);
		}

		void ChannelLinkTable::addLink(const DBChannelLink &link) {
			if (link.firstChannelID == link.secondChannelID) {
				throw ::mdb::FormatException("Can't link a channel to itself");
			}

			try {
				// Ensure secondID > firstID
				unsigned int firstID;
				unsigned int secondID;
				if (link.secondChannelID > link.firstChannelID) {
					firstID  = link.firstChannelID;
					secondID = link.secondChannelID;
				} else {
					firstID  = link.secondChannelID;
					secondID = link.firstChannelID;
					;
				}

				soci::transaction transaction(m_sql);

				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::first_id << ", "
					  << column::second_id << ") VALUES (:serverID, :firstID, :secondID)",
					soci::use(link.serverID), soci::use(firstID), soci::use(secondID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at linking channels " + std::to_string(link.firstChannelID) + " and "
					+ std::to_string(link.secondChannelID) + " on server with ID " + std::to_string(link.serverID)));
			}
		}

		void ChannelLinkTable::removeLink(const DBChannelLink &link) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::first_id << " = :firstID AND " << column::second_id << " = :secondID",
					soci::use(link.serverID), soci::use(link.firstChannelID), soci::use(link.secondChannelID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at removing link between channels " + std::to_string(link.firstChannelID) + " and "
					+ std::to_string(link.secondChannelID) + " on server with ID " + std::to_string(link.serverID)));
			}
		}

		bool ChannelLinkTable::linkExists(const DBChannelLink &link) {
			try {
				// Ensure secondID > firstID
				unsigned int firstID;
				unsigned int secondID;
				if (link.secondChannelID > link.firstChannelID) {
					firstID  = link.firstChannelID;
					secondID = link.secondChannelID;
				} else {
					firstID  = link.secondChannelID;
					secondID = link.firstChannelID;
				}

				int exists = false;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::first_id << " = :firstID AND " << column::second_id << " = :secondID",
					soci::use(link.serverID), soci::use(firstID), soci::use(secondID), soci::into(exists);

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at checking whether channels " + std::to_string(link.firstChannelID)
										   + " and " + std::to_string(link.secondChannelID) + " on server with ID "
										   + std::to_string(link.serverID) + " are linked"));
			}
		}

		std::vector< DBChannelLink > ChannelLinkTable::getAllLinks(unsigned int serverID) {
			try {
				std::vector< DBChannelLink > links;
				soci::row row;

				soci::transaction transaction(m_sql);

				soci::statement stmt =
					(m_sql.prepare << "SELECT " << column::first_id << ", " << column::second_id << " FROM \"" << NAME
								   << "\" WHERE " << column::server_id << " = :serverID",
					 soci::use(serverID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 2);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);
					assert(row.get_properties(1).get_data_type() == soci::dt_integer);

					DBChannelLink link;
					link.serverID = serverID;
					// Soci doesn't support unsigned integers directly, so we have to take the detour over
					// the signed int.
					link.firstChannelID = static_cast< unsigned int >(row.get< int >(0));
					link.firstChannelID = static_cast< unsigned int >(row.get< int >(1));

					links.push_back(std::move(link));
				}

				transaction.commit();

				return links;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting all link channels on server with ID "
															  + std::to_string(serverID)));
			}
		}

		void ChannelLinkTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code old table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 10) {
					// In v10 we renamed the columns "channel_id" -> "first_channel_id" and "link_id" ->
					// "second_channel_id"
					// -> Import all data from the old table into the new one
					// Note that we also added a new constraint that requires second_channel_id > first_channel_id,
					// so we have to make sure we are inserting the data in accordance to this new constraint.
					// clang-format off
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::first_id << ", "
						  << column::second_id << ") "
						  << "SELECT server_id, channel_id, link_id FROM \"channel_links" << mdb::Database::OLD_TABLE_SUFFIX << "\""
						  <<	"WHERE channel_id < link_id"
						  << "UNION"
						  << "SELECT server_id, link_id, channel_id FROM \"channel_links" << mdb::Database::OLD_TABLE_SUFFIX << "\""
						  <<	"WHERE link_id < channel_id";
					// clang-format on
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
