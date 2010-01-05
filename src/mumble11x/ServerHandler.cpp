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

ServerHandlerMessageEvent::ServerHandlerMessageEvent(QByteArray &msg, bool flush) : QEvent(static_cast<QEvent::Type>(SERVERSEND_EVENT)) {
	qbaMsg = msg;
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

		if (! cConnection->csCrypt.decrypt(reinterpret_cast<const unsigned char *>(encrypted), reinterpret_cast<unsigned char *>(buffer), buflen)) {
			if (cConnection->csCrypt.tLastGood.elapsed() > 5000000ULL) {
				if (cConnection->csCrypt.tLastRequest.elapsed() > 5000000ULL) {
					cConnection->csCrypt.tLastRequest.restart();
					MessageCryptSync mcs;
					sendMessage(&mcs);
				}
			}
			continue;
		}

		PacketDataStream pds(buffer, buflen-4);

		quint32 msgType, uiSession;
		pds >> msgType >> uiSession;

		if (msgType == Message::Ping) {
			quint64 t;
			pds >> t;
			Connection::updatePing(cConnection->dUDPPingAvg, cConnection->dUDPPingVar, cConnection->uiUDPPackets, tTimestamp.elapsed() - t);
		} else if (msgType == Message::Speex) {
			ClientPlayer *p = ClientPlayer::get(uiSession);
			AudioOutputPtr ao = g.ao;
			if (ao) {
				if (p) {
					if (! p->bLocalMute) {
						unsigned int iSeq;
						pds >> iSeq;
						QByteArray qbaSpeexPacket(pds.dataBlock(pds.left()));
						ao->addFrameToBuffer(p, qbaSpeexPacket, iSeq);
					}
				} else {
					ao->removeBuffer(p);
				}
			}
		}
	}
}

void ServerHandler::sendMessage(Message *mMsg) {
	bool mayUdp = (mMsg->messageType() == Message::Speex) || (mMsg->messageType() == Message::Ping);
	mMsg->uiSession = g.uiSession;

	if (mayUdp && !NetworkConfig::TcpModeEnabled()) {
		QMutexLocker qml(&qmUdp);
		if (! qusUdp)
			return;
		if (! cConnection->csCrypt.isValid())
			return;
		unsigned char buffer[65536];
		unsigned char crypto[65540];
		PacketDataStream pds(buffer, 65536);
		mMsg->messageToNetwork(pds);
		cConnection->csCrypt.encrypt(buffer, crypto, pds.size());
		qusUdp->writeDatagram(reinterpret_cast<const char *>(crypto), pds.size() + 4, qhaRemote, usPort);
	} else {
		QByteArray qbaBuffer;
		mMsg->messageToNetwork(qbaBuffer);

		ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaBuffer, mayUdp);
		QApplication::postEvent(this, shme);
	}
}

void ServerHandler::run() {
	QSslSocket *qtsSock = new QSslSocket(this);
	cConnection = ConnectionPtr(new Connection(this, qtsSock));

	qlErrors.clear();
	qscCert.clear();

	connect(qtsSock, SIGNAL(encrypted()), this, SLOT(serverConnectionConnected()));
	connect(cConnection.get(), SIGNAL(connectionClosed(QString)), this, SLOT(serverConnectionClosed(QString)));
	connect(cConnection.get(), SIGNAL(message(QByteArray &)), this, SLOT(message(QByteArray &)));
	connect(cConnection.get(), SIGNAL(handleSslErrors(const QList<QSslError> &)), this, SLOT(setSslErrors(const QList<QSslError> &)));
	qtsSock->connectToHostEncrypted(qsHostName, usPort);

	QTimer *ticker = new QTimer(this);
	connect(ticker, SIGNAL(timeout()), this, SLOT(sendPing()));
	ticker->start(5000);

	g.mw->rtLast = MessageServerReject::None;

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
	MessagePingStats mps;
	mps.uiTimestamp = tTimestamp.elapsed();
	mps.uiGood = cs.uiGood;
	mps.uiLate = cs.uiLate;
	mps.uiLost = cs.uiLost;
	mps.uiResync = cs.uiResync;
	mps.dUDPPingAvg = cConnection->dUDPPingAvg;
	mps.dUDPPingVar = cConnection->dUDPPingVar;
	mps.dTCPPingAvg = cConnection->dTCPPingAvg;
	mps.dTCPPingVar = cConnection->dTCPPingVar;
	sendMessage(&mps);

	if (!NetworkConfig::TcpModeEnabled()) {
		MessagePing mp;
		mp.uiTimestamp = mps.uiTimestamp;
		sendMessage(&mp);
	}
}

void ServerHandler::message(QByteArray &qbaMsg) {
	Message *mMsg = Message::networkToMessage(qbaMsg);
	if (! mMsg)
		return;

	ClientPlayer *p = ClientPlayer::get(mMsg->uiSession);
	AudioOutputPtr ao = g.ao;

	if (mMsg->messageType() == Message::Speex) {
		if (ao) {
			if (p) {
				MessageSpeex *msMsg=static_cast<MessageSpeex *>(mMsg);
				if (! p->bLocalMute)
					ao->addFrameToBuffer(p, msMsg->qbaSpeexPacket, msMsg->iSeq);
			} else {
				// Eek, we just got a late packet for a player already removed. Remove
				// the buffer and pretend this never happened.
				// If ~AudioOutputPlayer or decendants uses the Player object now,
				// Bad Things happen.
				ao->removeBuffer(p);
			}
		}
	} else if (mMsg->messageType() == Message::PingStats) {
		MessagePingStats *mpsMsg = static_cast<MessagePingStats *>(mMsg);
		CryptState &cs = cConnection->csCrypt;
		cs.uiRemoteGood = mpsMsg->uiGood;
		cs.uiRemoteLate = mpsMsg->uiLate;
		cs.uiRemoteLost = mpsMsg->uiLost;
		cs.uiRemoteResync = mpsMsg->uiResync;
		Connection::updatePing(cConnection->dTCPPingAvg, cConnection->dTCPPingVar, cConnection->uiTCPPackets, tTimestamp.elapsed() - mpsMsg->uiTimestamp);
	} else {
		if (mMsg->messageType() == Message::ServerLeave) {
			if (ao)
				ao->removeBuffer(p);
		}
		ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaMsg, false);
		QApplication::postEvent(g.mw, shme);
	}

	delete mMsg;
}

void ServerHandler::disconnect() {
	// Actual TCP object is in a different thread, so signal it
	QByteArray qbaBuffer;
	ServerHandlerMessageEvent *shme=new ServerHandlerMessageEvent(qbaBuffer, false);
	QApplication::postEvent(this, shme);
}

void ServerHandler::serverConnectionClosed(QString reason) {
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

	MessageServerAuthenticate msaMsg;
	msaMsg.qsUsername = qsUserName;
	msaMsg.qsPassword = qsPassword;

	cConnection->sendMessage(&msaMsg);

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
