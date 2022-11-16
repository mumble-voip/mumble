// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ChannelListenerTable.h"
#include "ChannelTable.h"
#include "UserTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
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

		constexpr const char *ChannelListenerTable::NAME;
		constexpr const char *ChannelListenerTable::column::server_id;
		constexpr const char *ChannelListenerTable::column::user_id;
		constexpr const char *ChannelListenerTable::column::channel_id;
		constexpr const char *ChannelListenerTable::column::volume_adjustment;
		constexpr const char *ChannelListenerTable::column::enabled;


		ChannelListenerTable::ChannelListenerTable(soci::session &sql, ::mdb::Backend backend,
												   const UserTable &userTable, const ChannelTable &channelTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column userCol(column::user_id, ::mdb::DataType(::mdb::DataType::Integer));
			userCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column channelCol(column::channel_id, ::mdb::DataType(::mdb::DataType::Integer));
			channelCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column volumeCol(column::volume_adjustment, ::mdb::DataType(::mdb::DataType::Float));
			volumeCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column enabledCol(column::enabled, ::mdb::DataType(::mdb::DataType::SmallInteger));
			enabledCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));


			setColumns({ serverCol, userCol, channelCol, volumeCol, enabledCol });


			::mdb::PrimaryKey pk({ column::server_id, column::user_id, column::channel_id });
			setPrimaryKey(pk);


			::mdb::ForeignKey fk1(userTable, { serverCol, userCol });
			addForeignKey(fk1);

			::mdb::ForeignKey fk2(channelTable, { serverCol, channelCol });
			addForeignKey(fk2);
		}

		void ChannelListenerTable::addListener(const DBChannelListener &listener) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::user_id << ", "
					  << column::channel_id << ", " << column::volume_adjustment << ", " << column::enabled
					  << ") VALUES (:serverID, :userID, :channelID, :volAdjustment, :enabled)",
					soci::use(listener.serverID), soci::use(listener.userID), soci::use(listener.channelID),
					soci::use(static_cast< double >(listener.volumeAdjustment)),
					soci::use(static_cast< int >(listener.enabled));

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at adding listener of user with ID " + std::to_string(listener.userID)
					+ " to channel with ID " + std::to_string(listener.channelID) + " on server with ID "
					+ std::to_string(listener.serverID)));
			}
		}

		void ChannelListenerTable::removeListener(const DBChannelListener &listener) {
			removeListener(listener.serverID, listener.userID, listener.channelID);
		}

		void ChannelListenerTable::removeListener(unsigned int serverID, unsigned int userID, unsigned int channelID) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID AND " << column::channel_id << " = :channelID",
					soci::use(serverID), soci::use(userID), soci::use(channelID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at removing listener of user with ID " + std::to_string(userID) + " to channel with ID "
					+ std::to_string(channelID) + " on server with ID " + std::to_string(serverID)));
			}
		}

		bool ChannelListenerTable::listenerExists(const DBChannelListener &listener) {
			return listenerExists(listener.serverID, listener.userID, listener.channelID);
		}

		bool ChannelListenerTable::listenerExists(unsigned int serverID, unsigned int userID, unsigned int channelID) {
			try {
				int exists = false;

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID AND " << column::channel_id << " = :channelID LIMIT 1",
					soci::use(serverID), soci::use(userID), soci::use(channelID), soci::into(exists);

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at checking existence of listener of user with ID "
										   + std::to_string(userID) + " to channel with ID " + std::to_string(channelID)
										   + " on server with ID " + std::to_string(serverID)));
			}
		}

		DBChannelListener ChannelListenerTable::getListenerDetails(const DBChannelListener &listener) {
			return getListenerDetails(listener.serverID, listener.userID, listener.channelID);
		}

		DBChannelListener ChannelListenerTable::getListenerDetails(unsigned int serverID, unsigned int userID,
																   unsigned int channelID) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				DBChannelListener listener(serverID, channelID, userID);
				int enabled;
				double volAdj;

				m_sql << "SELECT " << column::enabled << ", " << column::volume_adjustment << " FROM \"" << NAME
					  << "\" WHERE " << column::server_id << " = :serverID AND " << column::user_id << " = :userID AND "
					  << column::channel_id << " = :channelID",
					soci::use(serverID), soci::use(userID), soci::use(channelID), soci::into(enabled),
					soci::into(volAdj);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				listener.enabled          = enabled;
				listener.volumeAdjustment = volAdj;

				return listener;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting details of listener of user with ID "
															  + std::to_string(userID) + " to channel with ID "
															  + std::to_string(channelID) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		void ChannelListenerTable::updateListener(const DBChannelListener &listener) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "UPDATE \"" << NAME << "\" SET " << column::volume_adjustment << " = :volAdjustment, "
					  << column::enabled << " = :enabled WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID AND " << column::channel_id << " = :channelID",
					soci::use(static_cast< double >(listener.volumeAdjustment)),
					soci::use(static_cast< int >(listener.enabled)), soci::use(listener.serverID),
					soci::use(listener.userID), soci::use(listener.channelID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at updating listener of user with ID " + std::to_string(listener.userID)
					+ " to channel with ID " + std::to_string(listener.channelID) + " on server with ID "
					+ std::to_string(listener.serverID)));
			}
		}

		std::vector< DBChannelListener > ChannelListenerTable::getListenersForUser(unsigned int serverID,
																				   unsigned int userID) {
			try {
				std::vector< DBChannelListener > listeners;
				soci::row row;

				::mdb::TransactionHolder transaction = ensureTransaction();

				soci::statement stmt =
					(m_sql.prepare << "SELECT " << column::channel_id << ", " << column::volume_adjustment << ", "
								   << column::enabled << " FROM \"" << NAME << "\" WHERE " << column::server_id
								   << " = :serverID AND " << column::user_id << " = :userID",
					 soci::use(serverID), soci::use(userID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 3);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);
					assert(row.get_properties(1).get_data_type() == soci::dt_double);
					assert(row.get_properties(2).get_data_type() == soci::dt_integer);

					DBChannelListener listener;
					listener.serverID = serverID;
					listener.userID   = userID;
					// Soci doesn't support unsigned integers directly, so we have to take the detour over
					// the signed int.
					listener.channelID        = static_cast< unsigned int >(row.get< int >(0));
					listener.volumeAdjustment = static_cast< float >(row.get< double >(1));
					listener.enabled          = row.get< int >(2);

					listeners.push_back(std::move(listener));
				}

				transaction.commit();

				return listeners;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at getting all channel listeners for user with ID " + std::to_string(userID)
					+ " on server with ID " + std::to_string(serverID)));
			}
		}

		std::vector< DBChannelListener > ChannelListenerTable::getListenersForChannel(unsigned int serverID,
																					  unsigned int channelID) {
			try {
				std::vector< DBChannelListener > listeners;
				soci::row row;

				::mdb::TransactionHolder transaction = ensureTransaction();

				soci::statement stmt =
					(m_sql.prepare << "SELECT " << column::user_id << ", " << column::volume_adjustment << ", "
								   << column::enabled << " FROM \"" << NAME << "\" WHERE " << column::server_id
								   << " = :serverID AND " << column::channel_id << " = :channelID",
					 soci::use(serverID), soci::use(channelID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 3);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);
					assert(row.get_properties(1).get_data_type() == soci::dt_double);
					assert(row.get_properties(2).get_data_type() == soci::dt_integer);

					DBChannelListener listener;
					listener.serverID  = serverID;
					listener.channelID = channelID;
					// Soci doesn't support unsigned integers directly, so we have to take the detour over
					// the signed int.
					listener.userID           = static_cast< unsigned int >(row.get< int >(0));
					listener.volumeAdjustment = static_cast< float >(row.get< double >(1));
					listener.enabled          = row.get< int >(2);

					listeners.push_back(std::move(listener));
				}

				transaction.commit();

				return listeners;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at getting all channel listeners on channel with ID " + std::to_string(channelID)
					+ " on server with ID " + std::to_string(serverID)));
			}
		}

		void ChannelListenerTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code old table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 9) {
					// The table only got introduced in v9
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
