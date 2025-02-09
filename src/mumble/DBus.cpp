// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBus.h"

#include "Channel.h"
#include "ClientUser.h"
#include "MainWindow.h"
#include "ServerHandler.h"
#include "Global.h"

#include <QtCore/QUrlQuery>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

MumbleDBus::MumbleDBus(QObject *mw) : QDBusAbstractAdaptor(mw) {
}

void MumbleDBus::openUrl(const QString &url, const QDBusMessage &msg) {
	QUrl u     = QUrl::fromEncoded(url.toLatin1());
	bool valid = u.isValid();
	valid      = valid && (u.scheme() == QLatin1String("mumble"));
	if (!valid) {
		QDBusConnection::sessionBus().send(
			msg.createErrorReply(QLatin1String("net.sourceforge.mumble.Error.url"), QLatin1String("Invalid URL")));
	} else {
		Global::get().mw->openUrl(u);
	}
}

void MumbleDBus::getCurrentUrl(const QDBusMessage &msg) {
	if (!Global::get().sh || !Global::get().sh->isRunning() || !Global::get().uiSession) {
		QDBusConnection::sessionBus().send(msg.createErrorReply(
			QLatin1String("net.sourceforge.mumble.Error.connection"), QLatin1String("Not connected")));
		return;
	}
	QString host, user, pw;
	unsigned short port;
	QUrl u;

	Global::get().sh->getConnectionInfo(host, port, user, pw);
	u.setScheme(QLatin1String("mumble"));
	u.setHost(host);
	u.setPort(port);
	u.setUserName(user);

	QUrlQuery query;
	query.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
	u.setQuery(query);

	QStringList path;
	Channel *c = ClientUser::get(Global::get().uiSession)->cChannel;
	while (c->cParent) {
		path.prepend(c->qsName);
		c = c->cParent;
	}
	QString fullpath = path.join(QLatin1String("/"));
	// Make sure fullpath starts with a slash for non-empty paths. Setting
	// a path without a leading slash clears the whole QUrl.
	if (!fullpath.isEmpty()) {
		fullpath.prepend(QLatin1String("/"));
	}
	u.setPath(fullpath);
	QDBusConnection::sessionBus().send(msg.createReply(QString::fromLatin1(u.toEncoded())));
}

void MumbleDBus::getTalkingUsers(const QDBusMessage &msg) {
	if (!Global::get().sh || !Global::get().sh->isRunning() || !Global::get().uiSession) {
		QDBusConnection::sessionBus().send(msg.createErrorReply(
			QLatin1String("net.sourceforge.mumble.Error.connection"), QLatin1String("Not connected")));
		return;
	}
	QStringList names;
	foreach (ClientUser *cu, ClientUser::getTalking()) { names.append(cu->qsName); }
	QDBusConnection::sessionBus().send(msg.createReply(names));
}

void MumbleDBus::focus() {
	Global::get().mw->show();
	Global::get().mw->raise();
	Global::get().mw->activateWindow();
}

void MumbleDBus::setTransmitMode(unsigned int mode, const QDBusMessage &msg) {
	switch (mode) {
		case 0:
			Global::get().s.atTransmit = Settings::Continuous;
			break;
		case 1:
			Global::get().s.atTransmit = Settings::VAD;
			break;
		case 2:
			Global::get().s.atTransmit = Settings::PushToTalk;
			break;
		default:
			QDBusConnection::sessionBus().send(msg.createErrorReply(
				QLatin1String("net.sourceforge.mumble.Error.transmitMode"), QLatin1String("Invalid transmit mode")));
			return;
	}
	QMetaObject::invokeMethod(Global::get().mw, "updateTransmitModeComboBox", Qt::QueuedConnection);
}

unsigned int MumbleDBus::getTransmitMode() {
	return Global::get().s.atTransmit;
}

void MumbleDBus::toggleSelfMuted() {
	Global::get().mw->qaAudioMute->trigger();
}

void MumbleDBus::toggleSelfDeaf() {
	Global::get().mw->qaAudioDeaf->trigger();
}

void MumbleDBus::setSelfMuted(bool mute) {
	Global::get().mw->qaAudioMute->setChecked(!mute);
	Global::get().mw->qaAudioMute->trigger();
}

void MumbleDBus::setSelfDeaf(bool deafen) {
	Global::get().mw->qaAudioDeaf->setChecked(!deafen);
	Global::get().mw->qaAudioDeaf->trigger();
}

bool MumbleDBus::isSelfMuted() {
	return Global::get().s.bMute;
}

bool MumbleDBus::isSelfDeaf() {
	return Global::get().s.bDeaf;
}

void MumbleDBus::startTalking() {
	Global::get().mw->on_PushToTalk_triggered(true, QVariant());
}

void MumbleDBus::stopTalking() {
	Global::get().mw->on_PushToTalk_triggered(false, QVariant());
}
