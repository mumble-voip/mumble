// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBWrapper.h"
#include "ACL.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ExceptionUtils.h"
#include "Group.h"
#include "LegacyPasswordHash.h"
#include "Meta.h"
#include "MumbleConstants.h"
#include "PBKDF2.h"
#include "PasswordGenerator.h"
#include "Server.h"
#include "ServerUserInfo.h"
#include "VolumeAdjustment.h"

#include "database/Exception.h"
#include "database/FormatException.h"
#include "database/MetaTable.h"
#include "database/NoDataException.h"
#include "database/TransactionHolder.h"

#include "murmur/database/ACLCompat.h"
#include "murmur/database/ACLTable.h"
#include "murmur/database/BanTable.h"
#include "murmur/database/ChannelLinkTable.h"
#include "murmur/database/ChannelListenerTable.h"
#include "murmur/database/ChannelProperty.h"
#include "murmur/database/ChannelPropertyTable.h"
#include "murmur/database/ChannelTable.h"
#include "murmur/database/ChronoUtils.h"
#include "murmur/database/ConfigTable.h"
#include "murmur/database/DBAcl.h"
#include "murmur/database/DBBan.h"
#include "murmur/database/DBChannel.h"
#include "murmur/database/DBChannelLink.h"
#include "murmur/database/DBChannelListener.h"
#include "murmur/database/DBGroup.h"
#include "murmur/database/DBGroupMember.h"
#include "murmur/database/DBLogEntry.h"
#include "murmur/database/DBUser.h"
#include "murmur/database/GroupMemberTable.h"
#include "murmur/database/GroupTable.h"
#include "murmur/database/LogTable.h"
#include "murmur/database/ServerTable.h"
#include "murmur/database/UserPropertyTable.h"
#include "murmur/database/UserTable.h"

#include <nlohmann/json.hpp>

#include <cassert>
#include <limits>
#include <optional>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

#include <QDateTime>
#include <QString>

namespace mdb  = ::mumble::db;
namespace msdb = ::mumble::server::db;

DBWrapper::DBWrapper(const ::mdb::ConnectionParameter &connectionParams)
	: m_serverDB(connectionParams.applicability()) {
	// Immediately initialize the database connection
	m_serverDB.init(connectionParams);
}

/*
 * In many places in our code IDs are represented as signed integers in order to be able to represent an invalid ID
 * by a negative value. If such a value is converted to an unsigned integer, it is mapped to some value bigger than
 * the biggest positive value that can be represented with a signed integer. In order to catch cases in which such a
 * conversion happened, we perform below assertion. And even if at some point the signed integer representation gets
 * replaced by e.g. an optional< unsigned int >, producing IDs that are larger than INT_MAX is likely an error of
 * some sort as an int should (at least) have 2^32 possible positive values, which is expected to be far more than
 * enough for any valid use case.
 *
 * In case we are asserting on a signed type, we simply have to check that the value is not negative
 *
 * Note: We treat id == 0 as a separate branch instead of checking id >= 0 in order to prevent overly smart compilers
 * from warning us that this is always true for unsigned types.
 */
#define assertValidID(id) assert(id == 0 || (id > 0 && id <= std::numeric_limits< int >::max()));

/**
 * Asserts that the given userID represents a valid ID and belongs to a user that is actually registered on the given
 * server.
 */
#define assertRegisteredUserExists(serverID, userID) \
	assertValidID(userID);                           \
	assert(registeredUserExists(serverID, static_cast< unsigned int >(userID)));

#define WRAPPER_BEGIN                                 \
	assert(std::this_thread::get_id() == m_threadID); \
	try {
// Our error handling consists in properly printing the encountered error and then throwing
// a standard std::exception that should be caught in our QCoreApplication's notify function,
// which we have overridden to exit all event processing and thereby shutting down all servers.
#define WRAPPER_END                                                       \
	}                                                                     \
	catch (const ::mdb::Exception &e) {                                   \
		std::cerr << "[ERROR]: Encountered database error:" << std::endl; \
		mumble::printExceptionMessage(std::cerr, e, 1);                   \
		std::cerr << std::endl;                                           \
                                                                          \
		throw std::runtime_error("Database error");                       \
	}

std::vector< unsigned int > DBWrapper::getAllServers() {
	WRAPPER_BEGIN

	return m_serverDB.getServerTable().getAllServerIDs();

	WRAPPER_END
}

std::vector< unsigned int > DBWrapper::getBootServers() {
	WRAPPER_BEGIN

	std::vector< unsigned int > bootIDs;

	for (unsigned int id : m_serverDB.getServerTable().getAllServerIDs()) {
		bool boot = true;
		getConfigurationTo(id, "boot", boot);

		if (boot) {
			bootIDs.push_back(id);
		}
	}

	return bootIDs;

	WRAPPER_END
}

unsigned int DBWrapper::addServer() {
	WRAPPER_BEGIN

	::mdb::TransactionHolder transaction = m_serverDB.ensureTransaction();

	unsigned int serverID = m_serverDB.getServerTable().getFreeServerID();

	m_serverDB.getServerTable().addServer(serverID);

	// Ensure that the root channel exists
	::msdb::DBChannel rootChannel;
	rootChannel.serverID  = serverID;
	rootChannel.channelID = Mumble::ROOT_CHANNEL_ID;
	rootChannel.name      = "Root";
	createChannel(rootChannel);

	// Ensure that a SuperUser entry exists
	::msdb::DBUser superUser(serverID, Mumble::SUPERUSER_ID);
	m_serverDB.getUserTable().addUser(superUser, "SuperUser");

	// Generate a new, default password for the SuperUser
	constexpr const int pwSize = 32;
	std::string pw             = PasswordGenerator::generatePassword(pwSize).toStdString();
	setSuperUserPassword(serverID, pw);

	// Write the default password into the DB, in case it needs to be fetched at a later point
	logMessage(serverID, "Initialized 'SuperUser' password on server " + std::to_string(serverID) + " to '" + pw + "'");

	// Add server-wide admin group
	::msdb::DBGroup adminGroup;
	adminGroup.serverID       = serverID;
	adminGroup.groupID        = m_serverDB.getGroupTable().getFreeGroupID(serverID);
	adminGroup.channelID      = rootChannel.channelID;
	adminGroup.is_inheritable = true;
	adminGroup.inherit        = true;
	adminGroup.name           = "admin";

	m_serverDB.getGroupTable().addGroup(adminGroup);

	// Setup default ACLs
	::msdb::DBAcl acl;
	acl.serverID              = serverID;
	acl.applyInCurrentChannel = true;
	acl.applyInSubChannels    = true;
	acl.channelID             = Mumble::ROOT_CHANNEL_ID;
	acl.priority              = 1;

	// Add ACL granting "Write" privilege to server admin
	acl.affectedGroupID       = adminGroup.groupID;
	acl.grantedPrivilegeFlags = ChanACL::Write;
	m_serverDB.getACLTable().addACL(acl);

	// Add ACL granting privilege to create temp channels to authenticated users
	acl.priority++;
	acl.affectedGroupID.reset();
	acl.affectedMetaGroup     = ::msdb::DBAcl::MetaGroup::Auth;
	acl.grantedPrivilegeFlags = ChanACL::MakeTempChannel;
	m_serverDB.getACLTable().addACL(acl);

	// Add ACL granting everyone the possibility to self-register
	acl.priority++;
	acl.affectedMetaGroup     = ::msdb::DBAcl::MetaGroup::All;
	acl.grantedPrivilegeFlags = ChanACL::SelfRegister;
	m_serverDB.getACLTable().addACL(acl);

	transaction.commit();

	return serverID;

	WRAPPER_END
}

