// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CONNECTION_H_
#define MUMBLE_CONNECTION_H_

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
# include "win.h"
#endif

#include "CryptState.h"

#include <QtCore/QMutex>

#if QT_VERSION >= 0x040700
# include <QtCore/QElapsedTimer>
#else
# include <QtCore/QTime>
#endif

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtNetwork/QSslSocket>

#ifdef Q_OS_WIN
# include <ws2tcpip.h>
#endif

namespace google {
namespace protobuf {
class Message;
}
}

class Connection : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(Connection)
	protected:
		QSslSocket *qtsSocket;
#if QT_VERSION >= 0x040700
		QElapsedTimer qtLastPacket;
#else
		QTime qtLastPacket;
#endif
		unsigned int uiType;
		int iPacketLength;
#ifdef Q_OS_WIN
		static HANDLE hQoS;
		DWORD dwFlow;
#endif
	protected slots:
		void socketRead();
		void socketError(QAbstractSocket::SocketError);
		void socketDisconnected();
		void socketSslErrors(const QList<QSslError> &errors);
	public slots:
		void proceedAnyway();
	signals:
		void encrypted();
		void connectionClosed(QAbstractSocket::SocketError, const QString &reason);
		void message(unsigned int type, const QByteArray &);
		void handleSslErrors(const QList<QSslError> &);
	public:
		Connection(QObject *parent, QSslSocket *qtsSocket);
		~Connection();
		static void messageToNetwork(const ::google::protobuf::Message &msg, unsigned int msgType, QByteArray &cache);
		void sendMessage(const ::google::protobuf::Message &msg, unsigned int msgType, QByteArray &cache);
		void sendMessage(const QByteArray &qbaMsg);
		void disconnectSocket(bool force=false);
		void forceFlush();
		qint64 activityTime() const;
		void resetActivityTime();

#ifdef MURMUR
		/// qmCrypt locks access to csCrypt.
		QMutex qmCrypt;
#endif
		CryptState csCrypt;

		QList<QSslCertificate> peerCertificateChain() const;
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

#if QT_VERSION < 0x050000
Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
#endif

#endif
