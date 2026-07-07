// MUMBLE-TFAR
// Reimplementation of the TFAR "Teamspeak" facade (originally Teamspeak.cpp,
// built on the TeamSpeak3 plugin SDK) on top of the Mumble client internals.
//
// Everything below keeps the exact semantics the rest of the TFAR code base
// expects, so all other TFAR sources compile and behave unchanged.
//
// Design notes:
//  * Mumble has exactly one server connection, exposed to TFAR as the constant
//    handler ID 1 (SERVER_CONN).
//  * TSClientID == Mumble session ID (anyID was widened to 32 bit).
//  * TSChannelID == Mumble channel ID.
//  * TeamSpeak's live nickname rename does not exist in Mumble. Instead we keep
//    a "virtual nickname" (the in-game name) per session, broadcast it via a
//    PluginDataTransmission message ("TFARST") and use it everywhere TFAR asks
//    for a nickname. The original TFAR source itself suggests this approach.
//  * Client metadata (TS CLIENT_META_DATA) is replaced by the same "TFARST"
//    state broadcast (sent by the already existing TFAR service thread every
//    4 seconds and on every change).
//  * TS plugin commands are sent as PluginDataTransmission with dataID "TFAR".
//    The server does not echo messages back to the sender, so commands that
//    TeamSpeak would deliver back to us are looped back locally.

// Mumble / Qt headers first — TFAR's common.hpp defines macros (snprintf, ...)
// that must not leak into them.
#include "../AudioOutput.h"
#include "../AudioOutputToken.h"
#include "../Channel.h"
#include "../ClientUser.h"
#include "../Global.h"
#include "../Log.h"
#include "../MainWindow.h"
#include "../ServerHandler.h"
#include "../API.h"

#include "Mumble.pb.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QReadLocker>

#include "Teamspeak.hpp"
#include "TS3Compat.h"
#include "Logger.hpp"
#include "task_force_radio.hpp"
#include "clientData.hpp"
#include "helpers.hpp"
#include "version.h"

#include <atomic>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <unordered_map>

using namespace dataType;

// plugin.cpp declares this extern; keep a zeroed instance around so the legacy
// declaration stays valid even though nothing calls through it anymore.
struct TS3Functions ts3Functions = {};

static const TSServerID SERVER_CONN = TSServerID(1);

// Mumble's root channel has ID 0, but TFAR's TSChannelID treats 0 as invalid.
// All channel IDs crossing the compatibility boundary are therefore offset by 1.
static inline TSChannelID toTSChannel(unsigned int mumbleChannelId) {
    return TSChannelID(static_cast< uint64_t >(mumbleChannelId) + 1);
}
static inline unsigned int toMumbleChannel(TSChannelID channelId) {
    return static_cast< unsigned int >(channelId.baseType() - 1);
}

extern void processPluginCommand(std::string_view command); // plugin.cpp
void ts3plugin_onPluginCommandEventOld(uint64 serverConnectionHandlerID, const char *pluginName,
                                       const char *pluginCommand); // plugin.cpp

