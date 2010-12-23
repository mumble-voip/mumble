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

#include "murmur_pch.h"

#include "User.h"
#include "Channel.h"
#include "ACL.h"
#include "Group.h"
#include "Message.h"
#include "ServerDB.h"
#include "Connection.h"
#include "Server.h"
#include "DBus.h"
#include "Meta.h"
#include "PacketDataStream.h"
#include "ServerUser.h"

#define UDP_PACKET_SIZE 1024

LogEmitter::LogEmitter(QObject *p) : QObject(p) {
};

void LogEmitter::addLogEntry(const QString &msg) {
	emit newLogEntry(msg);
};

ExecEvent::ExecEvent(boost::function<void ()> f) : QEvent(static_cast<QEvent::Type>(EXEC_QEVENT)) {
	func = f;
}

void ExecEvent::execute() {
	func();
}

SslServer::SslServer(QObject *p) : QTcpServer(p) {
}

void SslServer::incomingConnection(int v) {
	QSslSocket *s = new QSslSocket(this);
	s->setSocketDescriptor(v);
	qlSockets.append(s);
}

QSslSocket *SslServer::nextPendingSSLConnection() {
	if (qlSockets.isEmpty())
		return NULL;
	return qlSockets.takeFirst();
}

Server::Server(int snum, QObject *p) : QThread(p) {
	bValid = true;
	iServerNum = snum;
#ifdef USE_BONJOUR
	bsRegistration = NULL;
#endif

#ifdef Q_OS_UNIX
	aiNotify[0] = aiNotify[1] = -1;
#else
	hNotify = NULL;
#endif
	qtTimeout = new QTimer(this);

	iCodecAlpha = iCodecBeta = 0;
	bPreferAlpha = false;

	qnamNetwork = NULL;

	readParams();
	initialize();

	foreach(const QHostAddress &qha, qlBind) {
		SslServer *ss = new SslServer(this);

		connect(ss, SIGNAL(newConnection()), this, SLOT(newClient()), Qt::QueuedConnection);

		if (! ss->listen(qha, usPort)) {
			log(QString("Server: TCP Listen on %1 failed: %2").arg(addressToString(qha,usPort), ss->errorString()));
			bValid = false;
		} else {
			log(QString("Server listening on %1").arg(addressToString(qha,usPort)));
		}
		qlServer << ss;
	}

	if (! bValid)
		return;

	foreach(SslServer *ss, qlServer) {
		sockaddr_storage addr;
#ifdef Q_OS_UNIX
		int tcpsock = ss->socketDescriptor();
		socklen_t len = sizeof(addr);
#else
		SOCKET tcpsock = ss->socketDescriptor();
		int len = sizeof(addr);
#endif
		memset(&addr, 0, sizeof(addr));
		getsockname(tcpsock, reinterpret_cast<struct sockaddr *>(&addr), &len);
#ifdef Q_OS_UNIX
		int sock = ::socket(addr.ss_family, SOCK_DGRAM, 0);
#else
#ifndef SIO_UDP_CONNRESET
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
#endif
		SOCKET sock = ::WSASocket(addr.ss_family, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
		DWORD dwBytesReturned = 0;
		BOOL bNewBehaviour = FALSE;
		if (WSAIoctl(sock, SIO_UDP_CONNRESET, &bNewBehaviour, sizeof(bNewBehaviour), NULL, 0, &dwBytesReturned, NULL, NULL) == SOCKET_ERROR) {
			log(QString("Failed to set SIO_UDP_CONNRESET: %1").arg(WSAGetLastError()));
		}
#endif
		if (sock == INVALID_SOCKET) {
			log("Failed to create UDP Socket");
			bValid = false;
			return;
		} else {
			if (::bind(sock, reinterpret_cast<sockaddr *>(&addr), len) == SOCKET_ERROR) {
				log(QString("Failed to bind UDP Socket to %1").arg(addressToString(ss->serverAddress(), usPort)));
			} else {
#ifdef Q_OS_UNIX
				int val = 0xe0;
				if (setsockopt(sock, IPPROTO_IP, IP_TOS, &val, sizeof(val))) {
					val = 0x80;
					if (setsockopt(sock, IPPROTO_IP, IP_TOS, &val, sizeof(val)))
						log("Server: Failed to set TOS for UDP Socket");
				}
#if defined(SO_PRIORITY)
				socklen_t optlen = sizeof(val);
				if (getsockopt(sock, SOL_SOCKET, SO_PRIORITY, &val, &optlen) == 0) {
					if (val == 0) {
						val = 6;
						setsockopt(sock, SOL_SOCKET, SO_PRIORITY, &val, sizeof(val));
					}
				}
#endif
#endif
			}
			QSocketNotifier *qsn = new QSocketNotifier(sock, QSocketNotifier::Read, this);
			connect(qsn, SIGNAL(activated(int)), this, SLOT(udpActivated(int)));
			qlUdpSocket << sock;
			qlUdpNotifier << qsn;
		}
	}

	bValid = bValid && (qlServer.count() == qlBind.count()) && (qlUdpSocket.count() == qlBind.count());
	if (! bValid)
		return;

#ifdef Q_OS_UNIX
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, aiNotify) != 0) {
		log("Failed to create notify socket");
		bValid = false;
		return;
	}
#else
	hNotify = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif

	connect(this, SIGNAL(tcpTransmit(QByteArray, unsigned int)), this, SLOT(tcpTransmitData(QByteArray, unsigned int)), Qt::QueuedConnection);
	connect(this, SIGNAL(reqSync(unsigned int)), this, SLOT(doSync(unsigned int)));

	for (int i=1;i<iMaxUsers*2;++i)
		qqIds.enqueue(i);

	connect(qtTimeout, SIGNAL(timeout()), this, SLOT(checkTimeout()));

	getBans();
	readChannels();
	readLinks();
	initializeCert();

	int major, minor, patch;
	QString release;
	Meta::getVersion(major, minor, patch, release);

	uiVersionBlob = qToBigEndian(static_cast<quint32>((major<<16) | (minor << 8) | patch));

	if (bValid) {
#ifdef USE_BONJOUR
		if (bBonjour)
			initBonjour();
#endif
		initRegister();

	}
}

