// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CONNECTION_H_
#define MUMBLE_CONNECTION_H_

#include "MumbleProtocol.h"

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include "crypto/CryptState.h"
#include "crypto/CryptStateOCB2.h"

#include <QtCore/QElapsedTimer>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtNetwork/QSslSocket>
#include <memory>

#ifdef Q_OS_WIN
#	include <ws2tcpip.h>
#endif

namespace google {
namespace protobuf {
	class Message;
}
} // namespace google

class Connection : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Connection)
protected:
	QSslSocket *qtsSocket;
	QElapsedTimer qtLastPacket;
	Mumble::Protocol::TCPMessageType m_type;
	int iPacketLength;
#ifdef Q_OS_WIN
	static HANDLE hQoS;
	DWORD dwFlow;
#endif
protected slots:
	void socketRead();
	void socketError(QAbstractSocket::SocketError);
	void socketDisconnected();
	void socketSslErrors(const QList< QSslError > &errors);
public slots:
	void proceedAnyway();
signals:
	void encrypted();
	void connectionClosed(QAbstractSocket::SocketError, const QString &reason);
	void message(Mumble::Protocol::TCPMessageType type, const QByteArray &);
	void handleSslErrors(const QList< QSslError > &);

public:
	Connection(QObject *parent, QSslSocket *qtsSocket);
	~Connection();
	static void messageToNetwork(const ::google::protobuf::Message &msg, Mumble::Protocol::TCPMessageType msgType,
								 QByteArray &cache);
	void sendMessage(const ::google::protobuf::Message &msg, Mumble::Protocol::TCPMessageType msgType,
					 QByteArray &cache);
	void sendMessage(const QByteArray &qbaMsg);
	void disconnectSocket(bool force = false);
	void forceFlush();
	qint64 activityTime() const;
	void resetActivityTime();

#ifdef MURMUR
	/// qmCrypt locks access to csCrypt.
	QMutex qmCrypt;
#endif
	std::unique_ptr< CryptState > csCrypt;
	/// Returns the peer's chain of digital certificates, starting with the peer's immediate certificate
	/// and ending with the CA's certificate.
	QList< QSslCertificate > peerCertificateChain() const;
	QSslCipher sessionCipher() const;
	QSsl::SslProtocol sessionProtocol() const;
	QString sessionProtocolString() const;
	QHostAddress peerAddress() const;
	quint16 peerPort() const;
	/// Look up the local address of this Connection.
	QHostAddress localAddress() const;
	/// Look up the local port of this Connection.
	quint16 localPort() const;
	bool bDisconnectedEmitted;

	void setToS();
#ifdef Q_OS_WIN
	static void setQoS(HANDLE hParentQoS);
#endif
};

#endif
