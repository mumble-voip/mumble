#include "CommandProcessor.hpp"
#include "helpers.hpp"
#include "serverData.hpp"
#include "clientData.hpp"
#include "task_force_radio.hpp"
#include "Logger.hpp"
#include "PlaybackHandler.hpp"
#include <public_rare_definitions.h>
#include "Locks.hpp"
#include "Teamspeak.hpp"
#include "antennaManager.h"
#include "version.h"
#include <iomanip>
#include <filesystem>
#include <string_view>

using namespace std::literals::string_view_literals;

volatile bool skipTangentOff = false;
volatile bool waitingForTangentOff = false;
CriticalSectionLock tangentCriticalSection{ "tangentCriticalSection" };
extern bool isSeriousModeEnabled(TSServerID serverConnectionHandlerID, TSClientID clientId);
extern void setGameClientMuteStatus(TSServerID serverConnectionHandlerID, TSClientID clientID, std::pair<bool, bool> isOverRadio = { false,false });

CommandProcessor::CommandProcessor() {
    vadEnabled = Teamspeak::hlp_checkVad();//Needed in case releaseAllTangents is called before the first tangent press

    TFAR::getInstance().doDiagReport.connect([this](std::stringstream& diag) {
        diag << "CP:\n";
        diag << TS_INDENT << "shouldRun: " << shouldRun << "\n";
        diag << TS_INDENT << "cmdQueueBacklog: " << commandQueue.size() << "\n";
        diag << TS_INDENT << "thread: " << myThread->get_id() << "\n";
    });


}


CommandProcessor::~CommandProcessor() {
    stopThread();
}

void CommandProcessor::stopThread() {
    if (!myThread)
        return;
    {
        std::lock_guard<std::mutex> lock(theadMutex);
        shouldRun = false;
    }
    threadWorkCondition.notify_one();
    if (myThread->joinable()) //This happened once.. When something else crashed and dllDetach was called unexpectedly
        myThread->join();
    myThread = nullptr;
}

void CommandProcessor::queueCommand(const std::string& command) {
    ProfileFunction;
    if (!myThread || !threadRunning) {
        shouldRun = true; // thread somehow exited? wtf..
        myThread = std::make_unique<std::thread>(&CommandProcessor::threadRun, this);
        threadRunning = true;
    }
    {
        std::lock_guard<std::mutex> lock(theadMutex);
        commandQueue.emplace(command);
    }
    threadWorkCondition.notify_one();
}



