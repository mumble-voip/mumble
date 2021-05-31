// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Settings.h"

#include "AudioInput.h"
#include "Cert.h"
#include "Log.h"
#include "SSL.h"
#include "Global.h"

#if defined(Q_OS_WIN)
#	include "GlobalShortcut_win.h"
#endif

#include "../../overlay/overlay.h"

#include <QtCore/QFileInfo>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QRegularExpression>
#include <QtCore/QStandardPaths>
#include <QtGui/QImageReader>
#include <QtWidgets/QSystemTrayIcon>
#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#	include <QOperatingSystemVersion>
#endif

#include <boost/typeof/typeof.hpp>

#include <limits>



const QPoint Settings::UNSPECIFIED_POSITION =
	QPoint(std::numeric_limits< int >::min(), std::numeric_limits< int >::max());

bool Shortcut::isServerSpecific() const {
	if (qvData.canConvert< ShortcutTarget >()) {
		const ShortcutTarget &sc = qvariant_cast< ShortcutTarget >(qvData);
		return sc.isServerSpecific();
	}
	return false;
}

bool Shortcut::operator<(const Shortcut &other) const {
	return (iIndex < other.iIndex);
}

bool Shortcut::operator==(const Shortcut &other) const {
	return (iIndex == other.iIndex) && (qlButtons == other.qlButtons) && (qvData == other.qvData)
		   && (bSuppress == other.bSuppress);
}

ShortcutTarget::ShortcutTarget() {
	bUsers            = true;
	bCurrentSelection = false;
	iChannel          = -3;
	bLinks = bChildren = bForceCenter = false;
}

bool ShortcutTarget::isServerSpecific() const {
	return !bCurrentSelection && !bUsers && iChannel >= 0;
}

bool ShortcutTarget::operator==(const ShortcutTarget &o) const {
	if ((bUsers != o.bUsers) || (bForceCenter != o.bForceCenter) || (bCurrentSelection != o.bCurrentSelection))
		return false;
	if (bUsers)
		return (qlUsers == o.qlUsers) && (qlSessions == o.qlSessions);
	else
		return (iChannel == o.iChannel) && (bLinks == o.bLinks) && (bChildren == o.bChildren) && (qsGroup == o.qsGroup);
}

quint32 qHash(const ShortcutTarget &t) {
	quint32 h = t.bForceCenter ? 0x55555555 : 0xaaaaaaaa;

	if (t.bCurrentSelection) {
		h ^= 0x20000000;
	}

	if (t.bUsers) {
		foreach (unsigned int u, t.qlSessions)
			h ^= u;
	} else {
		h ^= t.iChannel;
		if (t.bLinks)
			h ^= 0x80000000;
		if (t.bChildren)
			h ^= 0x40000000;
		h ^= qHash(t.qsGroup);
		h = ~h;
	}
	return h;
}

quint32 qHash(const QList< ShortcutTarget > &l) {
	quint32 h = l.count();
	foreach (const ShortcutTarget &st, l)
		h ^= qHash(st);
	return h;
}

QDataStream &operator<<(QDataStream &qds, const ShortcutTarget &st) {
	// Start by the version of this setting. This is needed to make sure we can stay compatible
	// with older versions (aka don't break existing shortcuts when updating the implementation)
	qds << QString::fromLatin1("v2");

	qds << st.bCurrentSelection << st.bUsers << st.bForceCenter;

	if (st.bCurrentSelection) {
		return qds << st.bLinks << st.bChildren;
	} else if (st.bUsers) {
		return qds << st.qlUsers;
	} else {
		return qds << st.iChannel << st.qsGroup << st.bLinks << st.bChildren;
	}
}

QDataStream &operator>>(QDataStream &qds, ShortcutTarget &st) {
	// Check for presence of a leading version string
	QString versionString;
	QIODevice *device = qds.device();

	if (device) {
		// Qt's way of serializing the stream requires us to read a few characters into
		// the stream in order to get accross some leading zeros and other meta stuff.
		char buf[16];

		// Init buf
		for (unsigned int i = 0; i < sizeof(buf); i++) {
			buf[i] = 0;
		}

		int read = device->peek(buf, sizeof(buf));

		for (int i = 0; i < read; i++) {
			if (buf[i] >= 31) {
				if (buf[i] == 'v') {
					qds >> versionString;
				} else {
					break;
				}
			}
		}
	} else {
		qCritical("Settings: Unable to determine version of setting for ShortcutTarget");
	}

	if (versionString == QLatin1String("v2")) {
		qds >> st.bCurrentSelection;
	}

	qds >> st.bUsers >> st.bForceCenter;

	if (st.bCurrentSelection) {
		return qds >> st.bLinks >> st.bChildren;
	} else if (st.bUsers) {
		return qds >> st.qlUsers;
	} else {
		return qds >> st.iChannel >> st.qsGroup >> st.bLinks >> st.bChildren;
	}
}

const QString Settings::cqsDefaultPushClickOn  = QLatin1String(":/on.ogg");
const QString Settings::cqsDefaultPushClickOff = QLatin1String(":/off.ogg");

const QString Settings::cqsDefaultMuteCue = QLatin1String(":/off.ogg");

OverlaySettings::OverlaySettings() {
	bEnable = false;

	fX    = 1.0f;
	fY    = 0.0f;
	fZoom = 0.875f;

#ifdef Q_OS_MACOS
	qsStyle = QLatin1String("Cleanlooks");
#endif

	osShow       = LinkedChannels;
	bAlwaysSelf  = true;
	uiActiveTime = 5;
	osSort       = Alphabetical;

	qcUserName[Settings::Passive]      = QColor(170, 170, 170);
	qcUserName[Settings::MutedTalking] = QColor(170, 170, 170);
	qcUserName[Settings::Talking]      = QColor(255, 255, 255);
	qcUserName[Settings::Whispering]   = QColor(128, 255, 128);
	qcUserName[Settings::Shouting]     = QColor(255, 128, 255);
	qcChannel                          = QColor(255, 255, 128);
	qcBoxPen                           = QColor(0, 0, 0, 224);
	qcBoxFill                          = QColor(0, 0, 0);

	setPreset();

	// FPS and Time display settings
	qcFps   = Qt::white;
	fFps    = 0.75f;
	qfFps   = qfUserName;
	qrfFps  = QRectF(0.0f, 0.05, -1, 0.023438f);
	bFps    = false;
	qrfTime = QRectF(0.0f, 0.0, -1, 0.023438f);
	bTime   = false;

	oemOverlayExcludeMode = OverlaySettings::LauncherFilterExclusionMode;
}

void OverlaySettings::setPreset(const OverlayPresets preset) {
	switch (preset) {
		case LargeSquareAvatar:
			uiColumns      = 2;
			fUserName      = 0.75f;
			fChannel       = 0.75f;
			fMutedDeafened = 0.5f;
			fAvatar        = 1.0f;

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
			qfUserName = QFont(QLatin1String("Verdana"), 20);
#else
			qfUserName = QFont(QLatin1String("Arial"), 20);
#endif
			qfChannel = qfUserName;

			fUser[Settings::Passive]      = 0.5f;
			fUser[Settings::MutedTalking] = 0.5f;
			fUser[Settings::Talking]      = (7.0f / 8.0f);
			fUser[Settings::Whispering]   = (7.0f / 8.0f);
			fUser[Settings::Shouting]     = (7.0f / 8.0f);

			qrfUserName      = QRectF(-0.0625f, 0.101563f - 0.0625f, 0.125f, 0.023438f);
			qrfChannel       = QRectF(-0.03125f, -0.0625f, 0.09375f, 0.015625f);
			qrfMutedDeafened = QRectF(-0.0625f, -0.0625f, 0.0625f, 0.0625f);
			qrfAvatar        = QRectF(-0.0625f, -0.0625f, 0.125f, 0.125f);

			fBoxPenWidth = (1.f / 256.0f);
			fBoxPad      = (1.f / 256.0f);

			bUserName      = true;
			bChannel       = true;
			bMutedDeafened = true;
			bAvatar        = true;
			bBox           = false;

			qaUserName      = Qt::AlignCenter;
			qaMutedDeafened = Qt::AlignLeft | Qt::AlignTop;
			qaAvatar        = Qt::AlignCenter;
			qaChannel       = Qt::AlignCenter;
			break;
		case AvatarAndName:
		default:
			uiColumns      = 1;
			fUserName      = 1.0f;
			fChannel       = (7.0f / 8.0f);
			fMutedDeafened = (7.0f / 8.0f);
			fAvatar        = 1.0f;

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
			qfUserName = QFont(QLatin1String("Verdana"), 20);
#else
			qfUserName = QFont(QLatin1String("Arial"), 20);
#endif
			qfChannel = qfUserName;

			fUser[Settings::Passive]      = 0.5f;
			fUser[Settings::MutedTalking] = 0.5f;
			fUser[Settings::Talking]      = (7.0f / 8.0f);
			fUser[Settings::Whispering]   = (7.0f / 8.0f);
			fUser[Settings::Shouting]     = (7.0f / 8.0f);

			qrfUserName      = QRectF(0.015625f, -0.015625f, 0.250f, 0.03125f);
			qrfChannel       = QRectF(0.03125f, -0.015625f, 0.1875f, 0.015625f);
			qrfMutedDeafened = QRectF(0.234375f, -0.015625f, 0.03125f, 0.03125f);
			qrfAvatar        = QRectF(-0.03125f, -0.015625f, 0.03125f, 0.03125f);

			fBoxPenWidth = 0.0f;
			fBoxPad      = (1.f / 256.0f);

			bUserName      = true;
			bChannel       = false;
			bMutedDeafened = true;
			bAvatar        = true;
			bBox           = true;

			qaUserName      = Qt::AlignLeft | Qt::AlignVCenter;
			qaMutedDeafened = Qt::AlignRight | Qt::AlignVCenter;
			qaAvatar        = Qt::AlignCenter;
			qaChannel       = Qt::AlignLeft | Qt::AlignTop;
			break;
	}
}

