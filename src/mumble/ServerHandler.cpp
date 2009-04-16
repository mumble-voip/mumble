/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include "ServerHandler.h"
#include "MainWindow.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "Message.h"
#include "Player.h"
#include "Connection.h"
#include "Global.h"
#include "Database.h"
#include "PacketDataStream.h"
#include "NetworkConfig.h"
#include "OSInfo.h"

ServerHandlerMessageEvent::ServerHandlerMessageEvent(const QByteArray &msg, unsigned int type, bool flush) : QEvent(static_cast<QEvent::Type>(SERVERSEND_EVENT)) {
	qbaMsg = msg;
	uiType = type;
	bFlush = flush;
}

ServerHandler::ServerHandler() {
	cConnection.reset();
	qusUdp = NULL;

	// For some strange reason, on Win32, we have to call supportsSsl before the cipher list is ready.
	qWarning("OpenSSL Support: %d", QSslSocket::supportsSsl());

	QList<QSslCipher> pref;
	foreach(QSslCipher c, QSslSocket::defaultCiphers()) {
		if (c.usedBits() < 128)
			continue;
		pref << c;
	}
	if (pref.isEmpty())
		qFatal("No ciphers of at least 128 bit found");
	QSslSocket::setDefaultCiphers(pref);

#ifdef Q_OS_WIN
	QOS_VERSION qvVer;
	qvVer.MajorVersion = 1;
	qvVer.MinorVersion = 0;

	hQoS = NULL;

	HMODULE hLib = LoadLibrary(L"qWave.dll");
	if (hLib == NULL) {
		qWarning("ServerHandler: Failed to load qWave.dll, no QoS available");
	} else {
		FreeLibrary(hLib);
		if (! QOSCreateHandle(&qvVer, &hQoS))
			qWarning("ServerHandler: Failed to create QOS2 handle");
		else
			Connection::setQoS(hQoS);
	}
#endif
}

ServerHandler::~ServerHandler() {
	wait();
	cConnection.reset();
#ifdef Q_OS_WIN
	if (hQoS) {
		QOSCloseHandle(hQoS);
		Connection::setQoS(NULL);
	}
#endif
}

void ServerHandler::customEvent(QEvent *evt) {
	if (evt->type() != SERVERSEND_EVENT)
		return;

	ServerHandlerMessageEvent *shme=static_cast<ServerHandlerMessageEvent *>(evt);

	if (cConnection) {
		if (shme->qbaMsg.size() > 0) {
			cConnection->sendMessage(shme->qbaMsg);
			if (shme->bFlush)
				cConnection->forceFlush();
		} else {
			exit(0);
		}
	}
}

void ServerHandler::udpReady() {
	while (qusUdp->hasPendingDatagrams()) {
		char encrypted[65536];
		char buffer[65535];
		unsigned int buflen = static_cast<unsigned int>(qusUdp->pendingDatagramSize());
		QHostAddress senderAddr;
		quint16 senderPort;
		qusUdp->readDatagram(encrypted, qMin(65536U, buflen), &senderAddr, &senderPort);

		if (!(senderAddr == qhaRemote) || (senderPort != usPort))
			continue;

		if (! cConnection)
			continue;

		if (! cConnection->csCrypt.isValid())
			continue;

		if (buflen < 5)
			continue;

		if (! cConnection->csCrypt.decrypt(reinterpret_cast<const unsigned char *>(encrypted), reinterpret_cast<unsigned char *>(buffer), buflen)) {
			if (cConnection->csCrypt.tLastGood.elapsed() > 5000000ULL) {
				if (cConnection->csCrypt.tLastRequest.elapsed() > 5000000ULL) {
					cConnection->csCrypt.tLastRequest.restart();
					MumbleProto::CryptSetup mpcs;
					sendMessage(mpcs);
				}
			}
			continue;
		}

		PacketDataStream pds(buffer + 1, buflen-5);

		unsigned int msgType = (buffer[0] >> 5) & 0x7;
		unsigned int msgFlags = buffer[0] & 0x1f;


		if (msgType == MessageHandler::UDPPing) {
			quint64 t;
			pds >> t;
			accUDP((tTimestamp.elapsed() - t) / 1000.0);
		} else if (msgType == MessageHandler::UDPVoice) {
			handleVoicePacket(msgFlags, pds);
		}
	}
}

void ServerHandler::handleVoicePacket(unsigned int msgFlags, PacketDataStream &pds) {
	unsigned int uiSession;
	pds >> uiSession;
	ClientPlayer *p = ClientPlayer::get(uiSession);
	AudioOutputPtr ao = g.ao;
	if (ao && p && ! p->bLocalMute) {
		unsigned int iSeq;
		pds >> iSeq;
		ao->addFrameToBuffer(p, pds.dataBlock(pds.left()), iSeq);
	}
}