DEFINE_API_PROFILER(processCommand)
std::string CommandProcessor::processCommand(const std::string& command) {
    Logger::log(LoggerTypes::gameCommands, command);
    API_PROFILER(processCommand);
    ProfileFunction;
    ProfilerLog(command);
    std::vector<std::string_view> tokens; tokens.reserve(28);
    helpers::split(command, '\t', tokens, 2); //may not be used in nickname
    const auto gameCommand = toGameCommand(tokens[0], tokens.size());
    if (gameCommand == gameCommand::unknown) return "UNKNOWN COMMAND";


    switch (gameCommand) {
        case gameCommand::TS_INFO: return ts_info(tokens[1]);
        case gameCommand::POS:
            //POS nickname [x,y,z] [viewdirUnitvector(x,y,z)] canSpeak canUseSWRadio canUseLRRadio canUseDDRadio vehicleID terrainInterception voiceVolume objectInterception
            queueCommand(command);//do processing async
            // MUMBLE-TFAR: fnc_sendPlayerInfo in the Arma mod (dev.341 SQF) treats any POS reply
            // other than the literal "OK" as a plugin error and renders it as a permanent hint in
            // the bottom right corner of the screen. Speaking states are fetched separately via
            // IS_SPEAKING/IS_SPEAKING_BULK, so nothing is lost by acknowledging here.
            return "OK";
        case gameCommand::IS_SPEAKING: {
            const auto nickname = convertNickname(tokens[1]);
            const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(Teamspeak::getCurrentServerConnection());
            if (!clientDataDir) return "00";
            const auto clientData = clientDataDir->getClientData(nickname);
            if (!clientData)
                return "00";
            const bool receivingTransmission = clientData->receivingTransmission > 0;

            const bool clientTalkingOnRadio = clientData->currentTransmittingTangentOverType != sendingRadioType::LISTEN_TO_NONE;
            if (clientData->clientTalkingNow || clientTalkingOnRadio)
                return receivingTransmission ? "11" : "10";
            return receivingTransmission ? "01" : "00";
        }
        case gameCommand::IS_SPEAKING_BULK:
        {
            tokens.clear();
            helpers::split(command, '\t', tokens); // Previously we only split two tokens for performance reasons

            std::string result;
            result.reserve((tokens.size() - 1) * 3 + 1);

            const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(Teamspeak::getCurrentServerConnection());
            if (!clientDataDir) // No data available, lets say no-one is speaking
            {
                for (size_t i = 1; i < tokens.size(); ++i)
                {
                    result += "00\t";
                }
                return result;
            }


            for (size_t i = 1; i < tokens.size(); ++i)
            {
                const auto nickname = convertNickname(tokens[i]);

                const auto clientData = clientDataDir->getClientData(nickname);
                if (!clientData) // Client not known, lets say they are not speaking
                {
                    result += "00\t";
                    continue;
                }

                const bool receivingTransmission = clientData->receivingTransmission > 0;
                const bool clientTalkingOnRadio = clientData->currentTransmittingTangentOverType != sendingRadioType::LISTEN_TO_NONE;
                // talking, receiving transmission

                if (clientData->clientTalkingNow || clientTalkingOnRadio)
                    result += receivingTransmission ? "11\t" : "10\t";
                else
                    result += receivingTransmission ? "01\t" : "00\t";
            }

            return result;
        }
        case gameCommand::RECV_FREQS: {
            const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(Teamspeak::getCurrentServerConnection());
            if (!clientDataDir) return "[]";
            const auto clientData = clientDataDir->myClientData;
            if (!clientData)
                return "[]";
            std::stringstream str;
            str << "[";
            for (auto& it : clientData->receivingFrequencies) {
                str << '"' << it << "\",";
            }
            str.seekg(-1, std::stringstream::cur);
            str << "]";
        }
        default: break;
    }

    // MUMBLE-TFAR: fire-and-forget commands normally arrive with a trailing '~' and are queued
    // before ever reaching this function. Depending on the game extension build they can also
    // arrive without the marker as a synchronous request. Dropping them here would silently break
    // tangents/settings/etc., and any reply other than "OK" can end up rendered on screen by the
    // SQF error path — so queue them for async processing and acknowledge.
    switch (gameCommand) {
        case gameCommand::DFRAME:
            queueCommand(command);
            return TFAR::config.needsRefresh() ? "NEEDCFG" : "OK";
        case gameCommand::FREQ:
        case gameCommand::KILLED:
        case gameCommand::TRACK:
        case gameCommand::SPEAKERS:
        case gameCommand::TANGENT:
        case gameCommand::RELEASE_ALL_TANGENTS:
        case gameCommand::SETCFG:
        case gameCommand::MISSIONEND:
        case gameCommand::AddRadioTower:
        case gameCommand::DeleteRadioTower:
        case gameCommand::collectDebugInfo:
            queueCommand(command);
            return "OK";
        default: break;
    }

    return "ASYNC COMMAND SENT IN SYNC CONTEXT";
}


