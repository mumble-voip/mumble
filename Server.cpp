/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#include <QDateTime>
#include <QRegExp>
#include "Server.h"
#include "ServerDB.h"

Server *g_sServer;
ServerParams g_sp;

uint qHash(const Peer &p) {
	return qHash(p.first.toIPv4Address()) ^ qHash(p.second);
}

ServerParams::ServerParams() {
	qsPassword = QString();
	bTestloop = false;
	iPort = 64738;
	qsWelcomeText = QString("Welcome to this server");
}

Server::Server() {
	qtsServer = new QTcpServer(this);

	connect(qtsServer, SIGNAL(newConnection()), this, SLOT(newClient()));

	if (! qtsServer->listen(QHostAddress::Any, g_sp.iPort))
		qFatal("Server: Listen failed");

	qusUdp = new QUdpSocket(this);
	if (! qusUdp->bind(g_sp.iPort))
		qFatal("Server: UDP Bind failed");

	connect(qusUdp, SIGNAL(readyRead()), this, SLOT(udpReady()));

	log(QString("Server listening on port %1").arg(g_sp.iPort));

	for(int i=1;i<255;i++)
		qqIds.enqueue(i);
}

void Server::udpReady() {
	while (qusUdp->hasPendingDatagrams()) {
		QByteArray qba;
		qba.resize(qusUdp->pendingDatagramSize());
	    QHostAddress senderAddr;
        quint16 senderPort;
		qusUdp->readDatagram(qba.data(), qba.size(), &senderAddr, &senderPort);
		Message *msg = Message::networkToMessage(qba);
		if (! msg)
			continue;
		if (msg->messageType() != Message::Speex) {
			delete msg;
			continue;
		}
		Peer p(senderAddr, senderPort);

		Connection *source;

		source = qhPeerConnections.value(p);

		if (source != qmConnections.value(msg->sPlayerId)) {
			source = qmConnections.value(msg->sPlayerId);
			if (! source || ! (source->peerAddress() == senderAddr)) {
				delete msg;
				continue;
			}
			qhPeerConnections[p] = source;
			qhPeers[source] = p;
		}
		message(qba, source);
		delete msg;
	}
}

void Server::log(QString s, Connection *c) {
	if (c) {
		Player *p = qmPlayers.value(c);

		int id = 0;
		int iid = -1;
		QString name;
		if (p) {
			id = p->sId;
			iid = p->iId;
			name = p->qsName;
		}
		qWarning("[%s] <%d:%s(%d)> %s", QDateTime::currentDateTime().toString(Qt::ISODate).toAscii().constData(),
				id, name.toAscii().constData(), iid, s.toAscii().constData());
	} else {
		qWarning("[%s] %s", QDateTime::currentDateTime().toString(Qt::ISODate).toAscii().constData(),
				s.toAscii().constData());
	}
}

void Server::newClient() {
	QTcpSocket *sock = qtsServer->nextPendingConnection();
	Connection *cCon = new Connection(this, sock);

	short id;

	if (qqIds.isEmpty()) {
		cCon->disconnect();
		return;
	}

	id=qqIds.dequeue();

	Player *pPlayer = Player::add(id);
	qmPlayers[cCon] = pPlayer;
	qmConnections[id] = cCon;

	connect(cCon, SIGNAL(connectionClosed(QString)), this, SLOT(connectionClosed(QString)));
	connect(cCon, SIGNAL(message(QByteArray &)), this, SLOT(message(QByteArray &)));

	log(QString("New connection: %1:%2").arg(sock->peerAddress().toString()).arg(sock->peerPort()), cCon);
}

void Server::connectionClosed(QString reason) {
	Connection *c = static_cast<Connection *>(sender());
	Player *pPlayer = qmPlayers.value(c);

	log(QString("Connection closed: %1").arg(reason), c);

	if (pPlayer->sState == Player::Authenticated) {
		MessageServerLeave mslMsg;
		mslMsg.sPlayerId=pPlayer->sId;
		sendExcept(&mslMsg, c);
	}

	qmConnections.remove(pPlayer->sId);
	qmPlayers.remove(c);

	Player::remove(pPlayer);

	qqIds.enqueue(pPlayer->sId);

	Peer udppeer = qhPeers.take(c);
	qhPeerConnections.remove(udppeer);

	delete pPlayer;
	c->deleteLater();
}

void Server::message(QByteArray &qbaMsg, Connection *cCon) {
        if (cCon == NULL)
		cCon = static_cast<Connection *>(sender());
	Message *mMsg = Message::networkToMessage(qbaMsg);

	  // Just leftovers from the buffer and we just kicked
	  // the user off.
	  if (! qmPlayers.contains(cCon))
	  	return;

	  if (mMsg) {
		mMsg->process(cCon);
		delete mMsg;
	  } else {
		cCon->disconnect();
	  }
}

