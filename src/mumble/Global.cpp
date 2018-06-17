// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "Global.h"

Global *Global::g_global_struct;

#ifndef Q_OS_WIN
static void migrateDataDir() {
#ifdef Q_OS_MAC
	QString olddir = QDir::homePath() + QLatin1String("/Library/Preferences/Mumble");
#if QT_VERSION >= 0x050000
	QString newdir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
	QString newdir = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif // QT_VERSION
	QString linksTo = QFile::readLink(olddir);
	if (!QFile::exists(newdir) && QFile::exists(olddir) && linksTo.isEmpty()) {
		QDir d;
		d.mkpath(newdir + QLatin1String("/.."));
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
#endif // Q_OS_MAC

// Qt4 used another data directory on Unix-like systems, to ensure a seamless
// transition we must first move the users data to the new directory.
#if defined(Q_OS_UNIX) && ! defined(Q_OS_MAC)
#if QT_VERSION >= 0x050000
	QString olddir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/data/Mumble");
	QString newdir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1String("/Mumble");

	if (!QFile::exists(newdir) && QFile::exists(olddir)) {
		QDir d;
		d.mkpath(newdir + QLatin1String("/.."));
		if (d.rename(olddir, newdir)) {
			qWarning("Migrated application data directory from '%s' to '%s'",
			         qPrintable(olddir), qPrintable(newdir));
			return;
		}
	} else {
		/* Data dir has already been migrated. */
		return;
	}

	qWarning("Application data migration failed.");
#endif // QT_VERSION
#endif // defined(Q_OS_UNIX) && ! defined(Q_OS_MAC)
}
#endif // Q_OS_WIN

Global::Global() {
	mw = 0;
	p = 0;
	nam = 0;
	c = 0;
	uiSession = 0;
	uiDoublePush = 1000000;
	iPushToTalk = 0;
	iTarget = 0;
	iPrevTarget = 0;
	bPushToMute = false;
	bCenterPosition = false;
	bPosTest = false;
	bInAudioWizard = false;
	iAudioPathTime = 0;
	iAudioBandwidth = -1;
	iMaxBandwidth = -1;

	iCodecAlpha = 0;
	iCodecBeta = 0;
	bPreferAlpha = true;
#ifdef USE_OPUS
	bOpus = true;
#else
	bOpus = false;
#endif

	bAttenuateOthers = false;
	prioritySpeakerActiveOverride = false;

	bAllowHTML = true;
	uiMessageLength = 5000;
	uiImageLength = 131072;
	uiMaxUsers = 0;

	qs = NULL;

	ocIntercept = NULL;
	bc = NULL;
	lcd = NULL;
	o = NULL;
	l = NULL;

	bHappyEaster = false;

	bQuit = false;

	QStringList qsl;
	qsl << QCoreApplication::instance()->applicationDirPath();
#if QT_VERSION >= 0x050000
	qsl << QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
	qsl << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
#if defined(Q_OS_WIN)
	QString appdata;
	wchar_t appData[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appData))) {
		appdata = QDir::fromNativeSeparators(QString::fromWCharArray(appData));

		if (!appdata.isEmpty()) {
			appdata.append(QLatin1String("/Mumble"));
			qsl << appdata;
		}
	}
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
#if QT_VERSION >= 0x050000
		qdBasePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
		qdBasePath = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
#endif
		if (! qdBasePath.exists()) {
			QDir::root().mkpath(qdBasePath.absolutePath());
			if (! qdBasePath.exists())
				qdBasePath = QDir::home();
		}
	}

	if (! qdBasePath.exists(QLatin1String("Plugins")))
		qdBasePath.mkpath(QLatin1String("Plugins"));
	if (! qdBasePath.exists(QLatin1String("Overlay")))
		qdBasePath.mkpath(QLatin1String("Overlay"));
	if (! qdBasePath.exists(QLatin1String("Themes")))
		qdBasePath.mkpath(QLatin1String("Themes"));

	qs->setIniCodec("UTF-8");
}

Global::~Global() {
	delete qs;
}

const char Global::ccHappyEaster[] = {072, 057, 0162, 0145, 0143, 056, 0163, 0166, 0147, 0, 0117, 0160, 0145, 0156, 040, 0164, 0150, 0145, 040, 0160, 0157, 0144, 040, 0142, 0141, 0171, 040, 0144, 0157, 0157, 0162, 0163, 054, 040, 0110, 0101, 0114, 056, 0, 0111, 047, 0155, 040, 0163, 0157, 0162, 0162, 0171, 054, 040, 045, 061, 056, 040, 0111, 047, 0155, 040, 0141, 0146, 0162, 0141, 0151, 0144, 040, 0111, 040, 0143, 0141, 0156, 047, 0164, 040, 0144, 0157, 040, 0164, 0150, 0141, 0164, 056, 0, 0121,0127,0151,0144,0147,0145,0164,0173,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,055,0143,0157,0154,0157,0162,072,0142,0154,0141,0143,0153,073,0141,0154,0164,0145,0162,0156,0141,0164,0145,055,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,055,0143,0157,0154,0157,0162,072,043,063,063,060,060,060,060,073,0143,0157,0154,0157,0162,072,0167,0150,0151,0164,0145,073,0175,0121,0124,0145,0170,0164,0102,0162,0157,0167,0163,0145,0162,0173,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,055,0151,0155,0141,0147,0145,072,0165,0162,0154,050,072,057,0162,0145,0143,056,0163,0166,0147,051,073,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,055,0160,0157,0163,0151,0164,0151,0157,0156,072,0143,0145,0156,0164,0145,0162,0143,0145,0156,0164,0145,0162,073,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,055,0143,0154,0151,0160,072,0160,0141,0144,0144,0151,0156,0147,073,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,055,0157,0162,0151,0147,0151,0156,072,0160,0141,0144,0144,0151,0156,0147,073,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,055,0162,0145,0160,0145,0141,0164,072,0156,0157,055,0162,0145,0160,0145,0141,0164,073,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,055,0141,0164,0164,0141,0143,0150,0155,0145,0156,0164,072,0146,0151,0170,0145,0144,073,0175,0121,0115,0145,0156,0165,0102,0141,0162,072,072,0151,0164,0145,0155,0173,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,072,0164,0162,0141,0156,0163,0160,0141,0162,0145,0156,0164,073,0175,0121,0110,0145,0141,0144,0145,0162,0126,0151,0145,0167,072,072,0163,0145,0143,0164,0151,0157,0156,0173,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,072,0164,0162,0141,0156,0163,0160,0141,0162,0145,0156,0164,073,0175,0121,0124,0141,0142,0102,0141,0162,072,072,0164,0141,0142,0173,0142,0141,0143,0153,0147,0162,0157,0165,0156,0144,072,0164,0162,0141,0156,0163,0160,0141,0162,0145,0156,0164,073,0175,0};

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