void DBWrapper::removeServer(unsigned int serverID) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	m_serverDB.getServerTable().removeServer(serverID);

	WRAPPER_END
}

bool DBWrapper::serverExists(unsigned int serverID) {
	WRAPPER_BEGIN

	return m_serverDB.getServerTable().serverExists(serverID);

	WRAPPER_END
}

void DBWrapper::setServerBootProperty(unsigned int serverID, bool boot) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	m_serverDB.getConfigTable().setConfig(serverID, "boot", std::to_string(boot));

	WRAPPER_END
}

std::optional< unsigned int > DBWrapper::loadPBKDF2IterationCount() {
	WRAPPER_BEGIN

	std::optional< std::string > strIterations = m_serverDB.getMetaTable().queryKey("pbkdf2_iterations");

	if (!strIterations) {
		return std::nullopt;
	}

	try {
		return static_cast< unsigned int >(std::stoi(strIterations.value()));
	} catch (const std::invalid_argument &) {
		// Conversion to number failed
		return std::nullopt;
	}

	WRAPPER_END
}

void DBWrapper::storePBKDF2IterationCount(unsigned int count) {
	WRAPPER_BEGIN

	m_serverDB.getMetaTable().setKey("pbkdf2_iterations", std::to_string(count));

	WRAPPER_END
}

void DBWrapper::setSuperUserPassword(unsigned int serverID, const std::string &password) {
	assertValidID(serverID);

	storeRegisteredUserPassword(serverID, Mumble::SUPERUSER_ID, password);
}

void DBWrapper::disableSuperUser(unsigned int serverID) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	const ::msdb::DBUser superUser(serverID, Mumble::SUPERUSER_ID);

	m_serverDB.getUserTable().clearPassword(superUser);

	WRAPPER_END
}

void DBWrapper::clearAllPerServerSLLConfigurations() {
	WRAPPER_BEGIN

	for (unsigned int serverID : getAllServers()) {
		for (const std::string &currentKey :
			 std::vector< std::string >{ "key", "certificate", "passphrase", "sslDHParams" }) {
			clearConfiguration(serverID, currentKey);
		}
	}

	WRAPPER_END
}

void DBWrapper::clearAllServerLogs() {
	WRAPPER_BEGIN

	for (unsigned int serverID : getAllServers()) {
		m_serverDB.getLogTable().clearLog(serverID);
	}

	WRAPPER_END
}

std::vector< Ban > DBWrapper::getBans(unsigned int serverID) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::vector< Ban > bans;

	for (const ::msdb::DBBan &currentBan : m_serverDB.getBanTable().getAllBans(serverID)) {
		assert(currentBan.serverID == serverID);

		Ban ban;

		ban.iDuration = static_cast< unsigned int >(currentBan.duration.count());
		ban.iMask     = currentBan.prefixLength;
		ban.qdtStart =
			QDateTime::fromSecsSinceEpoch(static_cast< qint64 >(::msdb::toEpochSeconds(currentBan.startDate)));
		ban.haAddress = HostAddress(currentBan.baseAddress);
		if (currentBan.reason) {
			ban.qsReason = QString::fromStdString(currentBan.reason.value());
		}
		if (currentBan.bannedUserCertHash) {
			ban.qsHash = QString::fromStdString(currentBan.bannedUserCertHash.value());
		}
		if (currentBan.bannedUserName) {
			ban.qsUsername = QString::fromStdString(currentBan.bannedUserName.value());
		}

		bans.push_back(std::move(ban));
	}

	return bans;

	WRAPPER_END
}

void DBWrapper::saveBans(unsigned int serverID, const std::vector< Ban > &bans) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::vector<::msdb::DBBan > dbBans;
	dbBans.reserve(bans.size());

	for (const Ban &currentBan : bans) {
		::msdb::DBBan dbBan;

		dbBan.serverID     = serverID;
		dbBan.duration     = std::chrono::seconds(currentBan.iDuration);
		dbBan.prefixLength = static_cast< decltype(dbBan.prefixLength) >(currentBan.iMask);
		dbBan.startDate =
			std::chrono::system_clock::time_point(std::chrono::seconds(currentBan.qdtStart.toSecsSinceEpoch()));
		dbBan.baseAddress = currentBan.haAddress.getByteRepresentation();
		if (!currentBan.qsHash.isEmpty()) {
			dbBan.bannedUserCertHash = currentBan.qsHash.toStdString();
		}
		if (!currentBan.qsUsername.isEmpty()) {
			dbBan.bannedUserName = currentBan.qsUsername.toStdString();
		}
		if (!currentBan.qsReason.isEmpty()) {
			dbBan.reason = currentBan.qsReason.toStdString();
		}

		dbBans.push_back(std::move(dbBan));
	}

	m_serverDB.getBanTable().setBans(serverID, dbBans);

	WRAPPER_END
}

