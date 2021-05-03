// Copyright 2007-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Server.h"

#include "ACL.h"
#include "Channel.h"
#include "Connection.h"
#include "EnvUtils.h"
#include "Group.h"
#include "HTMLFilter.h"
#include "HostAddress.h"
#include "Message.h"
#include "Meta.h"
#include "PacketDataStream.h"
#include "ServerDB.h"
#include "ServerUser.h"
#include "SpeechFlags.h"
#include "User.h"
#include "Version.h"

#ifdef USE_ZEROCONF
#	include "Zeroconf.h"
#endif

#include "Utils.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QSet>
#include <QtCore/QXmlStreamAttributes>
#include <QtCore/QtEndian>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QSslConfiguration>

#include <boost/bind/bind.hpp>

#ifdef Q_OS_WIN
#	include <qos2.h>
#	include <ws2tcpip.h>
#else
#	include <netinet/in.h>
#	include <poll.h>
#endif

#ifndef MAX
#	define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#define UDP_PACKET_SIZE 1024

ExecEvent::ExecEvent(boost::function< void() > f) : QEvent(static_cast< QEvent::Type >(EXEC_QEVENT)) {
	func = f;
}

void ExecEvent::execute() {
	func();
}

SslServer::SslServer(QObject *p) : QTcpServer(p) {
}

bool SslServer::hasDualStackSupport() {
	// Create a AF_INET6 socket and try to switch off IPV6_V6ONLY. This
	// should only fail if the system does not support dual-stack mode
	// for this socket type.

	bool result = false;
#ifdef Q_OS_UNIX
	int s = ::socket(AF_INET6, SOCK_STREAM, 0);
	if (s != -1) {
		const int ipv6only = 0;
		if (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast< const char * >(&ipv6only), sizeof(ipv6only))
			== 0) {
			result = true;
		}
		::close(s);
	}
#else
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0) {
		// Seems like we won't be doing any network stuff anyways
		return false;
	}

	SOCKET s = ::WSASocket(AF_INET6, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	if (s != INVALID_SOCKET) {
		const int ipv6only = 0;
		if (setsockopt(s, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast< const char * >(&ipv6only), sizeof(ipv6only))
			== 0) {
			result = true;
		}
		closesocket(s);
	}
	WSACleanup();
#endif
	return result;
}

void SslServer::incomingConnection(qintptr v) {
	QSslSocket *s = new QSslSocket(this);
	s->setSocketDescriptor(v);
	qlSockets.append(s);
}

QSslSocket *SslServer::nextPendingSSLConnection() {
	if (qlSockets.isEmpty())
		return nullptr;
	return qlSockets.takeFirst();
}

Server::Server(int snum, QObject *p) : QThread(p) {
	bValid     = true;
	iServerNum = snum;
#ifdef USE_ZEROCONF
	zeroconf = nullptr;
#endif
	bUsingMetaCert = false;

#ifdef Q_OS_UNIX
	aiNotify[0] = aiNotify[1] = -1;
#else
	hNotify = nullptr;
#endif
	qtTimeout = new QTimer(this);

	iCodecAlpha = iCodecBeta = 0;
	bPreferAlpha             = false;
	bOpus                    = true;

	qnamNetwork = nullptr;

	readParams();
	initialize();

	foreach (const QHostAddress &qha, qlBind) {
		SslServer *ss = new SslServer(this);

		connect(ss, SIGNAL(newConnection()), this, SLOT(newClient()), Qt::QueuedConnection);

		if (!ss->listen(qha, usPort)) {
			log(QString("Server: TCP Listen on %1 failed: %2").arg(addressToString(qha, usPort), ss->errorString()));
			bValid = false;
		} else {
			log(QString("Server listening on %1").arg(addressToString(qha, usPort)));
		}
		qlServer << ss;
	}

	if (!bValid)
		return;

	foreach (SslServer *ss, qlServer) {
		sockaddr_storage addr;
#ifdef Q_OS_UNIX
		int tcpsock   = static_cast< int >(ss->socketDescriptor());
		socklen_t len = sizeof(addr);
#else
		SOCKET tcpsock        = ss->socketDescriptor();
		int len               = sizeof(addr);
#endif
		memset(&addr, 0, sizeof(addr));
		getsockname(tcpsock, reinterpret_cast< struct sockaddr * >(&addr), &len);
#ifdef Q_OS_UNIX
		int sock = ::socket(addr.ss_family, SOCK_DGRAM, 0);
#	ifdef Q_OS_LINUX
		int sockopt = 1;
		if (setsockopt(sock, IPPROTO_IP, IP_PKTINFO, &sockopt, sizeof(sockopt)))
			log(QString("Failed to set IP_PKTINFO for %1").arg(addressToString(ss->serverAddress(), usPort)));
		sockopt = 1;
		if (setsockopt(sock, IPPROTO_IPV6, IPV6_RECVPKTINFO, &sockopt, sizeof(sockopt)))
			log(QString("Failed to set IPV6_RECVPKTINFO for %1").arg(addressToString(ss->serverAddress(), usPort)));
#	endif
#else
#	ifndef SIO_UDP_CONNRESET
#		define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR, 12)
#	endif
		SOCKET sock           = ::WSASocket(addr.ss_family, SOCK_DGRAM, IPPROTO_UDP, nullptr, 0, WSA_FLAG_OVERLAPPED);
		DWORD dwBytesReturned = 0;
		BOOL bNewBehaviour    = FALSE;
		if (WSAIoctl(sock, SIO_UDP_CONNRESET, &bNewBehaviour, sizeof(bNewBehaviour), nullptr, 0, &dwBytesReturned,
					 nullptr, nullptr)
			== SOCKET_ERROR) {
			log(QString("Failed to set SIO_UDP_CONNRESET: %1").arg(WSAGetLastError()));
		}
#endif
		if (sock == INVALID_SOCKET) {
			log("Failed to create UDP Socket");
			bValid = false;
			return;
		} else {
			if (addr.ss_family == AF_INET6) {
				// Copy IPV6_V6ONLY attribute from tcp socket, it defaults to nonzero on Windows
				// See https://msdn.microsoft.com/en-us/library/windows/desktop/ms738574%28v=vs.85%29.aspx
				// This will fail for WindowsXP which is ok. Our TCP code will have split that up
				// into two sockets.
				int ipv6only     = 0;
				socklen_t optlen = sizeof(ipv6only);
				if (::getsockopt(tcpsock, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast< char * >(&ipv6only), &optlen)
					== 0) {
					if (::setsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast< const char * >(&ipv6only),
									 optlen)
						== SOCKET_ERROR) {
						log(QString("Failed to copy IPV6_V6ONLY socket attribute from tcp to udp socket"));
					}
				}
			}

			if (::bind(sock, reinterpret_cast< sockaddr * >(&addr), len) == SOCKET_ERROR) {
				log(QString("Failed to bind UDP Socket to %1").arg(addressToString(ss->serverAddress(), usPort)));
			} else {
#ifdef Q_OS_UNIX
				int val = 0xe0;
				if (setsockopt(sock, IPPROTO_IP, IP_TOS, &val, sizeof(val))) {
					val = 0x80;
					if (setsockopt(sock, IPPROTO_IP, IP_TOS, &val, sizeof(val)))
						log("Server: Failed to set TOS for UDP Socket");
				}
#	if defined(SO_PRIORITY)
				socklen_t optlen = sizeof(val);
				if (getsockopt(sock, SOL_SOCKET, SO_PRIORITY, &val, &optlen) == 0) {
					if (val == 0) {
						val = 6;
						setsockopt(sock, SOL_SOCKET, SO_PRIORITY, &val, sizeof(val));
					}
				}
#	endif
#endif
			}
			QSocketNotifier *qsn = new QSocketNotifier(sock, QSocketNotifier::Read, this);
			connect(qsn, SIGNAL(activated(int)), this, SLOT(udpActivated(int)));
			qlUdpSocket << sock;
			qlUdpNotifier << qsn;
		}
	}

	bValid = bValid && (qlServer.count() == qlBind.count()) && (qlUdpSocket.count() == qlBind.count());
	if (!bValid)
		return;

#ifdef Q_OS_UNIX
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, aiNotify) != 0) {
		log("Failed to create notify socket");
		bValid = false;
		return;
	}
#else
	hNotify = CreateEvent(nullptr, FALSE, FALSE, nullptr);
#endif

	connect(this, SIGNAL(tcpTransmit(QByteArray, unsigned int)), this, SLOT(tcpTransmitData(QByteArray, unsigned int)),
			Qt::QueuedConnection);
	connect(this, SIGNAL(reqSync(unsigned int)), this, SLOT(doSync(unsigned int)));

	for (int i = 1; i < iMaxUsers * 2; ++i)
		qqIds.enqueue(i);

	connect(qtTimeout, SIGNAL(timeout()), this, SLOT(checkTimeout()));

	getBans();
	readChannels();
	readLinks();
	initializeCert();

	int major, minor, patch;
	QString release;
	Meta::getVersion(major, minor, patch, release);

	uiVersionBlob = qToBigEndian(static_cast< quint32 >((major << 16) | (minor << 8) | patch));

	if (bValid) {
#ifdef USE_ZEROCONF
		if (bBonjour)
			initZeroconf();
#endif
		initRegister();
	}
}

void Server::startThread() {
	if (!isRunning()) {
		log("Starting voice thread");
		bRunning = true;

		foreach (QSocketNotifier *qsn, qlUdpNotifier)
			qsn->setEnabled(false);
		start(QThread::HighestPriority);
#ifdef Q_OS_LINUX
		// QThread::HighestPriority == Same as everything else...
		int policy;
		struct sched_param param;
		if (pthread_getschedparam(pthread_self(), &policy, &param) == 0) {
			if (policy == SCHED_OTHER) {
				policy               = SCHED_FIFO;
				param.sched_priority = 1;
				pthread_setschedparam(pthread_self(), policy, &param);
			}
		}
#endif
	}
	if (!qtTimeout->isActive())
		qtTimeout->start(15500);
}

void Server::stopThread() {
	bRunning = false;
	if (isRunning()) {
		log("Ending voice thread");

#ifdef Q_OS_UNIX
		unsigned char val = 0;
		if (::write(aiNotify[1], &val, 1) != 1)
			log("Failed to signal voice thread");
#else
		SetEvent(hNotify);
#endif
		wait();

		foreach (QSocketNotifier *qsn, qlUdpNotifier)
			qsn->setEnabled(true);
	}
	qtTimeout->stop();
}

