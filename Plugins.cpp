/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QList>
#include <QTime>
#include <QSettings>
#include <QDir>
#include <QLibrary>
#include <QApplication>
#include <QTimer>
#include <QPushButton>
#include <QMessageBox>
#include <windows.h>
#include "Plugins.h"
#include "Log.h"
#include "plugins/mumble_plugin.h"
#include "Global.h"

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

PluginConfig::PluginConfig(QWidget *p) : ConfigWidget(p) {
	QGroupBox *qgbOptions = new QGroupBox(tr("Options"));
	QGroupBox *qgbPlugins = new QGroupBox(tr("Plugins"));
	QLabel *lab;
	QVBoxLayout *v;
	QHBoxLayout *h;
	QGridLayout *grid;

	grid=new QGridLayout();
	qcbTransmit = new QComboBox();

	qcbTransmit->addItem(tr("Nothing"), Settings::Nothing);
	qcbTransmit->addItem(tr("Position"), Settings::Position);
	qcbTransmit->addItem(tr("Position and Velocity"), Settings::PositionVelocity);
	qcbTransmit->setToolTip(tr("What information to transmit from plugins"));
	qcbTransmit->setWhatsThis(tr("This sets what information to transfer from the game to "
								"other players. <b>Position</b> enables them to hear your "
								"voice from the correct direction, and <b>Velocity</b> allows "
								"doppler effects as well (the weeeee-owwww sound a car makes just "
								"as it passes you)."));
	qcbTransmit->setCurrentIndex(static_cast<int>(g.s.ptTransmit));
	lab = new QLabel(tr("Transmit"));
	lab->setBuddy(qcbTransmit);
	grid->addWidget(lab, 0, 0);
	grid->addWidget(qcbTransmit, 0, 1);
	qgbOptions->setLayout(grid);

	qlwPlugins = new QListWidget();
	refillPluginList();

	QPushButton *reloadButton=new QPushButton("&Reload plugins");
	reloadButton->setObjectName("Reload");
	reloadButton->setToolTip(tr("Reloads all plugins"));
	reloadButton->setWhatsThis(tr("This rescans and reloads plugins. Use this if you just added or changed "
								"a plugin to the plugins directory."));
	QPushButton *aboutButton=new QPushButton("&About");
	aboutButton->setObjectName("About");
	aboutButton->setToolTip(tr("Information about plugin"));
	aboutButton->setWhatsThis(tr("This shows a small information message about the plugin."));
	QPushButton *configButton=new QPushButton("&Config");
	configButton->setObjectName("Config");
	configButton->setToolTip(tr("Show configuration page of plugin"));
	configButton->setWhatsThis(tr("This shows the configuration page of the plugin, if any."));

	h = new QHBoxLayout();
	h->addWidget(reloadButton);
	h->addStretch(1);
	h->addWidget(aboutButton);
	h->addWidget(configButton);

	v = new QVBoxLayout;
	v->addWidget(qlwPlugins);
	v->addLayout(h);
	qgbPlugins->setLayout(v);

    v = new QVBoxLayout;
    v->addWidget(qgbOptions);
    v->addWidget(qgbPlugins);
    v->addStretch(1);
    setLayout(v);

    QMetaObject::connectSlotsByName(this);
}

QString PluginConfig::title() const {
	return tr("Plugins");
}

QIcon PluginConfig::icon() const {
	return QIcon(":/icons/config_plugin.png");
}

void PluginConfig::accept() {
}

void PluginConfig::on_Config_clicked() {
	QListWidgetItem *i = qlwPlugins->currentItem();
	if (!i)
		return;
	PluginInfo *pi=qhInfos[i];

	if (pi->p->config)
		pi->p->config(winId());
	else
		QMessageBox::information(this, tr("Mumble"), tr("Plugin has no config function"), QMessageBox::Ok, QMessageBox::NoButton);
}

void PluginConfig::on_About_clicked() {
	QListWidgetItem *i = qlwPlugins->currentItem();
	if (!i)
		return;
	PluginInfo *pi=qhInfos[i];

	if (pi->p->about)
		pi->p->about(winId());
	else
		QMessageBox::information(this, tr("Mumble"), tr("Plugin has no about function"), QMessageBox::Ok, QMessageBox::NoButton);
}

void PluginConfig::on_Reload_clicked() {
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
	timer->setObjectName("Timer");
	timer->start(1000);
	locked = prevlocked = NULL;
	bValid = false;
	for(int i=0;i<3;i++)
		fPosition[i]=fVelocity[i]=fFront[i]=fTop[i]= 0.0;
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

#ifdef QT_NO_DEBUG
	QString path=QString("%1/plugins").arg(qApp->applicationDirPath());
#else
	QString path=QString("%1/../plugins").arg(qApp->applicationDirPath());
#endif

	QDir qd(path,"*.dll", QDir::Name, QDir::Files | QDir::Readable);
	QStringList libs = qd.entryList();
	foreach(QString libname, libs) {
		pi = new PluginInfo();
		pi->filename = QString("%1/%2").arg(path).arg(libname);
		pi->lib.setFileName(pi->filename);
		if (pi->lib.load()) {
			mumblePluginFunc mpf = (mumblePluginFunc)(pi->lib.resolve("getMumblePlugin"));
			if (mpf) {
				pi->p = mpf();
				if (pi->p) {
					pi->description=QString::fromUtf16(reinterpret_cast<const ushort *>(pi->p->description));
					pi->shortname=QString::fromUtf16(reinterpret_cast<const ushort *>(pi->p->shortname));
					qlPlugins << pi;
					continue;
				}
			}
			pi->lib.unload();
		}
		delete pi;
	}
}

void Plugins::fetch() {
	QMutexLocker lock(&qmPlugins);

	if (! locked) {
		bValid = bValidPos = bValidVel = false;
		return;
	}
	bool ok=locked->p->fetch(fPosition, fVelocity, fFront, fTop);
	if (! ok || bUnlink) {
		locked->p->unlock();
		locked->locked = false;
		prevlocked = locked;
		locked = NULL;
		for(int i=0;i<3;i++)
			fPosition[i]=fVelocity[i]=fFront[i]=fTop[i]= 0.0;
	}
	bValid = bValidPos = bValidVel = ok;
}

void Plugins::on_Timer_timeout() {
	QMutexLocker lock(&qmPlugins);

	if (prevlocked) {
		g.l->log(Log::Information,
				tr("Plugin %1 lost link.").arg(prevlocked->description),
				tr("%1 lost link.").arg(prevlocked->shortname));
		prevlocked = NULL;
	}

	if (locked)
		return;

	if (g.s.ptTransmit == Settings::Nothing)
		return;
	foreach(PluginInfo *pi, qlPlugins) {
		if (pi->p->trylock()) {
			g.l->log(Log::Information,
					tr("Plugin %1 linked.").arg(pi->description),
					tr("%1 linked.").arg(pi->shortname));
			pi->locked = true;
			bUnlink = false;
			locked = pi;
			return;
		}
	}
}