namespace ts3compat {

// ---------------------------------------------------------------- state ----

static std::atomic< bool > g_connectedAndSynchronized{ false };

// True when the server announced the mumble-tfar server extensions ("TFARSRV"):
// channel broadcast for TFAR messages, server-side state caching and push.
static std::atomic< bool > g_serverTFARSupport{ false };

static std::mutex g_stateLock;
// session -> in-game ("virtual") nickname, empty string = no override
static std::unordered_map< unsigned int, std::string > g_virtualNicknames;
// session -> TFAR metadata (the part between <TFAR></TFAR> in TeamSpeak)
static std::unordered_map< unsigned int, std::string > g_metadataCache;
static std::string g_myMetadata;
// last "TFARST" payload sent to the server (deduplication in enhanced mode)
static std::string g_lastSentState;

// emulation of TS voice input handling (see setVoiceDisabled / hlp_*Vad)
static std::atomic< bool > g_tfarInputActive{ false };
static std::atomic< bool > g_tfarVadSuppressed{ false };

bool isConnectedAndSynchronized() {
    return g_connectedAndSynchronized.load(std::memory_order_relaxed);
}

bool serverHasTFARSupport() {
    return g_serverTFARSupport.load(std::memory_order_relaxed);
}

static void updateForcedTransmission() {
    // TS semantics: transmission = INPUT_ACTIVE && (vad disabled || vad triggers).
    // "vad disabled" holds when TFAR suppressed it for the tangent (VAD users)
    // or when the user does not use voice activation at all (PTT/Continuous —
    // TS "vad" is false for those setups). Otherwise Mumble's own VAD/PTT logic
    // stays in charge.
    const bool vadOff = g_tfarVadSuppressed.load() || Global::get().s.atTransmit != Settings::VAD;
    const bool force  = g_tfarInputActive.load() && vadOff;
    API::PluginData::get().overwriteMicrophoneActivation.store(force);
}

// ------------------------------------------------------------- utilities ---

static QString bbCodeToHtml(std::string_view msg) {
    QString text = QString::fromUtf8(msg.data(), static_cast< int >(msg.size())).toHtmlEscaped();
    text.replace(QLatin1String("[B]"), QLatin1String("<b>"), Qt::CaseInsensitive);
    text.replace(QLatin1String("[/B]"), QLatin1String("</b>"), Qt::CaseInsensitive);
    text.replace(QLatin1String("\n"), QLatin1String("<br/>"));
    return text;
}

void clientLogMessage(const char *message, LogLevel severity) {
    const QString msg = QString::fromUtf8(message).trimmed();
    switch (severity) {
        case LogLevel_CRITICAL:
        case LogLevel_ERROR:
            qCritical("TFAR: %s", qPrintable(msg));
            break;
        case LogLevel_WARNING:
            qWarning("TFAR: %s", qPrintable(msg));
            break;
        default:
            qDebug("TFAR: %s", qPrintable(msg));
            break;
    }
}

std::chrono::milliseconds getPTTHoldDelay() {
    return std::chrono::milliseconds(Global::get().s.pttHold);
}

std::string getPluginResourcePath() {
    QSettings settings(QLatin1String("mumble-tfar"), QLatin1String("TFAR"));
    QString path = settings.value(QLatin1String("resourcePath")).toString();
    if (path.isEmpty())
        path = QCoreApplication::applicationDirPath() + QLatin1String("/tfar");
    if (!path.endsWith(QLatin1Char('/')) && !path.endsWith(QLatin1Char('\\')))
        path += QLatin1Char('/');
    return QDir::toNativeSeparators(path).toStdString();
}

// --------------------------------------------------- plugin data channel ---

static void sendPluginData(const char *dataID, const std::string &payload,
                           std::vector< unsigned int > receiverSessions, bool channelBroadcast = false) {
    if (!isConnectedAndSynchronized())
        return;

    const QByteArray data(payload.data(), static_cast< int >(payload.size()));
    const QByteArray id(dataID);

    // ServerHandler messages have to be produced on the main thread; TFAR calls
    // us from its own worker threads.
    // qApp as receiver context: always alive on the main thread (the MainWindow
    // may not exist yet while the game pipe is already up).
    QMetaObject::invokeMethod(
        QCoreApplication::instance(),
        [data, id, receiverSessions = std::move(receiverSessions), channelBroadcast]() {
            ServerHandlerPtr sh = Global::get().sh;
            if (!sh || Global::get().uiSession == 0)
                return;

            MumbleProto::PluginDataTransmission mpdt;
            mpdt.set_sendersession(Global::get().uiSession);
            if (!channelBroadcast) {
                for (const unsigned int session : receiverSessions) {
                    if (session != Global::get().uiSession && ClientUser::get(session))
                        mpdt.add_receiversessions(session);
                }
                if (mpdt.receiversessions_size() == 0)
                    return;
            }
            // channelBroadcast: an empty receiver list tells the mumble-tfar
            // server to fan the message out to the sender's current channel.
            mpdt.set_data(data.constData(), static_cast< std::size_t >(data.size()));
            mpdt.set_dataid(id.constData());
            sh->sendMessage(mpdt);
        },
        Qt::QueuedConnection);
}

static std::vector< unsigned int > sessionsInChannel(TSChannelID channelID) {
    std::vector< unsigned int > result;
    if (!channelID)
        return result;
    const unsigned int mumbleChannel = toMumbleChannel(channelID);
    QReadLocker lock(&ClientUser::c_qrwlUsers);
    for (auto it = ClientUser::c_qmUsers.constBegin(); it != ClientUser::c_qmUsers.constEnd(); ++it) {
        const ClientUser *user = it.value();
        if (user && user->cChannel && user->cChannel->iId == mumbleChannel)
            result.push_back(it.key());
    }
    return result;
}

static std::vector< unsigned int > allSessions() {
    std::vector< unsigned int > result;
    QReadLocker lock(&ClientUser::c_qrwlUsers);
    for (auto it = ClientUser::c_qmUsers.constBegin(); it != ClientUser::c_qmUsers.constEnd(); ++it)
        result.push_back(it.key());
    return result;
}

// Sends a TFAR message to every client in the given channel: through the
// server extension when available, otherwise by enumerating the sessions.
static void sendPluginDataToChannel(const char *dataID, const std::string &payload, TSChannelID channel) {
    if (serverHasTFARSupport())
        sendPluginData(dataID, payload, {}, true);
    else
        sendPluginData(dataID, payload, sessionsInChannel(channel));
}

// "TFARST" state broadcast: virtual nickname '\x01' metadata
static std::string buildStatePayload() {
    std::lock_guard< std::mutex > lock(g_stateLock);
    std::string payload;
    const auto found = g_virtualNicknames.find(Global::get().uiSession);
    if (found != g_virtualNicknames.end())
        payload = found->second;
    payload.push_back('\x01');
    payload += g_myMetadata;
    return payload;
}

static void broadcastStateToChannelID(TSChannelID channel) {
    if (!channel)
        return;
    const std::string payload = buildStatePayload();
    if (serverHasTFARSupport()) {
        // The server caches our state and re-pushes it to channel joiners —
        // only actual changes have to be transmitted.
        std::lock_guard< std::mutex > lock(g_stateLock);
        if (payload == g_lastSentState)
            return;
        g_lastSentState = payload;
    }
    sendPluginDataToChannel("TFARST", payload, channel);
}

static void broadcastStateToChannel() {
    broadcastStateToChannelID(Teamspeak::getChannelOfClient(SERVER_CONN, Teamspeak::getMyId(SERVER_CONN)));
}

void sendStateToClient(unsigned int session) {
    // With the server extension the server pushes cached states on channel
    // joins itself — no client-side unicast needed.
    if (serverHasTFARSupport())
        return;
    sendPluginData("TFARST", buildStatePayload(), { session });
}

// Called by the bridge (main thread) for every received PluginDataTransmission.
void handleIncomingPluginData(unsigned int senderSession, const char *dataID, const std::string &data) {
    if (strcmp(dataID, "TFARSRV") == 0) {
        // The mumble-tfar server announced its TFAR extensions.
        if (!g_serverTFARSupport.exchange(true)) {
            Logger::log(LoggerTypes::teamspeakClientlog,
                        "Server-side TFAR extensions active (channel broadcast, state caching)", LogLevel_INFO);
        }
        return;
    }
    if (strcmp(dataID, "TFAR") == 0) {
        ts3plugin_onPluginCommandEventOld(SERVER_CONN.baseType(), "TFAR", data.c_str());
    } else if (strcmp(dataID, "TFARST") == 0) {
        const auto sep = data.find('\x01');
        if (sep == std::string::npos)
            return;
        const std::string nickname = data.substr(0, sep);
        const std::string metadata = data.substr(sep + 1);

        bool nicknameChanged = false;
        {
            std::lock_guard< std::mutex > lock(g_stateLock);
            g_metadataCache[senderSession] = metadata;
            auto &current = g_virtualNicknames[senderSession];
            if (current != nickname) {
                current         = nickname;
                nicknameChanged = true;
            }
        }

        if (nicknameChanged) {
            // Update Mumble's local nickname display so the roster shows in-game names.
            if (ClientUser *user = ClientUser::get(senderSession))
                user->setLocalNickname(QString::fromStdString(nickname));
            // Renames the clientData entry inside TFAR's serverData directory.
            Teamspeak::_onClientUpdated(SERVER_CONN, senderSession);
        }
    }
}

void forgetSession(unsigned int session) {
    std::lock_guard< std::mutex > lock(g_stateLock);
    g_virtualNicknames.erase(session);
    g_metadataCache.erase(session);
}

void setConnectedState(bool connectedAndSynchronized) {
    g_connectedAndSynchronized.store(connectedAndSynchronized);
    if (!connectedAndSynchronized) {
        g_serverTFARSupport.store(false);
        std::lock_guard< std::mutex > lock(g_stateLock);
        g_virtualNicknames.clear();
        g_metadataCache.clear();
        g_lastSentState.clear();
    }
}

dataType::TSChannelID channelFromMumble(unsigned int mumbleChannelId) {
    return toTSChannel(mumbleChannelId);
}

// ------------------------------------------------------ chat commands ------
// Port of ts3plugin_processCommand ("/tfar <cmd>" in the TS3 client).

void processChatCommand(const std::string &command) {
    const bool debuggingAllowed = TFAR::config.get< bool >(Setting::allowDebugging);

    if (debuggingAllowed && command == "diag") {
        std::stringstream diag;
        const auto now       = std::chrono::system_clock::now();
        const auto in_time_t = std::chrono::system_clock::to_time_t(now);
        diag << "diag from " << std::put_time(std::localtime(&in_time_t), "%H:%M:%S") << "\n";
        TFAR::getInstance().doDiagReport(diag);
        Teamspeak::printMessageToCurrentTab(diag.str().c_str());
        return;
    }
    if (debuggingAllowed && command == "pos") {
        std::stringstream diag;
        TFAR::getInstance().doTypeDiagReport("pos", diag);
        Teamspeak::printMessageToCurrentTab(diag.str().c_str());
        return;
    }
    if (debuggingAllowed && command.substr(0, 4) == "full") {
        std::stringstream date;
        const auto now       = std::chrono::system_clock::now();
        const auto in_time_t = std::chrono::system_clock::to_time_t(now);
        date << std::put_time(std::localtime(&in_time_t), "%d-%m_%H-%M-%S");
        const auto dateString = date.str();

        const QString basePathQ = QDir::toNativeSeparators(
            QDir::home().filePath(QLatin1String("mumble-tfar-logs/")) + QString::fromStdString(dateString)
            + QLatin1Char('/'));
        QDir().mkpath(basePathQ);
        const auto basePath = basePathQ.toStdString();

        auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(Teamspeak::getCurrentServerConnection());
        if (!clientDataDir)
            return;
        clientDataDir->forEachClient([&basePath](const std::shared_ptr< clientData > &cli) {
            auto messages = std::move(cli->messages);
            cli->offset   = 0;
            auto nick     = cli->getNickname();
            constexpr auto illegalChars = R"(\/:?"<>|)"sv;
            for (auto it = nick.begin(); it < nick.end(); ++it) {
                if (illegalChars.find(*it) != std::string::npos)
                    *it = ' ';
            }
            std::ofstream fs(basePath + "CL_" + nick + ".log");
            for (auto &msg : messages)
                fs << msg << '\n';
            std::ofstream vdl(basePath + "VDL_" + nick + ".log");
            cli->verboseDataLog(vdl);
        });

        std::stringstream diag;
        diag << "diag from " << dateString << "\n";
        TFAR::getInstance().doDiagReport(diag);
        std::ofstream fsd(basePath + "diag.log");
        fsd << command << "\n" << diag.str();

        std::stringstream pos;
        TFAR::getInstance().doTypeDiagReport("pos", pos);
        std::ofstream fsp(basePath + "pos.log");
        fsp << command << "\n" << pos.str();

        Teamspeak::printMessageToCurrentTab((std::string("TFAR: logged to ") + basePath).c_str());
        return;
    }
    if (command == "rstflt") {
        auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(Teamspeak::getCurrentServerConnection());
        if (!clientDataDir)
            return;
        clientDataDir->forEachClient([](const std::shared_ptr< clientData > &cli) {
            cli->effects.resetRadioEffect();
            Teamspeak::printMessageToCurrentTab((std::string("TFAR: filter reset ") + cli->getNickname()).c_str());
        });
        return;
    }
    if (debuggingAllowed && command == "debug") {
        TFAR::debugUI.run();
        return;
    }

