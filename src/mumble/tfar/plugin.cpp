/*
 * TeamSpeak 3 demo plugin
 *
 * Copyright (c) 2008-2013 TeamSpeak Systems GmbH
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <numeric>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include "public_rare_definitions.h"
#include "ts3_functions.h"
#include "plugin.h"
#include <dspfilters/Butterworth.h>



#include "RadioEffect.hpp"


#include "clientData.hpp"
#include "serverData.hpp"
#include "task_force_radio.hpp"
#include "common.hpp"
#include "pipe_handler.hpp"
#include "helpers.hpp"
#include "PlaybackHandler.hpp"
#include "Logger.hpp"
#include "SharedMemoryHandler.hpp"
#include "Teamspeak.hpp"
#include <chrono>
#include "version.h"
#include "profilers.hpp"

#define PATH_BUFSIZE 512

std::thread threadPipeHandle;
std::thread threadService;

volatile bool exitThread = FALSE;
volatile bool pipeConnected = false;

void log_string(std::string message, LogLevel level) {
    Logger::log(LoggerTypes::teamspeakClientlog, message, level);//Default loglevel is Info
}

bool isSeriousModeEnabled(TSServerID serverConnectionHandlerID, TSClientID clientId) {
    std::string serious_mod_channel_name = TFAR::config.get<std::string>(Setting::serious_channelName);
    return !serious_mod_channel_name.empty() && Teamspeak::isInChannel(serverConnectionHandlerID, clientId, serious_mod_channel_name);
}

bool isPluginEnabledForUser(TSServerID serverConnectionHandlerID, TSClientID clientID);//defined below

float effectErrorFromDistance(sendingRadioType radioType, float distance, std::shared_ptr<clientData>& data) {
    auto maxD = 1.0f;//We don't want division by 0 do we?
    switch (radioType) {
        case sendingRadioType::LISTEN_TO_SW: maxD = static_cast<float>(data->range); break;
        case sendingRadioType::LISTEN_TO_LR: maxD = static_cast<float>(data->range);
        default: break;
    }
    return distance / maxD;
}

void setGameClientMuteStatus(TSServerID serverConnectionHandlerID, TSClientID clientID, std::pair<bool, bool> isOverRadio = { false,false }) {
    bool mute = false;
    if (isSeriousModeEnabled(serverConnectionHandlerID, clientID) && !TFAR::config.get<bool>(Setting::disableAutomaticMute)) {

        const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnectionHandlerID);
        std::shared_ptr<clientData> clientData;
        if (clientDataDir)
            clientData = clientDataDir->getClientData(clientID);
        auto myData = clientDataDir->myClientData;
        if (clientData == myData) return; //can't mute self... well you can but don't that's confusing to users

        if (clientData && myData && (TFAR::getInstance().m_gameData.alive && clientData->isAlive() || myData->isSpectating)) {
            auto distance = myData->getClientPosition().distanceTo(clientData->getClientPosition());
            mute = distance > (clientData->voiceVolume + 15);

            if (mute) {//If he is in range we don't need to check if we can hear him over radio as we can hear anyway.
                const bool isOnRadio = isOverRadio.first ? isOverRadio.second : !clientData->isOverRadio(myData, false, false).empty();
                mute = !isOnRadio;
            }
        } else if (clientData && myData && !clientData->isConfirmedDead()
                   && (TFAR::getInstance().m_gameData.alive || myData->isSpectating)
                   && isPluginEnabledForUser(serverConnectionHandlerID, clientID)) {
            //We know the client, he reports his plugin as connected and in
            //game, but his position data is stale (no update for a while /
            //data frames lagging). That happens under heavy load on large
            //events, when the position stream from the game can't keep up.
            //Fail open: keeping a living player muted because we lack data
            //silences him for the whole channel until he rejoins. Only an
            //explicit KILLED event (or him actually leaving the game — his
            //metadata then no longer reports "in game") may mute him.
            mute = false;
        } else {
            mute = true;
        }
        if (mute && clientData) clientData->effects.resetVoices();
    }
    Teamspeak::setClientMute(serverConnectionHandlerID, clientID, mute);
}

std::string getConnectionStatusInfo(bool pipeConnected, bool inGame, bool includeVersion) {
    std::ostringstream stringStream;
    stringStream << "Connected to Game: " << (pipeConnected ? "[B]Yes[/B]" : "[B]No[/B]") << std::endl;
    stringStream << "Playing: " << (inGame ? "[B]Yes[/B]" : "[B]No[/B]") << std::endl;
    if (includeVersion) {
        stringStream << "Plugin version: [B]" << PLUGIN_VERSION << "[/B]" << std::endl;
        stringStream << "Addon version: [B]" << TFAR::config.get<std::string>(Setting::addon_version) << "[/B]" << std::endl;
    }
    return stringStream.str();
}

void updateUserStatusInfo(bool pluginEnabled) {
    if (!Teamspeak::isConnected()) return;
    std::string result;
    if (pluginEnabled)
        result = getConnectionStatusInfo(pipeConnected, TFAR::getInstance().getCurrentlyInGame(), true);
    else
        result = "[B]Task Force Radio Plugin Disabled[/B]";
    Teamspeak::setMyMetaData(result);
}

PTTDelayArguments::subtypes PTTDelayArguments::stringToSubtype(std::string_view type) {
    switch (type.length()) {
        case "dd"sv.length(): return subtypes::dd;
        case "digital"sv.length(): return subtypes::digital;
        case "airborne"sv.length(): return subtypes::airborne;
        case "digital_lr"sv.length(): return subtypes::digital_lr;
        case "phone"sv.length(): return subtypes::phone;
        default: return subtypes::invalid;
    }
}

#include "CommandProcessor.hpp"

std::atomic<std::chrono::system_clock::time_point> lastInGame = std::chrono::system_clock::now();
std::atomic<std::chrono::system_clock::time_point> lastCheckForExpire = std::chrono::system_clock::now();
std::atomic<std::chrono::system_clock::time_point> lastInfoUpdate = std::chrono::system_clock::now();

void ServiceThread() {

    while (!exitThread) {
        if (!Teamspeak::isConnected()) {  //If not connected we don't have any clientData anyway
            Sleep(500);
            continue;
        }
        if ((std::chrono::system_clock::now() - lastCheckForExpire.load()) > MILLIS_TO_EXPIRE) {
            //bool isSerious = isSeriousModeEnabled(Teamspeak::getCurrentServerConnection(), Teamspeak::getMyId());

            if (TFAR::getInstance().getCurrentlyInGame()) {
                bool allowedToMove = true;
                if (TFAR::config.get<bool>(Setting::noAutomoveSpectator)) {
                    auto clientDir = TFAR::getServerDataDirectory()->getClientDataDirectory(Teamspeak::getCurrentServerConnection());
                    if (clientDir && clientDir->myClientData) {
                        allowedToMove = !clientDir->myClientData->isSpectating;
                    }
                }
                if (allowedToMove)
                    Teamspeak::moveToSeriousChannel();//#TODO people may want to leave SeriousChannel on purpose and not be moved back

                //Self-healing for stuck local mutes: mutes are normally only
                //re-evaluated by events (position updates, VOLUME/TANGENT
                //commands). If those events stop arriving for a client — which
                //happens on large events when the data stream is overloaded —
                //the mute would stick until he rejoins the channel, leaving a
                //living, talking player inaudible. Re-evaluate every muted
                //client periodically so such mutes resolve on their own.
                const auto serverConnection = Teamspeak::getCurrentServerConnection();
                for (const auto& mutedClient : Teamspeak::getLocallyMutedClients(serverConnection))
                    setGameClientMuteStatus(serverConnection, mutedClient);
            }

            lastCheckForExpire = std::chrono::system_clock::now();
            TFAR::getServerDataDirectory()->verify();
        }
        if ((std::chrono::system_clock::now() - lastInfoUpdate.load()) > 4000ms) {
            updateUserStatusInfo(true);
            lastInfoUpdate = std::chrono::system_clock::now();
        }
        Sleep(100);
    }
}
#define USE_SHAREDMEM
void PipeThread() {
#ifdef USE_SHAREDMEM
    SharedMemoryHandler pipeHandler;


    pipeHandler.onDisconnected.connect(
        [&]() {
        OutputDebugStringA("disconnected\n");
        pipeConnected = false;
        updateUserStatusInfo(true);
        TFAR::getInstance().onGameDisconnected();
    });

    pipeHandler.onConnected.connect(
        [&]() {
        OutputDebugStringA("connected\n");
        pipeConnected = true;
        TFAR::getInstance().onGameConnected();
    });

#else
    pipe_handler pipeHandler;
#endif


    std::vector<std::string> commands;
    while (!exitThread) {
        ProfileFunction;

        if (!pipeHandler.isConnected()) { //Need this call! It sets the lastPluginTick variable and causes on(dis)Connected events
            ProfileScopeN("no connect sleep");
            std::this_thread::sleep_for(100ms);
            continue;
        }

    #ifdef USE_SHAREDMEM
        pipeHandler.setConfigNeedsRefresh(TFAR::config.needsRefresh());//#TODO Signal/Slot ?
    #endif

        commands.resize(1);
        // pipeHandler.getDataMultiple
        if (!pipeHandler.getData(commands[0], 20ms))//This will still wait. 1ms if SHAMEM error. 20ms if game unconnected
            continue;

        for (auto& command : commands) {

            ProfileScopeN("got command");
            //speedTest gameCommandIn("gameCommandInPipe", false);

            if (command.back() == '~') {//a ~ at the end identifies an Async call
                command.pop_back();//removes ~ from end
#ifdef USE_SHAREDMEM
                TFAR::getCommandProcessor()->queueCommand(command);
#else
                bool dataReturned = false;
                if (pipeHandler.sendData("OK", 2)) {
                    log_string("Info to ARMA send async", LogLevel_DEBUG);
                    dataReturned = true;
                } else {
                    log_string("Can't send info to ARMA async", LogLevel_ERROR);
                }
#endif
            } else {
                //gameCommandIn.reset();
#ifdef USE_SHAREDMEM
                const auto commandResult = TFAR::getCommandProcessor()->processCommand(command);
                pipeHandler.sendData(commandResult);
                //if (gameCommandIn.getCurrentElapsedTime().count() >
                //#ifdef _DEBUG
                //    400)
                //#else
                //    200)
                //#endif
                //    log_string("gameinteraction " + std::to_string(gameCommandIn.getCurrentElapsedTime().count()) + command, LogLevel_INFO);   //#Release remove logging and creation variable
#else
                if (gameCommandIn.getCurrentElapsedTime().count() > 200)
                    log_string("gameinteraction " + std::to_string(gameCommandIn.getCurrentElapsedTime().count()) + command, LogLevel_INFO);   //#Release remove logging and creation variable

                if (!dataReturned) {
                    if (pipeHandler.sendData(commandResult)) {
                        log_string("Info to ARMA send", LogLevel_DEBUG);
                    } else {
                        log_string("Can't send info to ARMA", LogLevel_ERROR);
                    }
                }
#endif

            }

        }
    }
}

// MUMBLE-TFAR: PTT release delay and resource path come from the Mumble client
// (implemented in TeamspeakMumble.cpp) instead of the TeamSpeak settings database.
namespace ts3compat {
    std::chrono::milliseconds getPTTHoldDelay();
    std::string getPluginResourcePath();
}

/*
 * Custom code called right after loading the plugin. Returns 0 on success, 1 on failure.
 * If the function returns 1 on failure, the plugin will be unloaded again.
 */
