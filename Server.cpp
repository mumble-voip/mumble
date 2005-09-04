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

#include "Server.h"

Server *g_sServer;
ServerParams g_sp;

ServerParams::ServerParams() {
	qsPassword = QString();
	bTestloop = false;
	iPort = 64738;
}

Server::Server() {
	m_qtsServer = new QTcpServer(this);

	connect(m_qtsServer, SIGNAL(newConnection()), this, SLOT(newClient()));

	if (! m_qtsServer->listen(QHostAddress::Any, g_sp.iPort))
		qFatal("Server: Listen failed");
}

void Server::newClient() {
	Connection *cCon = new Connection(this, m_qtsServer->nextPendingConnection());

	short id;

	for(id=1;id<32000;id++)
		if (! g_sServer->m_qmConnections.contains(id))
			break;

	Player *pPlayer = Player::add(id);
	m_qmPlayers[cCon] = pPlayer;

	connect(cCon, SIGNAL(connectionClosed(Connection *)), this, SLOT(connectionClosed(Connection *)));
	connect(cCon, SIGNAL(message(QByteArray &, Connection *)), this, SLOT(message(QByteArray &, Connection *)));
}

void Server::connectionClosed(Connection *c) {
	Player *pPlayer = m_qmPlayers.value(c);

	if (pPlayer->m_sState == Player::Authenticated) {
		MessageServerLeave mslMsg;
		mslMsg.m_sPlayerId=pPlayer->m_sId;
		sendExcept(&mslMsg, c);
	}

	m_qmConnections.remove(pPlayer->m_sId);
	m_qmPlayers.remove(c);

	qWarning("Connection closed");

	Player::remove(pPlayer);

	delete pPlayer;
	c->deleteLater();
}

void Server::message(QByteArray &qbaMsg, Connection *cCon) {
	  Message *mMsg = Message::networkToMessage(qbaMsg);
	  if (mMsg) {
		mMsg->process(cCon);
	  } else {
		cCon->disconnect();
	  }
}

void Server::sendAll(Message *mMsg) {
	sendExcept(mMsg, NULL);
}

void Server::sendExcept(Message *mMsg, Connection *cCon) {
	QMapIterator<Connection *, Player *> iPlayers(m_qmPlayers);
	while (iPlayers.hasNext()) {
		iPlayers.next();
		if (iPlayers.key() != cCon)
			iPlayers.key()->sendMessage(mMsg);
	}
}

#define MSG_SETUP(st) \
  Player *pSrcPlayer = g_sServer->m_qmPlayers[cCon]; \
  Player *pDstPlayer = Player::get(m_sPlayerId); \
  if (pSrcPlayer->m_sState != st) \
  	return

void MessageServerAuthenticate::process(Connection *cCon) {
	MSG_SETUP(Player::Connected);

	pSrcPlayer->m_qsName = m_qsUsername;
	m_sPlayerId = pSrcPlayer->m_sId;
	g_sServer->m_qmConnections[pSrcPlayer->m_sId] = cCon;
	
	MessageServerReject msr;
	bool ok = false;

	bool nameok = ! m_qsUsername.isEmpty();
	for(int i=0;i<m_qsUsername.length();i++) {
		QChar c=m_qsUsername[i];
		if (! c.isLetterOrNumber() && (c != ' '))
			ok = false;
	}

	if (m_iVersion != MESSAGE_STREAM_VERSION) {
	  msr.m_qsReason = "Wrong version of mumble protocol";
	} else if (! g_sp.qsPassword.isEmpty() && g_sp.qsPassword != m_qsPassword) {
	  msr.m_qsReason = "Invalid Password";
	} else if (! nameok) {
	  msr.m_qsReason = "Invalid Username";
	} else {
	  ok = true;
	}
	
	if (! ok) {
	  cCon->sendMessage(&msr);
	  cCon->disconnect();
  	  qWarning("Player %d:%s rejected (%s)", pSrcPlayer->m_sId, m_qsUsername.toLatin1().constData(), msr.m_qsReason.toLatin1().constData());
	  return;
	}

	MessageServerJoin msjMsg;

	pSrcPlayer->m_sState = Player::Authenticated;
	msjMsg.m_sPlayerId = pSrcPlayer->m_sId;
	msjMsg.m_qsPlayerName = pSrcPlayer->m_qsName;
	g_sServer->sendExcept(&msjMsg, cCon);

	QMapIterator<Connection *, Player *> iPlayers(g_sServer->m_qmPlayers);
	while (iPlayers.hasNext()) {
		iPlayers.next();
		Player *pPlayer = iPlayers.value();
		msjMsg.m_sPlayerId = pPlayer->m_sId;
		msjMsg.m_qsPlayerName = pPlayer->m_qsName;
		cCon->sendMessage(&msjMsg);

		if (pPlayer->m_bMute) {
			MessagePlayerMute mpmMsg;
			mpmMsg.m_sPlayerId = pPlayer->m_sId;
			mpmMsg.m_bMute = pPlayer->m_bMute;
			cCon->sendMessage(&mpmMsg);
		}
		if (pPlayer->m_bDeaf) {
			MessagePlayerDeaf mpdMsg;
			mpdMsg.m_sPlayerId = pPlayer->m_sId;
			mpdMsg.m_bDeaf = pPlayer->m_bDeaf;
			cCon->sendMessage(&mpdMsg);
		}
	}
	qWarning("Player %d:%s joined", pSrcPlayer->m_sId, m_qsUsername.toLatin1().constData());
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

void MessageSpeex::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	m_sPlayerId = pSrcPlayer->m_sId;

	if (pSrcPlayer->m_bMute)
		return;

	QMapIterator<Connection *, Player *> iPlayers(g_sServer->m_qmPlayers);
	while (iPlayers.hasNext()) {
		iPlayers.next();
		Player *pPlayer = iPlayers.value();
		if (! pPlayer->m_bDeaf && (g_sp.bTestloop || (pPlayer != pSrcPlayer)))
			iPlayers.key()->sendMessage(this);
	}
}

void MessagePlayerMute::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	pDstPlayer->m_bMute = m_bMute;

	g_sServer->sendAll(this);
}

void MessagePlayerDeaf::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	pDstPlayer->m_bDeaf = m_bDeaf;

	g_sServer->sendAll(this);
}

void MessagePlayerKick::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	cCon = g_sServer->m_qmConnections[m_sPlayerId];
	cCon->sendMessage(this);
	cCon->disconnect();
}