    Teamspeak::printMessageToCurrentTab(
        "TFAR commands: /tfar diag, /tfar pos, /tfar full, /tfar rstflt, /tfar debug");
}

} // namespace ts3compat

// =========================================================================
// TeamspeakServerData (unchanged bookkeeping, copied from ts/src/Teamspeak.cpp)
// =========================================================================

std::vector< dataType::TSClientID > TeamspeakServerData::getMutedClients() {
    LockGuard_shared lock(m_criticalSection);
    std::vector< dataType::TSClientID > result;
    result.reserve(mutedClients.size());
    std::copy(mutedClients.begin(), mutedClients.end(), std::back_inserter(result));
    return result;
}

void TeamspeakServerData::setClientMuteStatus(TSClientID clientID, bool muted) {
    if (!clientID)
        return;
    LockGuard_exclusive lock(m_criticalSection);
    if (muted)
        mutedClients.emplace(clientID);
    else
        mutedClients.erase(clientID);
}

bool TeamspeakServerData::getClientMuteStatus(TSClientID clientID) {
    if (!clientID)
        return false;
    LockGuard_exclusive lock(m_criticalSection);
    return mutedClients.find(clientID) != mutedClients.end();
}

void TeamspeakServerData::clearMutedClients() {
    LockGuard_exclusive lock(m_criticalSection);
    mutedClients.clear();
}

