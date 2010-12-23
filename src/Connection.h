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

#ifndef _CONNECTION_H
#define _CONNECTION_H

#include "murmur_pch.h"
#include "CryptState.h"
#include "Mumble.pb.h"

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
		int activityTime() const;
		void resetActivityTime();

		CryptState csCrypt;

		QList<QSslCertificate> peerCertificateChain() const;
		QSslCipher sessionCipher() const;
		QHostAddress peerAddress() const;
		quint16 peerPort() const;
		bool bDisconnectedEmitted;

		void setToS();
#ifdef Q_OS_WIN
		static void setQoS(HANDLE hParentQoS);
#endif
};

Q_DECLARE_METATYPE(QAbstractSocket::SocketError)

#else
class Connection;
#endif