gameCommand CommandProcessor::toGameCommand(std::string_view textCommand, size_t tokenCount) {
    if (textCommand.length() < 3) return gameCommand::unknown;
    const auto hash = const_strhash(textCommand.data(), textCommand.length());
    switch (hash) {
        case FORCE_COMPILETIME(const_strhash("POS"sv)):
            return gameCommand::POS;
        case FORCE_COMPILETIME(const_strhash("IS_SPEAKING"sv)):
            return gameCommand::IS_SPEAKING;
        case FORCE_COMPILETIME(const_strhash("IS_SPEAKING_BULK"sv)):
            return gameCommand::IS_SPEAKING_BULK;
        case FORCE_COMPILETIME(const_strhash("TS_INFO"sv)):
            return gameCommand::TS_INFO;
        case FORCE_COMPILETIME(const_strhash("FREQ"sv)):
            return gameCommand::FREQ;
        case FORCE_COMPILETIME(const_strhash("KILLED"sv)):
            return gameCommand::KILLED;
        case FORCE_COMPILETIME(const_strhash("DFRAME"sv)):
            return gameCommand::DFRAME;
        case FORCE_COMPILETIME(const_strhash("TRACK"sv)):
            return gameCommand::TRACK;
        case FORCE_COMPILETIME(const_strhash("SPEAKERS"sv)):
            return gameCommand::SPEAKERS;
        case FORCE_COMPILETIME(const_strhash("RELEASE_ALL_TANGENTS"sv)):
            return gameCommand::RELEASE_ALL_TANGENTS;
        case FORCE_COMPILETIME(const_strhash("MISSIONEND"sv)):
            return gameCommand::MISSIONEND;
        case FORCE_COMPILETIME(const_strhash("SETCFG"sv)):
            return gameCommand::SETCFG;
        case FORCE_COMPILETIME(const_strhash("TANGENT"sv)):
        case FORCE_COMPILETIME(const_strhash("TANGENT_LR"sv)):
        case FORCE_COMPILETIME(const_strhash("TANGENT_DD"sv)):
            return gameCommand::TANGENT;
        case FORCE_COMPILETIME(const_strhash("RadioTwrAdd"sv)):
            return gameCommand::AddRadioTower;
        case FORCE_COMPILETIME(const_strhash("RadioTwrDel"sv)):
            return gameCommand::DeleteRadioTower;
        case FORCE_COMPILETIME(const_strhash("collectDebugInfo"sv)):
            return gameCommand::collectDebugInfo;
    }
    return gameCommand::unknown;
}

void CommandProcessor::threadRun() {
    threadRunning = true;
    while (shouldRun) {
        std::unique_lock<std::mutex> lock(theadMutex);
        threadWorkCondition.wait(lock, [this] {return !commandQueue.empty() || !shouldRun; });
        if (!shouldRun) return;
        ProfileFunction;
        const auto command(std::move(commandQueue.front()));
        commandQueue.pop();
        lock.unlock();
        processAsynchronousCommand(command);
    }
    threadRunning = false;
}