extern struct TS3Functions ts3Functions;

/* Plugin API version. Must be the same as the clients API major version, else the plugin fails to load. */

bool pluginInitialized = false;
int ts3plugin_init() {

    pluginInitialized = true;
    // MUMBLE-TFAR: resource path (radio-sounds etc.) is provided by the Mumble client.
    TFAR::getInstance().setPluginPath(ts3compat::getPluginResourcePath());

#if ENABLE_API_PROFILER
    Logger::registerLogger(LoggerTypes::profiler, std::make_shared<FileLogger>("P:/profiler.log"));
    Logger::registerLogger(LoggerTypes::gameCommands, std::make_shared<FileLogger>("P:/gameCommands.log"));
    Logger::registerLogger(LoggerTypes::pluginCommands, std::make_shared<FileLogger>("P:/pluginCommands.log"));
    Logger::registerLogger(LoggerTypes::teamspeakClientlog, std::make_shared<TeamspeakLogger>(LogLevel::LogLevel_INFO));
#endif
#if !ENABLE_API_PROFILER && ENABLE_PLUGIN_LOGS
    //#TODO disable both logs on release. Maybe add 2k ring buffered logger for /tfar full
    Logger::registerLogger(LoggerTypes::pluginCommands, std::make_shared<FileLogger>(std::string(getenv("appdata")) + "\\TS3Client\\TFAR_pluginCommands.log"));
    Logger::registerLogger(LoggerTypes::gameCommands, std::make_shared<FileLogger>(std::string(getenv("appdata")) + "\\TS3Client\\TFAR_gameCommands.log"));
#endif

    TFAR::getServerDataDirectory();//initializes the ServerdataDirectory so it connects its Slots to TFAR's Signals

    Teamspeak::_onInit();

    exitThread = false;
    threadPipeHandle = std::thread(&PipeThread);
    threadService = std::thread(&ServiceThread);

    TFAR::getInstance().onGameDisconnected.connect([]() {
        TFAR::getInstance().m_gameData.alive = false;
        TFAR::getInstance().m_gameData.currentDataFrame = INVALID_DATA_FRAME;
    });

    // MUMBLE-TFAR: take the PTT release delay from Mumble's audio settings ("hold time")
    // instead of reading TeamSpeak's settings.db via sqlite.
    TFAR::getInstance().m_gameData.pttDelay = ts3compat::getPTTHoldDelay();

    TFAR::getInstance().onTeamspeakClientJoined.connect([](TSServerID serverID, TSClientID clientID, [[maybe_unused]] const std::string& clientNickname) {
        setGameClientMuteStatus(serverID, clientID, { false, false }); //Mute non TFAR users if they join in serious channel
    });

    TFAR::getInstance().onSeriousModeChanged.connect([]([[maybe_unused]] bool isSerious) {
        auto serverID = Teamspeak::getCurrentServerConnection();
        TFAR::getServerDataDirectory()->getClientDataDirectory(serverID)->forEachClient(
            [serverID](const std::shared_ptr<clientData>& client) {
            if (client)
                setGameClientMuteStatus(serverID, client->clientId, { false, false });
        });
    });

    RadioEffect::onError = [](std::string err) {
        Teamspeak::printMessageToCurrentTab(err.c_str());
    };

    return 0;
}