Settings::Settings() {
#if defined(Q_OS_WIN)
	GlobalShortcutWin::registerMetaTypes();
#endif
	qRegisterMetaType< ShortcutTarget >("ShortcutTarget");
	qRegisterMetaTypeStreamOperators< ShortcutTarget >("ShortcutTarget");
	qRegisterMetaType< QVariant >("QVariant");
	qRegisterMetaType< PluginSetting >("PluginSetting");
	qRegisterMetaTypeStreamOperators< PluginSetting >("PluginSetting");
	qRegisterMetaType< Search::SearchDialog::UserAction >("SearchDialog::UserAction");
	qRegisterMetaType< Search::SearchDialog::ChannelAction >("SearchDialog::ChannelAction");

	atTransmit        = VAD;
	bTransmitPosition = false;
	bMute = bDeaf                  = false;
	bTTS                           = false;
	bTTSMessageReadBack            = false;
	bTTSNoScope                    = false;
	bTTSNoAuthor                   = false;
	iTTSVolume                     = 75;
	iTTSThreshold                  = 250;
	qsTTSLanguage                  = QString();
	iQuality                       = 40000;
	fVolume                        = 1.0f;
	fOtherVolume                   = 0.5f;
	bAttenuateOthersOnTalk         = false;
	bAttenuateOthers               = false;
	bAttenuateUsersOnPrioritySpeak = false;
	bOnlyAttenuateSameOutput       = false;
	bAttenuateLoopbacks            = false;
	iMinLoudness                   = 1000;
	iVoiceHold                     = 50;
	iJitterBufferSize              = 1;
	iFramesPerPacket               = 2;
#ifdef USE_RNNOISE
	noiseCancelMode = NoiseCancelRNN;
#else
	noiseCancelMode = NoiseCancelSpeex;
#endif
	iSpeexNoiseCancelStrength = -30;
	bAllowLowDelay            = true;
	uiAudioInputChannelMask   = 0xffffffffffffffffULL;

	// Idle auto actions
	iIdleTime                   = 5 * 60;
	iaeIdleAction               = Nothing;
	bUndoIdleActionUponActivity = false;

	vsVAD   = Amplitude;
	fVADmin = 0.80f;
	fVADmax = 0.98f;

	bTxAudioCue     = false;
	qsTxAudioCueOn  = cqsDefaultPushClickOn;
	qsTxAudioCueOff = cqsDefaultPushClickOff;

	bTxMuteCue  = true;
	qsTxMuteCue = cqsDefaultMuteCue;

	bUserTop = true;

	bWhisperFriends            = false;
	iMessageLimitUserThreshold = 20;

	uiDoublePush = 0;
	pttHold      = 0;

#ifdef NO_UPDATE_CHECK
	bUpdateCheck = false;
	bPluginCheck = false;
#else
	bUpdateCheck = true;
	bPluginCheck = true;
#endif
	bPluginAutoUpdate = false;

	qsImagePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

	ceExpand             = ChannelsWithUsers;
	ceChannelDrag        = Ask;
	ceUserDrag           = Move;
	bMinimalView         = false;
	bHideFrame           = false;
	aotbAlwaysOnTop      = OnTopNever;
	bAskOnQuit           = true;
	bEnableDeveloperMenu = false;
	bLockLayout          = false;
#ifdef Q_OS_WIN
	// Don't enable minimize to tray by default on Windows >= 7
#	if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
	// Since Qt 5.9 QOperatingSystemVersion is preferred over QSysInfo::WinVersion
	bHideInTray = QOperatingSystemVersion::current() < QOperatingSystemVersion::Windows7;
#	else
	const QSysInfo::WinVersion winVer = QSysInfo::windowsVersion();
	bHideInTray                       = (winVer < QSysInfo::WV_WINDOWS7);
#	endif
#else
	const bool isUnityDesktop =
		QProcessEnvironment::systemEnvironment().value(QLatin1String("XDG_CURRENT_DESKTOP")) == QLatin1String("Unity");
	bHideInTray = !isUnityDesktop && QSystemTrayIcon::isSystemTrayAvailable();
#endif
	bStateInTray              = true;
	bUsage                    = true;
	bShowUserCount            = false;
	bShowVolumeAdjustments    = true;
	bShowNicknamesOnly        = false;
	bChatBarUseSelection      = false;
	bFilterHidesEmptyChannels = true;
	bFilterActive             = false;

	wlWindowLayout            = LayoutClassic;
	bShowContextMenuInMenuBar = false;

	ssFilter = ShowReachable;

	iOutputDelay = 5;

	bASIOEnable = true;

	qsALSAInput  = QLatin1String("default");
	qsALSAOutput = QLatin1String("default");

	pipeWireInput  = 1;
	pipeWireOutput = 2;

	qsJackClientName  = QLatin1String("mumble");
	qsJackAudioOutput = QLatin1String("1");
	bJackStartServer  = false;
	bJackAutoConnect  = true;

#ifdef Q_OS_MACOS
	// The echo cancellation feature on macOS is experimental and known to be able to cause problems
	// (e.g. muting the user instead of only cancelling echo - https://github.com/mumble-voip/mumble/issues/4912)
	// Therefore we disable it by default until the issues are fixed.
	echoOption = EchoCancelOptionID::DISABLED;
#else
	// Everywhere else Speex works and thus we default to using that
	echoOption = EchoCancelOptionID::SPEEX_MIXED;
#endif

	bExclusiveInput  = false;
	bExclusiveOutput = false;

	iPortAudioInput  = -1; // default device
	iPortAudioOutput = -1; // default device

	bPositionalAudio     = true;
	bPositionalHeadphone = false;
	fAudioMinDistance    = 1.0f;
	fAudioMaxDistance    = 15.0f;
	fAudioMaxDistVolume  = 0.25f;
	fAudioBloom          = 0.5f;

	// OverlayPrivateWin
	iOverlayWinHelperRestartCooldownMsec = 10000;
	bOverlayWinHelperX86Enable           = true;
	bOverlayWinHelperX64Enable           = true;

	iLCDUserViewMinColWidth   = 50;
	iLCDUserViewSplitterWidth = 2;

	// PTT Button window
	bShowPTTButtonWindow = false;

	// Network settings
	bTCPCompat                     = false;
	bQoS                           = true;
	bReconnect                     = true;
	bAutoConnect                   = false;
	bDisablePublicList             = false;
	ptProxyType                    = NoProxy;
	usProxyPort                    = 0;
	iMaxInFlightTCPPings           = 4;
	bUdpForceTcpAddr               = true;
	iPingIntervalMsec              = 5000;
	iConnectionTimeoutDurationMsec = 30000;
	iMaxImageWidth                 = 1024; // Allow 1024x1024 resolution
	iMaxImageHeight                = 1024;
	bSuppressIdentity              = false;
	qsSslCiphers                   = MumbleSSL::defaultOpenSSLCipherString();
	bHideOS                        = false;

	bShowTransmitModeComboBox = false;

	// Accessibility
	bHighContrast = false;

	// Recording
	qsRecordingPath  = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	qsRecordingFile  = QLatin1String("Mumble-%date-%time-%host-%user");
	rmRecordingMode  = RecordingMixdown;
	iRecordingFormat = 0;

	// Special configuration options not exposed to UI
	bDisableCELT                = false;
	disableConnectDialogEditing = false;
	bPingServersDialogViewed    = false;

	// Config updates
	uiUpdateCounter = 0;

#if defined(AUDIO_TEST)
	lmLoopMode = Server;
#else
	lmLoopMode = None;
#endif
	dPacketLoss     = 0;
	dMaxPacketDelay = 0.0f;

	requireRestartToApply = false;

	iMaxLogBlocks       = 0;
	bLog24HourClock     = true;
	iChatMessageMargins = 3;

	qpTalkingUI_Position                = UNSPECIFIED_POSITION;
	bShowTalkingUI                      = false;
	bTalkingUI_LocalUserStaysVisible    = false;
	bTalkingUI_AbbreviateChannelNames   = true;
	bTalkingUI_AbbreviateCurrentChannel = false;
	bTalkingUI_ShowLocalListeners       = false;
	iTalkingUI_RelativeFontSize         = 100;
	iTalkingUI_SilentUserLifeTime       = 10;
	iTalkingUI_ChannelHierarchyDepth    = 1;
	iTalkingUI_MaxChannelNameLength     = 20;
	iTalkingUI_PrefixCharCount          = 3;
	iTalkingUI_PostfixCharCount         = 2;
	qsTalkingUI_AbbreviationReplacement = QLatin1String("...");

	qsHierarchyChannelSeparator = QLatin1String("/");

	manualPlugin_silentUserDisplaytime = 1;

	bShortcutEnable             = true;
	bSuppressMacEventTapWarning = false;
	bEnableEvdev                = false;
	bEnableXInput2              = true;
	bEnableGKey                 = false;
	bEnableXboxInput            = true;
	bEnableUIAccess             = true;

	for (int i = Log::firstMsgType; i <= Log::lastMsgType; ++i) {
		qmMessages.insert(i,
						  Settings::LogConsole | Settings::LogBalloon | Settings::LogTTS | Settings::LogMessageLimit);
		qmMessageSounds.insert(i, QString());
	}

	qmMessageSounds[Log::CriticalError]          = QLatin1String(":/Critical.ogg");
	qmMessageSounds[Log::PermissionDenied]       = QLatin1String(":/PermissionDenied.ogg");
	qmMessageSounds[Log::SelfMute]               = QLatin1String(":/SelfMutedDeafened.ogg");
	qmMessageSounds[Log::SelfUnmute]             = QLatin1String(":/SelfMutedDeafened.ogg");
	qmMessageSounds[Log::SelfDeaf]               = QLatin1String(":/SelfMutedDeafened.ogg");
	qmMessageSounds[Log::SelfUndeaf]             = QLatin1String(":/SelfMutedDeafened.ogg");
	qmMessageSounds[Log::ServerConnected]        = QLatin1String(":/ServerConnected.ogg");
	qmMessageSounds[Log::ServerDisconnected]     = QLatin1String(":/ServerDisconnected.ogg");
	qmMessageSounds[Log::TextMessage]            = QLatin1String(":/TextMessage.ogg");
	qmMessageSounds[Log::PrivateTextMessage]     = qmMessageSounds[Log::TextMessage];
	qmMessageSounds[Log::ChannelJoin]            = QLatin1String(":/UserJoinedChannel.ogg");
	qmMessageSounds[Log::ChannelLeave]           = QLatin1String(":/UserLeftChannel.ogg");
	qmMessageSounds[Log::ChannelJoinConnect]     = qmMessageSounds[Log::ChannelJoin];
	qmMessageSounds[Log::ChannelLeaveDisconnect] = qmMessageSounds[Log::ChannelLeave];
	qmMessageSounds[Log::YouMutedOther]          = QLatin1String(":/UserMutedYouOrByYou.ogg");
	qmMessageSounds[Log::YouMuted]               = QLatin1String(":/UserMutedYouOrByYou.ogg");
	qmMessageSounds[Log::YouKicked]              = QLatin1String(":/UserKickedYouOrByYou.ogg");
	qmMessageSounds[Log::Recording]              = QLatin1String(":/RecordingStateChanged.ogg");

	qmMessages[Log::DebugInfo]       = Settings::LogConsole;
	qmMessages[Log::Warning]         = Settings::LogConsole | Settings::LogBalloon;
	qmMessages[Log::Information]     = Settings::LogConsole;
	qmMessages[Log::UserJoin]        = Settings::LogConsole;
	qmMessages[Log::UserLeave]       = Settings::LogConsole;
	qmMessages[Log::UserKicked]      = Settings::LogConsole;
	qmMessages[Log::OtherSelfMute]   = Settings::LogConsole;
	qmMessages[Log::OtherMutedOther] = Settings::LogConsole;
	qmMessages[Log::UserRenamed]     = Settings::LogConsole;
	qmMessages[Log::PluginMessage]   = Settings::LogConsole;

	// Default search options
	searchForUsers       = true;
	searchForChannels    = true;
	searchCaseSensitive  = false;
	searchAsRegex        = false;
	searchOptionsShown   = false;
	searchUserAction     = Search::SearchDialog::UserAction::JOIN;
	searchChannelAction  = Search::SearchDialog::ChannelAction::JOIN;
	searchDialogPosition = Settings::UNSPECIFIED_POSITION;

	// Default theme
	themeName      = QLatin1String("Mumble");
	themeStyleName = QLatin1String("Lite");
}