DEFINE_API_PROFILER(processAsynchronousCommand)
void CommandProcessor::processAsynchronousCommand(const std::string& command) const {
    if (command.substr(0,3) != "POS") //else double log as POS is also logged in sync processing
        Logger::log(LoggerTypes::gameCommands, command);
    API_PROFILER(processAsynchronousCommand);
    ProfileFunction;
    ProfilerLog(command);
    std::vector<std::string_view> tokens; tokens.reserve(18);
    helpers::split(command, '\t', tokens); //may not be used in nickname
    const auto gameCommand = toGameCommand(tokens[0], tokens.size());
    if (gameCommand == gameCommand::unknown) return;
    TSServerID currentServerConnectionHandlerID = Teamspeak::getCurrentServerConnection();
    auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(currentServerConnectionHandlerID);
    if (!clientDataDir) return;


    switch (gameCommand) {

        case gameCommand::FREQ: {//async
                                 //FREQ, str(_freq), str(_freq_lr)
                                //_alive, speakVolume, _nickname, 
                                //waves, TF_terrain_interception_coefficient, _globalVolume,
                                //_receivingDistanceMultiplicator, TF_speakerDistance

            TFAR::getInstance().m_gameData.setFreqInfos(tokens);

            if (!clientDataDir->myClientData) return;//shouldn't be possible.. but safety first

            //Rename to ingame name. This is okey here as long as we always get FREQ commands. Even if now having any Radios
            //Could be replaced by telling everyone in channel our ingame name.. So they can assign it to clientID. If we really want to remove renaming in TS
            const std::string& nickname = convertNickname(tokens[5]);
            std::string myNickname = Teamspeak::getMyNickname(currentServerConnectionHandlerID);
            if (!myNickname.empty() && myNickname != nickname && (nickname != "Error: No unit" && nickname != "Error: No vehicle" && nickname != "any")) {
                if (Teamspeak::setMyNicknameToGameName(currentServerConnectionHandlerID, nickname)) {
                    TFAR::getInstance().onTeamspeakClientUpdated(currentServerConnectionHandlerID, clientDataDir->myClientData->clientId, nickname);
                }
            }
        } return;
        case gameCommand::POS: {
            //POS nickname [x,y,z] [viewdirUnitvector(x,y,z)] canSpeak canUseSWRadio canUseLRRadio canUseDDRadio vehicleID terrainInterception voiceVolume objectInterception
            unitPositionPacket packet{
                convertNickname(tokens[1]),                 //nickname
                Position3D(tokens[2]),                      //position
                Direction3D(tokens[3]),                     //direction
                helpers::isTrue(tokens[4]),                 //canSpeak
                helpers::isTrue(tokens[5]),                 //canUseSWRadio
                helpers::isTrue(tokens[6]),                 //canUseLRRadio
                helpers::isTrue(tokens[7]),                 //canUseDDRadio
                tokens[8],                                  //vehicleID
                helpers::parseArmaNumberToInt(tokens[9]),   //terrainInterception
                helpers::parseArmaNumber(tokens[10]),       //voiceVolume
                helpers::parseArmaNumberToInt(tokens[11]),  //objectInterception
                helpers::isTrue(tokens[12]),                //isSpectating
                helpers::isTrue(tokens[13])                 //isEnemyToPlayer
            };

            processUnitPosition(currentServerConnectionHandlerID, packet);
        } return;
        case gameCommand::KILLED:
            processUnitKilled(convertNickname(tokens[1]), currentServerConnectionHandlerID);
            return;
        case gameCommand::TRACK:
            TFAR::trackPiwik(tokens);
            return;
        case gameCommand::DFRAME:
            TFAR::getInstance().m_gameData.currentDataFrame++;
            return;
        case gameCommand::SPEAKERS:
            processSpeakers(tokens);
            return;
        case gameCommand::TANGENT: {//async
                                    //TANGENT, PRESSED/RELEASED, _freq+Radiocode, Range inclusive transmittingdist Multiplicator, Subtype, classname
            auto myClientData = clientDataDir->myClientData;
            if (!myClientData) return; //safety first

            const bool pressed = tokens[1] == "PRESSED"sv;

            const auto subtypeString = tokens[4];
            const auto subtype = PTTDelayArguments::stringToSubtype(tokens[4]);

            const bool changed = TFAR::getInstance().m_gameData.tangentPressed != pressed;

            if (!changed) //If nothing changed there is nothing to do.
                return;

            TFAR::getInstance().m_gameData.tangentPressed = pressed;

            myClientData->setCurrentTransmittingSubtype(subtypeString);

            const float globalVolume = TFAR::getInstance().m_gameData.globalVolume;//used for playing radio sounds

            if (pressed) {
                myClientData->range = pressed ? helpers::parseArmaNumberToInt(tokens[3]) : 0; //range needs to be set to indicate that we are transmitting
                myClientData->currentTransmittingTangentOverType = (tokens[0] == "TANGENT_LR") ? sendingRadioType::LISTEN_TO_LR : sendingRadioType::LISTEN_TO_SW;

                myClientData->setCurrentTransmittingFrequency(tokens[2]);
            } else {
                myClientData->range = 0; //range needs to be set to indicate that we are transmitting
                myClientData->currentTransmittingTangentOverType = sendingRadioType::LISTEN_TO_NONE;
                myClientData->setCurrentTransmittingFrequency(std::string("prev_").append(tokens[2]));
            }

           
            const auto commandToBroadcast = command + "\t" + (myClientData->canUseSWRadio ? "1\t" : "0\t") + (myClientData->canUseDDRadio ? "1\t" : "0\t") + myClientData->getNickname();
            auto frequency = tokens[2];
            //convenience function to remove duplicate code
            const auto playRadioSound = [currentServerConnectionHandlerID, globalVolume, &frequency](SoundFile& file,
                const std::map<std::string, FREQ_SETTINGS, std::less<>>& frequencyMap) {
                auto found = frequencyMap.find(frequency);
                if (found != frequencyMap.end()) {
                    const auto& freq = found->second;
                    TFAR::getPlaybackHandler()->playWavFile(currentServerConnectionHandlerID, file, helpers::volumeMultiplifier(static_cast<float>(freq.volume)) * globalVolume, freq.stereoMode);
                } else {
                    TFAR::getPlaybackHandler()->playWavFile(file);
                }
            };

            auto soundToPlay = SoundDirectory::getRadioSound(subtype, pressed);
            if (pressed) {

                LockGuard_shared frequencyLock(TFAR::getInstance().m_gameData.m_lock);
                if (!soundToPlay.isEmpty())
                    playRadioSound(soundToPlay, subtype == PTTDelayArguments::subtypes::digital ? TFAR::getInstance().m_gameData.mySwFrequencies : TFAR::getInstance().m_gameData.myLrFrequencies);
                frequencyLock.unlock();

                if (!TFAR::config.get<bool>(Setting::full_duplex) && tokens.size() == 6) {
                    TFAR::getInstance().m_gameData.setCurrentTransmittingRadio(tokens[5]);
                }
                //Force enable PTT
                LockGuard_exclusive lock(tangentCriticalSection);
                if (!waitingForTangentOff) {
                    vadEnabled = Teamspeak::hlp_checkVad();
                    if (vadEnabled) Teamspeak::hlp_disableVad();
                    // broadcast info about tangent pressed over all client
                    disableVoiceAndSendCommand(commandToBroadcast, currentServerConnectionHandlerID, pressed);
                } else skipTangentOff = true;
            } else {
                PTTDelayArguments args;
                args.commandToBroadcast = commandToBroadcast;
                args.currentServerConnectionHandlerID = currentServerConnectionHandlerID;
                args.pttDelay = TFAR::getInstance().m_gameData.pttDelay;
                args.tangentReleaseDelay = std::chrono::milliseconds(static_cast<int>(TFAR::config.get<float>(Setting::tangentReleaseDelay)));
                std::thread([args]() {process_tangent_off(args); }).detach();

                LockGuard_shared frequencyLock(TFAR::getInstance().m_gameData.m_lock);
                if (!soundToPlay.isEmpty())
                    playRadioSound(soundToPlay, subtype == PTTDelayArguments::subtypes::digital ? TFAR::getInstance().m_gameData.mySwFrequencies : TFAR::getInstance().m_gameData.myLrFrequencies);
                frequencyLock.unlock(); //setCurTransRadio acquires lock. This can deadlock if we don't unlock here

                if (!TFAR::config.get<bool>(Setting::full_duplex)) {
                    TFAR::getInstance().m_gameData.setCurrentTransmittingRadio("");
                }
            }
        } return;
        case gameCommand::RELEASE_ALL_TANGENTS: {
            const auto commandToSend = "RELEASE_ALL_TANGENTS\t" + convertNickname(tokens[1]);

            //Need to release tangent in case it's currently pressed else player will hotmic
            PTTDelayArguments args;
            args.commandToBroadcast = commandToSend;
            args.currentServerConnectionHandlerID = currentServerConnectionHandlerID;
            args.pttDelay = 0ms;
            args.tangentReleaseDelay = 0ms;
            std::thread([args]() {process_tangent_off(args); }).detach();
        } return;
        case gameCommand::SETCFG: {//async
            const auto key = tokens[1];
            const auto value = tokens[2];
            const Setting keyEnum(key);
            if (!settings::isValidKey(keyEnum)) {
                std::string msg;
                msg.reserve("Used invalid config key: "sv.length() + key.length() + " (You probably need to update your TFAR Teamspeak plugin)"sv.length());
                msg += "Used invalid config key: "sv;
                msg += key;
                msg += " (You probably need to update your TFAR Teamspeak plugin)"sv;

                MessageBoxA(0, msg.c_str(), "Task Force Radio", MB_OK);
                return;
            }
            if (tokens.size() == 4) {
                const auto type = tokens[3];
                if (type == "BOOL"sv) {
                    TFAR::config.set(key, value == "true"sv || value == "TRUE"sv);
                } else if (type == "SCALAR"sv) {
                    TFAR::config.set(key, helpers::parseArmaNumber(value));
                } else {//unsupported type or STRING
                    TFAR::config.set(key, value);
                }
            } else {
                TFAR::config.set(key, std::string(""));
            }
        } return;
        case gameCommand::MISSIONEND: //Handled by pipe extension. That sets last GameTick to 0 so SharedMemoryHandler::onDisconnected will fire
            //TFAR::getInstance().onGameDisconnected();
            return;
        case gameCommand::AddRadioTower: {
            auto data = helpers::split(tokens[1], 0xA);
            for (auto& element : data) {
                auto antennaData = helpers::split(element, ';');
                if (antennaData.size() == 3)
                  TFAR::getAntennaManager()->addAntenna(Antenna(NetID(antennaData[0]), Position3D(antennaData[2]), helpers::parseArmaNumber(antennaData[1])));
                else {
                    //#TODO log?
                }
            }
            return;

        }

        case gameCommand::DeleteRadioTower: {
            auto data = helpers::split(tokens[1], 0xA);
            for (auto& element : data) {
                TFAR::getAntennaManager()->removeAntenna(element);
            }
            return;

        }
        case gameCommand::collectDebugInfo: {
            std::stringstream date;
            const auto now = std::chrono::system_clock::now();
            const auto in_time_t = std::chrono::system_clock::to_time_t(now);
            date << std::put_time(std::localtime(&in_time_t), "%d-%m_%H-%M-%S");
            const auto dateString = date.str();

            auto basePath = std::string(getenv("appdata")) + R"(\TS3Client\logs\)" + dateString + "\\";
            std::error_code err;

            std::filesystem::create_directories(basePath,err);

            std::filesystem::copy(std::string(getenv("appdata")) + R"(\TS3Client\TFAR_pluginCommands.log)", basePath + "TFAR_pluginCommands.log",err);
            std::filesystem::copy(std::string(getenv("appdata")) + R"(\TS3Client\TFAR_gameCommands.log)", basePath + "TFAR_gameCommands.log", err);

            clientDataDir->forEachClient([&basePath](const std::shared_ptr<clientData>& cli) {
                auto messages = std::move(cli->messages);
                cli->offset = 0;
                auto nick = cli->getNickname();
                constexpr auto illegalChars = R"(\/:?"<>|)"sv;
                for (auto it = nick.begin(); it < nick.end(); ++it) {
                    if (illegalChars.find(*it) != std::string::npos) {
                        *it = ' ';
                    }
                }

                std::ofstream fs(basePath + "CL_" + nick + ".log");
                for (auto& msg : messages) {
                    fs << msg << '\n';
                }

                std::ofstream vdl(basePath + "VDL_" + nick + ".log");
                cli->verboseDataLog(vdl);
            });


            std::stringstream diag;
            diag << "diag from " << dateString << "\n";
            TFAR::getInstance().doDiagReport(diag);

            std::ofstream fsd(basePath + "diag.log");
            fsd << diag.str();

            std::stringstream pos;
            TFAR::getInstance().doTypeDiagReport("pos", diag);
            std::ofstream fsp(basePath + "pos.log");
            fsp << diag.str();
            Teamspeak::printMessageToCurrentTab((std::string("TFAR: logged to ") + basePath).c_str());

        }
    }
}