/* Custom code called right before the plugin is unloaded */
void ts3plugin_shutdown() {
    if (!pluginInitialized) return;//For some reason shutdown is called before init.
    pluginInitialized = false;
    /* Your plugin cleanup code here */
    Logger::log(LoggerTypes::teamspeakClientlog, "shutdown...", LogLevel_DEVEL);

    exitThread = true;
    if (threadPipeHandle.joinable())
        threadPipeHandle.join();
    if (threadService.joinable())
        threadService.join();
    TFAR::getCommandProcessor()->stopThread();
    TFAR::getInstance().onShutdown();//Call shutdown Signal

    TFAR::getInstance().m_gameData.alive = false;
    pipeConnected = false;
    updateUserStatusInfo(false);
    Teamspeak::unmuteAll();
}

/*
 * Dynamic content shown in the right column in the info frame. Memory for the data string needs to be allocated in this
 * function. The client will call ts3plugin_freeMemory once done with the string to release the allocated memory again.
 * Check the parameter "type" if you want to implement this feature only for specific item types. Set the parameter
 * "data" to NULL to have the client ignore the info data.
 */
void ts3plugin_infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data) {
    if (PLUGIN_CLIENT == type) {
        std::string metaData = Teamspeak::getMetaData(serverConnectionHandlerID, static_cast<anyID>(id));
        *data = static_cast<char*>(malloc(INFODATA_BUFSIZE * sizeof(char)));  /* Must be allocated in the plugin! */
        snprintf(*data, INFODATA_BUFSIZE, "%s", metaData.c_str());  /* bbCode is supported. HTML is not supported */
    } else {
        *data = nullptr;
    }
}

/************************** TeamSpeak callbacks ***************************/
/*
 * Following functions are optional, feel free to remove unused callbacks.
 * See the clientlib documentation for details on each function.
 */

 /* Clientlib */


bool isPluginEnabledForUser(TSServerID serverConnectionHandlerID, TSClientID clientID) {
    ProfileFunction;
    auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnectionHandlerID);
    if (!clientDataDir) return false;
    auto clientData = clientDataDir->getClientData(clientID);
    if (!clientData) return false;


    auto currentTime = std::chrono::system_clock::now();
    bool result;

    if (currentTime - clientData->pluginEnabledCheck < 10s) {
        result = clientData->pluginEnabled;
    } else {
        std::string clientInfo = Teamspeak::getMetaData(Teamspeak::getCurrentServerConnection(), clientID);
        if (clientInfo.empty()) return false;
        std::string shouldStartWith = getConnectionStatusInfo(true, true, false);  //slow
        result = clientData->pluginEnabled = helpers::startsWith(shouldStartWith, clientInfo);
    }

    clientData->pluginEnabledCheck = currentTime;

    return result;
}

#pragma region VoiceProcessing
//packet receive -> decode -> onEditPlaybackVoiceDataEvent -> 3D positioning -> onEditPostProcessVoiceDataEvent -> mixing -> onEditMixedPlaybackVoiceDataEvent -> speaker output
void ts3plugin_onEditMixedPlaybackVoiceDataEvent(uint64 serverConnectionHandlerID, short* samples, int sampleCount, int channels, const unsigned int* channelSpeakerArray, unsigned int* channelFillMask) {
    if (Teamspeak::getCurrentServerConnection() != serverConnectionHandlerID) return;
    TFAR::getPlaybackHandler()->onEditMixedPlaybackVoiceDataEvent(samples, sampleCount, channels, channelSpeakerArray, channelFillMask);
}

#define LOG3DMUTE(x)  clientData->circularLog(x);

