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

#include "Connection.h"

Connection::Connection(QObject *parent, QTcpSocket *qtsSock) : QObject(parent) {
	m_qtsSocket = qtsSock;
	m_iPacketLength = -1;
    connect(m_qtsSocket, SIGNAL(error(SocketError)), this, SLOT(socketError(SocketError)));
    connect(m_qtsSocket, SIGNAL(readyRead()), this, SLOT(socketRead()));
}

Connection::~Connection() {
	m_qtsSocket->deleteLater();
}

void Connection::socketRead() {
  int iAvailable;
  while (1) {
    iAvailable = m_qtsSocket->bytesAvailable();

    if (m_iPacketLength == -1) {
      if (iAvailable < 2)
        return;

      unsigned char a_ucBuffer[2];

	  m_qtsSocket->read((char *) a_ucBuffer, 2);
      m_iPacketLength = ((a_ucBuffer[0] << 8) & 0xff00) + a_ucBuffer[1];
      iAvailable -= 2;
    }

    if ((m_iPacketLength != -1) && (iAvailable >= m_iPacketLength)) {
	  QByteArray qbaBuffer = m_qtsSocket->read(m_iPacketLength);
	  Message *mMsg = Message::networkToMessage(qbaBuffer);
	  if (mMsg) {
		  mMsg->process(this);
		  delete mMsg;
	  } else {
		  disconnect();
	  }
      m_iPacketLength = -1;
    } else {
      return;
    }
  }
}

void Connection::socketError(QAbstractSocket::SocketError) {
	emit connectionClosed(this);
	m_qtsSocket->disconnectFromHost();
}

void Connection::sendMessage(Message *mMsg) {
	QByteArray qbaBuffer;
	unsigned char a_ucBuffer[2];

	mMsg->messageToNetwork(qbaBuffer);
	a_ucBuffer[0]=(qbaBuffer.size() >> 8) & 0xff;
	a_ucBuffer[1]=(qbaBuffer.size() & 0xff);
	m_qtsSocket->write((const char *) a_ucBuffer, 2);
	m_qtsSocket->write(qbaBuffer);
}

void Connection::disconnect() {
	emit connectionClosed(this);
	m_qtsSocket->disconnectFromHost();
}