Server::~Server() {
#ifdef USE_ZEROCONF
	removeZeroconf();
#endif

	stopThread();

	foreach (QSocketNotifier *qsn, qlUdpNotifier)
		delete qsn;

#ifdef Q_OS_UNIX
	foreach (int s, qlUdpSocket)
		close(s);

	if (aiNotify[0] >= 0)
		close(aiNotify[0]);
	if (aiNotify[1] >= 0)
		close(aiNotify[1]);
#else
	foreach (SOCKET s, qlUdpSocket)
		closesocket(s);
	if (hNotify)
		CloseHandle(hNotify);
#endif
	clearACLCache();

	log("Stopped");
}

/// normalizeSuggestVersion normalizes a 'suggestversion' config value.
/// The config value may be a version string, or a bitmasked
/// integer representing the version.
/// This function converts the 'suggestversion' config value to
/// always be a bitmasked integer representation.
///
/// On error, the function returns an empty QVariant.
static QVariant normalizeSuggestVersion(QVariant suggestVersion) {
	uint integerValue = suggestVersion.toUInt();

	// If the integer value is 0, it can mean two things:
	//
	// Either the suggestversion is set to 0.
	// Or, the suggestversion is a version string such as "1.3.0",
	// and cannot be converted to an integer value.
	//
	// We handle both cases the same: by pretending the
	// suggestversion is a version string in both cases.
	//
	// If it is a version string, the call to Version::getRaw()
	// will return the bitmasked representation.
	//
	// If it is not a version string, the call to Version::getRaw()
	// will return 0, so it is effectively a no-op.
	if (integerValue == 0) {
		integerValue = Version::getRaw(suggestVersion.toString());
	}

	if (integerValue != 0) {
		return integerValue;
	}

	return QVariant();
}