void processVoiceData(TSServerID serverConnectionHandlerID, TSClientID clientID, short* samples, int sampleCount, int channels, bool isFromMicrophone = false) {
    if (!TFAR::getInstance().getCurrentlyInGame())
        return;
    ProfileFunction;
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _mm_setcsr((_mm_getcsr() & ~0x0040) | (0x0040));//_MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    static std::chrono::system_clock::time_point last_no_info;
    const auto myId = Teamspeak::getMyId(serverConnectionHandlerID);
    auto myNickname = Teamspeak::getMyNickname(serverConnectionHandlerID);

    const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnectionHandlerID);
    if (!clientDataDir) return;//Unknown server
    //Senders Data
    auto clientData = clientDataDir->getClientData(clientID);
    if (!clientData) {
        //Should not happen..
        log_string(std::string("No info about ") + std::to_string(clientID.baseType()) + " " + Teamspeak::getClientNickname(serverConnectionHandlerID, clientID), LogLevel_ERROR);
        return; //Unknown client
    }

    auto sampleBuffer = SampleBuffer(samples, sampleCount, channels);
    const bool alive = TFAR::getInstance().m_gameData.alive && clientDataDir->myClientData;//If i don't know who i am... I am not alive

    const bool hasPluginEnabled = isPluginEnabledForUser(serverConnectionHandlerID, clientID);

    if (!clientData || !clientDataDir->myClientData || !hasPluginEnabled) {
        if (clientID == myId) {
            sampleBuffer.setToNull();
            return;
        }


        if (isSeriousModeEnabled(serverConnectionHandlerID, clientID)) {
            if (alive && TFAR::getInstance().getCurrentlyInGame() && hasPluginEnabled)
                sampleBuffer.setToNull(); // alive player hears only alive players in serious mode
        }
        if (std::chrono::system_clock::now() - last_no_info > MILLIS_TO_EXPIRE) {
            std::string nickname = clientData->getNickname();
            if (!hasPluginEnabled)
                log_string(std::string("No plugin enabled for ") + std::to_string(clientID.baseType()) + " " + nickname, LogLevel_DEBUG);
            last_no_info = std::chrono::system_clock::now();
        }
        LOG3DMUTE("TFAR nosim NoPlugEn");
        return;
    }
    const bool canSpeak = clientDataDir->myClientData->canSpeak;
    //If we are dead we can't hear anyone in seriousMode. And if we are alive we can't hear the dead.

    const bool isSpectator = clientData->isSpectating;

    // If I am dead and the speaker spectating, ignore if other spectators should be muted
    if ((TFAR::config.get<bool>(Setting::muteSpectators)) && clientDataDir->myClientData->isSpectating && isSpectator) {
        sampleBuffer.setToNull();
        return;
    }

    //NonPure normalPlayer->Spectator
    const bool isNotHearableInNonPureSpectator = clientDataDir->myClientData->isSpectating && ((clientData->isEnemyToPlayer && TFAR::config.get<bool>(Setting::spectatorNotHearEnemies)) || (!clientData->isEnemyToPlayer && !TFAR::config.get<bool>(Setting::spectatorCanHearFriendlies)));
    //Other player is also a spectator. So we always hear him without 3D positioning
    const bool isHearableInPureSpectator = clientDataDir->myClientData->isSpectating && clientData->isSpectating;
    const bool isHearableInSpectator = isHearableInPureSpectator || !isNotHearableInNonPureSpectator;

    //Only silence the speaker when we are dead ourselves or he is confirmed
    //dead (explicit KILLED event). Merely stale position data must not mute
    //him: on large events the position stream lags behind and treating that
    //as death made living players inaudible for the whole channel.
    if (!isHearableInSpectator && isSeriousModeEnabled(serverConnectionHandlerID, clientID) && (!alive || clientData->isConfirmedDead())) {
        sampleBuffer.setToNull();
        std::string message = "TFAR Mute L470";
        message += " isHearableInSpectator=" + std::to_string(isHearableInSpectator);
        message += " alive=" + std::to_string(alive);
        message += " cdalive=" + std::to_string(clientData->isAlive());
        message += " cddead=" + std::to_string(clientData->isConfirmedDead());

        LOG3DMUTE(message);
        return;
    }
    auto myData = clientDataDir->myClientData;
    auto myPosition = myData->getClientPosition();

    if (!clientData || !myData) return;
    sampleBuffer.applyGain(clientData->voiceVolumeMultiplifier);
    auto originalBuffer = sampleBuffer.copy();

    const bool shouldPlayerHear = clientData->canSpeak && canSpeak;

    const auto myVehicleDescriptor = myData->getVehicleDescriptor();
    const auto hisVehicleDesriptor = clientData->getVehicleDescriptor();

    const auto vehicleVolumeLoss = std::clamp(myVehicleDescriptor.vehicleIsolation + hisVehicleDesriptor.vehicleIsolation, 0.0f, 0.99f);
    const bool isInSameVehicle = myVehicleDescriptor.vehicleName == hisVehicleDesriptor.vehicleName && myVehicleDescriptor.vehicleName != "no";
    const auto distanceFromClient = myPosition.distanceTo(clientData->getClientPosition()) + 2 * clientData->objectInterception; //2m more dist for each obstacle

    //#### DIRECT SPEECH
    if (myId != clientID &&
        !isSpectator && !isNotHearableInNonPureSpectator && //We don't hear spectators and enemy units(if enabled in config)....
        distanceFromClient <= (clientData->voiceVolume + 15)
        ) {
        //Direct Speech
        ProfileScopeN("direct speech");

        if (shouldPlayerHear) {
            if (vehicleVolumeLoss < 0.01 || isInSameVehicle) {

                //auto atten = helpers::volumeAttenuation(distanceFromClient, shouldPlayerHear, clientData->voiceVolume);
                //if (atten < 0.15)
                //    LOG3DMUTE("TFAR SemiMute atten s1 <0.15");
                //helpers::applyGain(samples, sampleCount, channels, atten);
                if (!isInSameVehicle && clientData->objectInterception > 0) {
                    auto filter = clientData->effects.getFilterObjectInterception(clientData->objectInterception);
                    if (clientData->OISampleBuffer.getSampleCount() != 0) {
                        helpers::processFilterStereo(clientData->OISampleBuffer, 1.0f, filter, false); //pre-seed the filter with last sample
                    }
                    sampleBuffer.copy(clientData->OISampleBuffer);
                    helpers::processFilterStereo(sampleBuffer, 1.0f, filter); //getFilterObjectInterception
                } else {
                    sampleBuffer.copy(clientData->OISampleBuffer);
                }
            } else {
                helpers::processFilterStereo(sampleBuffer, helpers::volumeAttenuation(distanceFromClient, shouldPlayerHear, clientData->voiceVolume, 1.0f - vehicleVolumeLoss) * pow(1.0f - vehicleVolumeLoss, 1.2f), clientData->effects.getFilterVehicle("local_vehicle", vehicleVolumeLoss));
            }
        } else {
            LOG3DMUTE("TFAR SemiMute shouldn't hear");
            helpers::processFilterStereo(sampleBuffer, helpers::volumeAttenuation(distanceFromClient, shouldPlayerHear, clientData->voiceVolume) * CANT_SPEAK_GAIN, clientData->effects.getFilterCantSpeak("local_cantspeak"));
        }


        //process voice
        const auto relativePosition = myPosition.directionTo(clientData->getClientPosition());
        const auto myViewDirection = myData->getViewDirection();
        //Time differential based on direction
        clientData->effects.getClunk("voice_clunk")->process(sampleBuffer, relativePosition, myViewDirection);//interaural time difference
        //Volume differential based on direction
        helpers::applyILD(sampleBuffer, myPosition, myViewDirection, clientData->getClientPosition(), clientData->getViewDirection(), shouldPlayerHear, clientData->voiceVolume);

        //helpers::applyILD(samples, sampleCount, channels, relativePosition, myViewDirection);//interaural level difference

        processCompressor(&clientData->effects.compressor, sampleBuffer);




    } else if (!isHearableInPureSpectator) { //.... unless we are both spectating

        std::string message = "TFAR Mute L534";
        message += " isSpectator=" + std::to_string(isSpectator);
        message += " isNotHearableInNonPureSpectator=" + std::to_string(isNotHearableInNonPureSpectator);
        message += " distanceFromClient_=" + std::to_string(distanceFromClient);
        message += " <= voiceVolume=" + std::to_string(clientData->voiceVolume + 15);

        LOG3DMUTE(message);
        memset(samples, 0, channels * sampleCount * sizeof(short));
    }

    //#### RADIOS AND SPEAKERS

    // process radio here
    processCompressor(&clientData->effects.compressor, originalBuffer);

    std::vector<LISTED_INFO> listed_info = isSpectator ? std::vector<LISTED_INFO>{} : clientData->isOverRadio(myData, false, false);
    const auto radioDistance = myData->effectiveDistanceTo(clientData);
    if (listed_info.empty() && distanceFromClient > 30) LOG3DMUTE("TFAR NOR");
    for (auto& info : listed_info) {
        ProfileScopeN("listed info");
        if (isFromMicrophone && info.on == receivingRadioType::LISTED_ON_INTERCOM) continue; //We don't want to hear ourselves over intercom while doing direct speech
        auto radio_buffer = originalBuffer.copy();
        auto volumeLevel = helpers::volumeMultiplifier(static_cast<float>(info.volume));
        if (volumeLevel < 0.2) {
            LOG3DMUTE("VOL Low b=" + std::to_string(info.volume) + " mp =" + std::to_string(volumeLevel) + " efmp =" + std::to_string(volumeLevel* 0.35f));
        }

        std::stringstream processLog;

        if (info.on < receivingRadioType::LISTED_ON_NONE) {//don't do for onGround or Intercom

            //Volume modifier for lowered headset - Placed here because this part of code only applies to actual non-speaker Radios
            if (TFAR::config.get<bool>(Setting::headsetLowered)) {
                LOG3DMUTE("TFAR RSilent HeadLow");
                volumeLevel *= 0.1f;
            }


            switch (PTTDelayArguments::stringToSubtype(clientData->getCurrentTransmittingSubtype())) {
                case PTTDelayArguments::subtypes::digital: {
                    processLog << "digital ";
                    if (info.over == sendingRadioType::LISTEN_TO_SW) {
                        processLog << "sw rd=" << radioDistance << " rn="<< clientData->range;
                        const auto errorLevel = std::min(info.antennaConnection.getLoss(), effectErrorFromDistance(info.over, radioDistance, clientData));
                        
                        processLog << " rl=" << errorLevel;
                        clientData->effects.getSwRadioEffect(info.radio_id)->setErrorLeveL(errorLevel);

                        processRadioEffect(radio_buffer, volumeLevel * 0.35f, clientData->effects.getSwRadioEffect(info.radio_id), info.stereoMode);
                    } else {
                        processLog << "uw ";
                        const auto underwaterDist = myPosition.distanceUnderwater(clientData->getClientPosition());
                        const auto normalDist = myPosition.distanceTo(clientData->getClientPosition());
                        clientData->effects.getUnderwaterRadioEffect(info.radio_id)->setErrorLeveL(
                            std::min((underwaterDist * (clientData->range / helpers::distanceForDiverRadio()) + (normalDist - underwaterDist)) / clientData->range
                            , info.antennaConnection.getLoss())
                        );
                        processRadioEffect(radio_buffer, volumeLevel * 0.35f, clientData->effects.getUnderwaterRadioEffect(info.radio_id), info.stereoMode);
                    }
                }
                                                           break;
                case PTTDelayArguments::subtypes::airborne:
                    clientData->effects.getAirborneRadioEffect(info.radio_id)->setErrorLeveL(std::min(info.antennaConnection.getLoss(), effectErrorFromDistance(info.over, radioDistance, clientData)));
                    processRadioEffect(radio_buffer, volumeLevel * 0.35f, clientData->effects.getAirborneRadioEffect(info.radio_id), info.stereoMode);
                    break;
                case PTTDelayArguments::subtypes::digital_lr:
                    processLog << "lr rd=" << radioDistance << " rn=" << clientData->range;
                    clientData->effects.getLrRadioEffect(info.radio_id)->setErrorLeveL(std::min(info.antennaConnection.getLoss(), effectErrorFromDistance(info.over, radioDistance, clientData)));
                    processRadioEffect(radio_buffer, volumeLevel * 0.35f, clientData->effects.getLrRadioEffect(info.radio_id), info.stereoMode);
                    break;
                case  PTTDelayArguments::subtypes::phone:
                    helpers::processFilterStereo(radio_buffer, volumeLevel * 10.0f, clientData->effects.getSpeakerPhone(info.radio_id));
                    break;
                default:
                    LOG3DMUTE("TFAR RMute unkwn subtype");
                    radio_buffer.setToNull();
                    break;
            }
        }

        if (info.on == receivingRadioType::LISTED_ON_GROUND) {

            const auto distanceFromRadio = myPosition.distanceTo(info.pos);


            bool sameVehicleAsMe = myVehicleDescriptor.vehicleName == info.vehicle.vehicleName;

            //If we are inside the vehicle, we hear it over the internal speakers, 0 isolation.
            const auto radioVehicleVolumeLoss =
                sameVehicleAsMe ? 0
                    : std::clamp(myVehicleDescriptor.vehicleIsolation + info.vehicle.vehicleIsolation, 0.0f, 0.99f);

            helpers::processFilterStereo(radio_buffer, SPEAKER_GAIN, clientData->effects.getSpeakerFilter(info.radio_id));
            //Special handling for Radios that are louder than normal max. == statically placed radios
            const auto speakerDistance = (info.volume < 20) ? (info.volume / 10.f) * TFAR::getInstance().m_gameData.speakerDistance : info.volume*1.9f;
            if (radioVehicleVolumeLoss < 0.01f || isInSameVehicle) {
                radio_buffer.applyGain(helpers::volumeAttenuation(distanceFromRadio, shouldPlayerHear, speakerDistance));
            } else {
                helpers::processFilterStereo(radio_buffer, helpers::volumeAttenuation(distanceFromRadio, shouldPlayerHear, speakerDistance, 1.0f - radioVehicleVolumeLoss) * pow(1.0f - radioVehicleVolumeLoss, 1.2f), clientData->effects.getFilterVehicle(info.radio_id, radioVehicleVolumeLoss));
            }
            if (info.waveZ < UNDERWATER_LEVEL) {
                helpers::processFilterStereo(radio_buffer, CANT_SPEAK_GAIN, clientData->effects.getFilterCantSpeak(info.radio_id));
            }


            //Also need distance loss on speakers
            const auto errorLevel = std::min(info.antennaConnection.getLoss(), effectErrorFromDistance(info.over, radioDistance, clientData));
            clientData->effects.getSwRadioEffect(info.radio_id)->setErrorLeveL(errorLevel);
            processRadioEffect(radio_buffer, volumeLevel * 0.35f, clientData->effects.getSwRadioEffect(info.radio_id), info.stereoMode);

            const auto relativePosition = myPosition.directionTo(info.pos);
            const auto myViewDirection = myData->getViewDirection();

            clientData->effects.getClunk(info.radio_id)->process(radio_buffer, relativePosition, myViewDirection);//interaural time difference
            helpers::applyILD(radio_buffer, relativePosition, myViewDirection);//interaural level difference

        } else if (info.on == receivingRadioType::LISTED_ON_INTERCOM) {
            clientData->effects.getLrRadioEffect("intercom")->setErrorLeveL(0.f);
            auto intercomVolume = TFAR::config.get<float>(Setting::intercomVolume);
            if (TFAR::config.get<bool>(Setting::headsetLowered)) {
                intercomVolume *= 0.1f;
            }

            //Duck intercom if you have incoming transmission on LR
            if (
                std::find_if(listed_info.begin(), listed_info.end(), [](const LISTED_INFO& it) {
                    return it.on == receivingRadioType::LISTED_ON_LR && it.volume > 2;
                    }) != listed_info.end()
                )
                intercomVolume *= 1.f - TFAR::config.get<float>(Setting::intercomDucking);

            processRadioEffect(radio_buffer, intercomVolume, clientData->effects.getLrRadioEffect("intercom"), stereoMode::stereo);
        }

        radio_buffer.mixIntoAdditive(sampleBuffer);//Mix current Radio into samples
    }

    const auto globalGain = TFAR::getInstance().m_gameData.globalVolume;
    sampleBuffer.applyGain(globalGain);




    //minmax = std::minmax_element(samples, samples + (sampleCount * channels));
    //
    //if (*minmax.first < -20000 || *minmax.second > 20000) {
    //    Teamspeak::printMessageToCurrentTab(("Weird audio post? " + std::to_string(*minmax.first) + "|" + std::to_string(*minmax.second)).c_str());
    //}

}

