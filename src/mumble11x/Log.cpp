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

#include "Log.h"
#include "TextToSpeech.h"
#include "MainWindow.h"
#include "Global.h"

#ifdef Q_OS_MAC
extern bool qt_mac_execute_apple_script(const QString &script, AEDesc *ret);

static bool growl_available(void) {
	OSStatus err = LSFindApplicationForInfo('GRRR', CFSTR("com.Growl.GrowlHelperApp"), CFSTR("GrowlHelperApp.app"), NULL, NULL);
	return err != kLSApplicationNotFoundErr;
}
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

		twi->setToolTip(ColConsole, tr("Toggle console for %1 events").arg(messageName));
		twi->setToolTip(ColNotification, tr("Toggle pop-up notifications for %1 events").arg(messageName));
		twi->setToolTip(ColTTS, tr("Toggle Text-To-Speech for %1 events").arg(messageName));
		twi->setToolTip(ColStaticSound, tr("Click here to toggle sound notification for %1 events").arg(messageName));
		twi->setToolTip(ColStaticSoundPath, tr("Path to soundfile used for sound notifications in the case of %1 events<br />Single click to play<br />Doubleclick to change").arg(messageName));

		twi->setWhatsThis(ColConsole, tr("Click here to toggle console output for %1 events.<br />If checked, this option makes Mumble output all %1 events in its message log.").arg(messageName));
		twi->setWhatsThis(ColNotification, tr("Click here to toggle pop-up notifications for %1 events.<br />If checked, a notification pop-up will be created by Mumble for every %1 event.").arg(messageName));
		twi->setWhatsThis(ColTTS, tr("Click here to toggle Text-To-Speech for %1 events.<br />If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for soundfiles. Text-To-Speech and soundfiles cannot be used at the same time.").arg(messageName));
		twi->setWhatsThis(ColStaticSound, tr("Click here to toggle sound notification for %1 events.<br />If checked, Mumble uses a soundfile predefined by you to indicate %1 events. Soundfiles and Text-To-Speech cannot be used at the same time.").arg(messageName));
		twi->setWhatsThis(ColStaticSoundPath, tr("Path to soundfile used for sound notifications in the case of %1 events.<br />Single click to play<br />Doubleclick to change<br />Be sure that sound notifications for these events are enabled or this field will not have any effect.").arg(messageName));
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

void LogConfig::on_qtwMessages_itemChanged(QTreeWidgetItem* i, int column) {
	if (! i->isSelected()) return;
	switch (column) {
		case ColTTS:
			if (i->checkState(ColTTS))
				i->setCheckState(ColStaticSound, Qt::Unchecked);
			break;
		case ColStaticSound:
			if (i->checkState(ColStaticSound)) {
				i->setCheckState(ColTTS, Qt::Unchecked);
				if (i->text(ColStaticSoundPath).isEmpty()) browseForSpxFile();
			}
			break;
		default:
			break;
	}
}

void LogConfig::on_qtwMessages_itemClicked(QTreeWidgetItem * item, int column) {
	if (item && column == ColStaticSoundPath) {
		AudioOutputPtr ao = g.ao;
		if (ao) {
			if (!ao->playSample(item->text(ColStaticSoundPath), false))
				browseForSpxFile();
		}
	}
}

void LogConfig::on_qtwMessages_itemDoubleClicked(QTreeWidgetItem * item, int column) {
	if (item && column == ColStaticSoundPath)
		browseForSpxFile();
}

void LogConfig::browseForSpxFile() {
	QString file = QFileDialog::getOpenFileName(this, tr("Choose sound file"), QString(), QLatin1String("*.spx"));
	if (! file.isEmpty()) {
		if (AudioOutputSample::getPacketsFromFile(file).isEmpty()) {
			QMessageBox::critical(this,
			                      tr("Invalid sound file"),
			                      tr("The file '%1' does not exist or is not a valid speex file.").arg(file));
			return;
		}
		QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
		i->setText(ColStaticSoundPath, file);
		i->setCheckState(ColStaticSound, Qt::Checked);

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
	                       "tell application \"GrowlHelperApp\"\n"
	                       "	set the allNotificationsList to %1\n"
	                       "	set the enabledNotificationsList to %1\n"
	                       "	register as application \"Mumble\""
	                       "		all notifications allNotificationsList"
	                       "		default notifications enabledNotificationsList"
	                       "		icon of application \"Mumble\"\n"
	                       "end tell\n").arg(qsGrowlEvents);
	if (growl_available())
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

#ifdef USE_DBUS
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
#else
		if (true) {
#endif
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

				g.mw->qstiIcon->showMessage(msgName(mt), plain, msgIcon);
			}
		}
#ifdef Q_OS_MAC
		QString qsScript = QString(
		                       "tell application \"GrowlHelperApp\"\n"
		                       "	notify with name \"%1\" title \"%1\" description \"%2\" application name \"Mumble\"\n"
		                       "end tell\n").arg(msgName(mt)).arg(plain);
		if (growl_available())
			qt_mac_execute_apple_script(qsScript, NULL);
#endif
	}

	// Message notification with static sounds

	if ((flags & Settings::LogSoundfile)) {
		QString sSound = g.s.qmMessageSounds.value(mt);
		AudioOutputPtr ao = g.ao;
		if (!ao || !ao->playSample(sSound, false)) {
			qWarning() << "Sound file" << sSound << "is not a valid speex file, fallback to TTS.";
			flags ^= Settings::LogSoundfile | Settings::LogTTS; // Fallback to TTS
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