dataType::TSClientID TeamspeakServerData::getMyClientID() {
    LockGuard_shared lock(m_criticalSection);
    return myClientID;
}

void TeamspeakServerData::setMyClientID(dataType::TSClientID val) {
    LockGuard_exclusive lock(m_criticalSection);
    myClientID = val;
}

std::string TeamspeakServerData::getMyOriginalNickname() {
    LockGuard_shared lock(m_criticalSection);
    return myOriginalNickname;
}

void TeamspeakServerData::setMyOriginalNickname(std::string val) {
    LockGuard_exclusive lock(m_criticalSection);
    myOriginalNickname = std::move(val);
}

dataType::TSChannelID TeamspeakServerData::getMyOriginalChannel() {
    LockGuard_shared lock(m_criticalSection);
    return myOriginalChannel;
}

void TeamspeakServerData::setMyOriginalChannel(TSChannelID val) {
    LockGuard_exclusive lock(m_criticalSection);
    myOriginalChannel = val;
}

dataType::TSChannelID TeamspeakServerData::getChannelIDFromName(const std::string &channelName) {
    LockGuard_shared lock(m_criticalSection);
    const auto found = channelNameToID.find(channelName);
    if (found != channelNameToID.end())
        return found->second;
    return -1;
}

void TeamspeakServerData::addChannelCache(const char *channelName, TSChannelID channelID) {
    LockGuard_exclusive lock(m_criticalSection);
    channelNameToID.insert_or_assign(channelName, channelID);
}

void TeamspeakServerData::clearChannelCache() {
    LockGuard_exclusive lock(m_criticalSection);
    channelNameToID.clear();
}

// =========================================================================
// Teamspeak facade
// =========================================================================

Teamspeak::Teamspeak() {
    TFAR::getInstance().doDiagReport.connect([](std::stringstream &diag) {
        diag << "TS (Mumble):\n";
        diag << TS_INDENT << "serverTFARSupport: " << ts3compat::serverHasTFARSupport() << "\n";
        for (auto &it : getInstance().serverData) {
            diag << TS_INDENT << it.first.baseType() << ":\n";
            diag << TS_INDENT << TS_INDENT << "myCID: " << getMyId(it.first).baseType() << "\n";
            diag << TS_INDENT << TS_INDENT << "myONICK: " << it.second.getMyOriginalNickname() << "\n";
            diag << TS_INDENT << TS_INDENT << "myNICK: " << getMyNickname(it.first) << "\n";
            diag << TS_INDENT << TS_INDENT << "myOCHAN: " << it.second.getMyOriginalChannel().baseType() << "\n";
            diag << TS_INDENT << TS_INDENT << "myCHAN: " << getChannelOfClient(it.first).baseType() << "\n";
            diag << TS_INDENT << TS_INDENT << "myLCHAN: " << it.second.myLastKnownChannel.baseType() << "\n";
            diag << TS_INDENT << TS_INDENT << "curCHAN:\n";
            std::vector< TSClientID > clients = getChannelClients(it.first, getChannelOfClient(it.first));
            for (TSClientID clientId : clients) {
                diag << TS_INDENT << TS_INDENT << TS_INDENT << clientId.baseType() << " : "
                     << getClientNickname(it.first, clientId) << "\n";
            }
        }
    });
}

Teamspeak &Teamspeak::getInstance() {
    static Teamspeak instance;
    return instance;
}

dataType::TSServerID Teamspeak::getCurrentServerConnection() {
    return SERVER_CONN;
}

bool Teamspeak::isConnected(TSServerID serverConnectionHandlerID) {
    if (serverConnectionHandlerID != SERVER_CONN)
        return false;
    return ts3compat::isConnectedAndSynchronized();
}

TSClientID Teamspeak::getMyId(TSServerID serverConnectionHandlerID) {
    if (!isConnected(serverConnectionHandlerID))
        return {};
    const unsigned int session = Global::get().uiSession;
    if (session == 0)
        return {};
    return TSClientID(session);
}

// ------------------------------------------------------------- muting ------

void Teamspeak::unmuteAll(TSServerID serverConnectionHandlerID) {
    auto mutedClients = getInstance().serverData[serverConnectionHandlerID].getMutedClients();
    if (mutedClients.empty())
        return;
    setClientMute(serverConnectionHandlerID, std::move(mutedClients), false);
}

void Teamspeak::setClientMute(TSServerID serverConnectionHandlerID, TSClientID clientID, bool mute) {
    if (!clientID)
        return;
    setClientMute(serverConnectionHandlerID, std::vector< TSClientID >{ clientID }, mute);
}

