// MUMBLE-TFAR
// Qt glue object wiring Mumble client signals into the TFAR plugin code.

#include "TFARBridge.h"

#include "../ClientUser.h"
#include "../Global.h"
#include "../Log.h"
#include "../MainWindow.h"

#include "include/ts3_functions.h"
#include "plugin.h"

#include "Logger.hpp"
#include "TS3Compat.h"
#include "Teamspeak.hpp"
#include "task_force_radio.hpp"

#include <algorithm>
#include <cstring>

// plugin.cpp
extern volatile bool pipeConnected;

static TFARBridge *s_tfarBridge = nullptr;

TFARBridge::TFARBridge(QObject *parent) : QObject(parent) {
}

TFARBridge *TFARBridge::instance() {
    return s_tfarBridge;
}

void TFARBridge::initialize() {
    if (s_tfarBridge)
        return;
    s_tfarBridge = new TFARBridge();

    // Route TFAR's internal client log into the Mumble log.
    Logger::registerLogger(LoggerTypes::teamspeakClientlog,
                           std::make_shared< TeamspeakLogger >(LogLevel::LogLevel_INFO));

    TFAR::getInstance().setPluginID("TFAR");

    // Status messages in the Mumble chat log when the game attaches/detaches.
    TFAR::getInstance().onGameConnected.connect(
        []() { Teamspeak::printMessageToCurrentTab("TFAR: [B]Arma 3 connected[/B]"); });
    TFAR::getInstance().onGameDisconnected.connect(
        []() { Teamspeak::printMessageToCurrentTab("TFAR: [B]Arma 3 disconnected[/B]"); });

    // Starts the game connection (shared memory / named pipe) and service threads.
    ts3plugin_init();
}

void TFARBridge::shutdown() {
    if (!s_tfarBridge)
        return;
    ts3plugin_shutdown();
    delete s_tfarBridge;
    s_tfarBridge = nullptr;
}

bool TFARBridge::isGameConnected() {
    return pipeConnected;
}

bool TFARBridge::isInGame() {
    return TFAR::getInstance().getCurrentlyInGame();
}

// ------------------------------------------------------------ events -------

void TFARBridge::onServerSynchronized() {
    ts3compat::setConnectedState(true);
    Teamspeak::_onConnectStatusChangeEvent(Teamspeak::getCurrentServerConnection(), STATUS_CONNECTION_ESTABLISHED);

    // Track our own talking state to broadcast the VOLUME plugin command
    // (equivalent of TS3's onClientSelfVariableUpdateEvent CLIENT_FLAG_TALKING).
    if (ClientUser *self = ClientUser::get(Global::get().uiSession)) {
        connect(self, &ClientUser::talkingStateChanged, this, &TFARBridge::onSelfTalkingStateChanged,
                Qt::UniqueConnection);
    }
    m_lastSelfTalking = false;
}

void TFARBridge::onServerDisconnected() {
    ts3compat::setConnectedState(false);
    Teamspeak::_onConnectStatusChangeEvent(Teamspeak::getCurrentServerConnection(), STATUS_DISCONNECTED);
    m_lastSelfTalking = false;
}

void TFARBridge::onUserAdded(unsigned int userSessionID) {
    if (!ts3compat::isConnectedAndSynchronized() || userSessionID == Global::get().uiSession)
        return;
    const auto server = Teamspeak::getCurrentServerConnection();
    Teamspeak::_onClientJoined(server, userSessionID, Teamspeak::getChannelOfClient(server, userSessionID));
}

void TFARBridge::onUserRemoved(unsigned int userSessionID) {
    if (userSessionID == Global::get().uiSession)
        return;
    Teamspeak::_onClientLeft(Teamspeak::getCurrentServerConnection(), userSessionID);
}

void TFARBridge::onUserMoved(unsigned int userSessionID, std::optional< unsigned int > prevChannelID,
                             unsigned int newChannelID) {
    if (!ts3compat::isConnectedAndSynchronized())
        return;
    const dataType::TSChannelID oldChannel =
        prevChannelID ? ts3compat::channelFromMumble(*prevChannelID) : dataType::TSChannelID(-1);
    Teamspeak::_onClientMoved(Teamspeak::getCurrentServerConnection(), userSessionID, oldChannel,
                              ts3compat::channelFromMumble(newChannelID));
}