void Server::readParams() {
	qsPassword             = Meta::mp.qsPassword;
	usPort                 = static_cast< unsigned short >(Meta::mp.usPort + iServerNum - 1);
	iTimeout               = Meta::mp.iTimeout;
	iMaxBandwidth          = Meta::mp.iMaxBandwidth;
	iMaxUsers              = Meta::mp.iMaxUsers;
	iMaxUsersPerChannel    = Meta::mp.iMaxUsersPerChannel;
	iMaxTextMessageLength  = Meta::mp.iMaxTextMessageLength;
	iMaxImageMessageLength = Meta::mp.iMaxImageMessageLength;
	bAllowHTML             = Meta::mp.bAllowHTML;
	iDefaultChan           = Meta::mp.iDefaultChan;
	bRememberChan          = Meta::mp.bRememberChan;
	iRememberChanDuration  = Meta::mp.iRememberChanDuration;
	qsWelcomeText          = Meta::mp.qsWelcomeText;
	qsWelcomeTextFile      = Meta::mp.qsWelcomeTextFile;
	qlBind                 = Meta::mp.qlBind;
	qsRegName              = Meta::mp.qsRegName;
	qsRegPassword          = Meta::mp.qsRegPassword;
	qsRegHost              = Meta::mp.qsRegHost;
	qsRegLocation          = Meta::mp.qsRegLocation;
	qurlRegWeb             = Meta::mp.qurlRegWeb;
	bBonjour               = Meta::mp.bBonjour;
	bAllowPing             = Meta::mp.bAllowPing;
	bCertRequired          = Meta::mp.bCertRequired;
	bForceExternalAuth     = Meta::mp.bForceExternalAuth;
	qrUserName             = Meta::mp.qrUserName;
	qrChannelName          = Meta::mp.qrChannelName;
	iMessageLimit          = Meta::mp.iMessageLimit;
	iMessageBurst          = Meta::mp.iMessageBurst;
	iPluginMessageLimit    = Meta::mp.iPluginMessageLimit;
	iPluginMessageBurst    = Meta::mp.iPluginMessageBurst;
	qvSuggestVersion       = Meta::mp.qvSuggestVersion;
	qvSuggestPositional    = Meta::mp.qvSuggestPositional;
	qvSuggestPushToTalk    = Meta::mp.qvSuggestPushToTalk;
	iOpusThreshold         = Meta::mp.iOpusThreshold;
	iChannelNestingLimit   = Meta::mp.iChannelNestingLimit;
	iChannelCountLimit     = Meta::mp.iChannelCountLimit;

	QString qsHost = getConf("host", QString()).toString();
	if (!qsHost.isEmpty()) {
		qlBind.clear();
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
		foreach (const QString &host, qsHost.split(QRegExp(QLatin1String("\\s+")), Qt::SkipEmptyParts)) {
#else
		// Qt 5.14 introduced the Qt::SplitBehavior flags deprecating the QString fields
		foreach (const QString &host, qsHost.split(QRegExp(QLatin1String("\\s+")), QString::SkipEmptyParts)) {
#endif
			QHostAddress qhaddr;
			if (qhaddr.setAddress(qsHost)) {
				qlBind << qhaddr;
			} else {
				bool found   = false;
				QHostInfo hi = QHostInfo::fromName(host);
				foreach (QHostAddress qha, hi.addresses()) {
					if ((qha.protocol() == QAbstractSocket::IPv4Protocol)
						|| (qha.protocol() == QAbstractSocket::IPv6Protocol)) {
						qlBind << qha;
						found = true;
					}
				}
				if (!found) {
					log(QString("Lookup of bind hostname %1 failed").arg(host));
				}
			}
		}
		foreach (const QHostAddress &qha, qlBind)
			log(QString("Binding to address %1").arg(qha.toString()));
		if (qlBind.isEmpty())
			qlBind = Meta::mp.qlBind;
	}

	qsPassword             = getConf("password", qsPassword).toString();
	usPort                 = static_cast< unsigned short >(getConf("port", usPort).toUInt());
	iTimeout               = getConf("timeout", iTimeout).toInt();
	iMaxBandwidth          = getConf("bandwidth", iMaxBandwidth).toInt();
	iMaxUsers              = getConf("users", iMaxUsers).toInt();
	iMaxUsersPerChannel    = getConf("usersperchannel", iMaxUsersPerChannel).toInt();
	iMaxTextMessageLength  = getConf("textmessagelength", iMaxTextMessageLength).toInt();
	iMaxImageMessageLength = getConf("imagemessagelength", iMaxImageMessageLength).toInt();
	bAllowHTML             = getConf("allowhtml", bAllowHTML).toBool();
	iDefaultChan           = getConf("defaultchannel", iDefaultChan).toInt();
	bRememberChan          = getConf("rememberchannel", bRememberChan).toBool();
	iRememberChanDuration  = getConf("rememberchannelduration", iRememberChanDuration).toInt();
	qsWelcomeText          = getConf("welcometext", qsWelcomeText).toString();
	qsWelcomeTextFile      = getConf("welcometextfile", qsWelcomeTextFile).toString();

	if (!qsWelcomeTextFile.isEmpty()) {
		if (qsWelcomeText.isEmpty()) {
			QFile f(qsWelcomeTextFile);
			if (f.open(QFile::ReadOnly | QFile::Text)) {
				QTextStream in(&f);
				qsWelcomeText = in.readAll();
				f.close();
			} else {
				log(QString("Failed to open welcome text file %1").arg(qsWelcomeTextFile));
			}
		} else {
			log(QString("Ignoring welcometextfile %1 because welcometext is defined").arg(qsWelcomeTextFile));
		}
	}

	qsRegName          = getConf("registername", qsRegName).toString();
	qsRegPassword      = getConf("registerpassword", qsRegPassword).toString();
	qsRegHost          = getConf("registerhostname", qsRegHost).toString();
	qsRegLocation      = getConf("registerlocation", qsRegLocation).toString();
	qurlRegWeb         = QUrl(getConf("registerurl", qurlRegWeb.toString()).toString());
	bBonjour           = getConf("bonjour", bBonjour).toBool();
	bAllowPing         = getConf("allowping", bAllowPing).toBool();
	bCertRequired      = getConf("certrequired", bCertRequired).toBool();
	bForceExternalAuth = getConf("forceExternalAuth", bForceExternalAuth).toBool();

	qvSuggestVersion = normalizeSuggestVersion(getConf("suggestversion", qvSuggestVersion));
	if (qvSuggestVersion.toUInt() == 0)
		qvSuggestVersion = QVariant();

	qvSuggestPositional = getConf("suggestpositional", qvSuggestPositional);
	if (qvSuggestPositional.toString().trimmed().isEmpty())
		qvSuggestPositional = QVariant();

	qvSuggestPushToTalk = getConf("suggestpushtotalk", qvSuggestPushToTalk);
	if (qvSuggestPushToTalk.toString().trimmed().isEmpty())
		qvSuggestPushToTalk = QVariant();

	iOpusThreshold = getConf("opusthreshold", iOpusThreshold).toInt();

	iChannelNestingLimit = getConf("channelnestinglimit", iChannelNestingLimit).toInt();
	iChannelCountLimit   = getConf("channelcountlimit", iChannelCountLimit).toInt();

	qrUserName    = QRegExp(getConf("username", qrUserName.pattern()).toString());
	qrChannelName = QRegExp(getConf("channelname", qrChannelName.pattern()).toString());

	iMessageLimit = getConf("messagelimit", iMessageLimit).toUInt();
	if (iMessageLimit < 1) { // Prevent disabling messages entirely
		iMessageLimit = 1;
	}
	iMessageBurst = getConf("messageburst", iMessageBurst).toUInt();
	if (iMessageBurst < 1) { // Prevent disabling messages entirely
		iMessageBurst = 1;
	}

	iPluginMessageLimit = getConf("mpluginessagelimit", iPluginMessageLimit).toUInt();
	if (iPluginMessageLimit < 1) { // Prevent disabling messages entirely
		iPluginMessageLimit = 1;
	}
	iPluginMessageBurst = getConf("pluginmessageburst", iPluginMessageBurst).toUInt();
	if (iPluginMessageBurst < 1) { // Prevent disabling messages entirely
		iPluginMessageBurst = 1;
	}
}

void Server::setLiveConf(const QString &key, const QString &value) {
	QString v = value.trimmed().isEmpty() ? QString() : value;
	int i     = v.toInt();
	if ((key == "password") || (key == "serverpassword"))
		qsPassword = !v.isNull() ? v : Meta::mp.qsPassword;
	else if (key == "timeout")
		iTimeout = i ? i : Meta::mp.iTimeout;
	else if (key == "bandwidth") {
		int length = i ? i : Meta::mp.iMaxBandwidth;
		if (length != iMaxBandwidth) {
			iMaxBandwidth = length;
			MumbleProto::ServerConfig mpsc;
			mpsc.set_max_bandwidth(length);
			sendAll(mpsc);
		}
	} else if (key == "users") {
		int newmax = i ? i : Meta::mp.iMaxUsers;
		if (iMaxUsers == newmax)
			return;

		iMaxUsers = newmax;
		qqIds.clear();
		for (int id = 1; id < iMaxUsers * 2; ++id)
			if (!qhUsers.contains(id))
				qqIds.enqueue(id);

		MumbleProto::ServerConfig mpsc;
		mpsc.set_max_users(iMaxUsers);
		sendAll(mpsc);
	} else if (key == "usersperchannel")
		iMaxUsersPerChannel = i ? i : Meta::mp.iMaxUsersPerChannel;
	else if (key == "textmessagelength") {
		int length = i ? i : Meta::mp.iMaxTextMessageLength;
		if (length != iMaxTextMessageLength) {
			iMaxTextMessageLength = length;
			MumbleProto::ServerConfig mpsc;
			mpsc.set_message_length(length);
			sendAll(mpsc);
		}
	} else if (key == "imagemessagelength") {
		int length = i ? i : Meta::mp.iMaxImageMessageLength;
		if (length != iMaxImageMessageLength) {
			iMaxImageMessageLength = length;
			MumbleProto::ServerConfig mpsc;
			mpsc.set_image_message_length(length);
			sendAll(mpsc);
		}
	} else if (key == "allowhtml") {
		bool allow = !v.isNull() ? QVariant(v).toBool() : Meta::mp.bAllowHTML;
		if (allow != bAllowHTML) {
			bAllowHTML = allow;
			MumbleProto::ServerConfig mpsc;
			mpsc.set_allow_html(bAllowHTML);
			sendAll(mpsc);
		}
	} else if (key == "defaultchannel")
		iDefaultChan = i ? i : Meta::mp.iDefaultChan;
	else if (key == "rememberchannel")
		bRememberChan = !v.isNull() ? QVariant(v).toBool() : Meta::mp.bRememberChan;
	else if (key == "rememberchannelduration") {
		iRememberChanDuration = !v.isNull() ? v.toInt() : Meta::mp.iRememberChanDuration;
		if (iRememberChanDuration < 0) {
			iRememberChanDuration = 0;
		}
	} else if (key == "welcometext") {
		QString text = !v.isNull() ? v : Meta::mp.qsWelcomeText;
		if (text != qsWelcomeText) {
			qsWelcomeText = text;
		}
	} else if (key == "registername") {
		QString text = !v.isNull() ? v : Meta::mp.qsRegName;
		if (text != qsRegName) {
			qsRegName = text;
			if (!qsRegName.isEmpty()) {
				MumbleProto::ChannelState mpcs;
				mpcs.set_channel_id(0);
				mpcs.set_name(u8(qsRegName));
				sendAll(mpcs);
			}
		}
	} else if (key == "registerpassword")
		qsRegPassword = !v.isNull() ? v : Meta::mp.qsRegPassword;
	else if (key == "registerhostname")
		qsRegHost = !v.isNull() ? v : Meta::mp.qsRegHost;
	else if (key == "registerlocation")
		qsRegLocation = !v.isNull() ? v : Meta::mp.qsRegLocation;
	else if (key == "registerurl")
		qurlRegWeb = !v.isNull() ? v : Meta::mp.qurlRegWeb;
	else if (key == "certrequired")
		bCertRequired = !v.isNull() ? QVariant(v).toBool() : Meta::mp.bCertRequired;
	else if (key == "forceExternalAuth")
		bForceExternalAuth = !v.isNull() ? QVariant(v).toBool() : Meta::mp.bForceExternalAuth;
	else if (key == "bonjour") {
		bBonjour = !v.isNull() ? QVariant(v).toBool() : Meta::mp.bBonjour;
#ifdef USE_ZEROCONF
		if (bBonjour && !zeroconf) {
			initZeroconf();
		} else if (!bBonjour && zeroconf) {
			removeZeroconf();
		}
#endif
	} else if (key == "allowping")
		bAllowPing = !v.isNull() ? QVariant(v).toBool() : Meta::mp.bAllowPing;
	else if (key == "username")
		qrUserName = !v.isNull() ? QRegExp(v) : Meta::mp.qrUserName;
	else if (key == "channelname")
		qrChannelName = !v.isNull() ? QRegExp(v) : Meta::mp.qrChannelName;
	else if (key == "suggestversion")
		qvSuggestVersion =
			!v.isNull() ? (v.isEmpty() ? QVariant() : normalizeSuggestVersion(v)) : Meta::mp.qvSuggestVersion;
	else if (key == "suggestpositional")
		qvSuggestPositional = !v.isNull() ? (v.isEmpty() ? QVariant() : v) : Meta::mp.qvSuggestPositional;
	else if (key == "suggestpushtotalk")
		qvSuggestPushToTalk = !v.isNull() ? (v.isEmpty() ? QVariant() : v) : Meta::mp.qvSuggestPushToTalk;
	else if (key == "opusthreshold")
		iOpusThreshold = (i >= 0 && !v.isNull()) ? qBound(0, i, 100) : Meta::mp.iOpusThreshold;
	else if (key == "channelnestinglimit")
		iChannelNestingLimit = (i >= 0 && !v.isNull()) ? i : Meta::mp.iChannelNestingLimit;
	else if (key == "channelcountlimit")
		iChannelCountLimit = (i >= 0 && !v.isNull()) ? i : Meta::mp.iChannelCountLimit;
	else if (key == "messagelimit") {
		iMessageLimit = (!v.isNull()) ? v.toUInt() : Meta::mp.iMessageLimit;
		if (iMessageLimit < 1) {
			iMessageLimit = 1;
		}
	} else if (key == "messageburst") {
		iMessageBurst = (!v.isNull()) ? v.toUInt() : Meta::mp.iMessageBurst;
		if (iMessageBurst < 1) {
			iMessageBurst = 1;
		}
	}
}

#ifdef USE_ZEROCONF
void Server::initZeroconf() {
	zeroconf = new Zeroconf();
	if (zeroconf->isOk()) {
		log("Registering zeroconf service...");
		zeroconf->registerService(BonjourRecord(qsRegName, "_mumble._tcp", ""), usPort);
		return;
	}

	delete zeroconf;
	zeroconf = nullptr;
}

void Server::removeZeroconf() {
	if (!zeroconf) {
		return;
	}

	if (zeroconf->isOk()) {
		log("Unregistering zeroconf service...");
	}

	delete zeroconf;
	zeroconf = nullptr;
}
#endif

void Server::customEvent(QEvent *evt) {
	if (evt->type() == EXEC_QEVENT)
		static_cast< ExecEvent * >(evt)->execute();
}

void Server::udpActivated(int socket) {
	qint32 len;
	char encrypt[UDP_PACKET_SIZE];
	sockaddr_storage from;
#ifdef Q_OS_UNIX
#	ifdef Q_OS_LINUX
	struct msghdr msg;
	struct iovec iov[1];

	iov[0].iov_base = encrypt;
	iov[0].iov_len  = UDP_PACKET_SIZE;

	uint8_t controldata[CMSG_SPACE(MAX(sizeof(struct in6_pktinfo), sizeof(struct in_pktinfo)))];

	memset(&msg, 0, sizeof(msg));
	msg.msg_name       = reinterpret_cast< struct sockaddr * >(&from);
	msg.msg_namelen    = sizeof(from);
	msg.msg_iov        = iov;
	msg.msg_iovlen     = 1;
	msg.msg_control    = controldata;
	msg.msg_controllen = sizeof(controldata);

	int &sock = socket;
	len       = static_cast< quint32 >(::recvmsg(sock, &msg, MSG_TRUNC));
#	else
	socklen_t fromlen = sizeof(from);
	int &sock         = socket;
	len               = static_cast< qint32 >(
        ::recvfrom(sock, encrypt, UDP_PACKET_SIZE, MSG_TRUNC, reinterpret_cast< struct sockaddr * >(&from), &fromlen));
#	endif
#else
	int fromlen = sizeof(from);
	SOCKET sock = static_cast< SOCKET >(socket);
	len         = ::recvfrom(sock, encrypt, UDP_PACKET_SIZE, 0, reinterpret_cast< struct sockaddr * >(&from), &fromlen);
#endif

	// Cloned from ::run(), as it's the only UDP data we care about until the thread is started.
	quint32 *ping = reinterpret_cast< quint32 * >(encrypt);
	if ((len == 12) && (*ping == 0) && bAllowPing) {
		ping[0] = uiVersionBlob;
		ping[3] = qToBigEndian(static_cast< quint32 >(qhUsers.count()));
		ping[4] = qToBigEndian(static_cast< quint32 >(iMaxUsers));
		ping[5] = qToBigEndian(static_cast< quint32 >(iMaxBandwidth));

#ifdef Q_OS_LINUX
		// There will be space for only one header, and the only data we have asked for is the incoming
		// address. So we can reuse most of the same msg and control data.
		iov[0].iov_len = 6 * sizeof(quint32);
		::sendmsg(sock, &msg, 0);
#else
        ::sendto(sock, encrypt, 6 * sizeof(quint32), 0, reinterpret_cast< struct sockaddr * >(&from), fromlen);
#endif
	}
}

void Server::run() {
	qint32 len;
#if defined(__LP64__)
	char encbuff[UDP_PACKET_SIZE + 8];
	char *encrypt = encbuff + 4;
#else
	char encrypt[UDP_PACKET_SIZE];
#endif
	char buffer[UDP_PACKET_SIZE];

	sockaddr_storage from;
	int nfds = qlUdpSocket.count();

#ifdef Q_OS_UNIX
	socklen_t fromlen;
	STACKVAR(struct pollfd, fds, nfds + 1);

	for (int i = 0; i < nfds; ++i) {
		fds[i].fd      = qlUdpSocket.at(i);
		fds[i].events  = POLLIN;
		fds[i].revents = 0;
	}

	fds[nfds].fd      = aiNotify[0];
	fds[nfds].events  = POLLIN;
	fds[nfds].revents = 0;
#else
	int fromlen;
	STACKVAR(SOCKET, fds, nfds);
	STACKVAR(HANDLE, events, nfds + 1);
	for (int i = 0; i < nfds; ++i) {
		fds[i]    = qlUdpSocket.at(i);
		events[i] = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		::WSAEventSelect(fds[i], events[i], FD_READ);
	}
	events[nfds] = hNotify;
#endif

	++nfds;

	while (bRunning) {
#ifdef Q_OS_UNIX
		int pret = poll(fds, nfds, -1);
		if (pret <= 0) {
			if (errno == EINTR)
				continue;
			qCritical("poll failure");
			bRunning = false;
			break;
		}

		if (fds[nfds - 1].revents) {
			// Drain pipe
			unsigned char val;
			while (::recv(aiNotify[0], &val, 1, MSG_DONTWAIT) == 1) {
			};
			break;
		}

		for (int i = 0; i < nfds - 1; ++i) {
			if (fds[i].revents) {
				if (fds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
					qCritical("poll event failure");
					bRunning = false;
					break;
				}

				int sock = fds[i].fd;
#else
		for (int i = 0; i < 1; ++i) {
			{
				DWORD ret = WaitForMultipleObjects(nfds, events, FALSE, INFINITE);
				if (ret == (WAIT_OBJECT_0 + nfds - 1)) {
					break;
				}
				if (ret == WAIT_FAILED) {
					qCritical("UDP wait failed");
					bRunning = false;
					break;
				}
				SOCKET sock = fds[ret - WAIT_OBJECT_0];
#endif

				fromlen = sizeof(from);
#ifdef Q_OS_WIN
				len = ::recvfrom(sock, encrypt, UDP_PACKET_SIZE, 0, reinterpret_cast< struct sockaddr * >(&from),
								 &fromlen);
#else
#	ifdef Q_OS_LINUX
				struct msghdr msg;
				struct iovec iov[1];

				iov[0].iov_base = encrypt;
				iov[0].iov_len  = UDP_PACKET_SIZE;

				uint8_t controldata[CMSG_SPACE(MAX(sizeof(struct in6_pktinfo), sizeof(struct in_pktinfo)))];

				memset(&msg, 0, sizeof(msg));
				msg.msg_name       = reinterpret_cast< struct sockaddr * >(&from);
				msg.msg_namelen    = sizeof(from);
				msg.msg_iov        = iov;
				msg.msg_iovlen     = 1;
				msg.msg_control    = controldata;
				msg.msg_controllen = sizeof(controldata);

				len = static_cast< quint32 >(::recvmsg(sock, &msg, MSG_TRUNC));
				Q_UNUSED(fromlen);
#	else
				len = static_cast< qint32 >(::recvfrom(sock, encrypt, UDP_PACKET_SIZE, MSG_TRUNC,
													   reinterpret_cast< struct sockaddr * >(&from), &fromlen));
#	endif
#endif
				if (len == 0) {
					break;
				} else if (len == SOCKET_ERROR) {
					break;
				} else if (len < 5) {
					// 4 bytes crypt header + type + session
					continue;
				} else if (len > UDP_PACKET_SIZE) {
					continue;
				}

				QReadLocker rl(&qrwlVoiceThread);

				quint32 *ping = reinterpret_cast< quint32 * >(encrypt);

				if ((len == 12) && (*ping == 0) && bAllowPing) {
					ping[0] = uiVersionBlob;
					// 1 and 2 will be the timestamp, which we return unmodified.
					ping[3] = qToBigEndian(static_cast< quint32 >(qhUsers.count()));
					ping[4] = qToBigEndian(static_cast< quint32 >(iMaxUsers));
					ping[5] = qToBigEndian(static_cast< quint32 >(iMaxBandwidth));

#ifdef Q_OS_LINUX
					iov[0].iov_len = 6 * sizeof(quint32);
					::sendmsg(sock, &msg, 0);
#else
					::sendto(sock, encrypt, 6 * sizeof(quint32), 0, reinterpret_cast< struct sockaddr * >(&from),
							 fromlen);
#endif
					continue;
				}


				quint16 port = (from.ss_family == AF_INET6) ? (reinterpret_cast< sockaddr_in6 * >(&from)->sin6_port)
															: (reinterpret_cast< sockaddr_in * >(&from)->sin_port);
				const HostAddress &ha = HostAddress(from);

				const QPair< HostAddress, quint16 > &key = QPair< HostAddress, quint16 >(ha, port);

				ServerUser *u = qhPeerUsers.value(key);
				if (u) {
					if (!checkDecrypt(u, encrypt, buffer, len)) {
						continue;
					}
				} else {
					// Unknown peer
					foreach (ServerUser *usr, qhHostUsers.value(ha)) {
						if (checkDecrypt(usr, encrypt, buffer, len)) { // checkDecrypt takes the User's qrwlCrypt lock.
							// Every time we relock, reverify users' existence.
							// The main thread might delete the user while the lock isn't held.
							unsigned int uiSession = usr->uiSession;
							rl.unlock();
							qrwlVoiceThread.lockForWrite();
							if (qhUsers.contains(uiSession)) {
								u             = usr;
								u->sUdpSocket = sock;
								memcpy(&u->saiUdpAddress, &from, sizeof(from));
								qhHostUsers[from].remove(u);
								qhPeerUsers.insert(key, u);
							}
							qrwlVoiceThread.unlock();
							rl.relock();
							if (u && !qhUsers.contains(uiSession))
								u = nullptr;
							break;
						}
					}
					if (!u) {
						continue;
					}
				}
				len -= 4;

				MessageHandler::UDPMessageType msgType =
					static_cast< MessageHandler::UDPMessageType >((buffer[0] >> 5) & 0x7);

				if (msgType == MessageHandler::UDPVoiceSpeex || msgType == MessageHandler::UDPVoiceCELTAlpha
					|| msgType == MessageHandler::UDPVoiceCELTBeta || msgType == MessageHandler::UDPVoiceOpus) {
					// Allow all voice packets through by default.
					bool ok = true;
					// ...Unless we're in Opus mode. In Opus mode, only Opus packets are allowed.
					if (bOpus && msgType != MessageHandler::UDPVoiceOpus) {
						ok = false;
					}

					if (ok) {
						u->aiUdpFlag = 1;
						processMsg(u, buffer, len);
					}
				} else if (msgType == MessageHandler::UDPPing) {
					QByteArray qba;
					sendMessage(u, buffer, len, qba, true);
				}
#ifdef Q_OS_UNIX
				fds[i].revents = 0;
#endif
			}
		}
	}
#ifdef Q_OS_WIN
	for (int i = 0; i < nfds - 1; ++i) {
		::WSAEventSelect(fds[i], nullptr, 0);
		CloseHandle(events[i]);
	}
#endif
}

bool Server::checkDecrypt(ServerUser *u, const char *encrypt, char *plain, unsigned int len) {
	QMutexLocker l(&u->qmCrypt);

	if (u->csCrypt->isValid()
		&& u->csCrypt->decrypt(reinterpret_cast< const unsigned char * >(encrypt),
							   reinterpret_cast< unsigned char * >(plain), len))
		return true;

	if (u->csCrypt->tLastGood.elapsed() > 5000000ULL) {
		if (u->csCrypt->tLastRequest.elapsed() > 5000000ULL) {
			u->csCrypt->tLastRequest.restart();
			emit reqSync(u->uiSession);
		}
	}
	return false;
}

void Server::sendMessage(ServerUser *u, const char *data, int len, QByteArray &cache, bool force) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
	if ((u->aiUdpFlag.loadRelaxed() == 1 || force) && (u->sUdpSocket != INVALID_SOCKET)) {
#else
	// Qt 5.14 introduced QAtomicInteger::loadRelaxed() which deprecates QAtomicInteger::load()
	if ((u->aiUdpFlag.load() == 1 || force) && (u->sUdpSocket != INVALID_SOCKET)) {
#endif
#if defined(__LP64__)
		STACKVAR(char, ebuffer, len + 4 + 16);
		char *buffer = reinterpret_cast< char * >(((reinterpret_cast< quint64 >(ebuffer) + 8) & ~7) + 4);
#else
		STACKVAR(char, buffer, len + 4);
#endif
		{
			QMutexLocker wl(&u->qmCrypt);

			if (!u->csCrypt->isValid()) {
				return;
			}

			if (!u->csCrypt->encrypt(reinterpret_cast< const unsigned char * >(data),
									 reinterpret_cast< unsigned char * >(buffer), len)) {
				return;
			}
		}
#ifdef Q_OS_WIN
		DWORD dwFlow = 0;
		if (Meta::hQoS)
			QOSAddSocketToFlow(Meta::hQoS, u->sUdpSocket, reinterpret_cast< struct sockaddr * >(&u->saiUdpAddress),
							   QOSTrafficTypeVoice, QOS_NON_ADAPTIVE_FLOW, reinterpret_cast< PQOS_FLOWID >(&dwFlow));
#endif
#ifdef Q_OS_LINUX
		struct msghdr msg;
		struct iovec iov[1];

		iov[0].iov_base = buffer;
		iov[0].iov_len  = len + 4;

		uint8_t controldata[CMSG_SPACE(MAX(sizeof(struct in6_pktinfo), sizeof(struct in_pktinfo)))];
		memset(controldata, 0, sizeof(controldata));

		memset(&msg, 0, sizeof(msg));
		msg.msg_name    = reinterpret_cast< struct sockaddr * >(&u->saiUdpAddress);
		msg.msg_namelen = static_cast< socklen_t >(
			(u->saiUdpAddress.ss_family == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in));
		msg.msg_iov        = iov;
		msg.msg_iovlen     = 1;
		msg.msg_control    = controldata;
		msg.msg_controllen = CMSG_SPACE((u->saiUdpAddress.ss_family == AF_INET6) ? sizeof(struct in6_pktinfo)
																				 : sizeof(struct in_pktinfo));

		struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
		HostAddress tcpha(u->saiTcpLocalAddress);
		if (u->saiUdpAddress.ss_family == AF_INET6) {
			cmsg->cmsg_level            = IPPROTO_IPV6;
			cmsg->cmsg_type             = IPV6_PKTINFO;
			cmsg->cmsg_len              = CMSG_LEN(sizeof(struct in6_pktinfo));
			struct in6_pktinfo *pktinfo = reinterpret_cast< struct in6_pktinfo * >(CMSG_DATA(cmsg));
			memset(pktinfo, 0, sizeof(*pktinfo));
			memcpy(&pktinfo->ipi6_addr.s6_addr[0], &tcpha.qip6.c[0], sizeof(pktinfo->ipi6_addr.s6_addr));
		} else {
			cmsg->cmsg_level           = IPPROTO_IP;
			cmsg->cmsg_type            = IP_PKTINFO;
			cmsg->cmsg_len             = CMSG_LEN(sizeof(struct in_pktinfo));
			struct in_pktinfo *pktinfo = reinterpret_cast< struct in_pktinfo * >(CMSG_DATA(cmsg));
			memset(pktinfo, 0, sizeof(*pktinfo));
			if (tcpha.isV6())
				return;
			pktinfo->ipi_spec_dst.s_addr = tcpha.hash[3];
		}


		::sendmsg(u->sUdpSocket, &msg, 0);
#else
		::sendto(u->sUdpSocket, buffer, len + 4, 0, reinterpret_cast< struct sockaddr * >(&u->saiUdpAddress),
				 (u->saiUdpAddress.ss_family == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in));
#endif
#ifdef Q_OS_WIN
		if (Meta::hQoS && dwFlow)
			QOSRemoveSocketFromFlow(Meta::hQoS, 0, dwFlow, 0);
#else
#endif
	} else {
		if (cache.isEmpty())
			cache = QByteArray(data, len);
		emit tcpTransmit(cache, u->uiSession);
	}
}

#define SENDTO                                                 \
	if ((!pDst->bDeaf) && (!pDst->bSelfDeaf) && (pDst != u)) { \
		if ((poslen > 0) && (pDst->ssContext == u->ssContext)) \
			sendMessage(pDst, buffer, len, qba);               \
		else                                                   \
			sendMessage(pDst, buffer, len - poslen, qba_npos); \
	}

void Server::processMsg(ServerUser *u, const char *data, int len) {
	// Note that in this function we never have to aquire a read-lock on qrwlVoiceThread
	// as all places that call this function will hold that lock at the point of calling
	// this function.
	// This function is currently called from Server::msgUDPTunnel, Server::run and
	// Server::message
	if (u->sState != ServerUser::Authenticated || u->bMute || u->bSuppress || u->bSelfMute)
		return;

	QByteArray qba, qba_npos;
	unsigned int counter;
	char buffer[UDP_PACKET_SIZE];
	PacketDataStream pdi(data + 1, len - 1);
	PacketDataStream pds(buffer + 1, UDP_PACKET_SIZE - 1);
	unsigned int type   = data[0] & 0xe0;
	unsigned int target = data[0] & 0x1f;
	unsigned int poslen;

	// Check the voice data rate limit.
	{
		BandwidthRecord *bw = &u->bwr;

		// IP + UDP + Crypt + Data
		const int packetsize = 20 + 8 + 4 + len;

		if (!bw->addFrame(packetsize, iMaxBandwidth / 8)) {
			// Suppress packet.
			return;
		}
	}

	// Read the sequence number.
	pdi >> counter;

	// Skip to the end of the voice data.
	if ((type >> 5) != MessageHandler::UDPVoiceOpus) {
		do {
			counter = pdi.next8();
			pdi.skip(counter & 0x7f);
		} while ((counter & 0x80) && pdi.isValid());
	} else {
		int size;
		pdi >> size;
		pdi.skip(size & 0x1fff);
	}

	// Save location of the positional audio data.
	poslen = pdi.left();

	// Append session id to the new output stream.
	pds << u->uiSession;
	// Copy all voice and positional audio data to the output stream.
	pds.append(data + 1, len - 1);

	len = pds.size() + 1;

	if (poslen > static_cast< unsigned int >(len)) {
		// poslen must never ever be bigger than len as this could lead to negative buffer sizes (len - poslen) being
		// used when further processing the packet.
		// Usually this shouldn't happen in the first place but can happen with malformed/malicious packets in certain
		// cases.
		poslen = 0;
	}

	/// A set of users that'll receive the audio buffer because they are listening
	/// to a channel that received that audio.
	QSet< ServerUser * > listeningUsers;

	if (target == 0x1f) { // Server loopback
		buffer[0] = static_cast< char >(type | SpeechFlags::Normal);
		sendMessage(u, buffer, len, qba);
		return;
	} else if (target == 0) { // Normal speech
		Channel *c = u->cChannel;

		buffer[0] = static_cast< char >(type | SpeechFlags::Normal);

		// Send audio to all users that are listening to the channel
		foreach (unsigned int currentSession, m_channelListenerManager.getListenersForChannel(c->iId)) {
			ServerUser *pDst = static_cast< ServerUser * >(qhUsers.value(currentSession));
			if (pDst) {
				listeningUsers << pDst;
			}
		}

		// Send audio to all users in the same channel
		foreach (User *p, c->qlUsers) {
			ServerUser *pDst = static_cast< ServerUser * >(p);

			// As we send the audio to this particular user here, we want to make sure to not send it again due to a
			// listener proxy
			listeningUsers -= pDst;

			SENDTO;
		}

		// Send audio to all linked channels the user has speak-permission
		if (!c->qhLinks.isEmpty()) {
			QSet< Channel * > chans = c->allLinks();
			chans.remove(c);

			QMutexLocker qml(&qmCache);

			foreach (Channel *l, chans) {
				if (ChanACL::hasPermission(u, l, ChanACL::Speak, &acCache)) {
					// Send the audio stream to all users that are listening to the linked channel but are not
					// in the original channel the audio is coming from nor are they listening to the orignal
					// channel (in these cases they have received the audio already).
					foreach (unsigned int currentSession, m_channelListenerManager.getListenersForChannel(l->iId)) {
						ServerUser *pDst = static_cast< ServerUser * >(qhUsers.value(currentSession));
						if (pDst && pDst->cChannel != c
							&& !m_channelListenerManager.isListening(pDst->uiSession, c->iId)) {
							listeningUsers << pDst;
						}
					}

					// Send audio to users in the linked channel
					foreach (User *p, l->qlUsers) {
						if (!m_channelListenerManager.isListening(p->uiSession, c->iId)) {
							ServerUser *pDst = static_cast< ServerUser * >(p);

							// As we send the audio to this particular user here, we want to make sure to not send it
							// again due to a listener proxy
							listeningUsers -= pDst;

							SENDTO;
						}
					}
				}
			}
		}
	} else if (u->qmTargets.contains(target)) { // Whisper/Shout
		QSet< ServerUser * > channel;
		QSet< ServerUser * > direct;
		QSet< ServerUser * > listener;

		if (u->qmTargetCache.contains(target)) {
			const WhisperTargetCache &cache = u->qmTargetCache.value(target);
			channel                         = cache.channelTargets;
			direct                          = cache.directTargets;
			listener                        = cache.listeningTargets;
		} else {
			const WhisperTarget &wt = u->qmTargets.value(target);
			if (!wt.qlChannels.isEmpty()) {
				QMutexLocker qml(&qmCache);

				foreach (const WhisperTarget::Channel &wtc, wt.qlChannels) {
					Channel *wc = qhChannels.value(wtc.iId);
					if (wc) {
						bool link       = wtc.bLinks && !wc->qhLinks.isEmpty();
						bool dochildren = wtc.bChildren && !wc->qlChannels.isEmpty();
						bool group      = !wtc.qsGroup.isEmpty();
						if (!link && !dochildren && !group) {
							// Common case
							if (ChanACL::hasPermission(u, wc, ChanACL::Whisper, &acCache)) {
								foreach (User *p, wc->qlUsers) { channel.insert(static_cast< ServerUser * >(p)); }

								foreach (unsigned int currentSession,
										 m_channelListenerManager.getListenersForChannel(wc->iId)) {
									ServerUser *pDst = static_cast< ServerUser * >(qhUsers.value(currentSession));

									if (pDst) {
										listener << pDst;
									}
								}
							}
						} else {
							QSet< Channel * > channels;
							if (link)
								channels = wc->allLinks();
							else
								channels.insert(wc);
							if (dochildren)
								channels.unite(wc->allChildren());
							const QString &redirect = u->qmWhisperRedirect.value(wtc.qsGroup);
							const QString &qsg      = redirect.isEmpty() ? wtc.qsGroup : redirect;
							foreach (Channel *tc, channels) {
								if (ChanACL::hasPermission(u, tc, ChanACL::Whisper, &acCache)) {
									foreach (User *p, tc->qlUsers) {
										ServerUser *su = static_cast< ServerUser * >(p);

										if (!group || Group::isMember(tc, tc, qsg, su)) {
											channel.insert(su);
										}
									}

									foreach (unsigned int currentSession,
											 m_channelListenerManager.getListenersForChannel(tc->iId)) {
										ServerUser *pDst = static_cast< ServerUser * >(qhUsers.value(currentSession));

										if (pDst && (!group || Group::isMember(tc, tc, qsg, pDst))) {
											// Only send audio to listener if the user exists and it is in the group the
											// speech is directed at (if any)
											listener << pDst;
										}
									}
								}
							}
						}
					}
				}

				// If a user receives the audio through this shout anyways, we won't send it through the
				// listening channel again (and thus sending the audio twice)
				listener -= channel;
			}

			{
				QMutexLocker qml(&qmCache);

				foreach (unsigned int id, wt.qlSessions) {
					ServerUser *pDst = qhUsers.value(id);
					if (pDst && ChanACL::hasPermission(u, pDst->cChannel, ChanACL::Whisper, &acCache)
						&& !channel.contains(pDst))
						direct.insert(pDst);
				}
			}

			int uiSession = u->uiSession;
			qrwlVoiceThread.unlock();
			qrwlVoiceThread.lockForWrite();

			if (qhUsers.contains(uiSession))
				u->qmTargetCache.insert(target, { channel, direct, listener });
			qrwlVoiceThread.unlock();
			qrwlVoiceThread.lockForRead();
			if (!qhUsers.contains(uiSession))
				return;
		}
		if (!channel.isEmpty()) {
			// These users receive the audio because someone is shouting to their channel
			buffer[0] = static_cast< char >(type | SpeechFlags::Shout);
			foreach (ServerUser *pDst, channel) { SENDTO; }
			if (!direct.isEmpty()) {
				qba.clear();
				qba_npos.clear();
			}
		}
		if (!direct.isEmpty()) {
			buffer[0] = static_cast< char >(type | SpeechFlags::Whisper);
			foreach (ServerUser *pDst, direct) { SENDTO; }
		}

		// Add the listening users to the set of current listeners
		listeningUsers += listener;
	}

	// Send the audio to all listening users
	buffer[0] = static_cast< char >(type | SpeechFlags::Listen);
	foreach (ServerUser *pDst, listeningUsers) { SENDTO; }
}

void Server::log(ServerUser *u, const QString &str) const {
	QString msg = QString("<%1:%2(%3)> %4").arg(QString::number(u->uiSession), u->qsName, QString::number(u->iId), str);
	log(msg);
}

void Server::log(const QString &msg) const {
	dblog(msg);
	qWarning("%d => %s", iServerNum, msg.toUtf8().constData());
}

void Server::newClient() {
	SslServer *ss = qobject_cast< SslServer * >(sender());
	if (!ss)
		return;
	forever {
		QSslSocket *sock = ss->nextPendingSSLConnection();
		if (!sock)
			return;

		QHostAddress adr = sock->peerAddress();

		if (meta->banCheck(adr)) {
			log(QString("Ignoring connection: %1 (Global ban)")
					.arg(addressToString(sock->peerAddress(), sock->peerPort())));
			sock->disconnectFromHost();
			sock->deleteLater();
			return;
		}

		HostAddress ha(adr);

		QList< Ban > tmpBans = qlBans;
		foreach (const Ban &ban, qlBans) {
			if (ban.isExpired())
				tmpBans.removeOne(ban);
		}
		if (qlBans.count() != tmpBans.count()) {
			qlBans = tmpBans;
			saveBans();
		}

		foreach (const Ban &ban, qlBans) {
			if (ban.haAddress.match(ha, ban.iMask)) {
				log(QString("Ignoring connection: %1, Reason: %2, Username: %3, Hash: %4 (Server ban)")
						.arg(addressToString(sock->peerAddress(), sock->peerPort()), ban.qsReason, ban.qsUsername,
							 ban.qsHash));
				sock->disconnectFromHost();
				sock->deleteLater();
				return;
			}
		}

#ifdef Q_OS_MAC
		// One unexpected behavior of Qt's SSL backend is: it will add the key pair
		// it uses in a connection into the default keychain, and when access the private
		// key afterwards, a pop up will show up asking for user's permission.
		// In some case (OS X 10.15.5), this pop up will be suppressed somehow and no private
		// key is returned.
		// This env variable will avoid Qt directly adding the key pair into the default keychain,
		// using a temporary keychain instead.
		// See #4298 and https://codereview.qt-project.org/c/qt/qtbase/+/184243
		EnvUtils::setenv("QT_SSL_USE_TEMPORARY_KEYCHAIN", "1");
#endif
		sock->setPrivateKey(qskKey);
		sock->setLocalCertificateChain(qlCertificateChain);

		QSslConfiguration config = sock->sslConfiguration();

		config.setCiphers(Meta::mp.qlCiphers);
#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
		config.setDiffieHellmanParameters(qsdhpDHParams);
#endif
		sock->setSslConfiguration(config);

		if (qqIds.isEmpty()) {
			log(QString("Session ID pool (%1) empty, rejecting connection").arg(iMaxUsers));
			sock->disconnectFromHost();
			sock->deleteLater();
			return;
		}

		ServerUser *u = new ServerUser(this, sock);
		u->haAddress  = ha;
		HostAddress(sock->localAddress()).toSockaddr(&u->saiTcpLocalAddress);

		connect(u, SIGNAL(connectionClosed(QAbstractSocket::SocketError, const QString &)), this,
				SLOT(connectionClosed(QAbstractSocket::SocketError, const QString &)));
		connect(u, SIGNAL(message(unsigned int, const QByteArray &)), this,
				SLOT(message(unsigned int, const QByteArray &)));
		connect(u, SIGNAL(handleSslErrors(const QList< QSslError > &)), this,
				SLOT(sslError(const QList< QSslError > &)));
		connect(u, SIGNAL(encrypted()), this, SLOT(encrypted()));

		log(u, QString("New connection: %1").arg(addressToString(sock->peerAddress(), sock->peerPort())));

		u->setToS();

#if QT_VERSION >= 0x050500
		sock->setProtocol(QSsl::TlsV1_0OrLater);
#elif QT_VERSION >= 0x050400
		// In Qt 5.4, QSsl::SecureProtocols is equivalent
		// to "TLSv1.0 or later", which we require.
		sock->setProtocol(QSsl::SecureProtocols);
#else
		sock->setProtocol(QSsl::TlsV1_0);
#endif
		sock->startServerEncryption();

		meta->successfulConnectionFrom(adr);
	}
}

void Server::encrypted() {
	ServerUser *uSource = qobject_cast< ServerUser * >(sender());
	int major, minor, patch;
	QString release;

	Meta::getVersion(major, minor, patch, release);

	MumbleProto::Version mpv;
	mpv.set_version((major << 16) | (minor << 8) | patch);
	if (Meta::mp.bSendVersion) {
		mpv.set_release(u8(release));
		mpv.set_os(u8(meta->qsOS));
		mpv.set_os_version(u8(meta->qsOSVersion));
	}
	sendMessage(uSource, mpv);

	QList< QSslCertificate > certs = uSource->peerCertificateChain();
	if (!certs.isEmpty()) {
		const QSslCertificate &cert = certs.first();
		uSource->qslEmail           = cert.subjectAlternativeNames().values(QSsl::EmailEntry);
		uSource->qsHash             = QString::fromLatin1(cert.digest(QCryptographicHash::Sha1).toHex());
		if (!uSource->qslEmail.isEmpty() && uSource->bVerified) {
			QString subject;
			QString issuer;

			QStringList subjectList = cert.subjectInfo(QSslCertificate::CommonName);
			if (!subjectList.isEmpty()) {
				subject = subjectList.first();
			}

			QStringList issuerList = certs.first().issuerInfo(QSslCertificate::CommonName);
			if (!issuerList.isEmpty()) {
				issuer = issuerList.first();
			}

			log(uSource, QString::fromUtf8("Strong certificate for %1 <%2> (signed by %3)")
							 .arg(subject)
							 .arg(uSource->qslEmail.join(", "))
							 .arg(issuer));
		}

		foreach (const Ban &ban, qlBans) {
			if (ban.qsHash == uSource->qsHash) {
				log(uSource, QString("Certificate hash is banned: %1, Username: %2, Reason: %3.")
								 .arg(ban.qsHash, ban.qsUsername, ban.qsReason));
				uSource->disconnectSocket();
			}
		}
	}
}

void Server::sslError(const QList< QSslError > &errors) {
	ServerUser *u = qobject_cast< ServerUser * >(sender());
	if (!u)
		return;

	bool ok = true;
	foreach (QSslError e, errors) {
		switch (e.error()) {
			case QSslError::InvalidPurpose:
				// Allow email certificates.
				break;
			case QSslError::NoPeerCertificate:
			case QSslError::SelfSignedCertificate:
			case QSslError::SelfSignedCertificateInChain:
			case QSslError::UnableToGetLocalIssuerCertificate:
			case QSslError::UnableToVerifyFirstCertificate:
			case QSslError::HostNameMismatch:
			case QSslError::CertificateNotYetValid:
			case QSslError::CertificateExpired:
				u->bVerified = false;
				break;
			default:
				log(u, QString("SSL Error: %1").arg(e.errorString()));
				ok = false;
		}
	}

	if (ok) {
		u->proceedAnyway();
	} else {
		// Due to a regression in Qt 5 (QTBUG-53906),
		// we can't 'force' disconnect (which calls
		// QAbstractSocket->abort()) when built against Qt 5.
		//
		// The bug is that Qt doesn't update the
		// QSslSocket's socket state when QSslSocket->abort()
		// is called.
		//
		// Our call to abort() happens when QSslSocket is inside
		// startHandshake(). That is, a handshake is in progress.
		//
		// After emitting the peerVerifyError/sslErrors signals,
		// startHandshake() checks whether the connection is still
		// in QAbstractSocket::ConectedState.
		//
		// Unfortunately, because abort() doesn't update the socket's
		// state to signal that it is no longer connected, startHandshake()
		// still thinks the socket is connected and will continue to
		// attempt to finish the handshake.
		//
		// Because abort() tears down a lot of internal state
		// of the QSslSocket, including the 'SSL *' object
		// associated with the socket, this is fatal and leads
		// to crashes, such as attempting to derefernce a nullptr
		// 'SSL *' object.
		//
		// To avoid this, we use a non-forceful disconnect
		// until this is fixed upstream.
		//
		// See
		// https://bugreports.qt.io/browse/QTBUG-53906
		// https://github.com/mumble-voip/mumble/issues/2334

		u->disconnectSocket();
	}
}

void Server::connectionClosed(QAbstractSocket::SocketError err, const QString &reason) {
	if (reason.contains(QLatin1String("140E0197"))) {
		// A severe bug was introduced in qt/qtbase@93a803a6de27d9eb57931c431b5f3d074914f693.
		// q_SSL_shutdown() causes Qt to emit "error()" from unrelated QSslSocket(s), in addition to the correct one.
		// The issue causes this function to disconnect random authenticated clients.
		//
		// The workaround consists in ignoring a specific OpenSSL error:
		// "Error while reading: error:140E0197:SSL routines:SSL_shutdown:shutdown while in init [20]"
		//
		// Definitely not ideal, but it fixes a critical vulnerability.
		qWarning("Ignored OpenSSL error 140E0197 for %p", static_cast< void * >(sender()));
		return;
	}

	Connection *c = qobject_cast< Connection * >(sender());
	if (!c)
		return;
	if (c->bDisconnectedEmitted)
		return;
	c->bDisconnectedEmitted = true;

	ServerUser *u = static_cast< ServerUser * >(c);

	log(u, QString("Connection closed: %1 [%2]").arg(reason).arg(err));

	setLastDisconnect(u);

	if (u->sState == ServerUser::Authenticated) {
		if (m_channelListenerManager.isListeningToAny(u->uiSession)) {
			// Send nessage to all other clients that this particular user won't be listening
			// to any channel anymore
			MumbleProto::UserState mpus;
			mpus.set_session(u->uiSession);

			foreach (int channelID, m_channelListenerManager.getListenedChannelsForUser(u->uiSession)) {
				mpus.add_listening_channel_remove(channelID);

				// Also remove the client from the list on the server
				m_channelListenerManager.removeListener(u->uiSession, channelID);
			}

			sendExcept(u, mpus);
		}

		MumbleProto::UserRemove mpur;
		mpur.set_session(u->uiSession);
		sendExcept(u, mpur);

		emit userDisconnected(u);
	}

	Channel *old = u->cChannel;

	{
		QWriteLocker wl(&qrwlVoiceThread);

		qhUsers.remove(u->uiSession);
		qhHostUsers[u->haAddress].remove(u);

		quint16 port                             = (u->saiUdpAddress.ss_family == AF_INET6)
													   ? (reinterpret_cast< sockaddr_in6 * >(&u->saiUdpAddress)->sin6_port)
													   : (reinterpret_cast< sockaddr_in * >(&u->saiUdpAddress)->sin_port);
		const QPair< HostAddress, quint16 > &key = QPair< HostAddress, quint16 >(u->haAddress, port);
		qhPeerUsers.remove(key);

		if (old)
			old->removeUser(u);
	}

	if (old && old->bTemporary && old->qlUsers.isEmpty())
		QCoreApplication::instance()->postEvent(this,
												new ExecEvent(boost::bind(&Server::removeChannel, this, old->iId)));

	if (u->uiSession > 0 && static_cast< int >(u->uiSession) < iMaxUsers * 2)
		qqIds.enqueue(u->uiSession); // Reinsert session id into pool

	if (u->sState == ServerUser::Authenticated) {
		clearTempGroups(u);     // Also clears ACL cache
		recheckCodecVersions(); // Maybe can choose a better codec now
	}

	u->deleteLater();

	if (qhUsers.isEmpty())
		stopThread();
}

void Server::message(unsigned int uiType, const QByteArray &qbaMsg, ServerUser *u) {
	if (!u) {
		u = static_cast< ServerUser * >(sender());
	}

	if (u->sState == ServerUser::Authenticated) {
		u->resetActivityTime();
	}

	if (uiType == MessageHandler::UDPTunnel) {
		int len = qbaMsg.size();
		if (len < 2 || len > UDP_PACKET_SIZE) {
			// Drop messages that are too small to be senseful or that are bigger than allowed
			return;
		}

		QReadLocker rl(&qrwlVoiceThread);

		u->aiUdpFlag = 0;

		const char *buffer = qbaMsg.constData();

		MessageHandler::UDPMessageType msgType = static_cast< MessageHandler::UDPMessageType >((buffer[0] >> 5) & 0x7);

		if (msgType == MessageHandler::UDPVoiceSpeex || msgType == MessageHandler::UDPVoiceCELTAlpha
			|| msgType == MessageHandler::UDPVoiceCELTBeta || msgType == MessageHandler::UDPVoiceOpus) {
			// Allow all voice packets through by default.
			bool ok = true;
			// ...Unless we're in Opus mode. In Opus mode, only Opus packets are allowed.
			if (bOpus && msgType != MessageHandler::UDPVoiceOpus) {
				ok = false;
			}

			if (ok) {
				processMsg(u, buffer, len);
			}
		}

		return;
	}

#ifdef QT_NO_DEBUG
#	define MUMBLE_MH_MSG(x)                                             \
		case MessageHandler::x: {                                        \
			MumbleProto::x msg;                                          \
			if (msg.ParseFromArray(qbaMsg.constData(), qbaMsg.size())) { \
				msg.DiscardUnknownFields();                              \
				msg##x(u, msg);                                          \
			}                                                            \
			break;                                                       \
		}
#else
#	define MUMBLE_MH_MSG(x)                                             \
		case MessageHandler::x: {                                        \
			MumbleProto::x msg;                                          \
			if (msg.ParseFromArray(qbaMsg.constData(), qbaMsg.size())) { \
				if (uiType != MessageHandler::Ping) {                    \
					printf("== %s:\n", #x);                              \
					msg.PrintDebugString();                              \
				}                                                        \
				msg.DiscardUnknownFields();                              \
				msg##x(u, msg);                                          \
			}                                                            \
			break;                                                       \
		}
#endif

	switch (uiType) { MUMBLE_MH_ALL }

#undef MUMBLE_MH_MSG
}

void Server::checkTimeout() {
	QList< ServerUser * > qlClose;

	qrwlVoiceThread.lockForRead();
	foreach (ServerUser *u, qhUsers) {
		if (u->activityTime() > (iTimeout * 1000)) {
			log(u, "Timeout");
			qlClose.append(u);
		}
	}
	qrwlVoiceThread.unlock();
	foreach (ServerUser *u, qlClose)
		u->disconnectSocket(true);
}

void Server::tcpTransmitData(QByteArray a, unsigned int id) {
	Connection *c = qhUsers.value(id);
	if (c) {
		QByteArray qba;
		int len = a.size();

		qba.resize(len + 6);
		unsigned char *uc                      = reinterpret_cast< unsigned char * >(qba.data());
		*reinterpret_cast< quint16 * >(&uc[0]) = qToBigEndian(static_cast< quint16 >(MessageHandler::UDPTunnel));
		*reinterpret_cast< quint32 * >(&uc[2]) = qToBigEndian(static_cast< quint32 >(len));
		memcpy(uc + 6, a.constData(), len);

		c->sendMessage(qba);
		c->forceFlush();
	}
}

void Server::doSync(unsigned int id) {
	ServerUser *u = qhUsers.value(id);
	if (u) {
		log(u, "Requesting crypt-nonce resync");
		MumbleProto::CryptSetup mpcs;
		sendMessage(u, mpcs);
	}
}

void Server::sendProtoMessage(ServerUser *u, const ::google::protobuf::Message &msg, unsigned int msgType) {
	QByteArray cache;
	u->sendMessage(msg, msgType, cache);
}

void Server::sendProtoAll(const ::google::protobuf::Message &msg, unsigned int msgType, unsigned int version) {
	sendProtoExcept(nullptr, msg, msgType, version);
}

void Server::sendProtoExcept(ServerUser *u, const ::google::protobuf::Message &msg, unsigned int msgType,
							 unsigned int version) {
	QByteArray cache;
	foreach (ServerUser *usr, qhUsers)
		if ((usr != u) && (usr->sState == ServerUser::Authenticated))
			if ((version == 0) || (usr->uiVersion >= version)
				|| ((version & 0x80000000) && (usr->uiVersion < (~version))))
				usr->sendMessage(msg, msgType, cache);
}

void Server::removeChannel(int id) {
	Channel *c = qhChannels.value(id);
	if (c)
		removeChannel(c);
}

void Server::removeChannel(Channel *chan, Channel *dest) {
	Channel *c;
	User *p;

	if (!dest)
		dest = chan->cParent;

	{
		QWriteLocker wl(&qrwlVoiceThread);
		chan->unlink(nullptr);
	}

	foreach (c, chan->qlChannels) { removeChannel(c, dest); }

	foreach (p, chan->qlUsers) {
		{
			QWriteLocker wl(&qrwlVoiceThread);
			chan->removeUser(p);
		}

		Channel *target = dest;
		while (target->cParent
			   && (!hasPermission(static_cast< ServerUser * >(p), target, ChanACL::Enter)
				   || isChannelFull(target, static_cast< ServerUser * >(p))))
			target = target->cParent;

		MumbleProto::UserState mpus;
		mpus.set_session(p->uiSession);
		mpus.set_channel_id(target->iId);
		userEnterChannel(p, target, mpus);
		sendAll(mpus);
		emit userStateChanged(p);
	}

	foreach (unsigned int userSession, m_channelListenerManager.getListenersForChannel(chan->iId)) {
		m_channelListenerManager.removeListener(userSession, chan->iId);

		// Notify that all clients that have been listening to this channel, will do so no more
		MumbleProto::UserState mpus;
		mpus.set_session(userSession);
		mpus.add_listening_channel_remove(chan->iId);

		sendAll(mpus);
	}

	MumbleProto::ChannelRemove mpcr;
	mpcr.set_channel_id(chan->iId);
	sendAll(mpcr);

	removeChannelDB(chan);
	emit channelRemoved(chan);

	if (chan->cParent) {
		QWriteLocker wl(&qrwlVoiceThread);
		chan->cParent->removeChannel(chan);
	}

	delete chan;
}

bool Server::unregisterUser(int id) {
	if (!unregisterUserDB(id))
		return false;

	{
		QMutexLocker lock(&qmCache);

		foreach (Channel *c, qhChannels) {
			bool write            = false;
			QList< ChanACL * > ql = c->qlACL;

			foreach (ChanACL *acl, ql) {
				if (acl->iUserId == id) {
					c->qlACL.removeAll(acl);
					write = true;
				}
			}
			foreach (Group *g, c->qhGroups) {
				bool addrem = g->qsAdd.remove(id);
				bool remrem = g->qsRemove.remove(id);
				write       = write || addrem || remrem;
			}
			if (write)
				updateChannel(c);
		}
	}

	foreach (ServerUser *u, qhUsers) {
		if (u->iId == id) {
			clearACLCache(u);
			MumbleProto::UserState mpus;
			mpus.set_session(u->uiSession);
			mpus.set_user_id(-1);
			sendAll(mpus);

			u->iId = -1;
			break;
		}
	}
	return true;
}

void Server::userEnterChannel(User *p, Channel *c, MumbleProto::UserState &mpus) {
	if (p->cChannel == c)
		return;

	Channel *old = p->cChannel;

	{
		QWriteLocker wl(&qrwlVoiceThread);
		c->addUser(p);

		bool mayspeak = ChanACL::hasPermission(static_cast< ServerUser * >(p), c, ChanACL::Speak, nullptr);
		bool sup      = p->bSuppress;

		if (mayspeak == sup) {
			// Ok, he can speak and was suppressed, or vice versa
			p->bSuppress = !mayspeak;
			mpus.set_suppress(p->bSuppress);
		}
	}

	clearACLCache(p);
	setLastChannel(p);

	if (old && old->bTemporary && old->qlUsers.isEmpty()) {
		QCoreApplication::instance()->postEvent(this,
												new ExecEvent(boost::bind(&Server::removeChannel, this, old->iId)));
	}

	sendClientPermission(static_cast< ServerUser * >(p), c);
	if (c->cParent)
		sendClientPermission(static_cast< ServerUser * >(p), c->cParent);
}

bool Server::hasPermission(ServerUser *p, Channel *c, QFlags< ChanACL::Perm > perm) {
	QMutexLocker qml(&qmCache);
	return ChanACL::hasPermission(p, c, perm, &acCache);
}

QFlags< ChanACL::Perm > Server::effectivePermissions(ServerUser *p, Channel *c) {
	QMutexLocker qml(&qmCache);
	return ChanACL::effectivePermissions(p, c, &acCache);
}

void Server::sendClientPermission(ServerUser *u, Channel *c, bool forceupdate) {
	unsigned int perm;

	if (u->iId == 0)
		return;

	{
		QMutexLocker qml(&qmCache);
		ChanACL::hasPermission(u, c, ChanACL::Enter, &acCache);
		perm = acCache.value(u)->value(c);
	}

	if (forceupdate)
		u->iLastPermissionCheck = c->iId;

	if (u->qmPermissionSent.value(c->iId) != perm) {
		u->qmPermissionSent.insert(c->iId, perm);

		MumbleProto::PermissionQuery mppq;
		mppq.set_channel_id(c->iId);
		mppq.set_permissions(perm);

		sendMessage(u, mppq);
	}
}

/* This function is a helper for clearACLCache and assumes qmCache is held.
 * First, check if anything actually changed, or if the list is getting awfully large,
 * because this function is potentially quite expensive.
 * If all the items are still valid; great. If they aren't, send off the last channel
 * the client expliticly asked for -- this may not be what it wants, but it's our best
 * guess.
 */

void Server::flushClientPermissionCache(ServerUser *u, MumbleProto::PermissionQuery &mppq) {
	QMap< int, unsigned int >::const_iterator i;
	bool match = (u->qmPermissionSent.count() < 20);
	for (i = u->qmPermissionSent.constBegin(); (match && (i != u->qmPermissionSent.constEnd())); ++i) {
		Channel *c = qhChannels.value(i.key());
		if (!c) {
			match = false;
		} else {
			ChanACL::hasPermission(u, c, ChanACL::Enter, &acCache);
			unsigned int perm = acCache.value(u)->value(c);
			if (perm != i.value())
				match = false;
		}
	}

	if (match)
		return;

	u->qmPermissionSent.clear();

	Channel *c = qhChannels.value(u->iLastPermissionCheck);
	if (!c) {
		c                       = u->cChannel;
		u->iLastPermissionCheck = c->iId;
	}

	ChanACL::hasPermission(u, c, ChanACL::Enter, &acCache);
	unsigned int perm = acCache.value(u)->value(c);
	u->qmPermissionSent.insert(c->iId, perm);

	mppq.Clear();
	mppq.set_channel_id(c->iId);
	mppq.set_permissions(perm);
	mppq.set_flush(true);

	sendMessage(u, mppq);
}

void Server::clearACLCache(User *p) {
	MumbleProto::PermissionQuery mppq;

	{
		QMutexLocker qml(&qmCache);

		if (p) {
			ChanACL::ChanCache *h = acCache.take(p);
			delete h;

			flushClientPermissionCache(static_cast< ServerUser * >(p), mppq);
		} else {
			foreach (ChanACL::ChanCache *h, acCache)
				delete h;
			acCache.clear();

			foreach (ServerUser *u, qhUsers)
				if (u->sState == ServerUser::Authenticated)
					flushClientPermissionCache(u, mppq);
		}
	}

	// A change in ACLs means that the user might be able to whisper
	// to users it didn't have permission to do before (or vice versa)
	clearWhisperTargetCache();
}

void Server::clearWhisperTargetCache() {
	QWriteLocker lock(&qrwlVoiceThread);

	foreach (ServerUser *u, qhUsers) { u->qmTargetCache.clear(); }
}

QString Server::addressToString(const QHostAddress &adr, unsigned short port) {
	HostAddress ha(adr);

	if ((Meta::mp.iObfuscate != 0)) {
		QCryptographicHash h(QCryptographicHash::Sha1);
		h.addData(reinterpret_cast< const char * >(&Meta::mp.iObfuscate), sizeof(Meta::mp.iObfuscate));
		if (adr.protocol() == QAbstractSocket::IPv4Protocol) {
			quint32 num = adr.toIPv4Address();
			h.addData(reinterpret_cast< const char * >(&num), sizeof(num));
		} else if (adr.protocol() == QAbstractSocket::IPv6Protocol) {
			Q_IPV6ADDR num = adr.toIPv6Address();
			h.addData(reinterpret_cast< const char * >(num.c), sizeof(num.c));
		}
		return QString("<<%1:%2>>").arg(QString::fromLatin1(h.result().toHex()), QString::number(port));
	}
	return QString("%1:%2").arg(ha.toString(), QString::number(port));
}

bool Server::validateUserName(const QString &name) {
	// We expect the name passed to this function to be fully trimmed already. This way we
	// prevent "empty" names (at least with the default username restriction).
	return (name.trimmed().length() == name.length() && qrUserName.exactMatch(name) && (name.length() <= 512));
}

bool Server::validateChannelName(const QString &name) {
	return (qrChannelName.exactMatch(name) && (name.length() <= 512));
}

void Server::recheckCodecVersions(ServerUser *connectingUser) {
	QMap< int, int > qmCodecUsercount;
	QMap< int, int >::const_iterator i;
	int users = 0;
	int opus  = 0;

	// Count how many users use which codec
	foreach (ServerUser *u, qhUsers) {
		if (u->qlCodecs.isEmpty() && !u->bOpus)
			continue;

		++users;
		if (u->bOpus)
			++opus;

		foreach (int version, u->qlCodecs)
			++qmCodecUsercount[version];
	}

	if (!users)
		return;

	// Enable Opus if the number of users with Opus is higher than the threshold
	bool enableOpus = ((opus * 100 / users) >= iOpusThreshold);

	// Find the best possible codec most users support
	int version       = 0;
	int maximum_users = 0;
	i                 = qmCodecUsercount.constEnd();
	do {
		--i;
		if (i.value() > maximum_users) {
			version       = i.key();
			maximum_users = i.value();
		}
	} while (i != qmCodecUsercount.constBegin());

	int current_version = bPreferAlpha ? iCodecAlpha : iCodecBeta;

	// If we don't already use the compat bitstream version set
	// it as alpha and announce it. If another codec now got the
	// majority set it as the opposite of the currently valid bPreferAlpha
	// and announce it.

	if (current_version != version) {
		if (version == static_cast< qint32 >(0x8000000b))
			bPreferAlpha = true;
		else
			bPreferAlpha = !bPreferAlpha;

		if (bPreferAlpha)
			iCodecAlpha = version;
		else
			iCodecBeta = version;
	} else if (bOpus == enableOpus) {
		if (bOpus && connectingUser && !connectingUser->bOpus) {
			sendTextMessage(
				nullptr, connectingUser, false,
				QLatin1String(
					"<strong>WARNING:</strong> Your client doesn't support the Opus codec the server is using, you "
					"won't be able to talk or hear anyone. Please upgrade to a client with Opus support."));
		}
		return;
	}

	bOpus = enableOpus;

	MumbleProto::CodecVersion mpcv;
	mpcv.set_alpha(iCodecAlpha);
	mpcv.set_beta(iCodecBeta);
	mpcv.set_prefer_alpha(bPreferAlpha);
	mpcv.set_opus(bOpus);
	sendAll(mpcv);

	if (bOpus) {
		foreach (ServerUser *u, qhUsers) {
			// Prevent connected users that could not yet declare their opus capability during msgAuthenticate from
			// being spammed. Only authenticated users and the currently connecting user (if recheck is called in that
			// context) have a reliable u->bOpus.
			if ((u->sState == ServerUser::Authenticated || u == connectingUser) && !u->bOpus) {
				sendTextMessage(
					nullptr, u, false,
					QLatin1String(
						"<strong>WARNING:</strong> Your client doesn't support the Opus codec the server is switching "
						"to, you won't be able to talk or hear anyone. Please upgrade to a client with Opus support."));
			}
		}
	}

	log(QString::fromLatin1("CELT codec switch %1 %2 (prefer %3) (Opus %4)")
			.arg(iCodecAlpha, 0, 16)
			.arg(iCodecBeta, 0, 16)
			.arg(bPreferAlpha ? iCodecAlpha : iCodecBeta, 0, 16)
			.arg(bOpus));
}

void Server::hashAssign(QString &dest, QByteArray &hash, const QString &src) {
	dest = src;
	if (src.length() >= 128)
		hash = sha1(src);
	else
		hash = QByteArray();
}

void Server::hashAssign(QByteArray &dest, QByteArray &hash, const QByteArray &src) {
	dest = src;
	if (src.length() >= 128)
		hash = sha1(src);
	else
		hash = QByteArray();
}

bool Server::isTextAllowed(QString &text, bool &changed) {
	changed = false;

	if (!bAllowHTML) {
		QString out;
		if (HTMLFilter::filter(text, out)) {
			changed = true;
			text    = out;
		}
		return ((iMaxTextMessageLength == 0) || (text.length() <= iMaxTextMessageLength));
	} else {
		int length = text.length();

		// No limits
		if ((iMaxTextMessageLength == 0) && (iMaxImageMessageLength == 0))
			return true;

		// Over Image limit? (If so, always fail)
		if ((iMaxImageMessageLength != 0) && (length > iMaxImageMessageLength))
			return false;

		// Under textlength?
		if ((iMaxTextMessageLength == 0) || (length <= iMaxTextMessageLength))
			return true;

		// Over textlength, under imagelength. If no XML, this is a fail.
		if (!text.contains(QLatin1Char('<')))
			return false;

		// Strip value from <img>s src attributes to check text-length only -
		// we already ensured the img-length requirement is met
		QString qsOut;
		QXmlStreamReader qxsr(QString::fromLatin1("<document>%1</document>").arg(text));
		QXmlStreamWriter qxsw(&qsOut);
		while (!qxsr.atEnd()) {
			switch (qxsr.readNext()) {
				case QXmlStreamReader::Invalid:
					return false;
				case QXmlStreamReader::StartElement: {
					if (qxsr.name() == QLatin1String("img")) {
						qxsw.writeStartElement(qxsr.namespaceUri().toString(), qxsr.name().toString());
						foreach (const QXmlStreamAttribute &a, qxsr.attributes())
							if (a.name() != QLatin1String("src"))
								qxsw.writeAttribute(a);
					} else {
						qxsw.writeCurrentToken(qxsr);
					}
				} break;
				default:
					qxsw.writeCurrentToken(qxsr);
					break;
			}
		}

		length = qsOut.length();

		return (length <= iMaxTextMessageLength);
	}
}

bool Server::isChannelFull(Channel *c, ServerUser *u) {
	if (u && hasPermission(u, c, ChanACL::Write)) {
		return false;
	}
	if (c->uiMaxUsers) {
		return static_cast< unsigned int >(c->qlUsers.count()) >= c->uiMaxUsers;
	}
	if (iMaxUsersPerChannel) {
		return c->qlUsers.count() >= iMaxUsersPerChannel;
	}
	return false;
}

bool Server::canNest(Channel *newParent, Channel *channel) const {
	const int parentLevel  = newParent ? static_cast< int >(newParent->getLevel()) : -1;
	const int channelDepth = channel ? static_cast< int >(channel->getDepth()) : 0;

	return (parentLevel + channelDepth) < iChannelNestingLimit;
}

#undef MAX
#undef UDP_PACKET_SIZE
#undef SIO_UDP_CONNRESET
#undef SENDTO
