#pragma once
#include "common.hpp"
#include "datatypes.hpp"
#include <map>
#include "Locks.hpp"
#include <clientlib_publicdefinitions.h>
#include <public_rare_definitions.h>
#include <unordered_map>
#include <unordered_set>

//All Teamspeak interaction should be in here as static methods.

class TeamspeakServerData {
public:
    TeamspeakServerData() = default;
    ~TeamspeakServerData() = default;
    std::vector<dataType::TSClientID> getMutedClients();
    void setClientMuteStatus(dataType::TSClientID, bool muted);
    bool getClientMuteStatus(dataType::TSClientID);
    void clearMutedClients();
    dataType::TSClientID getMyClientID();
    void setMyClientID(dataType::TSClientID val);
    std::string getMyOriginalNickname();
    void setMyOriginalNickname(std::string val);
    dataType::TSChannelID getMyOriginalChannel();
    void setMyOriginalChannel(dataType::TSChannelID val);
    TSChannelID myLastKnownChannel{ -1 };//Only called via Teamspeak Events. So not multi-threaded

    TSChannelID getChannelIDFromName(const std::string& channelName);
    void addChannelCache(const char* channelName, TSChannelID channelID);
    void clearChannelCache();

private:
    CriticalSectionLock m_criticalSection{"TeamspeakServerData"};
    std::unordered_set<dataType::TSClientID> mutedClients;
    std::string myOriginalNickname;
    TSClientID myClientID{ -1 };//Too small to need mutex
    TSChannelID myOriginalChannel{ -1 };
    std::unordered_map<std::string, TSChannelID> channelNameToID;
};




class Teamspeak {
public:
    Teamspeak();
    ~Teamspeak() = default;
    static TSServerID getCurrentServerConnection();
    static void unmuteAll(TSServerID serverConnectionHandlerID = Teamspeak::getCurrentServerConnection());
    static void setClientMute(TSServerID serverConnectionHandlerID, TSClientID clientID, bool mute);
    static void setClientMute(TSServerID serverConnectionHandlerID, std::vector<TSClientID> clientIds, bool mute);
    static void moveToSeriousChannel(TSServerID serverConnectionHandlerID = getCurrentServerConnection());
    static void moveFromSeriousChannel(TSServerID serverConnectionHandlerID = getCurrentServerConnection());
    static bool setMyNicknameToGameName(TSServerID serverConnectionHandlerID, const std::string& nickname);
    static void resetMyNickname(TSServerID serverConnectionHandlerID = getCurrentServerConnection());


    static bool isConnected(TSServerID serverConnectionHandlerID = getCurrentServerConnection());
    static TSClientID getMyId(TSServerID serverConnectionHandlerID = getCurrentServerConnection());
    static bool isInChannel(TSServerID serverConnectionHandlerID, TSClientID clientId, const std::string& channelToCheck);
    static std::string getChannelName(TSServerID serverConnectionHandlerID = getCurrentServerConnection(), TSClientID clientId = getMyId());
    static TSChannelID getChannelOfClient(TSServerID serverConnectionHandlerID = getCurrentServerConnection(), TSClientID clientId = getMyId());
    static std::string getServerName(TSServerID serverConnectionHandlerID = getCurrentServerConnection());
    static std::string getServerUID(TSServerID serverConnectionHandlerID = getCurrentServerConnection());
    static bool isTalking(TSServerID currentServerConnectionHandlerID, TSClientID clientID);
    static std::vector<TSClientID> getChannelClients(TSServerID serverConnectionHandlerID, TSChannelID channelId);
    static std::string getMyNickname(TSServerID serverConnectionHandlerID);

    static TSChannelID findChannelByName(TSServerID serverConnectionHandlerID, const std::string& channelName);

    static std::string getMetaData(TSServerID serverConnectionHandlerID, TSClientID clientId);
    static void setMyMetaData(const std::string & metaData);
    static std::string getClientNickname(TSServerID serverConnectionHandlerID, TSClientID clientId);
    static void setMyClient3DPosition(TSServerID serverConnectionHandlerID, Position3D pos);
    static void setClient3DPosition(TSServerID serverConnectionHandlerID, TSClientID clientId, Position3D pos);

    static void sendPluginCommand(TSServerID serverConnectionHandlerID, std::string_view pluginID, std::string_view command, PluginTargetMode targetMode, std::vector<TSClientID> targets = {});
    static void playWavFile(const std::string& filePath);

    static void setVoiceDisabled(TSServerID serverConnectionHandlerID, bool disabled);


    // taken from https://github.com/MadStyleCow/A2TS_Rebuild/blob/master/src/ts3plugin.cpp#L1367
    static bool hlp_checkVad();
    static void hlp_enableVad();
    static void hlp_disableVad();

    static void log(std::string, unsigned long errorCode, LogLevel level = LogLevel_INFO);
    static void printMessageToCurrentTab(const char* msg);

    //Internal use
    static void _onConnectStatusChangeEvent(TSServerID serverConnectionHandlerID, ConnectStatus newState);
    static void _onChannelSwitchedEvent(TSServerID serverConnectionHandlerID, TSChannelID newChannel);
    static void _onClientMoved(TSServerID serverConnectionHandlerID, TSClientID clientID, TSChannelID oldChannel, TSChannelID newChannel);
    static void _onClientJoined(TSServerID serverConnectionHandlerID, TSClientID clientID, TSChannelID channel);
    static void _onClientLeft(TSServerID serverConnectionHandlerID, TSClientID clientID);
    static void _onClientUpdated(TSServerID serverConnectionHandlerID, TSClientID clientID);
    static void _onInit();
    static void _updateChanneNameCache(TSServerID serverConnectionHandlerID);
private:
    static Teamspeak& getInstance();  //Used for accessing stored variables




    std::map<dataType::TSServerID, TeamspeakServerData> serverData;

};

