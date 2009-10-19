/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include "Global.h"

Global *Global::g_global_struct;

Global::Global() {
	mw = 0;
	sh = 0;
	db = 0;
	p = 0;
	nam = 0;
	uiSession = 0;
	uiDoublePush = 1000000;
	iPushToTalk = 0;
	iTarget = 0;
	bPushToMute = false;
	bCenterPosition = false;
	bPosTest = false;
	iAudioPathTime = 0;
	iMaxBandwidth = -1;

	iCodecAlpha = 0;
	iCodecBeta = 0;
	bPreferAlpha = true;

	QString apppath = QCoreApplication::instance()->applicationDirPath();
	QFile inifile(QString::fromLatin1("%1/mumble.ini").arg(apppath));
	if (inifile.exists() && inifile.permissions().testFlag(QFile::WriteUser)) {
		qdBasePath = apppath;
		qs = new QSettings(inifile.fileName(), QSettings::IniFormat);
	} else {
		qs = new QSettings();
#if defined(Q_OS_WIN)
		QSettings settings(QSettings::UserScope, QLatin1String("Microsoft"), QLatin1String("Windows"));
		settings.beginGroup(QLatin1String("CurrentVersion/Explorer/Shell Folders"));
		QString dir = QDir::fromNativeSeparators(settings.value(QLatin1String("AppData")).toString());
		if (! dir.isEmpty()) {
			dir.append(QLatin1String("/Mumble"));
			qdBasePath.setPath(dir);
		}
#elif defined(Q_OS_MAC)
		qdBasePath.setPath(QDir::homePath() + QLatin1String("/Library/Preferences/Mumble/"));
#else
		qdBasePath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
		if (! qdBasePath.exists()) {
			QDir::root().mkpath(qdBasePath.absolutePath());
			if (! qdBasePath.exists())
				qdBasePath = QDir::home();
		}
	}
	if (! qdBasePath.exists(QLatin1String("Plugins")))
		qdBasePath.mkpath(QLatin1String("Plugins"));

	qs->setIniCodec("UTF-8");
}

Global::~Global() {
	delete qs;
}

QMultiMap<int, DeferInit *> *DeferInit::qmDeferers = NULL;

void DeferInit::add(int priority) {
	if (qmDeferers == NULL) {
		qmDeferers = new QMultiMap<int, DeferInit *>();
	}
	qmDeferers->insert(priority, this);
}

DeferInit::~DeferInit() {
}

void DeferInit::run_initializers() {
	if (! qmDeferers)
		return;
	foreach(DeferInit *d, *qmDeferers) {
		d->initialize();
	}
}

void DeferInit::run_destroyers() {
	if (! qmDeferers)
		return;
	foreach(DeferInit *d, *qmDeferers) {
		d->destroy();
	}
	delete qmDeferers;
	qmDeferers = NULL;
}
