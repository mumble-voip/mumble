// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SETTINGS_H_
#define MUMBLE_MUMBLE_SETTINGS_H_

#include <QByteArray>
#include <QColor>
#include <QFont>
#include <QHash>
#include <QList>
#include <QMap>
#include <QPair>
#include <QPoint>
#include <QRectF>
#include <QSslCertificate>
#include <QSslKey>
#include <QStandardPaths>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <Qt>

#include "Channel.h"
#include "EchoCancelOption.h"
#include "QuitBehavior.h"
#include "SSL.h"
#include "SearchDialog.h"

#include <nlohmann/json_fwd.hpp>

#include <array>

class QSettings;
struct MigratedPath;

// Global helper classes to spread variables around across threads
// especially helpful to initialize things like the stored
// preference for audio transmission, since the GUI elements
// will be created long before the AudioInput object, and the
// latter lives in a separate thread and so cannot touch the
// GUI.

struct Shortcut {
	int iIndex                  = 0;
	QList< QVariant > qlButtons = {};
	QVariant qvData             = {};
	bool bSuppress              = false;

	bool operator<(const Shortcut &) const;
	bool isServerSpecific() const;
	bool operator==(const Shortcut &) const;
};

struct ChannelTarget {
	unsigned int channelID = Channel::ROOT_ID;

	ChannelTarget() = default;
	ChannelTarget(unsigned int id) : channelID(id) {}

	friend bool operator==(const ChannelTarget &lhs, const ChannelTarget &rhs);
	friend bool operator<(const ChannelTarget &lhs, const ChannelTarget &rhs);
};

Q_DECLARE_METATYPE(ChannelTarget)

std::size_t qHash(const ChannelTarget &);

struct ShortcutTarget {
	bool bCurrentSelection           = false;
	bool bUsers                      = true;
	QStringList qlUsers              = {};
	QList< unsigned int > qlSessions = {};
	int iChannel                     = -3;
	QString qsGroup                  = {};
	bool bLinks                      = false;
	bool bChildren                   = false;
	bool bForceCenter                = false;

	ShortcutTarget() = default;
	bool isServerSpecific() const;
	bool operator==(const ShortcutTarget &) const;
};

Q_DECLARE_METATYPE(ShortcutTarget)

std::size_t qHash(const ShortcutTarget &);
std::size_t qHash(const QList< ShortcutTarget > &);

struct PluginSetting {
	QString path                 = {};
	bool enabled                 = false;
	bool positionalDataEnabled   = false;
	bool allowKeyboardMonitoring = false;

	friend bool operator==(const PluginSetting &lhs, const PluginSetting &rhs);
	friend bool operator!=(const PluginSetting &lhs, const PluginSetting &rhs);
};


struct OverlaySettings {
	enum OverlayPresets { AvatarAndName, LargeSquareAvatar };
	enum OverlayShow { Talking, Active, HomeChannel, LinkedChannels };
	enum OverlaySort { Alphabetical, LastStateChange };
	enum OverlayExclusionMode { LauncherFilterExclusionMode, WhitelistExclusionMode, BlacklistExclusionMode };

	bool bEnable = false;

	QString qsStyle = {};

	OverlayShow osShow = LinkedChannels;
	bool bAlwaysSelf   = true;
	// Time in seconds for a user to stay active after talking
	int uiActiveTime   = 5;
	OverlaySort osSort = Alphabetical;

	float fX = 1.0f;
	float fY = 0.0f;

	qreal fZoom            = 0.875f;
	unsigned int uiColumns = 1;

	std::array< QColor, 5 > qcUserName = {};
	QFont qfUserName                   = {};

	QColor qcChannel = QColor(255, 255, 128);
	QFont qfChannel  = {};

	QColor qcFps = Qt::white;
	QFont qfFps  = {};

	qreal fBoxPad      = 0.0f;
	qreal fBoxPenWidth = 0.0f;
	QColor qcBoxPen    = QColor(0, 0, 0, 224);
	QColor qcBoxFill   = QColor(0, 0, 0);

	bool bUserName      = true;
	bool bChannel       = false;
	bool bMutedDeafened = true;
	bool bAvatar        = true;
	bool bBox           = false;
	bool bFps           = false;
	bool bTime          = false;

	qreal fUserName              = 0.0f;
	qreal fChannel               = 0.0f;
	qreal fMutedDeafened         = 0.5f;
	qreal fAvatar                = 0.0f;
	std::array< qreal, 5 > fUser = {};
	qreal fFps                   = 0.75f;

