#include "task_force_radio.hpp"
#include <Windows.h>
#include <WinInet.h>
#include "common.hpp"
#include <thread>
#include <locale>
#include "CommandProcessor.hpp"
#include "PlaybackHandler.hpp"
#include "serverData.hpp"
#include "Teamspeak.hpp"
#include "Logger.hpp"
#include "version.h"

TFAR::TFAR() {
    onGameDisconnected.connect([this]() {
        Logger::log(LoggerTypes::profiler, "On Game Disconnected", LogLevel_DEVEL);
        Teamspeak::unmuteAll(Teamspeak::getCurrentServerConnection());
        if (getCurrentlyInGame()) {
            TFAR::getPlaybackHandler()->playWavFile(SoundFile("radio-sounds/off"));
            onGameEnd();
            TFAR::config.setRefresh();
        }
        setCurrentlyInGame(false);
    });

    onGameConnected.connect([this]() {
        Logger::log(LoggerTypes::profiler, "On Game Connected", LogLevel_DEVEL);
        if (!getCurrentlyInGame()) {
            getPlaybackHandler()->playWavFile(SoundFile("radio-sounds/on"));
            setCurrentlyInGame(true);
            onGameStart();
            m_gameData.currentDataFrame = 0;
        }

    });

    onGameStart.connect([]() {
        Logger::log(LoggerTypes::profiler, "On Respawn", LogLevel_DEVEL);
        if (!Teamspeak::isConnected())
            return;

        Teamspeak::moveToSeriousChannel();
    });

    onGameEnd.connect([]() {
        Logger::log(LoggerTypes::profiler, "On Game End", LogLevel_DEVEL);
        TSServerID currentServer = Teamspeak::getCurrentServerConnection();
        Teamspeak::moveFromSeriousChannel(currentServer);
        Teamspeak::resetMyNickname(currentServer);
        Teamspeak::unmuteAll(currentServer); //this may be called twice. If End was caused by PluginDisconnect. But will return immediatly if there are no muted clients
        Teamspeak::setVoiceDisabled(currentServer, !CommandProcessor::vadEnabled); //stop force PTT if we were in a transmission
        if (CommandProcessor::vadEnabled) Teamspeak::hlp_enableVad(); //If voice activation was enabled previously, we want to return it to it's original state
    });

    onShutdown.connect([this]() {

        if (getCurrentlyInGame())
            onGameEnd();
        Teamspeak::setMyMetaData("");

        if (m_commandProcessor)
            m_commandProcessor->stopThread();

        onGameStart.removeAllSlots(); //clear all Signals slots so they cannot possibly be called after shutdown
        onGameEnd.removeAllSlots();
        onGameConnected.removeAllSlots();
        onGameDisconnected.removeAllSlots();
        onTeamspeakServerConnect.removeAllSlots();
        onTeamspeakClientJoined.removeAllSlots();
        onTeamspeakClientLeft.removeAllSlots();
        onTeamspeakClientUpdated.removeAllSlots();
        debugUI.stop();
    });
    config.configValueSet.connect([this](const Setting& setting) {
        static std::chrono::system_clock::time_point lastMove;
        if (setting == Setting::serious_channelName
            && std::chrono::system_clock::now() - lastMove > 10ms) { //Teamspeak needs a little time to update currentChannel. This prevents doublejoin
            Teamspeak::moveToSeriousChannel();
            lastMove = std::chrono::system_clock::now();
        }
        checkIfSeriousModeEnabled(Teamspeak::getCurrentServerConnection());

        if (setting == Setting::minimumPluginVersion) {
            static bool warningShown = false;
            if (config.get<float>(Setting::minimumPluginVersion) >
#ifdef AppVeyorBuild
                AppVeyorBuildNum
#else
                5000
#endif
                && !warningShown) {
                    warningShown = true;
                    MessageBoxA(0,
                        "Arma requires a newer TFAR Plugin version than the one you have installed. Please update your Plugin.",
                        "Task Force Arrowhead Radio",
                        MB_OK | MB_ICONWARNING);
            }
        }

    });

    onTeamspeakChannelSwitched.connect([this](TSServerID serverID, TSChannelID channelID) {
        checkIfSeriousModeEnabled(serverID);
    });


    doDiagReport.connect([this](std::stringstream& diag) {
        config.diagReport(diag);
        diag << "TFAR:\n";
        diag << TS_INDENT << "currentlyInGame: " << getCurrentlyInGame() << "\n";
        diag << TS_INDENT << "pluginPath: " << getPluginPath() << "\n";
        diag << TS_INDENT << "pluginID: " << getPluginID() << "\n";

        diag << TS_INDENT << "gameData:\n";
        diag << TS_INDENT << TS_INDENT << "myVoiceVol: " << m_gameData.myVoiceVolume << "\n";
        diag << TS_INDENT << TS_INDENT << "alive: " << m_gameData.alive << "\n";
        diag << TS_INDENT << TS_INDENT << "wavesLvl: " << m_gameData.wavesLevel << "\n";
        diag << TS_INDENT << TS_INDENT << "terrIntCoeff: " << m_gameData.terrainIntersectionCoefficient << "\n";
        diag << TS_INDENT << TS_INDENT << "globalVol: " << m_gameData.globalVolume << "\n";
        diag << TS_INDENT << TS_INDENT << "recvDistMult: " << m_gameData.receivingDistanceMultiplicator << "\n";
        diag << TS_INDENT << TS_INDENT << "speakerDist: " << m_gameData.speakerDistance << "\n";
        diag << TS_INDENT << TS_INDENT << "curDFrame: " << m_gameData.currentDataFrame << "\n";
        diag << TS_INDENT << TS_INDENT << "curTransRadio: " << m_gameData.getCurrentTransmittingRadio() << "\n";
        diag << TS_INDENT << TS_INDENT << "tangentPressed: " << m_gameData.tangentPressed << "\n";
        diag << TS_INDENT << TS_INDENT << "pttDelay: " << m_gameData.pttDelay.count() << "ms\n";

        std::array<std::string_view, 3> stereoModeToString{ "stereo"sv, "left"sv, "right"sv };
        diag << TS_INDENT << TS_INDENT << "mySRFreq:\n";
        for (auto& it : m_gameData.mySwFrequencies) {
            diag << TS_INDENT << TS_INDENT << TS_INDENT << it.first << ":\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "radio: " << it.second.radioClassname << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "stereo: " << stereoModeToString[static_cast<uint8_t>(it.second.stereoMode)] << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "vol: " << it.second.volume << "\n";
        }
        diag << TS_INDENT << TS_INDENT << "myLRFreq:\n";
        for (auto& it : m_gameData.myLrFrequencies) {
            diag << TS_INDENT << TS_INDENT << TS_INDENT << it.first << ":\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "radio: " << it.second.radioClassname << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "stereo: " << stereoModeToString[static_cast<uint8_t>(it.second.stereoMode)] << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "vol: " << it.second.volume << "\n";
        }
        diag << TS_INDENT << TS_INDENT << "Speaker:\n";
        for (auto& it : m_gameData.speakers) {
            diag << TS_INDENT << TS_INDENT << TS_INDENT << it.first << ":\n";
            auto lockedClient = it.second.client.lock();
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "client: " << lockedClient << " (" << (lockedClient ? lockedClient->getNickname() : "null") << ")\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "pos: " << it.second.getPos().toString() << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "id: " << it.second.radio_id << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "vec: " << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "name: " << it.second.vehicle.vehicleName << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "IS: " << it.second.vehicle.intercomSlot << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "ISO: " << it.second.vehicle.vehicleIsolation << "\n";
            diag << TS_INDENT << TS_INDENT << TS_INDENT << TS_INDENT << "vol: " << it.second.volume << "\n";
        }
    });

}

