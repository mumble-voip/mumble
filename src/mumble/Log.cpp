// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Log.h"

#include "AudioOutput.h"
#include "AudioOutputSample.h"
#include "Channel.h"
#include "MainWindow.h"
#include "NetworkConfig.h"
#include "RichTextEditor.h"
#include "Screen.h"
#include "ServerHandler.h"
#include "TextToSpeech.h"
#include "Utils.h"

#include <QtNetwork/QNetworkReply>
#include <QtGui/QImageWriter>
#include <QtGui/QScreen>
#include <QtGui/QTextBlock>
#include <QtGui/QTextDocumentFragment>
#include <QtWidgets/QDesktopWidget>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

static ConfigWidget *LogConfigDialogNew(Settings &st) {
	return new LogConfig(st);
}

static ConfigRegistrar registrar(4000, LogConfigDialogNew);

LogConfig::LogConfig(Settings &st) : ConfigWidget(st) {
	setupUi(this);

#ifdef USE_NO_TTS
	qgbTTS->setDisabled(true);
#endif

#if QT_VERSION >= 0x050000
	qtwMessages->header()->setSectionResizeMode(ColMessage, QHeaderView::Stretch);
	qtwMessages->header()->setSectionResizeMode(ColConsole, QHeaderView::ResizeToContents);
	qtwMessages->header()->setSectionResizeMode(ColNotification, QHeaderView::ResizeToContents);
	qtwMessages->header()->setSectionResizeMode(ColHighlight, QHeaderView::ResizeToContents);
	qtwMessages->header()->setSectionResizeMode(ColTTS, QHeaderView::ResizeToContents);
	qtwMessages->header()->setSectionResizeMode(ColStaticSound, QHeaderView::ResizeToContents);
#else
	qtwMessages->header()->setResizeMode(ColMessage, QHeaderView::Stretch);
	qtwMessages->header()->setResizeMode(ColConsole, QHeaderView::ResizeToContents);
	qtwMessages->header()->setResizeMode(ColNotification, QHeaderView::ResizeToContents);
	qtwMessages->header()->setResizeMode(ColHighlight, QHeaderView::ResizeToContents);
	qtwMessages->header()->setResizeMode(ColTTS, QHeaderView::ResizeToContents);
	qtwMessages->header()->setResizeMode(ColStaticSound, QHeaderView::ResizeToContents);
#endif

	QTreeWidgetItem *twi;
	for (int i = Log::firstMsgType; i <= Log::lastMsgType; ++i) {
		Log::MsgType t = Log::msgOrder[i];
		const QString messageName = g.l->msgName(t);

		twi = new QTreeWidgetItem(qtwMessages);
		twi->setData(ColMessage, Qt::UserRole, static_cast<int>(t));
		twi->setText(ColMessage, messageName);
		twi->setCheckState(ColConsole, Qt::Unchecked);
		twi->setCheckState(ColNotification, Qt::Unchecked);
		twi->setCheckState(ColHighlight, Qt::Unchecked);
		twi->setCheckState(ColStaticSound, Qt::Unchecked);

		twi->setToolTip(ColConsole, tr("Toggle console for %1 events").arg(messageName));
		twi->setToolTip(ColNotification, tr("Toggle pop-up notifications for %1 events").arg(messageName));
		twi->setToolTip(ColHighlight, tr("Toggle window highlight (if not active) for %1 events").arg(messageName));
		twi->setToolTip(ColStaticSound, tr("Click here to toggle sound notification for %1 events").arg(messageName));
		twi->setToolTip(ColStaticSoundPath, tr("Path to sound file used for sound notifications in the case of %1 events<br />Single click to play<br />Double-click to change").arg(messageName));

		twi->setWhatsThis(ColConsole, tr("Click here to toggle console output for %1 events.<br />If checked, this option makes Mumble output all %1 events in its message log.").arg(messageName));
		twi->setWhatsThis(ColNotification, tr("Click here to toggle pop-up notifications for %1 events.<br />If checked, a notification pop-up will be created by Mumble for every %1 event.").arg(messageName));
		twi->setWhatsThis(ColHighlight, tr("Click here to toggle window highlight for %1 events.<br />If checked, Mumble's window will be highlighted for every %1 event, if not active.").arg(messageName));
		twi->setWhatsThis(ColStaticSound, tr("Click here to toggle sound notification for %1 events.<br />If checked, Mumble uses a sound file predefined by you to indicate %1 events. Sound files and Text-To-Speech cannot be used at the same time.").arg(messageName));
		twi->setWhatsThis(ColStaticSoundPath, tr("Path to sound file used for sound notifications in the case of %1 events.<br />Single click to play<br />Double-click to change<br />Ensure that sound notifications for these events are enabled or this field will not have any effect.").arg(messageName));
#ifndef USE_NO_TTS
		twi->setCheckState(ColTTS, Qt::Unchecked);
		twi->setToolTip(ColTTS, tr("Toggle Text-To-Speech for %1 events").arg(messageName));
		twi->setWhatsThis(ColTTS, tr("Click here to toggle Text-To-Speech for %1 events.<br />If checked, Mumble uses Text-To-Speech to read %1 events out loud to you. Text-To-Speech is also able to read the contents of the event which is not true for sound files. Text-To-Speech and sound files cannot be used at the same time.").arg(messageName));
#endif
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
		i->setCheckState(ColHighlight, (ml & Settings::LogHighlight) ? Qt::Checked : Qt::Unchecked);
#ifndef USE_NO_TTS
		i->setCheckState(ColTTS, (ml & Settings::LogTTS) ? Qt::Checked : Qt::Unchecked);
#endif
		i->setCheckState(ColStaticSound, (ml & Settings::LogSoundfile) ? Qt::Checked : Qt::Unchecked);
		i->setText(ColStaticSoundPath, r.qmMessageSounds.value(mt));
	}

	qsbMaxBlocks->setValue(r.iMaxLogBlocks);

#ifdef USE_NO_TTS
	qtwMessages->hideColumn(ColTTS);
#else
	loadSlider(qsVolume, r.iTTSVolume);
	qsbThreshold->setValue(r.iTTSThreshold);
	qcbReadBackOwn->setChecked(r.bTTSMessageReadBack);
	qcbNoScope->setChecked(r.bTTSNoScope);
	qcbNoAuthor->setChecked(r.bTTSNoAuthor);

#endif
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
		if (i->checkState(ColHighlight) == Qt::Checked)
			v |= Settings::LogHighlight;
#ifndef USE_NO_TTS
		if (i->checkState(ColTTS) == Qt::Checked)
			v |= Settings::LogTTS;
#endif
		if (i->checkState(ColStaticSound) == Qt::Checked)
			v |= Settings::LogSoundfile;
		s.qmMessages[mt] = v;
		s.qmMessageSounds[mt] = i->text(ColStaticSoundPath);
	}
	s.iMaxLogBlocks = qsbMaxBlocks->value();