bool Settings::doEcho() const {
	if (AudioInputRegistrar::qmNew) {
		AudioInputRegistrar *air = AudioInputRegistrar::qmNew->value(qsAudioInput);
		if (air) {
			if ((Global::get().s.echoOption != EchoCancelOptionID::DISABLED)
				&& air->canEcho(Global::get().s.echoOption, qsAudioOutput))
				return true;
		}
	}
	return false;
}

bool Settings::doPositionalAudio() const {
	return bPositionalAudio;
}

#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()


BOOST_TYPEOF_REGISTER_TYPE(Qt::Alignment)
BOOST_TYPEOF_REGISTER_TYPE(Settings::AudioTransmit)
BOOST_TYPEOF_REGISTER_TYPE(Settings::VADSource)
BOOST_TYPEOF_REGISTER_TYPE(Settings::LoopMode)
BOOST_TYPEOF_REGISTER_TYPE(Settings::OverlayShow)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ProxyType)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ChannelExpand)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ChannelDrag)
BOOST_TYPEOF_REGISTER_TYPE(Settings::ServerShow)
BOOST_TYPEOF_REGISTER_TYPE(Settings::NoiseCancel)
BOOST_TYPEOF_REGISTER_TYPE(Settings::WindowLayout)
BOOST_TYPEOF_REGISTER_TYPE(Settings::AlwaysOnTopBehaviour)
BOOST_TYPEOF_REGISTER_TYPE(Settings::RecordingMode)
BOOST_TYPEOF_REGISTER_TYPE(QString)
BOOST_TYPEOF_REGISTER_TYPE(QByteArray)
BOOST_TYPEOF_REGISTER_TYPE(QColor)
BOOST_TYPEOF_REGISTER_TYPE(QVariant)
BOOST_TYPEOF_REGISTER_TYPE(QFont)
BOOST_TYPEOF_REGISTER_TYPE(EchoCancelOptionID)
BOOST_TYPEOF_REGISTER_TEMPLATE(QList, 1)

#define LOAD(var, name) var = qvariant_cast< BOOST_TYPEOF(var) >(settings_ptr->value(QLatin1String(name), var))
#define LOADENUM(var, name) \
	var = static_cast< BOOST_TYPEOF(var) >(settings_ptr->value(QLatin1String(name), var).toInt())
#define LOADFLAG(var, name) \
	var = static_cast< BOOST_TYPEOF(var) >(settings_ptr->value(QLatin1String(name), static_cast< int >(var)).toInt())
#define DEPRECATED(name) \
	do {                 \
	} while (0)

// Workaround for mumble-voip/mumble#2638.
//
// Qt previously expected to be able to write
// NUL bytes in strings in plists. This is no
// longer possible, which causes Qt to write
// incomplete stings to the preferences plist.
// These are of the form "@Variant(", and, for
// Mumble, typically happen for float values.
//
// We detect this bad value and avoid loading
// it. This causes such settings to fall back
// to their defaults, instead of being set to
// a zero value.
#ifdef Q_OS_MACOS
#	undef LOAD
#	define LOAD(var, name)                                                                              \
		do {                                                                                             \
			if (settings_ptr->value(QLatin1String(name)).toString() != QLatin1String("@Variant(")) {     \
				var = qvariant_cast< BOOST_TYPEOF(var) >(settings_ptr->value(QLatin1String(name), var)); \
			}                                                                                            \
		} while (0)
