// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_SERVER_H_
#define MUMBLE_MURMUR_SERVER_H_

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
# include "win.h"
#endif

#include "ACL.h"
#include "Message.h"
#include "Mumble.pb.h"
#include "User.h"
#include "Timer.h"
#include "HostAddress.h"
#include "Ban.h"

#ifndef Q_MOC_RUN
# include <boost/function.hpp>
#endif

#include <QtCore/QEvent>
#include <QtCore/QMutex>
#include <QtCore/QTimer>
#include <QtCore/QQueue>
#include <QtCore/QReadWriteLock>
#include <QtCore/QStringList>
#include <QtCore/QSocketNotifier>
#include <QtCore/QThread>
#include <QtCore/QUrl>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QTcpServer>
#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
# include <QtNetwork/QSslDiffieHellmanParameters>
#endif

#ifdef Q_OS_WIN
# include <winsock2.h>
#endif

class BonjourServer;
class Channel;
class PacketDataStream;
class ServerUser;
class User;
class QNetworkAccessManager;

struct TextMessage {
	QList<unsigned int> qlSessions;
	QList<unsigned int> qlChannels;
	QList<unsigned int> qlTrees;
	QString qsText;
};

class SslServer : public QTcpServer {
	private:
		Q_OBJECT;
		Q_DISABLE_COPY(SslServer)
	protected:
		QList<QSslSocket *> qlSockets;
#if QT_VERSION >= 0x050000
		void incomingConnection(qintptr) Q_DECL_OVERRIDE;
#else
		void incomingConnection(int) Q_DECL_OVERRIDE;
#endif
	public:
		QSslSocket *nextPendingSSLConnection();
		SslServer(QObject *parent = NULL);

		/// Checks whether the AF_INET6 socket on this system has dual-stack support.
		static bool hasDualStackSupport();
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
		int iOpusThreshold;
		bool bAllowHTML;
		QString qsPassword;
		QString qsWelcomeText;
		bool bCertRequired;
		bool bForceExternalAuth;

		QString qsRegName;
		QString qsRegPassword;
		QString qsRegHost;
		QString qsRegLocation;
		QUrl qurlRegWeb;
		bool bBonjour;
		bool bAllowPing;

		QRegExp qrUserName;
		QRegExp qrChannelName;

		unsigned int iMessageLimit;
		unsigned int iMessageBurst;

		QVariant qvSuggestVersion;
		QVariant qvSuggestPositional;
		QVariant qvSuggestPushToTalk;

		bool bUsingMetaCert;
		QSslCertificate qscCert;
		QSslKey qskKey;

		/// qlIntermediates contains the certificates
		/// from this virtual server's certificate PEM
		// bundle that do not match the virtual server's
		// private key.
		///
		/// Simply put: it contains any certificates
		/// that aren't the main certificate, or "leaf"
		/// certificate.
		QList<QSslCertificate> qlIntermediates;
#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
		QSslDiffieHellmanParameters qsdhpDHParams;
#endif

		Timer tUptime;

		bool bValid;

		void readParams();

		int iCodecAlpha;
		int iCodecBeta;
		bool bPreferAlpha;
		bool bOpus;
		void recheckCodecVersions(ServerUser *connectingUser = 0);

#ifdef USE_BONJOUR
		void initBonjour();
		void removeBonjour();
#endif
		// Registration, implementation in Register.cpp
		QTimer qtTick;
		void initRegister();

	private:
		int iChannelNestingLimit;
		int iChannelCountLimit;

	public slots:
		void regSslError(const QList<QSslError> &);
		void finished();
		void update();

		// Certificate stuff, implemented partially in Cert.cpp
	public:
		static bool isKeyForCert(const QSslKey &key, const QSslCertificate &cert);
		/// Attempt to load a private key in PEM format from |buf|.
		/// If |passphrase| is non-empty, it will be used for decrypting the private key in |buf|.
		/// If a valid RSA, DSA or EC key is found, it is returned.
		/// If no valid private key is found, a null QSslKey is returned.
		static QSslKey privateKeyFromPEM(const QByteArray &buf, const QByteArray &pass = QByteArray());
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

		/// This lock provides synchronization between the
		/// main thread (where control channel messages and
		/// RPC happens), and the Server's voice thread.
		///
		/// These are the only two threads in Murmur that
		/// access a Server's data.
		///
		/// The easiest way to understand the locking strategy
		/// and synchronization between the main thread and the
		/// Server's voice thread is by using the concept of
		/// ownership.
		///
		/// A thread owning an object means that it is the only
		/// thread that is allowed to write to that object. To
		/// make changes to it.
		///
		/// Most data in the Server class is owned by the main
		/// thread. That means that the main thread is the only
		/// thread that writes/updates those structures.
		///
		/// When processing incoming voice data (and re-
		/// broadcasting) that voice data), the Server's voice
		/// thread needs to access various parts of Server's data,
		/// such as qhUsers, qhChannels, User->cChannel, etc.
		/// However, these are owned by the main thread.
		///
		/// To ensure correct synchronization between the two
		/// threads, the contract for using qrwlVoiceThread is
		/// as follows:
		///
		///  - When the Server's voice thread needs to read data
		///    owned by the main thread, it must hold a read lock
		///    on qrwlVoiceThread.
		///
		///  - The Server's voice thread does not write to any data
		///    that is owned by the main thread.
		///
		///  - When the main thread needs to write to data owned by
		///    itself that is accessed by the voice thread, it must
		///    hold a write lock on qrwlVoiceThread.
		///
		///  - When the main thread needs to read data that is owned
		///    by itself, it DOES NOT hold a lock on qrwlVoiceThread.
		///    That is because ownership of data guarantees that no
		///    other thread can write to that data.
		QReadWriteLock qrwlVoiceThread;
		QHash<unsigned int, ServerUser *> qhUsers;
		QHash<QPair<HostAddress, quint16>, ServerUser *> qhPeerUsers;
		QHash<HostAddress, QSet<ServerUser *> > qhHostUsers;
		QHash<unsigned int, Channel *> qhChannels;

