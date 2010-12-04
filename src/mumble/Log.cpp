/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

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
#include "Channel.h"
#include "ServerHandler.h"
#include "NetworkConfig.h"
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
		twi->setToolTip(ColStaticSoundPath, tr("Path to sound file used for sound notifications in the case of %1 events<br />Single click to play<br />Double-click to change").arg(messageName));

		twi->setWhatsThis(ColConsole, tr("Click here to toggle console output for %1 events.<br />If checked, this option makes Mumble output all %1 events in its message log.").arg(messageName));
		twi->setWhatsThis(ColNotification, tr("Click here to toggle pop-up notifications for %1 events.<br />If checked, a notification pop-up will be created by Mumble for every %1 event.").arg(messageName));
		twi->setWhatsThis(ColTTS, tr("Click here to toggle Text-To-Speech for %1 events.<br />If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.").arg(messageName));
		twi->setWhatsThis(ColStaticSound, tr("Click here to toggle sound notification for %1 events.<br />If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.").arg(messageName));
		twi->setWhatsThis(ColStaticSoundPath, tr("Path to sound file used for sound notifications in the case of %1 events.<br />Single click to play<br />Double-click to change<br />Ensure that sound notifications for these events are enabled or this field will not have any effect.").arg(messageName));
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
	qcbWhisperFriends->setChecked(r.bWhisperFriends);
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
	s.bWhisperFriends = qcbWhisperFriends->isChecked();
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
				if (i->text(ColStaticSoundPath).isEmpty()) browseForAudioFile();
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
				browseForAudioFile();
		}
	}
}

void LogConfig::on_qtwMessages_itemDoubleClicked(QTreeWidgetItem * item, int column) {
	if (item && column == ColStaticSoundPath)
		browseForAudioFile();
}

void LogConfig::browseForAudioFile() {
	QString file = AudioOutputSample::browseForSndfile();
	if (!file.isEmpty()) {
		QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
		i->setText(ColStaticSoundPath, file);
		i->setCheckState(ColStaticSound, Qt::Checked);
	}
}

Log::Log(QObject *p) : QObject(p) {
	tts=new TextToSpeech(this);
	tts->setVolume(g.s.iTTSVolume);
	uiLastId = 0;
	qdDate = QDate::currentDate();

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
	QT_TRANSLATE_NOOP("Log", "Debug"),
	QT_TRANSLATE_NOOP("Log", "Critical"),
	QT_TRANSLATE_NOOP("Log", "Warning"),
	QT_TRANSLATE_NOOP("Log", "Information"),
	QT_TRANSLATE_NOOP("Log", "Server Connected"),
	QT_TRANSLATE_NOOP("Log", "Server Disconnected"),
	QT_TRANSLATE_NOOP("Log", "User Joined Server"),
	QT_TRANSLATE_NOOP("Log", "User Left Server"),
	QT_TRANSLATE_NOOP("Log", "User recording state changed"),
	QT_TRANSLATE_NOOP("Log", "User kicked (you or by you)"),
	QT_TRANSLATE_NOOP("Log", "User kicked"),
	QT_TRANSLATE_NOOP("Log", "You self-muted/deafened"),
	QT_TRANSLATE_NOOP("Log", "Other self-muted/deafened"),
	QT_TRANSLATE_NOOP("Log", "User muted (you)"),
	QT_TRANSLATE_NOOP("Log", "User muted (by you)"),
	QT_TRANSLATE_NOOP("Log", "User muted (other)"),
	QT_TRANSLATE_NOOP("Log", "User Joined Channel"),
	QT_TRANSLATE_NOOP("Log", "User Left Channel"),
	QT_TRANSLATE_NOOP("Log", "Permission Denied"),
	QT_TRANSLATE_NOOP("Log", "Text Message")
};

QString Log::msgName(MsgType t) const {
	return tr(msgNames[t]);
}

const char *Log::colorClasses[] = {
	"time",
	"server",
	"privilege"
};

