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

#include "Plugins.h"
#include "Log.h"
#include "Message.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "../../plugins/mumble_plugin.h"
#include "Global.h"

static ConfigWidget *PluginConfigDialogNew(Settings &st) {
	return new PluginConfig(st);
}

static ConfigRegistrar registrar(5000, PluginConfigDialogNew);

struct PluginInfo {
	bool locked;
	bool enabled;
	QLibrary lib;
	QString filename;
	QString description;
	QString shortname;
	MumblePlugin *p;
	PluginInfo();
};

PluginInfo::PluginInfo() {
	locked = false;
	enabled = false;
	p = NULL;
}

PluginConfig::PluginConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	qtwPlugins->header()->setResizeMode(0, QHeaderView::Stretch);
	qtwPlugins->header()->setResizeMode(1, QHeaderView::ResizeToContents);

	refillPluginList();
}

QString PluginConfig::title() const {
	return tr("Plugins");
}

QIcon PluginConfig::icon() const {
	return QIcon(QLatin1String("skin:config_plugin.png"));
}

void PluginConfig::load(const Settings &r) {
	loadCheckBox(qcbTransmit, r.bTransmitPosition);
}

void PluginConfig::save() const {
	s.bTransmitPosition = qcbTransmit->isChecked();
	QMutexLocker lock(&g.p->qmPlugins);

	s.qmPositionalAudioPlugins.clear();
	foreach(int k, qhInfos.keys()) {
		PluginInfo *pi = qhInfos[k];
		bool enabled = qtwPlugins->topLevelItem(k)->checkState(1) == Qt::Checked;
		s.qmPositionalAudioPlugins[QFileInfo(pi->filename).fileName()] = enabled;
		pi->enabled = enabled;
	}

}

bool PluginConfig::expert(bool) {
	return false;
}

void PluginConfig::on_qpbConfig_clicked() {
	QModelIndex idx = qtwPlugins->currentIndex();
	if (!idx.isValid())
		return;
	PluginInfo *pi=qhInfos[idx.row()];

	if (pi->p->config)
		pi->p->config(winId());
	else
		QMessageBox::information(this, QLatin1String("Mumble"), tr("Plugin has no configure function."), QMessageBox::Ok, QMessageBox::NoButton);
}

void PluginConfig::on_qpbAbout_clicked() {
	QModelIndex idx = qtwPlugins->currentIndex();
	if (!idx.isValid())
		return;
	PluginInfo *pi=qhInfos[idx.row()];

	if (pi->p->about)
		pi->p->about(winId());
	else
		QMessageBox::information(this, QLatin1String("Mumble"), tr("Plugin has no about function."), QMessageBox::Ok, QMessageBox::NoButton);
}

void PluginConfig::on_qpbReload_clicked() {
	g.p->rescanPlugins();
	refillPluginList();
}

void PluginConfig::refillPluginList() {
	QMutexLocker lock(&g.p->qmPlugins);
	qtwPlugins->clear();
	qhInfos.clear();
	PluginInfo *pi;
	foreach(pi, g.p->qlPlugins) {
		QTreeWidgetItem *i = new QTreeWidgetItem(qtwPlugins);
		i->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		i->setCheckState(1, pi->enabled ? Qt::Checked : Qt::Unchecked);
		i->setText(0, pi->description);
		qhInfos[qtwPlugins->indexOfTopLevelItem(i)] = pi;
	}
}

