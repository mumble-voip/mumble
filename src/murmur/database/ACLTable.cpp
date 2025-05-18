// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ACLTable.h"
#include "ACLCompat.h"
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
#include "database/TransactionHolder.h"
#include "database/Trigger.h"
#include "database/Utils.h"

#include <soci/soci.h>

#include <cassert>
#include <exception>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

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
		constexpr const char *ACLTable::column::aff_meta_group_id;
		constexpr const char *ACLTable::column::access_token;
		constexpr const char *ACLTable::column::group_modifiers;
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

			::mdb::Column affectedMetaGroupCol(column::aff_meta_group_id, ::mdb::DataType(::mdb::DataType::Integer));
			affectedMetaGroupCol.setDefaultValue("NULL");

			::mdb::Column accessTokenCol(column::access_token, ::mdb::DataType(::mdb::DataType::VarChar, 255));
			accessTokenCol.setDefaultValue("NULL");

			::mdb::Column groupModifiersCol(column::group_modifiers, ::mdb::DataType(::mdb::DataType::VarChar, 31));
			groupModifiersCol.setDefaultValue("NULL");

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


			setColumns({ serverCol, channelIDCol, priorityCol, affectedUserCol, affectedGroupCol, affectedMetaGroupCol,
						 accessTokenCol, groupModifiersCol, applyCurCol, applySubCol, grantedCol, revokedCol });


			::mdb::PrimaryKey pk(std::vector< std::string >{ column::server_id, column::channel_id, column::priority });
			setPrimaryKey(pk);

			::mdb::ForeignKey fk1(channelTable, { serverCol, channelIDCol });
			addForeignKey(fk1);

			::mdb::ForeignKey fk2(userTable, { serverCol, affectedUserCol });
			addForeignKey(fk2);

			::mdb::ForeignKey fk3(groupTable, { serverCol, affectedGroupCol });
			addForeignKey(fk3);
		}

		std::string formatGroupModifiers(const std::vector< std::string > &modifiers) {
			std::string modifierString;
			for (std::size_t i = 0; i < modifiers.size(); ++i) {
				if (i > 0) {
					// Separate individual group modifiers by a semicolon
					modifierString += ";";
				}

				// Assert that the modifier itself does not contain a semicolon
				assert(std::find(modifiers[i].begin(), modifiers[i].end(), ';') == modifiers[i].end());

				modifierString += modifiers[i];
			}
			return modifierString;
		}

		void ACLTable::addACL(const DBAcl &acl) {
			if (!acl.affectedGroupID && !acl.affectedUserID && !acl.affectedMetaGroup && !acl.accessToken) {
				throw ::mdb::FormatException(
					"Can't add ACL that doesn't affect anyone (neither group nor user, nor access token)");
			}

			try {
				unsigned int userID      = 0;
				unsigned int groupID     = 0;
				unsigned int metaGroupID = 0;
				std::string accessToken;
				std::string groupModifiers;
				soci::indicator userInd        = soci::i_null;
				soci::indicator groupInd       = soci::i_null;
				soci::indicator metaGroupInd   = soci::i_null;
				soci::indicator accessTokenInd = soci::i_null;
				soci::indicator groupModInd    = soci::i_null;

				if (acl.affectedUserID) {
					userInd = soci::i_ok;
					userID  = acl.affectedUserID.value();
				}
				if (acl.affectedGroupID) {
					groupInd = soci::i_ok;
					groupID  = acl.affectedGroupID.value();
				}
				if (acl.affectedMetaGroup) {
					metaGroupInd = soci::i_ok;
					metaGroupID  = static_cast< unsigned int >(acl.affectedMetaGroup.value());
				}
				if (acl.accessToken) {
					accessTokenInd = soci::i_ok;
					accessToken    = acl.accessToken.value();
				}

				if (!acl.groupModifiers.empty()) {
					groupModifiers = formatGroupModifiers(acl.groupModifiers);

					groupModInd = soci::i_ok;
				}

				::mdb::TransactionHolder transaction = ensureTransaction();

				short applyInCurrentChannel = acl.applyInCurrentChannel;
				short applyInSubChannels    = acl.applyInSubChannels;

				m_sql << "INSERT INTO \"" << NAME << "\" (\"" << column::server_id << "\", \"" << column::channel_id
					  << "\", \"" << column::priority << "\", \"" << column::aff_user_id << "\", \""
					  << column::aff_group_id << "\", \"" << column::aff_meta_group_id << "\", \""
					  << column::access_token << "\", \"" << column::group_modifiers << "\", \""
					  << column::apply_in_current << "\", \"" << column::apply_in_sub << "\", \""
					  << column::granted_flags << "\", \"" << column::revoked_flags
					  << "\") VALUES (:serverID, :channelID, :prio, :affectedUserID, :affectedGroupID, "
						 ":affectedMetaGroupID, :accessToken, :groupModifiers, :applyCurrent, "
						 ":applySub, :granted, :revoked)",
					soci::use(acl.serverID), soci::use(acl.channelID), soci::use(acl.priority),
					soci::use(userID, userInd), soci::use(groupID, groupInd), soci::use(metaGroupID, metaGroupInd),
					soci::use(accessToken, accessTokenInd), soci::use(groupModifiers, groupModInd),
					soci::use(applyInCurrentChannel), soci::use(applyInSubChannels),
					soci::use(acl.grantedPrivilegeFlags), soci::use(acl.revokedPrivilegeFlags);

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
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE \"" << column::server_id << "\" = :serverID AND \""
					  << column::channel_id << "\" = :channelID AND \"" << column::priority << "\" = :prio",
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

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "SELECT 1 FROM \"" << NAME << "\" WHERE \"" << column::server_id << "\" = :serverID AND \""
					  << column::channel_id << "\" = :channelID AND \"" << column::priority << "\" = :prio LIMIT 1",
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
				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "DELETE FROM \"" << NAME << "\" WHERE \"" << column::server_id << "\" = :serverID AND \""
					  << column::channel_id << "\" = :channelID",
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

				::mdb::TransactionHolder transaction = ensureTransaction();

				soci::statement stmt =
					(m_sql.prepare << "SELECT \"" << column::priority << "\", \"" << column::aff_user_id << "\", \""
								   << column::aff_group_id << "\", \"" << column::aff_meta_group_id << "\", \""
								   << column::access_token << "\", \"" << column::group_modifiers << "\", \""
								   << column::apply_in_current << "\", \"" << column::apply_in_sub << "\", \""
								   << column::granted_flags << "\", \"" << column::revoked_flags << "\" FROM \"" << NAME
								   << "\" WHERE \"" << column::server_id << "\" = :serverID AND \""
								   << column::channel_id << "\" = :channelID ORDER BY \"" << column::priority << "\"",
					 soci::use(serverID), soci::use(channelID), soci::into(row));

				stmt.execute(false);

				while (stmt.fetch()) {
					assert(row.size() == 10);
					assert(row.get_properties(0).get_data_type() == soci::dt_integer);
					assert(row.get_properties(1).get_data_type() == soci::dt_integer);
					assert(row.get_properties(2).get_data_type() == soci::dt_integer);
					assert(row.get_properties(3).get_data_type() == soci::dt_integer);
					assert(row.get_properties(4).get_data_type() == soci::dt_string);
					assert(row.get_properties(5).get_data_type() == soci::dt_string);
					assert(row.get_properties(6).get_data_type() == soci::dt_integer);
					assert(row.get_properties(7).get_data_type() == soci::dt_integer);
					assert(row.get_properties(8).get_data_type() == soci::dt_integer);
					assert(row.get_properties(9).get_data_type() == soci::dt_integer);

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
					if (row.get_indicator(3) == soci::i_ok) {
						int metaGroup = row.get< int >(3);
						bool isValid  = false;
						// We use a switch without default case to get a compiler warning if a new entry is added to the
						// MetaGroup enum
						switch (static_cast< DBAcl::MetaGroup >(metaGroup)) {
							case DBAcl::MetaGroup::None:
							case DBAcl::MetaGroup::All:
							case DBAcl::MetaGroup::Auth:
							case DBAcl::MetaGroup::Strong:
							case DBAcl::MetaGroup::In:
							case DBAcl::MetaGroup::Out:
							case DBAcl::MetaGroup::Sub:
								isValid = true;
								break;
						}
						assert(isValid);
						if (!isValid) {
							// Forget about this ACL
							continue;
						}
						acl.affectedMetaGroup = static_cast< DBAcl::MetaGroup >(metaGroup);
					}
					if (row.get_indicator(4) == soci::i_ok) {
						acl.accessToken = row.get< std::string >(4);
					}
					if (row.get_indicator(5) == soci::i_ok) {
						std::string modifiers = row.get< std::string >(5);
						std::stringstream stream(modifiers);
						std::string modifier;

						// Extract the semicolon-delimited individual modifiers again
						while (std::getline(stream, modifier, ';')) {
							acl.groupModifiers.push_back(std::move(modifier));
							modifier.clear();
						}
					}
					acl.applyInCurrentChannel = row.get< int >(6);
					acl.applyInSubChannels    = row.get< int >(7);
					acl.grantedPrivilegeFlags = static_cast< unsigned int >(row.get< int >(8));
					acl.revokedPrivilegeFlags = static_cast< unsigned int >(row.get< int >(9));

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

				::mdb::TransactionHolder transaction = ensureTransaction();

				m_sql << "SELECT COUNT(*) FROM (SELECT 1 FROM \"" << NAME << "\" WHERE \"" << column::server_id
					  << "\" = :serverID) AS dummy",
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
			assert(fromSchemeVersion <= toSchemeVersion);

			try {
				if (fromSchemeVersion < 10) {
					// In v10 we renamed the table from "acl" to "access_control_lists" and the following columns have
					// been renamed as well:
					// "user_id" -> "affected_user_id"
					// "group_name" -> "affected_group_id" (Note that we also replaced using a group's name with
					// 		identifying groups by their ID instead)
					// 	"apply_here" -> "apply_in_current_channel"
					// 	"apply_sub" -> "apply_in_sub_channels"
					// 	"grantpriv" -> "granted_privilege_flags"
					// 	"revokepriv" -> "revoked_privilege_flags"

					soci::row row;
					soci::statement stmt = (m_sql.prepare << "SELECT \"server_id\", \"channel_id\", \"priority\", "
															 "\"user_id\", \"group_name\", \"apply_here\", "
															 "\"apply_sub\", \"grantpriv\", \"revokepriv\" FROM \"acl"
														  << ::mdb::Database::OLD_TABLE_SUFFIX << "\"",
											soci::into(row));

					stmt.execute(false);

					while (stmt.fetch()) {
						assert(row.size() == 9);
						assert(row.get_properties(0).get_data_type() == soci::dt_integer);
						assert(row.get_properties(1).get_data_type() == soci::dt_integer);
						assert(row.get_properties(2).get_data_type() == soci::dt_integer);
						assert(row.get_properties(3).get_data_type() == soci::dt_integer);
						assert(row.get_properties(4).get_data_type() == soci::dt_string);
						assert(row.get_properties(5).get_data_type() == soci::dt_integer);
						assert(row.get_properties(6).get_data_type() == soci::dt_integer);
						assert(row.get_properties(7).get_data_type() == soci::dt_integer);
						assert(row.get_properties(8).get_data_type() == soci::dt_integer);

						int serverID, channelID, priority, userID, applyHere, applySub, grantPriv, revokePriv;
						std::string groupName;
						soci::indicator userInd;

						serverID  = row.get< int >(0);
						channelID = row.get< int >(1);
						priority  = row.get< int >(2);
						if (row.get_indicator(3) != soci::i_null) {
							userID  = row.get< int >(3);
							userInd = soci::i_ok;
						} else {
							userInd = soci::i_null;
						}
						if (row.get_indicator(4) != soci::i_null) {
							groupName = row.get< std::string >(4);
						}

						applyHere  = row.get_indicator(5) != soci::i_null ? row.get< int >(5) == 1 : true;
						applySub   = row.get_indicator(6) != soci::i_null ? row.get< int >(6) == 1 : true;
						grantPriv  = row.get_indicator(7) != soci::i_null ? row.get< int >(7) : 0;
						revokePriv = row.get_indicator(8) != soci::i_null ? row.get< int >(8) : 0;

						int groupID = 0, metaGroupID = 0;
						std::string accessToken, groupModifiers;
						soci::indicator groupInd, metaGroupInd, accessTokenInd, groupModInd;
						groupInd = metaGroupInd = accessTokenInd = groupModInd = soci::i_null;
						if (!groupName.empty()) {
							LegacyGroupData data = parseLegacyGroupSpecification(groupName);

							if (data.isAccessToken) {
								if (data.name.empty()) {
									throw ::mdb::MigrationException(
										"Invalid access token (empty) encountered (original entry is '" + groupName
										+ "')");
								}
								accessToken    = std::move(data.name);
								accessTokenInd = soci::i_ok;
							} else {
								std::optional< DBAcl::MetaGroup > metaGroup = parseMetaGroup(data.name);

								if (metaGroup) {
									metaGroupID  = static_cast< int >(metaGroup.value());
									metaGroupInd = soci::i_ok;
								} else {
									// This is a proper group -> look up its ID
									m_sql << "SELECT \"" << GroupTable::column::group_id << "\" FROM \""
										  << GroupTable::NAME << "\" WHERE \"" << GroupTable::column::server_id
										  << "\" = :serverID AND \"" << GroupTable::column::group_name
										  << "\" = :groupName",
										soci::use(serverID), soci::use(data.name), soci::into(groupID);

									if (!m_sql.got_data()) {
										throw ::mdb::MigrationException("Existing ACL references non-existing group '"
																		+ data.name + "'");
									}

									groupInd = soci::i_ok;
								}
							}

							if (!data.modifiers.empty()) {
								groupModifiers = formatGroupModifiers(data.modifiers);
								groupModInd    = soci::i_ok;
							}
						}

						if (userInd == soci::i_null && groupInd == soci::i_null && metaGroupInd == soci::i_null
							&& accessTokenInd == soci::i_null) {
							// This ACL does not apply to anyone
							throw ::mdb::MigrationException("On server " + std::to_string(serverID) + ", channel "
															+ std::to_string(channelID)
															+ ": ACL does not apply to anyone");
						}
						if (grantPriv == 0 && revokePriv == 0) {
							// This ACL doesn't do anything
							throw ::mdb::MigrationException("On server " + std::to_string(serverID) + ", channel "
															+ std::to_string(channelID)
															+ ": ACL does not have any effect on permissions");
						}

						m_sql << "INSERT INTO \"" << NAME << "\" (\"" << column::server_id << "\", \""
							  << column::channel_id << "\", \"" << column::priority << "\", \"" << column::aff_user_id
							  << "\", \"" << column::aff_group_id << "\", \"" << column::aff_meta_group_id << "\", \""
							  << column::access_token << "\", \"" << column::group_modifiers << "\", \""
							  << column::apply_in_current << "\", \"" << column::apply_in_sub << "\", \""
							  << column::granted_flags << "\", \"" << column::revoked_flags
							  << "\") VALUES (:serverID, :channelID, :priority, :userID, :groupID, :metaGroupID, "
								 ":accessToken, :groupModifier, :applyCurrent, :applySub, :granted, :revoked)",
							soci::use(serverID), soci::use(channelID), soci::use(priority), soci::use(userID, userInd),
							soci::use(groupID, groupInd), soci::use(metaGroupID, metaGroupInd),
							soci::use(accessToken, accessTokenInd), soci::use(groupModifiers, groupModInd),
							soci::use(applyHere), soci::use(applySub), soci::use(grantPriv), soci::use(revokePriv);
					}
				} else {
					// Use default implementation to handle migration without change of format
					mdb::Table::migrate(fromSchemeVersion, toSchemeVersion);
				}
			} catch (const soci::soci_error &) {
				std::throw_with_nested(::mdb::MigrationException(
					std::string("Failed at migrating table \"") + NAME + "\" from scheme version "
					+ std::to_string(fromSchemeVersion) + " to " + std::to_string(toSchemeVersion)));
			} catch (const ::mdb::MigrationException &) {
				std::throw_with_nested(::mdb::MigrationException(
					std::string("Failed at migrating table \"") + NAME + "\" from scheme version "
					+ std::to_string(fromSchemeVersion) + " to " + std::to_string(toSchemeVersion)));
			}
		}

	} // namespace db
} // namespace server
} // namespace mumble
