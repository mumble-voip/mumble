// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserTable.h"
#include "ChannelTable.h"
#include "ChronoUtils.h"
#include "ServerTable.h"

#include "database/AccessException.h"
#include "database/Column.h"
#include "database/Constraint.h"
#include "database/DataType.h"
#include "database/Database.h"
#include "database/ForeignKey.h"
#include "database/FormatException.h"
#include "database/MigrationException.h"
#include "database/TransactionHolder.h"
#include "database/Trigger.h"
#include "database/Utils.h"

#include "MumbleConstants.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>

namespace mdb = ::mumble::db;

namespace mumble {
namespace server {
	namespace db {

		constexpr const char *UserTable::NAME;
		constexpr const char *UserTable::column::server_id;
		constexpr const char *UserTable::column::user_id;
		constexpr const char *UserTable::column::user_name;
		constexpr const char *UserTable::column::password_hash;
		constexpr const char *UserTable::column::salt;
		constexpr const char *UserTable::column::kdf_iterations;
		constexpr const char *UserTable::column::last_channel_id;
		constexpr const char *UserTable::column::texture;
		constexpr const char *UserTable::column::last_active;
		constexpr const char *UserTable::column::last_disconnect;


		UserTable::UserTable(soci::session &sql, ::mdb::Backend backend, const ServerTable &serverTable,
							 ChannelTable &channelTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column userIDCol(column::user_id, ::mdb::DataType(::mdb::DataType::Integer));
			userIDCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column usernameCol(column::user_name, ::mdb::DataType(::mdb::DataType::VarChar, 255));
			usernameCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column pwHashCol(column::password_hash, ::mdb::DataType(::mdb::DataType::VarChar, 255));
			pwHashCol.setDefaultValue("NULL");

			::mdb::Column pwSaltCol(column::salt, ::mdb::DataType(::mdb::DataType::VarChar, 128));
			pwSaltCol.setDefaultValue("NULL");

			::mdb::Column kdfIterCol(column::kdf_iterations, ::mdb::DataType(::mdb::DataType::Integer));
			kdfIterCol.setDefaultValue("NULL");

			::mdb::Column lastChannelCol(column::last_channel_id, ::mdb::DataType(::mdb::DataType::Integer));
			lastChannelCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column textureCol(column::texture, ::mdb::DataType(::mdb::DataType::Blob));
			textureCol.setDefaultValue("NULL");

			::mdb::Column lastActiveCol(column::last_active, ::mdb::DataType(::mdb::DataType::EpochTime));
			lastActiveCol.setDefaultValue("0");
			lastActiveCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column lastDisconnectCol(column::last_disconnect, ::mdb::DataType(::mdb::DataType::EpochTime));
			lastDisconnectCol.setDefaultValue("0");
			lastDisconnectCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));


			setColumns({ serverCol, userIDCol, usernameCol, pwHashCol, pwSaltCol, kdfIterCol, lastChannelCol,
						 textureCol, lastActiveCol, lastDisconnectCol });


			::mdb::PrimaryKey pk({ serverCol.getName(), userIDCol.getName() });
			setPrimaryKey(pk);


			::mdb::ForeignKey fk1(serverTable, { serverCol });
			addForeignKey(fk1);

			// Ensure that usernames are unique on a given server
			::mdb::Index nameIndex(std::string(NAME) + "_unique_" + column::user_name,
								   std::vector< std::string >{ column::server_id, column::user_name },
								   ::mdb::Index::UNIQUE);
			addIndex(nameIndex, false);