const QStringList Log::allowedSchemes() {
	QStringList qslAllowedSchemeNames;
	qslAllowedSchemeNames << QLatin1String("mumble");
	qslAllowedSchemeNames << QLatin1String("http");
	qslAllowedSchemeNames << QLatin1String("https");
	qslAllowedSchemeNames << QLatin1String("ftp");
	qslAllowedSchemeNames << QLatin1String("clientid");
	qslAllowedSchemeNames << QLatin1String("channelid");
	qslAllowedSchemeNames << QLatin1String("spotify");
	qslAllowedSchemeNames << QLatin1String("steam");
	qslAllowedSchemeNames << QLatin1String("irc");
	return qslAllowedSchemeNames;
}

QString Log::msgColor(const QString &text, LogColorType t) {
	QString classname;

	return QString::fromLatin1("<span class='log-%1'>%2</span>").arg(QString::fromLatin1(colorClasses[t])).arg(text);
}

QString Log::formatChannel(::Channel *c) {
	return QString::fromLatin1("<a href='channelid://%1/%3' class='log-channel'>%2</a>").arg(c->iId).arg(c->qsName).arg(QString::fromLatin1(g.sh->qbaDigest.toBase64()));
}

QString Log::formatClientUser(ClientUser *cu, LogColorType t) {
	QString className;
	if (t == Log::Target) {
		className = QString::fromLatin1("target");
	} else if (t == Log::Source) {
		className = QString::fromLatin1("source");
	}

	if (cu) {
		if (cu->qsHash.isEmpty()) {
			return QString::fromLatin1("<a href='clientid://%2/%4' class='log-user log-%1'>%3</a>").arg(className).arg(cu->uiSession).arg(cu->qsName).arg(QString::fromLatin1(g.sh->qbaDigest.toBase64()));
		} else {
			return QString::fromLatin1("<a href='clientid://%2' class='log-user log-%1'>%3</a>").arg(className).arg(cu->qsHash).arg(cu->qsName);
		}
	} else {
		return QString::fromLatin1("<span class='log-server log-%1'>%2</span>").arg(className).arg(tr("the server"));
	}
}

void Log::setIgnore(MsgType t, int ignore) {
	qmIgnore.insert(t, ignore);
}

void Log::clearIgnore() {
	qmIgnore.clear();
}

QString Log::imageToImg(const QByteArray &format, const QByteArray &image) {
	QString fmt = QLatin1String(format);

	if (fmt.isEmpty())
		fmt = QLatin1String("qt");

	QByteArray rawbase = image.toBase64();
	QByteArray encoded;
	int i = 0;
	int begin = 0, end = 0;
	do {
		begin = i*72;
		end = begin+72;

		encoded.append(QUrl::toPercentEncoding(QLatin1String(rawbase.mid(begin,72))));
		if (end < rawbase.length())
			encoded.append('\n');

		++i;
	} while (end < rawbase.length());

	return QString::fromLatin1("<img src=\"data:image/%1;base64,%2\" />").arg(fmt).arg(QLatin1String(encoded));
}

