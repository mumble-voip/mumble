/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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
#include "TextToSpeech.h"
#include "MainWindow.h"
#include "Log.h"
#include "Global.h"

static ConfigWidget *LogConfigDialogNew() {
	return new LogConfig();
}

static ConfigRegistrar registrar(40, LogConfigDialogNew);

LogConfig::LogConfig(QWidget *p) : ConfigWidget(p) {
	QGroupBox *qgbMessages = new QGroupBox(tr("Messages"));
	QLabel *lab;

	QGridLayout *l=new QGridLayout();

	lab=new QLabel(tr("Console"));
	l->addWidget(lab, 0, 1);
	lab=new QLabel(tr("TTS"));
	l->addWidget(lab, 0, 2);

	for(int i=Log::firstMsgType;i<=Log::lastMsgType;++i) {
		QCheckBox *qcb;
		Log::MsgType t=static_cast<Log::MsgType>(i);
		MsgSettings *ms=g.l->qhSettings[t];
		QString name = g.l->msgName(t);

		lab=new QLabel(name);
		l->addWidget(lab, i+1, 0);

		qcb=new QCheckBox();
		qcb->setCheckState(ms->bConsole ? Qt::Checked : Qt::Unchecked);
		qcb->setToolTip(tr("Enable console for %1").arg(name));
		qlConsole << qcb;
		l->addWidget(qcb, i+1, 1);

		qcb=new QCheckBox();
		qcb->setCheckState(ms->bTTS ? Qt::Checked : Qt::Unchecked);
		qcb->setToolTip(tr("Enable Text-To-Speech for %1").arg(name));
		qlTTS << qcb;
		l->addWidget(qcb, i+1, 2);
	}

	qgbMessages->setLayout(l);

	QGroupBox *qgbTTS = new QGroupBox(tr("Text To Speech"));
	l = new QGridLayout();

	lab=new QLabel(tr("Volume"));
	l->addWidget(lab,0,0);
	qsVolume = new QSlider(Qt::Horizontal);
	qsVolume->setRange(0, 100);
	qsVolume->setSingleStep(5);
	qsVolume->setPageStep(20);
	qsVolume->setValue(g.s.iTTSVolume);
	qsVolume->setObjectName(QString::fromAscii("Volume"));
	qsVolume->setToolTip(tr("Volume of Text-To-Speech Engine"));
	qsVolume->setWhatsThis(tr("<b>This is the volume used for the speech synthesis.</b>"));
	l->addWidget(qsVolume,0,1);

	qgbTTS->setLayout(l);


    QVBoxLayout *v = new QVBoxLayout;
    v->addWidget(qgbMessages);
    v->addWidget(qgbTTS);
    v->addStretch(1);
    setLayout(v);

    QMetaObject::connectSlotsByName(this);
}

QString LogConfig::title() const {
	return tr("Messages");
}

QIcon LogConfig::icon() const {
	return QIcon(QString::fromAscii(":/config_msgs.png"));
}

void LogConfig::accept() {
	for(int i=Log::firstMsgType;i<=Log::lastMsgType;++i) {
		Log::MsgType t=static_cast<Log::MsgType>(i);
		MsgSettings *ms=g.l->qhSettings[t];
		ms->bConsole = (qlConsole[i]->checkState() == Qt::Checked);
		ms->bTTS = (qlTTS[i]->checkState() == Qt::Checked);
	}
	g.s.iTTSVolume=qsVolume->value();
	g.l->tts->setVolume(g.s.iTTSVolume);
}

MsgSettings::MsgSettings() {
	bConsole = true;
	bTTS = true;
	iIgnore = 0;
}

Log::Log(QObject *p) : QObject(p) {
	for(int i=firstMsgType;i<=lastMsgType;++i)
		qhSettings[static_cast<MsgType>(i)]=new MsgSettings();
	tts=new TextToSpeech(this);
	loadSettings();
	tts->setVolume(g.s.iTTSVolume);
}

const char *Log::msgNames[] = {
	QT_TR_NOOP("Debug"),
	QT_TR_NOOP("Critical"),
	QT_TR_NOOP("Warning"),
	QT_TR_NOOP("Information"),
	QT_TR_NOOP("Server Connected"),
	QT_TR_NOOP("Server Disconnected"),
	QT_TR_NOOP("Player Joined Server"),
	QT_TR_NOOP("Player Left Server"),
	QT_TR_NOOP("Player kicked (you or by you)"),
	QT_TR_NOOP("Player kicked"),
	QT_TR_NOOP("You self-muted/deafened"),
	QT_TR_NOOP("Other self-muted/deafened"),
	QT_TR_NOOP("Player muted (you)"),
	QT_TR_NOOP("Player muted (by you)"),
	QT_TR_NOOP("Player muted (other)"),
	QT_TR_NOOP("Player Joined Channel"),
	QT_TR_NOOP("Player Left Channel"),
	QT_TR_NOOP("Permission Denied")
};

QString Log::msgName(MsgType t) const {
	return tr(msgNames[t]);
}

void Log::setIgnore(MsgType t, int ignore) {
	MsgSettings *ms=qhSettings.value(t);
	Q_ASSERT(ms);
	ms->iIgnore=ignore;
}

void Log::clearIgnore() {
	foreach(MsgSettings *ms, qhSettings)
		ms->iIgnore=0;
}

void Log::loadSettings() {
	for(int i=firstMsgType;i<=lastMsgType;++i) {
		MsgType t=static_cast<MsgType>(i);
		MsgSettings *ms=qhSettings[t];
		QString key=QString::fromAscii("msg%1").arg(QString::fromAscii(msgNames[t]));
		key = key.remove(QRegExp(QString::fromAscii("[ /\\(\\)-]")));
		int v = g.qs->value(key,0xff).toInt();
		ms->bConsole=(v & 0x01);
		ms->bTTS=(v & 0x02);
	}
}

void Log::saveSettings() const {
	for(int i=firstMsgType;i<=lastMsgType;++i) {
		MsgType t=static_cast<MsgType>(i);
		MsgSettings *ms=qhSettings[t];
		QString key=QString::fromAscii("msg%1").arg(QString::fromAscii(msgNames[t]));
		key = key.remove(QRegExp(QString::fromAscii("[ /\\(\\)-]")));
		int v = 0;
		if (ms->bConsole)
			v |= 0x01;
		if (ms->bTTS)
			v |= 0x02;
		g.qs->setValue(key, v);
	}
}

void Log::log(MsgType t, QString console, QString phonetic) {
	QTime now = QTime::currentTime();
	MsgSettings *ms=qhSettings.value(t);
	Q_ASSERT(ms);

	if (ms->iIgnore) {
		ms->iIgnore--;
		return;
	}

	if (ms->bConsole)
		g.mw->appendLog(tr("[%2] %1").arg(console).arg(now.toString(Qt::LocalDate)));
	if (! g.s.bTTS || ! ms->bTTS || (! phonetic.isNull() && phonetic.isEmpty()))
		return;
	tts->say(phonetic.isNull() ? console : phonetic);
}