			// Add triggers that shall essentially act as a foreign key constraint. Thus, it shall fail inserts and
			// updates that attempt to set a user's last channel to a non-existing channel. We don't use an actual
			// foreign key for this as that would require us to specify a behavior for when a channel gets deleted.
			// However, what we would like to do in this case (reset the  affected last channel entries to point to the
			// root channel) is not supported with foreign keys.
			std::string condition = std::string("NOT EXISTS (SELECT 1 FROM \"") + ChannelTable::NAME + "\" WHERE "
									+ ChannelTable::column::server_id + " = NEW." + column::server_id + " AND "
									+ ChannelTable::column::channel_id + " = NEW." + column::last_channel_id + ")";
			std::string channelExistsTriggerBody = ::mdb::utils::triggerErrorStatement(
				m_backend, std::string("Attempted to use a non-existing channel as ") + column::last_channel_id);

			::mdb::Trigger channelExistsConstraintInsert(
				std::string(NAME) + "_channel_exists_constraint_insert_trigger", ::mdb::Trigger::Timing::Before,
				::mdb::Trigger::Event::Insert, channelExistsTriggerBody, condition);
			::mdb::Trigger channelExistsConstraintUpdate(
				std::string(NAME) + "_channel_exists_constraint_update_trigger", ::mdb::Trigger::Timing::Before,
				::mdb::Trigger::Event::Update, channelExistsTriggerBody, condition);
			addTrigger(channelExistsConstraintInsert, false);
			addTrigger(channelExistsConstraintUpdate, false);

			// Add a trigger to the channel table that ensures that every time a channel is deleted, the corresponding
			// last channel entries in the users table are reset to point to the root channel
			std::string resetChannelTriggerBody = std::string("UPDATE \"") + NAME + "\" SET " + column::last_channel_id
												  + " = " + std::to_string(Mumble::ROOT_CHANNEL_ID) + " WHERE "
												  + column::server_id + " = OLD." + ChannelTable::column::server_id
												  + " AND " + column::last_channel_id + " = OLD."
												  + ChannelTable::column::channel_id + ";";
			::mdb::Trigger resetLastChannelOnDelete(std::string(NAME) + "_reset_last_channel_on_delete_trigger",
													::mdb::Trigger::Timing::After, ::mdb::Trigger::Event::Delete,
													resetChannelTriggerBody);
			channelTable.addTrigger(resetLastChannelOnDelete, false);
		}

