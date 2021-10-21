// Copyright 2007-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBAL_H_
#define MUMBLE_MUMBLE_GLOBAL_H_

#include <QtCore/QDir>
#include <boost/shared_ptr.hpp>

#include "ACL.h"
#include "ChannelListenerManager.h"
#include "Settings.h"
#include "Timer.h"
#include "Version.h"

#include <memory>

// Global helper class to spread variables around across threads.

class MainWindow;
class ServerHandler;
class AudioInput;
class AudioOutput;
class Database;
class Log;
class PluginManager;
class QSettings;
class Overlay;
class LCD;
class Zeroconf;
class OverlayClient;
class CELTCodec;
class OpusCodec;
class LogEmitter;
class DeveloperConsole;
class TalkingUI;

class QNetworkAccessManager;

struct Global Q_DECL_FINAL {
private:
	Q_DISABLE_COPY(Global)
public:
	static Global *g_global_struct;
	static Global &get();

	MainWindow *mw;
	Settings s;
	boost::shared_ptr< ServerHandler > sh;
	boost::shared_ptr< AudioInput > ai;
	boost::shared_ptr< AudioOutput > ao;
	/**
	 * @remark Must only be accessed from the main event loop
	 */
	Database *db;
	Log *l;
	/// A pointer to the PluginManager that is used in this session
	PluginManager *pluginManager;
#ifdef USE_OVERLAY
	Overlay *o;
#endif
	LCD *lcd;
	Zeroconf *zeroconf;
	QNetworkAccessManager *nam;
	QSharedPointer< LogEmitter > le;
	DeveloperConsole *c;
	TalkingUI *talkingUI;
	int iPushToTalk;
	Timer tDoublePush;
	quint64 uiDoublePush;
	/// Holds the current VoiceTarget ID to send audio to
	int iTarget;
	/// Holds the value of iTarget before its last change until the current
	/// audio-stream ends (and it has a value > 0). See the comment in
	/// AudioInput::flushCheck for further details on this.
	int iPrevTarget;
	bool bPushToMute;
	bool bCenterPosition;
	bool bPosTest;
	bool bInAudioWizard;
#ifdef USE_OVERLAY
	OverlayClient *ocIntercept;
#endif
	int iAudioPathTime;
	/// A unique ID for the current user. It is being assigned by the server right
	/// after connecting to it. An ID of 0 indicates that the user currently isn't
	/// connected to a server.
	unsigned int uiSession;
	ChanACL::Permissions pPermissions;
	int iMaxBandwidth;
	int iAudioBandwidth;
	QDir qdBasePath;
	QMap< int, CELTCodec * > qmCodecs;
	OpusCodec *oCodec;
	int iCodecAlpha, iCodecBeta;
	bool bPreferAlpha;
	bool bOpus;
	bool bAttenuateOthers;
	/// If set the AudioOutput::mix will forcefully adjust the volume of all
	/// non-priority speakers.
	bool prioritySpeakerActiveOverride;
	bool bAllowHTML;
	unsigned int uiMessageLength;
	unsigned int uiImageLength;
	unsigned int uiMaxUsers;
	bool recordingAllowed;
	bool bQuit;
	QString windowTitlePostfix;
	bool bDebugDumpInput;
	bool bDebugPrintQueue;
	std::unique_ptr< ChannelListenerManager > channelListenerManager;

	bool bHappyEaster;
	static const char ccHappyEaster[];

	QString migratedDBPath;

	Global(const QString &qsConfigPath = QString());
	~Global() = default;

private:
	void migrateDataDir(const QDir &toDir);
};

// Class to handle ordered initialization of globals.
// This allows the same link-time magic as used everywhere else
// for globals that need an init before the GUI starts, but
// after we reach main().

class DeferInit {
private:
	Q_DISABLE_COPY(DeferInit)
protected:
	static QMultiMap< int, DeferInit * > *qmDeferers;
	void add(int priority);

public:
	DeferInit(int priority) { add(priority); };
	DeferInit() { add(0); };
	virtual ~DeferInit();
	virtual void initialize(){};
	virtual void destroy(){};
	static void run_initializers();
	static void run_destroyers();
};

/// Special exit code which causes mumble to restart itself. The outward facing return code with be 0
const int MUMBLE_EXIT_CODE_RESTART = 64738;

#endif
