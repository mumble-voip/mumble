// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "BanTable.h"
#include "ChronoUtils.h"
#include "ServerTable.h"

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

		constexpr const char *BanTable::NAME;
		constexpr const char *BanTable::column::server_id;
		constexpr const char *BanTable::column::base_address;
		constexpr const char *BanTable::column::prefix_length;
		constexpr const char *BanTable::column::user_name;
		constexpr const char *BanTable::column::cert_hash;
		constexpr const char *BanTable::column::reason;
		constexpr const char *BanTable::column::start_date;
		constexpr const char *BanTable::column::duration;


		BanTable::BanTable(soci::session &sql, ::mdb::Backend backend, const ServerTable &serverTable)
			: ::mdb::Table(sql, backend, NAME) {
			::mdb::Column serverCol(column::server_id, ::mdb::DataType(::mdb::DataType::Integer));
			serverCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			// 45 characters is the maximum possible length of a valid textual representation of an IPv6 address
			// See https://stackoverflow.com/a/166157
			::mdb::Column addressCol(column::base_address, ::mdb::DataType(::mdb::DataType::VarChar, 45));
			addressCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column prefixCol(column::prefix_length, ::mdb::DataType(::mdb::DataType::Integer));
			prefixCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));

			::mdb::Column userNameCol(column::user_name, ::mdb::DataType(::mdb::DataType::VarChar, 255));
			userNameCol.setDefaultValue("NULL");

			::mdb::Column userCertCol(column::cert_hash, ::mdb::DataType(::mdb::DataType::VarChar, 255));
			userCertCol.setDefaultValue("NULL");

			::mdb::Column reasonCol(column::reason, ::mdb::DataType(::mdb::DataType::Text));
			reasonCol.setDefaultValue("NULL");

			::mdb::Column startDateCol(column::start_date, ::mdb::DataType(::mdb::DataType::EpochTime));
			startDateCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));
			startDateCol.setDefaultValue("0");

			::mdb::Column durationCol(column::duration, ::mdb::DataType(::mdb::DataType::Integer));
			durationCol.addConstraint(::mdb::Constraint(::mdb::Constraint::NotNull));
			durationCol.setDefaultValue("0");


			setColumns(
				{ serverCol, addressCol, prefixCol, userNameCol, userCertCol, reasonCol, startDateCol, durationCol });


			::mdb::PrimaryKey pk(
				std::vector< std::string >{ column::server_id, column::base_address, column::prefix_length });
			setPrimaryKey(pk);

			::mdb::ForeignKey fk(serverTable, { serverCol });
			addForeignKey(fk);
		}

		void BanTable::addBan(const DBBan &ban) {
			try {
				std::string baseAddress = DBBan::ipv6ToString(ban.baseAddress);
				unsigned int startEpoch = toEpochSeconds(ban.startDate);
				unsigned int duration   = ban.duration.count();
				std::string userName;
				std::string userCert;
				std::string reason;
				soci::indicator nameInd   = soci::i_null;
				soci::indicator certInd   = soci::i_null;
				soci::indicator reasonInd = soci::i_null;

				if (ban.bannedUserName) {
					userName = ban.bannedUserName.get();
					nameInd  = soci::i_ok;
				}
				if (ban.bannedUserCertHash) {
					userCert = ban.bannedUserCertHash.get();
					certInd  = soci::i_ok;
				}
				if (ban.reason) {
					reason    = ban.reason.get();
					reasonInd = soci::i_ok;
				}

				soci::transaction transaction(m_sql);

				m_sql << "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::base_address << ", "
					  << column::prefix_length << ", " << column::user_name << ", " << column::cert_hash << ", "
					  << column::reason << ", " << column::start_date << ", " << column::duration
					  << ") VALUES (:serverID, LOWER(:baseAddr), :prefixLength, :userName, :userCert, :reason, "
						 ":startDate, :duration)",
					soci::use(ban.serverID), soci::use(baseAddress), soci::use(ban.prefixLength),
					soci::use(userName, nameInd), soci::use(userCert, certInd), soci::use(reason, reasonInd),
					soci::use(startEpoch), soci::use(duration);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at adding new Ban  for " + DBBan::ipv6ToString(ban.baseAddress) + "/"
					+ std::to_string(ban.prefixLength) + " on server with ID " + std::to_string(ban.serverID)));
			}
		}

		void BanTable::removeBan(const DBBan &ban) {
			removeBan(ban.serverID, DBBan::ipv6ToString(ban.baseAddress), ban.prefixLength);
		}

		void BanTable::removeBan(unsigned int serverID, const std::string &baseAddress, std::uint8_t prefixLength) {
			try {
				soci::transaction transaction(m_sql);

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND LOWER("
					  << column::base_address << ") = LOWER(:baseAddress) AND " << column::prefix_length
					  << " = :prefixLength",
					soci::use(serverID), soci::use(baseAddress), soci::use(prefixLength);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at removing Ban  for " + baseAddress + "/"
															  + std::to_string(prefixLength) + " on server with ID "
															  + std::to_string(serverID)));
			}
		}

		bool BanTable::banExists(const DBBan &ban) {
			return banExists(ban.serverID, DBBan::ipv6ToString(ban.baseAddress), ban.prefixLength);
		}

		bool BanTable::banExists(unsigned int serverID, const std::string &baseAddress, std::uint8_t prefixLength) {
			try {
				int exists = false;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE " << column::server_id << " = :serverID AND LOWER("
					  << column::base_address << ") = LOWER(:baseAddress) AND " << column::prefix_length
					  << " = :prefixLength",
					soci::use(serverID), soci::use(baseAddress), soci::use(prefixLength), soci::into(exists);

				transaction.commit();

				return exists;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException(
					"Failed at checking whether Ban for " + baseAddress + "/" + std::to_string(prefixLength)
					+ " exists on server with ID " + std::to_string(serverID)));
			}
		}

		DBBan BanTable::getBanDetails(unsigned int serverID, const DBBan::ipv6_type &baseAddress,
									  std::uint8_t prefixLength) {
			return getBanDetails(serverID, DBBan::ipv6ToString(baseAddress), prefixLength);
		}

		DBBan BanTable::getBanDetails(unsigned int serverID, const std::string &baseAddress,
									  std::uint8_t prefixLength) {
			try {
				DBBan ban;
				ban.serverID     = serverID;
				ban.baseAddress  = DBBan::ipv6FromString(baseAddress);
				ban.prefixLength = prefixLength;

				unsigned int startEpoch;
				unsigned int duration;
				std::string userName;
				std::string userCert;
				std::string reason;
				soci::indicator nameInd;
				soci::indicator certInd;
				soci::indicator reasonInd;

				soci::transaction transaction(m_sql);

				m_sql << "SELECT " << column::user_name << ", " << column::cert_hash << ", " << column::reason << ", "
					  << column::start_date << ", " << column::duration << " FROM \"" << NAME << "\" WHERE "
					  << column::server_id << " = :serverID AND " << column::base_address << " = :baseAddress AND "
					  << column::prefix_length << " = :prefix_length",
					soci::into(userName, nameInd), soci::into(userCert, certInd), soci::into(reason, reasonInd),
					soci::into(startEpoch), soci::into(duration), soci::use(serverID), soci::use(baseAddress),
					soci::use(prefixLength);

				::mdb::utils::verifyQueryResultedInData(m_sql);

				if (nameInd == soci::i_ok) {
					ban.bannedUserName = std::move(userName);
				}
				if (certInd == soci::i_ok) {
					ban.bannedUserCertHash = std::move(userCert);
				}
				if (reasonInd == soci::i_ok) {
					ban.reason = std::move(reason);
				}
				ban.startDate = std::chrono::time_point< std::chrono::steady_clock >(std::chrono::seconds(startEpoch));
				ban.duration  = std::chrono::seconds(duration);

				transaction.commit();

				return ban;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::AccessException("Failed at getting details for Ban of " + baseAddress
															  + "/" + std::to_string(prefixLength)
															  + " on server with ID " + std::to_string(serverID)));
			}
		}

		std::vector< DBBan > BanTable::getAllBans(unsigned int serverID) {
			try {
				std::vector< DBBan > bans;
				soci::row row;

				soci::transaction transaction(m_sql);

				soci::statement stmt =
					(m_sql.prepare << "SELECT " << column::base_address << ", " << column::prefix_length << ", "
								   << column::user_name << ", " << column::cert_hash << ", " << column::reason << ", "
								   << column::start_date << ", " << column::duration << " FROM \"" << NAME
								   << "\" WHERE " << column::server_id << " = :serverID",
					 soci::use(serverID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 7);
					assert(row.get_properties(0).get_data_type() == soci::dt_string);
					assert(row.get_properties(1).get_data_type() == soci::dt_integer);
					assert(row.get_properties(2).get_data_type() == soci::dt_string);
					assert(row.get_properties(3).get_data_type() == soci::dt_string);
					assert(row.get_properties(4).get_data_type() == soci::dt_string);
					assert(row.get_properties(5).get_data_type() == soci::dt_long_long);
					assert(row.get_properties(6).get_data_type() == soci::dt_integer);

					DBBan ban;
					ban.serverID     = serverID;
					ban.baseAddress  = DBBan::ipv6FromString(row.get< std::string >(0));
					ban.prefixLength = row.get< int >(1);
					if (row.get_indicator(2) == soci::i_ok) {
						ban.bannedUserName = row.get< std::string >(2);
					}
					if (row.get_indicator(3) == soci::i_ok) {
						ban.bannedUserCertHash = row.get< std::string >(3);
					}
					if (row.get_indicator(4) == soci::i_ok) {
						ban.reason = row.get< std::string >(4);
					}
					ban.startDate = std::chrono::time_point< std::chrono::steady_clock >(
						std::chrono::seconds(row.get< long long >(5)));
					ban.duration = std::chrono::seconds(row.get< int >(6));

					bans.push_back(std::move(ban));
				}

				transaction.commit();

				return bans;
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at getting Bans on server with ID " + std::to_string(serverID)));
			}
		}

		void doClearBans(soci::session &sql, unsigned int serverID) {
			sql << "DELETE FROM \"" << BanTable::NAME << "\" WHERE " << BanTable::column::server_id << " = :serverID",
				soci::use(serverID);
		}

		void BanTable::clearBans(unsigned int serverID) {
			try {
				soci::transaction transaction(m_sql);

				doClearBans(m_sql, serverID);

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at clearing Bans on server with ID " + std::to_string(serverID)));
			}
		}

		void BanTable::setBans(unsigned int serverID, const std::vector< DBBan > &bans) {
			try {
				soci::transaction transaction(m_sql);

				// Step 1: Clear old bans
				doClearBans(m_sql, serverID);

				// Step 2: Insert new bans
				soci::statement stmt =
					m_sql.prepare
					<< "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::base_address << ", "
					<< column::prefix_length << ", " << column::user_name << ", " << column::cert_hash << ", "
					<< column::reason << ", " << column::start_date << ", " << column::duration
					<< ") VALUES (:serverID, LOWER(:baseAddr), :prefixLength, :userName, :userCert, :reason, "
					   ":startDate, :duration)";

				for (const DBBan &currentBan : bans) {
					assert(currentBan.serverID == serverID);

					std::string baseAddress = DBBan::ipv6ToString(currentBan.baseAddress);
					unsigned int startEpoch = toEpochSeconds(currentBan.startDate);
					unsigned int duration   = currentBan.duration.count();
					std::string userName;
					std::string userCert;
					std::string reason;
					soci::indicator nameInd   = soci::i_null;
					soci::indicator certInd   = soci::i_null;
					soci::indicator reasonInd = soci::i_null;

					if (currentBan.bannedUserName) {
						userName = currentBan.bannedUserName.get();
						nameInd  = soci::i_ok;
					}
					if (currentBan.bannedUserCertHash) {
						userCert = currentBan.bannedUserCertHash.get();
						certInd  = soci::i_ok;
					}
					if (currentBan.reason) {
						reason    = currentBan.reason.get();
						reasonInd = soci::i_ok;
					}

					stmt.exchange(soci::use(serverID));
					stmt.exchange(soci::use(baseAddress));
					stmt.exchange(soci::use(currentBan.prefixLength));
					stmt.exchange(soci::use(userName, nameInd));
					stmt.exchange(soci::use(userCert, certInd));
					stmt.exchange(soci::use(reason, reasonInd));
					stmt.exchange(soci::use(startEpoch));
					stmt.exchange(soci::use(duration));

					stmt.define_and_bind();
					stmt.execute(true);
					stmt.bind_clean_up();
				}

				transaction.commit();
			} catch (const soci::soci_error &) {
				std::throw_with_nested(
					::mdb::AccessException("Failed at setting Bans on server with ID " + std::to_string(serverID)));
			}
		}

		void BanTable::migrate(unsigned int fromSchemeVersion, unsigned int toSchemeVersion) {
			// Note: Always hard-code table and column names in this function in order to ensure that this
			// migration path always stays the same regardless of whether the respective named constants change.
			assert(fromSchemeVersion < toSchemeVersion);

			try {
				if (fromSchemeVersion < 10) {
					// Before v4, we stored IPv4 addresses in the DB and there only were the fields server_id, base (the
					// IPv4 address) and mask.
					// In v10 the following columns have been renamed:
					// "base" -> "ipv6_base_address" Also we switched from storing a binary representation of the IP
					//		address to storing a textual representation, instead.
					// "mask" -> "prefix_length"
					// "name" -> "banned_user_name"
					// "hash" -> "banned_user_cert_hash"
					// "start" -> "start_date" Also we changed its type from a native DATE format into using epoch
					//		seconds.
					soci::row row;

					std::string startConversion;
					std::string baseConversion;
					switch (m_backend) {
						case ::mdb::Backend::SQLite:
							startConversion = "UNIXEPOCH(start)";
							baseConversion  = "HEX(base)";
							break;
						case ::mdb::Backend::MySQL:
							startConversion = "UNIX_TIMESTAMP(start)";
							baseConversion  = "HEX(base)";
							break;
						case ::mdb::Backend::PostgreSQL:
							startConversion = "EXTRACT(EPOCH FROM start)";
							baseConversion  = "ENCODE(base::bytea, 'hex')";
							break;
					}
					assert(!startConversion.empty());
					std::string selectQuery =
						fromSchemeVersion < 4 ? std::string("SELECT server_id, ") + baseConversion + ", mask"
											  : std::string("SELECT server_id, ") + baseConversion
													+ ", mask, name, hash, reason, " + startConversion + ", duration";

					soci::statement selectStmt =
						(m_sql.prepare << selectQuery << " FROM \"bans" << ::mdb::Database::OLD_TABLE_SUFFIX << "\"",
						 soci::into(row));

					soci::statement insertStmt =
						m_sql.prepare
						<< "INSERT INTO \"" << NAME << "\" (" << column::server_id << ", " << column::base_address
						<< ", " << column::prefix_length << ", " << column::user_name << ", " << column::cert_hash
						<< ", " << column::reason << ", " << column::start_date << ", " << column::duration
						<< ") VALUES (:serverID, :baseAddr, :prefixLength, :userName, :certHash, :reason, "
						   ":startDate, :duration)";

					selectStmt.execute(false);

					while (selectStmt.fetch()) {
						int serverID;
						std::string baseAddress;
						int prefixLength;
						std::string bannedName;
						soci::indicator nameInd = soci::i_null;
						std::string bannedCertHash;
						soci::indicator certInd = soci::i_null;
						std::string reason;
						soci::indicator reasonInd = soci::i_null;
						long long startDate       = 0;
						int duration              = 0;

						assert(fromSchemeVersion < 4 ? row.size() == 3 : row.size() == 8);
						assert(row.get_properties(0).get_data_type() == soci::dt_integer);
						assert(row.get_indicator(0) == soci::i_ok);
						assert(row.get_properties(1).get_data_type() == soci::dt_string);
						assert(row.get_indicator(1) == soci::i_ok);
						assert(row.get_properties(2).get_data_type() == soci::dt_integer);
						assert(row.get_indicator(2) == soci::i_ok);

						serverID     = row.get< int >(0);
						baseAddress  = row.get< std::string >(1);
						prefixLength = row.get< int >(2);

						if (fromSchemeVersion < 4) {
							bool success                       = false;
							std::array< std::uint8_t, 4 > ipv4 = DBBan::toByteArray< 4 >(baseAddress, &success);

							if (!success) {
								throw ::mdb::MigrationException(
									"Encountered invalid hex-representation of IPv4 address: '" + baseAddress + "'");
							}

							// The IPv4 was stored in the host's byte order instead of network byte order
							baseAddress = DBBan::ipv6ToString(DBBan::ipv4ToIpv6(ipv4, true));

							prefixLength = DBBan::subNetMaskToPrefixLength(static_cast< std::uint8_t >(prefixLength));
						} else {
							assert(row.get_properties(3).get_data_type() == soci::dt_string);
							assert(row.get_properties(4).get_data_type() == soci::dt_string);
							assert(row.get_properties(5).get_data_type() == soci::dt_string);
							assert(row.get_properties(6).get_data_type() == soci::dt_integer);
							assert(row.get_indicator(6) == soci::i_ok);
							assert(row.get_properties(7).get_data_type() == soci::dt_integer);
							assert(row.get_indicator(7) == soci::i_ok);

							bool success          = false;
							DBBan::ipv6_type ipv6 = DBBan::toByteArray< 16 >(baseAddress, &success);

							if (!success) {
								throw ::mdb::MigrationException(
									"Encountered invalid hex-representation of IPv6 address: '" + baseAddress + "'");
							}

							baseAddress = DBBan::ipv6ToString(ipv6);

							if (row.get_indicator(3) == soci::i_ok) {
								bannedName = row.get< std::string >(3);
								nameInd    = soci::i_ok;
							}
							if (row.get_indicator(4) == soci::i_ok) {
								bannedCertHash = row.get< std::string >(4);
								certInd        = soci::i_ok;
							}
							if (row.get_indicator(5) == soci::i_ok) {
								reason    = row.get< std::string >(5);
								reasonInd = soci::i_ok;
							}

							startDate = row.get< long long >(6);
							duration  = row.get< int >(7);
						}

						insertStmt.exchange(soci::use(serverID));
						insertStmt.exchange(soci::use(baseAddress));
						insertStmt.exchange(soci::use(prefixLength));
						insertStmt.exchange(soci::use(bannedName, nameInd));
						insertStmt.exchange(soci::use(bannedCertHash, certInd));
						insertStmt.exchange(soci::use(reason, reasonInd));
						insertStmt.exchange(soci::use(startDate));
						insertStmt.exchange(soci::use(duration));

						insertStmt.define_and_bind();
						insertStmt.execute(true);
						insertStmt.bind_clean_up();
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