void readChildren(::msdb::ServerDatabase &db, Channel *parent, Server &server) {
	for (unsigned int currentChildID : db.getChannelTable().getChildrenOf(server.iServerNum, parent->iId)) {
		::msdb::DBChannel channelInfo = db.getChannelTable().getChannelData(server.iServerNum, currentChildID);

		Channel *currentChild     = new Channel(currentChildID, QString::fromStdString(channelInfo.name), parent);
		currentChild->bInheritACL = channelInfo.inheritACL;

		server.qhChannels.insert(currentChildID, currentChild);

		// Recurse
		readChildren(db, currentChild, server);
	}
}

void DBWrapper::initializeChannels(Server &server) {
	WRAPPER_BEGIN

	::msdb::DBChannel root = m_serverDB.getChannelTable().getChannelData(server.iServerNum, Mumble::ROOT_CHANNEL_ID);

	Channel *rootChannel     = new Channel(Mumble::ROOT_CHANNEL_ID, QString::fromStdString(root.name), &server);
	rootChannel->bInheritACL = root.inheritACL;

	server.qhChannels.insert(rootChannel->iId, rootChannel);

	readChildren(m_serverDB, rootChannel, server);

	initializeChannelDetails(server);

	WRAPPER_END
}

void DBWrapper::initializeChannelDetails(Server &server) {
	WRAPPER_BEGIN

	for (Channel *currentChannel : server.qhChannels) {
		assert(currentChannel);

		// Read and set channel properties
		std::string description = m_serverDB.getChannelPropertyTable().getProperty< std::string, false >(
			server.iServerNum, currentChannel->iId, ::msdb::ChannelProperty::Description);
		if (!description.empty()) {
			Server::hashAssign(currentChannel->qsDesc, currentChannel->qbaDescHash,
							   QString::fromStdString(description));
		}

		currentChannel->iPosition = m_serverDB.getChannelPropertyTable().getProperty< int, false >(
			server.iServerNum, currentChannel->iId, ::msdb::ChannelProperty::Position);

		currentChannel->uiMaxUsers = m_serverDB.getChannelPropertyTable().getProperty< unsigned int, false >(
			server.iServerNum, currentChannel->iId, ::msdb::ChannelProperty::MaxUsers);


		// Read and initialize the groups defined for the current channel
		for (const ::msdb::DBGroup &currentGroup :
			 m_serverDB.getGroupTable().getAllGroups(server.iServerNum, currentChannel->iId)) {
			Group *group        = new Group(currentChannel, QString::fromStdString(currentGroup.name));
			group->bInherit     = currentGroup.inherit;
			group->bInheritable = currentGroup.is_inheritable;

			for (const ::msdb::DBGroupMember &currrentMember :
				 m_serverDB.getGroupMemberTable().getEntries(server.iServerNum, currentGroup.groupID)) {
				if (currrentMember.addToGroup) {
					group->qsAdd << static_cast< int >(currrentMember.userID);
				} else {
					group->qsRemove << static_cast< int >(currrentMember.userID);
				}
			}
		}

		// Read and set access control lists
		for (const ::msdb::DBAcl &currentAcl :
			 m_serverDB.getACLTable().getAllACLs(server.iServerNum, currentChannel->iId)) {
			ChanACL *acl = new ChanACL(currentChannel);
			acl->iUserId = currentAcl.affectedUserID ? static_cast< int >(currentAcl.affectedUserID.value()) : -1;
			acl->qsGroup = QString::fromStdString(::msdb::getLegacyGroupData(currentAcl, m_serverDB.getGroupTable()));

			acl->bApplyHere = currentAcl.applyInCurrentChannel;
			acl->bApplySubs = currentAcl.applyInSubChannels;
			acl->pAllow     = static_cast< ChanACL::Permissions >(currentAcl.grantedPrivilegeFlags);
			acl->pDeny      = static_cast< ChanACL::Permissions >(currentAcl.revokedPrivilegeFlags);
		}
	}

	WRAPPER_END
}

void DBWrapper::initializeChannelLinks(Server &server) {
	WRAPPER_BEGIN

	for (const ::msdb::DBChannelLink &currentLink : m_serverDB.getChannelLinkTable().getAllLinks(server.iServerNum)) {
		Channel *first  = server.qhChannels.value(currentLink.firstChannelID);
		Channel *second = server.qhChannels.value(currentLink.secondChannelID);

		if (first && second) {
			// Linking A to B will automatically link B to A as well
			first->link(second);
		}
	}

	WRAPPER_END
}

unsigned int DBWrapper::getNextAvailableChannelID(unsigned int serverID) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	return m_serverDB.getChannelTable().getFreeChannelID(serverID);

	WRAPPER_END
}

::msdb::DBChannel channelToDB(unsigned int serverID, const Channel &channel) {
	::msdb::DBChannel dbChannel;
	dbChannel.serverID   = serverID;
	dbChannel.channelID  = channel.iId;
	dbChannel.name       = channel.qsName.toStdString();
	dbChannel.parentID   = channel.cParent ? channel.cParent->iId : channel.iId;
	dbChannel.inheritACL = channel.bInheritACL;

	return dbChannel;
}

::msdb::DBGroup groupToDB(unsigned int serverID, unsigned int groupID, const Group &group) {
	assert(group.c);

	::msdb::DBGroup dbGroup;
	dbGroup.serverID       = serverID;
	dbGroup.channelID      = group.c->iId;
	dbGroup.groupID        = groupID;
	dbGroup.inherit        = group.bInherit;
	dbGroup.is_inheritable = group.bInheritable;
	dbGroup.name           = group.qsName.toStdString();

	// Assert that we are not trying to add a meta group to the DB (those shouldn't be Group objects to begin with)
	assert(!::msdb::parseMetaGroup(::msdb::parseLegacyGroupSpecification(dbGroup.name).name).has_value());

	return dbGroup;
}

::msdb::DBAcl aclToDB(unsigned int serverID, unsigned int priority, std::optional< unsigned int > groupID,
					  std::optional<::msdb::DBAcl::MetaGroup > metaGroup, std::optional< std::string > accessToken,
					  std::vector< std::string > groupModifiers, const ChanACL &acl) {
	::msdb::DBAcl dbAcl;
	assert(acl.c);

	dbAcl.serverID              = serverID;
	dbAcl.channelID             = acl.c->iId;
	dbAcl.priority              = priority;
	dbAcl.applyInCurrentChannel = acl.bApplyHere;
	dbAcl.applyInSubChannels    = acl.bApplySubs;
	dbAcl.affectedGroupID       = std::move(groupID);
	dbAcl.affectedMetaGroup     = std::move(metaGroup);
	dbAcl.accessToken           = std::move(accessToken);
	dbAcl.groupModifiers        = std::move(groupModifiers);
	if (acl.iUserId >= 0) {
		dbAcl.affectedUserID = acl.iUserId;
	}
	dbAcl.grantedPrivilegeFlags = static_cast< unsigned int >(acl.pAllow);
	dbAcl.revokedPrivilegeFlags = static_cast< unsigned int >(acl.pDeny);

	return dbAcl;
}

