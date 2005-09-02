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

#include "ServerHandler.h"
#include "MainWindow.h"
#include "AudioOutput.h"
#include <QApplication>

extern MainWindow *g_mwMainWindow;

ServerHandler *g_shServer;

ServerHandlerMessageEvent::ServerHandlerMessageEvent(QByteArray &msg) : QEvent((QEvent::Type) SERVERSEND_EVENT) {
	qbaMsg = msg;
}

ServerHandler::ServerHandler()
{
	cConnection = NULL;
}

ServerHandler::~ServerHandler()
{
}

void ServerHandler::customEvent(QEvent *evt) {
	if (evt->type() != SERVERSEND_EVENT)
		return;

	ServerHandlerMessageEvent *shme=static_cast<ServerHandlerMessageEvent *>(evt);

	if (cConnection) {
		cConnection->sendMessage(shme->qbaMsg);
	}
}

void ServerHandler::sendMessage(Message *mMsg)
{
	QByteArray qbaBuffer;
	mMsg->messageToNetwork(qbaBuffer);
	ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaBuffer);
	QApplication::postEvent(this, shme);
}

void ServerHandler::run()
{
	QTcpSocket *qtsSock = new QTcpSocket(this);
	cConnection = new Connection(this, qtsSock);

	connect(qtsSock, SIGNAL(connected()), this, SLOT(serverConnectionConnected()));
	connect(cConnection, SIGNAL(connectionClosed(Connection *)), this, SLOT(serverConnectionClosed(Connection *)));
	connect(cConnection, SIGNAL(message(QByteArray &, Connection *)), this, SLOT(message(QByteArray &, Connection *)));
	qtsSock->connectToHost(m_qsHostName, 64738);
	exec();
	cConnection->disconnect();
	delete cConnection;
	delete qtsSock;
}

void ServerHandler::message(QByteArray &qbaMsg, Connection *) {
	// Handle speex directly (into player threads)
	// But for now....

	Message *mMsg = Message::networkToMessage(qbaMsg);
	if (! mMsg)
		return;

	if (mMsg->messageType() == Message::Speex) {
		if (g_aoOutput)
			g_aoOutput->addFrameToBuffer(mMsg->m_sPlayerId, static_cast<MessageSpeex *>(mMsg)->m_qbaSpeexPacket);
	} else {
		if (mMsg->messageType() == Message::ServerLeave)
			if (g_aoOutput)
				g_aoOutput->removeBuffer(mMsg->m_sPlayerId);
		ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaMsg);
		QApplication::postEvent(g_mwMainWindow, shme);
	}

	delete mMsg;
}

void ServerHandler::disconnect() {
	cConnection->disconnect();
}

void ServerHandler::serverConnectionClosed(Connection *) {
	g_aoOutput->wipe();

	emit disconnected();

	exit(0);
	// Clean up player threads
}

void ServerHandler::serverConnectionConnected() {
	MessageServerJoin msjMsg;
	msjMsg.m_qsPlayerName = m_qsUserName;
	cConnection->sendMessage(&msjMsg);

	emit connected();
}


void ServerHandler::setConnectionInfo(QString qsHostName, QString qsUserName) {
	m_qsHostName = qsHostName;
	m_qsUserName = qsUserName;
}
