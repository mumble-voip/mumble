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
		QString name;
		if (p) {
			id = p->sId;
			name = p->qsName;
		}
		qWarning("[%s] <%d:%s> %s", QDateTime::currentDateTime().toString(Qt::ISODate).toAscii().constData(),
				id, name.toAscii().constData(), s.toAscii().constData());
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

#define MSG_SETUP(st) \
	Player *pSrcPlayer = g_sServer->qmPlayers[cCon]; \
	sPlayerId = pSrcPlayer->sId; \
	if (pSrcPlayer->sState != st) \
		return

#define VICTIM_SETUP \
	Player *pDstPlayer = Player::get(sVictim); \
	Q_UNUSED(pDstPlayer) \
	Connection *cDst = g_sServer->qmConnections.value(sVictim); \
	Q_UNUSED(cDst)

void MessageServerAuthenticate::process(Connection *cCon) {
	MSG_SETUP(Player::Connected);

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
			mpdMsg.sPlayerId = pPlayer->sId;
			mpdMsg.bDeaf = pPlayer->bDeaf;
			g_sServer->sendMessage(cCon, &mpdMsg);
		} else if (pPlayer->bMute) {
			MessagePlayerMute mpmMsg;
			mpmMsg.sPlayerId = pPlayer->sId;
			mpmMsg.bMute = pPlayer->bMute;
			g_sServer->sendMessage(cCon, &mpmMsg);
		}
		if (pPlayer->bSelfDeaf || pPlayer->bSelfMute) {
			MessagePlayerSelfMuteDeaf mpsmdMsg;
			mpsmdMsg.sPlayerId = pPlayer->sId;
			mpsmdMsg.bDeaf = pPlayer->bSelfDeaf;
			mpsmdMsg.bMute = pPlayer->bSelfMute;
			g_sServer->sendMessage(cCon, &mpsmdMsg);
		}
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

void MessageSpeex::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	if (pSrcPlayer->bMute)
		return;

	QHash<Connection *, Player *>::const_iterator i;
	for (i=g_sServer->qmPlayers.constBegin(); i != g_sServer->qmPlayers.constEnd(); ++i) {
		Player *pPlayer = i.value();
		if (! pPlayer->bDeaf && ! pPlayer->bSelfDeaf && (g_sp.bTestloop || (pPlayer != pSrcPlayer)))
			g_sServer->sendMessage(i.key(), this);
	}
}

void MessagePlayerMute::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (pSrcPlayer->iId < 0)
		return;

	if (pDstPlayer) {
		if (pDstPlayer->bMute == bMute)
			return;

		pDstPlayer->bMute = bMute;
		g_sServer->sendAll(this);

		if (! bMute && pDstPlayer->bDeaf) {
			pDstPlayer->bDeaf = false;
		}
	}
}

void MessagePlayerDeaf::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (pSrcPlayer->iId < 0)
		return;

	if (pDstPlayer) {
		if (pDstPlayer->bDeaf == bDeaf)
			return;

		pDstPlayer->bDeaf = bDeaf;
		g_sServer->sendAll(this);

		if (bDeaf && ! pDstPlayer->bMute) {
			pDstPlayer->bMute = true;
		}
	}
}

void MessagePlayerKick::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (pSrcPlayer->iId < 0)
		return;

	sPlayerId = pSrcPlayer->sId;
	if (cDst) {
		g_sServer->sendAll(this);
		cDst->disconnect();
	}
}

void MessagePlayerSelfMuteDeaf::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	pSrcPlayer->bSelfMute = bMute;
	pSrcPlayer->bSelfDeaf = bDeaf;
	g_sServer->sendAll(this);
}
