// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "DBus.h"

#include "Channel.h"
#include "ClientUser.h"
#include "MainWindow.h"
#include "ServerHandler.h"

#include <QtCore/QUrlQuery>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>

// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#include "Global.h"

MumbleDBus::MumbleDBus(QObject *mw) : QDBusAbstractAdaptor(mw) {
}

void MumbleDBus::openUrl(const QString &url, const QDBusMessage &msg) {
	QUrl u = QUrl::fromEncoded(url.toLatin1());
	bool valid = u.isValid();
	valid = valid && (u.scheme() == QLatin1String("mumble"));
	if (! valid) {
		QDBusConnection::sessionBus().send(msg.createErrorReply(QLatin1String("net.sourceforge.mumble.Error.url"), QLatin1String("Invalid URL")));
	} else {
		g.mw->openUrl(u);
	}
}

void MumbleDBus::getCurrentUrl(const QDBusMessage &msg) {
	if (!g.sh || !g.sh->isRunning() || ! g.uiSession) {
		QDBusConnection::sessionBus().send(msg.createErrorReply(QLatin1String("net.sourceforge.mumble.Error.connection"), QLatin1String("Not connected")));
		return;
	}
	QString host, user, pw;
	unsigned short port;
	QUrl u;

	g.sh->getConnectionInfo(host, port, user, pw);
	u.setScheme(QLatin1String("mumble"));
	u.setHost(host);
	u.setPort(port);
	u.setUserName(user);

#if QT_VERSION >= 0x050000
	QUrlQuery query;
	query.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
	u.setQuery(query);
#else
	u.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
#endif

	QStringList path;
	Channel *c = ClientUser::get(g.uiSession)->cChannel;
	while (c->cParent) {
		path.prepend(c->qsName);
		c = c->cParent;
	}
	u.setPath(path.join(QLatin1String("/")));
	QDBusConnection::sessionBus().send(msg.createReply(QString::fromLatin1(u.toEncoded())));
}

void MumbleDBus::getTalkingUsers(const QDBusMessage &msg) {
	if (!g.sh || !g.sh->isRunning() || ! g.uiSession) {
		QDBusConnection::sessionBus().send(msg.createErrorReply(QLatin1String("net.sourceforge.mumble.Error.connection"), QLatin1String("Not connected")));
		return;
	}
	QStringList names;
	foreach(ClientUser *cu, ClientUser::getTalking()) {
		names.append(cu->qsName);
	}
	QDBusConnection::sessionBus().send(msg.createReply(names));
}

void MumbleDBus::focus() {
	g.mw->show();
	g.mw->raise();
	g.mw->activateWindow();
}

void MumbleDBus::setSelfMuted(bool mute) {
	g.mw->qaAudioMute->setChecked(!mute);
	g.mw->qaAudioMute->trigger();
}

void MumbleDBus::setSelfDeaf(bool deafen) {
	g.mw->qaAudioDeaf->setChecked(!deafen);
	g.mw->qaAudioDeaf->trigger();
}

bool MumbleDBus::isSelfMuted() {
	return g.s.bMute;
}

bool MumbleDBus::isSelfDeaf() {
	return g.s.bDeaf;
}

void MumbleDBus::startTalking() {
    g.mw->on_PushToTalk_triggered(true, QVariant());
}

void MumbleDBus::stopTalking () {
    g.mw->on_PushToTalk_triggered(false, QVariant());
}