#endif

void OverlaySettings::load() {
	load(Global::get().qs);
}

void OverlaySettings::load(QSettings *settings_ptr) {
	LOAD(bEnable, "enable");

	LOADENUM(osShow, "show");
	LOAD(bAlwaysSelf, "alwaysself");
	LOAD(uiActiveTime, "activetime");
	LOADENUM(osSort, "sort");

	LOAD(fX, "x");
	LOAD(fY, "y");
	LOAD(fZoom, "zoom");
	LOAD(uiColumns, "columns");

	settings_ptr->beginReadArray(QLatin1String("states"));
	for (int i = 0; i < 4; ++i) {
		settings_ptr->setArrayIndex(i);
		LOAD(qcUserName[i], "color");
		LOAD(fUser[i], "opacity");
	}
	settings_ptr->endArray();

	LOAD(qfUserName, "userfont");
	LOAD(qfChannel, "channelfont");
	LOAD(qcChannel, "channelcolor");
	LOAD(qfFps, "fpsfont");
	LOAD(qcFps, "fpscolor");

	LOAD(fBoxPad, "padding");
	LOAD(fBoxPenWidth, "penwidth");
	LOAD(qcBoxPen, "pencolor");
	LOAD(qcBoxFill, "fillcolor");

	LOAD(bUserName, "usershow");
	LOAD(bChannel, "channelshow");
	LOAD(bMutedDeafened, "mutedshow");
	LOAD(bAvatar, "avatarshow");
	LOAD(bBox, "boxshow");
	LOAD(bFps, "fpsshow");
	LOAD(bTime, "timeshow");

	LOAD(fUserName, "useropacity");
	LOAD(fChannel, "channelopacity");
	LOAD(fMutedDeafened, "mutedopacity");
	LOAD(fAvatar, "avataropacity");
	LOAD(fFps, "fpsopacity");

	LOAD(qrfUserName, "userrect");
	LOAD(qrfChannel, "channelrect");
	LOAD(qrfMutedDeafened, "mutedrect");
	LOAD(qrfAvatar, "avatarrect");
	LOAD(qrfFps, "fpsrect");
	LOAD(qrfTime, "timerect");

	LOADFLAG(qaUserName, "useralign");
	LOADFLAG(qaChannel, "channelalign");
	LOADFLAG(qaMutedDeafened, "mutedalign");
	LOADFLAG(qaAvatar, "avataralign");

	LOADENUM(oemOverlayExcludeMode, "mode");
	LOAD(qslLaunchers, "launchers");
	LOAD(qslLaunchersExclude, "launchersexclude");
	LOAD(qslWhitelist, "whitelist");
	LOAD(qslWhitelistExclude, "whitelistexclude");
	LOAD(qslPaths, "paths");
	LOAD(qslPathsExclude, "pathsexclude");
	LOAD(qslBlacklist, "blacklist");
	LOAD(qslBlacklistExclude, "blacklistexclude");
}

void Settings::load() {
	load(Global::get().qs);
}