void ServerHandler::sendMessage(const char *data, int len) {
	STACKVAR(unsigned char, crypto, len+4);

	QMutexLocker qml(&qmUdp);

	if (! qusUdp)
		return;
	if (! cConnection->csCrypt.isValid())
		return;

	if (NetworkConfig::TcpModeEnabled()) {
		QByteArray qba;

		qba.resize(len + 4);
		unsigned char *uc = reinterpret_cast<unsigned char *>(qba.data());
		uc[0] = MessageHandler::UDPTunnel;
		uc[1] = (len >> 16) & 0xFF;
		uc[2] = (len >> 8) & 0xFF;
		uc[3] = len & 0xFF;
		memcpy(uc + 4, data, len);

		QApplication::postEvent(this, new ServerHandlerMessageEvent(qba, MessageHandler::UDPTunnel, true));
	} else {
		cConnection->csCrypt.encrypt(reinterpret_cast<const unsigned char *>(data), crypto, len);
		qusUdp->writeDatagram(reinterpret_cast<const char *>(crypto), len + 4, qhaRemote, usPort);
	}
}

void ServerHandler::sendProtoMessage(const ::google::protobuf::Message &msg, unsigned int msgType) {
	QByteArray qba;

	if (QThread::currentThread() != thread()) {
		Connection::messageToNetwork(msg, msgType, qba);
		ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qba, 0, false);
		QApplication::postEvent(this, shme);
	} else {
		cConnection->sendMessage(msg, msgType, qba);
	}
}

void ServerHandler::run() {
	QSslSocket *qtsSock = new QSslSocket(this);
	cConnection = ConnectionPtr(new Connection(this, qtsSock));

	qlErrors.clear();
	qscCert.clear();

	connect(qtsSock, SIGNAL(encrypted()), this, SLOT(serverConnectionConnected()));
	connect(cConnection.get(), SIGNAL(connectionClosed(const QString &)), this, SLOT(serverConnectionClosed(const QString &)));
	connect(cConnection.get(), SIGNAL(message(unsigned int, const QByteArray &)), this, SLOT(message(unsigned int, const QByteArray &)));
	connect(cConnection.get(), SIGNAL(handleSslErrors(const QList<QSslError> &)), this, SLOT(setSslErrors(const QList<QSslError> &)));
	qtsSock->connectToHostEncrypted(qsHostName, usPort);

	QTimer *ticker = new QTimer(this);
	connect(ticker, SIGNAL(timeout()), this, SLOT(sendPing()));
	ticker->start(5000);

	g.mw->rtLast = MumbleProto::Reject_RejectType_None;

	accUDP = accTCP = accClean;

	uiVersion = 0;
	qsRelease = QString();
	qsOS = QString();
	qsOSVersion = QString();

	MumbleProto::Version mpv;
	mpv.set_release(u8(QLatin1String(MUMBLE_RELEASE)));

	QRegExp rx(QLatin1String("(\\d+)\\.(\\d+)\\.(\\d+)"));
	if (rx.exactMatch(QLatin1String(MUMTEXT(MUMBLE_VERSION_STRING)))) {
			int major = rx.cap(1).toInt();
			int minor = rx.cap(2).toInt();
			int patch = rx.cap(3).toInt();
			mpv.set_version((major << 16) | (minor << 8) | patch);
	}
	mpv.set_os(u8(OSInfo::getOS()));
	mpv.set_os_version(u8(OSInfo::getOSVersion()));
	sendMessage(mpv);

	exec();

	if (qusUdp) {
		QMutexLocker qml(&qmUdp);

#ifdef Q_OS_WIN
		if (hQoS != NULL) {
			if (! QOSRemoveSocketFromFlow(hQoS, 0, dwFlowUDP, 0))
				qWarning("ServerHandler: Failed to remove UDP from QoS");
			dwFlowUDP = 0;
		}
#endif
		delete qusUdp;
		qusUdp = NULL;
	}

	ticker->stop();
	cConnection->disconnectSocket(true);
	cConnection.reset();
}

void ServerHandler::setSslErrors(const QList<QSslError> &errors) {
	qscCert = cConnection->peerCertificateChain();
	if ((qscCert.size() > 0)  && (QString::fromLatin1(qscCert.at(0).digest(QCryptographicHash::Sha1).toHex()) == Database::getDigest(qsHostName, usPort)))
		cConnection->proceedAnyway();
	else
		qlErrors = errors;
}