void DBWrapper::createChannel(unsigned int serverID, const Channel &channel) {
	assertValidID(serverID);
	assertValidID(channel.iId);
	assert(!channel.bTemporary);

	createChannel(channelToDB(serverID, channel), static_cast< unsigned int >(channel.iPosition), channel.uiMaxUsers,
				  channel.qsDesc.toStdString());
}

void DBWrapper::createChannel(const ::mumble::server::db::DBChannel &channel, unsigned int position,
							  unsigned int maxUsers, const std::string &description) {
	WRAPPER_BEGIN

	assertValidID(channel.serverID);
	assertValidID(channel.channelID);

	// Add the given channel to the DB
	m_serverDB.getChannelTable().addChannel(channel);

	// Add channel properties to DB
	if (!description.empty()) {
		m_serverDB.getChannelPropertyTable().setProperty(channel.serverID, channel.channelID,
														 ::msdb::ChannelProperty::Description, description);
	} else {
		m_serverDB.getChannelPropertyTable().clearProperty(channel.serverID, channel.channelID,
														   ::msdb::ChannelProperty::Description);
	}

	m_serverDB.getChannelPropertyTable().setProperty(channel.serverID, channel.channelID,
													 ::msdb::ChannelProperty::Position, std::to_string(position));

	m_serverDB.getChannelPropertyTable().setProperty(channel.serverID, channel.channelID,
													 ::msdb::ChannelProperty::MaxUsers, std::to_string(maxUsers));

	WRAPPER_END
}

void DBWrapper::deleteChannel(unsigned int serverID, unsigned int channelID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertValidID(channelID);

	m_serverDB.getChannelTable().removeChannel(serverID, channelID);

	WRAPPER_END
}

void DBWrapper::updateChannelData(unsigned int serverID, const Channel &channel) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertValidID(channel.iId);
	assert(!channel.bTemporary);

	if (channel.bTemporary) {
		// Temporary channels by definition are not stored in the DB
		return;
	}

	// Wrap all actions in a single transaction
	::mdb::TransactionHolder transaction = m_serverDB.ensureTransaction();

	::msdb::DBChannel dbChannel = channelToDB(serverID, channel);

	// Update channel object itself
	m_serverDB.getChannelTable().updateChannel(dbChannel);

	// Update channel properties
	if (!channel.qsDesc.isEmpty()) {
		m_serverDB.getChannelPropertyTable().setProperty(serverID, channel.iId, ::msdb::ChannelProperty::Description,
														 channel.qsDesc.toStdString());
	} else {
		m_serverDB.getChannelPropertyTable().clearProperty(serverID, channel.iId, ::msdb::ChannelProperty::Description);
	}

	m_serverDB.getChannelPropertyTable().setProperty(serverID, channel.iId, ::msdb::ChannelProperty::Position,
													 std::to_string(channel.iPosition));

	m_serverDB.getChannelPropertyTable().setProperty(serverID, channel.iId, ::msdb::ChannelProperty::MaxUsers,
													 std::to_string(channel.uiMaxUsers));

	// First, clear old groups and ACLs
	// (Clearing the groups automatically clear all entries referencing that group - in particular any members of that
	// group)
	m_serverDB.getGroupTable().clearGroups(serverID, channel.iId);
	m_serverDB.getACLTable().clearACLs(serverID, channel.iId);

	// Add current groups with their member information
	for (const Group *currentGroup : channel.qhGroups) {
		assert(currentGroup);

		unsigned int groupID = m_serverDB.getGroupTable().getFreeGroupID(serverID);

		::msdb::DBGroup group = groupToDB(serverID, groupID, *currentGroup);

		m_serverDB.getGroupTable().addGroup(group);

		for (int addedGroupMemberID : currentGroup->qsAdd) {
			assert(addedGroupMemberID >= 0);
			m_serverDB.getGroupMemberTable().addEntry(serverID, groupID,
													  static_cast< unsigned int >(addedGroupMemberID), true);
		}
		for (int removeddGroupMemberID : currentGroup->qsRemove) {
			assert(removeddGroupMemberID >= 0);
			m_serverDB.getGroupMemberTable().addEntry(serverID, groupID,
													  static_cast< unsigned int >(removeddGroupMemberID), false);
		}
	}


	// Why start at 5? Because the legacy code did so! ¯\_(ツ)_/¯
	unsigned int priority = 5;

	for (const ChanACL *currentACL : channel.qlACL) {
		assert(currentACL);

		std::optional< unsigned int > associatedGroupID;
		std::optional<::msdb::DBAcl::MetaGroup > metaGroup;
		std::optional< std::string > accessToken;
		std::vector< std::string > groupModifiers;

		if (!currentACL->qsGroup.isEmpty()) {
			::msdb::LegacyGroupData data = ::msdb::parseLegacyGroupSpecification(currentACL->qsGroup.toStdString());

			groupModifiers = std::move(data.modifiers);

			if (data.isAccessToken) {
				accessToken = std::move(data.name);
			} else {
				metaGroup = ::msdb::parseMetaGroup(data.name);
			}

			if (!metaGroup && !accessToken) {
				associatedGroupID = m_serverDB.getGroupTable().findGroupID(serverID, data.name);

				if (!associatedGroupID) {
					throw ::mdb::NoDataException("Required ID of non-existing group \"" + data.name + "\"");
				}
			}
		}

		::msdb::DBAcl acl = aclToDB(serverID, priority++, std::move(associatedGroupID), std::move(metaGroup),
									std::move(accessToken), std::move(groupModifiers), *currentACL);

		m_serverDB.getACLTable().addACL(acl);
	}

	transaction.commit();

	WRAPPER_END
}