		QMutex qmCache;
		ChanACL::ACLCache acCache;

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
		QFlags<ChanACL::Perm> effectivePermissions(ServerUser *p, Channel *c);
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

		void log(const QString &) const;
		void log(ServerUser *u, const QString &) const;

		void removeChannel(int id);
		void removeChannel(Channel *c, Channel *dest = NULL);
		void userEnterChannel(User *u, Channel *c, MumbleProto::UserState &mpus);
		bool unregisterUser(int id);

		Server(int snum, QObject *parent = NULL);
		~Server();

		bool canNest(Channel *newParent, Channel *channel = NULL) const;

		// RPC functions. Implementation in RPC.cpp
		void connectAuthenticator(QObject *p);
		void disconnectAuthenticator(QObject *p);
		void connectListener(QObject *p);
		void disconnectListener(QObject *p);
		void setTempGroups(int userid, int sessionId, Channel *cChannel, const QStringList &groups);
		void clearTempGroups(User *user, Channel *cChannel = NULL, bool recurse = true);
	signals:
		void registerUserSig(int &, const QMap<int, QString> &);
		void unregisterUserSig(int &, int);
		void getRegisteredUsersSig(const QString &, QMap<int, QString > &);
		void getRegistrationSig(int &, int, QMap<int, QString> &);
		void authenticateSig(int &, QString &, int, const QList<QSslCertificate> &, const QString &, bool, const QString &);
		void setInfoSig(int &, int, const QMap<int, QString> &);
		void setTextureSig(int &, int, const QByteArray &);
		void idToNameSig(QString &, int);
		void nameToIdSig(int &, const QString &);
		void idToTextureSig(QByteArray &, int);

		void userStateChanged(const User *);
		void userTextMessage(const User *, const TextMessage &);
		void userConnected(const User *);
		void userDisconnected(const User *);
		void channelStateChanged(const Channel *);
		void channelCreated(const Channel *);
		void channelRemoved(const Channel *);

		void textMessageFilterSig(int &, const User *, MumbleProto::TextMessage &);

		void contextAction(const User *, const QString &, unsigned int, int);
	public:
		void setUserState(User *p, Channel *parent, bool mute, bool deaf, bool suppressed, bool prioritySpeaker, const QString& name = QString(), const QString &comment = QString());

		/// Update a channel's state using the ChannelState protobuf message and
		/// broadcast the changes appropriately to the server. On return, if
		/// err is non-empty, the operation failed, and err contains a description
		/// of the error.
		///
		/// This method is equivalent to the logic that happens in
		/// Server::msgChannelState  when a ChannelState message is recieved from
		/// a user. However, this method doesn't do permissions checking.
		///
		/// This method is used by the gRPC implementation to perform channel
		/// state changes.
		bool setChannelStateGRPC(const MumbleProto::ChannelState &cs, QString &err);

		bool setChannelState(Channel *c, Channel *parent, const QString &qsName, const QSet<Channel *> &links, const QString &desc = QString(), const int position = 0);

		/// Send a text message using the TextMessage protobuf message
		/// as a template.
		/// This is equivalent to the logic that happens in Server::msgTextMessage
		/// when sending a receieved TextMessage, with the exception that this
		/// method does not perform any permission checks.
		/// It is used by our gRPC implementation to send text messages.
		void sendTextMessageGRPC(const ::MumbleProto::TextMessage &tm);

		void sendTextMessage(Channel *cChannel, ServerUser *pUser, bool tree, const QString &text);

		/// Returns true if a channel is full. If a user is provided, false will always
		/// be returned if the user has write permission in the channel.
		bool isChannelFull(Channel *c, ServerUser *u = 0);

		// Database / DBus functions. Implementation in ServerDB.cpp
		void initialize();
		int authenticate(QString &name, const QString &pw, int sessionId = 0, const QStringList &emails = QStringList(), const QString &certhash = QString(), bool bStrongCert = false, const QList<QSslCertificate> & = QList<QSslCertificate>());
		Channel *addChannel(Channel *c, const QString &name, bool temporary = false, int position = 0, unsigned int maxUsers = 0);
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
		QList<UserInfo> getRegisteredUsersEx();
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
		void dblog(const QString &str) const;

		// From msgHandler. Implementation in Messages.cpp
#define MUMBLE_MH_MSG(x) void msg##x(ServerUser *, MumbleProto:: x &);
		MUMBLE_MH_ALL
#undef MUMBLE_MH_MSG
};

#endif
