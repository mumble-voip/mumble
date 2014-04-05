/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#include "mumble_pch.hpp"

#include "VersionCheck.h"

#include "Global.h"
#include "MainWindow.h"
#include "WebFetch.h"

VersionCheck::VersionCheck(bool autocheck, QObject *p, bool focus) : QObject(p) {
	bSilent = autocheck;

	QUrl url;
	url.setPath(focus ? QLatin1String("/focus.php") : QLatin1String("/ver.php"));

	QList<QPair<QString, QString> > queryItems;
	queryItems << qMakePair(QString::fromLatin1("ver"), QString::fromLatin1(QUrl::toPercentEncoding(QLatin1String(MUMBLE_RELEASE))));
	queryItems << qMakePair(QString::fromLatin1("date"), QString::fromLatin1(QUrl::toPercentEncoding(QLatin1String(__DATE__))));
	queryItems << qMakePair(QString::fromLatin1("time"), QString::fromLatin1(QUrl::toPercentEncoding(QLatin1String(__TIME__))));
#if defined(Q_OS_WIN)
	queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("Win32"));
#elif defined(Q_OS_MAC)
# if defined(USE_MAC_UNIVERSAL)
	queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("MacOSX-Universal"));
# else
	queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("MacOSX"));
# endif
#else
	queryItems << qMakePair(QString::fromLatin1("os"), QString::fromLatin1("Unix"));
#endif
	if (! g.s.bUsage)
		queryItems << qMakePair(QString::fromLatin1("nousage"), QString::fromLatin1("1"));
	if (autocheck)
		queryItems << qMakePair(QString::fromLatin1("auto"), QString::fromLatin1("1"));

	queryItems << qMakePair(QString::fromLatin1("locale"), g.s.qsLanguage.isEmpty() ? QLocale::system().name() : g.s.qsLanguage);

	QFile f(qApp->applicationFilePath());
	if (! f.open(QIODevice::ReadOnly)) {
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

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	QUrlQuery query;
	query.setQueryItems(queryItems);
	url.setQuery(query);
#else
	for (int i = 0; i < queryItems.size(); i++) {
		const QPair<QString, QString> &queryPair = queryItems.at(i);
		url.addQueryItem(queryPair.first, queryPair.second);
	}
#endif
	WebFetch::fetch(url, this, SLOT(fetched(QByteArray,QUrl)));
}

void VersionCheck::fetched(QByteArray a, QUrl url) {
	if (! a.isNull()) {
		if (! a.isEmpty()) {
#ifdef SNAPSHOT_BUILD
			if (url.path() == QLatin1String("/focus.php")) {
				g.mw->msgBox(QString::fromUtf8(a));
			} else if (url.path() == QLatin1String("/ver.php")) {
#ifndef Q_OS_WIN
				g.mw->msgBox(QString::fromUtf8(a));
#else
				QDomDocument qdd;
				qdd.setContent(a);

				QDomElement elem = qdd.firstChildElement(QLatin1String("p"));
				elem = elem.firstChildElement(QLatin1String("a"));

				QUrl fetch = QUrl(elem.attribute(QLatin1String("href")));
				fetch.setHost(QString());
				fetch.setScheme(QString());
				if (! fetch.isValid()) {
					g.mw->msgBox(QString::fromUtf8(a));
				} else {
					QString filename = g.qdBasePath.absoluteFilePath(QLatin1String("Snapshots/") + QFileInfo(fetch.path()).fileName());

					QFile qf(filename);
					if (qf.exists()) {
						std::wstring native = QDir::toNativeSeparators(filename).toStdWString();

						WINTRUST_FILE_INFO file;
						ZeroMemory(&file, sizeof(file));
						file.cbStruct = sizeof(file);
						file.pcwszFilePath = native.c_str();

						WINTRUST_DATA data;
						ZeroMemory(&data, sizeof(data));
						data.cbStruct = sizeof(data);
						data.dwUIChoice = WTD_UI_NONE;
						data.fdwRevocationChecks = WTD_REVOKE_NONE;
						data.dwUnionChoice = WTD_CHOICE_FILE;
						data.pFile = &file;
						data.dwProvFlags = WTD_SAFER_FLAG | WTD_USE_DEFAULT_OSVER_CHECK | WTD_LIFETIME_SIGNING_FLAG;
						data.dwUIContext = WTD_UICONTEXT_INSTALL;

						static GUID guid = WINTRUST_ACTION_GENERIC_VERIFY_V2;

						LONG ts = WinVerifyTrust(0, &guid , &data);

						if (ts == 0) {
							if (QMessageBox::question(g.mw,
							                          tr("Upgrade Mumble"),
							                          tr("A new version of Mumble has been detected and automatically downloaded. It is recommended that you either upgrade to this version, or downgrade to the latest stable release. Do you want to launch the installer now?"),
							                          QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
								SHELLEXECUTEINFOW execinfo;
								std::wstring filenative = filename.toStdWString();
								std::wstring dirnative = QDir::toNativeSeparators(QDir::tempPath()).toStdWString();
								ZeroMemory(&execinfo, sizeof(execinfo));
								execinfo.cbSize = sizeof(execinfo);
								execinfo.lpFile = filenative.c_str();
								execinfo.lpDirectory = dirnative.c_str();
								execinfo.nShow = SW_NORMAL;

								if (ShellExecuteExW(&execinfo)) {
									g.mw->bSuppressAskOnQuit = true;
									qApp->closeAllWindows();
								} else {
									g.mw->msgBox(tr("Failed to launch snapshot installer."));
								}
							}

						} else {
							g.mw->msgBox(tr("Corrupt download of new version detected. Automatically removed."));
							qf.remove();
						}

						// Delete all but the N most recent snapshots
						size_t numberOfSnapshotsToKeep = 1;

						QDir snapdir(g.qdBasePath.absolutePath() + QLatin1String("/Snapshots/"),
						             QString(),
						             QDir::Name,
						             QDir::Files);

						foreach(const QFileInfo fileInfo, snapdir.entryInfoList(QStringList(), QDir::NoFilter, QDir::Time)) {
							if (numberOfSnapshotsToKeep) {
								--numberOfSnapshotsToKeep;
								continue;
							}

							qWarning() << "Purging old snapshot" << fileInfo.fileName();
							QFile file(fileInfo.absoluteFilePath());
							file.remove();
						}
					} else {
						g.mw->msgBox(tr("Downloading new snapshot from %1 to %2").arg(Qt::escape(fetch.toString()), Qt::escape(filename)));
						WebFetch::fetch(fetch, this, SLOT(fetched(QByteArray,QUrl)));
						return;
					}
				}
			} else {
				QString filename = g.qdBasePath.absoluteFilePath(QLatin1String("Snapshots/") + QFileInfo(url.path()).fileName());

				QFile qf(filename);
				if (qf.open(QIODevice::WriteOnly)) {
					qf.write(a);
					qf.close();
					new VersionCheck(true, g.mw);
				} else {
					g.mw->msgBox(tr("Failed to write new version to disk."));
				}
#endif
			}
#else
			Q_UNUSED(url);
			g.mw->msgBox(QString::fromUtf8(a));
#endif
		}
	} else if (bSilent) {
		g.mw->msgBox(tr("Mumble failed to retrieve version information from the central server."));
	}

	deleteLater();
}
