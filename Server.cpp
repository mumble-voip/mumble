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

Server::Server() {
	m_qtsServer = new QTcpServer(this);

	connect(m_qtsServer, SIGNAL(newConnection()), this, SLOT(newClient()));

	if (! m_qtsServer->listen(QHostAddress::Any, 64738))
		qFatal("Server: Listen failed");
}

void Server::newClient() {
	Connection *cCon = new Connection(this, m_qtsServer->nextPendingConnection());
	Player *pPlayer = new Player();
	m_qmPlayers[cCon] = pPlayer;

	connect(cCon, SIGNAL(connectionClosed(Connection *)), this, SLOT(connectionClosed(Connection *)));
	connect(cCon, SIGNAL(message(Message *,Connection *, bool *)), this, SLOT(message(Message *, Connection *, bool *)));
}

void Server::connectionClosed(Connection *c) {
	Player *pPlayer = m_qmPlayers.value(c);

	if (pPlayer->state == Player::Authenticated) {
		MessageServerLeave mslMsg;
		mslMsg.m_sPlayerId=pPlayer->m_sId;
		sendExcept(&mslMsg, c);
	}

	m_qmConnections.remove(pPlayer->m_sId);
	m_qmPlayers.remove(c);

	qWarning("Connection closed");

	delete pPlayer;
	c->deleteLater();
}

void Server::message(Message *mMsg, Connection *cCon, bool *) {
	mMsg->process(cCon);
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
  Player *pPlayer = g_sServer->m_qmPlayers[cCon]; \
  if (pPlayer->state != st) \
  	return

void MessageServerJoin::process(Connection *cCon) {
	MSG_SETUP(Player::Connected);
	short id;

	for(id=1;id<32000;id++)
		if (! g_sServer->m_qmConnections.contains(id))
			break;

	qWarning("Player joined %s", m_qsPlayername.toLatin1().constData());

	pPlayer->m_sId = id;
	m_sPlayerId = id;
	g_sServer->sendAll(this);

	pPlayer->state = Player::Authenticated;
}

void MessageServerLeave::process(Connection *) {
}

void MessageSpeex::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	m_sPlayerId = pPlayer->m_sId;
	g_sServer->sendExcept(this, cCon);
}