void Teamspeak::setClientMute(TSServerID serverConnectionHandlerID, std::vector< TSClientID > clientIds, bool mute) {
    if (clientIds.empty())
        return;
    auto &serverDataDir = getInstance().serverData[serverConnectionHandlerID];

    // remove clients that are already in the desired mute state
    clientIds.erase(std::remove_if(clientIds.begin(), clientIds.end(),
                                   [&](TSClientID client) { return serverDataDir.getClientMuteStatus(client) == mute; }),
                    clientIds.end());
    if (clientIds.empty())
        return;

    std::vector< unsigned int > sessions;
    sessions.reserve(clientIds.size());
    for (auto &client : clientIds) {
        serverDataDir.setClientMuteStatus(client, mute);
        sessions.push_back(client.baseType());
    }

    // qApp as receiver context: always alive on the main thread (the MainWindow
    // may not exist yet while the game pipe is already up).
    QMetaObject::invokeMethod(
        QCoreApplication::instance(),
        [sessions = std::move(sessions), mute]() {
            for (const unsigned int session : sessions) {
                if (ClientUser *user = ClientUser::get(session))
                    user->setLocalMute(mute);
            }
        },
        Qt::QueuedConnection);
}

// ---------------------------------------------------------- channels -------

void Teamspeak::_updateChanneNameCache(TSServerID serverConnectionHandlerID) {
    auto &serverData = getInstance().serverData[serverConnectionHandlerID];
    serverData.clearChannelCache();
    QReadLocker lock(&Channel::c_qrwlChannels);
    for (auto it = Channel::c_qhChannels.constBegin(); it != Channel::c_qhChannels.constEnd(); ++it) {
        const Channel *channel = it.value();
        if (channel)
            serverData.addChannelCache(channel->qsName.toUtf8().constData(), toTSChannel(channel->iId));
    }
}

TSChannelID Teamspeak::findChannelByName(TSServerID serverConnectionHandlerID, const std::string &channelName) {
    if (channelName.empty())
        return -1;
    // Channels can be created/renamed at any time — refresh the cache before lookup.
    _updateChanneNameCache(serverConnectionHandlerID);
    return getInstance().serverData[serverConnectionHandlerID].getChannelIDFromName(channelName);
}

bool Teamspeak::isInChannel(TSServerID serverConnectionHandlerID, TSClientID clientId,
                            const std::string &channelToCheck) {
    const auto currentChannelID = getChannelOfClient(serverConnectionHandlerID, clientId);
    // "ID:x" refers to the TFAR channel ID as reported to the game via
    // TS_INFO CHANNELID / "/tfar diag" (Mumble channel ID + 1) — same semantics
    // as the original plugin.
    if (helpers::startsWith("ID:", channelToCheck))
        return currentChannelID.baseType()
               == static_cast< uint64_t >(helpers::parseArmaNumberToInt(channelToCheck.substr(3)));
    return currentChannelID == findChannelByName(serverConnectionHandlerID, channelToCheck);
}

dataType::TSChannelID Teamspeak::getChannelOfClient(TSServerID serverConnectionHandlerID, TSClientID clientId) {
    if (!clientId)
        return -1;
    if (const ClientUser *user = ClientUser::get(clientId.baseType())) {
        if (user->cChannel)
            return toTSChannel(user->cChannel->iId);
    }
    return -1;
}

std::string Teamspeak::getChannelName(TSServerID serverConnectionHandlerID, TSClientID clientId) {
    if (!clientId)
        return "";
    const auto channelId = getChannelOfClient(serverConnectionHandlerID, clientId);
    if (!channelId)
        return "";
    QReadLocker lock(&Channel::c_qrwlChannels);
    if (const Channel *channel = Channel::get(toMumbleChannel(channelId)))
        return channel->qsName.toStdString();
    return "";
}

std::vector< TSClientID > Teamspeak::getChannelClients(TSServerID serverConnectionHandlerID, TSChannelID channelId) {
    std::vector< TSClientID > result;
    if (!channelId)
        return result;
    for (const unsigned int session : ts3compat::sessionsInChannel(channelId))
        result.emplace_back(session);
    return result;
}

void Teamspeak::moveToSeriousChannel(TSServerID serverConnectionHandlerID) {
#ifdef _WIN32
    auto foregroundHWND = GetForegroundWindow();
    if (foregroundHWND && !TFAR::config.get< bool >(Setting::moveWhileTabbedOut)) {
        wchar_t buffer[32];
        std::wstring className(buffer, GetClassNameW(foregroundHWND, buffer, 32));
        // Should be "Arma 3"
        std::transform(className.begin(), className.end(), className.begin(), ::tolower);
        if (className.find(L"arma") == std::string::npos)
            return; // No switch when no Arma
    }
#endif
    auto seriousChannelName = TFAR::config.get< std::string >(Setting::serious_channelName);

    // "ID:x" — TFAR channel ID (Mumble channel ID + 1), see isInChannel.
    auto seriousChannelID =
        helpers::startsWith("ID:", seriousChannelName)
            ? TSChannelID(helpers::parseArmaNumberToInt(seriousChannelName.substr(3)))
            : findChannelByName(serverConnectionHandlerID, seriousChannelName);

    if (!seriousChannelID) // Channel not found
        return;
    const auto currentChannel = getChannelOfClient(serverConnectionHandlerID, getMyId(serverConnectionHandlerID));
    if (currentChannel == seriousChannelID)
        return;
    getInstance().serverData[serverConnectionHandlerID].setMyOriginalChannel(currentChannel);
    const auto seriousChannelPassword = TFAR::config.get< std::string >(Setting::serious_channelPassword);

    const unsigned int targetChannel = toMumbleChannel(seriousChannelID);
    // qApp as receiver context: always alive on the main thread (the MainWindow
    // may not exist yet while the game pipe is already up).
    QMetaObject::invokeMethod(
        QCoreApplication::instance(),
        [targetChannel, seriousChannelPassword]() {
            ServerHandlerPtr sh = Global::get().sh;
            if (!sh || Global::get().uiSession == 0)
                return;
            QStringList tokens;
            if (!seriousChannelPassword.empty())
                tokens << QString::fromStdString(seriousChannelPassword);
            sh->joinChannel(Global::get().uiSession, targetChannel, tokens);
        },
        Qt::QueuedConnection);
}