void ServerHandler::sendPing() {
	CryptState &cs = cConnection->csCrypt;

	quint64 t = tTimestamp.elapsed();

	if (qusUdp) {
		unsigned char buffer[256];
		PacketDataStream pds(buffer + 1, 255);
		buffer[0] = MessageHandler::UDPPing << 5;
		pds << t;
		sendMessage(reinterpret_cast<const char *>(buffer), pds.size() + 1);
	}

	MumbleProto::Ping mpp;

	mpp.set_timestamp(t);
	mpp.set_good(cs.uiGood);
	mpp.set_late(cs.uiLate);
	mpp.set_lost(cs.uiLost);
	mpp.set_resync(cs.uiResync);

	mpp.set_udp_ping_avg(boost::accumulators::mean(accUDP));
	mpp.set_udp_ping_var(boost::accumulators::variance(accUDP));
	mpp.set_udp_packets(boost::accumulators::count(accUDP));
	mpp.set_tcp_ping_avg(boost::accumulators::mean(accTCP));
	mpp.set_tcp_ping_var(boost::accumulators::variance(accTCP));
	mpp.set_tcp_packets(boost::accumulators::count(accTCP));
	sendMessage(mpp);
}

void ServerHandler::message(unsigned int msgType, const QByteArray &qbaMsg) {
	const char *ptr = qbaMsg.constData();
	if (msgType == MessageHandler::UDPTunnel) {
		if (qbaMsg.length() < 1)
			return;

		unsigned int msgType = (ptr[0] >> 5) & 0x7;
		unsigned int msgFlags = ptr[0] & 0x1f;
		PacketDataStream pds(qbaMsg.constData() + 1, qbaMsg.size());

		if (msgType == MessageHandler::UDPVoice) {
			handleVoicePacket(msgFlags, pds);
		}
	} else if (msgType == MessageHandler::Ping) {
		MumbleProto::Ping msg;
		if (msg.ParseFromArray(qbaMsg.constData(), qbaMsg.size())) {
			CryptState &cs = cConnection->csCrypt;
			cs.uiRemoteGood = msg.good();
			cs.uiRemoteLate = msg.late();
			cs.uiRemoteLost = msg.lost();
			cs.uiRemoteResync = msg.resync();
			accTCP((g.sh->tTimestamp.elapsed() - msg.timestamp()) / 1000.0);
		}
	} else {
		ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaMsg, msgType, false);
		QApplication::postEvent(g.mw, shme);
	}
}

void ServerHandler::disconnect() {
	// Actual TCP object is in a different thread, so signal it
	QByteArray qbaBuffer;
	ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaBuffer, 0, false);
	QApplication::postEvent(this, shme);
}

void ServerHandler::serverConnectionClosed(const QString &reason) {
	AudioOutputPtr ao = g.ao;
	if (ao)
		ao->wipe();
	emit disconnected(reason);
	exit(0);
}

void ServerHandler::serverConnectionConnected() {
	qscCert = cConnection->peerCertificateChain();
	qscCipher = cConnection->sessionCipher();

	cConnection->setToS();

	MumbleProto::Authenticate mpa;
	mpa.set_username(u8(qsUserName));
	mpa.set_password(u8(qsPassword));

	sendMessage(mpa);

	{
		QMutexLocker qml(&qmUdp);

		qusUdp = new QUdpSocket(this);
		qusUdp->bind();
		connect(qusUdp, SIGNAL(readyRead()), this, SLOT(udpReady()));

		qhaRemote = cConnection->peerAddress();

#if defined(Q_OS_UNIX)
		int val = 0xe0;
		if (setsockopt(qusUdp->socketDescriptor(), IPPROTO_IP, IP_TOS, &val, sizeof(val)))
			qWarning("ServerHandler: Failed to set TOS for UDP Socket");
#elif defined(Q_OS_WIN)
		if (hQoS != NULL) {
			struct sockaddr_in addr;
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(usPort);
			addr.sin_addr.s_addr = htonl(qhaRemote.toIPv4Address());

			dwFlowUDP = 0;
			if (! QOSAddSocketToFlow(hQoS, qusUdp->socketDescriptor(), reinterpret_cast<sockaddr *>(&addr), QOSTrafficTypeVoice, QOS_NON_ADAPTIVE_FLOW, &dwFlowUDP))
				qWarning("ServerHandler: Failed to add UDP to QOS");
		}
#endif
	}

	emit connected();
}

void ServerHandler::setConnectionInfo(const QString &host, unsigned short port, const QString &username, const QString &pw) {
	qsHostName = host;
	usPort = port;
	qsUserName = username;
	qsPassword = pw;
}

void ServerHandler::getConnectionInfo(QString &host, unsigned short &port, QString &username, QString &pw) {
	host = qsHostName;
	port = usPort;
	username = qsUserName;
	pw = qsPassword;
}