TFAR& TFAR::getInstance() {
    static TFAR tfarSingleton;
    return tfarSingleton;
}

settings TFAR::config;//declaring the static config
DebugUI TFAR::debugUI;//declaring the static config

void TFAR::checkIfSeriousModeEnabled(TSServerID serverID) {
    std::string serious_mod_channel_name = TFAR::config.get<std::string>(Setting::serious_channelName);
    bool isSerious = false;
    if (!serious_mod_channel_name.empty() && Teamspeak::isInChannel(serverID, Teamspeak::getMyId(serverID), serious_mod_channel_name)) isSerious = true;
    if (isSeriousMode != isSerious) {
        isSeriousMode = isSerious;
        onSeriousModeChanged(isSerious);
    }
}

bool TFAR::isUpdateAvailable() {
    DWORD dwBytes;
    char ch;
    std::string pluginVersion;

    DWORD r = 0;
    if (!InternetGetConnectedState(&r, 0)) return false;
    if (r & INTERNET_CONNECTION_OFFLINE) return false;

    const HINTERNET Initialize = InternetOpen(L"TFAR", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
    const HINTERNET Connection = InternetConnect(Initialize, UPDATE_URL, INTERNET_DEFAULT_HTTP_PORT, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
    const HINTERNET File = HttpOpenRequest(Connection, nullptr, UPDATE_FILE, nullptr, nullptr, nullptr, 0, 0);

    if (HttpSendRequest(File, nullptr, 0, nullptr, 0)) {
        while (InternetReadFile(File, &ch, 1, &dwBytes)) {
            if (dwBytes != 1)break;
            pluginVersion += ch;
        }
    }

    InternetCloseHandle(File);
    InternetCloseHandle(Connection);
    InternetCloseHandle(Initialize);

    const std::string currentVersion = PLUGIN_VERSION;
    if (pluginVersion.length() > 3) {
        return Version(pluginVersion) > Version(currentVersion);
    } else {
        return false;
    }
}
#include <sstream>
void TFAR::trackPiwik(const std::vector<std::string_view>& piwikDataIn) {

    /*
    piwikData
     {
         0 "TRACK",
         1 string actiontype,
         2 string uid,
         3 array customVariables    [1,"name","value"]
     }

    */
    std::vector<std::string> piwikData;
    for (auto& it : piwikDataIn)
        piwikData.emplace_back(it);

    std::thread([piwikData = std::move(piwikData)]() {

        struct trackerCustomVariable {
            unsigned int customVarID;
            std::string name;
            std::string value;
        };
        auto parseCustomVariables = [](std::string_view customVarArray) -> std::vector<trackerCustomVariable> {
            std::vector<trackerCustomVariable> out;

            //https://stackoverflow.com/questions/1448467/initializing-a-c-stdistringstream-from-an-in-memory-buffer
            struct OneShotReadBuf : public std::streambuf
            {
                OneShotReadBuf(char* s, std::size_t n)
                {
                    setg(s, s, s + n);
                }
            };

            OneShotReadBuf buf(const_cast<char*>(customVarArray.data()), customVarArray.length());
            std::istream inStream(&buf);
            char curChar;
            //   [[1,"name","value"], [2, "name2", "value2"]]
            inStream >> curChar;//should be [
            //  [[1,"name","value"], [2, "name2", "value2"]]
            while (curChar != '[' && !inStream.eof()) inStream >> curChar;//but we still wait till [ in case input starts with whitespace
            //[1,"name","value"], [2, "name2", "value2"]]


            //http://stackoverflow.com/a/17976541
            auto trimWhitespaceColon = [](const std::string &s) -> std::string {
                auto wsfront = std::find_if_not(s.begin(), s.end(), [](char c) {return c == ' ' || c == '"'; });
                auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](char c) {return c == ' ' || c == '"'; }).base();
                return wsback <= wsfront ? std::string() : std::string(wsfront, wsback);
            };


            bool firstBracket = true; //Used to skip first [ of [[
            while (!inStream.eof()) {
                if (curChar == '[') {  //new custom Variable
                    if (firstBracket) {
                        firstBracket = false;
                        inStream >> curChar;
                        continue;
                    }
                    //[1,"name","value"], [2, "name2", "value2"]]
                    trackerCustomVariable newCustomVar;
                    std::string id;
                    inStream >> curChar; //curChar has ID now
                    while (curChar != ',') {
                        id.push_back(curChar);
                        inStream >> curChar;
                    }
                    try {
                        newCustomVar.customVarID = std::stoi(id);
                    }
                    catch (...) { return std::vector<trackerCustomVariable>(); }

                    inStream >> curChar;//curChar has first char of name
                    //"name","value"], [2, "name2", 22]]
                    while (curChar != ',') {
                        newCustomVar.name.push_back(curChar);
                        inStream >> curChar;
                    }
                    //newCustomVar.name can be '  "name"  ' or '  223  ' so need to trim " and whitespace
                    newCustomVar.name = trimWhitespaceColon(newCustomVar.name);

                    //"value"], [2, "name2", "value2"]]
                    inStream >> curChar;//curChar has "
                    while (curChar != ']') {
                        newCustomVar.value.push_back(curChar);
                        inStream >> curChar;
                    }
                    newCustomVar.value = trimWhitespaceColon(newCustomVar.value);

                    out.push_back(newCustomVar);
                    //], [2, "name2", "value2"]]
                }
                inStream >> curChar;
            }
            return out;
        };

        std::vector<trackerCustomVariable> customVariables = parseCustomVariables(piwikData[3]);
        if (customVariables.empty()) return;
        std::stringstream request;
        if (piwikData[1] == "beta")//#Release remove on release
            request << "piwik.php?idsite=2&rec=1&url=\"piwik.dedmen.de\"";
        else
            request << "piwik.php?idsite=2&rec=1&url=\"radio.task-force.ru\"";
        request << "&action_name=" << piwikData[1];
        request << "&rand=" << rand();
        request << "&uid=" << piwikData[2];
        request << "&cvar={";
        bool firstVar = true;
        for (trackerCustomVariable& customVar : customVariables) {
            if (!firstVar) request << ',';
            firstVar = false;
            request << '"' << customVar.customVarID << "\":";//"1":
            request << '[';
            request << '"' << customVar.name << "\",";//"name",
            request << '"' << customVar.value << '"';//"value"
            request << ']';
            //"1":["name","value"]
        }
        request << "}";//closing cvar again
        std::string requestString = request.str();

        DWORD dwBytes;
        DWORD r = 0;
        char ch;
        if (!InternetGetConnectedState(&r, 0)) return;
        if (r & INTERNET_CONNECTION_OFFLINE) return;

        HINTERNET Initialize = InternetOpen(L"TFAR", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
        HINTERNET Connection;
        if (piwikData[1] == "beta")
            Connection = InternetConnect(Initialize, L"piwik.dedmen.de", INTERNET_DEFAULT_HTTP_PORT, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
        else
            Connection = InternetConnect(Initialize, PIWIK_URL, INTERNET_DEFAULT_HTTP_PORT, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
        HINTERNET File = HttpOpenRequestA(Connection, nullptr, requestString.c_str(), nullptr, nullptr, nullptr, 0, 0);
        if (HttpSendRequest(File, nullptr, 0, nullptr, 0)) {
            while (InternetReadFile(File, &ch, 1, &dwBytes)) {
                if (dwBytes != 1) break;
            }
        }
        InternetCloseHandle(File);
        InternetCloseHandle(Connection);
        InternetCloseHandle(Initialize);
    }).detach();
}

void TFAR::createCheckForUpdateThread() {
    std::thread([]() {
        if (isUpdateAvailable()) {
            MessageBoxA(0, "New version of Task Force Arrowhead Radio is available. Check radio.task-force.ru/en", "Task Force Arrowhead Radio Update", MB_OK);
        }
    }).detach();
}

std::shared_ptr<CommandProcessor>& TFAR::getCommandProcessor() {
    if (!getInstance().m_commandProcessor)
        getInstance().m_commandProcessor = std::make_shared<CommandProcessor>();
    return getInstance().m_commandProcessor;
}

std::shared_ptr<PlaybackHandler>& TFAR::getPlaybackHandler() {
    if (!getInstance().m_playbackHandler)
        getInstance().m_playbackHandler = std::make_shared<PlaybackHandler>();
    return getInstance().m_playbackHandler;
}

std::shared_ptr<serverDataDirectory>& TFAR::getServerDataDirectory() {
    if (!getInstance().m_serverData)
        getInstance().m_serverData = std::make_shared<serverDataDirectory>();
    return getInstance().m_serverData;
}

std::shared_ptr<AntennaManager>& TFAR::getAntennaManager() {
    if (!getInstance().m_antennaManger)
        getInstance().m_antennaManger = std::make_shared<AntennaManager>();
    return getInstance().m_antennaManger;
}