void Settings::load(QSettings *settings_ptr) {
	// Config updates
	LOAD(uiUpdateCounter, "lastupdate");

	LOAD(qsDatabaseLocation, "databaselocation");

	LOAD(bMute, "audio/mute");
	LOAD(bDeaf, "audio/deaf");
	LOADENUM(atTransmit, "audio/transmit");
	LOAD(uiDoublePush, "audio/doublepush");
	LOAD(pttHold, "audio/ptthold");
	LOAD(bTxAudioCue, "audio/pushclick");
	LOAD(qsTxAudioCueOn, "audio/pushclickon");
	LOAD(qsTxAudioCueOff, "audio/pushclickoff");
	LOAD(bTxMuteCue, "audio/mutecue");
	LOAD(qsTxMuteCue, "audio/mutecuepath");
	LOAD(iQuality, "audio/quality");
	LOAD(iMinLoudness, "audio/loudness");
	LOAD(fVolume, "audio/volume");
	LOAD(fOtherVolume, "audio/othervolume");
	LOAD(bAttenuateOthers, "audio/attenuateothers");
	LOAD(bAttenuateOthersOnTalk, "audio/attenuateothersontalk");
	LOAD(bAttenuateUsersOnPrioritySpeak, "audio/attenuateusersonpriorityspeak");
	LOAD(bOnlyAttenuateSameOutput, "audio/onlyattenuatesameoutput");
	LOAD(bAttenuateLoopbacks, "audio/attenuateloopbacks");
	LOADENUM(vsVAD, "audio/vadsource");
	LOAD(fVADmin, "audio/vadmin");
	LOAD(fVADmax, "audio/vadmax");

	int oldNoiseSuppress = 0;
	LOAD(oldNoiseSuppress, "audio/noisesupress");
	LOAD(iSpeexNoiseCancelStrength, "audio/speexNoiseCancelStrength");

	// Select the most negative of the 2 (one is expected to be zero as it is
	// unset). This is for compatibility as we have renamed the setting at some point.
	iSpeexNoiseCancelStrength = std::min(oldNoiseSuppress, iSpeexNoiseCancelStrength);

	LOADENUM(noiseCancelMode, "audio/noiseCancelMode");

#ifndef USE_RNNOISE
	if (noiseCancelMode == NoiseCancelRNN || noiseCancelMode == NoiseCancelBoth) {
		// Use Speex instead as this Mumble build was built without support for RNNoise
		noiseCancelMode = NoiseCancelSpeex;
	}
#endif

	LOAD(bAllowLowDelay, "audio/allowlowdelay");
	LOAD(uiAudioInputChannelMask, "audio/inputchannelmask");
	LOAD(iVoiceHold, "audio/voicehold");
	LOAD(iOutputDelay, "audio/outputdelay");

	// Idle auto actions
	LOAD(iIdleTime, "audio/idletime");
	LOADENUM(iaeIdleAction, "audio/idleaction");
	LOAD(bUndoIdleActionUponActivity, "audio/undoidleactionuponactivity");

	LOAD(fAudioMinDistance, "audio/mindistance");
	LOAD(fAudioMaxDistance, "audio/maxdistance");
	LOAD(fAudioMaxDistVolume, "audio/maxdistancevolume");
	LOAD(fAudioBloom, "audio/bloom");
	DEPRECATED("audio/echo");
	DEPRECATED("audio/echomulti");
	LOAD(bExclusiveInput, "audio/exclusiveinput");
	LOAD(bExclusiveOutput, "audio/exclusiveoutput");
	LOAD(bPositionalAudio, "audio/positional");
	LOAD(bPositionalHeadphone, "audio/headphone");
	LOAD(qsAudioInput, "audio/input");
	LOAD(qsAudioOutput, "audio/output");
	LOAD(bWhisperFriends, "audio/whisperfriends");
	LOAD(iMessageLimitUserThreshold, "audio/messagelimitusers");
	LOAD(bTransmitPosition, "audio/postransmit");

	if (settings_ptr->contains("audio/echooptionid")) {
		// Load the new echo cancel option instead
		LOADFLAG(echoOption, "audio/echooptionid");
	} else {
#ifndef Q_OS_MACOS
		// Compatibility layer for overtaking the old (now deprecated) settings
		// This block should only be called once at the first start of the new Mumble version
		// As echo cancellation was not available on macOS before, we don't have to run this compatibility
		// code on macOS (instead simply use the new default as set in the constructor).
		if (settings_ptr->contains("audio/echo")) {
			bool deprecatedEcho      = false;
			bool deprecatedEchoMulti = false;

			LOAD(deprecatedEcho, "audio/echo");
			LOAD(deprecatedEchoMulti, "audio/echomulti");

			if (deprecatedEcho) {
				if (deprecatedEchoMulti) {
					echoOption = EchoCancelOptionID::SPEEX_MULTICHANNEL;
				} else {
					echoOption = EchoCancelOptionID::SPEEX_MIXED;
				}
			} else {
				echoOption = EchoCancelOptionID::DISABLED;
			}
		}
#endif
	}

	LOAD(iJitterBufferSize, "net/jitterbuffer");
	LOAD(iFramesPerPacket, "net/framesperpacket");

	LOAD(bASIOEnable, "asio/enable");
	LOAD(qsASIOclass, "asio/class");
	LOAD(qlASIOmic, "asio/mic");
	LOAD(qlASIOspeaker, "asio/speaker");

	LOAD(qsWASAPIInput, "wasapi/input");
	LOAD(qsWASAPIOutput, "wasapi/output");
	LOAD(qsWASAPIRole, "wasapi/role");

	LOAD(qsALSAInput, "alsa/input");
	LOAD(qsALSAOutput, "alsa/output");

	LOAD(pipeWireInput, "pipewire/input");
	LOAD(pipeWireOutput, "pipewire/output");

	LOAD(qsPulseAudioInput, "pulseaudio/input");
	LOAD(qsPulseAudioOutput, "pulseaudio/output");

	LOAD(qsJackAudioOutput, "jack/output");
	LOAD(bJackStartServer, "jack/startserver");
	LOAD(bJackAutoConnect, "jack/autoconnect");

	LOAD(qsOSSInput, "oss/input");
	LOAD(qsOSSOutput, "oss/output");

	LOAD(qsCoreAudioInput, "coreaudio/input");
	LOAD(qsCoreAudioOutput, "coreaudio/output");

	LOAD(iPortAudioInput, "portaudio/input");
	LOAD(iPortAudioOutput, "portaudio/output");

	LOAD(bTTS, "tts/enable");
	LOAD(iTTSVolume, "tts/volume");
	LOAD(iTTSThreshold, "tts/threshold");
	LOAD(bTTSMessageReadBack, "tts/readback");
	LOAD(bTTSNoScope, "tts/noscope");
	LOAD(bTTSNoAuthor, "tts/noauthor");
	LOAD(qsTTSLanguage, "tts/language");

	// Network settings
	LOAD(bTCPCompat, "net/tcponly");
	LOAD(bQoS, "net/qos");
	LOAD(bReconnect, "net/reconnect");
	LOAD(bAutoConnect, "net/autoconnect");
	LOAD(bSuppressIdentity, "net/suppress");
	LOADENUM(ptProxyType, "net/proxytype");
	LOAD(qsProxyHost, "net/proxyhost");
	LOAD(usProxyPort, "net/proxyport");
	LOAD(qsProxyUsername, "net/proxyusername");
	LOAD(qsProxyPassword, "net/proxypassword");
	DEPRECATED("net/maximagesize");
	LOAD(iMaxImageWidth, "net/maximagewidth");
	LOAD(iMaxImageHeight, "net/maximageheight");
	LOAD(qsServicePrefix, "net/serviceprefix");
	LOAD(iMaxInFlightTCPPings, "net/maxinflighttcppings");
	LOAD(iPingIntervalMsec, "net/pingintervalmsec");
	LOAD(iConnectionTimeoutDurationMsec, "net/connectiontimeoutdurationmsec");
	LOAD(bUdpForceTcpAddr, "net/udpforcetcpaddr");

	// Network settings - SSL
	LOAD(qsSslCiphers, "net/sslciphers");

	// Privacy settings
	LOAD(bHideOS, "privacy/hideos");

	LOAD(qsLanguage, "ui/language");
	LOAD(themeName, "ui/theme");
	LOAD(themeStyleName, "ui/themestyle");
	LOADENUM(ceExpand, "ui/expand");
	LOADENUM(ceChannelDrag, "ui/drag");
	LOADENUM(ceUserDrag, "ui/userdrag");
	LOADENUM(aotbAlwaysOnTop, "ui/alwaysontop");
	LOAD(bAskOnQuit, "ui/askonquit");
	LOAD(bEnableDeveloperMenu, "ui/developermenu");
	LOAD(bLockLayout, "ui/locklayout");
	LOAD(bMinimalView, "ui/minimalview");
	LOAD(bHideFrame, "ui/hideframe");
	LOAD(bUserTop, "ui/usertop");
	LOAD(qbaMainWindowGeometry, "ui/geometry");
	LOAD(qbaMainWindowState, "ui/state");
	LOAD(qbaMinimalViewGeometry, "ui/minimalviewgeometry");
	LOAD(qbaMinimalViewState, "ui/minimalviewstate");
	LOAD(qbaConfigGeometry, "ui/ConfigGeometry");
	LOADENUM(wlWindowLayout, "ui/WindowLayout");
	LOAD(qbaSplitterState, "ui/splitter");
	LOAD(qbaHeaderState, "ui/header");
	LOAD(qsUsername, "ui/username");
	LOAD(qsLastServer, "ui/server");
	LOADENUM(ssFilter, "ui/serverfilter");

	LOAD(bUpdateCheck, "ui/updatecheck");
	LOAD(bPluginCheck, "ui/plugincheck");
	LOAD(bPluginAutoUpdate, "ui/pluginAutoUpdate");

	LOAD(bHideInTray, "ui/hidetray");
	LOAD(bStateInTray, "ui/stateintray");
	LOAD(bUsage, "ui/usage");
	LOAD(bShowUserCount, "ui/showusercount");
	LOAD(bShowVolumeAdjustments, "ui/showVolumeAdjustments");
	LOAD(bShowNicknamesOnly, "ui/showNicknamesOnly");
	LOAD(bChatBarUseSelection, "ui/chatbaruseselection");
	LOAD(bFilterHidesEmptyChannels, "ui/filterhidesemptychannels");
	LOAD(bFilterActive, "ui/filteractive");
	LOAD(qsImagePath, "ui/imagepath");
	LOAD(bShowContextMenuInMenuBar, "ui/showcontextmenuinmenubar");
	LOAD(qbaConnectDialogGeometry, "ui/connect/geometry");
	LOAD(qbaConnectDialogHeader, "ui/connect/header");
	LOAD(bShowTransmitModeComboBox, "ui/transmitmodecombobox");
	LOAD(bHighContrast, "ui/HighContrast");
	LOAD(iMaxLogBlocks, "ui/MaxLogBlocks");
	LOAD(bLog24HourClock, "ui/24HourClock");
	LOAD(iChatMessageMargins, "ui/ChatMessageMargins");
	LOAD(bDisablePublicList, "ui/disablepubliclist");

	// TalkingUI
	LOAD(qpTalkingUI_Position, "ui/talkingUIPosition");
	LOAD(bShowTalkingUI, "ui/showTalkingUI");
	LOAD(bTalkingUI_LocalUserStaysVisible, "ui/talkingUI_LocalUserStaysVisible");
	LOAD(bTalkingUI_AbbreviateChannelNames, "ui/talkingUI_AbbreviateChannelNames");
	LOAD(bTalkingUI_AbbreviateCurrentChannel, "ui/talkingUI_AbbreviateCurrentChannel");
	LOAD(bTalkingUI_ShowLocalListeners, "ui/talkingUI_ShowLocalListeners");
	LOAD(iTalkingUI_RelativeFontSize, "ui/talkingUI_RelativeFontSize");
	LOAD(iTalkingUI_SilentUserLifeTime, "ui/talkingUI_SilentUserLifeTime");
	LOAD(iTalkingUI_ChannelHierarchyDepth, "ui/talkingUI_ChannelHierarchieDepth");
	LOAD(iTalkingUI_MaxChannelNameLength, "ui/talkingUI_MaxChannelNameLength");
	LOAD(iTalkingUI_PrefixCharCount, "ui/talkingUI_PrefixCharCount");
	LOAD(iTalkingUI_PostfixCharCount, "ui/talkingUI_PostfixCharCount");
	LOAD(qsTalkingUI_AbbreviationReplacement, "ui/talkingUI_AbbreviationReplacement");

	// Load the old setting first in case it is set and then load the actual setting
	LOAD(qsHierarchyChannelSeparator, "ui/talkingUI_ChannelSeparator");
	LOAD(qsHierarchyChannelSeparator, "ui/hierarchy_channelSeparator");

	LOAD(manualPlugin_silentUserDisplaytime, "ui/manualPlugin_silentUserDisplaytime");

	// PTT Button window
	LOAD(bShowPTTButtonWindow, "ui/showpttbuttonwindow");
	LOAD(qbaPTTButtonWindowGeometry, "ui/pttbuttonwindowgeometry");

	// Recording
	LOAD(qsRecordingPath, "recording/path");
	LOAD(qsRecordingFile, "recording/file");
	LOADENUM(rmRecordingMode, "recording/mode");
	LOAD(iRecordingFormat, "recording/format");

	// Special configuration options not exposed to UI
	LOAD(bDisableCELT, "audio/disablecelt");
	LOAD(disableConnectDialogEditing, "ui/disableconnectdialogediting");
	LOAD(bPingServersDialogViewed, "consent/pingserversdialogviewed");

	// OverlayPrivateWin
	LOAD(iOverlayWinHelperRestartCooldownMsec, "overlay_win/helper/restart_cooldown_msec");
	LOAD(bOverlayWinHelperX86Enable, "overlay_win/helper/x86/enable");
	LOAD(bOverlayWinHelperX64Enable, "overlay_win/helper/x64/enable");

	// LCD
	LOAD(iLCDUserViewMinColWidth, "lcd/userview/mincolwidth");
	LOAD(iLCDUserViewSplitterWidth, "lcd/userview/splitterwidth");

	QByteArray qba = qvariant_cast< QByteArray >(settings_ptr->value(QLatin1String("net/certificate")));
	if (!qba.isEmpty())
		kpCertificate = CertWizard::importCert(qba);

	LOAD(bShortcutEnable, "shortcut/enable");
	LOAD(bSuppressMacEventTapWarning, "shortcut/mac/suppresswarning");
	LOAD(bEnableEvdev, "shortcut/linux/evdev/enable");
	LOAD(bEnableXInput2, "shortcut/x11/xinput2/enable");
	LOAD(bEnableGKey, "shortcut/gkey");
	LOAD(bEnableXboxInput, "shortcut/windows/xbox/enable");
	LOAD(bEnableUIAccess, "shortcut/windows/uiaccess/enable");

	// Search options
	LOAD(searchForUsers, "search/search_for_users");
	LOAD(searchForChannels, "search/search_for_channels");
	LOAD(searchCaseSensitive, "search/search_case_sensitive");
	LOAD(searchAsRegex, "search/search_as_regex");
	LOAD(searchOptionsShown, "search/search_options_shown");
	LOADFLAG(searchUserAction, "search/search_user_action");
	LOADFLAG(searchChannelAction, "search/search_channel_action");
	LOAD(searchDialogPosition, "search/search_dialog_position");

	int nshorts = settings_ptr->beginReadArray(QLatin1String("shortcuts"));
	for (int i = 0; i < nshorts; i++) {
		settings_ptr->setArrayIndex(i);
		Shortcut s;

		s.iIndex = -2;

		LOAD(s.iIndex, "index");
		LOAD(s.qlButtons, "keys");
		LOAD(s.bSuppress, "suppress");
		s.qvData = settings_ptr->value(QLatin1String("data"));
		if (s.iIndex >= -1)
			qlShortcuts << s;
	}
	settings_ptr->endArray();

	settings_ptr->beginReadArray(QLatin1String("messages"));
	for (QMap< int, quint32 >::const_iterator it = qmMessages.constBegin(); it != qmMessages.constEnd(); ++it) {
		settings_ptr->setArrayIndex(it.key());
		LOAD(qmMessages[it.key()], "log");
	}
	settings_ptr->endArray();

	settings_ptr->beginReadArray(QLatin1String("messagesounds"));
	for (QMap< int, QString >::const_iterator it = qmMessageSounds.constBegin(); it != qmMessageSounds.constEnd();
		 ++it) {
		settings_ptr->setArrayIndex(it.key());
		LOAD(qmMessageSounds[it.key()], "logsound");
	}
	settings_ptr->endArray();

	settings_ptr->beginGroup(QLatin1String("lcd/devices"));
	foreach (const QString &d, settings_ptr->childKeys()) {
		qmLCDDevices.insert(d, settings_ptr->value(d, true).toBool());
	}
	settings_ptr->endGroup();

	// Plugins
	settings_ptr->beginGroup(QLatin1String("plugins"));
	foreach (const QString &pluginKey, settings_ptr->childGroups()) {
		QString pluginHash;

		if (pluginKey.contains(QLatin1String("_"))) {
			// The key contains the filename as well as the hash
			int index  = pluginKey.lastIndexOf(QLatin1String("_"));
			pluginHash = pluginKey.right(pluginKey.size() - index - 1);
		} else {
			pluginHash = pluginKey;
		}

		PluginSetting pluginSettings;
		pluginSettings.path = settings_ptr->value(pluginKey + QLatin1String("/path")).toString();
		pluginSettings.allowKeyboardMonitoring =
			settings_ptr->value(pluginKey + QLatin1String("/allowKeyboardMonitoring")).toBool();
		pluginSettings.enabled = settings_ptr->value(pluginKey + QLatin1String("/enabled")).toBool();
		pluginSettings.positionalDataEnabled =
			settings_ptr->value(pluginKey + QLatin1String("/positionalDataEnabled")).toBool();

		qhPluginSettings.insert(pluginHash, pluginSettings);
	}
	settings_ptr->endGroup();

	settings_ptr->beginGroup(QLatin1String("overlay"));
	os.load(settings_ptr);
	settings_ptr->endGroup();
}