void TFARBridge::onSelfTalkingStateChanged() {
    const ClientUser *self = ClientUser::get(Global::get().uiSession);
    if (!self)
        return;
    const bool talking = self->tsState == Settings::Talking || self->tsState == Settings::Shouting
                         || self->tsState == Settings::Whispering;
    if (talking == m_lastSelfTalking)
        return;
    m_lastSelfTalking = talking;
    ts3plugin_onClientSelfVariableUpdateEvent(Teamspeak::getCurrentServerConnection().baseType(), CLIENT_FLAG_TALKING,
                                              talking ? "0" : "1", talking ? "1" : "0");
}

// ------------------------------------------------------- plugin data -------

void TFARBridge::handleReceivedPluginData(unsigned int senderSession, const QString &dataID, const QByteArray &data) {
    const QByteArray id = dataID.toUtf8();
    if (!id.startsWith("TFAR"))
        return;
    ts3compat::handleIncomingPluginData(senderSession, id.constData(),
                                        std::string(data.constData(), static_cast< size_t >(data.size())));
}

// ---------------------------------------------------------- chat cmds ------

bool TFARBridge::handleChatCommand(const QString &text) {
    const QString trimmed = text.trimmed();
    if (!trimmed.startsWith(QLatin1String("/tfar"), Qt::CaseInsensitive))
        return false;
    const QString argument = trimmed.mid(5).trimmed();
    ts3compat::processChatCommand(argument.toStdString());
    return true;
}

// -------------------------------------------------------------- audio ------
// All three slots run on the audio threads via direct connections — exactly
// like TS3 invoked the corresponding plugin callbacks on its audio threads.

void TFARBridge::onAudioInput(short *inputPCM, unsigned int sampleCount, unsigned int channelCount,
                              unsigned int sampleRate, bool isSpeech) {
    Q_UNUSED(sampleRate)
    // TS3 semantics: bit 1 of "edited" == "sound will be sent to the server".
    int edited = isSpeech ? 2 : 0;
    ts3plugin_onEditCapturedVoiceDataEvent(Teamspeak::getCurrentServerConnection().baseType(), inputPCM,
                                           static_cast< int >(sampleCount), static_cast< int >(channelCount), &edited);
}

void TFARBridge::onAudioSourceFetched(float *outputPCM, unsigned int sampleCount, unsigned int channelCount,
                                      unsigned int sampleRate, bool isSpeech, const ClientUser *user) {
    Q_UNUSED(sampleRate)
    if (!user || !isSpeech || !outputPCM)
        return;
    if (!TFAR::getInstance().getCurrentlyInGame() || !ts3compat::isConnectedAndSynchronized())
        return;

    const size_t total = static_cast< size_t >(sampleCount) * channelCount;
    if (m_sourceScratch.size() < total)
        m_sourceScratch.resize(total);

    for (size_t i = 0; i < total; ++i) {
        const float sample  = std::clamp(outputPCM[i], -1.0f, 1.0f);
        m_sourceScratch[i] = static_cast< short >(sample * 32767.0f);
    }

    ts3plugin_onEditPostProcessVoiceDataEvent(Teamspeak::getCurrentServerConnection().baseType(), user->uiSession,
                                              m_sourceScratch.data(), static_cast< int >(sampleCount),
                                              static_cast< int >(channelCount), nullptr, nullptr);

    for (size_t i = 0; i < total; ++i)
        outputPCM[i] = static_cast< float >(m_sourceScratch[i]) / 32768.0f;
}

void TFARBridge::onAudioOutputAboutToPlay(float *outputPCM, unsigned int sampleCount, unsigned int channelCount,
                                          unsigned int sampleRate, bool *modifiedAudio) {
    Q_UNUSED(sampleRate)
    if (!outputPCM)
        return;

    const size_t total = static_cast< size_t >(sampleCount) * channelCount;
    if (m_mixScratch.size() < total)
        m_mixScratch.resize(total);
    memset(m_mixScratch.data(), 0, total * sizeof(short));

    // TS3 semantics: TFAR's PlaybackHandler mixes its pending radio sounds and
    // processed self-voice into the final output buffer.
    unsigned int channelFillMask = 0;
    TFAR::getPlaybackHandler()->onEditMixedPlaybackVoiceDataEvent(m_mixScratch.data(), static_cast< int >(sampleCount),
                                                                  static_cast< int >(channelCount), nullptr,
                                                                  &channelFillMask);

    if (channelFillMask != 0) {
        for (size_t i = 0; i < total; ++i)
            outputPCM[i] += static_cast< float >(m_mixScratch[i]) / 32768.0f;
        if (modifiedAudio)
            *modifiedAudio = true;
    }
}