Plugins::Plugins(QObject *p) : QObject(p) {
	QTimer *timer=new QTimer(this);
	timer->setObjectName(QLatin1String("Timer"));
	timer->start(500);
	locked = prevlocked = NULL;
	bValid = false;
	iPluginTry = 0;
	for (int i=0;i<3;i++)
		fPosition[i]=fFront[i]=fTop[i]= 0.0;
	QMetaObject::connectSlotsByName(this);

#ifndef PLUGIN_PATH
#ifndef Q_OS_MAC
	qsSystemPlugins=QString::fromLatin1("%1/plugins").arg(qApp->applicationDirPath());
#else
	qsSystemPlugins=QString::fromLatin1("%1/../Plugins").arg(qApp->applicationDirPath());
#endif
#else
	qsSystemPlugins=QLatin1String(MUMTEXT(PLUGIN_PATH));
#endif

	qsUserPlugins = g.qdBasePath.absolutePath() + QLatin1String("/Plugins");

#ifdef Q_OS_WIN
	// According to MS KB Q131065, we need this to OpenProcess()

	hToken = FALSE;

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken)) {
		if (GetLastError() == ERROR_NO_TOKEN) {
			ImpersonateSelf(SecurityImpersonation);
			OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken);
		}
	}

	TOKEN_PRIVILEGES tp;
	LUID luid;
	cbPrevious=sizeof(TOKEN_PRIVILEGES);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious);
#endif
}

Plugins::~Plugins() {
	clearPlugins();

#ifdef Q_OS_WIN
	AdjustTokenPrivileges(hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL);
	CloseHandle(hToken);
#endif
}

void Plugins::clearPlugins() {
	QMutexLocker lock(&qmPlugins);
	foreach(PluginInfo *pi, qlPlugins) {
		if (pi->locked)
			pi->p->unlock();
		pi->lib.unload();
		delete pi;
	}
	qlPlugins.clear();
}

void Plugins::rescanPlugins() {
	clearPlugins();

	QMutexLocker lock(&qmPlugins);
	prevlocked = locked = NULL;
	bValid = false;

	QDir qud(qsUserPlugins, QString(), QDir::Name, QDir::Files | QDir::Readable);
	QDir qd(qsSystemPlugins, QString(), QDir::Name, QDir::Files | QDir::Readable);
	QFileInfoList libs = qud.entryInfoList() + qd.entryInfoList();
	QSet<QString> loaded;
	foreach(const QFileInfo &libinfo, libs) {
		QString fname = libinfo.fileName();
		QString libname = libinfo.absoluteFilePath();
		if (!loaded.contains(fname) && QLibrary::isLibrary(libname)) {
			PluginInfo *pi = new PluginInfo();
			pi->filename = libname;
			pi->lib.setFileName(pi->filename);
			if (pi->lib.load()) {
				mumblePluginFunc mpf = reinterpret_cast<mumblePluginFunc>(pi->lib.resolve("getMumblePlugin"));
				if (mpf) {
					pi->p = mpf();
					if (pi->p && (pi->p->magic == MUMBLE_PLUGIN_MAGIC)) {
						pi->description = QString::fromStdWString(pi->p->description);
						pi->shortname = QString::fromStdWString(pi->p->shortname);
						qlPlugins << pi;
						loaded.insert(fname);
						pi->enabled = g.s.qmPositionalAudioPlugins.contains(fname) ? g.s.qmPositionalAudioPlugins[fname] : true;
						continue;
					}
				}
				pi->lib.unload();
			} else {
				qWarning("Plugins: Failed to load %s: %s", qPrintable(pi->filename), qPrintable(pi->lib.errorString()));
			}
			delete pi;
		}
	}
}

bool Plugins::fetch() {
	if (g.bPosTest) {
		fPosition[0] = fPosition[1] = fPosition[2] = 0.0f;
		fFront[0] = 0.0f;
		fFront[1] = 0.0f;
		fFront[2] = 1.0f;
		fTop[0] = 0.0f;
		fTop[1] = 1.0f;
		fTop[2] = 0.0f;

		for (int i=0;i<3;++i) {
			fCameraPosition[i] = fPosition[i];
			fCameraFront[i] = fFront[i];
			fCameraTop[i] = fTop[i];
		}

		bValid = true;
		return true;
	}

	if (! locked) {
		bValid = false;
		return bValid;
	}

	QMutexLocker lock(&qmPlugins);
	if (! locked) {
		bValid = false;
		return bValid;
	}

	if (!locked->enabled)
		bUnlink = true;

	bool ok = locked->p->fetch(fPosition, fFront, fTop, fCameraPosition, fCameraFront, fCameraTop, ssContext, swsIdentity);
	if (! ok || bUnlink) {
		locked->p->unlock();
		locked->locked = false;
		prevlocked = locked;
		locked = NULL;
		for (int i=0;i<3;i++)
			fPosition[i]=fFront[i]=fTop[i]=fCameraPosition[i]=fCameraFront[i]=fCameraTop[i] = 0.0f;
	}
	bValid = ok;
	return bValid;
}

