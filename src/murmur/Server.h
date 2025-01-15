// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_SERVER_H_
#define MUMBLE_MURMUR_SERVER_H_

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#include "ACL.h"
#include "AudioReceiverBuffer.h"
#include "Ban.h"
#include "ChannelListenerManager.h"
#include "HostAddress.h"
#include "Mumble.pb.h"
#include "MumbleProtocol.h"
#include "Timer.h"
#include "User.h"
#include "Version.h"
#include "VolumeAdjustment.h"

#ifndef Q_MOC_RUN
#	include <boost/function.hpp>
#endif

#include <QtCore/QEvent>
#include <QtCore/QMutex>
#include <QtCore/QQueue>
#include <QtCore/QReadWriteLock>
#include <QtCore/QRegularExpression>
#include <QtCore/QSocketNotifier>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QTcpServer>
#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
#	include <QtNetwork/QSslDiffieHellmanParameters>
#endif

#ifdef Q_OS_WIN
#	include <winsock2.h>
#endif

class Zeroconf;
class Channel;
class PacketDataStream;
class ServerUser;
class User;
class QNetworkAccessManager;

struct TextMessage {
	QList< unsigned int > qlSessions;
	QList< unsigned int > qlChannels;
	QList< unsigned int > qlTrees;
	QString qsText;
};

class SslServer : public QTcpServer {
private:
	Q_OBJECT
	Q_DISABLE_COPY(SslServer)
protected:
	QList< QSslSocket * > qlSockets;
	void incomingConnection(qintptr) Q_DECL_OVERRIDE;

public:
	QSslSocket *nextPendingSSLConnection();
	SslServer(QObject *parent = nullptr);
};

#define EXEC_QEVENT (QEvent::User + 959)

class ExecEvent : public QEvent {
	Q_DISABLE_COPY(ExecEvent)

protected:
	boost::function< void() > func;

public:
	ExecEvent(boost::function< void() >);
	void execute();
};

class Server : public QThread {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Server)

protected:
	bool bRunning;

	QNetworkAccessManager *qnamNetwork;

#ifdef USE_ZEROCONF
	Zeroconf *zeroconf;
#endif
	void startThread();
	void stopThread();

	void customEvent(QEvent *evt);
	// Former ServerParams
public:
	QList< QHostAddress > qlBind;
	unsigned short usPort;
	int iTimeout;
	int iMaxBandwidth;
	unsigned int iMaxUsers;
	unsigned int iMaxUsersPerChannel;
	unsigned int iDefaultChan;
	bool bRememberChan;
	int iRememberChanDuration;
	int iMaxTextMessageLength;
	int iMaxImageMessageLength;
	int iOpusThreshold;
	bool bAllowHTML;
	QString qsPassword;
	QString qsWelcomeText;
	QString qsWelcomeTextFile;
	bool bCertRequired;
	bool bForceExternalAuth;
	unsigned int m_botCount = 0;

	QString qsRegName;
	QString qsRegPassword;
	QString qsRegHost;
	QString qsRegLocation;
	QUrl qurlRegWeb;
	bool bBonjour;
	bool bAllowPing;
	bool allowRecording;
	unsigned int rollingStatsWindow;

	QRegularExpression qrUserName;
	QRegularExpression qrChannelName;

	unsigned int iMessageLimit;
	unsigned int iMessageBurst;

	unsigned int iPluginMessageLimit;
	unsigned int iPluginMessageBurst;

	bool broadcastListenerVolumeAdjustments;

	Version::full_t m_suggestVersion;

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
	QList< QSslCertificate > qlIntermediates;
#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	QSslDiffieHellmanParameters qsdhpDHParams;
#endif

	Timer tUptime;

	bool bValid;

	ChannelListenerManager m_channelListenerManager;


	Mumble::Protocol::UDPDecoder< Mumble::Protocol::Role::Server > m_udpDecoder;
	Mumble::Protocol::UDPDecoder< Mumble::Protocol::Role::Server > m_tcpTunnelDecoder;
	Mumble::Protocol::UDPPingEncoder< Mumble::Protocol::Role::Server > m_udpPingEncoder;
	Mumble::Protocol::UDPAudioEncoder< Mumble::Protocol::Role::Server > m_udpAudioEncoder;
	Mumble::Protocol::UDPAudioEncoder< Mumble::Protocol::Role::Server > m_tcpAudioEncoder;

	gsl::span< const Mumble::Protocol::byte >
		handlePing(const Mumble::Protocol::UDPDecoder< Mumble::Protocol::Role::Server > &decoder,
				   Mumble::Protocol::UDPPingEncoder< Mumble::Protocol::Role::Server > &encoder, bool expectExtended);

	void readParams();

	int iCodecAlpha;
	int iCodecBeta;
	bool bPreferAlpha;
	bool bOpus;
	void recheckCodecVersions(ServerUser *connectingUser = 0);