	QRectF qrfUserName      = {};
	QRectF qrfChannel       = {};
	QRectF qrfMutedDeafened = {};
	QRectF qrfAvatar        = {};
	QRectF qrfFps           = QRectF(0.0f, 0.05f, -1, 0.023438f);
	QRectF qrfTime          = QRectF(0.0f, 0.0f, -1, 0.023438f);

	Qt::Alignment qaUserName      = Qt::AlignLeft;
	Qt::Alignment qaChannel       = Qt::AlignLeft;
	Qt::Alignment qaMutedDeafened = Qt::AlignLeft;
	Qt::Alignment qaAvatar        = Qt::AlignLeft;

	OverlayExclusionMode oemOverlayExcludeMode = LauncherFilterExclusionMode;
	QStringList qslLaunchers                   = {};
	QStringList qslLaunchersExclude            = {};
	QStringList qslWhitelist                   = {};
	QStringList qslWhitelistExclude            = {};
	QStringList qslPaths                       = {};
	QStringList qslPathsExclude                = {};
	QStringList qslBlacklist                   = {};
	QStringList qslBlacklistExclude            = {};

	OverlaySettings();
	void setPreset(const OverlayPresets preset = AvatarAndName);

	void load(const QString &filename);
	void savePresets(const QString &filename);
	void legacyLoad(QSettings *settings);

	friend bool operator==(const OverlaySettings &lhs, const OverlaySettings &rhs);
	friend bool operator!=(const OverlaySettings &lhs, const OverlaySettings &rhs);
};

struct Settings {
	enum AudioTransmit { Continuous, VAD, PushToTalk };
	enum VADSource { Amplitude, SignalToNoise };
	enum LoopMode { None, Local, Server };
	enum ChannelExpand { NoChannels, ChannelsWithUsers, AllChannels };
	enum ChannelDrag { Ask, DoNothing, Move };
	enum ServerShow { ShowPopulated, ShowReachable, ShowAll };
	enum TalkState { Passive, Talking, Whispering, Shouting, MutedTalking };
	enum IdleAction { Nothing, Deafen, Mute };
	enum NoiseCancel { NoiseCancelOff, NoiseCancelSpeex, NoiseCancelRNN, NoiseCancelBoth };
	enum MessageLog {
		LogNone         = 0x00,
		LogConsole      = 0x01,
		LogTTS          = 0x02,
		LogBalloon      = 0x04,
		LogSoundfile    = 0x08,
		LogHighlight    = 0x10,
		LogMessageLimit = 0x20,
	};
	enum WindowLayout { LayoutClassic, LayoutStacked, LayoutHybrid, LayoutCustom };
	enum AlwaysOnTopBehaviour { OnTopNever, OnTopAlways, OnTopInMinimal, OnTopInNormal };
	enum ProxyType { NoProxy, HttpProxy, Socks5Proxy };
	enum RecordingMode { RecordingMixdown, RecordingMultichannel };

	typedef QPair< QList< QSslCertificate >, QSslKey > KeyPair;

	static const QString cqsDefaultPushClickOn;
	static const QString cqsDefaultPushClickOff;
	static const QString cqsDefaultMuteCue;
	static const QPoint UNSPECIFIED_POSITION;

	AudioTransmit atTransmit = VAD;
	quint64 uiDoublePush     = 0;
	quint64 pttHold          = 0;

	bool audioCueEnabledPTT = true;
	bool audioCueEnabledVAD = false;
	QString qsTxAudioCueOn  = cqsDefaultPushClickOn;
	QString qsTxAudioCueOff = cqsDefaultPushClickOff;

	bool bTxMuteCue     = true;
	bool muteCueShown   = false;
	QString qsTxMuteCue = cqsDefaultMuteCue;