void Plugins::on_Timer_timeout() {
	fetch();

	QMutexLocker lock(&qmPlugins);

	if (prevlocked) {
		g.l->log(Log::Information, tr("%1 lost link.").arg(prevlocked->shortname));
		prevlocked = NULL;
	}

	if (! locked) {
		ssContext.clear();
		swsIdentity.clear();
	}

	std::string context;
	if (locked)
		context.assign(u8(QString::fromStdWString(locked->p->shortname)) + static_cast<char>(0) + ssContext);

	if (! g.uiSession) {
		ssContextSent.clear();
		swsIdentitySent.clear();
	} else if ((context != ssContextSent) || (swsIdentity != swsIdentitySent)) {
		MumbleProto::UserState mpus;
		mpus.set_session(g.uiSession);
		if (context != ssContextSent) {
			ssContextSent.assign(context);
			mpus.set_plugin_context(context);
		}
		if (swsIdentity != swsIdentitySent) {
			swsIdentitySent.assign(swsIdentity);
			mpus.set_plugin_identity(u8(QString::fromStdWString(swsIdentitySent)));
		}
		g.sh->sendMessage(mpus);
	}

	if (locked) {
		return;
	}

	if (! g.s.bTransmitPosition)
		return;

	if (qlPlugins.isEmpty())
		return;

	++iPluginTry;
	if (iPluginTry >= qlPlugins.count())
		iPluginTry = 0;

	PluginInfo *pi = qlPlugins.at(iPluginTry);
	if (pi->enabled && pi->p->trylock()) {
		pi->shortname = QString::fromStdWString(pi->p->shortname);
		g.l->log(Log::Information, tr("%1 linked.").arg(pi->shortname));
		pi->locked = true;
		bUnlink = false;
		locked = pi;
	}
}

void Plugins::checkUpdates() {
	QUrl url(QLatin1String("http://mumble.info/plugins.php"));

	url.addQueryItem(QLatin1String("ver"), QLatin1String(QUrl::toPercentEncoding(QLatin1String(MUMBLE_RELEASE))));
#if defined(Q_OS_WIN)
	url.addQueryItem(QLatin1String("os"), QLatin1String("Win32"));
#elif defined(Q_OS_MAC)
	url.addQueryItem(QLatin1String("os"), QLatin1String("MacOSX"));
#else
	url.addQueryItem(QLatin1String("os"), QLatin1String("Unix"));
#endif

	QNetworkRequest req(url);
	QNetworkReply *rep = g.nam->get(req);
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
}

