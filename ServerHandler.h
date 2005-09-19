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

#ifndef _SERVERHANDLER_H
#define _SERVERHANDLER_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QEvent>
#include <QHostAddress>
#include "Connection.h"
#include "Message.h"

#define SERVERSEND_EVENT 3501

class ServerHandlerMessageEvent : public QEvent
{
	public:
		QByteArray qbaMsg;
		bool bUdp;
		ServerHandlerMessageEvent(QByteArray &msg, bool udp);
};

class ServerHandler : public QThread
{
	Q_OBJECT
	protected:
		QString qsHostName;
		QString qsUserName;
		QString qsPassword;
		int iPort;
		bool bUdp;
		Connection *cConnection;

		QHostAddress qhaRemote;
		QUdpSocket *qusUdp;
	public:
		ServerHandler();
		~ServerHandler();
		void setConnectionInfo(QString host, int port, bool udp, QString username, QString pw);
		void customEvent(QEvent *evt);
		void sendMessage(Message *m);
		void disconnect();
		void run();
	signals:
		void disconnected(QString reason);
		void connected();
	protected slots:
		void message(QByteArray &);
		void serverConnectionConnected();
		void serverConnectionClosed(QString);
		void udpReady();
};

#else
class ServerHandler;
#endif