#ifndef USE_NO_TTS
	s.iTTSVolume=qsVolume->value();
	s.iTTSThreshold=qsbThreshold->value();
	s.bTTSMessageReadBack = qcbReadBackOwn->isChecked();
	s.bTTSNoScope = qcbNoScope->isChecked();
	s.bTTSNoAuthor = qcbNoAuthor->isChecked();
#endif
	s.bWhisperFriends = qcbWhisperFriends->isChecked();
}

void LogConfig::accept() const {
	g.l->tts->setVolume(s.iTTSVolume);
	g.mw->qteLog->document()->setMaximumBlockCount(s.iMaxLogBlocks);
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
	QTreeWidgetItem *i = qtwMessages->selectedItems()[0];
	QString defaultpath(i->text(ColStaticSoundPath));
	QString file = AudioOutputSample::browseForSndfile(defaultpath);
	if (!file.isEmpty()) {
		i->setText(ColStaticSoundPath, file);
		i->setCheckState(ColStaticSound, Qt::Checked);
	}
}

Log::Log(QObject *p) : QObject(p) {
	tts=new TextToSpeech(this);
	tts->setVolume(g.s.iTTSVolume);
	uiLastId = 0;
	qdDate = QDate::currentDate();
}

// Display order in settingsscreen, allows to insert new events without breaking config-compatibility with older versions
const Log::MsgType Log::msgOrder[] = {
	DebugInfo, CriticalError, Warning, Information,
	ServerConnected, ServerDisconnected,
	UserJoin, UserLeave,
	Recording,
	YouKicked, UserKicked,
	UserRenamed,
	SelfMute, SelfUnmute, SelfDeaf, SelfUndeaf,
	OtherSelfMute, YouMuted, YouMutedOther, OtherMutedOther,
	SelfChannelJoin, SelfChannelJoinOther,
	ChannelJoin, ChannelLeave,
	ChannelJoinConnect, ChannelLeaveDisconnect,
	PermissionDenied,
	TextMessage, PrivateTextMessage
};

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
	QT_TRANSLATE_NOOP("Log", "You self-muted"),
	QT_TRANSLATE_NOOP("Log", "Other self-muted/deafened"),
	QT_TRANSLATE_NOOP("Log", "User muted (you)"),
	QT_TRANSLATE_NOOP("Log", "User muted (by you)"),
	QT_TRANSLATE_NOOP("Log", "User muted (other)"),
	QT_TRANSLATE_NOOP("Log", "User Joined Channel"),
	QT_TRANSLATE_NOOP("Log", "User Left Channel"),
	QT_TRANSLATE_NOOP("Log", "Permission Denied"),
	QT_TRANSLATE_NOOP("Log", "Text Message"),
	QT_TRANSLATE_NOOP("Log", "You self-unmuted"),
	QT_TRANSLATE_NOOP("Log", "You self-deafened"),
	QT_TRANSLATE_NOOP("Log", "You self-undeafened"),
	QT_TRANSLATE_NOOP("Log", "User renamed"),
	QT_TRANSLATE_NOOP("Log", "You Joined Channel"),
	QT_TRANSLATE_NOOP("Log", "You Joined Channel (moved)"),
	QT_TRANSLATE_NOOP("Log", "User connected and entered channel"),
	QT_TRANSLATE_NOOP("Log", "User left channel and disconnected"),
	QT_TRANSLATE_NOOP("Log", "Private text message")
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
	qslAllowedSchemeNames << QLatin1String("gg"); // Gadu-Gadu http://gg.pl - Polish instant massager
	qslAllowedSchemeNames << QLatin1String("mailto");
	qslAllowedSchemeNames << QLatin1String("xmpp");
	qslAllowedSchemeNames << QLatin1String("skype");
	qslAllowedSchemeNames << QLatin1String("rtmp"); // http://en.wikipedia.org/wiki/Real_Time_Messaging_Protocol

	return qslAllowedSchemeNames;
}