//packet receive -> decode -> onEditPlaybackVoiceDataEvent -> 3D positioning -> onEditPostProcessVoiceDataEvent -> mixing -> onEditMixedPlaybackVoiceDataEvent -> speaker output
//Data from other clients to us. After 3D processing
void ts3plugin_onEditPostProcessVoiceDataEvent(uint64 serverConnectionHandlerID, anyID clientID, short* samples, int sampleCount, int channels, const unsigned int*, unsigned int*) {


    //auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnectionHandlerID);
    //if (!clientDataDir) return; //Unknown server
    //                            //Senders Data
    //auto clientData = clientDataDir->getClientData(clientID);
    //
    ////auto errorLevel = info.antennaConnection.isNull() ? effectErrorFromDistance(info.over, radioDistance, clientData) : info.antennaConnection.getLoss();
    //clientData->effects.getSwRadioEffect("local_radio")->setErrorLeveL(0.2f);
    //
    //processRadioEffect(samples, channels, sampleCount, 0.540100 * 0.35f, clientData->effects.getSwRadioEffect("local_radio"), stereoMode::stereo);
    //
    //processCompressor(&clientData->effects.compressor, samples, channels, sampleCount);


    if (!TFAR::getInstance().getCurrentlyInGame())
        return;

    ProfileFunction;

    if (channels != 2) {
        short* stereo = new short[sampleCount * 2];
        for (auto q = 0; q < sampleCount; q++) {
            for (auto g = 0; g < 2; g++)
                stereo[q * 2 + g] = samples[q * channels + g];
        }

        processVoiceData(serverConnectionHandlerID, clientID, stereo, sampleCount, 2);
        for (auto q = 0; q < sampleCount; q++) {
            for (auto g = 0; g < 2; g++)
                samples[q * channels + g] = stereo[q * 2 + g];
            for (auto g = 2; g < channels; g++)
                samples[q * channels + g] = 0;
        }
        delete[] stereo;
    } else {
        processVoiceData(serverConnectionHandlerID, clientID, samples, sampleCount, 2);
    }
}