void Plugins::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());

	bool rescan = false;

	if (rep->error() == QNetworkReply::NoError) {
		const QString &path = rep->url().path();
		if (path == QLatin1String("/plugins.php")) {
			qmPluginHash.clear();
			QDomDocument doc;
			doc.setContent(rep->readAll());

			QDomElement root=doc.documentElement();
			QDomNode n = root.firstChild();
			while (!n.isNull()) {
				QDomElement e = n.toElement();
				if (!e.isNull()) {
					if (e.tagName() == QLatin1String("plugin")) {
						QString name = QFileInfo(e.attribute(QLatin1String("name"))).fileName();
						QString hash = e.attribute(QLatin1String("hash"));
						qmPluginHash.insert(name, hash);
					}
				}
				n = n.nextSibling();
			}

			QDir qd(qsSystemPlugins, QString(), QDir::Name, QDir::Files | QDir::Readable);
			QDir qdu(qsUserPlugins, QString(), QDir::Name, QDir::Files | QDir::Readable);

			QFileInfoList libs = qd.entryInfoList();
			foreach(const QFileInfo &libinfo, libs) {
				QString libname = libinfo.absoluteFilePath();
				QString filename = libinfo.fileName();
				QString wanthash = qmPluginHash.value(filename);
				if (! wanthash.isNull() && QLibrary::isLibrary(libname)) {
					QFile f(libname);
					if (wanthash.isEmpty()) {
						// Outdated plugin
						if (f.exists()) {
							clearPlugins();
							f.remove();
							rescan=true;
						}
					} else if (f.open(QIODevice::ReadOnly)) {
						QString h = QLatin1String(sha1(f.readAll()).toHex());
						f.close();
						if (h == wanthash) {
							if (qd != qdu) {
								QFile qfuser(qsUserPlugins + QString::fromLatin1("/") + filename);
								if (qfuser.exists()) {
									clearPlugins();
									qfuser.remove();
									rescan=true;
								}
							}
							// Mark for removal from userplugins
							qmPluginHash.insert(filename, QString());
						}
					}
				}
			}

			if (qd != qdu) {
				libs = qdu.entryInfoList();
				foreach(const QFileInfo &libinfo, libs) {
					QString libname = libinfo.absoluteFilePath();
					QString filename = libinfo.fileName();
					QString wanthash = qmPluginHash.value(filename);
					if (! wanthash.isNull() && QLibrary::isLibrary(libname)) {
						QFile f(libname);
						if (wanthash.isEmpty()) {
							// Outdated plugin
							if (f.exists()) {
								clearPlugins();
								f.remove();
								rescan=true;
							}
						} else if (f.open(QIODevice::ReadOnly)) {
							QString h = QLatin1String(sha1(f.readAll()).toHex());
							f.close();
							if (h == wanthash) {
								qmPluginHash.remove(filename);
							}
						}
					}
				}
			}
			QMap<QString, QString>::const_iterator i;
			for (i = qmPluginHash.constBegin(); i != qmPluginHash.constEnd(); ++i) {
				if (! i.value().isEmpty()) {
					QUrl url(QLatin1String("http://mumble.info/"));
					url.setPath(QString::fromLatin1("plugins/%1").arg(i.key()));

					QNetworkRequest req(url);
					QNetworkReply *r = g.nam->get(req);
					connect(r, SIGNAL(finished()), this, SLOT(finished()));
				}
			}
		} else {
			QString fname = QFileInfo(path).fileName();
			if (qmPluginHash.contains(fname)) {
				QByteArray qba = rep->readAll();
				if (qmPluginHash.value(fname) == QLatin1String(sha1(qba).toHex())) {
					bool verified = true;
#ifdef Q_OS_WIN
					verified = false;
					QString tempname, tempnative;
					{
						QTemporaryFile temp(QDir::tempPath() + QLatin1String("/plugin_XXXXXX.dll"));
						if (temp.open()) {
							tempname = temp.fileName();
							tempnative = QDir::toNativeSeparators(tempname);
							temp.write(qba);
							temp.setAutoRemove(false);
						}
					}
					if (! tempname.isNull()) {
						WINTRUST_FILE_INFO file;
						ZeroMemory(&file, sizeof(file));
						file.cbStruct = sizeof(file);
						file.pcwszFilePath = tempnative.utf16();

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

						HRESULT hr = WinVerifyTrust(0, &guid , &data);

						QFile deltemp(tempname);
						deltemp.remove();
						verified = (hr == 0);
					}
#endif
					if (verified) {
						clearPlugins();

						QFile f;
						f.setFileName(qsSystemPlugins + QLatin1String("/") + fname);
						if (f.open(QIODevice::WriteOnly)) {
							f.write(qba);
							f.close();
							g.mw->msgBox(tr("Downloaded new or updated plugin to %1.").arg(f.fileName()));
						} else {
							f.setFileName(qsUserPlugins + QLatin1String("/") + fname);
							if (f.open(QIODevice::WriteOnly)) {
								f.write(qba);
								f.close();
								g.mw->msgBox(tr("Downloaded new or updated plugin to %1.").arg(f.fileName()));
							} else {
								g.mw->msgBox(tr("Failed to install new plugin to %1.").arg(f.fileName()));
							}
						}

						rescan=true;
					}
				}
			}
		}

	}

	if (rescan)
		rescanPlugins();

	rep->deleteLater();
}
