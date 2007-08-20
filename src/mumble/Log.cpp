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

static ConfigWidget *LogConfigDialogNew(Settings &st) {
	return new LogConfig(st);
}

static ConfigRegistrar registrar(40, LogConfigDialogNew);

LogConfig::LogConfig(Settings &st) : ConfigWidget(st) {
	QGroupBox *qgbMessages = new QGroupBox(tr("Messages"));
	QLabel *lab;

	QGridLayout *l=new QGridLayout();

	lab=new QLabel(tr("Console"));
	l->addWidget(lab, 0, 1);
	lab=new QLabel(tr("TTS"));
	l->addWidget(lab, 0, 2);

	for (int i=Log::firstMsgType;i<=Log::lastMsgType;++i) {
		QCheckBox *qcb;
		Log::MsgType t=static_cast<Log::MsgType>(i);
		QString name = g.l->msgName(t);

		lab=new QLabel(name);
		l->addWidget(lab, i+1, 0);

		qcb=new QCheckBox();
		qcb->setToolTip(tr("Enable console for %1").arg(name));
		qlConsole << qcb;
		l->addWidget(qcb, i+1, 1);

		qcb=new QCheckBox();
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
	qsVolume->setObjectName(QLatin1String("Volume"));
	qsVolume->setToolTip(tr("Volume of Text-To-Speech Engine"));
	qsVolume->setWhatsThis(tr("<b>This is the volume used for the speech synthesis.</b>"));
	l->addWidget(qsVolume,0,1);

	lab=new QLabel(tr("Length threshold"));
	l->addWidget(lab,1,0);
	qsbThreshold = new QSpinBox();
	qsbThreshold->setRange(0, 5000);
	qsbThreshold->setSingleStep(10);
	qsbThreshold->setObjectName(QLatin1String("Threshold"));
	qsbThreshold->setToolTip(tr("Message length threshold for Text-To-Speech Engine"));
	qsbThreshold->setWhatsThis(tr("<b>This is the length threshold used for the Text-To-Speech Engine.</b><br />"
	                              "Messages longer than this limit will not be read aloud in their full length."));
	l->addWidget(qsbThreshold,1,1);

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
	return QIcon(QLatin1String("skin:config_msgs.png"));
}

void LogConfig::load(const Settings &r) {
	for (int i=Log::firstMsgType;i<=Log::lastMsgType;++i) {
		Settings::MessageLog ml = static_cast<Settings::MessageLog>(r.qmMessages.value(i));
		qlConsole[i]->setCheckState((ml & Settings::LogConsole) ? Qt::Checked : Qt::Unchecked);
		qlTTS[i]->setCheckState((ml & Settings::LogTTS) ? Qt::Checked : Qt::Unchecked);
	}

	qsVolume->setValue(r.iTTSVolume);
	qsbThreshold->setValue(r.iTTSThreshold);
}

void LogConfig::save() const {
	for (int i=Log::firstMsgType;i<=Log::lastMsgType;++i) {
		int v = 0;
		if (qlConsole[i]->checkState() == Qt::Checked)
			v |= Settings::LogConsole;
		if (qlTTS[i]->checkState() == Qt::Checked)
			v |= Settings::LogTTS;
		s.qmMessages[i] = v;
	}
	s.iTTSVolume=qsVolume->value();
	s.iTTSThreshold=qsbThreshold->value();
}

void LogConfig::accept() const {
	g.l->tts->setVolume(s.iTTSVolume);
}

Log::Log(QObject *p) : QObject(p) {
	tts=new TextToSpeech(this);
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
	QT_TR_NOOP("Permission Denied"),
	QT_TR_NOOP("Text Message")
};

QString Log::msgName(MsgType t) const {
	return tr(msgNames[t]);
}

void Log::setIgnore(MsgType t, int ignore) {
	qmIgnore.insert(t, ignore);
}

void Log::clearIgnore() {
	qmIgnore.clear();
}

void Log::log(MsgType mt, const QString &console, const QString &terse) {
	QTime now = QTime::currentTime();

	int ignore = qmIgnore.value(mt);
	if (ignore) {
		ignore--;
		qmIgnore.insert(mt, ignore);
		return;
	}

	QTextDocument t;
	t.setUndoRedoEnabled(false);
	t.setHtml(console);
	QString plain = t.toPlainText();

	quint32 flags = g.s.qmMessages.value(mt);

	if ((flags & Settings::LogConsole)) {
		QTextCursor tc=g.mw->qteLog->textCursor();
		tc.movePosition(QTextCursor::End);
		if (plain.contains(QRegExp(QLatin1String("[\\r\\n]")))) {
		        QTextFrameFormat qttf;
		        qttf.setBorder(1);
		        qttf.setPadding(2);
		        qttf.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
		        tc.insertFrame(qttf);
		        tc.insertHtml(QString::fromLatin1("[%2] %1\n").arg(console).arg(now.toString(Qt::LocalDate)));
		} else {
			tc.insertBlock();
			tc.insertHtml(QString::fromLatin1("[%2] %1\n").arg(console).arg(now.toString(Qt::LocalDate)));
		}
		tc.movePosition(QTextCursor::End);
		g.mw->qteLog->setTextCursor(tc);
		g.mw->qteLog->ensureCursorVisible();
	}

	if (! g.s.bTTS || ! (flags & Settings::LogTTS))
		return;

	/* TTS threshold limiter. */
	if (plain.length() <= g.s.iTTSThreshold)
		tts->say(plain);
	else if ((! terse.isEmpty()) && (terse.length() <= g.s.iTTSThreshold))
		tts->say(terse);
}