void DBWrapper::addChannelLink(unsigned int serverID, const Channel &first, const Channel &second) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertValidID(first.iId);
	assertValidID(second.iId);
	assert(!first.bTemporary);
	assert(!second.bTemporary);

	::msdb::DBChannelLink link(serverID, first.iId, second.iId);

	m_serverDB.getChannelLinkTable().addLink(link);

	WRAPPER_END
}

void DBWrapper::removeChannelLink(unsigned int serverID, const Channel &first, const Channel &second) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertValidID(first.iId);
	assertValidID(second.iId);
	assert(!first.bTemporary);
	assert(!second.bTemporary);

	::msdb::DBChannelLink link(serverID, first.iId, second.iId);

	m_serverDB.getChannelLinkTable().removeLink(link);

	WRAPPER_END
}

void DBWrapper::getConfigurationTo(unsigned int serverID, const std::string &configKey, QString &outVar) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::string property = m_serverDB.getConfigTable().getConfig(serverID, configKey);

	if (!property.empty()) {
		outVar = QString::fromStdString(property);
	}

	WRAPPER_END
}

void DBWrapper::getConfigurationTo(unsigned int serverID, const std::string &configKey, std::string &outVar) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::string property = m_serverDB.getConfigTable().getConfig(serverID, configKey);

	if (!property.empty()) {
		outVar = std::move(property);
	}

	WRAPPER_END
}

void DBWrapper::getConfigurationTo(unsigned int serverID, const std::string &configKey, QByteArray &outVar) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::string property = m_serverDB.getConfigTable().getConfig(serverID, configKey);

	if (!property.empty()) {
		outVar = QByteArray::fromStdString(property);
	}

	WRAPPER_END
}

void DBWrapper::getConfigurationTo(unsigned int serverID, const std::string &configKey, unsigned short &outVar) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::string property = m_serverDB.getConfigTable().getConfig(serverID, configKey);

	if (!property.empty()) {
		try {
			unsigned long value = std::stoul(property);

			if (value > std::numeric_limits< unsigned short >::max()) {
				throw ::mdb::FormatException("Fetched property for key \"" + configKey
											 + "\" holds a value too big for the requested output type");
			}

			outVar = static_cast< unsigned short >(value);
		} catch (const std::invalid_argument &) {
			std::throw_with_nested(
				::mdb::FormatException("Fetched property for key \"" + configKey + "\" can't be parsed as a number"));
		}
	}

	WRAPPER_END
}

bool stringToBool(const std::string &str) {
	return boost::iequals(str, "true") || str == "1";
}

void DBWrapper::getConfigurationTo(unsigned int serverID, const std::string &configKey, bool &outVar) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::string property = m_serverDB.getConfigTable().getConfig(serverID, configKey);

	if (!property.empty()) {
		outVar = stringToBool(property);
	}

	WRAPPER_END
}

void DBWrapper::getConfigurationTo(unsigned int serverID, const std::string &configKey, int &outVar) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::string property = m_serverDB.getConfigTable().getConfig(serverID, configKey);

	if (!property.empty()) {
		try {
			outVar = std::stoi(property);
		} catch (const std::invalid_argument &) {
			std::throw_with_nested(
				::mdb::FormatException("Fetched property for key \"" + configKey + "\" can't be parsed as a number"));
		}
	}

	WRAPPER_END
}

void DBWrapper::getConfigurationTo(unsigned int serverID, const std::string &configKey, unsigned int &outVar) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::string property = m_serverDB.getConfigTable().getConfig(serverID, configKey);

	if (!property.empty()) {
		try {
			unsigned long value = std::stoul(property);

			if (value > std::numeric_limits< unsigned int >::max()) {
				throw ::mdb::FormatException("Fetched property for key \"" + configKey
											 + "\" holds a value too big for the requested output type");
			}

			outVar = static_cast< unsigned int >(value);
		} catch (const std::invalid_argument &) {
			std::throw_with_nested(
				::mdb::FormatException("Fetched property for key \"" + configKey + "\" can't be parsed as a number"));
		}
	}

	WRAPPER_END
}

void DBWrapper::getConfigurationTo(unsigned int serverID, const std::string &configKey, std::optional< bool > &outVar) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::string property = m_serverDB.getConfigTable().getConfig(serverID, configKey);

	outVar = property.empty() ? std::nullopt : std::optional< bool >(stringToBool(property));

	WRAPPER_END
}

std::vector< std::pair< std::string, std::string > > DBWrapper::getAllConfigurations(unsigned int serverID) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::vector< std::pair< std::string, std::string > > configs;

	for (std::pair< std::string, std::string > currentConfig : m_serverDB.getConfigTable().getAllConfigs(serverID)) {
		configs.push_back(std::move(currentConfig));
	}

	return configs;

	WRAPPER_END
}

void DBWrapper::setConfiguration(unsigned int serverID, const std::string &configKey, const std::string &value) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	m_serverDB.getConfigTable().setConfig(serverID, configKey, value);

	WRAPPER_END
}

void DBWrapper::clearConfiguration(unsigned int serverID, const std::string &configKey) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	m_serverDB.getConfigTable().clearConfig(serverID, configKey);

	WRAPPER_END
}

void DBWrapper::logMessage(unsigned int serverID, const std::string &msg) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	::msdb::DBLogEntry entry(msg);

	m_serverDB.getLogTable().logMessage(serverID, entry);

	WRAPPER_END
}

std::vector<::msdb::DBLogEntry > DBWrapper::getLogs(unsigned int serverID, unsigned int startOffset, int amount) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	return m_serverDB.getLogTable().getLogs(
		serverID, static_cast< unsigned int >(amount >= 0 ? amount : std::numeric_limits< int >::max()), startOffset);

	WRAPPER_END
}

std::size_t DBWrapper::getLogSize(unsigned int serverID) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	return m_serverDB.getLogTable().getLogSize(serverID);

	WRAPPER_END
}

void DBWrapper::updateLastDisconnect(unsigned int serverID, unsigned int userID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);

	m_serverDB.getUserTable().setLastDisconnect(user);

	WRAPPER_END
}

