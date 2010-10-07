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

#include "VersionCheck.h"
#include "Global.h"
#include "MainWindow.h"
#include "NetworkConfig.h"

VersionCheck::VersionCheck(bool autocheck, QObject *p, bool focus) : QObject(p) {
	bSilent = autocheck;

	QUrl url;
	url.setScheme(QLatin1String("http"));
	url.setHost(g.qsRegionalHost);
	url.setPath(focus ? QLatin1String("/focus.php") : QLatin1String("/ver.php"));

	url.addQueryItem(QLatin1String("ver"), QLatin1String(QUrl::toPercentEncoding(QLatin1String(MUMBLE_RELEASE))));
	url.addQueryItem(QLatin1String("date"), QLatin1String(QUrl::toPercentEncoding(QLatin1String(__DATE__))));
	url.addQueryItem(QLatin1String("time"), QLatin1String(QUrl::toPercentEncoding(QLatin1String(__TIME__))));
#if defined(Q_OS_WIN)
	url.addQueryItem(QLatin1String("os"), QLatin1String("Win32"));
#elif defined(Q_OS_MAC)
	url.addQueryItem(QLatin1String("os"), QLatin1String("MacOSX"));
#else
	url.addQueryItem(QLatin1String("os"), QLatin1String("Unix"));
#endif
	if (! g.s.bUsage)
		url.addQueryItem(QLatin1String("nousage"), QLatin1String("1"));
	if (autocheck)
		url.addQueryItem(QLatin1String("auto"), QLatin1String("1"));

	url.addQueryItem(QLatin1String("locale"), g.s.qsLanguage.isEmpty() ? QLocale::system().name() : g.s.qsLanguage);

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
			url.addQueryItem(QLatin1String("sha1"), QLatin1String(qch.result().toHex()));
		}
	}

	QNetworkReply *rep = Network::get(url);
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
}

void VersionCheck::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
	QUrl url = rep->request().url();

	if (rep->error() == QNetworkReply::NoError) {
		const QByteArray &a=rep->readAll();
		if (! a.isEmpty()) {
#if defined(SNAPSHOT_BUILD) && ! defined(COMPAT_CLIENT)
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
				if (! fetch.isValid()) {
					g.mw->msgBox(QString::fromUtf8(a));
				} else {
					QString filename = g.qdBasePath.absoluteFilePath(QLatin1String("Snapshots/") + QFileInfo(fetch.path()).fileName());

					QFile qf(filename);
					if (qf.exists()) {
						QString	native = QDir::toNativeSeparators(filename);

						WINTRUST_FILE_INFO file;
						ZeroMemory(&file, sizeof(file));
						file.cbStruct = sizeof(file);
						file.pcwszFilePath = native.utf16();

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
								ZeroMemory(&execinfo, sizeof(execinfo));
								execinfo.cbSize = sizeof(execinfo);
								execinfo.lpFile = filename.utf16();
								execinfo.lpDirectory = QDir::toNativeSeparators(QDir::tempPath()).utf16();
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
					} else {
						fetch.setHost(g.qsRegionalHost);
						g.mw->msgBox(tr("Downloading new snapshot from %1 to %2").arg(fetch.toString(), filename));

						QNetworkReply *nrep = Network::get(fetch);
						connect(nrep, SIGNAL(finished()), this, SLOT(finished()));

						rep->deleteLater();
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
			g.mw->msgBox(QString::fromUtf8(a));
#endif
		}
	} else {
		if (url.host() == g.qsRegionalHost) {
			url.setHost(QLatin1String("mumble.info"));
			QNetworkReply *nrep = Network::get(url);
			connect(nrep, SIGNAL(finished()), this, SLOT(finished()));

			rep->deleteLater();
			return;
		} else if (bSilent) {
			g.mw->msgBox(tr("Mumble failed to retrieve version information from the central server."));
		}
	}

	rep->deleteLater();
	deleteLater();
}
