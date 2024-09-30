// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SERVERHANDLER_H_
#define MUMBLE_MUMBLE_SERVERHANDLER_H_

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
#	include "win.h"
#endif

#ifndef Q_MOC_RUN
#	include <boost/accumulators/accumulators.hpp>
#	include <boost/accumulators/statistics/mean.hpp>
#	include <boost/accumulators/statistics/stats.hpp>
#	include <boost/accumulators/statistics/variance.hpp>
#	include <boost/shared_ptr.hpp>
#endif

#include <QtCore/QEvent>
#include <QtCore/QMutex>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QSslCipher>
#include <QtNetwork/QSslError>

#define SERVERSEND_EVENT 3501

#include "Mumble.pb.h"
#include "MumbleProtocol.h"
#include "ServerAddress.h"
#include "Timer.h"

class Connection;
class Database;
class PacketDataStream;
class QUdpSocket;
class QSslSocket;
class VoiceRecorder;

class ServerHandlerMessageEvent : public QEvent {
public:
	Mumble::Protocol::TCPMessageType type;
	QByteArray qbaMsg;
	bool bFlush;
	ServerHandlerMessageEvent(const QByteArray &msg, Mumble::Protocol::TCPMessageType type, bool flush = false);
};

typedef boost::shared_ptr< Connection > ConnectionPtr;

class ServerHandler : public QThread {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ServerHandler)

	Database *database;

	static QMutex nextConnectionIDMutex;
	static int nextConnectionID;

protected:
	QString qsHostName;
	QString qsUserName;
	QString qsPassword;
	unsigned short usPort;
	unsigned short usResolvedPort;
	bool bUdp;
	bool bStrong;
	int connectionID;
	Mumble::Protocol::UDPPingEncoder< Mumble::Protocol::Role::Client > m_udpPingEncoder;
	Mumble::Protocol::UDPDecoder< Mumble::Protocol::Role::Client > m_udpDecoder;
	Mumble::Protocol::UDPDecoder< Mumble::Protocol::Role::Client > m_tcpTunnelDecoder;

	/// Flag indicating whether the server we are currently connected to has
	/// finished synchronizing already.
	bool serverSynchronized = false;

#ifdef Q_OS_WIN
	HANDLE hQoS;
	DWORD dwFlowUDP;
#endif

	QHostAddress qhaRemote;
	QHostAddress qhaLocal;
	QUdpSocket *qusUdp;
	QMutex qmUdp;

	void handleVoicePacket(const Mumble::Protocol::AudioData &audioData);

public:
	Timer tTimestamp;
	int iInFlightTCPPings;
	QTimer *tConnectionTimeoutTimer;
	QList< QSslError > qlErrors;
	QList< QSslCertificate > qscCert;
	QSslCipher qscCipher;
	ConnectionPtr cConnection;
	QByteArray qbaDigest;
	boost::shared_ptr< VoiceRecorder > recorder;
	QSslSocket *qtsSock;
	QList< ServerAddress > qlAddresses;
	QHash< ServerAddress, QString > qhHostnames;
	ServerAddress saTargetServer;

	Version::full_t m_version;
	QString qsRelease;
	QString qsOS;
	QString qsOSVersion;

	/**
	 * A flag indicating whether this connection makes use of PFS or not. Note that this flag only has meaning, if the
	 * used version of Qt is >= 5.7.
	 */
	bool connectionUsesPerfectForwardSecrecy = false;

	boost::accumulators::accumulator_set<
		double, boost::accumulators::stats< boost::accumulators::tag::mean, boost::accumulators::tag::variance,
											boost::accumulators::tag::count > >
		accTCP, accUDP, accClean;

	ServerHandler();
	~ServerHandler();
	void setConnectionInfo(const QString &host, unsigned short port, const QString &username, const QString &pw);
	void getConnectionInfo(QString &host, unsigned short &port, QString &username, QString &pw) const;
	bool isStrong() const;
	void customEvent(QEvent *evt) Q_DECL_OVERRIDE;
	int getConnectionID() const;

	void setProtocolVersion(Version::full_t version);

	void sendProtoMessage(const ::google::protobuf::Message &msg, Mumble::Protocol::TCPMessageType type);
	void sendMessage(const unsigned char *data, int len, bool force = false);

	/// @returns Whether this handler is currently connected to a server.
	bool isConnected() const;

	/// @returns Whether the server this handler is currently connected to, has finished
	/// 	synchronizing yet.
	bool hasSynchronized() const;

	/// @param synchronized Whether the server has finished synchronization
	void setServerSynchronized(bool synchronized);

#define PROCESS_MUMBLE_TCP_MESSAGE(name, value) \
	void sendMessage(const MumbleProto::name &msg) { sendProtoMessage(msg, Mumble::Protocol::TCPMessageType::name); }
	MUMBLE_ALL_TCP_MESSAGES
#undef PROCESS_MUMBLE_TCP_MESSAGE

	void requestUserStats(unsigned int uiSession, bool statsOnly);
	void joinChannel(unsigned int uiSession, unsigned int channel);
	void joinChannel(unsigned int uiSession, unsigned int channel, const QStringList &temporaryAccessTokens);
	void startListeningToChannel(unsigned int channel);
	void startListeningToChannels(const QList< unsigned int > &channelIDs);
	void stopListeningToChannel(unsigned int channel);
	void stopListeningToChannels(const QList< unsigned int > &channelIDs);
	void createChannel(unsigned int parent_id, const QString &name, const QString &description, unsigned int position,
					   bool temporary, unsigned int maxUsers);
	void requestBanList();
	void requestUserList();
	void requestACL(unsigned int channel);
	void registerUser(unsigned int uiSession);
	void kickBanUser(unsigned int uiSession, const QString &reason, bool ban);
	void sendUserTextMessage(unsigned int uiSession, const QString &message_);
	void sendChannelTextMessage(unsigned int channel, const QString &message_, bool tree);
	void setUserComment(unsigned int uiSession, const QString &comment);
	void setUserTexture(unsigned int uiSession, const QByteArray &qba);
	void setTokens(const QStringList &tokens);
	void removeChannel(unsigned int channel);
	void addChannelLink(unsigned int channel, unsigned int link);
	void removeChannelLink(unsigned int channel, unsigned int link);
	void requestChannelPermissions(unsigned int channel);
	void setSelfMuteDeafState(bool mute, bool deaf);
	void announceRecordingState(bool recording);

	/// Return connection information as a URL
	QUrl getServerURL(bool withPassword = false) const;

	void disconnect();
	void run() Q_DECL_OVERRIDE;
signals:
	void error(QAbstractSocket::SocketError, QString reason);
	// This signal is basically the same as disconnected but it will be emitted
	// *right before* disconnected is emitted. Thus this can be used by slots
	// that need to block the disconnected signal from being emitted (using a
	// direct connection) before they're done.
	void aboutToDisconnect(QAbstractSocket::SocketError, QString reason);
	void disconnected(QAbstractSocket::SocketError, QString reason);
	void connected();
	void pingRequested();
protected slots:
	void message(Mumble::Protocol::TCPMessageType type, const QByteArray &);
	void serverConnectionConnected();
	void serverConnectionTimeoutOnConnect();
	void serverConnectionStateChanged(QAbstractSocket::SocketState);
	void serverConnectionClosed(QAbstractSocket::SocketError, const QString &);
	void setSslErrors(const QList< QSslError > &);
	void udpReady();
	void hostnameResolved();
private slots:
	void sendPingInternal();
public slots:
	void sendPing();
};

typedef boost::shared_ptr< ServerHandler > ServerHandlerPtr;

#endif