void CommandProcessor::processSpeakers(std::vector<std::string_view>& tokens) const {
    //#TODO we don't really need to send Speakers every time.. Their pos is always static. Either on a person or on ground
    LockGuard_exclusive lock(TFAR::getInstance().m_gameData.m_lock);
    TFAR::getInstance().m_gameData.speakers.clear();
    if (tokens.size() != 2)
        return;
    ProfileFunction;

    //if you switch TS tab... You don't get speakers bro!
    const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(Teamspeak::getCurrentServerConnection());
    if (!clientDataDir) return;

    for (auto& speaker : helpers::split(tokens[1], 0xB)) {
        if (speaker.empty()) continue;

        std::vector<std::string_view> parts; parts.reserve(7);
        helpers::split(speaker, 0xA, parts);
        if (parts.size() < 6) return;
        //parts radio_id,nickname,pos,volume,vehicle,(waveZ)
        const auto clientData = clientDataDir->getClientData(convertNickname(parts[2]));

        SPEAKER_DATA data;
        data.radio_id = parts[0];
        data.client = clientData;
        data.pos = Position3D(parts[3]);
        data.volume = helpers::parseArmaNumberToInt(parts[4]);
        data.vehicle = helpers::getVehicleDescriptor(parts[5]);
        if (parts.size() > 6)
            data.waveZ = helpers::parseArmaNumber(parts[6]);
        else
            data.waveZ = data.pos.isNull() ? 1 : data.pos[2];

        for (auto& freq : helpers::split(parts[1], '|')) {
            TFAR::getInstance().m_gameData.speakers.insert(std::pair<std::string, SPEAKER_DATA>(freq, data));
        }
    }
}

