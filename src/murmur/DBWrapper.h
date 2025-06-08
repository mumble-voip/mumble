// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_DBWRAPPER_H_
#define MUMBLE_SERVER_DBWRAPPER_H_

#include "NonCopyable.h"
#include "murmur/database/DBChannel.h"
#include "murmur/database/DBLogEntry.h"
#include "murmur/database/DBUserData.h"
#include "murmur/database/ServerDatabase.h"
#include "murmur/database/UserProperty.h"

#include "database/ConnectionParameter.h"

#include "Ban.h"
#include "User.h"

#include <nlohmann/json_fwd.hpp>

#include <optional>
#include <string>
#include <thread>
#include <vector>

class Server;
class ServerUserInfo;
class Channel;
class Meta;
class ChannelListenerManager;

class QByteArray;

class DBWrapper : public NonCopyable {
public:
	DBWrapper(const ::mumble::db::ConnectionParameter &connectionParams);

	// Server management
	std::vector< unsigned int > getAllServers();
	std::vector< unsigned int > getBootServers();
	unsigned int addServer();
	void removeServer(unsigned int serverID);
	bool serverExists(unsigned int serverID);
	void setServerBootProperty(unsigned int serverID, bool boot);
	std::optional< unsigned int > loadPBKDF2IterationCount();
	void storePBKDF2IterationCount(unsigned int count);

	void setSuperUserPassword(unsigned int serverID, const std::string &password);
	void disableSuperUser(unsigned int serverID);

	void clearAllPerServerSLLConfigurations();
	void clearAllServerLogs();

	std::vector< Ban > getBans(unsigned int serverID);
	void saveBans(unsigned int serverID, const std::vector< Ban > &bans);

	void initializeChannels(Server &server);
	void initializeChannelDetails(Server &server);
	void initializeChannelLinks(Server &server);

	unsigned int getNextAvailableChannelID(unsigned int serverID);
	void createChannel(unsigned int serverID, const Channel &channel);
	void createChannel(const ::mumble::server::db::DBChannel &channel, unsigned int position = 0,
					   unsigned int maxUsers = 0, const std::string &description = {});
	void deleteChannel(unsigned int serverID, unsigned int channelID);
	void updateChannelData(unsigned int serverID, const Channel &channel);

	void addChannelLink(unsigned int serverID, const Channel &first, const Channel &second);
	void removeChannelLink(unsigned int serverID, const Channel &first, const Channel &second);


	void getConfigurationTo(unsigned int serverID, const std::string &configKey, QString &outVar);
	void getConfigurationTo(unsigned int serverID, const std::string &configKey, std::string &outVar);
	void getConfigurationTo(unsigned int serverID, const std::string &configKey, QByteArray &outVar);
	void getConfigurationTo(unsigned int serverID, const std::string &configKey, unsigned short &outVar);
	void getConfigurationTo(unsigned int serverID, const std::string &configKey, bool &outVar);
	void getConfigurationTo(unsigned int serverID, const std::string &configKey, int &outVar);
	void getConfigurationTo(unsigned int serverID, const std::string &configKey, unsigned int &outVar);
	void getConfigurationTo(unsigned int serverID, const std::string &configKey, std::optional< bool > &outVar);

	std::vector< std::pair< std::string, std::string > > getAllConfigurations(unsigned int serverID);

	void setConfiguration(unsigned int serverID, const std::string &configKey, const std::string &value);
	void clearConfiguration(unsigned int serverID, const std::string &configKey);


	void logMessage(unsigned int serverID, const std::string &msg);
	std::vector< mumble::server::db::DBLogEntry > getLogs(unsigned int serverID, unsigned int startOffset = 0,
														  int amount = -1);
	std::size_t getLogSize(unsigned int serverID);

	/**
	 * Sets the last-disconnected status of the given user to the current time
	 */
	void updateLastDisconnect(unsigned int serverID, unsigned int userID);

	void addChannelListenerIfNotExists(unsigned int serverID, unsigned int userID, unsigned int channelID);
	void disableChannelListenerIfExists(unsigned int serverID, unsigned int userID, unsigned int channelID);
	void deleteChannelListener(unsigned int serverID, unsigned int userID, unsigned int channelID);
	void loadChannelListenersOf(unsigned int serverID, const ServerUserInfo &userInfo, ChannelListenerManager &manager);
	void storeChannelListenerVolume(unsigned int serverID, unsigned int userID, unsigned int channelID,
									float volumeFactor);
	float getChannelListenerVolume(unsigned int serverID, unsigned int userID, unsigned int channelID);
	bool channelListenerExists(unsigned int serverID, unsigned int userID, unsigned int channelID);

	/**
	 * Performs the registration of the given user in the database
	 *
	 * @returns The user ID assigned to the newly registered user
	 */
	unsigned int registerUser(unsigned int serverID, const ServerUserInfo &userInfo);
	void unregisterUser(unsigned int serverID, unsigned int userID);
	int registeredUserNameToID(unsigned int serverID, const std::string &name);
	bool registeredUserExists(unsigned int serverID, unsigned int userID);
	::mumble::server::db::DBUserData getRegisteredUserData(unsigned int serverID, unsigned int userID);
	QMap< int, QString > getRegisteredUserDetails(unsigned int serverID, unsigned int userID);
	void addAllRegisteredUserInfoTo(std::vector< UserInfo > &userInfo, unsigned int serverID,
									const std::string &nameFilter);
	std::optional< unsigned int > findRegisteredUserByCert(unsigned int serverID, const std::string &certHash);
	std::optional< unsigned int > findRegisteredUserByEmail(unsigned int serverID, const std::string &email);
	void storeRegisteredUserPassword(unsigned int serverID, unsigned int userID, const QString &password,
									 unsigned int kdfIterations = 0);
	void storeRegisteredUserPassword(unsigned int serverID, unsigned int userID, const std::string &password,
									 unsigned int kdfIterations = 0);
	std::vector< unsigned int > getRegisteredUserIDs(unsigned int serverID);

	void setLastChannel(unsigned int serverID, const ServerUserInfo &userInfo);
	void setLastChannel(unsigned int serverID, unsigned int userID, unsigned int channelID);
	unsigned int getLastChannelID(unsigned int serverID, unsigned int userID, unsigned int maxRememberDuration = 0,
								  std::size_t serverUptimeSecs = 0);


	QByteArray getUserTexture(unsigned int serverID, unsigned int userID);
	void storeUserTexture(unsigned int serverID, const ServerUserInfo &userInfo);

	std::string getUserProperty(unsigned int serverID, unsigned int userID,
								::mumble::server::db::UserProperty property);
	void storeUserProperty(unsigned int serverID, unsigned int userID, ::mumble::server::db::UserProperty prop,
						   const std::string &value);
	void setUserProperties(unsigned int serverID, unsigned int userID,
						   const std::vector< std::pair< unsigned int, std::string > > &properties);
	std::vector< std::pair< unsigned int, std::string > > getUserProperties(unsigned int serverID, unsigned int userID);

	std::string getUserName(unsigned int serverID, unsigned int userID);

	unsigned int getNextAvailableUserID(unsigned int serverID);

	void setUserData(unsigned int serverID, unsigned int userID, const ::mumble::server::db::DBUserData &data);

	nlohmann::json exportDBToJSON();

	void importFromJSON(const nlohmann::json &json, bool createMissingTables);

protected:
	::mumble::server::db::ServerDatabase m_serverDB;
	const std::thread::id m_threadID = std::this_thread::get_id();
};

#endif // MUMBLE_SERVER_DBWRAPPER_H_