void Teamspeak::moveFromSeriousChannel(TSServerID serverConnectionHandlerID) {
    const auto notSeriousChannelId = getInstance().serverData[serverConnectionHandlerID].getMyOriginalChannel();
    if (!notSeriousChannelId)
        return;
    if (getChannelOfClient(serverConnectionHandlerID, getMyId(serverConnectionHandlerID)) == notSeriousChannelId)
        return;

    const unsigned int targetChannel = toMumbleChannel(notSeriousChannelId);
    // qApp as receiver context: always alive on the main thread (the MainWindow
    // may not exist yet while the game pipe is already up).
    QMetaObject::invokeMethod(
        QCoreApplication::instance(),
        [targetChannel]() {
            ServerHandlerPtr sh = Global::get().sh;
            if (!sh || Global::get().uiSession == 0)
                return;
            sh->joinChannel(Global::get().uiSession, targetChannel, QStringList());
        },
        Qt::QueuedConnection);

    getInstance().serverData[serverConnectionHandlerID].setMyOriginalChannel(-1);
}

// --------------------------------------------------------- nicknames -------

std::string Teamspeak::getClientNickname(TSServerID serverConnectionHandlerID, TSClientID clientId) {
    if (!clientId)
        return "";
    {
        std::lock_guard< std::mutex > lock(ts3compat::g_stateLock);
        const auto found = ts3compat::g_virtualNicknames.find(clientId.baseType());
        if (found != ts3compat::g_virtualNicknames.end() && !found->second.empty())
            return found->second;
    }
    if (const ClientUser *user = ClientUser::get(clientId.baseType()))
        return user->qsName.toStdString();
    return "";
}

std::string Teamspeak::getMyNickname(TSServerID serverConnectionHandlerID) {
    const auto myId = getMyId(serverConnectionHandlerID);
    if (!myId)
        return "";
    return getClientNickname(serverConnectionHandlerID, myId);
}

bool Teamspeak::setMyNicknameToGameName(TSServerID serverConnectionHandlerID, const std::string &nickname) {
    if (getMyNickname(serverConnectionHandlerID) != getInstance().serverData[serverConnectionHandlerID].getMyOriginalNickname())
        getInstance().serverData[serverConnectionHandlerID].setMyOriginalNickname(getMyNickname(serverConnectionHandlerID));

    const unsigned int mySession = Global::get().uiSession;
    if (mySession == 0)
        return false;

    {
        std::lock_guard< std::mutex > lock(ts3compat::g_stateLock);
        ts3compat::g_virtualNicknames[mySession] = nickname;
    }
    // Show the in-game name in our own Mumble UI as well.
    // qApp as receiver context: always alive on the main thread (the MainWindow
    // may not exist yet while the game pipe is already up).
    QMetaObject::invokeMethod(
        QCoreApplication::instance(),
        [mySession, nickname]() {
            if (ClientUser *user = ClientUser::get(mySession))
                user->setLocalNickname(QString::fromStdString(nickname));
        },
        Qt::QueuedConnection);

    ts3compat::broadcastStateToChannel();
    return true;
}

void Teamspeak::resetMyNickname(TSServerID serverConnectionHandlerID) {
    const auto origNickname = getInstance().serverData[serverConnectionHandlerID].getMyOriginalNickname();
    if (origNickname.empty())
        return;

    const unsigned int mySession = Global::get().uiSession;
    {
        std::lock_guard< std::mutex > lock(ts3compat::g_stateLock);
        ts3compat::g_virtualNicknames.erase(mySession);
    }
    getInstance().serverData[serverConnectionHandlerID].setMyOriginalNickname("");

    // qApp as receiver context: always alive on the main thread (the MainWindow
    // may not exist yet while the game pipe is already up).
    QMetaObject::invokeMethod(
        QCoreApplication::instance(),
        [mySession]() {
            if (ClientUser *user = ClientUser::get(mySession))
                user->setLocalNickname(QString());
        },
        Qt::QueuedConnection);

    ts3compat::broadcastStateToChannel();
}

// ----------------------------------------------------------- metadata ------

std::string Teamspeak::getMetaData(TSServerID serverConnectionHandlerID, TSClientID clientId) {
    if (!clientId)
        return "";
    std::lock_guard< std::mutex > lock(ts3compat::g_stateLock);
    if (clientId.baseType() == Global::get().uiSession)
        return ts3compat::g_myMetadata;
    const auto found = ts3compat::g_metadataCache.find(clientId.baseType());
    if (found != ts3compat::g_metadataCache.end())
        return found->second;
    return "";
}

void Teamspeak::setMyMetaData(const std::string &metaData) {
    {
        std::lock_guard< std::mutex > lock(ts3compat::g_stateLock);
        ts3compat::g_myMetadata = metaData;
    }
    ts3compat::broadcastStateToChannel();
}

// -------------------------------------------------------- server info ------