void DBWrapper::addChannelListenerIfNotExists(unsigned int serverID, unsigned int userID, unsigned int channelID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);
	assertValidID(channelID);

	::msdb::DBChannelListener listener(serverID, channelID, userID);

	if (!m_serverDB.getChannelListenerTable().listenerExists(listener)) {
		m_serverDB.getChannelListenerTable().addListener(listener);
	} else {
		listener = m_serverDB.getChannelListenerTable().getListenerDetails(serverID, userID, channelID);

		if (!listener.enabled) {
			// Mark this listener as enabled again
			listener.enabled = true;
			m_serverDB.getChannelListenerTable().updateListener(listener);
		}
	}

	WRAPPER_END
}

void DBWrapper::disableChannelListenerIfExists(unsigned int serverID, unsigned int userID, unsigned int channelID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);
	assertValidID(channelID);

	::msdb::DBChannelListener listener(serverID, channelID, userID);

	if (m_serverDB.getChannelListenerTable().listenerExists(listener)) {
		listener = m_serverDB.getChannelListenerTable().getListenerDetails(listener);

		if (listener.enabled) {
			listener.enabled = false;
			m_serverDB.getChannelListenerTable().updateListener(listener);
		}
	}

	WRAPPER_END
}

void DBWrapper::deleteChannelListener(unsigned int serverID, unsigned int userID, unsigned int channelID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);
	assertValidID(channelID);

	m_serverDB.getChannelListenerTable().removeListener(serverID, userID, channelID);

	WRAPPER_END
}

void DBWrapper::loadChannelListenersOf(unsigned int serverID, const ServerUserInfo &userInfo,
									   ChannelListenerManager &manager) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userInfo.iId);

	for (const ::msdb::DBChannelListener &currentListener : m_serverDB.getChannelListenerTable().getListenersForUser(
			 serverID, static_cast< unsigned int >(userInfo.iId))) {
		if (currentListener.enabled) {
			manager.addListener(userInfo.uiSession, currentListener.channelID);
			manager.setListenerVolumeAdjustment(userInfo.uiSession, currentListener.channelID,
												VolumeAdjustment::fromFactor(currentListener.volumeAdjustment));
		}
	}

	WRAPPER_END
}

void DBWrapper::storeChannelListenerVolume(unsigned int serverID, unsigned int userID, unsigned int channelID,
										   float volumeFactor) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);
	assertValidID(channelID);
	assert(m_serverDB.getChannelListenerTable().listenerExists(serverID, userID, channelID));

	::msdb::DBChannelListener listener =
		m_serverDB.getChannelListenerTable().getListenerDetails(serverID, userID, channelID);

	if (listener.volumeAdjustment != volumeFactor) {
		listener.volumeAdjustment = volumeFactor;
		m_serverDB.getChannelListenerTable().updateListener(listener);
	}

	WRAPPER_END
}

float DBWrapper::getChannelListenerVolume(unsigned int serverID, unsigned int userID, unsigned int channelID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);
	assertValidID(channelID);
	assert(m_serverDB.getChannelListenerTable().listenerExists(serverID, userID, channelID));

	return m_serverDB.getChannelListenerTable().getListenerDetails(serverID, userID, channelID).volumeAdjustment;

	WRAPPER_END
}

bool DBWrapper::channelListenerExists(unsigned int serverID, unsigned int userID, unsigned int channelID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);
	assertValidID(channelID);

	return m_serverDB.getChannelListenerTable().listenerExists(serverID, userID, channelID);

	WRAPPER_END
}

unsigned int DBWrapper::registerUser(unsigned int serverID, const ServerUserInfo &userInfo) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	assert(!userInfo.qsName.isEmpty());

	::mdb::TransactionHolder transaction = m_serverDB.ensureTransaction();

	unsigned int userID = userInfo.iId < 0 ? m_serverDB.getUserTable().getFreeUserID(serverID)
										   : static_cast< unsigned int >(userInfo.iId);

	::msdb::DBUser user(serverID, userID);

	m_serverDB.getUserTable().addUser(user, userInfo.qsName.toStdString());

	::msdb::DBUserData data;
	data.name          = userInfo.qsName.toStdString();
	data.lastChannelID = userInfo.cChannel ? userInfo.cChannel->iId : Mumble::ROOT_CHANNEL_ID;
	if (!userInfo.qbaTexture.isEmpty()) {
		data.texture.resize(static_cast< std::size_t >(userInfo.qbaTexture.size()));

		static_assert(sizeof(decltype(data.texture)::value_type) == sizeof(decltype(userInfo.qbaTexture)::value_type),
					  "Data types are not compatible");
		std::memcpy(data.texture.data(), reinterpret_cast< const std::uint8_t * >(userInfo.qbaTexture.data()),
					static_cast< std::size_t >(userInfo.qbaTexture.size()));
	}

	setUserData(serverID, userID, data);


	std::vector< std::pair< unsigned int, std::string > > properties;
	properties.push_back(
		{ static_cast< unsigned int >(::msdb::UserProperty::CertificateHash), userInfo.qsHash.toStdString() });

	if (!userInfo.qslEmail.isEmpty()) {
		properties.push_back(
			{ static_cast< unsigned int >(::msdb::UserProperty::Email), userInfo.qslEmail.first().toStdString() });
	}

	if (!userInfo.qsComment.isEmpty()) {
		properties.push_back(
			{ static_cast< unsigned int >(::msdb::UserProperty::Comment), userInfo.qsComment.toStdString() });
	}

	setUserProperties(serverID, userID, properties);


	transaction.commit();

	return userID;

	WRAPPER_END
}

void DBWrapper::unregisterUser(unsigned int serverID, unsigned int userID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);
	m_serverDB.getUserTable().removeUser(user);

	WRAPPER_END
}

int DBWrapper::registeredUserNameToID(unsigned int serverID, const std::string &name) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::optional< unsigned int > id = m_serverDB.getUserTable().findUser(serverID, name, false);

	return id ? static_cast< int >(id.value()) : -1;

	WRAPPER_END
}

bool DBWrapper::registeredUserExists(unsigned int serverID, unsigned int userID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertValidID(userID);

	::msdb::DBUser user(serverID, userID);

	return m_serverDB.getUserTable().userExists(user);

	WRAPPER_END
}

::msdb::DBUserData DBWrapper::getRegisteredUserData(unsigned int serverID, unsigned int userID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);

	return m_serverDB.getUserTable().getData(user);

	WRAPPER_END
}

