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

#include "Global.h"
#include "MainWindow.h"
#include "DBus.h"
#include "ServerHandler.h"
#include "Channel.h"
#include "ClientUser.h"


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
	u.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
	QStringList path;
	Channel *c = ClientUser::get(g.uiSession)->cChannel;
	while (c->cParent) {
		path.prepend(c->qsName);
		c = c->cParent;
	}
	u.setPath(path.join(QLatin1String("/")));
	QDBusConnection::sessionBus().send(msg.createReply(QString::fromLatin1(u.toEncoded())));
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