#ifdef USE_ZEROCONF
	void initZeroconf();
	void removeZeroconf();
#endif
	// Registration, implementation in Register.cpp
	QTimer qtTick;
	void initRegister();

	WhisperTargetCache createWhisperTargetCacheFor(ServerUser &speaker, const WhisperTarget &target);

private:
	int iChannelNestingLimit;
	int iChannelCountLimit;

	AudioReceiverBuffer m_udpAudioReceivers;
	AudioReceiverBuffer m_tcpAudioReceivers;

public slots:
	void regSslError(const QList< QSslError > &);
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
	void sslError(const QList< QSslError > &);
	void message(Mumble::Protocol::TCPMessageType, const QByteArray &, ServerUser *cCon = nullptr);
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
	QQueue< unsigned int > qqIds;
	QList< SslServer * > qlServer;
	QTimer *qtTimeout;

#ifdef Q_OS_UNIX
	int aiNotify[2];
	QList< int > qlUdpSocket;
#else
	HANDLE hNotify;
	QList< SOCKET > qlUdpSocket;
#endif
	QList< QSocketNotifier * > qlUdpNotifier;

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
	QHash< unsigned int, ServerUser * > qhUsers;
	QHash< QPair< HostAddress, quint16 >, ServerUser * > qhPeerUsers;
	QHash< HostAddress, QSet< ServerUser * > > qhHostUsers;
	QHash< unsigned int, Channel * > qhChannels;

	QMutex qmCache;
	ChanACL::ACLCache acCache;

	QHash< int, QString > qhUserNameCache;
	QHash< QString, int > qhUserIDCache;

	QList< Ban > qlBans;

	void addListener(QHash< ServerUser *, VolumeAdjustment > &listeners, ServerUser &user, const Channel &channel);
	void processMsg(ServerUser *u, Mumble::Protocol::AudioData audioData, AudioReceiverBuffer &buffer,
					Mumble::Protocol::UDPAudioEncoder< Mumble::Protocol::Role::Server > &encoder);
	void sendMessage(ServerUser &u, const unsigned char *data, int len, QByteArray &cache, bool force = false);
	void run();

	bool validateChannelName(const QString &name);
	bool validateUserName(const QString &name);

	bool checkDecrypt(ServerUser *u, const unsigned char *encrypted, unsigned char *plain, unsigned int cryptlen);

	bool hasPermission(ServerUser *p, Channel *c, QFlags< ChanACL::Perm > perm);
	QFlags< ChanACL::Perm > effectivePermissions(ServerUser *p, Channel *c);
	void sendClientPermission(ServerUser *u, Channel *c, bool explicitlyRequested = false);
	void flushClientPermissionCache(ServerUser *u, MumbleProto::PermissionQuery &mpqq);
	void clearACLCache(User *p = nullptr);
	void clearWhisperTargetCache();

	void sendProtoAll(const ::google::protobuf::Message &msg, Mumble::Protocol::TCPMessageType type,
					  Version::full_t version, Version::CompareMode mode);
	void sendProtoExcept(ServerUser *, const ::google::protobuf::Message &msg, Mumble::Protocol::TCPMessageType type,
						 Version::full_t version, Version::CompareMode mode);
	void sendProtoMessage(ServerUser *, const ::google::protobuf::Message &msg, Mumble::Protocol::TCPMessageType type);

	// sendAll sends a protobuf message to all users on the server whose version is either bigger than v or
	// lower than ~v. If v == 0 the message is sent to everyone.
#define PROCESS_MUMBLE_TCP_MESSAGE(name, value)                                                        \
	void sendAll(const MumbleProto::name &msg, Version::full_t v = Version::UNKNOWN,                   \
				 Version::CompareMode mode = Version::CompareMode::AtLeast) {                          \
		sendProtoAll(msg, Mumble::Protocol::TCPMessageType::name, v, mode);                            \
	}                                                                                                  \
	void sendExcept(ServerUser *u, const MumbleProto::name &msg, Version::full_t v = Version::UNKNOWN, \
					Version::CompareMode mode = Version::CompareMode::AtLeast) {                       \
		sendProtoExcept(u, msg, Mumble::Protocol::TCPMessageType::name, v, mode);                      \
	}                                                                                                  \
	void sendMessage(ServerUser *u, const MumbleProto::name &msg) {                                    \
		sendProtoMessage(u, msg, Mumble::Protocol::TCPMessageType::name);                              \
	}

	MUMBLE_ALL_TCP_MESSAGES
