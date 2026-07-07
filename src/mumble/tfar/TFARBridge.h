// MUMBLE-TFAR
// Qt glue object wiring Mumble client signals into the TFAR plugin code.

#ifndef MUMBLE_MUMBLE_TFAR_TFARBRIDGE_H_
#define MUMBLE_MUMBLE_TFAR_TFARBRIDGE_H_

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <optional>
#include <vector>

class ClientUser;

class TFARBridge : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(TFARBridge)

public:
    /// Creates the bridge singleton and starts the TFAR plugin core
    /// (game pipe / shared memory threads). Call once on client startup,
    /// after the Global instance and the MainWindow exist.
    static void initialize();
    /// Stops the TFAR plugin core. Call once on client shutdown.
    static void shutdown();
    /// nullptr until initialize() was called.
    static TFARBridge *instance();

    /// Handles a received PluginDataTransmission (called from Messages.cpp, main thread).
    void handleReceivedPluginData(unsigned int senderSession, const QString &dataID, const QByteArray &data);

    /// Handles "/tfar ..." chat commands. Returns true if the text was a TFAR
    /// command and must not be sent as a chat message.
    bool handleChatCommand(const QString &text);

    /// Status for the config UI / diagnostics.
    static bool isGameConnected();
    static bool isInGame();

public slots:
    // server / roster events (main thread)
    void onServerSynchronized();
    void onServerDisconnected();
    void onUserAdded(unsigned int userSessionID);
    void onUserRemoved(unsigned int userSessionID);
    void onUserMoved(unsigned int userSessionID, std::optional< unsigned int > prevChannelID,
                     unsigned int newChannelID);

    // audio pipeline (audio threads, direct connections)
    void onAudioInput(short *inputPCM, unsigned int sampleCount, unsigned int channelCount, unsigned int sampleRate,
                      bool isSpeech);
    void onAudioSourceFetched(float *outputPCM, unsigned int sampleCount, unsigned int channelCount,
                              unsigned int sampleRate, bool isSpeech, const ClientUser *user);
    void onAudioOutputAboutToPlay(float *outputPCM, unsigned int sampleCount, unsigned int channelCount,
                                  unsigned int sampleRate, bool *modifiedAudio);

private slots:
    void onSelfTalkingStateChanged();

private:
    explicit TFARBridge(QObject *parent = nullptr);
    ~TFARBridge() override = default;

    bool m_lastSelfTalking = false;
    // scratch buffers for float <-> short conversion in the audio callbacks
    std::vector< short > m_sourceScratch;
    std::vector< short > m_mixScratch;
};

#endif // MUMBLE_MUMBLE_TFAR_TFARBRIDGE_H_
