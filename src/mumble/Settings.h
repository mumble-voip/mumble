/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "mumble_pch.hpp"

// Global helper classes to spread variables around across threads
// especially helpfull to initialize things like the stored
// preference for audio transmission, since the GUI elements
// will be created long before the AudioInput object, and the
// latter lives in a separate thread and so cannot touch the
// GUI.

struct Shortcut {
	int iIndex;
	QList<QVariant> qlButtons;
	QVariant qvData;
	bool bSuppress;
	bool operator <(const Shortcut &) const;
	bool isServerSpecific() const;
	bool operator ==(const Shortcut &) const;
};

struct ShortcutTarget {
	bool bUsers;
	QStringList qlUsers;
	QList<unsigned int> qlSessions;
	int iChannel;
	QString qsGroup;
	bool bLinks;
	bool bChildren;
	bool bForceCenter;
	ShortcutTarget();
	bool isServerSpecific() const;
	bool operator <(const ShortcutTarget &) const;
	bool operator ==(const ShortcutTarget &) const;
};

quint32 qHash(const ShortcutTarget &);
quint32 qHash(const QList<ShortcutTarget> &);

QDataStream &operator<<(QDataStream &, const ShortcutTarget &);
QDataStream &operator>>(QDataStream &, ShortcutTarget &);
Q_DECLARE_METATYPE(ShortcutTarget)

struct OverlaySettings {
	enum OverlayPresets { AvatarAndName, LargeSquareAvatar };

	enum OverlayShow { Talking, HomeChannel, LinkedChannels };

	bool bEnable;

	QString qsStyle;

	OverlayShow osShow;
	bool bAlwaysSelf;

	float fX;
	float fY;

	qreal fZoom;
	unsigned int uiColumns;

	QColor qcUserName[4];
	QFont qfUserName;

	QColor qcChannel;
	QFont qfChannel;

	QColor qcFps;
	QFont qfFps;

	qreal fBoxPad;
	qreal fBoxPenWidth;
	QColor qcBoxPen;
	QColor qcBoxFill;

	bool bUserName;
	bool bChannel;
	bool bMutedDeafened;
	bool bAvatar;
	bool bBox;
	bool bFps;

	qreal fUserName;
	qreal fChannel;
	qreal fMutedDeafened;
	qreal fAvatar;
	qreal fUser[4];
	qreal fFps;

	QRectF qrfUserName;
	QRectF qrfChannel;
	QRectF qrfMutedDeafened;
	QRectF qrfAvatar;
	QRectF qrfFps;

	Qt::Alignment qaUserName;
	Qt::Alignment qaChannel;
	Qt::Alignment qaMutedDeafened;
	Qt::Alignment qaAvatar;

	bool bUseWhitelist;
	QStringList qslBlacklist;
	QStringList qslWhitelist;

	OverlaySettings();
	void setPreset(const OverlayPresets preset = AvatarAndName);

	void load();
	void load(QSettings*);
	void save();
	void save(QSettings*);
};

struct Settings {
	enum AudioTransmit { Continous, VAD, PushToTalk };
	enum VADSource { Amplitude, SignalToNoise };
	enum LoopMode { None, Local, Server };
	enum ChannelExpand { NoChannels, ChannelsWithUsers, AllChannels };
	enum ChannelDrag { Ask, DoNothing, Move };
	enum ServerShow { ShowPopulated, ShowReachable, ShowAll };
	enum TalkState { Passive, Talking, Whispering, Shouting };
	typedef QPair<QList<QSslCertificate>, QSslKey> KeyPair;

	AudioTransmit atTransmit;
	quint64 uiDoublePush;

	bool bExpert;

	bool bPushClick;
	static const QString cqsDefaultPushClickOn;
	static const QString cqsDefaultPushClickOff;
	QString qsPushClickOn;
	QString qsPushClickOff;

	bool bTransmitPosition;
	bool bMute, bDeaf;
	bool bTTS;
	bool bUserTop;
	bool bWhisperFriends;
	int iTTSVolume, iTTSThreshold;
	int iQuality, iMinLoudness, iVoiceHold, iJitterBufferSize;
	int iNoiseSuppress;
	unsigned int iIdleTime;
	VADSource vsVAD;
	float fVADmin, fVADmax;
	int iFramesPerPacket;
	QString qsAudioInput, qsAudioOutput;
	float fVolume;
	float fOtherVolume;
	bool bAttenuateOthersOnTalk;
	bool bAttenuateOthers;
	int iOutputDelay;

