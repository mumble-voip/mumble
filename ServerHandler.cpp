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


#include <QApplication>
#include <QHostAddress>
#include <QTimer>
#include "ServerHandler.h"
#include "MainWindow.h"
#include "AudioOutput.h"
#include "Message.h"
#include "Player.h"
#include "Connection.h"
#include "Global.h"

ServerHandlerMessageEvent::ServerHandlerMessageEvent(QByteArray &msg, bool udp) : QEvent(static_cast<QEvent::Type>(SERVERSEND_EVENT)) {
	qbaMsg = msg;
	bUdp = udp;
}

ServerHandler::ServerHandler()
{
	cConnection = NULL;
	qusUdp = NULL;
}

ServerHandler::~ServerHandler()
{
	wait();
}

void ServerHandler::customEvent(QEvent *evt) {
	if (evt->type() != SERVERSEND_EVENT)
		return;

	ServerHandlerMessageEvent *shme=static_cast<ServerHandlerMessageEvent *>(evt);

	if (cConnection) {
		if (shme->qbaMsg.size() > 0) {
			if (shme->bUdp) {
				if (! qusUdp) {
					if (cConnection->peerAddress().isNull())
						return;
					qusUdp = new QUdpSocket(this);
					qusUdp->bind();
					connect(qusUdp, SIGNAL(readyRead()), this, SLOT(udpReady()));
					qhaRemote = cConnection->peerAddress();
				}
				qusUdp->writeDatagram(shme->qbaMsg, qhaRemote, iPort);
			} else {
				cConnection->sendMessage(shme->qbaMsg);
			}
		} else
			cConnection->disconnect();
	}
}

void ServerHandler::udpReady() {
	while (qusUdp->hasPendingDatagrams()) {
		QByteArray qba;
		qba.resize(qusUdp->pendingDatagramSize());
		QHostAddress senderAddr;
		quint16 senderPort;
		qusUdp->readDatagram(qba.data(), qba.size(), &senderAddr, &senderPort);

		if (!(senderAddr == qhaRemote) || (senderPort != iPort))
			continue;

		Message *msg = Message::networkToMessage(qba);

		if (! msg)
			continue;
		if ((msg->messageType() != Message::Speex)  && (msg->messageType() != Message::MultiSpeex)) {
			delete msg;
			continue;
		}
        message(qba);
		delete msg;
	}
}

void ServerHandler::sendMessage(Message *mMsg)
{
	QByteArray qbaBuffer;
	mMsg->sPlayerId = g.sId;
	mMsg->messageToNetwork(qbaBuffer);

	bool mayUdp = bUdp && g.sId;
	mayUdp = mayUdp && ((mMsg->messageType() == Message::Speex) || (mMsg->messageType() == Message::MultiSpeex) || (mMsg->messageType() == Message::Ping));

	ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaBuffer, mayUdp);
	QApplication::postEvent(this, shme);
}

void ServerHandler::run()
{
	QTcpSocket *qtsSock = new QTcpSocket(this);
	cConnection = new Connection(this, qtsSock);
	qusUdp = NULL;

	connect(qtsSock, SIGNAL(connected()), this, SLOT(serverConnectionConnected()));
	connect(cConnection, SIGNAL(connectionClosed(QString)), this, SLOT(serverConnectionClosed(QString)));
	connect(cConnection, SIGNAL(message(QByteArray &)), this, SLOT(message(QByteArray &)));
	qtsSock->connectToHost(qsHostName, iPort);

	QTimer *ticker = new QTimer(this);
	connect(ticker, SIGNAL(timeout()), this, SLOT(sendPing()));
	ticker->start(10000);

	exec();

	ticker->stop();
	cConnection->disconnect();
	delete cConnection;
	cConnection = NULL;
	if (qusUdp) {
		delete qusUdp;
		qusUdp = NULL;
	}
}

void ServerHandler::sendPing() {
	bool oldudp = bUdp;

	MessagePing mp;
	bUdp = false;
	sendMessage(&mp);
	bUdp = oldudp;
	if (bUdp)
		sendMessage(&mp);
}

void ServerHandler::message(QByteArray &qbaMsg) {
	Message *mMsg = Message::networkToMessage(qbaMsg);
	if (! mMsg)
		return;

	Player *p = Player::get(mMsg->sPlayerId);

	if (mMsg->messageType() == Message::Speex) {
		QWriteLocker alocka(&g.qrwlAudio);
		if (g.ao) {
			if (p) {
				MessageSpeex *msMsg=static_cast<MessageSpeex *>(mMsg);
				g.ao->addFrameToBuffer(p, msMsg->qbaSpeexPacket, msMsg->iSeq);
			} else {
				// Eek, we just got a late packet for a player already removed. Remove
				// the buffer and pretend this never happened.
				// If ~AudioOutputPlayer or decendants uses the Player object now,
				// Bad Things happen.
				g.ao->removeBuffer(p);
			}
		}
	} else if (mMsg->messageType() == Message::MultiSpeex) {
		QWriteLocker alocka(&g.qrwlAudio);
		if (g.ao) {
			if (p) {
				MessageMultiSpeex *mmsMsg=static_cast<MessageMultiSpeex *>(mMsg);
				int idx = 0;
				foreach(QByteArray qba, mmsMsg->qlFrames) {
					g.ao->addFrameToBuffer(p, qba, mmsMsg->iSeq + idx);
					idx++;
				}
			} else {
				g.ao->removeBuffer(p);
			}
		}
	} else {
		if(mMsg->messageType() == Message::ServerLeave) {
			QReadLocker alockb(&g.qrwlAudio);
			if (g.ao)
				g.ao->removeBuffer(p);
		}
		ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaMsg, false);
		QApplication::postEvent(g.mw, shme);
	}

	delete mMsg;
}

void ServerHandler::disconnect() {
	// Actual TCP object is in a different thread, so signal it
	QByteArray qbaBuffer;
	ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaBuffer, false);
	QApplication::postEvent(this, shme);
}

void ServerHandler::serverConnectionClosed(QString reason) {
	{
		QReadLocker alock(&g.qrwlAudio);
		if (g.ao)
			g.ao->wipe();
	}

	emit disconnected(reason);
	exit(0);
}

void ServerHandler::serverConnectionConnected() {
	MessageServerAuthenticate msaMsg;
	msaMsg.qsUsername = qsUserName;
	msaMsg.qsPassword = qsPassword;
	cConnection->sendMessage(&msaMsg);
	emit connected();
}

void ServerHandler::setConnectionInfo(QString host, int port, bool udp, QString username, QString pw) {
	qsHostName = host;
	iPort = port;
	bUdp = udp;
	qsUserName = username;
	qsPassword = pw;
}