QMap< int, QString > DBWrapper::getRegisteredUserDetails(unsigned int serverID, unsigned int userID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	QMap< int, QString > details;

	::msdb::DBUser user(serverID, userID);
	::msdb::DBUserData userData = m_serverDB.getUserTable().getData(user);

	details.insert({ static_cast< int >(::msdb::UserProperty::Name) }, QString::fromStdString(userData.name));
	details.insert({ static_cast< int >(::msdb::UserProperty::LastActive) },
				   QDateTime::fromSecsSinceEpoch(static_cast< qint64 >(::msdb::toEpochSeconds(userData.lastActive)))
					   .toString(Qt::ISODate));

	for (const std::pair< unsigned int, std::string > &currentProps : getUserProperties(serverID, userID)) {
		details.insert(static_cast< int >(currentProps.first), QString::fromStdString(currentProps.second));
	}

	return details;

	WRAPPER_END
}


void DBWrapper::addAllRegisteredUserInfoTo(std::vector< UserInfo > &userInfo, unsigned int serverID,
										   const std::string &nameFilter) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	for (const ::msdb::DBUser &currentUser : m_serverDB.getUserTable().getRegisteredUsers(serverID, nameFilter)) {
		::msdb::DBUserData userData = m_serverDB.getUserTable().getData(currentUser);

		UserInfo info;
		info.name    = QString::fromStdString(userData.name);
		info.user_id = static_cast< int >(currentUser.registeredUserID);
		info.last_active =
			QDateTime::fromSecsSinceEpoch(static_cast< qint64 >(::msdb::toEpochSeconds(userData.lastActive)));
		info.last_channel = userData.lastChannelID;

		userInfo.push_back(std::move(info));
	}

	WRAPPER_END
}

std::optional< unsigned int > DBWrapper::findRegisteredUserByCert(unsigned int serverID, const std::string &certHash) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::vector< unsigned int > candidates = m_serverDB.getUserPropertyTable().findUsersWithProperty(
		serverID, ::msdb::UserProperty::CertificateHash, certHash);

	// We don't expect that multiple users with the same certificate hash can exist on the same server
	assert(candidates.size() < 2);

	// Only return the client ID, if the chosen client is unique
	return candidates.size() == 1 ? std::optional< unsigned int >(candidates[0]) : std::nullopt;

	WRAPPER_END
}

std::optional< unsigned int > DBWrapper::findRegisteredUserByEmail(unsigned int serverID, const std::string &email) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::vector< unsigned int > candidates =
		m_serverDB.getUserPropertyTable().findUsersWithProperty(serverID, ::msdb::UserProperty::Email, email);

	// Only return the client ID, if the chosen client is unique
	return candidates.size() == 1 ? std::optional< unsigned int >(candidates[0]) : std::nullopt;

	WRAPPER_END
}

void DBWrapper::storeRegisteredUserPassword(unsigned int serverID, unsigned int userID, const QString &password,
											unsigned int kdfIterations) {
	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	storeRegisteredUserPassword(serverID, userID, password.toStdString(), kdfIterations);
}

void DBWrapper::storeRegisteredUserPassword(unsigned int serverID, unsigned int userID, const std::string &password,
											unsigned int kdfIterations) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	const ::msdb::DBUser user(serverID, userID);

	::msdb::DBUserData::PasswordData pwData;

	if (password.empty()) {
	} else {
		if (Meta::mp->legacyPasswordHash) {
			pwData.passwordHash = getLegacyPasswordHash(password).toStdString();
		} else {
			assert(Meta::mp->kdfIterations >= 0);
			pwData.kdfIterations =
				kdfIterations > 0 ? kdfIterations : static_cast< unsigned int >(Meta::mp->kdfIterations);
			pwData.salt         = PBKDF2::getSalt().toStdString();
			pwData.passwordHash = PBKDF2::getHash(QString::fromStdString(pwData.salt), QString::fromStdString(password),
												  static_cast< int >(pwData.kdfIterations))
									  .toStdString();
		}
	}

	m_serverDB.getUserTable().setPassword(user, pwData);

	WRAPPER_END
}

std::vector< unsigned int > DBWrapper::getRegisteredUserIDs(unsigned int serverID) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	std::vector< unsigned int > userIDs;

	std::vector<::msdb::DBUser > users = m_serverDB.getUserTable().getRegisteredUsers(serverID);

	userIDs.reserve(users.size());

	for (const ::msdb::DBUser &currentUser : users) {
		userIDs.push_back(currentUser.registeredUserID);
	}

	return userIDs;

	WRAPPER_END
}

void DBWrapper::setLastChannel(unsigned int serverID, const ServerUserInfo &userInfo) {
	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userInfo.iId);
	assert(userInfo.cChannel);
	assertValidID(userInfo.cChannel->iId);

	setLastChannel(serverID, static_cast< unsigned int >(userInfo.iId), userInfo.cChannel->iId);
}

void DBWrapper::setLastChannel(unsigned int serverID, unsigned int userID, unsigned int channelID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);
	assertValidID(channelID);

	::msdb::DBUser user(serverID, userID);

	m_serverDB.getUserTable().setLastChannelID(user, channelID);

	WRAPPER_END
}

unsigned int DBWrapper::getLastChannelID(unsigned int serverID, unsigned int userID, unsigned int maxRememberDuration,
										 std::size_t serverUptimeSecs) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);

	::msdb::DBUserData userData = m_serverDB.getUserTable().getData(user);

	if (maxRememberDuration == 0) {
		// Channels shall be remembered forever
		return userData.lastChannelID;
	}

	if (userData.lastDisconnect < userData.lastActive) {
		// If the last disconnect time is smaller than the last active time, the disconnect time is invalid.
		// This can happen, if the entire server is shut down while the user still was connected.
		// In such cases, we instead take the server's up time as a reference point.
		userData.lastDisconnect = std::chrono::system_clock::now();
		userData.lastDisconnect -= std::chrono::seconds(serverUptimeSecs);
	}

	long inactiveSeconds =
		std::chrono::duration_cast< std::chrono::seconds >(std::chrono::system_clock::now() - userData.lastDisconnect)
			.count();

	assert(inactiveSeconds >= 0);

	return maxRememberDuration >= static_cast< unsigned long >(inactiveSeconds) ? userData.lastChannelID
																				: Mumble::ROOT_CHANNEL_ID;

	WRAPPER_END
}

