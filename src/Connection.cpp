/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "Message.h"
#include "Connection.h"

/*!
  \fn void Connection::socketRead()
  This function waits until a complete package is received and then emits it as a message.
  It gets called everytime new data is available and interprets the message prefix header
  to figure out the type and length. It then waits until the complete message is buffered
  and emits it as a message so it can be handled by the corresponding message handler
  routine.

  \see QSslSocket::readyRead()
  \see void ServerHandler::message(unsigned int msgType, const QByteArray &qbaMsg)
  \see void Server::message(unsigned int uiType, const QByteArray &qbaMsg, ServerUser *u)
*/

#ifdef Q_OS_UNIX
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

#ifdef Q_OS_WIN
HANDLE Connection::hQoS = NULL;
#endif

Connection::Connection(QObject *p, QSslSocket *qtsSock) : QObject(p) {
	qtsSocket = qtsSock;
	qtsSocket->setParent(this);
	iPacketLength = -1;
	bDisconnectedEmitted = false;

	static bool bDeclared = false;
	if (! bDeclared) {
		bDeclared = true;
		qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
	}

	connect(qtsSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(qtsSocket, SIGNAL(encrypted()), this, SIGNAL(encrypted()));
	connect(qtsSocket, SIGNAL(readyRead()), this, SLOT(socketRead()));
	connect(qtsSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	connect(qtsSocket, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(socketSslErrors(const QList<QSslError> &)));
	qtLastPacket.restart();
#ifdef Q_OS_WIN
	dwFlow = 0;
#endif


}

Connection::~Connection() {
#ifdef Q_OS_WIN
	if (dwFlow && hQoS) {
		if (! QOSRemoveSocketFromFlow(hQoS, 0, dwFlow, 0))
			qWarning("Connection: Failed to remove flow from QoS");
	}
#endif
}

void Connection::setToS() {
#if defined(Q_OS_WIN)
	if (dwFlow || ! hQoS)
		return;

	dwFlow = 0;
	if (! QOSAddSocketToFlow(hQoS, qtsSocket->socketDescriptor(), NULL, QOSTrafficTypeAudioVideo, QOS_NON_ADAPTIVE_FLOW, &dwFlow))
		qWarning("Connection: Failed to add flow to QOS");
#elif defined(Q_OS_UNIX)
	int val = 0xa0;
	if (setsockopt(qtsSocket->socketDescriptor(), IPPROTO_IP, IP_TOS, &val, sizeof(val))) {
		val = 0x60;
		if (setsockopt(qtsSocket->socketDescriptor(), IPPROTO_IP, IP_TOS, &val, sizeof(val)))
			qWarning("Connection: Failed to set TOS for TCP Socket");
	}
#if defined(SO_PRIORITY)
	socklen_t optlen = sizeof(val);
	if (getsockopt(qtsSocket->socketDescriptor(), SOL_SOCKET, SO_PRIORITY, &val, &optlen) == 0) {
		if (val == 0) {
			val = 6;
			setsockopt(qtsSocket->socketDescriptor(), SOL_SOCKET, SO_PRIORITY, &val, sizeof(val));
		}
	}
#endif

#endif
}

int Connection::activityTime() const {
	return qtLastPacket.elapsed();
}

void Connection::resetActivityTime() {
	qtLastPacket.restart();
}

void Connection::socketRead() {
	while (true) {
		qint64 iAvailable = qtsSocket->bytesAvailable();
		if (iPacketLength == -1) {
			if (iAvailable < 6)
				return;

			unsigned char a_ucBuffer[6];

			qtsSocket->read(reinterpret_cast<char *>(a_ucBuffer), 6);
			uiType = qFromBigEndian<quint16>(&a_ucBuffer[0]);
			iPacketLength = qFromBigEndian<quint32>(&a_ucBuffer[2]);
			iAvailable -= 6;
		}

		if ((iPacketLength == -1) || (iAvailable < iPacketLength))
			return;

		QByteArray qbaBuffer = qtsSocket->read(iPacketLength);
		iPacketLength = -1;
		iAvailable -= iPacketLength;

		emit message(uiType, qbaBuffer);
	}
}

void Connection::socketError(QAbstractSocket::SocketError err) {
	emit connectionClosed(err, qtsSocket->errorString());
}

void Connection::socketSslErrors(const QList<QSslError> &qlErr) {
	emit handleSslErrors(qlErr);
}

void Connection::proceedAnyway() {
	qtsSocket->ignoreSslErrors();
}

void Connection::socketDisconnected() {
	emit connectionClosed(QAbstractSocket::UnknownSocketError, QString());
}

void Connection::messageToNetwork(const ::google::protobuf::Message &msg, unsigned int msgType, QByteArray &cache) {
	int len = msg.ByteSize();
	if (len > 0x7fffff)
		return;
	cache.resize(len + 6);
	unsigned char *uc = reinterpret_cast<unsigned char *>(cache.data());
	qToBigEndian<quint16>(msgType, & uc[0]);
	qToBigEndian<quint32>(len, & uc[2]);

	msg.SerializeToArray(uc + 6, len);
}

void Connection::sendMessage(const ::google::protobuf::Message &msg, unsigned int msgType, QByteArray &cache) {
	if (cache.isEmpty()) {
		messageToNetwork(msg, msgType, cache);
	}

	sendMessage(cache);
}

void Connection::sendMessage(const QByteArray &qbaMsg) {
	if (! qbaMsg.isEmpty())
		qtsSocket->write(qbaMsg);
}

void Connection::forceFlush() {
	if (qtsSocket->state() != QAbstractSocket::ConnectedState)
		return;

	if (! qtsSocket->isEncrypted())
		return;

	int nodelay;

	qtsSocket->flush();

	nodelay = 1;
	setsockopt(qtsSocket->socketDescriptor(), IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char *>(&nodelay), sizeof(nodelay));
	nodelay = 0;
	setsockopt(qtsSocket->socketDescriptor(), IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char *>(&nodelay), sizeof(nodelay));
}

void Connection::disconnectSocket(bool force) {
	if (force)
		qtsSocket->abort();
	else
		qtsSocket->disconnectFromHost();
}

QHostAddress Connection::peerAddress() const {
	return qtsSocket->peerAddress();
}

quint16 Connection::peerPort() const {
	return qtsSocket->peerPort();
}

QList<QSslCertificate> Connection::peerCertificateChain() const {
	const QSslCertificate cert = qtsSocket->peerCertificate();
	if (cert.isNull())
		return QList<QSslCertificate>();
	else
		return qtsSocket->peerCertificateChain() << cert;
}

QSslCipher Connection::sessionCipher() const {
	return qtsSocket->sessionCipher();
}

#ifdef Q_OS_WIN
void Connection::setQoS(HANDLE hParentQoS) {
	hQoS = hParentQoS;
}
#endif
