/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#ifdef Q_OS_MAC
extern bool qt_mac_execute_apple_script(const QString &script, AEDesc *ret);
#endif

static ConfigWidget *LogConfigDialogNew(Settings &st) {
	return new LogConfig(st);
}

static ConfigRegistrar registrar(4000, LogConfigDialogNew);

LogConfig::LogConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

	
	qtwMessages->header()->setResizeMode(ColMessage, QHeaderView::Stretch);
	qtwMessages->header()->setResizeMode(ColConsole, QHeaderView::ResizeToContents);
	qtwMessages->header()->setResizeMode(ColNotification, QHeaderView::ResizeToContents);
	qtwMessages->header()->setResizeMode(ColTTS, QHeaderView::ResizeToContents);
	qtwMessages->header()->setResizeMode(ColStaticSound, QHeaderView::ResizeToContents);
	//qtwMessages->setColumnHidden(ColStaticSoundPath, true);

	QTreeWidgetItem *twi;
	for (int i = Log::firstMsgType; i <= Log::lastMsgType; ++i) {
		Log::MsgType t = static_cast<Log::MsgType>(i);
		const QString messageName = g.l->msgName(t);

		twi = new QTreeWidgetItem(qtwMessages);
		twi->setData(ColMessage, Qt::UserRole, i);
		twi->setText(ColMessage, messageName);
		twi->setCheckState(ColConsole, Qt::Unchecked);
		twi->setCheckState(ColNotification, Qt::Unchecked);
		twi->setCheckState(ColTTS, Qt::Unchecked);
		twi->setCheckState(ColStaticSound, Qt::Unchecked);
		twi->setToolTip(ColConsole, tr("Enable console for %1").arg(messageName));
		twi->setToolTip(ColNotification, tr("Enable ballon tool-tip for %1").arg(messageName));
		twi->setToolTip(ColTTS, tr("Enable Text-To-Speech for %1").arg(messageName));
		twi->setToolTip(ColStaticSound, tr("Enable static sound for %1").arg(messageName));

	}
	on_qtwMessages_itemSelectionChanged();
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
		Log::MsgType mt = static_cast<Log::MsgType>(i->data(ColMessage, Qt::UserRole).toInt());
		Settings::MessageLog ml = static_cast<Settings::MessageLog>(r.qmMessages.value(mt));

		i->setCheckState(ColConsole, (ml & Settings::LogConsole) ? Qt::Checked : Qt::Unchecked);
		i->setCheckState(ColNotification, (ml & Settings::LogBalloon) ? Qt::Checked : Qt::Unchecked);
		i->setCheckState(ColTTS, (ml & Settings::LogTTS) ? Qt::Checked : Qt::Unchecked);
		i->setCheckState(ColStaticSound, (ml & Settings::LogSoundfile) ? Qt::Checked : Qt::Unchecked);
		i->setText(ColStaticSoundPath, r.qmMessageSounds.value(mt));
	}

	loadSlider(qsVolume, r.iTTSVolume);
	qsbThreshold->setValue(r.iTTSThreshold);
}

