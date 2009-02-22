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

#include "Server.h"
#include "Player.h"
#include "Channel.h"
#include "Group.h"
#include "Meta.h"
#include "Version.h"

void Server::setPlayerState(Player *pPlayer, Channel *cChannel, bool mute, bool deaf, bool suppressed) {
	bool changed = false;

	if (deaf)
		mute = true;
	if (! mute)
		deaf = false;

	if ((pPlayer->bDeaf != deaf) && (deaf || (!deaf && mute))) {
		pPlayer->bDeaf = deaf;
		pPlayer->bMute = mute;
		MessagePlayerDeaf mpd;
		mpd.uiSession = 0;
		mpd.uiVictim=pPlayer->uiSession;
		mpd.bDeaf = deaf;
		sendAll(&mpd);
		changed = true;
	} else if ((pPlayer->bDeaf != deaf) || (pPlayer->bMute != mute)) {
		pPlayer->bDeaf = deaf;
		pPlayer->bMute = mute;

		MessagePlayerMute mpm;
		mpm.uiSession = 0;
		mpm.uiVictim=pPlayer->uiSession;
		mpm.bMute=mute;
		sendAll(&mpm);
		changed = true;
	}

	if (cChannel != pPlayer->cChannel) {
		playerEnterChannel(pPlayer, cChannel);
		MessagePlayerMove mpm;
		mpm.uiSession = 0;
		mpm.uiVictim = pPlayer->uiSession;
		mpm.iChannelId = cChannel->iId;
		sendAll(&mpm);
		changed = true;
	}

	emit playerStateChanged(pPlayer);
}

bool Server::setChannelState(Channel *cChannel, Channel *cParent, const QString &qsName, const QSet<Channel *> &links) {
	bool changed = false;
	bool updated = false;

	if (cChannel->qsName != qsName) {
		cChannel->qsName = qsName;
		MessageChannelRename mcr;
		mcr.uiSession = 0;
		mcr.iId = cChannel->iId;
		mcr.qsName = cChannel->qsName;
		sendAll(&mcr);
		updated = true;
		changed = true;
	}

	if ((cParent != cChannel) && (cParent != cChannel->cParent)) {
		Channel *p = cParent;
		while (p) {
			if (p == cChannel)
				return false;
			p = p->cParent;
		}

		cChannel->cParent->removeChannel(cChannel);
		cParent->addChannel(cChannel);

		MessageChannelMove mcm;
		mcm.uiSession = 0;
		mcm.iId = cChannel->iId;
		mcm.iParent = cParent->iId;
		sendAll(&mcm);

		updated = true;
		changed = true;
	}

	const QSet<Channel *> &oldset = cChannel->qsPermLinks;

	if (links != oldset) {
		// Remove
		foreach(Channel *l, links) {
			if (! links.contains(l)) {
				removeLink(cChannel, l);

				MessageChannelLink mcl;
				mcl.uiSession = 0;
				mcl.iId = cChannel->iId;
				mcl.qlTargets << l->iId;
				mcl.ltType = MessageChannelLink::Unlink;
				sendAll(&mcl);
			}
		}

		// Add
		foreach(Channel *l, links) {
			if (! oldset.contains(l)) {
				addLink(cChannel, l);

				MessageChannelLink mcl;
				mcl.uiSession = 0;
				mcl.iId = cChannel->iId;
				mcl.qlTargets << l->iId;
				mcl.ltType = MessageChannelLink::Link;
				sendAll(&mcl);
			}
		}

		changed = true;
	}

	if (updated)
		updateChannel(cChannel);
	if (changed)
		emit channelStateChanged(cChannel);

	return true;
}

void Server::sendTextMessage(Channel *cChannel, User *pPlayer, bool tree, const QString &text) {
	MessageTextMessage mtm;
	mtm.uiSession = 0;
	mtm.qsMessage = text;
	if (pPlayer) {
		mtm.uiVictim = pPlayer->uiSession;
		mtm.iChannel = -1;
		mtm.bTree = false;
		sendMessage(pPlayer, &mtm);
	} else {
		mtm.uiVictim = 0;
		mtm.iChannel = cChannel->iId;
		mtm.bTree = tree;

		QSet<Channel *> chans;
		QQueue<Channel *> q;
		q << cChannel;
		chans.insert(cChannel);
		Channel *c;

		if (tree) {
			while (! q.isEmpty()) {
				c = q.dequeue();
				chans.insert(c);
				foreach(c, c->qlChannels)
					q.enqueue(c);
			}
		}
		foreach(c, chans) {
			foreach(Player *p, c->qlPlayers)
				sendMessage(static_cast<User *>(p), &mtm);
		}
	}
}

void Server::setTempGroups(int playerid, Channel *cChannel, const QStringList &groups) {
	if (! cChannel)
		cChannel = qhChannels.value(0);

	Group *g;
	foreach(g, cChannel->qhGroups)
		g->qsTemporary.remove(playerid);

	QString gname;
	foreach(gname, groups) {
		g = cChannel->qhGroups.value(gname);
		if (! g) {
			g = new Group(cChannel, gname);
		}
		g->qsTemporary.insert(playerid);
	}

	Player *p = qhUsers.value(playerid);
	if (p)
		clearACLCache(p);
}