#undef PROCESS_MUMBLE_TCP_MESSAGE

	static void hashAssign(QString &destination, QByteArray &hash, const QString &str);
	static void hashAssign(QByteArray &destination, QByteArray &hash, const QByteArray &source);
	bool isTextAllowed(QString &str, bool &changed);

	void setLiveConf(const QString &key, const QString &value);

	QString addressToString(const QHostAddress &, unsigned short port);

	void log(const QString &) const;
	void log(ServerUser *u, const QString &) const;

	void removeChannel(unsigned int id);
	void removeChannel(Channel *c, Channel *dest = nullptr);
	void userEnterChannel(User *u, Channel *c, MumbleProto::UserState &mpus);
	bool unregisterUser(int id);

	Server(int snum, QObject *parent = nullptr);
	~Server();

	bool canNest(Channel *newParent, Channel *channel = nullptr) const;

	// RPC functions. Implementation in RPC.cpp
	void connectAuthenticator(QObject *p);
	void disconnectAuthenticator(QObject *p);
	void connectListener(QObject *p);
	void disconnectListener(QObject *p);
	void setTempGroups(int userid, int sessionId, Channel *cChannel, const QStringList &groups);
	void clearTempGroups(User *user, Channel *cChannel = nullptr, bool recurse = true);
	void startListeningToChannel(ServerUser *user, Channel *cChannel);
	void stopListeningToChannel(ServerUser *user, Channel *cChannel);
	void setListenerVolumeAdjustment(ServerUser *user, const Channel *cChannel,
									 const VolumeAdjustment &volumeAdjustment);
	void sendWelcomeMessageTo(ServerUser *user);
signals:
	void registerUserSig(int &, const QMap< int, QString > &);
	void unregisterUserSig(int &, int);
	void getRegisteredUsersSig(const QString &, QMap< int, QString > &);
	void getRegistrationSig(int &, int, QMap< int, QString > &);
	void authenticateSig(int &, QString &, int, const QList< QSslCertificate > &, const QString &, bool,
						 const QString &);
	void setInfoSig(int &, int, const QMap< int, QString > &);
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
	void setUserState(User *p, Channel *parent, bool mute, bool deaf, bool suppressed, bool prioritySpeaker,
					  const QString &name = QString(), const QString &comment = QString());

	bool setChannelState(Channel *c, Channel *parent, const QString &qsName, const QSet< Channel * > &links,
						 const QString &desc = QString(), const int position = 0);

	void sendTextMessage(Channel *cChannel, ServerUser *pUser, bool tree, const QString &text);

	/// Returns true if a channel is full. If a user is provided, false will always
	/// be returned if the user has write permission in the channel.
	bool isChannelFull(Channel *c, ServerUser *u = 0);

	// Database. Implementation in ServerDB.cpp
	void initialize();
	int authenticate(QString &name, const QString &pw, int sessionId = 0, const QStringList &emails = QStringList(),
					 const QString &certhash = QString(), bool bStrongCert = false,
					 const QList< QSslCertificate > & = QList< QSslCertificate >());
	Channel *addChannel(Channel *c, const QString &name, bool temporary = false, int position = 0,
						unsigned int maxUsers = 0);
	void removeChannelDB(const Channel *c);
	void readChannels(Channel *p = nullptr);
	void readLinks();
	void updateChannel(const Channel *c);
	void readChannelPrivs(Channel *c);
	void setLastChannel(const User *u);
	int readLastChannel(int id);

	/// Set last_disconnect of a registered user to the current time
	void setLastDisconnect(const User *u);
	void dumpChannel(const Channel *c);
	int getUserID(const QString &name);
	QString getUserName(int id);
	QByteArray getUserTexture(int id);
	QMap< int, QString > getRegistration(int id);
	int registerUser(const QMap< int, QString > &info);
	bool unregisterUserDB(int id);
	QList< UserInfo > getRegisteredUsersEx();
	QMap< int, QString > getRegisteredUsers(const QString &filter = QString());
	bool setInfo(int id, const QMap< int, QString > &info);
	bool setTexture(int id, const QByteArray &texture);
	bool isUserId(int id);
	void addLink(Channel *c, Channel *l);
	void removeLink(Channel *c, Channel *l);
	void getBans();
	void saveBans();
	QVariant getConf(const QString &key, QVariant def);
	void setConf(const QString &key, const QVariant &value);
	void dblog(const QString &str) const;

	// These functions perform both the necessary changes to ChannelListeners as
	// well as persisting the changed listeners state to the DB. You should use
	// these unless you have a good reason not to
	void loadChannelListenersOf(const ServerUser &user);
	void addChannelListener(const ServerUser &user, const Channel &channel);
	void disableChannelListener(const ServerUser &user, const Channel &channel);
	void deleteChannelListener(const ServerUser &user, const Channel &channel);
	void setChannelListenerVolume(const ServerUser &user, const Channel &channel, float volumeAdjustment);

	// Implementation in Messages.cpp
#define PROCESS_MUMBLE_TCP_MESSAGE(name, value) void msg##name(ServerUser *, MumbleProto::name &);
	MUMBLE_ALL_TCP_MESSAGES
#undef PROCESS_MUMBLE_TCP_MESSAGE
};

#endif