void CommandProcessor::processUnitKilled(std::string &&name, TSServerID serverConnection) const {
    ProfileFunction;
    const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnection);
    if (clientDataDir) {
        if (const auto clientData = clientDataDir->getClientData(name); clientData) {
            clientData->dataFrame = INVALID_DATA_FRAME;
        }
    }

    if (!clientDataDir)
        return;

    const bool isSeriousMode = isSeriousModeEnabled(serverConnection, Teamspeak::getMyId(serverConnection));

    if (clientDataDir->myClientData) {
        if (clientDataDir->myClientData->dataFrame == INVALID_DATA_FRAME)
            TFAR::getInstance().m_gameData.alive = false;
    }

    const auto clientsIds = Teamspeak::getChannelClients(serverConnection, Teamspeak::getChannelOfClient(serverConnection));

    if (!isSeriousMode && !TFAR::getInstance().m_gameData.alive) {
        //If not seriousMode we can hear everyone when Dead
        Teamspeak::setClientMute(serverConnection, clientsIds, false);
        return;
    }

    const auto myId = Teamspeak::getMyId(serverConnection);
    std::vector<TSClientID> aliveClients; aliveClients.reserve(clientsIds.size());
    std::vector<TSClientID> deadClients;  deadClients.reserve(clientsIds.size());
    for (auto& id : clientsIds) {
        if (id == myId) continue;
        auto clientData = clientDataDir->getClientData(id);
        if (clientData && clientData->isAlive()) {
            aliveClients.push_back(id);
        } else if (clientData && clientData->isConfirmedDead()) {
            deadClients.push_back(id);
        }
        //Clients we know nothing about (no clientData) or whose position data
        //is merely stale are classified as neither: muting them as "dead"
        //silenced living players whose data stream lagged on large events,
        //and the mute stuck until they rejoined the channel.
    }
    /*
    serious mode:
    dead can hear dead.
    alive can hear alive
    alive cant hear dead
    dead cant hear alive

    non-serious mode:
    dead can hear dead.
    alive can hear alive
    alive can hear dead
    dead can hear alive
    */

    if (!TFAR::config.get<bool>(Setting::disableAutomaticMute)) {
        Teamspeak::setClientMute(serverConnection, deadClients, TFAR::getInstance().m_gameData.alive && isSeriousMode); //Mute dead people if seriousMode
        Teamspeak::setClientMute(serverConnection, aliveClients, !TFAR::getInstance().m_gameData.alive && isSeriousMode); //Mute alive people if seriousMode
    }
}