//Data from our microphone before its sent
void ts3plugin_onEditCapturedVoiceDataEvent(uint64 serverConnectionHandlerID, short* samples, int sampleCount, int channels, int* edited) {
    if (!TFAR::getInstance().getCurrentlyInGame() || !(*edited & 2))
        return;

    ProfileFunction;

    if (!TFAR::getInstance().m_gameData.alive || TFAR::getInstance().m_gameData.speakers.empty()) return;
    auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnectionHandlerID);
    if (!clientDataDir || !clientDataDir->myClientData) return;

    //processVoiceData is always modifying data so we need copy.
    std::vector<short> voice;
    voice.resize(sampleCount * 2);
    if (channels == 1) {  //copy to stereo
        for (auto q = 0; q < sampleCount; q++) {
            voice[q * 2] = samples[q];
            voice[q * 2 + 1] = samples[q];
        }
    } else if (channels == 2) {
        memcpy(voice.data(), samples, sampleCount * 2 * sizeof(short));
    } else {
        uint32_t posInTarget = 0;
        for (auto q = 0; q < sampleCount*channels; q += channels) {
            voice[posInTarget++] = samples[q];//copy left channel
            voice[posInTarget++] = samples[q + 1];//copy right channel
        }
    }

    processVoiceData(serverConnectionHandlerID, clientDataDir->myClientData->clientId, voice.data(), sampleCount, 2, true);
    TFAR::getPlaybackHandler()->appendPlayback("my_radio_voice", SoundFile(voice.data(), sampleCount, 2));
}

void ts3plugin_onCustom3dRolloffCalculationClientEvent(uint64 serverConnectionHandlerID, anyID clientID, float distance, float* volume) {
    *volume = 1.0f;// custom gain applied
}

#pragma endregion VoiceProcessing