void Server::connectAuthenticator(QObject *obj) {
	connect(this, SIGNAL(registerPlayerSig(int &, const QString &)), obj, SLOT(registerPlayerSlot(int &, const QString &)));
	connect(this, SIGNAL(unregisterPlayerSig(int &, int)), obj, SLOT(unregisterPlayerSlot(int &, int)));
	connect(this, SIGNAL(getRegisteredPlayersSig(const QString &, QMap<int, QPair<QString, QString> > &)), obj, SLOT(getRegisteredPlayersSlot(const QString &, QMap<int, QPair<QString, QString> > &)));
	connect(this, SIGNAL(getRegistrationSig(int &, int, QString &, QString &)), obj, SLOT(getRegistrationSlot(int &, int, QString &, QString &)));
	connect(this, SIGNAL(authenticateSig(int &, QString &, const QString &)), obj, SLOT(authenticateSlot(int &, QString &, const QString &)));
	connect(this, SIGNAL(setPwSig(int &, int, const QString &)), obj, SLOT(setPwSlot(int &, int, const QString &)));
	connect(this, SIGNAL(setEmailSig(int &, int, const QString &)), obj, SLOT(setEmailSlot(int &, int, const QString &)));
	connect(this, SIGNAL(setNameSig(int &, int, const QString &)), obj, SLOT(setNameSlot(int &, int, const QString &)));
	connect(this, SIGNAL(setTextureSig(int &, int, const QByteArray &)), obj, SLOT(setTextureSlot(int &, int, const QByteArray &)));
	connect(this, SIGNAL(idToNameSig(QString &, int)), obj, SLOT(idToNameSlot(QString &, int)));
	connect(this, SIGNAL(nameToIdSig(int &, const QString &)), obj, SLOT(nameToIdSlot(int &, const QString &)));
	connect(this, SIGNAL(idToTextureSig(QByteArray &, int)), obj, SLOT(idToTextureSlot(QByteArray &, int)));
}

void Server::disconnectAuthenticator(QObject *obj) {
	disconnect(this, SIGNAL(registerPlayerSig(int &, const QString &)), obj, SLOT(registerPlayerSlot(int &, const QString &)));
	disconnect(this, SIGNAL(unregisterPlayerSig(int &, int)), obj, SLOT(unregisterPlayerSlot(int &, int)));
	disconnect(this, SIGNAL(getRegisteredPlayersSig(const QString &, QMap<int, QPair<QString, QString> > &)), obj, SLOT(getRegisteredPlayersSlot(const QString &, QMap<int, QPair<QString, QString> > &)));
	disconnect(this, SIGNAL(getRegistrationSig(int &, int, QString &, QString &)), obj, SLOT(getRegistrationSlot(int &, int, QString &, QString &)));
	disconnect(this, SIGNAL(authenticateSig(int &, QString &, const QString &)), obj, SLOT(authenticateSlot(int &, QString &, const QString &)));
	disconnect(this, SIGNAL(setPwSig(int &, int, const QString &)), obj, SLOT(setPwSlot(int &, int, const QString &)));
	disconnect(this, SIGNAL(setEmailSig(int &, int, const QString &)), obj, SLOT(setEmailSlot(int &, int, const QString &)));
	disconnect(this, SIGNAL(setNameSig(int &, int, const QString &)), obj, SLOT(setNameSlot(int &, int, const QString &)));
	disconnect(this, SIGNAL(setTextureSig(int &, int, const QByteArray &)), obj, SLOT(setTextureSlot(int &, int, const QByteArray &)));
	disconnect(this, SIGNAL(idToNameSig(QString &, int)), obj, SLOT(idToNameSlot(QString &, int)));
	disconnect(this, SIGNAL(nameToIdSig(int &, const QString &)), obj, SLOT(nameToIdSlot(int &, const QString &)));
	disconnect(this, SIGNAL(idToTextureSig(QByteArray &, int)), obj, SLOT(idToTextureSlot(QByteArray &, int)));
}

void Server::connectListener(QObject *obj) {
	connect(this, SIGNAL(playerStateChanged(const Player *)), obj, SLOT(playerStateChanged(const Player *)));
	connect(this, SIGNAL(playerConnected(const Player *)), obj, SLOT(playerConnected(const Player *)));
	connect(this, SIGNAL(playerDisconnected(const Player *)), obj, SLOT(playerDisconnected(const Player *)));
	connect(this, SIGNAL(channelStateChanged(const Channel *)), obj, SLOT(channelStateChanged(const Channel *)));
	connect(this, SIGNAL(channelCreated(const Channel *)), obj, SLOT(channelCreated(const Channel *)));
	connect(this, SIGNAL(channelRemoved(const Channel *)), obj, SLOT(channelRemoved(const Channel *)));
}

void Server::disconnectListener(QObject *obj) {
	disconnect(this, SIGNAL(playerStateChanged(const Player *)), obj, SLOT(playerStateChanged(const Player *)));
	disconnect(this, SIGNAL(playerConnected(const Player *)), obj, SLOT(playerConnected(const Player *)));
	disconnect(this, SIGNAL(playerDisconnected(const Player *)), obj, SLOT(playerDisconnected(const Player *)));
	disconnect(this, SIGNAL(channelStateChanged(const Channel *)), obj, SLOT(channelStateChanged(const Channel *)));
	disconnect(this, SIGNAL(channelCreated(const Channel *)), obj, SLOT(channelCreated(const Channel *)));
	disconnect(this, SIGNAL(channelRemoved(const Channel *)), obj, SLOT(channelRemoved(const Channel *)));
}

void Meta::connectListener(QObject *obj) {
	connect(this, SIGNAL(started(Server *)), obj, SLOT(started(Server *)));
	connect(this, SIGNAL(stopped(Server *)), obj, SLOT(stopped(Server *)));
}

void Meta::getVersion(int &major, int &minor, int &patch, QString &string) {
	string = QLatin1String(MUMBLE_RELEASE);
	major = minor = patch = 0;
	QRegExp rx(QLatin1String("(\\d+)\\.(\\d+)\\.(\\d+)"));
	if (rx.exactMatch(QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)))) {
		major = rx.cap(1).toInt();
		minor = rx.cap(2).toInt();
		patch = rx.cap(3).toInt();
	}
}