	bool bTransmitPosition         = false;
	bool unmuteOnUndeaf            = false;
	bool bMute                     = false;
	bool bDeaf                     = false;
	bool bTTS                      = false;
	bool bUserTop                  = true;
	float notificationVolume       = 1.0f;
	float cueVolume                = 1.0f;
	bool bWhisperFriends           = false;
	int iMessageLimitUserThreshold = 20;
	bool bTTSMessageReadBack       = false;
	bool bTTSNoScope               = false;
	bool bTTSNoAuthor              = false;
	int iTTSVolume                 = 75;
	int iTTSThreshold              = 250;
	/// The Text-to-Speech language to use. This setting overrides
	/// the default language for the Text-to-Speech engine, which
	/// is usually inferred from the current locale.
	///
	/// The language is expected to be in BCP47 form.
	///
	/// The setting is currently only supported by the speech-dispatcher
	/// backend.
	QString qsTTSLanguage = {};
	int iQuality          = 40000;
	int iMinLoudness      = 1000;
	/// Actual mic hold time is (iVoiceHold / 100) seconds, where iVoiceHold is specified in 'frames',
	/// each of which is has a size of iFrameSize (see AudioInput.h)
	int iVoiceHold                  = 20;
	int iJitterBufferSize           = 1;
	bool bAllowLowDelay             = true;
	NoiseCancel noiseCancelMode     = NoiseCancelSpeex;
	int iSpeexNoiseCancelStrength   = -30;
	quint64 uiAudioInputChannelMask = 0xffffffffffffffffULL;

	// Idle auto actions
	unsigned int iIdleTime           = 5 * 60;
	IdleAction iaeIdleAction         = Nothing;
	bool bUndoIdleActionUponActivity = false;

	VADSource vsVAD                     = Amplitude;
	float fVADmin                       = 0.80f;
	float fVADmax                       = 0.98f;
	int iFramesPerPacket                = 2;
	QString qsAudioInput                = {};
	QString qsAudioOutput               = {};
	float fVolume                       = 1.0f;
	float fOtherVolume                  = 0.5f;
	bool bAttenuateOthersOnTalk         = false;
	bool bAttenuateOthers               = false;
	bool bAttenuateUsersOnPrioritySpeak = false;
	bool bOnlyAttenuateSameOutput       = false;
	bool bAttenuateLoopbacks            = false;
	int iOutputDelay                    = 5;

	QString qsALSAInput        = QStringLiteral("default");
	QString qsALSAOutput       = QStringLiteral("default");
	uint8_t pipeWireInput      = 1;
	uint8_t pipeWireOutput     = 2;
	QString qsPulseAudioInput  = {};
	QString qsPulseAudioOutput = {};
	QString qsJackClientName   = QStringLiteral("mumble");
	QString qsJackAudioOutput  = QStringLiteral("1");
	bool bJackStartServer      = false;
	bool bJackAutoConnect      = true;
	QString qsOSSInput         = {};
	QString qsOSSOutput        = {};
	int iPortAudioInput        = -1; // default device
	int iPortAudioOutput       = -1; // default device

	bool bASIOEnable                = true;
	QString qsASIOclass             = {};
	QList< QVariant > qlASIOmic     = {};
	QList< QVariant > qlASIOspeaker = {};

	QString qsCoreAudioInput  = {};
	QString qsCoreAudioOutput = {};

	QString qsWASAPIInput  = {};
	QString qsWASAPIOutput = {};
	/// qsWASAPIRole is configured via 'wasapi/role'.
	/// It is a string explaining Mumble's purpose for opening
	/// the audio device. This can be used to force Windows
	/// to not treat Mumble as a communications program
	/// (the default).
	///
	/// The default is "communications". When this is set,
	/// Windows treats Mumble as a telephony app, including
	/// potential audio ducking.
	///
	/// Other values include:
	///
	///   "console", which should be used for games, system
	///              notification sounds, and voice commands.
	///
	///   "multimedia", which should be used for music, movies,
	///                 narration, and live music recording.
	///
	/// This is practically a direct mapping of the ERole enum
	/// from Windows: https://msdn.microsoft.com/en-us/library/windows/desktop/dd370842
	QString qsWASAPIRole = {};

	bool bExclusiveInput          = false;
	bool bExclusiveOutput         = false;
	EchoCancelOptionID echoOption = EchoCancelOptionID::SPEEX_MIXED;
	bool bPositionalAudio         = false;
	bool bPositionalHeadphone     = false;
	float fAudioMinDistance       = 1.0f;
	float fAudioMaxDistance       = 15.0f;
	float fAudioMaxDistVolume     = 0.0f;
	float fAudioBloom             = 0.5f;
	/// Contains the settings for each individual plugin. The key in this map is the Hex-represented SHA-1
	/// hash of the plugin's UTF-8 encoded absolute file-path on the hard-drive.
	QHash< QString, PluginSetting > qhPluginSettings = {};

	OverlaySettings os = {};

	int iOverlayWinHelperRestartCooldownMsec = 10000;
	bool bOverlayWinHelperX86Enable          = true;
	bool bOverlayWinHelperX64Enable          = true;