#undef LOAD
#define SAVE(var, name)                                   \
	if (var != def.var)                                   \
		settings_ptr->setValue(QLatin1String(name), var); \
	else                                                  \
		settings_ptr->remove(QLatin1String(name))
#define SAVEFLAG(var, name)                                                   \
	if (var != def.var)                                                       \
		settings_ptr->setValue(QLatin1String(name), static_cast< int >(var)); \
	else                                                                      \
		settings_ptr->remove(QLatin1String(name))
#undef DEPRECATED
#define DEPRECATED(name) settings_ptr->remove(QLatin1String(name))

void OverlaySettings::save() {
	save(Global::get().qs);
}

void OverlaySettings::save(QSettings *settings_ptr) {
	OverlaySettings def;

	settings_ptr->setValue(QLatin1String("version"), QLatin1String(MUMTEXT(MUMBLE_VERSION)));
	settings_ptr->sync();

#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
	if (settings_ptr->format() == QSettings::IniFormat)
#endif
	{
		QFile f(settings_ptr->fileName());
		f.setPermissions(f.permissions()
						 & ~(QFile::ReadGroup | QFile::WriteGroup | QFile::ExeGroup | QFile::ReadOther
							 | QFile::WriteOther | QFile::ExeOther));
	}

	SAVE(bEnable, "enable");

	SAVE(osShow, "show");
	SAVE(bAlwaysSelf, "alwaysself");
	SAVE(uiActiveTime, "activetime");
	SAVE(osSort, "sort");
	SAVE(fX, "x");
	SAVE(fY, "y");
	SAVE(fZoom, "zoom");
	SAVE(uiColumns, "columns");

	settings_ptr->beginReadArray(QLatin1String("states"));
	for (int i = 0; i < 4; ++i) {
		settings_ptr->setArrayIndex(i);
		SAVE(qcUserName[i], "color");
		SAVE(fUser[i], "opacity");
	}
	settings_ptr->endArray();

	SAVE(qfUserName, "userfont");
	SAVE(qfChannel, "channelfont");
	SAVE(qcChannel, "channelcolor");
	SAVE(qfFps, "fpsfont");
	SAVE(qcFps, "fpscolor");

	SAVE(fBoxPad, "padding");
	SAVE(fBoxPenWidth, "penwidth");
	SAVE(qcBoxPen, "pencolor");
	SAVE(qcBoxFill, "fillcolor");

	SAVE(bUserName, "usershow");
	SAVE(bChannel, "channelshow");
	SAVE(bMutedDeafened, "mutedshow");
	SAVE(bAvatar, "avatarshow");
	SAVE(bBox, "boxshow");
	SAVE(bFps, "fpsshow");
	SAVE(bTime, "timeshow");

	SAVE(fUserName, "useropacity");
	SAVE(fChannel, "channelopacity");
	SAVE(fMutedDeafened, "mutedopacity");
	SAVE(fAvatar, "avataropacity");
	SAVE(fFps, "fpsopacity");

	SAVE(qrfUserName, "userrect");
	SAVE(qrfChannel, "channelrect");
	SAVE(qrfMutedDeafened, "mutedrect");
	SAVE(qrfAvatar, "avatarrect");
	SAVE(qrfFps, "fpsrect");
	SAVE(qrfTime, "timerect");

	SAVEFLAG(qaUserName, "useralign");
	SAVEFLAG(qaChannel, "channelalign");
	SAVEFLAG(qaMutedDeafened, "mutedalign");
	SAVEFLAG(qaAvatar, "avataralign");

	SAVE(oemOverlayExcludeMode, "mode");
	settings_ptr->setValue(QLatin1String("launchers"), qslLaunchers);
	settings_ptr->setValue(QLatin1String("launchersexclude"), qslLaunchersExclude);
	settings_ptr->setValue(QLatin1String("whitelist"), qslWhitelist);
	settings_ptr->setValue(QLatin1String("whitelistexclude"), qslWhitelistExclude);
	settings_ptr->setValue(QLatin1String("paths"), qslPaths);
	settings_ptr->setValue(QLatin1String("pathsexclude"), qslPathsExclude);
	settings_ptr->setValue(QLatin1String("blacklist"), qslBlacklist);
	settings_ptr->setValue(QLatin1String("blacklistexclude"), qslBlacklistExclude);
}