	QString qsALSAInput, qsALSAOutput;
	QString qsPulseAudioInput, qsPulseAudioOutput;
	QString qsOSSInput, qsOSSOutput;
	int iPortAudioInput, iPortAudioOutput;
	QString qsASIOclass;
	QList<QVariant> qlASIOmic;
	QList<QVariant> qlASIOspeaker;
	QString qsCoreAudioInput, qsCoreAudioOutput;
	QString qsWASAPIInput, qsWASAPIOutput;
	QByteArray qbaDXInput, qbaDXOutput;

	bool bExclusiveInput, bExclusiveOutput;
	bool bEcho;
	bool bEchoMulti;
	bool bPositionalAudio;
	bool bPositionalHeadphone;
	float fAudioMinDistance, fAudioMaxDistance, fAudioMaxDistVolume, fAudioBloom;
	QMap<QString, bool> qmPositionalAudioPlugins;

	OverlaySettings os;

	int iLCDUserViewMinColWidth;
	int iLCDUserViewSplitterWidth;
	QMap<QString, bool> qmLCDDevices;

	QList<Shortcut> qlShortcuts;

	enum MessageLog { LogNone = 0x00, LogConsole = 0x01, LogTTS = 0x02, LogBalloon = 0x04, LogSoundfile = 0x08};
	QMap<int, QString> qmMessageSounds;
	QMap<int, quint32> qmMessages;

	QString qsLanguage;
	QString qsStyle;
	QString qsSkin;
	QByteArray qbaMainWindowGeometry, qbaMainWindowState, qbaMinimalViewGeometry, qbaMinimalViewState, qbaSplitterState, qbaHeaderState;
	QByteArray qbaConfigGeometry;
	enum WindowLayout { LayoutClassic, LayoutStacked, LayoutHybrid, LayoutCustom };
	WindowLayout wlWindowLayout;
	ChannelExpand ceExpand;
	ChannelDrag ceChannelDrag;
	bool bMinimalView;
	bool bHideFrame;
	enum AlwaysOnTopBehaviour { OnTopNever, OnTopAlways, OnTopInMinimal, OnTopInNormal };
	AlwaysOnTopBehaviour aotbAlwaysOnTop;
	bool bAskOnQuit;
	bool bHideInTray;
	bool bStateInTray;
	bool bUsage;
	bool bShowUserCount;
	QByteArray qbaConnectDialogHeader, qbaConnectDialogGeometry;
	bool bShowContextMenuInMenuBar;

	QString qsUsername;
	QString qsLastServer;
	ServerShow ssFilter;

	QString qsImagePath;

	bool bUpdateCheck;
	bool bPluginOverlayCheck;

	// Network settings
	enum ProxyType { NoProxy, HttpProxy, Socks5Proxy };
	bool bTCPCompat;
	bool bReconnect;
	bool bAutoConnect;
	bool bQoS;
	ProxyType ptProxyType;
	QString qsProxyHost, qsProxyUsername, qsProxyPassword;
	unsigned short usProxyPort;

	static const int ciDefaultMaxImageSize = 50 * 1024; // Restrict to 50KiB as a default
	int iMaxImageSize;
	int iMaxImageWidth;
	int iMaxImageHeight;
	KeyPair kpCertificate;
	bool bSuppressIdentity;

	// Accessibility
	bool bHighContrast;

	// Recording
	QString qsRecordingPath;
	QString qsRecordingFile;
	enum RecordingMode { RecordingMixdown, RecordingMultichannel };
	RecordingMode rmRecordingMode;
	int iRecordingFormat;

	// Config updates
	unsigned int uiUpdateCounter;

	// Nonsaved
	LoopMode lmLoopMode;
	float dPacketLoss;
	float dMaxPacketDelay;

	bool doEcho() const;
	bool doPositionalAudio() const;

	Settings();
	void load();
	void load(QSettings*);
	void save();
};

#else
struct Settings;
#endif