#pragma region pluginCommands
/* Clientlib rare */
void ts3plugin_onClientSelfVariableUpdateEvent(uint64 serverConnectionHandlerID, int flag, const char* oldValue, const char* newValue) {
    ProfileFunction;
    if (flag == CLIENT_FLAG_TALKING && TFAR::getInstance().getCurrentlyInGame()) {
        std::string one = "1";
        const bool start = one == newValue;

        const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnectionHandlerID);
        if (clientDataDir && clientDataDir->myClientData) {
            clientDataDir->myClientData->clientTalkingNow = start;
        }
        const auto serverId = Teamspeak::getCurrentServerConnection();
        const auto myNickname = Teamspeak::getMyNickname(serverId);
        const auto command = "VOLUME\t" + myNickname + "\t" + std::to_string(TFAR::getInstance().m_gameData.myVoiceVolume) + "\t" + (start ? "true" : "false");
        Logger::log(LoggerTypes::pluginCommands, "Send " + command);
        Teamspeak::sendPluginCommand(serverId, TFAR::getInstance().getPluginID(), command, PluginCommandTarget_CURRENT_CHANNEL);
    }

    /*
    Don't know what this was supposed to be. So I'll just leave it here - Dedmen
    Edit: This re-activates talking when client stops pressing PTT. Still don't know what use this should serve.
    if (flag == CLIENT_FLAG_TALKING && *newValue == '0' && inGame) {
        uint64 serverId = ts3Functions.getCurrentServerConnectionHandlerID();
        bool set_talk_to_true = false;
        if (TFAR::getInstance().m_gameData.tangentPressed) {
            set_talk_to_true = true;
        }
        if (set_talk_to_true) {
            / *DWORD error;
            if ((error = ts3Functions.setClientSelfVariableAsInt(ts3Functions.getCurrentServerConnectionHandlerID(), CLIENT_INPUT_DEACTIVATED, INPUT_ACTIVE)) != ERROR_ok) {
                log("Can't active talking by tangent", error);
            }
            error = ts3Functions.flushClientSelfUpdates(ts3Functions.getCurrentServerConnectionHandlerID(), NULL);
            if (error != ERROR_ok && error != ERROR_ok_no_update) {
                log("Can't flush self updates", error);
            };* /
        }
    }
    */
}

void processAllTangentRelease(TSServerID serverId, const std::vector<std::string_view> &tokens) {
    const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverId);
    if (!clientDataDir) return;
    const auto& nickname = tokens[1];//only copying for readability.. hope compiler optimizes that out
    auto clientData = clientDataDir->getClientData(nickname);
    if (!clientData) return;

    clientData->currentTransmittingTangentOverType = sendingRadioType::LISTEN_TO_NONE;
}

void processTangentPress(TSServerID serverId, const std::vector<std::string_view> &tokens, std::string_view command) {
    ProfileFunction;
    std::vector<LISTED_INFO> listedInfos;
    const auto nickname = tokens.back();
    //Input validation first.
    const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverId);
    if (!clientDataDir) return;
    auto senderClientData = clientDataDir->getClientData(nickname);
    if (!senderClientData) {
        log_string(std::string("PLUGIN FROM UNKNOWN NICKNAME ").append(nickname));
        return;
    }
    auto myClientData = clientDataDir->myClientData;
    if (!myClientData) //if we don't know who we are we also can't hear anything
        return;

    if (myClientData == senderClientData) //We received our own tangent press.
        return;

    senderClientData->circularLog(std::string("Inc Tangent ").append(command));

    const auto time = std::chrono::system_clock::now();
    const bool pressed = tokens[1] == "PRESSED";

    if (!pressed) //Need to do this before we reset all his variables because after that we won't receive him anymore
        listedInfos = senderClientData->isOverRadio(myClientData, senderClientData->canUseSWRadio, senderClientData->canUseLRRadio);

    sendingRadioType sendingRadioType;
    if (tokens[0] == "TANGENT_LR")
        sendingRadioType = sendingRadioType::LISTEN_TO_LR;
    else
        sendingRadioType = sendingRadioType::LISTEN_TO_SW;

    auto frequency = tokens[2];
    const auto range = helpers::parseArmaNumberToInt(tokens[3]);
    const auto subtype = tokens[4];


    auto playPressed = false;

    const auto alive = TFAR::getInstance().m_gameData.alive;



    senderClientData->pluginEnabled = true;//He just sent us a Plugin command... Either he has Plugin enabled or he is a hacker
    senderClientData->pluginEnabledCheck = time;
    senderClientData->setLastPositionUpdateTime(time);
    senderClientData->setCurrentTransmittingSubtype(subtype);






    //If he could press the tangent.. He is obviously able to use that radio... unless he is using telekinesis...
    if (sendingRadioType == sendingRadioType::LISTEN_TO_LR) senderClientData->canUseLRRadio = true;
    senderClientData->canUseSWRadio = tokens[6] == "1";
    senderClientData->canUseDDRadio = tokens[7] == "1";




    if ((senderClientData->currentTransmittingTangentOverType != sendingRadioType::LISTEN_TO_NONE) != pressed) {
        playPressed = pressed;
    }

    //tell his clientData where he is transmitting from
    if (pressed) {
        senderClientData->currentTransmittingTangentOverType = sendingRadioType;
        senderClientData->setCurrentTransmittingFrequency(frequency);
    } else {
        senderClientData->currentTransmittingTangentOverType = sendingRadioType::LISTEN_TO_NONE;
        //prev_ is just for debugging. Might aswell be empty string
        senderClientData->setCurrentTransmittingFrequency(std::string("prev_").append(frequency));
    }

    senderClientData->range = pressed ? range : 0; //Setting range to 0 on transmit end helps identifying if he is currently sending

    const auto clientId = senderClientData->clientId;
    bool transmissionCounted = false;

    std::stringstream tangLog;

    tangLog << "pressed " << pressed << "\n";
    tangLog << "playPressed " << playPressed << "\n";
    tangLog << "pressed " << pressed << "\n";
    tangLog << "pressed " << pressed << "\n";



    //Check where we can Receive him. Radios or Speakers
    if (pressed)
        listedInfos = senderClientData->isOverRadio(myClientData, senderClientData->canUseSWRadio, senderClientData->canUseLRRadio);
    for (LISTED_INFO & listedInfo : listedInfos) {
        std::stringstream recvLog;
        recvLog << "RECV\n##\n" << listedInfo << "##\n";

        if (alive && listedInfo.on != receivingRadioType::LISTED_ON_NONE && listedInfo.on != receivingRadioType::LISTED_ON_INTERCOM) {
            const auto vehicleDescriptor = myClientData->getVehicleDescriptor();

            auto vehicleVolumeLoss = 0.f;
            bool vehicleCheck = true;

            if (listedInfo.on == receivingRadioType::LISTED_ON_GROUND) {
                vehicleVolumeLoss = std::clamp(vehicleDescriptor.vehicleIsolation + listedInfo.vehicle.vehicleIsolation, 0.0f, 0.99f);
                vehicleCheck = vehicleDescriptor.vehicleName == listedInfo.vehicle.vehicleName;
            }

            const auto gain = helpers::volumeMultiplifier(static_cast<float>(listedInfo.volume)) * TFAR::getInstance().m_gameData.globalVolume;
            recvLog << "gain " << gain << "\n";
            recvLog << "gvol " << TFAR::getInstance().m_gameData.globalVolume << "\n";
            if (listedInfo.on == receivingRadioType::LISTED_ON_GROUND) {
                recvLog << "vehicleCheck " << vehicleCheck << "\n";
                recvLog << "vehicleVolumeLoss " << vehicleVolumeLoss << "\n";
            }
            

            if (playPressed && !transmissionCounted) {
                //#TODO keep an array of client weak pointers of the receiving transmissions.
                //#TODO what happens if WE change frequency while the other guy is still transmitting. Or when we change frequency into a currently active transmission
                myClientData->receivingTransmission += 1; //Set that we are receiving a transmission. For the EventHandler
                myClientData->receivingFrequencies.emplace(frequency);
                transmissionCounted = true;//prevent from multiple adds if you have multiple radios on same freq
                senderClientData->circularLog("TPEH Recv g=" + std::to_string(gain));
            }

            auto soundToPlay = SoundDirectory::getRemoteRadioSound(PTTDelayArguments::stringToSubtype(subtype), playPressed);
            if (!soundToPlay.isEmpty())
                TFAR::getPlaybackHandler()->playWavFile(serverId, soundToPlay, gain, listedInfo.pos, listedInfo.on == receivingRadioType::LISTED_ON_GROUND, listedInfo.volume, listedInfo.waveZ < UNDERWATER_LEVEL, vehicleVolumeLoss, vehicleCheck, listedInfo.stereoMode);

        }
        recvLog << "END\n";
        senderClientData->circularLog(recvLog.str());
    }
    if (!playPressed) {
        auto found = myClientData->receivingFrequencies.find(frequency);
        if (found != myClientData->receivingFrequencies.end()) {
            myClientData->receivingTransmission -= 1;
            myClientData->receivingFrequencies.erase(found);
        }

        senderClientData->circularLog("TPEH NRecv");
    }

    setGameClientMuteStatus(serverId, clientId, { true,!listedInfos.empty() });
}

