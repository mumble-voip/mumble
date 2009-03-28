/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#ifdef Q_OS_UNIX
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

int Connection::iReceiveLevel = 0;
QSet<Connection *> Connection::qsReceivers;

#ifdef Q_OS_WIN
HANDLE Connection::hQoS = NULL;
#endif

Connection::Connection(QObject *p, QSslSocket *qtsSock) : QObject(p) {
	qtsSocket = qtsSock;
	qtsSocket->setParent(this);
	iPacketLength = -1;
	bDisconnectedEmitted = false;

	connect(qtsSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
	connect(qtsSocket, SIGNAL(readyRead()), this, SLOT(socketRead()));
	connect(qtsSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
	connect(qtsSocket, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(socketSslErrors(const QList<QSslError> &)));
	qtLastPacket.restart();
#ifdef Q_OS_WIN
	dwFlow = 0;
#endif
}

Connection::~Connection() {
	qsReceivers.remove(this);
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
#endif
}

int Connection::activityTime() const {
	return qtLastPacket.elapsed();
}

void Connection::socketRead() {
	while (true) {
		qint64 iAvailable = qtsSocket->bytesAvailable();
		if (iPacketLength == -1) {
			if (iAvailable < 4)
				return;

			unsigned char a_ucBuffer[4];

			qtsSocket->read(reinterpret_cast<char *>(a_ucBuffer), 4);
			uiType = a_ucBuffer[0];
			iPacketLength = ((a_ucBuffer[1] << 16) & 0xff0000) + ((a_ucBuffer[2] << 8) & 0xff00) + a_ucBuffer[3];
			iAvailable -= 4;
		}

		if ((iPacketLength == -1) || (iAvailable < iPacketLength))
			return;

		QByteArray qbaBuffer = qtsSocket->read(iPacketLength);
		iPacketLength = -1;
		qtLastPacket.restart();
		iAvailable -= iPacketLength;

		if (! qbaBuffer.isEmpty())
			emit message(uiType, qbaBuffer);
	}
}

void Connection::socketError(QAbstractSocket::SocketError) {
	if (! bDisconnectedEmitted) {
		bDisconnectedEmitted = true;
		emit connectionClosed(qtsSocket->errorString());
	}
}

void Connection::socketSslErrors(const QList<QSslError> &qlErr) {
	emit handleSslErrors(qlErr);
}

void Connection::proceedAnyway() {
	qtsSocket->ignoreSslErrors();
}

void Connection::socketDisconnected() {
	if (! bDisconnectedEmitted) {
		bDisconnectedEmitted = true;
		emit connectionClosed(QString());
	}
}

void Connection::messageToNetwork(const ::google::protobuf::Message &msg, unsigned int msgType, QByteArray &cache) {
	int len = msg.ByteSize();
	if (len > 0x7fffff)
		return;
	cache.resize(len + 4);
	unsigned char *uc = reinterpret_cast<unsigned char *>(cache.data());
	uc[0] = msgType;
	uc[1] = (len >> 16) & 0xFF;
	uc[2] = (len >> 8) & 0xFF;
	uc[3] = len & 0xFF;

	msg.SerializeToArray(uc + 4, len);
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

void Connection::updatePing(double &avg, double &var, quint32 &samples, quint64 usec) {
	samples++;
	double x = static_cast<double>(usec) / 1000.0;
	double delta = x - avg;
	avg += delta / static_cast<double>(samples);
	var += delta * (x - avg);
};

QHostAddress Connection::peerAddress() const {
	return qtsSocket->peerAddress();
}

quint16 Connection::peerPort() const {
	return qtsSocket->peerPort();
}

QList<QSslCertificate> Connection::peerCertificateChain() const {
	return qtsSocket->peerCertificateChain();
}

QSslCipher Connection::sessionCipher() const {
	return qtsSocket->sessionCipher();
}

#ifdef Q_OS_WIN
void Connection::setQoS(HANDLE hParentQoS) {
	hQoS = hParentQoS;
}
#endif