void Settings::save() {
	QSettings *settings_ptr = Global::get().qs;
	Settings def;

	// Config updates
	SAVE(uiUpdateCounter, "lastupdate");

	SAVE(qsDatabaseLocation, "databaselocation");

	SAVE(bMute, "audio/mute");
	SAVE(bDeaf, "audio/deaf");
	SAVE(atTransmit, "audio/transmit");
	SAVE(uiDoublePush, "audio/doublepush");
	SAVE(pttHold, "audio/ptthold");
	SAVE(bTxAudioCue, "audio/pushclick");
	SAVE(qsTxAudioCueOn, "audio/pushclickon");
	SAVE(qsTxAudioCueOff, "audio/pushclickoff");
	SAVE(bTxMuteCue, "audio/mutecue");
	SAVE(qsTxMuteCue, "audio/mutecuepath");
	SAVE(iQuality, "audio/quality");
	SAVE(iMinLoudness, "audio/loudness");
	SAVE(fVolume, "audio/volume");
	SAVE(fOtherVolume, "audio/othervolume");
	SAVE(bAttenuateOthers, "audio/attenuateothers");
	SAVE(bAttenuateOthersOnTalk, "audio/attenuateothersontalk");
	SAVE(bAttenuateUsersOnPrioritySpeak, "audio/attenuateusersonpriorityspeak");
	SAVE(bOnlyAttenuateSameOutput, "audio/onlyattenuatesameoutput");
	SAVE(bAttenuateLoopbacks, "audio/attenuateloopbacks");
	SAVE(vsVAD, "audio/vadsource");
	SAVE(fVADmin, "audio/vadmin");
	SAVE(fVADmax, "audio/vadmax");
	SAVE(noiseCancelMode, "audio/noiseCancelMode");
	SAVE(iSpeexNoiseCancelStrength, "audio/speexNoiseCancelStrength");
	SAVE(bAllowLowDelay, "audio/allowlowdelay");
	SAVE(uiAudioInputChannelMask, "audio/inputchannelmask");
	SAVE(iVoiceHold, "audio/voicehold");
	SAVE(iOutputDelay, "audio/outputdelay");

	// Idle auto actions
	SAVE(iIdleTime, "audio/idletime");
	SAVE(iaeIdleAction, "audio/idleaction");
	SAVE(bUndoIdleActionUponActivity, "audio/undoidleactionuponactivity");

	SAVE(fAudioMinDistance, "audio/mindistance");
	SAVE(fAudioMaxDistance, "audio/maxdistance");
	SAVE(fAudioMaxDistVolume, "audio/maxdistancevolume");
	SAVE(fAudioBloom, "audio/bloom");
	DEPRECATED("audio/echo");
	DEPRECATED("audio/echomulti");
	SAVE(bExclusiveInput, "audio/exclusiveinput");
	SAVE(bExclusiveOutput, "audio/exclusiveoutput");
	SAVE(bPositionalAudio, "audio/positional");
	SAVE(bPositionalHeadphone, "audio/headphone");
	SAVE(qsAudioInput, "audio/input");
	SAVE(qsAudioOutput, "audio/output");
	SAVE(bWhisperFriends, "audio/whisperfriends");
	SAVE(iMessageLimitUserThreshold, "audio/messagelimitusers");
	SAVE(bTransmitPosition, "audio/postransmit");
	SAVEFLAG(echoOption, "audio/echooptionid");

	SAVE(iJitterBufferSize, "net/jitterbuffer");
	SAVE(iFramesPerPacket, "net/framesperpacket");

	SAVE(bASIOEnable, "asio/enable");
	SAVE(qsASIOclass, "asio/class");
	SAVE(qlASIOmic, "asio/mic");
	SAVE(qlASIOspeaker, "asio/speaker");

	SAVE(qsWASAPIInput, "wasapi/input");
	SAVE(qsWASAPIOutput, "wasapi/output");
	SAVE(qsWASAPIRole, "wasapi/role");

	SAVE(qsALSAInput, "alsa/input");
	SAVE(qsALSAOutput, "alsa/output");

	SAVE(pipeWireInput, "pipewire/input");
	SAVE(pipeWireOutput, "pipewire/output");

	SAVE(qsPulseAudioInput, "pulseaudio/input");
	SAVE(qsPulseAudioOutput, "pulseaudio/output");

	SAVE(qsJackAudioOutput, "jack/output");
	SAVE(bJackStartServer, "jack/startserver");
	SAVE(bJackAutoConnect, "jack/autoconnect");

	SAVE(qsOSSInput, "oss/input");
	SAVE(qsOSSOutput, "oss/output");

	SAVE(qsCoreAudioInput, "coreaudio/input");
	SAVE(qsCoreAudioOutput, "coreaudio/output");

	SAVE(iPortAudioInput, "portaudio/input");
	SAVE(iPortAudioOutput, "portaudio/output");

	SAVE(bTTS, "tts/enable");
	SAVE(iTTSVolume, "tts/volume");
	SAVE(iTTSThreshold, "tts/threshold");
	SAVE(bTTSMessageReadBack, "tts/readback");
	SAVE(bTTSNoScope, "tts/noscope");
	SAVE(bTTSNoAuthor, "tts/noauthor");
	SAVE(qsTTSLanguage, "tts/language");

	// Network settings
	SAVE(bTCPCompat, "net/tcponly");
	SAVE(bQoS, "net/qos");
	SAVE(bReconnect, "net/reconnect");
	SAVE(bAutoConnect, "net/autoconnect");
	SAVE(ptProxyType, "net/proxytype");
	SAVE(qsProxyHost, "net/proxyhost");
	SAVE(usProxyPort, "net/proxyport");
	SAVE(qsProxyUsername, "net/proxyusername");
	SAVE(qsProxyPassword, "net/proxypassword");
	DEPRECATED("net/maximagesize");
	SAVE(iMaxImageWidth, "net/maximagewidth");
	SAVE(iMaxImageHeight, "net/maximageheight");
	SAVE(qsServicePrefix, "net/serviceprefix");
	SAVE(iMaxInFlightTCPPings, "net/maxinflighttcppings");
	SAVE(iPingIntervalMsec, "net/pingintervalmsec");
	SAVE(iConnectionTimeoutDurationMsec, "net/connectiontimeoutdurationmsec");
	SAVE(bUdpForceTcpAddr, "net/udpforcetcpaddr");

	// Network settings - SSL
	SAVE(qsSslCiphers, "net/sslciphers");

	// Privacy settings
	SAVE(bHideOS, "privacy/hideos");

	SAVE(qsLanguage, "ui/language");
	SAVE(themeName, "ui/theme");
	SAVE(themeStyleName, "ui/themestyle");
	SAVE(ceExpand, "ui/expand");
	SAVE(ceChannelDrag, "ui/drag");
	SAVE(ceUserDrag, "ui/userdrag");
	SAVE(aotbAlwaysOnTop, "ui/alwaysontop");
	SAVE(bAskOnQuit, "ui/askonquit");
	SAVE(bEnableDeveloperMenu, "ui/developermenu");
	SAVE(bLockLayout, "ui/locklayout");
	SAVE(bMinimalView, "ui/minimalview");
	SAVE(bHideFrame, "ui/hideframe");
	SAVE(bUserTop, "ui/usertop");
	SAVE(qbaMainWindowGeometry, "ui/geometry");
	SAVE(qbaMainWindowState, "ui/state");
	SAVE(qbaMinimalViewGeometry, "ui/minimalviewgeometry");
	SAVE(qbaMinimalViewState, "ui/minimalviewstate");
	SAVE(qbaConfigGeometry, "ui/ConfigGeometry");
	SAVE(wlWindowLayout, "ui/WindowLayout");
	SAVE(qbaSplitterState, "ui/splitter");
	SAVE(qbaHeaderState, "ui/header");
	SAVE(qsUsername, "ui/username");
	SAVE(qsLastServer, "ui/server");
	SAVE(ssFilter, "ui/serverfilter");
#ifndef NO_UPDATE_CHECK
	// If this flag has been set, we don't load the following settings so we shouldn't overwrite them here either
	SAVE(bUpdateCheck, "ui/updatecheck");
	SAVE(bPluginCheck, "ui/plugincheck");
	SAVE(bPluginAutoUpdate, "ui/pluginAutoUpdate");
#endif
	SAVE(bHideInTray, "ui/hidetray");
	SAVE(bStateInTray, "ui/stateintray");
	SAVE(bUsage, "ui/usage");
	SAVE(bShowUserCount, "ui/showusercount");
	SAVE(bShowVolumeAdjustments, "ui/showVolumeAdjustments");
	SAVE(bShowNicknamesOnly, "ui/showNicknamesOnly");
	SAVE(bChatBarUseSelection, "ui/chatbaruseselection");
	SAVE(bFilterHidesEmptyChannels, "ui/filterhidesemptychannels");
	SAVE(bFilterActive, "ui/filteractive");
	SAVE(qsImagePath, "ui/imagepath");
	SAVE(bShowContextMenuInMenuBar, "ui/showcontextmenuinmenubar");
	SAVE(qbaConnectDialogGeometry, "ui/connect/geometry");
	SAVE(qbaConnectDialogHeader, "ui/connect/header");
	SAVE(bShowTransmitModeComboBox, "ui/transmitmodecombobox");
	SAVE(bHighContrast, "ui/HighContrast");
	SAVE(iMaxLogBlocks, "ui/MaxLogBlocks");
	SAVE(bLog24HourClock, "ui/24HourClock");
	SAVE(iChatMessageMargins, "ui/ChatMessageMargins");
	SAVE(bDisablePublicList, "ui/disablepubliclist");

	// TalkingUI
	SAVE(qpTalkingUI_Position, "ui/talkingUIPosition");
	SAVE(bShowTalkingUI, "ui/showTalkingUI");
	SAVE(bTalkingUI_LocalUserStaysVisible, "ui/talkingUI_LocalUserStaysVisible");
	SAVE(bTalkingUI_AbbreviateChannelNames, "ui/talkingUI_AbbreviateChannelNames");
	SAVE(bTalkingUI_AbbreviateCurrentChannel, "ui/talkingUI_AbbreviateCurrentChannel");
	SAVE(bTalkingUI_ShowLocalListeners, "ui/talkingUI_ShowLocalListeners");
	SAVE(iTalkingUI_RelativeFontSize, "ui/talkingUI_RelativeFontSize");
	SAVE(iTalkingUI_SilentUserLifeTime, "ui/talkingUI_SilentUserLifeTime");
	SAVE(iTalkingUI_ChannelHierarchyDepth, "ui/talkingUI_ChannelHierarchieDepth");
	SAVE(iTalkingUI_MaxChannelNameLength, "ui/talkingUI_MaxChannelNameLength");
	SAVE(iTalkingUI_PrefixCharCount, "ui/talkingUI_PrefixCharCount");
	SAVE(iTalkingUI_PostfixCharCount, "ui/talkingUI_PostfixCharCount");
	SAVE(qsTalkingUI_AbbreviationReplacement, "ui/talkingUI_AbbreviationReplacement");

	DEPRECATED("ui/talkingUI_ChannelSeparator");
	SAVE(qsHierarchyChannelSeparator, "ui/hierarchy_channelSeparator");

	SAVE(manualPlugin_silentUserDisplaytime, "ui/manualPlugin_silentUserDisplaytime");

	// PTT Button window
	SAVE(bShowPTTButtonWindow, "ui/showpttbuttonwindow");
	SAVE(qbaPTTButtonWindowGeometry, "ui/pttbuttonwindowgeometry");

	// Recording
	SAVE(qsRecordingPath, "recording/path");
	SAVE(qsRecordingFile, "recording/file");
	SAVE(rmRecordingMode, "recording/mode");
	SAVE(iRecordingFormat, "recording/format");

	// Special configuration options not exposed to UI
	SAVE(bDisableCELT, "audio/disablecelt");
	SAVE(disableConnectDialogEditing, "ui/disableconnectdialogediting");
	SAVE(bPingServersDialogViewed, "consent/pingserversdialogviewed");

	// OverlayPrivateWin
	SAVE(iOverlayWinHelperRestartCooldownMsec, "overlay_win/helper/restart_cooldown_msec");
	SAVE(bOverlayWinHelperX86Enable, "overlay_win/helper/x86/enable");
	SAVE(bOverlayWinHelperX64Enable, "overlay_win/helper/x64/enable");

	// LCD
	SAVE(iLCDUserViewMinColWidth, "lcd/userview/mincolwidth");
	SAVE(iLCDUserViewSplitterWidth, "lcd/userview/splitterwidth");

	QByteArray qba = CertWizard::exportCert(kpCertificate);
	settings_ptr->setValue(QLatin1String("net/certificate"), qba);

	SAVE(bShortcutEnable, "shortcut/enable");
	SAVE(bSuppressMacEventTapWarning, "shortcut/mac/suppresswarning");
	SAVE(bEnableEvdev, "shortcut/linux/evdev/enable");
	SAVE(bEnableXInput2, "shortcut/x11/xinput2/enable");
	SAVE(bEnableGKey, "shortcut/gkey");
	SAVE(bEnableXboxInput, "shortcut/windows/xbox/enable");
	SAVE(bEnableUIAccess, "shortcut/windows/uiaccess/enable");

	// Search options
	SAVE(searchForUsers, "search/search_for_users");
	SAVE(searchForChannels, "search/search_for_channels");
	SAVE(searchCaseSensitive, "search/search_case_sensitive");
	SAVE(searchAsRegex, "search/search_as_regex");
	SAVE(searchOptionsShown, "search/search_options_shown");
	SAVEFLAG(searchUserAction, "search/search_user_action");
	SAVEFLAG(searchChannelAction, "search/search_channel_action");
	SAVE(searchDialogPosition, "search/search_dialog_position");

	settings_ptr->beginWriteArray(QLatin1String("shortcuts"));
	int idx = 0;
	foreach (const Shortcut &s, qlShortcuts) {
		if (!s.isServerSpecific()) {
			settings_ptr->setArrayIndex(idx++);
			settings_ptr->setValue(QLatin1String("index"), s.iIndex);
			settings_ptr->setValue(QLatin1String("keys"), s.qlButtons);
			settings_ptr->setValue(QLatin1String("suppress"), s.bSuppress);
			settings_ptr->setValue(QLatin1String("data"), s.qvData);
		}
	}
	settings_ptr->endArray();

	settings_ptr->beginWriteArray(QLatin1String("messages"));
	for (QMap< int, quint32 >::const_iterator it = qmMessages.constBegin(); it != qmMessages.constEnd(); ++it) {
		settings_ptr->setArrayIndex(it.key());
		SAVE(qmMessages[it.key()], "log");
	}
	settings_ptr->endArray();

	settings_ptr->beginWriteArray(QLatin1String("messagesounds"));
	for (QMap< int, QString >::const_iterator it = qmMessageSounds.constBegin(); it != qmMessageSounds.constEnd();
		 ++it) {
		settings_ptr->setArrayIndex(it.key());
		SAVE(qmMessageSounds[it.key()], "logsound");
	}
	settings_ptr->endArray();

	settings_ptr->beginGroup(QLatin1String("lcd/devices"));
	foreach (const QString &d, qmLCDDevices.keys()) {
		bool v = qmLCDDevices.value(d);
		if (!v)
			settings_ptr->setValue(d, v);
		else
			settings_ptr->remove(d);
	}
	settings_ptr->endGroup();

	// Plugins
	foreach (const QString &pluginHash, qhPluginSettings.keys()) {
		QString savePath             = QString::fromLatin1("plugins/");
		const PluginSetting settings = qhPluginSettings.value(pluginHash);
		const QFileInfo info(settings.path);
		QString baseName            = info.baseName(); // Get the filename without file extensions
		const bool containsNonASCII = baseName.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")));

		if (containsNonASCII || baseName.isEmpty()) {
			savePath += pluginHash;
		} else {
			// Make sure there are no spaces in the name
			baseName.replace(QLatin1Char(' '), QLatin1Char('_'));

			// Also include the plugin's filename in the savepath in order
			// to allow for easier identification
			savePath += baseName + QLatin1String("__") + pluginHash;
		}

		settings_ptr->beginGroup(savePath);
		settings_ptr->setValue(QLatin1String("path"), settings.path);
		settings_ptr->setValue(QLatin1String("enabled"), settings.enabled);
		settings_ptr->setValue(QLatin1String("positionalDataEnabled"), settings.positionalDataEnabled);
		settings_ptr->setValue(QLatin1String("allowKeyboardMonitoring"), settings.allowKeyboardMonitoring);
		settings_ptr->endGroup();
	}


	settings_ptr->beginGroup(QLatin1String("overlay"));
	os.save(settings_ptr);
	settings_ptr->endGroup();
}

QDataStream &operator>>(QDataStream &arch, PluginSetting &setting) {
	arch >> setting.enabled;
	arch >> setting.positionalDataEnabled;
	arch >> setting.allowKeyboardMonitoring;

	return arch;
}

QDataStream &operator<<(QDataStream &arch, const PluginSetting &setting) {
	arch << setting.enabled;
	arch << setting.positionalDataEnabled;
	arch << setting.allowKeyboardMonitoring;

	return arch;
}

#undef LOAD
#undef LOADENUM
#undef LOADFLAG
#undef DEPRECATED
#undef SAVE
#undef SAVEFLAG