		void UserTable::addUser(const DBUser &user, const std::string &name) {
			if (name.empty()) {
				throw ::mdb::FormatException("A user's \"name\" can't be empty");
			}

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::user_id << ", "
					  << column::user_name << ", " << column::last_channel_id
					  << ") VALUES (:serverID, :userID, :userName, :lastChannel)",
					soci::use(user.serverID), soci::use(user.registeredUserID), soci::use(name),
					soci::use(Mumble::ROOT_CHANNEL_ID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at adding user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		void UserTable::removeUser(const DBUser &user) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID",
					soci::use(user.serverID), soci::use(user.registeredUserID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at removing user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		bool UserTable::userExists(const DBUser &user) {
			try {
				int exists = false;

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID LIMIT 1",
					soci::use(user.serverID), soci::use(user.registeredUserID), soci::into(exists);

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at checking for existence of user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		void UserTable::updateData(const DBUser &user, const DBUserData &data) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				// Ensure that the blob's lifetime never escapes the transaction
				{
					soci::indicator pwHashInd  = soci::i_ok;
					soci::indicator pwSaltInd  = soci::i_ok;
					soci::indicator kdfIterInd = soci::i_ok;
					soci::indicator textureInd = soci::i_ok;

					soci::blob textureBlob(m_sql);

					if (data.name.empty()) {
						throw ::mdb::FormatException("A \"name\" is required when updating a user's data");
					}

					// Handle optional/unset values
					if (data.password.passwordHash.empty()) {
						pwHashInd = soci::i_null;
					}
					if (data.password.salt.empty()) {
						pwSaltInd = soci::i_null;
					}
					if (data.password.kdfIterations == 0) {
						kdfIterInd = soci::i_null;
					}
					if (data.texture.empty()) {
						textureInd = soci::i_null;
					} else {
						// Copy the data into a BLOB object
						textureBlob.write_from_start(reinterpret_cast< const char * >(data.texture.data()),
													 data.texture.size());
					}

					// These fields will be 0, if the corresponding time-point was not set explicitly (and was thus
					// default-constructed), which coincides with our default value for these properties.
					std::size_t lastActive     = toEpochSeconds(data.lastActive);
					std::size_t lastDisconnect = toEpochSeconds(data.lastDisconnect);

					m_sql << "UPDATE \"" << NAME << "\" SET " << column::password_hash << " = :pwHash, " << column::salt
						  << " = :salt, " << column::kdf_iterations << " = :kdfIter, " << column::user_name
						  << " = :name, " << column::last_channel_id << " = :lastChannelID, " << column::texture
						  << " = :texture, " << column::last_active << " = :lastActive, " << column::last_disconnect
						  << " = :lastDisconnect WHERE " << column::server_id << " = :serverID AND " << column::user_id
						  << " = :userID",
						soci::use(data.password.passwordHash, pwHashInd), soci::use(data.password.salt, pwSaltInd),
						soci::use(data.password.kdfIterations, kdfIterInd), soci::use(data.name),
						soci::use(data.lastChannelID), soci::use(textureBlob, textureInd), soci::use(lastActive),
						soci::use(lastDisconnect), soci::use(user.serverID), soci::use(user.registeredUserID);
				}

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at updating user data for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		DBUserData UserTable::getData(const DBUser &user) {
			DBUserData data;

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				// Ensure that the blob's lifetime never escapes the transaction
				{
					soci::blob textureBlob(m_sql);
					std::size_t lastActive     = 0;
					std::size_t lastDisconnect = 0;

					// These indicators are required in order to be able to handle NULL values
					soci::indicator pwHashInd, pwSaltInd, kdfIterInd, textureInd;

					m_sql << "SELECT " << column::user_name << ", " << column::password_hash << ", " << column::salt
						  << ", " << column::kdf_iterations << ", " << column::last_channel_id << ", "
						  << column::texture << ", " << column::last_active << ", " << column::last_disconnect
						  << " FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
						  << column::user_id << " = :userID",
						soci::use(user.serverID), soci::use(user.registeredUserID), soci::into(data.name),
						soci::into(data.password.passwordHash, pwHashInd), soci::into(data.password.salt, pwSaltInd),
						soci::into(data.password.kdfIterations, kdfIterInd), soci::into(data.lastChannelID),
						soci::into(textureBlob, textureInd), soci::into(lastActive), soci::into(lastDisconnect);

					::mdb::utils::verifyQueryResultedInData(m_sql);

					if (textureInd != soci::i_null) {
						// Copy texture data
						data.texture.resize(textureBlob.get_len());
						textureBlob.read_from_start(reinterpret_cast< char * >(data.texture.data()),
													data.texture.size());
					}

					data.lastActive     = std::chrono::steady_clock::time_point(std::chrono::seconds(lastActive));
					data.lastDisconnect = std::chrono::steady_clock::time_point(std::chrono::seconds(lastDisconnect));
				}

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting data for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}

			return data;
		}

		unsigned int UserTable::getFreeUserID(unsigned int serverID) {
			DBUserData data;

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				unsigned int id;

				m_sql << ::mdb::utils::getLowestUnoccupiedIDStatement(
					m_backend, NAME, column::user_id, { ::mdb::utils::ColAlias(column::server_id, "serverID") }),
					soci::use(serverID, "serverID"), soci::into(id);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return id;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting free user ID on server with ID "
															  + std::to_string(serverID)));
			}
		}

		void UserTable::clearLastDisconnect(const DBUser &user) { setLastDisconnect(user, {}); }

		void UserTable::setLastDisconnect(const DBUser &user, const std::chrono::steady_clock::time_point &timepoint) {
			assert(userExists(user));

			try {
				std::size_t lastDisconnect = toEpochSeconds(timepoint);

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "UPDATE \"" << NAME << "\" SET " << column::last_disconnect << " = :lastDisconnect WHERE "
					  << column::server_id << " = :serverID AND " << column::user_id << " = :userID",
					soci::use(lastDisconnect), soci::use(user.serverID), soci::use(user.registeredUserID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at setting last_disconnect for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		std::chrono::steady_clock::time_point UserTable::getLastDisconnect(const DBUser &user) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				std::size_t lastDisconnected = 0;

				m_sql << "SELECT " << column::last_disconnect << " FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID AND " << column::user_id << " = :userID",
					soci::use(user.serverID), soci::use(user.registeredUserID), soci::into(lastDisconnected);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return std::chrono::steady_clock::time_point(std::chrono::seconds(lastDisconnected));
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting last_disconnect for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		void UserTable::clearLastActive(const DBUser &user) { setLastActive(user, {}); }

		void UserTable::setLastActive(const DBUser &user, const std::chrono::steady_clock::time_point &timepoint) {
			assert(userExists(user));

			try {
				std::size_t lastActive = toEpochSeconds(timepoint);

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "UPDATE \"" << NAME << "\" SET " << column::last_active << " = :lastActive WHERE "
					  << column::server_id << " = :serverID AND " << column::user_id << " = :userID",
					soci::use(lastActive), soci::use(user.serverID), soci::use(user.registeredUserID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at setting last_active for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		std::chrono::steady_clock::time_point UserTable::getLastActive(const DBUser &user) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				std::size_t lastActive = 0;

				m_sql << "SELECT " << column::last_active << " FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID AND " << column::user_id << " = :userID",
					soci::use(user.serverID), soci::use(user.registeredUserID), soci::into(lastActive);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return std::chrono::steady_clock::time_point(std::chrono::seconds(lastActive));
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting last_active for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		void UserTable::clearLastChannelID(const DBUser &user) {
			// The last channel of a user can't really be cleared, but we can reset it to point to the root channel,
			// which has the same effect as not having a last channel stored.
			setLastChannelID(user, Mumble::ROOT_CHANNEL_ID);
		}

		void UserTable::setLastChannelID(const DBUser &user, unsigned int channelID) {
			assert(userExists(user));

			// Setting a user's last channel tends to indicate that said user just moved in a new channel and thus was
			// just active
			std::size_t lastActive = toEpochSeconds(std::chrono::steady_clock::now());

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "UPDATE \"" << NAME << "\" SET " << column::last_channel_id << " = :lastChannel, "
					  << column::last_active << " = :lastActive WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID",
					soci::use(channelID), soci::use(lastActive), soci::use(user.serverID),
					soci::use(user.registeredUserID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at setting last_channel_id for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		unsigned int UserTable::getLastChannelID(const DBUser &user) {
			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				unsigned int last_channel_id = Mumble::ROOT_CHANNEL_ID;

				m_sql << "SELECT " << column::last_channel_id << " FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID AND " << column::user_id << " = :userID",
					soci::use(user.serverID), soci::use(user.registeredUserID), soci::into(last_channel_id);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return last_channel_id;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting last_channel_id for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		void UserTable::clearTexture(const DBUser &user) { setTexture(user, {}); }

		void UserTable::setTexture(const DBUser &user, gsl::span< std::uint8_t > texture) {
			assert(userExists(user));

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				// Ensure that the blob's lifetime never escapes the transaction
				{
					soci::blob textureBlob(m_sql);
					soci::indicator ind = soci::i_null;

					if (!texture.empty()) {
						ind = soci::i_ok;
						// Copy data to BLOB object
						textureBlob.write_from_start(reinterpret_cast< const char * >(texture.data()), texture.size());
					}

					m_sql << "UPDATE \"" << NAME << "\" SET " << column::texture << " = :texture WHERE "
						  << column::server_id << " = :serverID AND " << column::user_id << " = :userID",
						soci::use(textureBlob, ind), soci::use(user.serverID), soci::use(user.registeredUserID);
				}

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at setting texture for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		std::vector< std::uint8_t > UserTable::getTexture(const DBUser &user) {
			try {
				std::vector< std::uint8_t > texture;

				::mdb::TransactionHolder transaction = ensureTransaction();

				// Ensure that the blob's lifetime never escapes the transaction
				{
					soci::blob textureBlob(m_sql);

					// This indicator is required in order to be able to handle NULL values
					soci::indicator textureInd;

					m_sql << "SELECT " << column::texture << " FROM \"" << NAME << "\" WHERE " << column::server_id
						  << " = :serverID AND " << column::user_id << " = :userID",
						soci::use(user.serverID), soci::use(user.registeredUserID), soci::into(textureBlob, textureInd);

					::mdb::utils::verifyQueryResultedInData(m_sql);

					if (textureInd != soci::i_null) {
						// Copy texture data
						texture.resize(textureBlob.get_len());
						textureBlob.read_from_start(reinterpret_cast< char * >(texture.data()), texture.size());
					}
				}

				transaction.commit();

				return texture;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting texture for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		void UserTable::clearPassword(const DBUser &user) { setPassword(user, {}); }

		void UserTable::setPassword(const DBUser &user, const DBUserData::PasswordData &password) {
			assert(userExists(user));

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				soci::indicator pwInd   = password.passwordHash.empty() ? soci::i_null : soci::i_ok;
				soci::indicator saltInd = password.salt.empty() ? soci::i_null : soci::i_ok;
				soci::indicator kdfInd  = password.kdfIterations == 0 ? soci::i_null : soci::i_ok;

				m_sql << "UPDATE \"" << NAME << "\" SET " << column::password_hash << " = :pwHash, " << column::salt
					  << " = :salt, " << column::kdf_iterations << " = :kdfIter  WHERE " << column::server_id
					  << " = :serverID AND " << column::user_id << " = :userID",
					soci::use(password.passwordHash, pwInd), soci::use(password.salt, saltInd),
					soci::use(password.kdfIterations, kdfInd), soci::use(user.serverID),
					soci::use(user.registeredUserID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at setting password data for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		DBUserData::PasswordData UserTable::getPassword(const DBUser &user) {
			try {
				DBUserData::PasswordData pwData;

				soci::indicator pwHashInd;
				soci::indicator pwSaltInd;
				soci::indicator kdfIterInd;

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "SELECT " << column::password_hash << ", " << column::salt << ", " << column::kdf_iterations
					  << " FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND "
					  << column::user_id << " = :userID",
					soci::use(user.serverID), soci::use(user.registeredUserID),
					soci::into(pwData.passwordHash, pwHashInd), soci::into(pwData.salt, pwSaltInd),
					soci::into(pwData.kdfIterations, kdfIterInd);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				return pwData;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting password of user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		void UserTable::setName(const DBUser &user, const std::string &name) {
			assert(userExists(user));

			if (name.empty()) {
				throw ::mdb::FormatException("A user's \"name\" can't be empty");
			}

			try {
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "UPDATE \"" << NAME << "\" SET " << column::user_name << " = :name WHERE " << column::server_id
					  << " = :serverID AND " << column::user_id << " = :userID",
					soci::use(name), soci::use(user.serverID), soci::use(user.registeredUserID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at setting name for user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		std::string UserTable::getName(const DBUser &user) {
			try {
				std::string name;

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "SELECT " << column::user_name << " FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID AND " << column::user_id << " = :userID",
					soci::use(user.serverID), soci::use(user.registeredUserID), soci::into(name);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				transaction.commit();

				assert(!name.empty());

				return name;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting name of user with ID "
															  + std::to_string(user.registeredUserID)
															  + " on server with ID " + std::to_string(user.serverID)));
			}
		}

		boost::optional< unsigned int > UserTable::findUser(unsigned int serverID, const std::string &name,
															bool caseSensitive) {
			try {
				unsigned int id;

				::mdb::TransactionHolder transaction = ensureTransaction();

				std::string nameCol =
					caseSensitive ? column::user_name : std::string("LOWER(") + column::user_name + ")";
				std::string nameParam = caseSensitive ? ":userName" : "LOWER(:userName)";

				m_sql << "SELECT " << column::user_id << " FROM \"" << NAME << "\" WHERE " << column::server_id
					  << " = :serverID AND " << nameCol << " = " << nameParam,
					soci::use(serverID), soci::use(name), soci::into(id);

				transaction.commit();

				return m_sql.got_data() ? id : boost::optional< unsigned int >{};
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at searching for user with name \"" + name + "\""
															  + " on server with ID " + std::to_string(serverID)));
			}
		}

		std::vector< DBUser > UserTable::getRegisteredUsers(unsigned int serverID) {
			try {
				std::vector< DBUser > users;
				soci::row row;

				::mdb::TransactionHolder transaction = ensureTransaction();

				soci::statement stmt = (m_sql.prepare << "SELECT " << column::user_id << " FROM \"" << NAME
													  << "\" WHERE " << column::server_id << " = :serverID",
										soci::use(serverID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 1);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);

					// Soci doesn't support unsigned integers directly, so we have to take the detour over
					// the signed int.
					users.push_back(DBUser{ serverID, static_cast< unsigned int >(row.get< int >(0)) });
				}

				transaction.commit();

				return users;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting registered users on server with ID "
															  + std::to_string(serverID)));
			}
		}


		void UserTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code old table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 4) {
					// Before v4, the table was called "players" and the "user_id" col was called "player_id"
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::user_id << ", "
						  << column::user_name << ", " << column::password_hash << ", " << column::last_channel_id
						  << ", " << column::texture << ", " << column::last_active
						  << ") SELECT server_id, player_id, name, pw, lastchannel, texture, last_active FROM \"players"
						  << ::mdb::Database::OLD_TABLE_SUFFIX << "\"";
				} else if (fromSchemeVersion < 6) {
					// Before v6, there were no salt an kdfIteration columns
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::user_id << ", "
						  << column::user_name << ", " << column::password_hash << ", " << column::last_channel_id
						  << ", " << column::texture << ", " << column::last_active
						  << ") SELECT server_id, user_id, name, pw, lastchannel, texture, last_active FROM \"users"
						  << ::mdb::Database::OLD_TABLE_SUFFIX << "\"";
				} else if (fromSchemeVersion < 8) {
					// Before v8 there was no last_disconnect column
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::user_id << ", "
						  << column::user_name << ", " << column::password_hash << ", " << column::salt << ", "
						  << column::kdf_iterations << ", " << column::last_channel_id << ", " << column::texture
						  << ", " << column::last_active
						  << ") SELECT server_id, user_id, name, pw, salt, kdfiterations, lastchannel, "
							 "texture, last_active FROM \"users"
						  << ::mdb::Database::OLD_TABLE_SUFFIX << "\"";
				} else if (fromSchemeVersion < 10) {
					// In v10, we renamed columns "name" -> "user_name", "pw" -> "password_hash",
					// "kdfiterations" -> "kdf_iterations" and "lastchannel" -> "last_channel_id"
					m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::user_id << ", "
						  << column::user_name << ", " << column::password_hash << ", " << column::salt << ", "
						  << column::kdf_iterations << ", " << column::last_channel_id << ", " << column::texture
						  << ", " << column::last_active << ", " << column::last_disconnect
						  << ") SELECT server_id, user_id, name, pw, salt, kdfiterations, lastchannel, "
							 "texture, last_active, last_disconnect FROM \"users"
						  << ::mdb::Database::OLD_TABLE_SUFFIX << "\"";
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