std::string Teamspeak::getServerName(TSServerID serverConnectionHandlerID) {
    ServerHandlerPtr sh = Global::get().sh;
    if (!sh)
        return "ERROR_GETTING_SERVER_NAME";
    // qsHostName/usPort are protected — use the public accessor.
    // Note: Mumble builds with QT_USE_FAST_CONCATENATION, so operator+ yields a
    // QStringBuilder — materialize a QString before calling toStdString().
    QString host, username, pw;
    unsigned short port = 0;
    sh->getConnectionInfo(host, port, username, pw);
    const QString serverName = host + QLatin1Char(':') + QString::number(port);
    return serverName.toStdString();
}

std::string Teamspeak::getServerUID(TSServerID serverConnectionHandlerID) {
    return getServerName(serverConnectionHandlerID);
}

// ------------------------------------------------------- talk status -------

bool Teamspeak::isTalking(TSServerID currentServerConnectionHandlerID, TSClientID clientID) {
    if (!clientID)
        return false;
    if (const ClientUser *user = ClientUser::get(clientID.baseType()))
        return user->tsState != Settings::Passive && user->tsState != Settings::MutedTalking;
    return false;
}

// -------------------------------------------------------- plugin cmds ------

void Teamspeak::sendPluginCommand(TSServerID serverConnectionHandlerID, std::string_view pluginID,
                                  std::string_view command, PluginTargetMode targetMode,
                                  std::vector< TSClientID > targets) {
    std::vector< unsigned int > receivers;
    bool loopbackToSelf = false;

    switch (targetMode) {
        case PluginCommandTarget_CURRENT_CHANNEL:
        case PluginCommandTarget_CURRENT_CHANNEL_SUBSCRIBED_CLIENTS: {
            // TeamSpeak delivers channel commands back to the sender as well
            loopbackToSelf = true;
            if (ts3compat::serverHasTFARSupport()) {
                // Server extension: the server fans the message out to our channel.
                ts3compat::sendPluginData("TFAR", std::string(command), {}, true);
                ts3plugin_onPluginCommandEventOld(serverConnectionHandlerID.baseType(), "TFAR",
                                                  std::string(command).c_str());
                return;
            }
            const auto myChannel = getChannelOfClient(serverConnectionHandlerID, getMyId(serverConnectionHandlerID));
            receivers            = ts3compat::sessionsInChannel(myChannel);
            break;
        }
        case PluginCommandTarget_SERVER:
            receivers      = ts3compat::allSessions();
            loopbackToSelf = true;
            break;
        case PluginCommandTarget_CLIENT:
            for (const auto &target : targets) {
                if (target.baseType() == Global::get().uiSession)
                    loopbackToSelf = true;
                else
                    receivers.push_back(target.baseType());
            }
            break;
        default:
            return;
    }

    ts3compat::sendPluginData("TFAR", std::string(command), std::move(receivers));

    if (loopbackToSelf)
        ts3plugin_onPluginCommandEventOld(serverConnectionHandlerID.baseType(), "TFAR",
                                          std::string(command).c_str());
}

// ------------------------------------------------------------- sounds ------

void Teamspeak::playWavFile(const std::string &filePath) {
    const QString path = QString::fromStdString(filePath);
    // qApp as receiver context: always alive on the main thread (the MainWindow
    // may not exist yet while the game pipe is already up).
    QMetaObject::invokeMethod(
        QCoreApplication::instance(),
        [path]() {
            AudioOutputPtr ao = Global::get().ao;
            if (ao)
                ao->playSample(path, 1.0f, false);
        },
        Qt::QueuedConnection);
}

// -------------------------------------------------------- voice input ------

void Teamspeak::setVoiceDisabled(TSServerID serverConnectionHandlerID, bool disabled) {
    ts3compat::g_tfarInputActive.store(!disabled);
    ts3compat::updateForcedTransmission();
}

bool Teamspeak::hlp_checkVad() {
    return Global::get().s.atTransmit == Settings::VAD;
}

void Teamspeak::hlp_enableVad() {
    ts3compat::g_tfarVadSuppressed.store(false);
    ts3compat::updateForcedTransmission();
}

void Teamspeak::hlp_disableVad() {
    ts3compat::g_tfarVadSuppressed.store(true);
    ts3compat::updateForcedTransmission();
}

// ------------------------------------------------------------ logging ------

void Teamspeak::log(std::string message, unsigned long errorCode, LogLevel level) {
    if (errorCode != 0)
        message += " : error " + std::to_string(errorCode);
    Logger::log(LoggerTypes::teamspeakClientlog, message, level);
}

void Teamspeak::printMessageToCurrentTab(const char *msg) {
    const QString html = ts3compat::bbCodeToHtml(msg);
    // qApp as receiver context: always alive on the main thread (the MainWindow
    // may not exist yet while the game pipe is already up).
    QMetaObject::invokeMethod(
        QCoreApplication::instance(),
        [html]() {
            if (Global::get().l)
                Global::get().l->log(Log::Information, html);
        },
        Qt::QueuedConnection);
}

// ----------------------------------------------------- 3D positioning ------
// TFAR performs all spatialization itself (Clunk HRTF + X3DAudio ILD). The
// TeamSpeak 3D engine calls therefore become no-ops in the Mumble build.

void Teamspeak::setMyClient3DPosition(TSServerID serverConnectionHandlerID, dataType::Position3D pos) {
    (void) serverConnectionHandlerID;
    (void) pos;
}

void Teamspeak::setClient3DPosition(TSServerID serverConnectionHandlerID, TSClientID clientId,
                                    dataType::Position3D pos) {
    (void) serverConnectionHandlerID;
    (void) clientId;
    (void) pos;
}

