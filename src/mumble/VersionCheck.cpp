// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VersionCheck.h"

#include "MainWindow.h"
#include "Utils.h"
#include "WebFetch.h"
#include "Global.h"

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include <QtCore/QUrlQuery>
#include <QtWidgets/QMessageBox>
#include <QtXml/QDomDocument>

#include <QtConcurrent/QtConcurrent>

#ifdef Q_OS_WIN
#	include <shellapi.h>
#	include <softpub.h>
#endif

VersionCheck::VersionCheck(bool autocheck, QObject *p, bool focus) : QObject(p), m_preparationWatcher() {
	connect(&m_preparationWatcher, &QFutureWatcher< void >::finished, this, &VersionCheck::performRequest);

	QFuture< void > future = QtConcurrent::run([this, autocheck, focus]() {
		m_requestURL.setPath(focus ? QLatin1String("/v1/banner") : QLatin1String("/v1/version-check"));

		QList< QPair< QString, QString > > queryItems;
		queryItems << qMakePair(QString::fromLatin1("ver"),
								QString::fromLatin1(QUrl::toPercentEncoding(Version::getRelease())));
#if defined(Q_OS_WIN)
#	if defined(Q_OS_WIN64)
		queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("WinX64"));
#	else
		queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("Win32"));
#	endif
#elif defined(Q_OS_MAC)
#	if defined(USE_MAC_UNIVERSAL)
		queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("MacOSX-Universal"));
#	else
		queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("MacOSX"));
#	endif
#else
		queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("Unix"));
#endif
		if (!Global::get().s.bUsage)
			queryItems << qMakePair(QString::fromLatin1("nousage"), QString::fromLatin1("1"));
		if (autocheck)
			queryItems << qMakePair(QString::fromLatin1("auto"), QString::fromLatin1("1"));

		queryItems << qMakePair(QString::fromLatin1("locale"), Global::get().s.qsLanguage.isEmpty()
																   ? QLocale::system().name()
																   : Global::get().s.qsLanguage);

		QFile f(qApp->applicationFilePath());
		if (!f.open(QIODevice::ReadOnly)) {
			qWarning("VersionCheck: Failed to open binary");
		} else {
			QByteArray a = f.readAll();
			if (a.size() < 1) {
				qWarning("VersionCheck: suspiciously small binary");
			} else {
				QCryptographicHash qch(QCryptographicHash::Sha1);
				qch.addData(a);
				queryItems << qMakePair(QString::fromLatin1("sha1"), QString::fromLatin1(qch.result().toHex()));
			}
		}

		QUrlQuery query;
		query.setQueryItems(queryItems);
		m_requestURL.setQuery(query);
	});

	m_preparationWatcher.setFuture(future);
}

void VersionCheck::performRequest() {
	WebFetch::fetch(QLatin1String("update"), m_requestURL, this, SLOT(fetched(QByteArray, QUrl)));
}