void processPluginCommand(std::string_view command) {
    ProfileFunction;
    auto tokens = helpers::split(command, '\t'); // may not be used in nickname
    const auto serverId = Teamspeak::getCurrentServerConnection();

    const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverId);
    if (!clientDataDir) return;


    //PRESSED is 7 tokens with half-duplex plus 2 tokens for canUseDD and canUseSW
    if ((tokens.size() == 8 || tokens.size() == 9) && (tokens[0] == "TANGENT"sv || tokens[0] == "TANGENT_LR"sv || tokens[0] == "TANGENT_DD"sv)) {
        processTangentPress(serverId, tokens, command);
    } else if (tokens.size() == 2 && tokens[0] == "RELEASE_ALL_TANGENTS"sv) {
        processAllTangentRelease(serverId, tokens);
    } else if (tokens.size() == 4 && tokens[0] == "VOLUME"sv) {
        auto nickname = tokens[1];
        auto clientData = clientDataDir->getClientData(nickname);
        if (!clientData) return; //Don't know who the sender is.. so we don't care

        const auto volume = tokens[2];
        const bool start = helpers::isTrue(tokens[3]);
        const bool myCommand = clientData == clientDataDir->myClientData;

        clientData->voiceVolume = std::atoi(volume.data());
        clientData->pluginEnabled = true;
        clientData->pluginEnabledCheck = std::chrono::system_clock::now();
        clientData->clientTalkingNow = start;
        if (!myCommand) {
            setGameClientMuteStatus(serverId, clientData->clientId);
        }
    } else if (tokens.size() == 2 && tokens[0] == "REQVOL"sv) {//#TODO request volume onGameStart for each client not currently known
        if (!clientDataDir->myClientData)
            return;
        const TSClientID clientID = std::stoi(std::string(tokens[1].data(), tokens[1].length()));
        const auto myNickname = clientDataDir->myClientData->getNickname();
        const bool curTalking = clientDataDir->myClientData->clientTalkingNow;
        const auto cmd = "VOLUME\t" + myNickname + "\t" + std::to_string(TFAR::getInstance().m_gameData.myVoiceVolume) + "\t" + (curTalking ? "true" : "false");

        Teamspeak::sendPluginCommand(Teamspeak::getCurrentServerConnection(), TFAR::getInstance().getPluginID(), cmd, PluginCommandTarget_CLIENT, { clientID });
    } else {
        log_string(std::string("UNKNOWN PLUGIN COMMAND ").append(command));
    }
}

void ts3plugin_onPluginCommandEventNew(uint64 serverConnectionHandlerID, const char* pluginName, const char* pluginCommand, anyID invokerClientID, const char* invokerName, const char* invokerUniqueIdentity) {
    ts3plugin_onPluginCommandEventOld(serverConnectionHandlerID, pluginName, pluginCommand);
}

void ts3plugin_onPluginCommandEventOld(uint64 serverConnectionHandlerID, const char* pluginName, const char* pluginCommand) {
    ProfileFunction;
    Logger::log(LoggerTypes::pluginCommands, std::string(pluginName) + ":" + std::string(pluginCommand));
    log_string(std::string("ON PLUGIN COMMAND ") + pluginName + " " + pluginCommand, LogLevel_DEVEL);
    if (Teamspeak::getCurrentServerConnection() == serverConnectionHandlerID) {
        if (strncmp(pluginName, "TFAR", "TFAR"sv.length()) == 0) {
            processPluginCommand(std::string_view(pluginCommand));
        } else {
            Logger::log(LoggerTypes::teamspeakClientlog, "Plugin command event failure", LogLevel_ERROR);
        }
    } else {
        Logger::log(LoggerTypes::teamspeakClientlog, "Plugin command unknown ID", LogLevel_ERROR);
    }
}

#pragma endregion pluginCommands


void testFunc() {
    TFAR::getPlaybackHandler()->playWavFile(Teamspeak::getCurrentServerConnection(),
        SoundFile("radio-sounds/lr/remote_start"),
        //0.2401f
        0.f
        , { 0,0,0 }, receivingRadioType::LISTED_ON_SW == receivingRadioType::LISTED_ON_GROUND, 6,
        1.f < UNDERWATER_LEVEL, 0.f, true, stereoMode::rightOnly);

}