void LogConfig::save() const {
	QList<QTreeWidgetItem *> qlItems = qtwMessages->findItems(QString(), Qt::MatchContains);
	foreach(QTreeWidgetItem *i, qlItems) {
		Log::MsgType mt = static_cast<Log::MsgType>(i->data(ColMessage, Qt::UserRole).toInt());

		int v = 0;
		if (i->checkState(ColConsole) == Qt::Checked)
			v |= Settings::LogConsole;
		if (i->checkState(ColNotification) == Qt::Checked)
			v |= Settings::LogBalloon;
		if (i->checkState(ColTTS) == Qt::Checked)
			v |= Settings::LogTTS;
		if (i->checkState(ColStaticSound) == Qt::Checked)
			v |= Settings::LogSoundfile;
		s.qmMessages[mt] = v;
		s.qmMessageSounds[mt] = i->text(ColStaticSoundPath);
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

void LogConfig::on_qtwMessages_itemSelectionChanged() {
	QList<QTreeWidgetItem *> qlSelected = qtwMessages->selectedItems();
	if (qlSelected.empty()) {
		// Nothing selected
		qcbConsole->setEnabled(false);
		qcbNotification->setEnabled(false);
		qrbTTS->setEnabled(false);
		qrbSoundfile->setEnabled(false);
		qrbOff->setEnabled(false);
		qlePath->setText("");
		qlePath->setEnabled(false);
		qpbBrowse->setEnabled(false);
		qpbPlay->setEnabled(false);
	}
	else {
		QTreeWidgetItem *i = qlSelected[0];

		qcbConsole->setEnabled(true);
		qcbNotification->setEnabled(true);
		qrbTTS->setEnabled(true);
		qrbSoundfile->setEnabled(true);
		qrbOff->setEnabled(true);
		qlePath->setEnabled(true);
		qpbBrowse->setEnabled(true);
		qpbPlay->setEnabled(true);

		qcbConsole->setChecked(i->checkState(ColConsole));
		qcbNotification->setChecked(i->checkState(ColNotification));
		if(i->checkState(ColTTS)) qrbTTS->setChecked(true);
		else if(i->checkState(ColStaticSound)) qrbSoundfile->setChecked(true);
		else qrbOff->setChecked(true);
		qlePath->setText(i->text(ColStaticSoundPath));
	}
}

void LogConfig::on_qtwMessages_itemChanged(QTreeWidgetItem* i, int column) {
	if (! i->isSelected()) return;
	switch (column) {
		case ColConsole: qcbConsole->setChecked(i->checkState(column)); break;
		case ColNotification: qcbNotification->setChecked(i->checkState(column)); break;
		case ColTTS:
		case ColStaticSound:
			if(i->checkState(ColTTS) && column == ColTTS) {
				i->setCheckState(ColStaticSound, Qt::Unchecked);
				qrbTTS->setChecked(true);
			}
			else if(i->checkState(ColStaticSound)) {
				i->setCheckState(ColTTS, Qt::Unchecked);
				qrbSoundfile->setChecked(true);
			}
			else qrbOff->setChecked(true);
			break;
		case ColStaticSoundPath: qlePath->setText(i->text(ColStaticSoundPath)); break;
		default:break;
	}		
}

void LogConfig::on_qpbPlay_clicked() {
	QString file = qlePath->text();
	if (! file.isEmpty() && g.ao) {
		(g.ao)->playSample(file, false);
	}
}

void LogConfig::on_qpbBrowse_clicked() {
	QString file = QFileDialog::getOpenFileName(this, tr("Choose sound file"), QString(), QLatin1String("*.spx"));
	if (! file.isEmpty()) {
		QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
		i->setText(ColStaticSoundPath, file);
		i->setCheckState(ColStaticSound, Qt::Checked);

	}
}

void LogConfig::on_qlePath_editingFinished() {
	QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
	i->setText(ColStaticSoundPath, qlePath->text());
	i->setCheckState(ColStaticSound, Qt::Checked);
}

void LogConfig::on_qcbConsole_toggled(bool checked) {
	QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
	i->setCheckState(ColConsole, checked ? Qt::Checked : Qt::Unchecked);
}

void LogConfig::on_qcbNotification_toggled(bool checked) {
	QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
	i->setCheckState(ColNotification, checked ? Qt::Checked : Qt::Unchecked);
}

void LogConfig::on_qrbTTS_toggled(bool checked) {
	QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
	i->setCheckState(ColTTS, checked ? Qt::Checked : Qt::Unchecked);
}

void LogConfig::on_qrbSoundfile_toggled(bool checked) {
	QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
	i->setCheckState(ColStaticSound, checked ? Qt::Checked : Qt::Unchecked);
}

void LogConfig::on_qrbOff_toggled(bool checked) {
	if (checked) {
		QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
		i->setCheckState(ColTTS, Qt::Unchecked);
		i->setCheckState(ColStaticSound, Qt::Unchecked);
	}
}

Log::Log(QObject *p) : QObject(p) {
	tts=new TextToSpeech(this);
	tts->setVolume(g.s.iTTSVolume);
	uiLastId = 0;

#ifdef Q_OS_MAC
	QStringList qslAllEvents;
	for (int i = Log::firstMsgType; i <= Log::lastMsgType; ++i) {
		Log::MsgType t = static_cast<Log::MsgType>(i);
		qslAllEvents << QString("\"%1\"").arg(g.l->msgName(t));
	}
	QString qsGrowlEvents = QString("{%1}").arg(qslAllEvents.join(","));
	QString qsScript = QString(
		"tell application \"System Events\"\n"
		"	set isRunning to count of (every process whose name is \"GrowlHelperApp\") > 0\n"
		"end tell\n"
		"if isRunning then\n"
		"	tell application \"GrowlHelperApp\"\n"
		"		set the allNotificationsList to %1\n"
		"		set the enabledNotificationsList to %1\n"
		"		register as application \"Mumble\""
		"			all notifications allNotificationsList"
		"			default notifications enabledNotificationsList"
		"			icon of application \"Mumble\"\n"
		"	end tell\n"
		"end if\n").arg(qsGrowlEvents);
	qt_mac_execute_apple_script(qsScript, NULL);
#endif
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
	QT_TR_NOOP("Text Message"),
	QT_TR_NOOP("Script Errors")
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

	const QString plain = QTextDocumentFragment::fromHtml(console).toPlainText();

	quint32 flags = g.s.qmMessages.value(mt);

	// Message output on console
	if ((flags & Settings::LogConsole)) {
		QTextCursor tc=g.mw->qteLog->textCursor();
		tc.movePosition(QTextCursor::End);
		if (plain.contains(QRegExp(QLatin1String("[\\r\\n]")))) {
			QTextFrameFormat qttf;
			qttf.setBorder(1);
			qttf.setPadding(2);
			qttf.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
			tc.insertFrame(qttf);
		} else if (! g.mw->qteLog->document()->isEmpty()) {
			tc.insertBlock();
		}
		tc.insertHtml(QString::fromLatin1("[%2] %1\n").arg(console).arg(now.toString(Qt::LocalDate)));
		tc.movePosition(QTextCursor::End);
		g.mw->qteLog->setTextCursor(tc);
		g.mw->qteLog->ensureCursorVisible();
	}

	// Message notification with balloon tooltips
	if ((flags & Settings::LogBalloon) && !(g.mw->isActiveWindow() && g.mw->qdwLog->isVisible()))  {
		QString qsIcon;
		switch (mt) {
			case DebugInfo:
			case CriticalError:
				qsIcon=QLatin1String("gtk-dialog-error");
				break;
			case Warning:
				qsIcon=QLatin1String("gtk-dialog-warning");
				break;
			case TextMessage:
				qsIcon=QLatin1String("gtk-edit");
				break;
			default:
				qsIcon=QLatin1String("gtk-dialog-info");
				break;
		}

		QDBusMessage response;
		QVariantMap hints;
		hints.insert(QLatin1String("desktop-entry"), QLatin1String("mumble"));


		{
			QDBusInterface kde(QLatin1String("org.kde.VisualNotifications"), QLatin1String("/VisualNotifications"), QLatin1String("org.kde.VisualNotifications"));
			if (kde.isValid()) {
				QList<QVariant> args;
				args.append(QLatin1String("mumble"));
				args.append(uiLastId);
				args.append(QString());
				args.append(QLatin1String("mumble"));
				args.append(msgName(mt));
				args.append(console);
				args.append(QStringList());
				args.append(hints);
				args.append(5000);

				response = kde.callWithArgumentList(QDBus::AutoDetect, QLatin1String("Notify"), args);
			}
		}

		if (response.type()!=QDBusMessage::ReplyMessage || response.arguments().at(0).toUInt()==0) {
			QDBusInterface gnome(QLatin1String("org.freedesktop.Notifications"), QLatin1String("/org/freedesktop/Notifications"), QLatin1String("org.freedesktop.Notifications"));
			if (gnome.isValid())
				response = gnome.call(QLatin1String("Notify"), QLatin1String("Mumble"), uiLastId, qsIcon, msgName(mt), console, QStringList(), hints, -1);
		}

		if (response.type()==QDBusMessage::ReplyMessage && response.arguments().count() == 1) {
			uiLastId = response.arguments().at(0).toUInt();
		} else {
			if (g.mw->qstiIcon->isSystemTrayAvailable() && g.mw->qstiIcon->supportsMessages()) {
				QSystemTrayIcon::MessageIcon msgIcon;

				switch (mt) {
					case DebugInfo:
					case CriticalError:
						msgIcon=QSystemTrayIcon::Critical;
						break;
					case Warning:
						msgIcon=QSystemTrayIcon::Warning;
						break;
					default:
						msgIcon=QSystemTrayIcon::Information;
						break;
				}

				g.mw->qstiIcon->showMessage(msgName(mt), console, msgIcon);
			}
		}
#ifdef Q_OS_MAC
	QString qsScript = QString(
		"tell application \"System Events\"\n"
		"	set isRunning to count of (every process whose name is \"GrowlHelperApp\") > 0\n"
		"end tell\n"
		"if isRunning then\n"
		"	tell application \"GrowlHelperApp\"\n"
		"		notify with name \"%1\" title \"%1\" description \"%2\" application name \"Mumble\"\n"
		"	end tell\n"
		"end if\n").arg(msgName(mt)).arg(console);
	qt_mac_execute_apple_script(qsScript, NULL);
#endif
	}

	// Message notification with static sounds
	if ((flags & Settings::LogSoundfile) && g.ao) {
		QString sSound = g.s.qmMessageSounds.value(mt, QString(""));
		if (sSound != "") {
			(g.ao)->playSample(sSound, false);
		}
	}

	// Message notification with Text-To-Speech
	if (! g.s.bTTS || !(flags & Settings::LogTTS))
		return;

	/* TTS threshold limiter. */
	if (plain.length() <= g.s.iTTSThreshold)
		tts->say(plain);
	else if ((! terse.isEmpty()) && (terse.length() <= g.s.iTTSThreshold))
		tts->say(terse);
}

LogDocument::LogDocument(QObject *p) : QTextDocument(p) {
}

QVariant LogDocument::loadResource(int type, const QUrl &url) {
	if ((type != QTextDocument::ImageResource))
		return QLatin1String("No external resources allowed.");

	QImage qi(0, 0, QImage::Format_Mono);

	addResource(type, url, qi);

	if (! url.isValid() || url.isRelative())
		return qi;

	if ((url.scheme() != QLatin1String("http")) && (url.scheme() != QLatin1String("https")))
		return qi;

	qWarning() << "LogDocument::loadResource " << url.toString();

	QNetworkRequest req(url);
	QNetworkReply *rep = g.nam->get(req);
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
	return qi;
}

void LogDocument::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());

	if (rep->error() == QNetworkReply::NoError) {
		QVariant qv = rep->readAll();
		addResource(QTextDocument::ImageResource, rep->request().url(), qv);
		g.mw->qteLog->setDocument(this);
	}
	rep->deleteLater();
}