	int iLCDUserViewMinColWidth        = 50;
	int iLCDUserViewSplitterWidth      = 2;
	QMap< QString, bool > qmLCDDevices = {};

	bool bShortcutEnable             = true;
	bool bSuppressMacEventTapWarning = false;
	bool bEnableEvdev                = false;
	bool bEnableXInput2              = true;
	bool bEnableGKey                 = false;
	bool bEnableXboxInput            = true;
	/// Enable use of UIAccess (Windows's UI automation feature). This allows Mumble to receive WM_INPUT messages when
	/// an application with elevated privileges is in foreground.
	bool bEnableUIAccess          = true;
	QList< Shortcut > qlShortcuts = {};

	int iMaxLogBlocks       = 0;
	bool bLog24HourClock    = true;
	int iChatMessageMargins = 3;

	QPoint qpTalkingUI_Position              = UNSPECIFIED_POSITION;
	bool bShowTalkingUI                      = false;
	bool talkingUI_UsersAlwaysVisible        = false;
	bool bTalkingUI_LocalUserStaysVisible    = false;
	bool bTalkingUI_AbbreviateChannelNames   = true;
	bool bTalkingUI_AbbreviateCurrentChannel = false;
	bool bTalkingUI_ShowLocalListeners       = false;
	/// relative font size in %
	int iTalkingUI_RelativeFontSize             = 100;
	int iTalkingUI_SilentUserLifeTime           = 10;
	int iTalkingUI_ChannelHierarchyDepth        = 1;
	int iTalkingUI_MaxChannelNameLength         = 20;
	int iTalkingUI_PrefixCharCount              = 3;
	int iTalkingUI_PostfixCharCount             = 2;
	QString qsTalkingUI_AbbreviationReplacement = QStringLiteral("...");

	QString qsHierarchyChannelSeparator = QStringLiteral("/");

	int manualPlugin_silentUserDisplaytime = 1;

	QMap< int, QString > qmMessageSounds = {};
	QMap< int, quint32 > qmMessages      = {};

	QString qsLanguage = {};

	/// Name of the theme to use. @see Themes
	QString themeName = QStringLiteral("Mumble");
	/// Name of the style to use from theme. @see Themes
	QString themeStyleName = QStringLiteral("Lite");

	QByteArray qbaMainWindowGeometry     = {};
	QByteArray qbaMainWindowState        = {};
	QByteArray qbaMinimalViewGeometry    = {};
	QByteArray qbaMinimalViewState       = {};
	QByteArray qbaConfigGeometry         = {};
	WindowLayout wlWindowLayout          = LayoutClassic;
	ChannelExpand ceExpand               = ChannelsWithUsers;
	ChannelDrag ceChannelDrag            = Ask;
	ChannelDrag ceUserDrag               = Move;
	bool bMinimalView                    = false;
	bool bHideFrame                      = false;
	AlwaysOnTopBehaviour aotbAlwaysOnTop = OnTopNever;
	QuitBehavior quitBehavior            = QuitBehavior::ALWAYS_ASK;
	bool bEnableDeveloperMenu            = false;
	bool bLockLayout                     = false;
	bool bHideInTray                     = false;
	bool bStateInTray                    = true;
	bool bUsage                          = true;
	bool bShowUserCount                  = false;
	bool bShowVolumeAdjustments          = true;
	bool bShowNicknamesOnly              = false;
	bool bChatBarUseSelection            = false;
	bool bFilterHidesEmptyChannels       = true;
	bool bFilterActive                   = false;
	QByteArray qbaConnectDialogHeader    = {};
	QByteArray qbaConnectDialogGeometry  = {};
	bool bShowContextMenuInMenuBar       = false;

	// Search settings
	bool searchForUsers                                     = true;
	bool searchForChannels                                  = true;
	bool searchCaseSensitive                                = false;
	bool searchAsRegex                                      = false;
	bool searchOptionsShown                                 = false;
	Search::SearchDialog::UserAction searchUserAction       = Search::SearchDialog::UserAction::JOIN;
	Search::SearchDialog::ChannelAction searchChannelAction = Search::SearchDialog::ChannelAction::JOIN;
	QPoint searchDialogPosition                             = UNSPECIFIED_POSITION;

	QString qsUsername   = {};
	QString qsLastServer = {};
	ServerShow ssFilter  = ShowReachable;

	QString qsImagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

	bool bUpdateCheck      = true;
	bool bPluginCheck      = true;
	bool bPluginAutoUpdate = false;