DEFINE_API_PROFILER(processUnitPosition)

void CommandProcessor::processUnitPosition(TSServerID serverConnection, unitPositionPacket& packet) {
    API_PROFILER(processUnitPosition);
    ProfileFunction;
    const auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnection);
    if (!clientDataDir) return;

    auto clientData = clientDataDir->getClientData(packet.nickname);
    if (!clientData) return;

    packet.myData = clientData == clientDataDir->myClientData;

    clientData->updatePosition(packet);
    //bool clientTalkingOnRadio = (clientData->currentTransmittingTangentOverType != sendingRadioType::LISTEN_TO_NONE) || clientData->clientTalkingNow;

    if (packet.myData) {
        Teamspeak::setMyClient3DPosition(serverConnection, Position3D());
    } else {
        setGameClientMuteStatus(serverConnection, clientData->clientId);
        Teamspeak::setClient3DPosition(serverConnection, clientData->clientId, Position3D());
    }
}

std::string CommandProcessor::ts_info(std::string_view command) {
    if (command == "SERVER")
        return Teamspeak::getServerName();
    if (command == "SERVERUID") 
        return Teamspeak::getServerUID();
    if (command == "CHANNEL")
        return Teamspeak::getChannelName();
    if (command == "CHANNELID")
        return std::to_string(Teamspeak::getChannelOfClient().baseType());
    if (command == "PING")
        return "PONG";
    if (command == "VERSION")
        return PLUGIN_VERSION;
    return "FAIL";
}

