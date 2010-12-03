/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

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

#ifndef _SERVER_H
#define _SERVER_H

#include "murmur_pch.h"
#include "Message.h"
#include "Timer.h"
#include "User.h"
#include "Connection.h"
#include "Net.h"
#include "ACL.h"
#include "DBus.h"

#ifdef USE_BONJOUR
#include "BonjourServer.h"
#endif

class Channel;
class PacketDataStream;
class ServerUser;

class LogEmitter : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(LogEmitter)
	signals:
		void newLogEntry(const QString &msg);
	public:
		LogEmitter(QObject *parent = NULL);
		void addLogEntry(const QString &msg);
};

class SslServer : public QTcpServer {
	private:
		Q_OBJECT;
		Q_DISABLE_COPY(SslServer)
	protected:
		QList<QSslSocket *> qlSockets;
		void incomingConnection(int);
	public:
		QSslSocket *nextPendingSSLConnection();
		SslServer(QObject *parent = NULL);
};

#define EXEC_QEVENT (QEvent::User + 959)

class ExecEvent : public QEvent {
		Q_DISABLE_COPY(ExecEvent);
	protected:
		boost::function<void ()> func;
	public:
		ExecEvent(boost::function<void ()>);
		void execute();
};

class Server : public QThread {
	private:
		Q_OBJECT;
		Q_DISABLE_COPY(Server);
	protected:
		bool bRunning;

		QNetworkAccessManager *qnamNetwork;

#ifdef USE_BONJOUR
		BonjourServer *bsRegistration;
#endif
		void startThread();
		void stopThread();

		void customEvent(QEvent *evt);
		// Former ServerParams
	public:
		QList<QHostAddress> qlBind;
		unsigned short usPort;
		int iTimeout;
		int iMaxBandwidth;
		int iMaxUsers;
		int iMaxUsersPerChannel;
		int iDefaultChan;
		bool bRememberChan;
		int iMaxTextMessageLength;
		int iMaxImageMessageLength;
		bool bAllowHTML;
		QString qsPassword;
		QString qsWelcomeText;
		bool bCertRequired;

		QString qsRegName;
		QString qsRegPassword;
		QString qsRegHost;
		QString qsRegLocation;
		QUrl qurlRegWeb;
		bool bBonjour;
		bool bAllowPing;

		QRegExp qrUserName;
		QRegExp qrChannelName;

		QList<QSslCertificate> qlCA;
		QSslCertificate qscCert;
		QSslKey qskKey;

		Timer tUptime;

		bool bValid;

		void readParams();

		int iCodecAlpha;
		int iCodecBeta;
		bool bPreferAlpha;
		void recheckCodecVersions();

#ifdef USE_BONJOUR
		void initBonjour();
		void removeBonjour();
#endif
		// Registration, implementation in Register.cpp
		QTimer qtTick;
		void initRegister();
	public slots:
		void regSslError(const QList<QSslError> &);
		void finished();
		void update();

		// Certificate stuff, implemented partially in Cert.cpp
	public:
		static bool isKeyForCert(const QSslKey &key, const QSslCertificate &cert);
		void initializeCert();
		const QString getDigest() const;

	public slots:
		void newClient();
		void connectionClosed(QAbstractSocket::SocketError, const QString &);
		void sslError(const QList<QSslError> &);
		void message(unsigned int, const QByteArray &, ServerUser *cCon = NULL);
		void checkTimeout();
		void tcpTransmitData(QByteArray, unsigned int);
		void doSync(unsigned int);
		void encrypted();
		void udpActivated(int);
	signals:
		void reqSync(unsigned int);
		void tcpTransmit(QByteArray, unsigned int id);
	public:
		int iServerNum;
		QQueue<int> qqIds;
		QList<SslServer *> qlServer;
		QTimer *qtTimeout;

#ifdef Q_OS_UNIX
		int aiNotify[2];
		QList<int> qlUdpSocket;
#else
		HANDLE hNotify;
		QList<SOCKET> qlUdpSocket;
#endif
		quint32 uiVersionBlob;
		QList<QSocketNotifier *> qlUdpNotifier;

		QHash<unsigned int, ServerUser *> qhUsers;
		QHash<QPair<HostAddress, quint16>, ServerUser *> qhPeerUsers;
		QHash<HostAddress, QSet<ServerUser *> > qhHostUsers;
		QHash<unsigned int, Channel *> qhChannels;
		QReadWriteLock qrwlUsers;
		ChanACL::ACLCache acCache;
		QMutex qmCache;
		QHash<int, QString> qhUserNameCache;
		QHash<QString, int> qhUserIDCache;

		QList<Ban> qlBans;

		void processMsg(ServerUser *u, const char *data, int len);
		void sendMessage(ServerUser *u, const char *data, int len, QByteArray &cache, bool force = false);
		void run();

		bool validateChannelName(const QString &name);
		bool validateUserName(const QString &name);

		bool checkDecrypt(ServerUser *u, const char *encrypted, char *plain, unsigned int cryptlen);

		bool hasPermission(ServerUser *p, Channel *c, QFlags<ChanACL::Perm> perm);
		void sendClientPermission(ServerUser *u, Channel *c, bool updatelast = false);
		void flushClientPermissionCache(ServerUser *u, MumbleProto::PermissionQuery &mpqq);
		void clearACLCache(User *p = NULL);