void Server::sendMessage(short id, Message *mMsg) {
	Connection *c = qmConnections.value(id);
	sendMessage(c, mMsg);
}

void Server::sendMessage(Connection *c, Message *mMsg) {
	if ((mMsg->messageType() == Message::Speex) && qhPeers.contains(c)) {
		Peer p = qhPeers[c];
		QByteArray qba;
		mMsg->messageToNetwork(qba);
		qusUdp->writeDatagram(qba, p.first, p.second);
	} else {
		c->sendMessage(mMsg);
	}
}

void Server::sendAll(Message *mMsg) {
	sendExcept(mMsg, NULL);
}

void Server::sendExcept(Message *mMsg, Connection *cCon) {
	QHash<Connection *, Player *>::const_iterator i;
	for(i=qmPlayers.constBegin(); i != qmPlayers.constEnd(); ++i) {
		if (i.key() != cCon)
			sendMessage(i.key(), mMsg);
	}
}

void Server::removeChannel(Channel *chan, Player *src, Channel *dest) {
	Channel *c;
	Player *p;

	if (dest == NULL)
		dest = Channel::get(chan->iParent);

	foreach(c, chan->qlChannels) {
		removeChannel(c, src, dest);
	}

	foreach(p, chan->qlPlayers) {
		chan->removePlayer(p);

		MessagePlayerMove mpm;
		mpm.sPlayerId = 0;
		mpm.sVictim = p->sId;
		mpm.iChannelId = dest->iId;
		sendAll(&mpm);

		playerEnterChannel(p, dest);

		ServerDB::setLastChannel(p);
	}

	MessageChannelRemove mcr;
	mcr.sPlayerId = src->sId;
	mcr.iId = chan->iId;
	sendAll(&mcr);

	ServerDB::removeChannel(chan);

	if (chan->cParent)
		chan->cParent->removeChannel(chan);

	delete chan;
}

void Server::playerEnterChannel(Player *p, Channel *c) {
	if (p->cChannel == c)
		return;

	c->addPlayer(p);

	bool mayspeak = ChanACL::hasPermission(p, c, ChanACL::Speak);
	bool sup = p->bSuppressed;

	if (! p->bMute) {
		if (mayspeak == sup) {
			// Ok, he can speak and was suppressed, or vice versa
			p->bSuppressed = ! mayspeak;

			MessagePlayerMute mpm;
			mpm.sPlayerId = p->sId;
			mpm.bMute = p->bSuppressed;
			g_sServer->sendAll(&mpm);
		}
	}
}

#define MSG_SETUP(st) \
	Player *pSrcPlayer = g_sServer->qmPlayers[cCon]; \
	MessagePermissionDenied mpd; \
	sPlayerId = pSrcPlayer->sId; \
	if (pSrcPlayer->sState != st) \
		return

#define VICTIM_SETUP \
	Player *pDstPlayer = Player::get(sVictim); \
	Q_UNUSED(pDstPlayer) \
	Connection *cDst = g_sServer->qmConnections.value(sVictim); \
	Q_UNUSED(cDst)

#define PERM_DENIED(who, where, what) \
	mpd.qsReason = QString("%1 not allowed to %2 in %3").arg(who->qsName).arg(ChanACL::permName(what)).arg(where->qsName); \
	cCon->sendMessage(&mpd); \
	g_sServer->log(mpd.qsReason, cCon)
#define PERM_DENIED_TEXT(text) \
	mpd.qsReason = text; \
	cCon->sendMessage(&mpd)