	// PTT Button window
	bool bShowPTTButtonWindow             = false;
	QByteArray qbaPTTButtonWindowGeometry = {};

	// Network settings
	bool bTCPCompat   = false;
	bool bReconnect   = true;
	bool bAutoConnect = false;
	bool bQoS         = true;
	/// Disables the "Public Internet" section in the connect dialog if set.
	bool bDisablePublicList    = false;
	ProxyType ptProxyType      = NoProxy;
	QString qsProxyHost        = {};
	QString qsProxyUsername    = {};
	QString qsProxyPassword    = {};
	unsigned short usProxyPort = 0;

	/// The ping interval in milliseconds. The Mumble client
	/// will regularly send TCP and UDP pings to the remote
	/// server. This setting specifies the time (in milliseconds)
	/// between each ping message.
	int iPingIntervalMsec = 5000;

	/// The connection timeout duration in milliseconds.
	/// If a connection is not fully established to the
	/// server within this duration, the client will
	/// forcefully disconnect.
	int iConnectionTimeoutDurationMsec = 30000;

	/// bUdpForceTcpAddr forces Mumble to bind its UDP
	/// socket to the same address as its TCP
	/// connection is using.
	bool bUdpForceTcpAddr = true;

	/// iMaxInFlightTCPPings specifies the maximum
	/// number of ping messages that the client has
	/// sent, but not yet received a response for
	/// from the server. This value is checked when
	/// the client sends its next ping message. If
	/// the maximum is reached, the connection will
	/// be closed.
	/// If this setting is assigned a value of 0 or
	/// a negative number, the TCP ping check is
	/// disabled.
	int iMaxInFlightTCPPings = 4;

	/// The service prefix that the WebFetch class will use
	/// when it constructs its fully-qualified URL. If this
	/// is empty, no prefix is used.
	///
	/// When the WebFetch class receives a HTTP response which
	/// includes the header "Use-Service-Prefix", this setting
	/// is updated to reflect the received service prefix.
	///
	/// For more information, see the documentation for WebFetch::fetch().
	QString qsServicePrefix = {};

	// Network settings - SSL
	QString qsSslCiphers = MumbleSSL::defaultOpenSSLCipherString();

	// Privacy settings
	bool bHideOS = false;

	// Allow 1024x1024 resolution
	int iMaxImageWidth  = 1024;
	int iMaxImageHeight = 1024;

	KeyPair kpCertificate = {};

	bool bShowTransmitModeComboBox = false;

	// Accessibility
	bool bHighContrast = false;

	// Recording
	QString qsRecordingPath       = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	QString qsRecordingFile       = QStringLiteral("Mumble-%date-%time-%host-%user");
	RecordingMode rmRecordingMode = RecordingMixdown;
	int iRecordingFormat          = 0;

	// Special configuration options not exposed to UI

	/// Removes the add and edit options in the connect dialog if set.
	bool disableConnectDialogEditing = false;

	/// Asks the user for consent to ping servers in the public server list if not set.
	bool bPingServersDialogViewed = false;

	/// Whether the audio wizard has been shown to the user yet (at some point during Mumble's installation)
	bool audioWizardShown = false;

	/// Path to SQLite-DB
	QString qsDatabaseLocation = {};

	/// The email address that has been specified most recently in the crash reporter
	QString crashReportEmail = {};

	// Nonsaved
	bool bSuppressIdentity = false;
	LoopMode lmLoopMode    = None;
	float dPacketLoss      = 0.0f;
	float dMaxPacketDelay  = 0.0f;
	/// If true settings in this structure require a client restart to apply fully
	bool requireRestartToApply = false;
	QString settingsLocation   = {};
	/// A flag indicating whether the current Mumble session has already backed up the settings it was started with,
	/// before writing new ones.
	mutable bool createdSettingsBackup = false;

	/// A flag used in order to determine whether or not to offer loading the setting's backup file instead
	bool mumbleQuitNormally = false;

	bool doEcho() const;
	bool doPositionalAudio() const;

	Settings();

	void save(const QString &path) const;
	void save() const;

	void load(const QString &path);
	void load();

	void legacyLoad(const QString &path = {});

	void migratePluginSettings(const MigratedPath &path);

private:
	void verifySettingsKeys() const;
	QString findSettingsLocation(bool legacy = false, bool *foundExistingFile = nullptr) const;
};

#endif // MUMBLE_MUMBLE_SETTINGS_H_