QString Log::imageToImg(QImage img) {
	if ((img.width() > 480) || (img.height() > 270)) {
		img = img.scaled(480, 270, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	int quality = 100;
	QByteArray format = "PNG";

	QByteArray qba;
	{
		QBuffer qb(&qba);
		qb.open(QIODevice::WriteOnly);

		QImageWriter imgwrite(&qb, format);
		imgwrite.write(img);
	}

	while ((qba.length() >= 65536) && (quality > 0)) {
		qba.clear();
		QBuffer qb(&qba);
		qb.open(QIODevice::WriteOnly);

		format = "JPEG";

		QImageWriter imgwrite(&qb, format);
		imgwrite.setQuality(quality);
		imgwrite.write(img);
		quality -= 10;
	}
	if (qba.length() < 65536) {
		return imageToImg(format, qba);
	}
	return QString();
}

QString Log::validHtml(const QString &html, bool allowReplacement, QTextCursor *tc) {
	QDesktopWidget dw;
	ValidDocument qtd(allowReplacement);
	bool valid = false;

	QRectF qr = dw.availableGeometry(dw.screenNumber(g.mw));
	qtd.setTextWidth(qr.width() / 2);
	qtd.setDefaultStyleSheet(qApp->styleSheet());

	qtd.setHtml(html);
	valid = qtd.isValid();

	QStringList qslAllowed = allowedSchemes();
	for (QTextBlock qtb = qtd.begin(); qtb != qtd.end(); qtb = qtb.next()) {
		for (QTextBlock::iterator qtbi = qtb.begin(); qtbi != qtb.end(); ++qtbi) {
			const QTextFragment &qtf = qtbi.fragment();
			QTextCharFormat qcf = qtf.charFormat();
			if (! qcf.anchorHref().isEmpty()) {
				QUrl url(qcf.anchorHref());
				if (! url.isValid() || ! qslAllowed.contains(url.scheme())) {
					QTextCharFormat qcfn = QTextCharFormat();
					QTextCursor qtc(&qtd);
					qtc.setPosition(qtf.position(), QTextCursor::MoveAnchor);
					qtc.setPosition(qtf.position()+qtf.length(), QTextCursor::KeepAnchor);
					qtc.setCharFormat(qcfn);
					qtbi = qtb.begin();
				}
			}
			if (qcf.isImageFormat()) {
				QTextImageFormat qtif = qcf.toImageFormat();
				QUrl url(qtif.name());
				if (! qtif.name().isEmpty() && ! url.isValid())
					valid = false;
			}
		}
	}

	qtd.adjustSize();
	QSizeF s = qtd.size();

	if (!valid || (s.width() > qr.width()) || (s.height() > qr.height())) {
		qtd.setPlainText(html);
		qtd.adjustSize();
		s = qtd.size();

		if ((s.width() > qr.width()) || (s.height() > qr.height())) {
			QString errorMessage = tr("[[ Text object too large to display ]]");
			if (tc) {
				tc->insertText(errorMessage);
				return QString();
			} else {
				return errorMessage;
			}
		}
	}

	if (tc) {
		QTextCursor tcNew(&qtd);
		tcNew.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
		tc->insertFragment(tcNew.selection());
		return QString();
	} else {
		return qtd.toHtml();
	}
}

void Log::log(MsgType mt, const QString &console, const QString &terse, bool console_only) {
	QDateTime dt = QDateTime::currentDateTime();

	int ignore = qmIgnore.value(mt);
	if (ignore) {
		ignore--;
		qmIgnore.insert(mt, ignore);
		return;
	}

	QString plain = QTextDocumentFragment::fromHtml(console).toPlainText();

	quint32 flags = g.s.qmMessages.value(mt);

	// Message output on console
	if ((flags & Settings::LogConsole)) {
		QTextCursor tc=g.mw->qteLog->textCursor();
		tc.movePosition(QTextCursor::End);

		if (qdDate != dt.date()) {
			qdDate = dt.date();
			tc.insertBlock();
			tc.insertHtml(tr("[Date changed to %1]\n").arg(qdDate.toString(Qt::DefaultLocaleShortDate)));
			tc.movePosition(QTextCursor::End);
		}

		if (plain.contains(QRegExp(QLatin1String("[\\r\\n]")))) {
			QTextFrameFormat qttf;
			qttf.setBorder(1);
			qttf.setPadding(2);
			qttf.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
			tc.insertFrame(qttf);
		} else if (! g.mw->qteLog->document()->isEmpty()) {
			tc.insertBlock();
		}
		tc.insertHtml(Log::msgColor(QString::fromLatin1("[%1] ").arg(dt.time().toString(Qt::DefaultLocaleShortDate)), Log::Time));
		validHtml(console, true, &tc);
		tc.movePosition(QTextCursor::End);
		g.mw->qteLog->setTextCursor(tc);
		g.mw->qteLog->ensureCursorVisible();
	}

	if (console_only)
		return;

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

	// Don't make any noise if we are self deafened
	if (g.s.bDeaf)
		return;

	// Message notification with static sounds
	if ((flags & Settings::LogSoundfile)) {
		QString sSound = g.s.qmMessageSounds.value(mt);
		AudioOutputPtr ao = g.ao;
		if (!ao || !ao->playSample(sSound, false)) {
			qWarning() << "Sound file" << sSound << "is not a valid audio file, fallback to TTS.";
			flags ^= Settings::LogSoundfile | Settings::LogTTS; // Fallback to TTS
		}
	}

	// Message notification with Text-To-Speech
	if (! g.s.bTTS || !(flags & Settings::LogTTS))
		return;

	// Apply simplifications to spoken text
	QRegExp identifyURL(QLatin1String("[a-z-]+://[^ <$]*"),
	                    Qt::CaseInsensitive,
	                    QRegExp::RegExp2);

	QStringList qslAllowed = allowedSchemes();

	int pos = 0;
	while ((pos = identifyURL.indexIn(plain, pos)) != -1) {
		QUrl url(identifyURL.cap(0).toLower());
		int len = identifyURL.matchedLength();
		if (url.isValid() && qslAllowed.contains(url.scheme())) {
			// Replace it appropriatly
			QString replacement;
			QString host = url.host().replace(QRegExp(QLatin1String("^www.")), QString());

			if (url.scheme() == QLatin1String("http") || url.scheme() == QLatin1String("https"))
				replacement = tr("link to %1").arg(host);
			else if (url.scheme() == QLatin1String("ftp"))
				replacement = tr("ftp link to %1").arg(host);
			else if (url.scheme() == QLatin1String("clientid"))
				replacement = tr("player link");
			else if (url.scheme() == QLatin1String("channelid"))
				replacement = tr("channel link");
			else
				replacement = tr("%1 link").arg(url.scheme());

			plain.replace(pos, len, replacement);
		} else {
			pos += len;
		}
	}

	// TTS threshold limiter.
	if (plain.length() <= g.s.iTTSThreshold)
		tts->say(plain);
	else if ((! terse.isEmpty()) && (terse.length() <= g.s.iTTSThreshold))
		tts->say(terse);
}

ValidDocument::ValidDocument(bool allowhttp, QObject *p) : QTextDocument(p) {
	bValid = true;
	qslValidImage << QLatin1String("data");
	if (allowhttp) {
		qslValidImage << QLatin1String("http");
		qslValidImage << QLatin1String("https");
	}
}

QVariant ValidDocument::loadResource(int type, const QUrl &url) {
	QVariant v = QLatin1String("PlaceHolder");
	if ((type == QTextDocument::ImageResource) && qslValidImage.contains(url.scheme()))
		return QTextDocument::loadResource(type, url);
	bValid = false;
	return v;
}

bool ValidDocument::isValid() const {
	return bValid;
}

LogDocument::LogDocument(QObject *p) : QTextDocument(p) {
}

QVariant LogDocument::loadResource(int type, const QUrl &url) {
	if (type != QTextDocument::ImageResource)
		return QLatin1String("No external resources allowed.");
	if (g.s.iMaxImageSize <= 0)
		return QLatin1String("Image download disabled.");

	if (url.scheme() == QLatin1String("data")) {
		QVariant v = QTextDocument::loadResource(type, url);
		addResource(type, url, v);
		return v;
	}

	qWarning() << "LogDocument::loadResource " << type << url.toString();

	QImage qi(1, 1, QImage::Format_Mono);
	addResource(type, url, qi);

	if (! url.isValid() || url.isRelative())
		return qi;

	if ((url.scheme() != QLatin1String("http")) && (url.scheme() != QLatin1String("https")))
		return qi;

	QNetworkReply *rep = Network::get(url);
	connect(rep, SIGNAL(metaDataChanged()), this, SLOT(receivedHead()));
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));
	return qi;
}

void LogDocument::receivedHead() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());
	QVariant length = rep->header(QNetworkRequest::ContentLengthHeader);
	if (length == QVariant::Invalid || length.toInt() > g.s.iMaxImageSize) {
		qWarning() << "Image "<< rep->url().toString() <<" (" << length.toInt() << " byte) to big, request aborted. ";
		rep->abort();
	}
}

void LogDocument::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());

	if (rep->error() == QNetworkReply::NoError) {
		QVariant qv = rep->readAll();
		QImage qi;

		if (qi.loadFromData(qv.toByteArray()) && qi.width() <= g.s.iMaxImageWidth && qi.height() <= g.s.iMaxImageHeight) {
			addResource(QTextDocument::ImageResource, rep->request().url(), qi);
			g.mw->qteLog->setDocument(this);
		} else qWarning() << "Image "<< rep->url().toString() <<" (" << qi.width() << "x" << qi.height() <<") to large.";
	} else qWarning() << "Image "<< rep->url().toString() << " download failed.";

	rep->deleteLater();
}