void MessageServerAuthenticate::process(Connection *cCon) {
	MSG_SETUP(Player::Connected);

	Channel *c;

	pSrcPlayer->qsName = qsUsername;

	MessageServerReject msr;
	bool ok = false;

	QRegExp re("[\\w\\[\\]\\{\\}\\(\\)\\@\\|]+");

	bool nameok = re.exactMatch(qsUsername);
	if (nameok && qsUsername[0] == '@')
		nameok = false;
	if (nameok && qsUsername[0] == '#')
		nameok = false;

	// Fetch ID and stored username
	int id = ServerDB::authenticate(qsUsername, qsPassword);
	pSrcPlayer->iId = id;
	pSrcPlayer->qsName = qsUsername;

	if (iVersion != MESSAGE_STREAM_VERSION) {
	  msr.qsReason = "Wrong version of mumble protocol";
	} else if (! nameok) {
	  msr.qsReason = "Invalid Username";
	} else if ((id==-1) || (id==-2 && ! g_sp.qsPassword.isEmpty() && g_sp.qsPassword != qsPassword)) {
	  msr.qsReason = "Invalid Password";
	} else {
	  ok = true;
	}

	if (! ok) {
	  g_sServer->log(QString("Rejected connection: %1").arg(msr.qsReason), cCon);
	  g_sServer->sendMessage(cCon, &msr);
	  cCon->disconnect();
	  return;
	}

	int lchan = ServerDB::readLastChannel(pSrcPlayer);
	Channel *lc = Channel::get(lchan);
	if (! lc)
		lc = Channel::get(0);
	else if (! ChanACL::hasPermission(pSrcPlayer, lc, ChanACL::Enter))
		lc = Channel::get(0);

	g_sServer->playerEnterChannel(pSrcPlayer, lc);

	QQueue<Channel *> q;
	q << Channel::get(0);
	while (! q.isEmpty()) {
		c = q.dequeue();
		MessageChannelAdd mca;
		mca.sPlayerId = 0;
		mca.iId = c->iId;
		mca.iParent = c->iParent;
		mca.qsName = c->qsName;
		if (c->iId != 0)
			g_sServer->sendMessage(cCon, &mca);

		foreach(c, c->qlChannels)
			q.enqueue(c);
	}

	MessageServerJoin msjMsg;

	pSrcPlayer->sState = Player::Authenticated;
	msjMsg.sPlayerId = pSrcPlayer->sId;
	msjMsg.iId = pSrcPlayer->iId;
	msjMsg.qsPlayerName = pSrcPlayer->qsName;
	g_sServer->sendExcept(&msjMsg, cCon);

	foreach(Player *pPlayer, g_sServer->qmPlayers) {
		msjMsg.sPlayerId = pPlayer->sId;
		msjMsg.iId = pPlayer->iId;
		msjMsg.qsPlayerName = pPlayer->qsName;
		g_sServer->sendMessage(cCon, &msjMsg);

		if (pPlayer->bDeaf) {
			MessagePlayerDeaf mpdMsg;
			mpdMsg.sPlayerId = 0;
			mpdMsg.sVictim = pPlayer->sId;
			mpdMsg.bDeaf = true;
			g_sServer->sendMessage(cCon, &mpdMsg);
		} else if (pPlayer->bMute || pPlayer->bSuppressed) {
			MessagePlayerMute mpmMsg;
			mpmMsg.sPlayerId = 0;
			mpmMsg.sVictim = pPlayer->sId;
			mpmMsg.bMute = true;
			g_sServer->sendMessage(cCon, &mpmMsg);
		}
		if (pPlayer->bSelfDeaf || pPlayer->bSelfMute) {
			MessagePlayerSelfMuteDeaf mpsmdMsg;
			mpsmdMsg.sPlayerId = pPlayer->sId;
			mpsmdMsg.bDeaf = pPlayer->bSelfDeaf;
			mpsmdMsg.bMute = pPlayer->bSelfMute;
			g_sServer->sendMessage(cCon, &mpsmdMsg);
		}

		MessagePlayerMove mpm;
		mpm.sPlayerId = 0;
		mpm.sVictim = pPlayer->sId;
		mpm.iChannelId = pPlayer->cChannel->iId;
		g_sServer->sendMessage(cCon, &mpm);
	}

	MessageServerSync mssMsg;
	mssMsg.sPlayerId = pSrcPlayer->sId;
	mssMsg.qsWelcomeText = g_sp.qsWelcomeText;
	g_sServer->sendMessage(cCon, &mssMsg);
	g_sServer->log(QString("Authenticated: %1").arg(qsUsername), cCon);
}

void MessageServerLeave::process(Connection *cCon) {
  cCon->disconnect();
}

void MessageServerJoin::process(Connection *cCon) {
  cCon->disconnect();
}

void MessageServerReject::process(Connection *cCon) {
  cCon->disconnect();
}

void MessageServerSync::process(Connection *cCon) {
  cCon->disconnect();
}

void MessagePermissionDenied::process(Connection *cCon) {
  cCon->disconnect();
}

void MessageSpeex::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	Player *p;

	if (pSrcPlayer->bMute || pSrcPlayer->bSuppressed)
		return;

	foreach(p, pSrcPlayer->cChannel->qlPlayers) {
		if (! p->bDeaf && ! p->bSelfDeaf && (g_sp.bTestloop || (p != pSrcPlayer)))
			g_sServer->sendMessage(p->sId, this);
	}
}