void CommandProcessor::process_tangent_off(PTTDelayArguments arguments) {
    ProfileFunction;
    waitingForTangentOff = true;
    if (arguments.pttDelay > 0ms)
        std::this_thread::sleep_for(arguments.pttDelay);
    if (arguments.tangentReleaseDelay > 0ms)
        std::this_thread::sleep_for(arguments.tangentReleaseDelay);

    LockGuard_exclusive lock(tangentCriticalSection);
    if (!skipTangentOff) {
        if (vadEnabled) Teamspeak::hlp_enableVad();
        disableVoiceAndSendCommand(arguments.commandToBroadcast, arguments.currentServerConnectionHandlerID, false);
        waitingForTangentOff = false;
    } else {
        skipTangentOff = false;
    }
}

void CommandProcessor::disableVoiceAndSendCommand(std::string_view commandToBroadcast, TSServerID currentServerConnectionHandlerID, bool pressed) {
    Teamspeak::setVoiceDisabled(currentServerConnectionHandlerID, !(pressed || vadEnabled));
    Teamspeak::sendPluginCommand(currentServerConnectionHandlerID, TFAR::getInstance().getPluginID(), commandToBroadcast, PluginCommandTarget_CURRENT_CHANNEL);
}

std::string CommandProcessor::convertNickname(std::string_view nickname) {
    ProfileFunction;
    if (!nickname.empty() && (nickname.front() == ' ' || nickname.back() == ' ')) {
        std::string newName(nickname);
        if (nickname.front() == ' ') {
            newName.replace(0, nickname.find_first_not_of(' '), nickname.find_first_not_of(' '), '_');
        }
        if (nickname.back() == ' ') {
            newName.replace(nickname.find_last_not_of(' ') + 1, newName.length() - nickname.find_last_not_of(' ') - 1, newName.length() - nickname.find_last_not_of(' ') - 1, '_');
        }
        return newName;
    }
    return std::string(nickname);
}