// ------------------------------------------------------------- events ------
// These are verbatim from the original Teamspeak.cpp — they only orchestrate
// TFAR signals and the bookkeeping above.

void Teamspeak::_onConnectStatusChangeEvent(TSServerID serverConnectionHandlerID, ConnectStatus newState) {
    if (newState == STATUS_CONNECTION_ESTABLISHED) {
        _updateChanneNameCache(serverConnectionHandlerID);
        if (TFAR::getInstance().getCurrentlyInGame())
            moveToSeriousChannel(serverConnectionHandlerID); // rejoin channel at server reconnect

        TFAR::getInstance().onTeamspeakServerConnect(serverConnectionHandlerID);
        _onChannelSwitchedEvent(serverConnectionHandlerID, getChannelOfClient(serverConnectionHandlerID));

        auto clientDataDir = TFAR::getServerDataDirectory()->getClientDataDirectory(serverConnectionHandlerID);
        if (clientDataDir)
            clientDataDir->myClientData = clientDataDir->getClientData(getMyId(serverConnectionHandlerID));
    } else if (newState == STATUS_DISCONNECTED) {
        TFAR::getInstance().onTeamspeakServerDisconnect(serverConnectionHandlerID);
        getInstance().serverData.erase(serverConnectionHandlerID);
    }
}

void Teamspeak::_onChannelSwitchedEvent(TSServerID serverConnectionHandlerID, TSChannelID newChannel) {
    if (getInstance().serverData[serverConnectionHandlerID].myLastKnownChannel == newChannel)
        return;
    getInstance().serverData[serverConnectionHandlerID].myLastKnownChannel = newChannel;

    TFAR::getInstance().onTeamspeakClientLeft(serverConnectionHandlerID, -2);

    auto clients = getChannelClients(serverConnectionHandlerID, newChannel);
    for (const auto &clientId : clients) {
        auto clientNickname = getClientNickname(serverConnectionHandlerID, clientId);
        if (clientNickname.empty())
            continue;
        TFAR::getInstance().onTeamspeakClientJoined(serverConnectionHandlerID, clientId, clientNickname);
    }
    TFAR::getInstance().onTeamspeakChannelSwitched(serverConnectionHandlerID, newChannel);

    // Everyone in the new channel needs our current state (in-game name, plugin status).
    // Note: this event can fire before Mumble applied our own channel change, so
    // explicitly target the new channel instead of querying our current one.
    ts3compat::broadcastStateToChannelID(newChannel);
}

void Teamspeak::_onClientMoved(TSServerID serverConnectionHandlerID, TSClientID clientID, TSChannelID oldChannel,
                               TSChannelID newChannel) {
    if (clientID == getMyId(serverConnectionHandlerID)) { // we switched channel
        _onChannelSwitchedEvent(serverConnectionHandlerID, newChannel);
        return;
    }

    if (getInstance().serverData[serverConnectionHandlerID].myLastKnownChannel == newChannel) {
        auto clientNickname = getClientNickname(serverConnectionHandlerID, clientID);
        if (clientNickname.empty())
            return;
        TFAR::getInstance().onTeamspeakClientJoined(serverConnectionHandlerID, clientID, clientNickname);
        // The newcomer needs our state (in-game name, plugin status).
        ts3compat::sendStateToClient(clientID.baseType());
    } else if (getInstance().serverData[serverConnectionHandlerID].myLastKnownChannel == oldChannel) {
        if (getInstance().serverData[serverConnectionHandlerID].getClientMuteStatus(clientID))
            setClientMute(serverConnectionHandlerID, clientID, false); // Unmute him if he is muted
        TFAR::getInstance().onTeamspeakClientLeft(serverConnectionHandlerID, clientID);
    }
}

void Teamspeak::_onClientJoined(TSServerID serverConnectionHandlerID, TSClientID clientID, TSChannelID channel) {
    if (getInstance().serverData[serverConnectionHandlerID].myLastKnownChannel != channel)
        return;

    auto clientNickname = getClientNickname(serverConnectionHandlerID, clientID);
    if (clientNickname.empty())
        return;
    TFAR::getInstance().onTeamspeakClientJoined(serverConnectionHandlerID, clientID, clientNickname);

    ts3compat::sendStateToClient(clientID.baseType());

    // Request the client's voice volume — keeps behaviour identical to TS build.
    const auto command = "REQVOL\t" + std::to_string(getMyId().baseType());
    Teamspeak::sendPluginCommand(getCurrentServerConnection(), TFAR::getInstance().getPluginID(), command,
                                 PluginCommandTarget_CLIENT, { clientID });
}

void Teamspeak::_onClientLeft(TSServerID serverConnectionHandlerID, TSClientID clientID) {
    if (getInstance().serverData[serverConnectionHandlerID].getClientMuteStatus(clientID))
        setClientMute(serverConnectionHandlerID, clientID, false); // Unmute him if he was muted
    TFAR::getInstance().onTeamspeakClientLeft(serverConnectionHandlerID, clientID);
    ts3compat::forgetSession(clientID.baseType());
}

void Teamspeak::_onClientUpdated(TSServerID serverConnectionHandlerID, TSClientID clientID) {
    std::string clientNickname = getClientNickname(serverConnectionHandlerID, clientID);
    if (clientNickname.empty())
        return;
    TFAR::getInstance().onTeamspeakClientUpdated(serverConnectionHandlerID, clientID, clientNickname);
}

void Teamspeak::_onInit() {
    // Called on plugin init. If Mumble is already connected to a server, replay the connect event.
    if (ts3compat::isConnectedAndSynchronized())
        _onConnectStatusChangeEvent(SERVER_CONN, STATUS_CONNECTION_ESTABLISHED);
}