void MessagePlayerMute::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MuteDeafen)) {
		PERM_DENIED(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MuteDeafen);
		return;
	}

	if (! bMute && pDstPlayer->bSuppressed) {
		pDstPlayer->bSuppressed = false;
	} else if (pDstPlayer->bMute == bMute) {
		return;
	}

	pDstPlayer->bMute = bMute;
	g_sServer->sendAll(this);

	if (! bMute && pDstPlayer->bDeaf) {
		pDstPlayer->bDeaf = false;
	}
	g_sServer->log(QString("Muted %1 (%2)").arg(pDstPlayer->qsName).arg(bMute), cCon);
}

void MessagePlayerDeaf::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MuteDeafen)) {
		PERM_DENIED(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MuteDeafen);
		return;
	}

	if (pDstPlayer->bDeaf == bDeaf)
		return;

	pDstPlayer->bDeaf = bDeaf;
	g_sServer->sendAll(this);

	if (bDeaf && ! pDstPlayer->bMute) {
		pDstPlayer->bMute = true;
	}
	g_sServer->log(QString("Deafened %1 (%2)").arg(pDstPlayer->qsName).arg(bDeaf), cCon);
}

void MessagePlayerKick::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MoveKick)) {
		PERM_DENIED(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MoveKick);
		return;
	}

	g_sServer->sendAll(this);
	g_sServer->log(QString("Kicked %1 (%2)").arg(pDstPlayer->qsName).arg(qsReason), cCon);
	cDst->disconnect();
}

void MessagePlayerSelfMuteDeaf::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	pSrcPlayer->bSelfMute = bMute;
	pSrcPlayer->bSelfDeaf = bDeaf;
	g_sServer->sendAll(this);
}

void MessagePlayerMove::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	Channel *c = Channel::get(iChannelId);
	if (!c || (c == pDstPlayer->cChannel))
		return;

	if ((pSrcPlayer != pDstPlayer) && ! ChanACL::hasPermission(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MoveKick)) {
		PERM_DENIED(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MoveKick);
		return;
	}

	if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::MoveKick) && ! ChanACL::hasPermission(pDstPlayer, c, ChanACL::Enter)) {
		PERM_DENIED(pDstPlayer, c, ChanACL::Enter);
		return;
	}


	g_sServer->sendAll(this);
	g_sServer->playerEnterChannel(pDstPlayer, c);

	ServerDB::setLastChannel(pDstPlayer);
	g_sServer->log(QString("Moved to %1 (%2)").arg(c->qsName).arg(pDstPlayer->qsName), cCon);
}

void MessageChannelAdd::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	Channel *p = Channel::get(iParent);
	if (!p)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, p, ChanACL::MakeChannel)) {
		PERM_DENIED(pSrcPlayer, p, ChanACL::MakeChannel);
		return;
	}

	QRegExp re("[\\w\\[\\]\\{\\}\\(\\)\\@\\|]+");

	if (! re.exactMatch(qsName)) {
		PERM_DENIED_TEXT("Illegal channel name");
		return;
	}

	Channel *c = ServerDB::addChannel(p, qsName);
	if (pSrcPlayer->iId >= 0) {
		Group *g = new Group(c, "admin");
		g->qsAdd << pSrcPlayer->iId;
	}
	ServerDB::updateChannel(c);

	iId = c->iId;
	g_sServer->sendAll(this);
	g_sServer->log(QString("Added channel %1 (%2)").arg(qsName).arg(p->qsName), cCon);
}

void MessageChannelRemove::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = Channel::get(iId);
	if (!c)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::Write) || (iId == 0)) {
		PERM_DENIED(pSrcPlayer, c, ChanACL::Write);
		return;
	}

	g_sServer->log(QString("Removed channel %1").arg(c->qsName), cCon);

	g_sServer->removeChannel(c, pSrcPlayer);
}

void MessageChannelMove::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = Channel::get(iId);
	Channel *np = Channel::get(iParent);
	if (!c || ! np)
		return;

	if (np == c->cParent)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::Write)) {
		PERM_DENIED(pSrcPlayer, c, ChanACL::Write);
		return;
	}

	if (! ChanACL::hasPermission(pSrcPlayer, np, ChanACL::MakeChannel)) {
		PERM_DENIED(pSrcPlayer, np, ChanACL::MakeChannel);
		return;
	}

	// Can't move to a subchannel of itself
	Channel *p = np;
	while (p) {
		if (p == c)
			return;
		p = p->cParent;
	}

	g_sServer->log(QString("Moved channel %1 (%2 -> %3)").arg(c->qsName).arg(c->cParent->qsName).arg(np->qsName), cCon);

	c->cParent->removeChannel(c);
	np->addChannel(c);
	ServerDB::updateChannel(c);
	g_sServer->sendAll(this);
}