		void sendProtoAll(const ::google::protobuf::Message &msg, unsigned int msgType, unsigned int minversion);
		void sendProtoExcept(ServerUser *, const ::google::protobuf::Message &msg, unsigned int msgType, unsigned int minversion);
		void sendProtoMessage(ServerUser *, const ::google::protobuf::Message &msg, unsigned int msgType);

		// sendAll sends a protobuf message to all users on the server whose version is either bigger than v or
		// lower than ~v. If v == 0 the message is sent to everyone.
#define MUMBLE_MH_MSG(x) \
		void sendAll(const MumbleProto:: x &msg, unsigned int v = 0) { sendProtoAll(msg, MessageHandler:: x, v); } \
		void sendExcept(ServerUser *u, const MumbleProto:: x &msg, unsigned int v = 0) { sendProtoExcept(u, msg, MessageHandler:: x, v); } \
		void sendMessage(ServerUser *u, const MumbleProto:: x &msg) { sendProtoMessage(u, msg, MessageHandler:: x); }

		MUMBLE_MH_ALL
#undef MUMBLE_MH_MSG

		static void hashAssign(QString &destination, QByteArray &hash, const QString &str);
		static void hashAssign(QByteArray &destination, QByteArray &hash, const QByteArray &source);
		bool isTextAllowed(QString &str, bool &changed);

		void setLiveConf(const QString &key, const QString &value);

		QString addressToString(const QHostAddress &, unsigned short port);

		void log(const QString &);
		void log(ServerUser *u, const QString &);

		void removeChannel(int id);
		void removeChannel(Channel *c, Channel *dest = NULL);
		void userEnterChannel(User *u, Channel *c, MumbleProto::UserState &mpus);
		bool unregisterUser(int id);

		Server(int snum, QObject *parent = NULL);
		~Server();

		// RPC functions. Implementation in RPC.cpp
		void connectAuthenticator(QObject *p);
		void disconnectAuthenticator(QObject *p);
		void connectListener(QObject *p);
		void disconnectListener(QObject *p);
		void setTempGroups(const int userid, Channel *cChannel, const QStringList &groups);
		void clearTempGroups(User *user, Channel *cChannel = NULL, bool recurse = true);
	signals:
		void registerUserSig(int &, const QMap<int, QString> &);
		void unregisterUserSig(int &, int);
		void getRegisteredUsersSig(const QString &, QMap<int, QString > &);
		void getRegistrationSig(int &, int, QMap<int, QString> &);
		void authenticateSig(int &, QString &, const QList<QSslCertificate> &, const QString &, bool, const QString &);
		void setInfoSig(int &, int, const QMap<int, QString> &);
		void setTextureSig(int &, int, const QByteArray &);
		void idToNameSig(QString &, int);
		void nameToIdSig(int &, const QString &);
		void idToTextureSig(QByteArray &, int);

		void userStateChanged(const User *);
		void userConnected(const User *);
		void userDisconnected(const User *);
		void channelStateChanged(const Channel *);
		void channelCreated(const Channel *);
		void channelRemoved(const Channel *);

		void contextAction(const User *, const QString &, unsigned int, int);
	public:
		void setUserState(User *p, Channel *parent, bool mute, bool deaf, bool suppressed, bool prioritySpeaker, const QString &comment = QString());
		bool setChannelState(Channel *c, Channel *parent, const QString &qsName, const QSet<Channel *> &links, const QString &desc = QString(), const int position = 0);
		void sendTextMessage(Channel *cChannel, ServerUser *pUser, bool tree, const QString &text);

		// Database / DBus functions. Implementation in ServerDB.cpp
		void initialize();
		int authenticate(QString &name, const QString &pw, const QStringList &emails = QStringList(), const QString &certhash = QString(), bool bStrongCert = false, const QList<QSslCertificate> & = QList<QSslCertificate>());
		Channel *addChannel(Channel *c, const QString &name, bool temporary = false, int position = 0);
		void removeChannelDB(const Channel *c);
		void readChannels(Channel *p = NULL);
		void readLinks();
		void updateChannel(const Channel *c);
		void readChannelPrivs(Channel *c);
		void setLastChannel(const User *u);
		int readLastChannel(int id);
		void dumpChannel(const Channel *c);
		int getUserID(const QString &name);
		QString getUserName(int id);
		QByteArray getUserTexture(int id);
		QMap<int, QString> getRegistration(int id);
		int registerUser(const QMap<int, QString> &info);
		bool unregisterUserDB(int id);
		QMap<int, QString > getRegisteredUsers(const QString &filter = QString());
		bool setInfo(int id, const QMap<int, QString> &info);
		bool setTexture(int id, const QByteArray &texture);
		bool isUserId(int id);
		void addLink(Channel *c, Channel *l);
		void removeLink(Channel *c, Channel *l);
		void getBans();
		void saveBans();
		QVariant getConf(const QString &key, QVariant def);
		void setConf(const QString &key, const QVariant &value);
		void dblog(const QString &str);

		// From msgHandler. Implementation in Messages.cpp
#define MUMBLE_MH_MSG(x) void msg##x(ServerUser *, MumbleProto:: x &);
		MUMBLE_MH_ALL
#undef MUMBLE_MH_MSG
};

#else
class Server;
#endif