void VersionCheck::fetched(QByteArray a, QUrl url) {
	if (!a.isNull()) {
		if (!a.isEmpty()) {
#ifdef SNAPSHOT_BUILD
			if (url.path() == QLatin1String("/v1/banner")) {
				Global::get().mw->msgBox(QString::fromUtf8(a));
			} else if (url.path() == QLatin1String("/v1/version-check")) {
#	ifndef Q_OS_WIN
				Global::get().mw->msgBox(QString::fromUtf8(a));
#	else
				QDomDocument qdd;
				qdd.setContent(a);

				QDomElement elem = qdd.firstChildElement(QLatin1String("p"));
				elem             = elem.firstChildElement(QLatin1String("a"));

				QUrl fetch = QUrl(elem.attribute(QLatin1String("href")));
				fetch.setHost(QString());
				fetch.setScheme(QString());
				if (!fetch.isValid()) {
					Global::get().mw->msgBox(QString::fromUtf8(a));
				} else {
					QString filename = Global::get().qdBasePath.absoluteFilePath(QLatin1String("Snapshots/")
																				 + QFileInfo(fetch.path()).fileName());

					QFile qf(filename);
					if (qf.exists()) {
						std::wstring native = QDir::toNativeSeparators(filename).toStdWString();

						WINTRUST_FILE_INFO file;
						ZeroMemory(&file, sizeof(file));
						file.cbStruct      = sizeof(file);
						file.pcwszFilePath = native.c_str();

						WINTRUST_DATA data;
						ZeroMemory(&data, sizeof(data));
						data.cbStruct            = sizeof(data);
						data.dwUIChoice          = WTD_UI_NONE;
						data.fdwRevocationChecks = WTD_REVOKE_NONE;
						data.dwUnionChoice       = WTD_CHOICE_FILE;
						data.pFile               = &file;
						data.dwProvFlags         = WTD_SAFER_FLAG | WTD_USE_DEFAULT_OSVER_CHECK;
						data.dwUIContext         = WTD_UICONTEXT_INSTALL;

						static GUID guid = WINTRUST_ACTION_GENERIC_VERIFY_V2;

						LONG ts = WinVerifyTrust(0, &guid, &data);

						if (ts == 0) {
							if (QMessageBox::question(
									Global::get().mw, tr("Upgrade Mumble"),
									tr("A new version of Mumble has been detected and automatically downloaded. It is "
									   "recommended that you either upgrade to this version, or downgrade to the "
									   "latest stable release. Do you want to launch the installer now?"),
									QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes)
								== QMessageBox::Yes) {
								SHELLEXECUTEINFOW execinfo;
								std::wstring filenative = filename.toStdWString();
								std::wstring dirnative  = QDir::toNativeSeparators(QDir::tempPath()).toStdWString();
								ZeroMemory(&execinfo, sizeof(execinfo));
								execinfo.cbSize      = sizeof(execinfo);
								execinfo.lpFile      = filenative.c_str();
								execinfo.lpDirectory = dirnative.c_str();
								execinfo.nShow       = SW_NORMAL;

								if (ShellExecuteExW(&execinfo)) {
									Global::get().mw->forceQuit = true;
									qApp->closeAllWindows();
								} else {
									Global::get().mw->msgBox(tr("Failed to launch snapshot installer."));
								}
							}

						} else {
							Global::get().mw->msgBox(
								tr("Corrupt download of new version detected. Automatically removed."));
							qf.remove();
						}

						// Delete all but the N most recent snapshots
						size_t numberOfSnapshotsToKeep = 1;

						QDir snapdir(Global::get().qdBasePath.absolutePath() + QLatin1String("/Snapshots/"), QString(),
									 QDir::Name, QDir::Files);

						foreach (const QFileInfo fileInfo,
								 snapdir.entryInfoList(QStringList(), QDir::NoFilter, QDir::Time)) {
							if (numberOfSnapshotsToKeep) {
								--numberOfSnapshotsToKeep;
								continue;
							}

							qWarning() << "Purging old snapshot" << fileInfo.fileName();
							QFile file(fileInfo.absoluteFilePath());
							file.remove();
						}
					} else {
						Global::get().mw->msgBox(tr("Downloading new snapshot from %1 to %2")
													 .arg(fetch.toString().toHtmlEscaped(), filename.toHtmlEscaped()));
						WebFetch::fetch(QLatin1String("dl"), fetch, this, SLOT(fetched(QByteArray, QUrl)));
						return;
					}
				}
			} else {
				QString filename = Global::get().qdBasePath.absoluteFilePath(QLatin1String("Snapshots/")
																			 + QFileInfo(url.path()).fileName());

				QFile qf(filename);
				if (qf.open(QIODevice::WriteOnly)) {
					qf.write(a);
					qf.close();
					new VersionCheck(true, Global::get().mw);
				} else {
					Global::get().mw->msgBox(tr("Failed to write new version to disk."));
				}
#	endif
			}
#else
			Q_UNUSED(url);
			Global::get().mw->msgBox(QString::fromUtf8(a));
#endif
		}
	} else {
		Global::get().mw->msgBox(tr("Mumble failed to retrieve version information from the central server."));
	}

	deleteLater();
}