QString Log::msgColor(const QString &text, LogColorType t) {
	QString classname;

	return QString::fromLatin1("<span class='log-%1'>%2</span>").arg(QString::fromLatin1(colorClasses[t])).arg(text);
}

QString Log::formatChannel(::Channel *c) {
	return QString::fromLatin1("<a href='channelid://%1/%3' class='log-channel'>%2</a>").arg(c->iId).arg(Qt::escape(c->qsName)).arg(QString::fromLatin1(g.sh->qbaDigest.toBase64()));
}

QString Log::formatClientUser(ClientUser *cu, LogColorType t, const QString &displayName) {
	QString className;
	if (t == Log::Target) {
		className = QString::fromLatin1("target");
	} else if (t == Log::Source) {
		className = QString::fromLatin1("source");
	}

	if (cu) {
		QString name = Qt::escape(displayName.isNull() ? cu->qsName : displayName);
		if (cu->qsHash.isEmpty()) {
			return QString::fromLatin1("<a href='clientid://%2/%4' class='log-user log-%1'>%3</a>").arg(className).arg(cu->uiSession).arg(name).arg(QString::fromLatin1(g.sh->qbaDigest.toBase64()));
		} else {
			return QString::fromLatin1("<a href='clientid://%2' class='log-user log-%1'>%3</a>").arg(className).arg(cu->qsHash).arg(name);
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

QString Log::validHtml(const QString &html, QTextCursor *tc) {
	LogDocument qtd;

	QRectF qr = Screen::screenFromWidget(*g.mw)->availableGeometry();
	qtd.setTextWidth(qr.width() / 2);
	qtd.setDefaultStyleSheet(qApp->styleSheet());

	// Call documentLayout on our LogDocument to ensure
	// it has a layout backing it. With a layout set on
	// the document, it will attempt to load all the
	// resources it contains as soon as we call setHtml(),
	// allowing our validation checks for things such as
	// data URL images to run.
	(void) qtd.documentLayout();
	qtd.setHtml(html);

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
		}
	}

	qtd.adjustSize();
	QSizeF s = qtd.size();

	if (!s.isValid()) {
		QString errorInvalidSizeMessage = tr("[[ Invalid size ]]");
		if (tc) {
			tc->insertText(errorInvalidSizeMessage);
			return QString();
		} else {
			return errorInvalidSizeMessage;
		}
	}

	int messageSize = s.width() * s.height();
	int allowedSize = 2048 * 2048;

	if (messageSize > allowedSize) {
		QString errorSizeMessage = tr("[[ Text object too large to display ]]");
		if (tc) {
			tc->insertText(errorSizeMessage);
			return QString();
		} else {
			return errorSizeMessage;
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

void Log::log(MsgType mt, const QString &console, const QString &terse, bool ownMessage, const QString &overrideTTS) {
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
		QTextCursor tc = g.mw->qteLog->textCursor();

		LogTextBrowser *tlog = g.mw->qteLog;
		const int oldscrollvalue = tlog->getLogScroll();
		const bool scroll = (oldscrollvalue == tlog->getLogScrollMaximum());

		tc.movePosition(QTextCursor::End);

		if (qdDate != dt.date()) {
			qdDate = dt.date();
			tc.insertBlock();
			tc.insertHtml(tr("[Date changed to %1]\n").arg(Qt::escape(qdDate.toString(Qt::DefaultLocaleShortDate))));
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
		tc.insertHtml(Log::msgColor(QString::fromLatin1("[%1] ").arg(Qt::escape(dt.time().toString())), Log::Time));
		validHtml(console, &tc);
		tc.movePosition(QTextCursor::End);
		g.mw->qteLog->setTextCursor(tc);

		if (scroll || ownMessage)
			tlog->scrollLogToBottom();
		else
			tlog->setLogScroll(oldscrollvalue);
	}

	if (!ownMessage) {
		if (!(g.mw->isActiveWindow() && g.mw->qdwLog->isVisible())) {
			// Message notification with window highlight
			if (flags & Settings::LogHighlight) {
				QApplication::alert(g.mw);
			}

			// Message notification with balloon tooltips
			if (flags & Settings::LogBalloon) {
				postNotification(mt, plain);
			}
		}

		// Don't make any noise if we are self deafened (Unless it is the sound for activating self deaf)
		if (g.s.bDeaf && mt != Log::SelfDeaf) {
			return;
		}

		// Message notification with static sounds
		if ((flags & Settings::LogSoundfile)) {
			QString sSound = g.s.qmMessageSounds.value(mt);
			AudioOutputPtr ao = g.ao;
			if (!ao || !ao->playSample(sSound, false)) {
				qWarning() << "Sound file" << sSound << "is not a valid audio file, fallback to TTS.";
				flags ^= Settings::LogSoundfile | Settings::LogTTS; // Fallback to TTS
			}
		}
	} else if (!g.s.bTTSMessageReadBack) {
		return;
	}

	// Message notification with Text-To-Speech
	if (g.s.bDeaf || !g.s.bTTS || !(flags & Settings::LogTTS)) {
		return;
	}

	// If overrideTTS is a valid string use its contents as message
	if (!overrideTTS.isNull()) {
		plain = overrideTTS;
	}

	// Apply simplifications to spoken text
	QRegExp identifyURL(QLatin1String("[a-z-]+://[^ <]*"),
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
				replacement = tr("FTP link to %1").arg(host);
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

// Post a notification using the MainWindow's QSystemTrayIcon.
void Log::postQtNotification(MsgType mt, const QString &plain) {
	if (g.mw->qstiIcon->isSystemTrayAvailable() && g.mw->qstiIcon->supportsMessages()) {
		QSystemTrayIcon::MessageIcon msgIcon;
		switch (mt) {
			case DebugInfo:
			case CriticalError:
				msgIcon = QSystemTrayIcon::Critical;
				break;
			case Warning:
				msgIcon = QSystemTrayIcon::Warning;
				break;
			default:
				msgIcon = QSystemTrayIcon::Information;
				break;
		}
		g.mw->qstiIcon->showMessage(msgName(mt), plain, msgIcon);
	}
}

LogDocument::LogDocument(QObject *p)
	: QTextDocument(p) {
}

QVariant LogDocument::loadResource(int type, const QUrl &url) {
	// Ignore requests for all external resources
	// that aren't images. We don't support any of them.
	if (type != QTextDocument::ImageResource) {
		addResource(type, url, QByteArray());
		return QByteArray();
	}

	QImage qi(1, 1, QImage::Format_Mono);
	addResource(type, url, qi);

	if (! url.isValid()) {
		return qi;
	}

	if (url.scheme() != QLatin1String("data")) {
		return qi;
	}

	QNetworkReply *rep = Network::get(url);
	connect(rep, SIGNAL(finished()), this, SLOT(finished()));

	// Handle data URLs immediately without a roundtrip to the event loop.
	// We need this to perform proper validation for data URL images when
	// a LogDocument is used inside Log::validHtml().
	QCoreApplication::sendPostedEvents(rep, 0);

	return qi;
}

void LogDocument::finished() {
	QNetworkReply *rep = qobject_cast<QNetworkReply *>(sender());

	if (rep->error() == QNetworkReply::NoError) {
		QByteArray ba = rep->readAll();
		QByteArray fmt;
		QImage qi;

		// Sniff the format instead of relying on the MIME type.
		// There are many misconfigured servers out there and
		// Mumble has historically sniffed the received data
		// instead of strictly requiring a correct Content-Type.
		if (RichTextImage::isValidImage(ba, fmt)) {
			if (qi.loadFromData(ba, fmt)) {
				addResource(QTextDocument::ImageResource, rep->request().url(), qi);

				// Force a re-layout of the QTextEdit the next
				// time we enter the event loop.
				// We must not trigger a re-layout immediately.
				// Doing so can trigger crashes deep inside Qt
				// if the QTextDocument has just been set on the
				// text edit widget.
				QTextEdit *qte = qobject_cast<QTextEdit *>(parent());
				if (qte != NULL) {
					QEvent *e = new QEvent(QEvent::FontChange);
					QApplication::postEvent(qte, e);

					e = new LogDocumentResourceAddedEvent();
					QApplication::postEvent(qte, e);
				}
			}
		}
	}

	rep->deleteLater();
}

LogDocumentResourceAddedEvent::LogDocumentResourceAddedEvent()
	: QEvent(LogDocumentResourceAddedEvent::Type) {
}
