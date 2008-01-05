/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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

#include "Log.h"
#include "TextToSpeech.h"
#include "MainWindow.h"
#include "Global.h"

static ConfigWidget *LogConfigDialogNew(Settings &st) {
	return new LogConfig(st);
}

static ConfigRegistrar registrar(40, LogConfigDialogNew);

LogConfig::LogConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	qtwMessages->header()->setResizeMode(0, QHeaderView::Stretch);
	qtwMessages->header()->setResizeMode(1, QHeaderView::ResizeToContents);
	qtwMessages->header()->setResizeMode(2, QHeaderView::ResizeToContents);

	QTreeWidgetItem *twi;
	for (int i = Log::firstMsgType; i <= Log::lastMsgType; ++i) {
		Log::MsgType t = static_cast<Log::MsgType>(i);
		const QString messageName = g.l->msgName(t);

		twi = new QTreeWidgetItem(qtwMessages);
		twi->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
		twi->setData(0, Qt::UserRole, i);
		twi->setText(0, messageName);
		twi->setCheckState(1, Qt::Unchecked);
		twi->setCheckState(2, Qt::Unchecked);
		twi->setToolTip(1, tr("Enable console for %1").arg(messageName));
		twi->setToolTip(2, tr("Enable Text-To-Speech for %1").arg(messageName));
	}
}

QString LogConfig::title() const {
	return windowTitle();
}

QIcon LogConfig::icon() const {
	return QIcon(QLatin1String("skin:config_msgs.png"));
}

void LogConfig::load(const Settings &r) {
	QList<QTreeWidgetItem *> qlItems = qtwMessages->findItems(QString(), Qt::MatchContains);
	foreach(QTreeWidgetItem *i, qlItems) {
		Log::MsgType mt = static_cast<Log::MsgType>(i->data(0, Qt::UserRole).toInt());
		Settings::MessageLog ml = static_cast<Settings::MessageLog>(r.qmMessages.value(mt));

		i->setCheckState(1, (ml & Settings::LogConsole) ? Qt::Checked : Qt::Unchecked);
		i->setCheckState(2, (ml & Settings::LogTTS) ? Qt::Checked : Qt::Unchecked);
	}

	loadSlider(qsVolume, r.iTTSVolume);
	qsbThreshold->setValue(r.iTTSThreshold);
}

void LogConfig::save() const {
	QList<QTreeWidgetItem *> qlItems = qtwMessages->findItems(QString(), Qt::MatchContains);
	foreach(QTreeWidgetItem *i, qlItems) {
		Log::MsgType mt = static_cast<Log::MsgType>(i->data(0, Qt::UserRole).toInt());

		int v = 0;
		if (i->checkState(1) == Qt::Checked)
			v |= Settings::LogConsole;
		if (i->checkState(2) == Qt::Checked)
			v |= Settings::LogTTS;
		s.qmMessages[mt] = v;
	}

	s.iTTSVolume=qsVolume->value();
	s.iTTSThreshold=qsbThreshold->value();
}

void LogConfig::accept() const {
	g.l->tts->setVolume(s.iTTSVolume);
}

bool LogConfig::expert(bool) {
	return false;
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

	if (! g.s.bTTS || !(flags & Settings::LogTTS))
		return;

	/* TTS threshold limiter. */
	if (plain.length() <= g.s.iTTSThreshold)
		tts->say(plain);
	else if ((! terse.isEmpty()) && (terse.length() <= g.s.iTTSThreshold))
		tts->say(terse);
}
