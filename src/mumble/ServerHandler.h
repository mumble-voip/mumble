/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_SERVERHANDLER_H_
#define MUMBLE_MUMBLE_SERVERHANDLER_H_

#ifndef Q_MOC_RUN
# include <boost/accumulators/accumulators.hpp>
# include <boost/accumulators/statistics/stats.hpp>
# include <boost/accumulators/statistics/mean.hpp>
# include <boost/accumulators/statistics/variance.hpp>
# include <boost/shared_ptr.hpp>
#endif

#include <QtCore/QEvent>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslCipher>
#include <QtNetwork/QSslError>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#define SERVERSEND_EVENT 3501

#include "Timer.h"
#include "Message.h"
#include "Mumble.pb.h"

class Connection;
class Message;
class PacketDataStream;
class QUdpSocket;
class VoiceRecorder;

class ServerHandlerMessageEvent : public QEvent {
	public:
		unsigned int uiType;
		QByteArray qbaMsg;
		bool bFlush;
		ServerHandlerMessageEvent(const QByteArray &msg, unsigned int type, bool flush = false);
};

typedef boost::shared_ptr<Connection> ConnectionPtr;

class ServerHandler : public QThread {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(ServerHandler)
	protected:
		QString qsHostName;
		QString qsUserName;
		QString qsPassword;
		unsigned short usPort;
		bool bUdp;
		bool bStrong;

#ifdef Q_OS_WIN
		HANDLE hQoS;
		DWORD dwFlowUDP;
#endif

		QHostAddress qhaRemote;
		QUdpSocket *qusUdp;
		QMutex qmUdp;

		void handleVoicePacket(unsigned int msgFlags, PacketDataStream &pds, MessageHandler::UDPMessageType type);
	public:
		Timer tTimestamp;
		QTimer *tConnectionTimeoutTimer;
		QList<QSslError> qlErrors;
		QList<QSslCertificate> qscCert;
		QSslCipher qscCipher;
		ConnectionPtr cConnection;
		QByteArray qbaDigest;
		boost::shared_ptr<VoiceRecorder> recorder;

		unsigned int uiVersion;
		QString qsRelease;
		QString qsOS;
		QString qsOSVersion;
		QString qsOSDisplayableVersion;

		boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::variance, boost::accumulators::tag::count> > accTCP, accUDP, accClean;

		ServerHandler();
		~ServerHandler();
		void setConnectionInfo(const QString &host, unsigned short port, const QString &username, const QString &pw);
		void getConnectionInfo(QString &host, unsigned short &port, QString &username, QString &pw) const;
		bool isStrong() const;
		void customEvent(QEvent *evt);

		void sendProtoMessage(const ::google::protobuf::Message &msg, unsigned int msgType);
		void sendMessage(const char *data, int len, bool force = false);

#define MUMBLE_MH_MSG(x) void sendMessage(const MumbleProto:: x &msg) { sendProtoMessage(msg, MessageHandler:: x); }
		MUMBLE_MH_ALL
#undef MUMBLE_MH_MSG

		void requestUserStats(unsigned int uiSession, bool statsOnly);
		void joinChannel(unsigned int channel);
		void createChannel(unsigned int parent_, const QString &name, const QString &description, unsigned int position, bool temporary);
		void setTexture(const QByteArray &qba);
		void requestBanList();
		void requestUserList();
		void requestACL(unsigned int channel);
		void registerUser(unsigned int uiSession);
		void kickBanUser(unsigned int uiSession, const QString &reason, bool ban);
		void sendUserTextMessage(unsigned int uiSession, const QString &message_);
		void sendChannelTextMessage(unsigned int channel, const QString &message_, bool tree);
		void setUserComment(unsigned int uiSession, const QString &comment);
		void removeChannel(unsigned int channel);
		void addChannelLink(unsigned int channel, unsigned int link);
		void removeChannelLink(unsigned int channel, unsigned int link);
		void requestChannelPermissions(unsigned int channel);
		void setSelfMuteDeafState(bool mute, bool deaf);
		void announceRecordingState(bool recording);

		void disconnect();
		void run();
	signals:
		void disconnected(QAbstractSocket::SocketError, QString reason);
		void connected();
	protected slots:
		void message(unsigned int, const QByteArray &);
		void serverConnectionConnected();
		void serverConnectionTimeoutOnConnect();
		void serverConnectionStateChanged(QAbstractSocket::SocketState);
		void serverConnectionClosed(QAbstractSocket::SocketError, const QString &);
		void setSslErrors(const QList<QSslError> &);
		void udpReady();
	public slots:
		void sendPing();
};

typedef boost::shared_ptr<ServerHandler> ServerHandlerPtr;

#endif
