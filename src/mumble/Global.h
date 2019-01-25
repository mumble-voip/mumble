// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_GLOBAL_H_
#define MUMBLE_MUMBLE_GLOBAL_H_

#include <boost/shared_ptr.hpp>
#include <QtCore/QDir>

#include "ACL.h"
#include "Settings.h"
#include "Timer.h"
#include "Version.h"

// Global helper class to spread variables around across threads.

class MainWindow;
class ServerHandler;
class AudioInput;
class AudioOutput;
class Database;
class Log;
class Plugins;
class QSettings;
class Overlay;
class LCD;
class BonjourClient;
class OverlayClient;
class CELTCodec;
class OpusCodec;
class LogEmitter;
class DeveloperConsole;

class QNetworkAccessManager;

struct Global Q_DECL_FINAL {
private:
	Q_DISABLE_COPY(Global)
public:
	static Global *g_global_struct;
	MainWindow *mw;
	Settings s;
	boost::shared_ptr<ServerHandler> sh;
	boost::shared_ptr<AudioInput> ai;
	boost::shared_ptr<AudioOutput> ao;
	/**
	 * @remark Must only be accessed from the main event loop
	 */
	Database *db;
	Log *l;
	Plugins *p;
	QSettings *qs;
	Overlay *o;
	LCD *lcd;
	BonjourClient *bc;
	QNetworkAccessManager *nam;
	QSharedPointer<LogEmitter> le;
	DeveloperConsole *c;
	int iPushToTalk;
	Timer tDoublePush;
	quint64 uiDoublePush;
	int iTarget;
	int iPrevTarget;
	bool bPushToMute;
	bool bCenterPosition;
	bool bPosTest;
	bool bInAudioWizard;
	OverlayClient *ocIntercept;
	int iAudioPathTime;
	unsigned int uiSession;
	ChanACL::Permissions pPermissions;
	int iMaxBandwidth;
	int iAudioBandwidth;
	QDir qdBasePath;
	QMap<int, CELTCodec *> qmCodecs;
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
	bool bQuit;

	bool bHappyEaster;
	static const char ccHappyEaster[];

	Global();
	~Global();
};

// Class to handle ordered initialization of globals.
// This allows the same link-time magic as used everywhere else
// for globals that need an init before the GUI starts, but
// after we reach main().

class DeferInit {
	private:
		Q_DISABLE_COPY(DeferInit)
	protected:
		static QMultiMap<int, DeferInit *> *qmDeferers;
		void add(int priority);
	public:
		DeferInit(int priority) {
			add(priority);
		};
		DeferInit() {
			add(0);
		};
		virtual ~DeferInit();
		virtual void initialize() { };
		virtual void destroy() { };
		static void run_initializers();
		static void run_destroyers();
};

/// Special exit code which causes mumble to restart itself. The outward facing return code with be 0
const int MUMBLE_EXIT_CODE_RESTART = 64738;

// -Wshadow is bugged. If an inline function of a class uses a variable or
// parameter named 'g', that will generate a warning even if the class header
// is included long before this definition.

#define g (*Global::g_global_struct)

#endif