QByteArray DBWrapper::getUserTexture(unsigned int serverID, unsigned int userID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);
	::msdb::DBUserData data = m_serverDB.getUserTable().getData(user);

	QByteArray texture;
	if (!data.texture.empty()) {
		texture.resize(static_cast< int >(data.texture.size()));

		std::memcpy(reinterpret_cast< std::uint8_t * >(texture.data()), data.texture.data(), data.texture.size());
	}

	if (texture.size() == 600 * 60 * 4) {
		texture = qCompress(texture);
	}

	return texture;

	WRAPPER_END
}

void DBWrapper::storeUserTexture(unsigned int serverID, const ServerUserInfo &userInfo) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userInfo.iId);

	::msdb::DBUser user(serverID, static_cast< unsigned int >(userInfo.iId));
	::msdb::DBUserData data = m_serverDB.getUserTable().getData(user);

	QByteArray texture =
		userInfo.qbaTexture.size() == 600 * 60 * 4 ? qCompress(userInfo.qbaTexture) : userInfo.qbaTexture;

	data.texture.resize(static_cast< std::size_t >(texture.size()));
	std::memcpy(data.texture.data(), reinterpret_cast< const std::uint8_t * >(texture.data()),
				static_cast< std::size_t >(texture.size()));

	m_serverDB.getUserTable().updateData(user, data);

	WRAPPER_END
}

std::string DBWrapper::getUserProperty(unsigned int serverID, unsigned int userID, ::msdb::UserProperty property) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);

	return m_serverDB.getUserPropertyTable().getProperty< std::string, false >(user, property, {});

	WRAPPER_END
}

void DBWrapper::storeUserProperty(unsigned int serverID, unsigned int userID, ::msdb::UserProperty property,
								  const std::string &value) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);

	if (value.empty()) {
		m_serverDB.getUserPropertyTable().clearProperty(user, property);
	} else {
		m_serverDB.getUserPropertyTable().setProperty(user, property, value);
	}

	WRAPPER_END
}

void DBWrapper::setUserProperties(unsigned int serverID, unsigned int userID,
								  const std::vector< std::pair< unsigned int, std::string > > &properties) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);

	for (const std::pair< unsigned int, std::string > &current : properties) {
		assert(current.first <= std::numeric_limits< std::underlying_type<::msdb::UserProperty >::type >::max());

		// Note that this casting could potentially cause UserProperty to take on new values (not actually defined in
		// the enum). However, the properties are only stored as integers in the database anyway and since above
		// assertion has checked (at least in debug mode) that the used type to represent a UserProperty is large enough
		// to hold the value, this should be fine.
		// Ideally, this will be changed at some point, but for now it should work.
		::msdb::UserProperty property = static_cast<::msdb::UserProperty >(current.first);

		if (property == ::msdb::UserProperty::Name || property == ::msdb::UserProperty::kdfIterations
			|| property == ::msdb::UserProperty::LastActive || property == ::msdb::UserProperty::Password) {
			// These are all properties that are supposed to be stored in the user table rather than the user property
			// table We assume that the calling code has taken care of this and won't even pass those down here.
			assert(false && "These properties should have been processed separately, before calling this function");
			continue;
		}

		m_serverDB.getUserPropertyTable().setProperty(user, property, current.second);
	}

	WRAPPER_END
}

std::vector< std::pair< unsigned int, std::string > > DBWrapper::getUserProperties(unsigned int serverID,
																				   unsigned int userID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	std::vector< std::pair< unsigned int, std::string > > properties;

	::mdb::TransactionHolder transaction = m_serverDB.ensureTransaction();

	// Start with user properties that are stored in the user table itself
	::msdb::DBUser user(serverID, userID);
	::msdb::DBUserData userData = m_serverDB.getUserTable().getData(user);

	properties.push_back({ static_cast< unsigned int >(::msdb::UserProperty::Name), userData.name });
	properties.push_back(
		{ static_cast< unsigned int >(::msdb::UserProperty::LastActive),
		  QDateTime::fromSecsSinceEpoch(static_cast< qint64 >(::msdb::toEpochSeconds(userData.lastActive)))
			  .toString(Qt::ISODate)
			  .toStdString() });
	// Note: we explicitly don't insert the password and kdfIterations property - those are secret and not handed out!

	// Fetch remaining properties (but only those that we know of)
	std::string email =
		m_serverDB.getUserPropertyTable().getProperty< std::string, false >(user, ::msdb::UserProperty::Email);
	if (!email.empty()) {
		properties.push_back({ static_cast< unsigned int >(::msdb::UserProperty::Email), std::move(email) });
	}

	std::string comment =
		m_serverDB.getUserPropertyTable().getProperty< std::string, false >(user, ::msdb::UserProperty::Comment);
	if (!comment.empty()) {
		properties.push_back({ static_cast< unsigned int >(::msdb::UserProperty::Comment), std::move(comment) });
	}

	std::string certHash = m_serverDB.getUserPropertyTable().getProperty< std::string, false >(
		user, ::msdb::UserProperty::CertificateHash);
	if (!certHash.empty()) {
		properties.push_back(
			{ static_cast< unsigned int >(::msdb::UserProperty::CertificateHash), std::move(certHash) });
	}

	transaction.commit();

	return properties;

	WRAPPER_END
}

std::string DBWrapper::getUserName(unsigned int serverID, unsigned int userID) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);

	return m_serverDB.getUserTable().getData(user).name;

	WRAPPER_END
}

unsigned int DBWrapper::getNextAvailableUserID(unsigned int serverID) {
	WRAPPER_BEGIN

	assertValidID(serverID);

	return m_serverDB.getUserTable().getFreeUserID(serverID);

	WRAPPER_END
}

void DBWrapper::setUserData(unsigned int serverID, unsigned int userID, const ::msdb::DBUserData &data) {
	WRAPPER_BEGIN

	assertValidID(serverID);
	assertRegisteredUserExists(serverID, userID);

	::msdb::DBUser user(serverID, userID);

	m_serverDB.getUserTable().updateData(user, data);

	WRAPPER_END
}

nlohmann::json DBWrapper::exportDBToJSON() {
	WRAPPER_BEGIN

	return m_serverDB.exportToJSON();

	WRAPPER_END
}

void DBWrapper::importFromJSON(const nlohmann::json &json, bool createMissingTables) {
	WRAPPER_BEGIN

	m_serverDB.importFromJSON(json, createMissingTables);

	WRAPPER_END
}

#undef assertValidID
#undef assertRegisteredUserExists
