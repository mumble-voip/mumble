/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

static void migrateDataDir() {
#ifdef Q_OS_MAC
	QString olddir = QDir::homePath() + QLatin1String("/Library/Preferences/Mumble");
	QString newdir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QString linksTo = QFile::readLink(olddir);
	if (!QFile::exists(newdir) && QFile::exists(olddir) && linksTo.isEmpty()) {
		QDir d;
		if (d.rename(olddir, newdir)) {
			if (d.cd(QDir::homePath() + QLatin1String("/Library/Preferences"))) {
				if (QFile::link(d.relativeFilePath(newdir), olddir)) {
					qWarning("Migrated application data directory from '%s' to '%s'",
					             qPrintable(olddir), qPrintable(newdir));
					return;
				}
			}
		}
	} else {
		/* Data dir has already been migrated. */
		return;
	}

	qWarning("Application data migration failed.");
#endif
}

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
	iPrevTarget = 0;
	bPushToMute = false;
	bCenterPosition = false;
	bPosTest = false;
	iAudioPathTime = 0;
	iMaxBandwidth = -1;

	iCodecAlpha = 0;
	iCodecBeta = 0;
	bPreferAlpha = true;

	bAttenuateOthers = false;

	bAllowHTML = true;
	uiMessageLength = 5000;
	uiImageLength = 131072;

	qs = NULL;

	ocIntercept = NULL;
	bc = NULL;
	lcd = NULL;
	o = NULL;
	l = NULL;

	bQuit = false;

	QStringList qsl;
	qsl << QCoreApplication::instance()->applicationDirPath();
	qsl << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#if defined(Q_OS_WIN)
	QSettings settings(QSettings::UserScope, QLatin1String("Microsoft"), QLatin1String("Windows"));
	settings.beginGroup(QLatin1String("CurrentVersion/Explorer/Shell Folders"));
	QString appdata = QDir::fromNativeSeparators(settings.value(QLatin1String("AppData")).toString());
	if (! appdata.isEmpty()) {
		appdata.append(QLatin1String("/Mumble"));
		qsl << appdata;
	}
	settings.endGroup();
#endif

	foreach(const QString &dir, qsl) {
		QFile inifile(QString::fromLatin1("%1/mumble.ini").arg(dir));
		if (inifile.exists() && inifile.permissions().testFlag(QFile::WriteUser)) {
			qdBasePath = dir;
			qs = new QSettings(inifile.fileName(), QSettings::IniFormat);
			break;
		}
	}

	if (!qs) {
		qs = new QSettings();
#if defined(Q_OS_WIN)
		if (! appdata.isEmpty())
			qdBasePath.setPath(appdata);
#else
		migrateDataDir();
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

#if QT_VERSION >= 0x040500
	qs->setIniCodec("UTF-8");
#endif
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
