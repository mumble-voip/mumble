/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009-2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

class QNetworkAccessManager;

struct Global {
private:
	Q_DISABLE_COPY(Global)
public:
	static Global *g_global_struct;
	MainWindow *mw;
	Settings s;
	boost::shared_ptr<ServerHandler> sh;
	boost::shared_ptr<AudioInput> ai;
	boost::shared_ptr<AudioOutput> ao;
	Database *db;
	Log *l;
	Plugins *p;
	QSettings *qs;
	Overlay *o;
	LCD *lcd;
	BonjourClient *bc;
	QNetworkAccessManager *nam;
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
	bool bQuit;
	QString qsCurrentStyle;

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

// -Wshadow is bugged. If an inline function of a class uses a variable or
// parameter named 'g', that will generate a warning even if the class header
// is included long before this definition.

#define g (*Global::g_global_struct)

#endif
