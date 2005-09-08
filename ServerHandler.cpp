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
#include "Global.h"

#include <QApplication>

ServerHandlerMessageEvent::ServerHandlerMessageEvent(QByteArray &msg) : QEvent(static_cast<QEvent::Type>(SERVERSEND_EVENT)) {
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
		if (shme->qbaMsg.size() > 0)
			cConnection->sendMessage(shme->qbaMsg);
		else
			cConnection->disconnect();
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
	connect(cConnection, SIGNAL(connectionClosed(QString)), this, SLOT(serverConnectionClosed(QString)));
	connect(cConnection, SIGNAL(message(QByteArray &)), this, SLOT(message(QByteArray &)));
	qtsSock->connectToHost(qsHostName, iPort);
	exec();
	cConnection->disconnect();
	delete cConnection;
	delete qtsSock;
}

void ServerHandler::message(QByteArray &qbaMsg) {
	Message *mMsg = Message::networkToMessage(qbaMsg);
	if (! mMsg)
		return;

	if (mMsg->messageType() == Message::Speex) {
		if (g.ao) {
			MessageSpeex *msMsg=static_cast<MessageSpeex *>(mMsg);
			g.ao->addFrameToBuffer(mMsg->sPlayerId, msMsg->qbaSpeexPacket, msMsg->iSeq);
		}
	} else {
		switch(mMsg->messageType()) {
			case Message::ServerLeave:
				if (g.ao)
					g.ao->removeBuffer(mMsg->sPlayerId);
				break;
			default:
				break;
		}
		ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaMsg);
		QApplication::postEvent(g.mw, shme);
	}

	delete mMsg;
}

void ServerHandler::disconnect() {
	// Actual TCP object is in a different thread, so signal it
	QByteArray qbaBuffer;
	ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaBuffer);
	QApplication::postEvent(this, shme);
}

void ServerHandler::serverConnectionClosed(QString reason) {
	g.ao->wipe();

	emit disconnected(reason);

	exit(0);
	// Clean up player threads
}

void ServerHandler::serverConnectionConnected() {
	MessageServerAuthenticate msaMsg;
	msaMsg.qsUsername = qsUserName;
	msaMsg.qsPassword = qsPassword;
	cConnection->sendMessage(&msaMsg);
	emit connected();
}

void ServerHandler::setConnectionInfo(QString host, int port, QString username, QString pw) {
	qsHostName = host;
	iPort = port;
	qsUserName = username;
	qsPassword = pw;
}
