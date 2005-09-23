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

#include <QHostAddress>
#include "Connection.h"

Connection::Connection(QObject *p, QTcpSocket *qtsSock) : QObject(p) {
	qtsSocket = qtsSock;
	qtsSocket->setParent(this);
	iPacketLength = -1;
	bDisconnectedEmitted = false;
    connect(qtsSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(qtsSocket, SIGNAL(readyRead()), this, SLOT(socketRead()));
    connect(qtsSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

Connection::~Connection() {
}

void Connection::socketRead() {
  int iAvailable;
  while (1) {
    iAvailable = qtsSocket->bytesAvailable();

    if (iPacketLength == -1) {
      if (iAvailable < 2)
        return;

      unsigned char a_ucBuffer[2];

	  qtsSocket->read(reinterpret_cast<char *>(a_ucBuffer), 2);
      iPacketLength = ((a_ucBuffer[0] << 8) & 0xff00) + a_ucBuffer[1];
      iAvailable -= 2;
    }

    if ((iPacketLength != -1) && (iAvailable >= iPacketLength)) {
	  QByteArray qbaBuffer = qtsSocket->read(iPacketLength);
	  emit message(qbaBuffer);
      iPacketLength = -1;
    } else {
      return;
    }
  }
}

void Connection::socketError(QAbstractSocket::SocketError) {
	if (! bDisconnectedEmitted) {
		bDisconnectedEmitted = true;
		emit connectionClosed(qtsSocket->errorString());
	}
	qtsSocket->disconnectFromHost();
}

void Connection::socketDisconnected() {
	if (! bDisconnectedEmitted) {
		bDisconnectedEmitted = true;
		emit connectionClosed(QString());
	}
}

void Connection::sendMessage(Message *mMsg) {
	QByteArray qbaBuffer;

	mMsg->messageToNetwork(qbaBuffer);
	sendMessage(qbaBuffer);
}

void Connection::sendMessage(QByteArray &qbaMsg) {
	unsigned char a_ucBuffer[2];

	if (qtsSocket->state() != QAbstractSocket::ConnectedState)
		return;

	if (qbaMsg.size() > 0xffff) {
		qFatal("Connection: Oversized message (%d bytes)", qbaMsg.size());
	}

	a_ucBuffer[0]=(qbaMsg.size() >> 8) & 0xff;
	a_ucBuffer[1]=(qbaMsg.size() & 0xff);
	qtsSocket->write(reinterpret_cast<const char *>(a_ucBuffer), 2);
	qtsSocket->write(qbaMsg);
}

void Connection::disconnect() {
	qtsSocket->disconnectFromHost();
}


QHostAddress Connection::peerAddress() const {
	return qtsSocket->peerAddress();
}

quint16 Connection::peerPort() const {
	return qtsSocket->peerPort();
}