void Server::startThread() {
	if (! isRunning()) {
		log("Starting voice thread");
		bRunning = true;

		foreach(QSocketNotifier *qsn, qlUdpNotifier)
			qsn->setEnabled(false);
		start(QThread::HighestPriority);
#ifdef Q_OS_LINUX
		// QThread::HighestPriority == Same as everything else...
		int policy;
		struct sched_param param;
		if (pthread_getschedparam(pthread_self(), &policy, &param) == 0) {
			if (policy == SCHED_OTHER) {
				policy = SCHED_FIFO;
				param.sched_priority = 1;
				pthread_setschedparam(pthread_self(), policy, &param);
			}
		}
#endif
	}
	if (! qtTimeout->isActive())
		qtTimeout->start(15500);
}

void Server::stopThread() {
	bRunning = false;
	if (isRunning()) {
		log("Ending voice thread");

#ifdef Q_OS_UNIX
		unsigned char val = 0;
		::write(aiNotify[1], &val, 1);
#else
		SetEvent(hNotify);
#endif
		wait();

		foreach(QSocketNotifier *qsn, qlUdpNotifier)
			qsn->setEnabled(true);
	}
	qtTimeout->stop();
}

Server::~Server() {
#ifdef USE_BONJOUR
	removeBonjour();
#endif

	stopThread();

	foreach(QSocketNotifier *qsn, qlUdpNotifier)
		delete qsn;

#ifdef Q_OS_UNIX
	foreach(int s, qlUdpSocket)
		close(s);

	if (aiNotify[0] >= 0)
		close(aiNotify[0]);
	if (aiNotify[1] >= 0)
		close(aiNotify[1]);
#else
	foreach(SOCKET s, qlUdpSocket)
		closesocket(s);
	if (hNotify)
		CloseHandle(hNotify);
#endif
	clearACLCache();

	log("Stopped");
}

void Server::readParams() {
	qsPassword = Meta::mp.qsPassword;
	usPort = static_cast<unsigned short>(Meta::mp.usPort + iServerNum - 1);
	iTimeout = Meta::mp.iTimeout;
	iMaxBandwidth = Meta::mp.iMaxBandwidth;
	iMaxUsers = Meta::mp.iMaxUsers;
	iMaxUsersPerChannel = Meta::mp.iMaxUsersPerChannel;
	iMaxTextMessageLength = Meta::mp.iMaxTextMessageLength;
	iMaxImageMessageLength = Meta::mp.iMaxImageMessageLength;
	bAllowHTML = Meta::mp.bAllowHTML;
	iDefaultChan = Meta::mp.iDefaultChan;
	bRememberChan = Meta::mp.bRememberChan;
	qsWelcomeText = Meta::mp.qsWelcomeText;
	qlBind = Meta::mp.qlBind;
	qsRegName = Meta::mp.qsRegName;
	qsRegPassword = Meta::mp.qsRegPassword;
	qsRegHost = Meta::mp.qsRegHost;
	qsRegLocation = Meta::mp.qsRegLocation;
	qurlRegWeb = Meta::mp.qurlRegWeb;
	bBonjour = Meta::mp.bBonjour;
	bAllowPing = Meta::mp.bAllowPing;
	bCertRequired = Meta::mp.bCertRequired;
	qrUserName = Meta::mp.qrUserName;
	qrChannelName = Meta::mp.qrChannelName;

	QString qsHost = getConf("host", QString()).toString();
	if (! qsHost.isEmpty()) {
		qlBind.clear();
		foreach(const QString &host, qsHost.split(QRegExp(QLatin1String("\\s+")), QString::SkipEmptyParts)) {
			QHostAddress qhaddr;
			if (qhaddr.setAddress(qsHost)) {
				qlBind << qhaddr;
			} else {
				bool found = false;
				QHostInfo hi = QHostInfo::fromName(host);
				foreach(QHostAddress qha, hi.addresses()) {
					if ((qha.protocol() == QAbstractSocket::IPv4Protocol) || (qha.protocol() == QAbstractSocket::IPv6Protocol)) {
						qlBind << qha;
						found = true;
					}
				}
				if (! found) {
					log(QString("Lookup of bind hostname %1 failed").arg(host));
				}
			}
		}
		foreach(const QHostAddress &qha, qlBind)
			log(QString("Binding to address %1").arg(qha.toString()));
		if (qlBind.isEmpty())
			qlBind = Meta::mp.qlBind;
	}

	qsPassword = getConf("password", qsPassword).toString();
	usPort = static_cast<unsigned short>(getConf("port", usPort).toUInt());
	iTimeout = getConf("timeout", iTimeout).toInt();
	iMaxBandwidth = getConf("bandwidth", iMaxBandwidth).toInt();
	iMaxUsers = getConf("users", iMaxUsers).toInt();
	iMaxUsersPerChannel = getConf("usersperchannel", iMaxUsersPerChannel).toInt();
	iMaxTextMessageLength = getConf("textmessagelength", iMaxTextMessageLength).toInt();
	iMaxImageMessageLength = getConf("imagemessagelength", iMaxImageMessageLength).toInt();
	bAllowHTML = getConf("allowhtml", bAllowHTML).toBool();
	iDefaultChan = getConf("defaultchannel", iDefaultChan).toInt();
	bRememberChan = getConf("rememberchannel", bRememberChan).toBool();
	qsWelcomeText = getConf("welcometext", qsWelcomeText).toString();

	qsRegName = getConf("registername", qsRegName).toString();
	qsRegPassword = getConf("registerpassword", qsRegPassword).toString();
	qsRegHost = getConf("registerhostname", qsRegHost).toString();
	qsRegLocation = getConf("registerlocation", qsRegLocation).toString();
	qurlRegWeb = QUrl(getConf("registerurl", qurlRegWeb.toString()).toString());
	bBonjour = getConf("bonjour", bBonjour).toBool();
	bAllowPing = getConf("allowping", bAllowPing).toBool();
	bCertRequired = getConf("certrequired", bCertRequired).toBool();

	qrUserName=QRegExp(getConf("username", qrUserName.pattern()).toString());
	qrChannelName=QRegExp(getConf("channelname", qrChannelName.pattern()).toString());
}

