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
#include "../../plugins/mumble_plugin.h"
#include "Global.h"

static ConfigWidget *PluginConfigDialogNew(Settings &st) {
	return new PluginConfig(st);
}

static ConfigRegistrar registrar(5000, PluginConfigDialogNew);

struct PluginInfo {
	bool locked;
	QLibrary lib;
	QString filename;
	QString description;
	QString shortname;
	MumblePlugin *p;
	PluginInfo();
};

PluginInfo::PluginInfo() {
	locked = false;
	p = NULL;
}

PluginConfig::PluginConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

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
}

bool PluginConfig::expert(bool) {
	return false;
}

void PluginConfig::on_qpbConfig_clicked() {
	QListWidgetItem *i = qlwPlugins->currentItem();
	if (!i)
		return;
	PluginInfo *pi=qhInfos[i];

	if (pi->p->config)
		pi->p->config(winId());
	else
		QMessageBox::information(this, QLatin1String("Mumble"), tr("Plugin has no configure function."), QMessageBox::Ok, QMessageBox::NoButton);
}

void PluginConfig::on_qpbAbout_clicked() {
	QListWidgetItem *i = qlwPlugins->currentItem();
	if (!i)
		return;
	PluginInfo *pi=qhInfos[i];

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
	qlwPlugins->clear();
	qhInfos.clear();
	PluginInfo *pi;
	foreach(pi, g.p->qlPlugins) {
		QListWidgetItem *i = new QListWidgetItem(pi->description, qlwPlugins);
		i->setData(Qt::ToolTipRole, pi->lib.fileName());
		qhInfos[i]=pi;
	}
}

Plugins::Plugins(QObject *p) : QObject(p) {
	QTimer *timer=new QTimer(this);
	timer->setObjectName(QLatin1String("Timer"));
	timer->start(1000);
	locked = prevlocked = NULL;
	bValid = false;
	for (int i=0;i<3;i++)
		fPosition[i]=fFront[i]=fTop[i]= 0.0;
	QMetaObject::connectSlotsByName(this);
}

void Plugins::rescanPlugins() {
	QMutexLocker lock(&qmPlugins);
	PluginInfo *pi;
	foreach(pi, qlPlugins) {
		if (pi->locked)
			pi->p->unlock();
		pi->lib.unload();
		delete pi;
	}
	qlPlugins.clear();
	prevlocked = locked = NULL;
	bValid = false;

#ifndef PLUGIN_PATH
#ifndef Q_OS_MAC
	const QString path=QString::fromLatin1("%1/plugins").arg(qApp->applicationDirPath());
#else
	const QString path=QString::fromLatin1("%1/../Plugins").arg(qApp->applicationDirPath());
#endif
#else
	const QString path=QLatin1String(MUMTEXT(PLUGIN_PATH));
#endif
	QDir qd(path,QString(), QDir::Name, QDir::Files | QDir::Readable);
	QFileInfoList libs = qd.entryInfoList();
	foreach(const QFileInfo &libinfo, libs) {
		QString libname = libinfo.absoluteFilePath();
		if (QLibrary::isLibrary(libname)) {
			pi = new PluginInfo();
			pi->filename = libname;
			pi->lib.setFileName(pi->filename);
			if (pi->lib.load()) {
				mumblePluginFunc mpf = reinterpret_cast<mumblePluginFunc>(pi->lib.resolve("getMumblePlugin"));
				if (mpf) {
					pi->p = mpf();
					if (pi->p && (pi->p->magic == MUMBLE_PLUGIN_MAGIC)) {
						pi->description=QString::fromStdWString(pi->p->description);
						pi->shortname=QString::fromStdWString(pi->p->shortname);
						qlPlugins << pi;
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
	float dummy_pos[3], dummy_front[3], dummy_top[3];
	std::string dummy_context;
	std::wstring dummy_identity;
	bool ok=locked->p->fetch(fPosition, dummy_front, dummy_top, dummy_pos, fFront, fTop, dummy_context, dummy_identity);
	if (! ok || bUnlink) {
		locked->p->unlock();
		locked->locked = false;
		prevlocked = locked;
		locked = NULL;
		for (int i=0;i<3;i++)
			fPosition[i]=fFront[i]=fTop[i]= 0.0f;
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

	if (locked)
		return;

	if (! g.s.bTransmitPosition)
		return;

#ifdef Q_OS_WIN
	// According to MS KB Q131065, we need this to OpenProcess()

	HANDLE hToken = NULL;

	if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken)) {
		if (GetLastError() == ERROR_NO_TOKEN) {
			ImpersonateSelf(SecurityImpersonation);
			OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken);
		}
	}

	TOKEN_PRIVILEGES tp;
	LUID luid;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpPrevious, &cbPrevious);
#endif

	foreach(PluginInfo *pi, qlPlugins) {
		if (pi->p->trylock()) {
			pi->shortname = QString::fromStdWString(pi->p->shortname);
			g.l->log(Log::Information, tr("%1 linked.").arg(pi->shortname));
			pi->locked = true;
			bUnlink = false;
			locked = pi;
			break;
		}
	}

#ifdef Q_OS_WIN
	AdjustTokenPrivileges(hToken, FALSE, &tpPrevious, cbPrevious, NULL, NULL);
	CloseHandle(hToken);
#endif
}