void Server::setLiveConf(const QString &key, const QString &value) {
	QString v = value.trimmed().isEmpty() ? QString() : value;
	int i = v.toInt();
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
		for (int i=iMaxUsers*2;i<newmax*2;++i)
			qqIds.enqueue(i);
		iMaxUsers = newmax;
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
	else if (key == "welcometext") {
		QString text = !v.isNull() ? v : Meta::mp.qsWelcomeText;
		if (text != qsWelcomeText) {
			qsWelcomeText = text;
			if (! qsWelcomeText.isEmpty()) {
				MumbleProto::ServerConfig mpsc;
				mpsc.set_welcome_text(u8(qsWelcomeText));
				sendAll(mpsc);
			}
		}
	} else if (key == "registername") {
		QString text = !v.isNull() ? v : Meta::mp.qsRegName;
		if (text != qsRegName) {
			qsRegName = text;
			if (! qsRegName.isEmpty()) {
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
	else if (key == "bonjour") {
		bBonjour = !v.isNull() ? QVariant(v).toBool() : Meta::mp.bBonjour;
#ifdef USE_BONJOUR
		if (bBonjour && !bsRegistration)
			initBonjour();
		else if (!bBonjour && bsRegistration)
			removeBonjour();
#endif
	} else if (key == "allowping")
		bAllowPing = !v.isNull() ? QVariant(v).toBool() : Meta::mp.bAllowPing;
	else if (key == "username")
		qrUserName=!v.isNull() ? QRegExp(v) : Meta::mp.qrUserName;
	else if (key == "channelname")
		qrChannelName=!v.isNull() ? QRegExp(v) : Meta::mp.qrChannelName;
}

#ifdef USE_BONJOUR
void Server::initBonjour() {
	bsRegistration = new BonjourServer();
	if (bsRegistration->bsrRegister) {
		log("Announcing server via bonjour");
		bsRegistration->bsrRegister->registerService(BonjourRecord(qsRegName, "_mumble._tcp", ""),
		        usPort);
	}
}

void Server::removeBonjour() {
	if (bsRegistration) {
		delete bsRegistration;
		bsRegistration = NULL;
		log("Stopped announcing server via bonjour");
	}
}
#endif

void Server::customEvent(QEvent *evt) {
	if (evt->type() == EXEC_QEVENT)
		static_cast<ExecEvent *>(evt)->execute();
}

void Server::udpActivated(int socket) {
	qint32 len;
	char encrypt[UDP_PACKET_SIZE];
	sockaddr_storage from;
#ifdef Q_OS_UNIX
	socklen_t fromlen = sizeof(from);
	int &sock = socket;
	len=static_cast<qint32>(::recvfrom(sock, encrypt, UDP_PACKET_SIZE, MSG_TRUNC, reinterpret_cast<struct sockaddr *>(&from), &fromlen));
#else
	int fromlen = sizeof(from);
	SOCKET sock = static_cast<SOCKET>(socket);
	len=::recvfrom(sock, encrypt, UDP_PACKET_SIZE, 0, reinterpret_cast<struct sockaddr *>(&from), &fromlen);
#endif

	// Cloned from ::run(), as it's the only UDP data we care about until the thread is started.
	quint32 *ping = reinterpret_cast<quint32 *>(encrypt);
	if ((len == 12) && (*ping == 0) && bAllowPing) {
		ping[0] = uiVersionBlob;
		ping[3] = qToBigEndian(static_cast<quint32>(qhUsers.count()));
		ping[4] = qToBigEndian(static_cast<quint32>(iMaxUsers));
		ping[5] = qToBigEndian(static_cast<quint32>(iMaxBandwidth));

		::sendto(sock, encrypt, 6 * sizeof(quint32), 0, reinterpret_cast<struct sockaddr *>(&from), fromlen);
	}
}

void Server::run() {
	qint32 len;
#if defined(__LP64__)
	char encbuff[UDP_PACKET_SIZE+8];
	char *encrypt = encbuff + 4;
#else
	char encrypt[UDP_PACKET_SIZE];
#endif
	char buffer[UDP_PACKET_SIZE];

	sockaddr_storage from;
	int nfds = qlUdpSocket.count();

#ifdef Q_OS_UNIX
	socklen_t fromlen;
	STACKVAR(struct pollfd, fds, nfds+1);

	for (int i=0;i<nfds;++i) {
		fds[i].fd = qlUdpSocket.at(i);
		fds[i].events = POLLIN;
		fds[i].revents = 0;
	}

	fds[nfds].fd=aiNotify[0];
	fds[nfds].events = POLLIN;
	fds[nfds].revents = 0;
#else
	int fromlen;
	STACKVAR(SOCKET, fds, nfds);
	STACKVAR(HANDLE, events, nfds+1);
	for (int i=0;i<nfds;++i) {
		fds[i] = qlUdpSocket.at(i);
		events[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
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
			while (::recv(aiNotify[0], &val, 1, MSG_DONTWAIT) == 1) {};
			break;
		}

		for (int i=0;i<nfds-1;++i) {
			if (fds[i].revents) {
				if (fds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
					qCritical("poll event failure");
					bRunning = false;
					break;
				}

				int sock = fds[i].fd;
#else
		{
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
				len=::recvfrom(sock, encrypt, UDP_PACKET_SIZE, 0, reinterpret_cast<struct sockaddr *>(&from), &fromlen);
#else
				len=static_cast<qint32>(::recvfrom(sock, encrypt, UDP_PACKET_SIZE, MSG_TRUNC, reinterpret_cast<struct sockaddr *>(&from), &fromlen));
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

				QReadLocker rl(&qrwlUsers);

				quint32 *ping = reinterpret_cast<quint32 *>(encrypt);

				if ((len == 12) && (*ping == 0) && bAllowPing) {
					ping[0] = uiVersionBlob;
					// 1 and 2 will be the timestamp, which we return unmodified.
					ping[3] = qToBigEndian(static_cast<quint32>(qhUsers.count()));
					ping[4] = qToBigEndian(static_cast<quint32>(iMaxUsers));
					ping[5] = qToBigEndian(static_cast<quint32>(iMaxBandwidth));

					::sendto(sock, encrypt, 6 * sizeof(quint32), 0, reinterpret_cast<struct sockaddr *>(&from), fromlen);
					continue;
				}


				quint16 port = (from.ss_family == AF_INET6) ? (reinterpret_cast<sockaddr_in6 *>(&from)->sin6_port) : (reinterpret_cast<sockaddr_in *>(&from)->sin_port);
				const HostAddress &ha = HostAddress(from);

				const QPair<HostAddress, quint16> &key = QPair<HostAddress, quint16>(ha, port);

				ServerUser *u = qhPeerUsers.value(key);
				if (u) {
					if (! checkDecrypt(u, encrypt, buffer, len)) {
						continue;
					}
				} else {
					// Unknown peer
					foreach(ServerUser *usr, qhHostUsers.value(ha)) {
						if (usr->csCrypt.isValid() && checkDecrypt(usr, encrypt, buffer, len)) {
							// Every time we relock, reverify users' existance.
							// The main thread might delete the user while the lock isn't held.
							unsigned int uiSession = usr->uiSession;
							rl.unlock();
							qrwlUsers.lockForWrite();
							if (qhUsers.contains(uiSession)) {
								u = usr;
								u->sUdpSocket = sock;
								memcpy(& u->saiUdpAddress, &from, sizeof(from));
								qhHostUsers[from].remove(u);
								qhPeerUsers.insert(key, u);
								qrwlUsers.unlock();
								rl.relock();
								if (! qhUsers.contains(uiSession))
									u = NULL;
							}
							break;
						}
					}
					if (! u) {
						continue;
					}
				}
				len -= 4;


				MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((buffer[0] >> 5) & 0x7);

				switch (msgType) {
					case MessageHandler::UDPVoiceSpeex:
					case MessageHandler::UDPVoiceCELTAlpha:
					case MessageHandler::UDPVoiceCELTBeta: {
							u->bUdp = true;
							processMsg(u, buffer, len);
							break;
						}
					case MessageHandler::UDPPing: {
							QByteArray qba;
							sendMessage(u, buffer, len, qba, true);
						}
				}
#ifdef Q_OS_UNIX
				fds[i].revents = 0;
#endif
			}
		}
	}
#ifdef Q_OS_WIN
	for (int i=0;i<nfds-1;++i) {
		::WSAEventSelect(fds[i], NULL, 0);
		CloseHandle(events[i]);
	}
#endif
}

bool Server::checkDecrypt(ServerUser *u, const char *encrypt, char *plain, unsigned int len) {
	if (u->csCrypt.isValid() && u->csCrypt.decrypt(reinterpret_cast<const unsigned char *>(encrypt), reinterpret_cast<unsigned char *>(plain), len))
		return true;

	if (u->csCrypt.tLastGood.elapsed() > 5000000ULL) {
		if (u->csCrypt.tLastRequest.elapsed() > 5000000ULL) {
			u->csCrypt.tLastRequest.restart();
			emit reqSync(u->uiSession);
		}
	}
	return false;
}

void Server::sendMessage(ServerUser *u, const char *data, int len, QByteArray &cache, bool force) {
	if ((u->bUdp || force) && (u->sUdpSocket != INVALID_SOCKET) && u->csCrypt.isValid()) {
#if defined(__LP64__)
		STACKVAR(char, ebuffer, len+4+16);
		char *buffer = reinterpret_cast<char *>(((reinterpret_cast<quint64>(ebuffer) + 8) & ~7) + 4);
#else
		STACKVAR(char, buffer, len+4);
#endif
		u->csCrypt.encrypt(reinterpret_cast<const unsigned char *>(data), reinterpret_cast<unsigned char *>(buffer), len);
#ifdef Q_OS_WIN
		DWORD dwFlow = 0;
		if (Meta::hQoS)
			QOSAddSocketToFlow(Meta::hQoS, u->sUdpSocket, reinterpret_cast<struct sockaddr *>(& u->saiUdpAddress), QOSTrafficTypeVoice, QOS_NON_ADAPTIVE_FLOW, &dwFlow);
#endif
		::sendto(u->sUdpSocket, buffer, len+4, 0, reinterpret_cast<struct sockaddr *>(& u->saiUdpAddress), (u->saiUdpAddress.ss_family == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in));
#ifdef Q_OS_WIN
		if (Meta::hQoS && dwFlow)
			QOSRemoveSocketFromFlow(Meta::hQoS, 0, dwFlow, 0);
#else
#endif


	} else {
		if (cache.isEmpty())
			cache = QByteArray(data, len);
		emit tcpTransmit(cache,u->uiSession);
	}
}

#define SENDTO \
		if ((!pDst->bDeaf) && (!pDst->bSelfDeaf) && (pDst != u)) { \
			if ((poslen > 0) && (pDst->ssContext == u->ssContext)) \
				sendMessage(pDst, buffer, len, qba); \
			else \
				sendMessage(pDst, buffer, len - poslen, qba_npos); \
		}

void Server::processMsg(ServerUser *u, const char *data, int len) {
	if (u->sState != ServerUser::Authenticated || u->bMute || u->bSuppress || u->bSelfMute)
		return;

	User *p;
	BandwidthRecord *bw = & u->bwr;
	Channel *c = u->cChannel;
	QByteArray qba, qba_npos;
	unsigned int counter;
	char buffer[UDP_PACKET_SIZE];
	PacketDataStream pdi(data + 1, len - 1);
	PacketDataStream pds(buffer+1, UDP_PACKET_SIZE-1);
	unsigned int type = data[0] & 0xe0;
	unsigned int target = data[0] & 0x1f;
	unsigned int poslen;

	// IP + UDP + Crypt + Data
	int packetsize = 20 + 8 + 4 + len;

	if (! bw->addFrame(packetsize, iMaxBandwidth/8)) {
		// Suppress packet.
		return;
	}

	pdi >> counter;

	do {
		counter = pdi.next8();
		pdi.skip(counter & 0x7f);
	} while ((counter & 0x80) && pdi.isValid());

	poslen = pdi.left();

	pds << u->uiSession;
	pds.append(data + 1, len - 1);

	len = pds.size() + 1;

	if (target == 0x1f) { // Server loopback
		buffer[0] = static_cast<char>(type | 0);
		sendMessage(u, buffer, len, qba);
		return;
	} else if (target == 0) { // Normal speech
		buffer[0] = static_cast<char>(type | 0);
		foreach(p, c->qlUsers) {
			ServerUser *pDst = static_cast<ServerUser *>(p);
			SENDTO;
		}

		if (! c->qhLinks.isEmpty()) {
			QSet<Channel *> chans = c->allLinks();
			chans.remove(c);

			QMutexLocker qml(&qmCache);

			foreach(Channel *l, chans) {
				if (ChanACL::hasPermission(u, l, ChanACL::Speak, acCache)) {
					foreach(p, l->qlUsers) {
						ServerUser *pDst = static_cast<ServerUser *>(p);
						SENDTO;
					}
				}
			}
		}
	} else if (u->qmTargets.contains(target)) { // Whisper
		QSet<ServerUser *> channel;
		QSet<ServerUser *> direct;

		if (u->qmTargetCache.contains(target)) {
			const ServerUser::TargetCache &cache = u->qmTargetCache.value(target);
			channel = cache.first;
			direct = cache.second;
		} else {
			const WhisperTarget &wt = u->qmTargets.value(target);
			if (! wt.qlChannels.isEmpty()) {
				QMutexLocker qml(&qmCache);

				foreach(const WhisperTarget::Channel &wtc, wt.qlChannels) {
					Channel *wc = qhChannels.value(wtc.iId);
					if (wc) {
						bool link = wtc.bLinks && ! wc->qhLinks.isEmpty();
						bool dochildren = wtc.bChildren && ! wc->qlChannels.isEmpty();
						bool group = ! wtc.qsGroup.isEmpty();
						if (!link && !dochildren && ! group) {
							// Common case
							if (ChanACL::hasPermission(u, wc, ChanACL::Whisper, acCache)) {
								foreach(p, wc->qlUsers) {
									channel.insert(static_cast<ServerUser *>(p));
								}
							}
						} else {
							QSet<Channel *> channels;
							if (link)
								channels = wc->allLinks();
							else
								channels.insert(wc);
							if (dochildren)
								channels.unite(wc->allChildren());
							const QString &redirect = u->qmWhisperRedirect.value(wtc.qsGroup);
							const QString &qsg = redirect.isEmpty() ? wtc.qsGroup : redirect;
							foreach(Channel *tc, channels) {
								if (ChanACL::hasPermission(u, tc, ChanACL::Whisper, acCache)) {
									foreach(p, tc->qlUsers) {
										ServerUser *su = static_cast<ServerUser *>(p);
										if (! group || Group::isMember(tc, tc, qsg, su)) {
											channel.insert(su);
										}
									}
								}
							}
						}
					}
				}
			}

			foreach(unsigned int id, wt.qlSessions) {
				ServerUser *pDst = qhUsers.value(id);
				if (pDst && ! channel.contains(pDst))
					direct.insert(pDst);
			}

			int uiSession = u->uiSession;
			qrwlUsers.unlock();
			qrwlUsers.lockForWrite();

			if (qhUsers.contains(uiSession))
				u->qmTargetCache.insert(target, ServerUser::TargetCache(channel, direct));
			qrwlUsers.unlock();
			qrwlUsers.lockForRead();
			if (! qhUsers.contains(uiSession))
				return;
		}
		if (! channel.isEmpty()) {
			buffer[0] = static_cast<char>(type | 1);
			foreach(ServerUser *pDst, channel) {
				SENDTO;
			}
			if (! direct.isEmpty()) {
				qba.clear();
				qba_npos.clear();
			}
		}
		if (! direct.isEmpty()) {
			buffer[0] = static_cast<char>(type | 2);
			foreach(ServerUser *pDst, direct) {
				SENDTO;
			}
		}
	}
}

void Server::log(ServerUser *u, const QString &str) {
	QString msg = QString("<%1:%2(%3)> %4").arg(QString::number(u->uiSession),
	              u->qsName,
	              QString::number(u->iId),
	              str);
	log(msg);
}

void Server::log(const QString &msg) {
	dblog(msg);
	qWarning("%d => %s", iServerNum, msg.toUtf8().constData());
}

void Server::newClient() {
	SslServer *ss = qobject_cast<SslServer *>(sender());
	if (! ss)
		return;
	forever {
		QSslSocket *sock = ss->nextPendingSSLConnection();
		if (! sock)
			return;

		QHostAddress adr = sock->peerAddress();

		if (meta->banCheck(adr)) {
			log(QString("Ignoring connection: %1 (Global ban)").arg(addressToString(sock->peerAddress(), sock->peerPort())));
			sock->disconnectFromHost();
			sock->deleteLater();
			return;
		}

		HostAddress ha(adr);

		QList<Ban> tmpBans = qlBans;
		foreach(const Ban &ban, qlBans) {
			if (ban.isExpired())
				tmpBans.removeOne(ban);
		}
		if (qlBans.count() != tmpBans.count()) {
			qlBans = tmpBans;
			saveBans();
		}

		foreach(const Ban &ban, qlBans) {
			if (ban.haAddress.match(ha, ban.iMask)) {
				log(QString("Ignoring connection: %1 (Server ban)").arg(addressToString(sock->peerAddress(), sock->peerPort())));
				sock->disconnectFromHost();
				sock->deleteLater();
				return;
			}
		}

		sock->setPrivateKey(qskKey);
		sock->setLocalCertificate(qscCert);
		sock->addCaCertificates(qlCA);

		if (qqIds.isEmpty()) {
			log(QString("Session ID pool (%1) empty, rejecting connection").arg(iMaxUsers));
			sock->disconnectFromHost();
			sock->deleteLater();
			return;
		}

		ServerUser *u = new ServerUser(this, sock);
		u->uiSession = qqIds.dequeue();
		u->haAddress = ha;

		{
			QWriteLocker wl(&qrwlUsers);
			qhUsers.insert(u->uiSession, u);
			qhHostUsers[ha].insert(u);
		}

		connect(u, SIGNAL(connectionClosed(QAbstractSocket::SocketError, const QString &)), this, SLOT(connectionClosed(QAbstractSocket::SocketError, const QString &)));
		connect(u, SIGNAL(message(unsigned int, const QByteArray &)), this, SLOT(message(unsigned int, const QByteArray &)));
		connect(u, SIGNAL(handleSslErrors(const QList<QSslError> &)), this, SLOT(sslError(const QList<QSslError> &)));
		connect(u, SIGNAL(encrypted()), this, SLOT(encrypted()));

		log(u, QString("New connection: %1").arg(addressToString(sock->peerAddress(), sock->peerPort())));

		u->setToS();

		sock->setProtocol(QSsl::TlsV1);
		sock->startServerEncryption();
	}
}

void Server::encrypted() {
	ServerUser *uSource = qobject_cast<ServerUser *>(sender());
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

	QList<QSslCertificate> certs = uSource->peerCertificateChain();
	if (!certs.isEmpty()) {
		const QSslCertificate &cert = certs.last();
		uSource->qslEmail = cert.alternateSubjectNames().values(QSsl::EmailEntry);
		uSource->qsHash = cert.digest(QCryptographicHash::Sha1).toHex();
		if (! uSource->qslEmail.isEmpty() && uSource->bVerified) {
			log(uSource, QString("Strong certificate for %1 <%2> (signed by %3)").arg(cert.subjectInfo(QSslCertificate::CommonName)).arg(uSource->qslEmail.join(", ")).arg(certs.first().issuerInfo(QSslCertificate::CommonName)));
		}

		foreach(const Ban &ban, qlBans) {
			if (ban.qsHash == uSource->qsHash) {
				log(uSource, QString("Certificate hash is banned."));
				uSource->disconnectSocket();
			}
		}
	}
}

void Server::sslError(const QList<QSslError> &errors) {
	ServerUser *u = qobject_cast<ServerUser *>(sender());
	bool ok = true;
	foreach(QSslError e, errors) {
		switch (e.error()) {
			case QSslError::InvalidPurpose:
				// Allow email certificates.
				break;
			case QSslError::NoPeerCertificate:
			case QSslError::SelfSignedCertificate:
			case QSslError::SelfSignedCertificateInChain:
			case QSslError::UnableToGetLocalIssuerCertificate:
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
	if (! u)
		return;

	if (ok)
		u->proceedAnyway();
	else
		u->disconnectSocket(true);
}

void Server::connectionClosed(QAbstractSocket::SocketError err, const QString &reason) {
	Connection *c = qobject_cast<Connection *>(sender());
	if (! c)
		return;
	if (c->bDisconnectedEmitted)
		return;
	c->bDisconnectedEmitted = true;

	ServerUser *u = static_cast<ServerUser *>(c);

	log(u, QString("Connection closed: %1 [%2]").arg(reason).arg(err));

	if (u->sState == ServerUser::Authenticated) {
		MumbleProto::UserRemove mpur;
		mpur.set_session(u->uiSession);
		sendExcept(u, mpur);

		emit userDisconnected(u);
	}

	Channel *old = u->cChannel;

	{
		QWriteLocker wl(&qrwlUsers);

		qhUsers.remove(u->uiSession);
		qhHostUsers[u->haAddress].remove(u);

		quint16 port = (u->saiUdpAddress.ss_family == AF_INET6) ? (reinterpret_cast<sockaddr_in6 *>(&u->saiUdpAddress)->sin6_port) : (reinterpret_cast<sockaddr_in *>(&u->saiUdpAddress)->sin_port);
		const QPair<HostAddress, quint16> &key = QPair<HostAddress, quint16>(u->haAddress, port);
		qhPeerUsers.remove(key);

		if (old)
			old->removeUser(u);
	}

	if (old && old->bTemporary && old->qlUsers.isEmpty())
		QCoreApplication::instance()->postEvent(this, new ExecEvent(boost::bind(&Server::removeChannel, this, old->iId)));

	qqIds.enqueue(u->uiSession); // Reinsert session id into pool

	if (u->sState == ServerUser::Authenticated) {
		clearTempGroups(u); // Also clears ACL cache
	}

	u->deleteLater();

	if (qhUsers.isEmpty())
		stopThread();
}

void Server::message(unsigned int uiType, const QByteArray &qbaMsg, ServerUser *u) {
	if (u == NULL) {
		u = static_cast<ServerUser *>(sender());
	}

	if (u->sState == ServerUser::Authenticated) {
		u->resetActivityTime();
	}

	if (uiType == MessageHandler::UDPTunnel) {
		int l = qbaMsg.size();
		if (l < 2)
			return;

		QReadLocker rl(&qrwlUsers);

		u->bUdp = false;

		const char *buffer = qbaMsg.constData();

		MessageHandler::UDPMessageType msgType = static_cast<MessageHandler::UDPMessageType>((buffer[0] >> 5) & 0x7);

		switch (msgType) {
			case MessageHandler::UDPVoiceCELTAlpha:
			case MessageHandler::UDPVoiceCELTBeta:
			case MessageHandler::UDPVoiceSpeex:
				processMsg(u, buffer, l);
				break;
			default:
				break;
		}

		return;
	}

#ifdef QT_NO_DEBUG
#define MUMBLE_MH_MSG(x) case MessageHandler:: x : { \
		MumbleProto:: x msg; \
		if (msg.ParseFromArray(qbaMsg.constData(), qbaMsg.size())) { \
			msg.DiscardUnknownFields(); \
			msg##x(u, msg); \
		} \
		break; \
	}
#else
#define MUMBLE_MH_MSG(x) case MessageHandler:: x : { \
		MumbleProto:: x msg; \
		if (msg.ParseFromArray(qbaMsg.constData(), qbaMsg.size())) { \
			if (uiType != MessageHandler::Ping) { \
				printf("== %s:\n", #x); \
				msg.PrintDebugString(); \
			} \
			msg.DiscardUnknownFields(); \
			msg##x(u, msg); \
		} \
		break; \
	}
#endif

	switch (uiType) {
			MUMBLE_MH_ALL
	}
}

void Server::checkTimeout() {
	QList<ServerUser *> qlClose;

	qrwlUsers.lockForRead();
	foreach(ServerUser *u, qhUsers) {
		if (u->activityTime() > (iTimeout * 1000)) {
			log(u, "Timeout");
			qlClose.append(u);
		}
	}
	qrwlUsers.unlock();
	foreach(ServerUser *u, qlClose)
		u->disconnectSocket(true);
}

void Server::tcpTransmitData(QByteArray a, unsigned int id) {
	Connection *c = qhUsers.value(id);
	if (c) {
		QByteArray qba;
		int len = a.size();

		qba.resize(len + 6);
		unsigned char *uc = reinterpret_cast<unsigned char *>(qba.data());
		* reinterpret_cast<quint16 *>(& uc[0]) = qToBigEndian(static_cast<quint16>(MessageHandler::UDPTunnel));
		* reinterpret_cast<quint32 *>(& uc[2]) = qToBigEndian(static_cast<quint32>(len));
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
	sendProtoExcept(NULL, msg, msgType, version);
}

void Server::sendProtoExcept(ServerUser *u, const ::google::protobuf::Message &msg, unsigned int msgType, unsigned int version) {
	QByteArray cache;
	foreach(ServerUser *usr, qhUsers)
		if ((usr != u) && (usr->sState == ServerUser::Authenticated))
			if ((version == 0) || (usr->uiVersion >= version) || ((version & 0x80000000) && (usr->uiVersion < (~version))))
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

	if (dest == NULL)
		dest = chan->cParent;

	chan->unlink(NULL);

	foreach(c, chan->qlChannels) {
		removeChannel(c, dest);
	}

	foreach(p, chan->qlUsers) {
		chan->removeUser(p);

		Channel *target = dest;
		while (target->cParent && ! hasPermission(static_cast<ServerUser *>(p), target, ChanACL::Enter))
			target = target->cParent;

		MumbleProto::UserState mpus;
		mpus.set_session(p->uiSession);
		mpus.set_channel_id(target->iId);
		userEnterChannel(p, target, mpus);
		sendAll(mpus);
		emit userStateChanged(p);
	}

	MumbleProto::ChannelRemove mpcr;
	mpcr.set_channel_id(chan->iId);
	sendAll(mpcr);

	removeChannelDB(chan);
	emit channelRemoved(chan);

	if (chan->cParent) {
		QWriteLocker wl(&qrwlUsers);
		chan->cParent->removeChannel(chan);
	}

	delete chan;
}

bool Server::unregisterUser(int id) {
	if (! unregisterUserDB(id))
		return false;

	{
		QMutexLocker lock(&qmCache);

		foreach(Channel *c, qhChannels) {
			bool write = false;
			QList<ChanACL *> ql = c->qlACL;

			foreach(ChanACL *acl, ql) {
				if (acl->iUserId == id) {
					c->qlACL.removeAll(acl);
					write = true;
				}
			}
			foreach(Group *g, c->qhGroups) {
				bool addrem = g->qsAdd.remove(id);
				bool remrem = g->qsRemove.remove(id);
				write = write || addrem || remrem;
			}
			if (write)
				updateChannel(c);
		}
	}

	foreach(ServerUser *u, qhUsers) {
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
		QWriteLocker wl(&qrwlUsers);
		c->addUser(p);
	}

	clearACLCache(p);

	setLastChannel(p);

	bool mayspeak = hasPermission(static_cast<ServerUser *>(p), c, ChanACL::Speak);
	bool sup = p->bSuppress;

	if (mayspeak == sup) {
		// Ok, he can speak and was suppressed, or vice versa
		p->bSuppress = ! mayspeak;
		mpus.set_suppress(p->bSuppress);
	}

	if (old && old->bTemporary && old->qlUsers.isEmpty()) {
		QCoreApplication::instance()->postEvent(this, new ExecEvent(boost::bind(&Server::removeChannel, this, old->iId)));
	}

	sendClientPermission(static_cast<ServerUser *>(p), c);
	if (c->cParent)
		sendClientPermission(static_cast<ServerUser *>(p), c->cParent);
}

bool Server::hasPermission(ServerUser *p, Channel *c, QFlags<ChanACL::Perm> perm) {
	QMutexLocker qml(&qmCache);
	return ChanACL::hasPermission(p, c, perm, acCache);
}

void Server::sendClientPermission(ServerUser *u, Channel *c, bool forceupdate) {
	unsigned int perm;

	if (u->iId == 0)
		return;

	{
		QMutexLocker qml(&qmCache);
		ChanACL::hasPermission(u, c, ChanACL::Enter, acCache);
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
	QMap<int, unsigned int>::const_iterator i;
	bool match = (u->qmPermissionSent.count() < 20);
	for (i = u->qmPermissionSent.constBegin(); (match && (i != u->qmPermissionSent.constEnd())); ++i) {
		Channel *c = qhChannels.value(i.key());
		if (! c) {
			match = false;
		} else {
			ChanACL::hasPermission(u, c, ChanACL::Enter, acCache);
			unsigned int perm = acCache.value(u)->value(c);
			if (perm != i.value())
				match = false;
		}
	}

	if (match)
		return;

	u->qmPermissionSent.clear();

	Channel *c = qhChannels.value(u->iLastPermissionCheck);
	if (! c) {
		c = u->cChannel;
		u->iLastPermissionCheck = c->iId;
	}

	ChanACL::hasPermission(u, c, ChanACL::Enter, acCache);
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

			flushClientPermissionCache(static_cast<ServerUser *>(p), mppq);
		} else {
			foreach(ChanACL::ChanCache *h, acCache)
				delete h;
			acCache.clear();

			foreach(ServerUser *u, qhUsers)
				if (u->sState == ServerUser::Authenticated)
					flushClientPermissionCache(u, mppq);
		}
	}

	{
		QWriteLocker lock(&qrwlUsers);

		foreach(ServerUser *u, qhUsers)
			u->qmTargetCache.clear();
	}
}

QString Server::addressToString(const QHostAddress &adr, unsigned short port) {
	HostAddress ha(adr);

	if ((Meta::mp.iObfuscate != 0) && adr.protocol() == QAbstractSocket::IPv4Protocol) {
		quint32 num = adr.toIPv4Address() ^ Meta::mp.iObfuscate;

		QHostAddress n(num);
		ha = HostAddress(n);
	}
	QString p = QString::number(port);
	return QString("%1:%2").arg(ha.toString(), p);
}

bool Server::validateUserName(const QString &name) {
	return (qrUserName.exactMatch(name) && (name.length() <= 512));
}

bool Server::validateChannelName(const QString &name) {
	return (qrChannelName.exactMatch(name) && (name.length() <= 512));
}

void Server::recheckCodecVersions() {
	QMap<int, int> qmCodecUsercount;
	QMap<int, int>::const_iterator i;
	int users = 0;

	// Count how many users use which codec
	foreach(ServerUser *u, qhUsers) {
		if (u->qlCodecs.isEmpty())
			continue;

		++users;
		foreach(int version, u->qlCodecs)
			++ qmCodecUsercount[version];
	}

	if (! users)
		return;

	// Find the best possible codec most users support
	int version = 0;
	int maximum_users = 0;
	i = qmCodecUsercount.constEnd();
	do {
		--i;
		if (i.value() > maximum_users) {
			version = i.key();
			maximum_users = i.value();
		}
	} while (i != qmCodecUsercount.constBegin());

	int current_version = bPreferAlpha ? iCodecAlpha : iCodecBeta;
	if (current_version == version)
		return;

	MumbleProto::CodecVersion mpcv;

	// If we don't already use the compat bitstream version set
	// it as alpha and announce it. If another codec now got the
	// majority set it as the opposite of the currently valid bPreferAlpha
	// and announce it.
	if (version == static_cast<qint32>(0x8000000b))
		bPreferAlpha = true;
	else
		bPreferAlpha = ! bPreferAlpha;

	if (bPreferAlpha)
		iCodecAlpha = version;
	else
		iCodecBeta = version;

	mpcv.set_alpha(iCodecAlpha);
	mpcv.set_beta(iCodecBeta);
	mpcv.set_prefer_alpha(bPreferAlpha);
	sendAll(mpcv);

	log(QString::fromLatin1("CELT codec switch %1 %2 (prefer %3)").arg(iCodecAlpha,0,16).arg(iCodecBeta,0,16).arg(bPreferAlpha ? iCodecAlpha : iCodecBeta,0,16));
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

	if (! bAllowHTML) {
		if (! text.contains(QLatin1Char('<'))) {
			text = text.simplified();
		} else {
			QXmlStreamReader qxsr(QString::fromLatin1("<document>%1</document>").arg(text));
			QString qs;
			while (! qxsr.atEnd()) {
				switch (qxsr.readNext()) {
					case QXmlStreamReader::Invalid:
						return false;
					case QXmlStreamReader::Characters:
						qs += qxsr.text();
						break;
					case QXmlStreamReader::EndElement:
						if ((qxsr.name() == QLatin1String("br")) || (qxsr.name() == QLatin1String("p")))
							qs += "\n";
						break;
					default:
						break;
				}
			}
			text = qs.simplified();
		}
		changed = true;
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
		if (! text.contains(QLatin1Char('<')))
			return false;

		QString qsOut;
		QXmlStreamReader qxsr(QString::fromLatin1("<document>%1</document>").arg(text));
		QXmlStreamWriter qxsw(&qsOut);
		while (! qxsr.atEnd()) {
			switch (qxsr.readNext()) {
				case QXmlStreamReader::Invalid:
					return false;
				case QXmlStreamReader::StartElement: {
						if (qxsr.name() == QLatin1String("img")) {
							QXmlStreamAttributes attr = qxsr.attributes();

							qxsw.writeStartElement(qxsr.namespaceUri().toString(), qxsr.name().toString());
							foreach(const QXmlStreamAttribute &a, qxsr.attributes())
								if (a.name() != QLatin1String("src"))
									qxsw.writeAttribute(a);
						} else {
							qxsw.writeCurrentToken(qxsr);
						}
					}
					break;
				default:
					qxsw.writeCurrentToken(qxsr);
					break;
			}
		}

		length = qsOut.length();

		return (length <= iMaxTextMessageLength);
	}
}
