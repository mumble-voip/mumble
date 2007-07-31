/* Copyright (C) 2005-2007, Thorvald Natvig <thorvald@natvig.com>

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

#include "Player.h"
#include "Channel.h"
#include "ACL.h"
#include "Group.h"
#include "Message.h"
#include "ServerDB.h"
#include "Connection.h"
#include "Server.h"
#include "DBus.h"
#include "PacketDataStream.h"
#include "Cert.h"

#ifdef Q_OS_UNIX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#endif

ServerParams g_sp;

uint qHash(const Peer &p) {
	return p.first ^ p.second;
}

LogEmitter::LogEmitter(QObject *p) : QObject(p) {
};

void LogEmitter::addLogEntry(const QString &msg) {
	emit newLogEntry(msg);
};

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

User::User(Server *parent, QSslSocket *socket) : Connection(parent, socket), Player() {
	uiAddress = 0;
	usPort = 0;
}

ServerParams::ServerParams() {
	qsPassword = QString();
	iPort = 64738;
	iTimeout = 30;
	iMaxBandwidth = 10000;
	iMaxUsers = 1000;
	qsWelcomeText = QString("Welcome to this server");
	qsDatabase = QString();
	iDBPort = 0;
	qsDBDriver = "QSQLITE";
	qsLogfile = "murmur.log";
	qsSSLStore = "murmur.pem";
	qhaBind = QHostAddress(QHostAddress::Any);
}

void ServerParams::read(QString fname) {
	if (fname.isEmpty())
		fname = "murmur.ini";
	else {
		if (! QFile(fname).exists())
			qFatal("Specified ini file %s could not be opened", qPrintable(fname));
	}
	QSettings qs(fname, QSettings::IniFormat);

	qDebug("Initializing settings from %s", qPrintable(qs.fileName()));

	QString qsHost = qs.value("host", QString()).toString();
	if (! qsHost.isEmpty()) {
		if (! qhaBind.setAddress(qsHost)) {
			QHostInfo hi = QHostInfo::fromName(qsHost);
			foreach(QHostAddress qha, hi.addresses()) {
				if (qha.protocol() == QAbstractSocket::IPv4Protocol) {
					qhaBind = qha;
					break;
				}
			}
			if ((qhaBind == QHostAddress::Any) || (qhaBind.isNull())) {
				qFatal("Lookup of bind hostname %s failed", qPrintable(qsHost));
			}

		}
		qDebug("Binding to address %s", qPrintable(qhaBind.toString()));
	}

	qsPassword = qs.value("serverpassword", qsPassword).toString();
	iPort = qs.value("port", iPort).toInt();
	iTimeout = qs.value("timeout", iTimeout).toInt();
	iMaxBandwidth = qs.value("bandwidth", iMaxBandwidth).toInt();
	iMaxUsers = qs.value("users", iMaxUsers).toInt();
	qsWelcomeText = qs.value("welcometext", qsWelcomeText).toString();

	qsDatabase = qs.value("database", qsDatabase).toString();

	qsDBDriver = qs.value("dbDriver", qsDBDriver).toString();
	qsDBUserName = qs.value("dbUsername", qsDBUserName).toString();
	qsDBPassword = qs.value("dbPassword", qsDBPassword).toString();
	qsDBHostName = qs.value("dbHost", qsDBHostName).toString();
	qsDBPrefix = qs.value("dbPrefix", qsDBPrefix).toString();
	iDBPort = qs.value("dbPort", iDBPort).toInt();

	qsDBus = qs.value("dbus", qsDBus).toString();
	qsLogfile = qs.value("logfile", qsLogfile).toString();

	qsRegName = qs.value("registerName", qsRegName).toString();
	qsRegPassword = qs.value("registerPassword", qsRegPassword).toString();
	qsRegHost = qs.value("registerHostname", qsRegHost).toString();
	qurlRegWeb = QUrl(qs.value("registerUrl", qurlRegWeb.toString()).toString());

	qsSSLCert = qs.value("sslCert", qsSSLCert).toString();
	qsSSLKey = qs.value("sslKey", qsSSLKey).toString();
	qsSSLStore = qs.value("sslStore", qsSSLStore).toString();
}

BandwidthRecord::BandwidthRecord() {
	iRecNum = 0;
	iSum = 0;
	for (int i=0;i<N_BANDWIDTH_SLOTS;i++)
		a_iBW[i] = 0;
}

void BandwidthRecord::addFrame(int size) {
	iSum -= a_iBW[iRecNum];
	a_iBW[iRecNum] = size;
	iSum += a_iBW[iRecNum];

	a_qtWhen[iRecNum].restart();

	iRecNum++;
	if (iRecNum == N_BANDWIDTH_SLOTS)
		iRecNum = 0;
}

int BandwidthRecord::bytesPerSec() {
	// Multiply by 45; give 10% leniency
	quint64 elapsed = a_qtWhen[iRecNum].elapsed();
	return (iSum * 1000000LL) / elapsed;
}

void Server::run() {
	qDebug("Starting UDP Thread");
	qusUdp = new QUdpSocket();
	if (! qusUdp->bind(g_sp.qhaBind, g_sp.iPort, QUdpSocket::DontShareAddress))
		qFatal("Server: UDP Bind to port %d failed",g_sp.iPort);

#ifdef Q_OS_UNIX
	int val = IPTOS_PREC_FLASHOVERRIDE | IPTOS_LOWDELAY | IPTOS_THROUGHPUT;
	if (setsockopt(qusUdp->socketDescriptor(), SOL_IP, IP_TOS, &val, sizeof(val)))
		qWarning("Server: Failed to set TOS for UDP Socket");
#endif
	connect(this, SIGNAL(tcpTransmit(QByteArray, unsigned int)), this, SLOT(tcpTransmitData(QByteArray, unsigned int)), Qt::QueuedConnection);

	QHostAddress senderAddr;
	quint16 senderPort;
	qint32 len;
	char buffer[65535];

	quint32 msgType;
	int uiSession;

	qDebug("Entering UDP event loop");

	while (qusUdp->waitForReadyRead(-1)) {
		QReadLocker rl(&qrwlUsers);
		while (qusUdp->hasPendingDatagrams()) {

			len=qusUdp->readDatagram(buffer, 65535, &senderAddr, &senderPort);

			PacketDataStream pds(buffer, len);
			pds >> msgType >> uiSession;

			if ((msgType != Message::Speex) && (msgType != Message::Ping))
				continue;
			if (! pds.isValid())
				continue;

			Peer p(senderAddr.toIPv4Address(), senderPort);

			if (p != qhPeers.value(uiSession)) {
				Connection *source = qhUsers.value(uiSession);
				if (! source || !(source->peerAddress() == senderAddr)) {
					continue;
				}
				// At any point after this, the connection might go away WHILE we're processing. That is "bad".
				rl.unlock();
				{
					QWriteLocker wl(&qrwlUsers);
					if (qhUsers.contains(uiSession)) {
						qhHosts[uiSession] = senderAddr;
						qhPeers[uiSession] = p;
					}
				}
				rl.relock();
				if (! qhUsers.contains(uiSession)) {
					continue;
				}
			}

			if (msgType == Message::Ping)
				qusUdp->writeDatagram(buffer, len, senderAddr, senderPort);
			else {
				processMsg(pds, qhUsers.value(uiSession));
			}
		}
	}
}

void Server::fakeUdpPacket(Message *msg, Connection *source) {
	char buffer[65535];
	PacketDataStream pds(buffer, 65535);
	msg->messageToNetwork(pds);
	pds.rewind();

	quint32 msgType;
	int uiSession;

	pds >> msgType >> uiSession;

	QReadLocker rl(&qrwlUsers);
	processMsg(pds, source);
}

void Server::sendMessage(unsigned int id, const char *data, int len, QByteArray &cache) {
	if (qhPeers.contains(id)) {
		qusUdp->writeDatagram(data, len, qhHosts[id], qhPeers[id].second);
	} else {
		if (cache.isEmpty())
			cache = QByteArray(data, len);
		emit tcpTransmit(cache,id);
	}
}

void Server::processMsg(PacketDataStream &pds, Connection *cCon) {
	User *u = static_cast<User *>(cCon);
	if (! u || (u->sState != Player::Authenticated))
		return;

	Player *p;
	int seq, flags;

	if (u->bMute || u->bSuppressed)
		return;

	BandwidthRecord *bw = & u->bwr;

	pds >> seq;
	pds >> flags;

	int nframes = ((flags >> 4) & 0x03) + 1;
	int packetsize = (20 + 8 + 3 + pds.left() + nframes - 1) / nframes;
	for (int i = 0; i < nframes; i++)
		bw->addFrame(packetsize);

	if (bw->bytesPerSec() > g_sp.iMaxBandwidth) {
		// Suppress packet.
		return;
	}

	Channel *c = u->cChannel;

	QByteArray qba;

	pds.rewind();
	const char *data = pds.charPtr();
	int len = pds.left();

	if (flags & MessageSpeex::LoopBack) {
		sendMessage(u->uiSession, data, len, qba);
		return;
	}

	foreach(p, c->qlPlayers) {
		if (! p->bDeaf && ! p->bSelfDeaf && (p != static_cast<Player *>(u)))
			sendMessage(p->uiSession, data, len, qba);
	}

	if (! c->qhLinks.isEmpty()) {
		QSet<Channel *> chans = c->allLinks();
		chans.remove(c);

		foreach(Channel *l, chans) {
			if (ChanACL::hasPermission(u, l, (flags & MessageSpeex::AltSpeak) ? ChanACL::AltSpeak : ChanACL::Speak)) {
				foreach(p, l->qlPlayers) {
					if (! p->bDeaf && ! p->bSelfDeaf)
						sendMessage(p->uiSession, data, len, qba);
				}
			}
		}
	}
}


Server::Server(QObject *p) : QThread(p) {
	qtsServer = new SslServer(this);

	connect(qtsServer, SIGNAL(newConnection()), this, SLOT(newClient()), Qt::QueuedConnection);

	if (! qtsServer->listen(g_sp.qhaBind, g_sp.iPort))
		qFatal("Server: TCP Listen on port %d failed",g_sp.iPort);

	log(QString("Server listening on port %1").arg(g_sp.iPort));

	for (int i=1;i<5000;i++)
		qqIds.enqueue(i);

	qtTimeout = new QTimer(this);
	connect(qtTimeout, SIGNAL(timeout()), this, SLOT(checkTimeout()));
	qtTimeout->start(5500);

	qlBans = getBans();
}

void Server::log(QString s, Connection *c) {
	if (c) {
		User *u = static_cast<User *>(c);

		int id = 0;
		int iid = -1;
		QString name;
		id = u->uiSession;
		iid = u->iId;
		name = u->qsName;
		qWarning("<%d:%s(%d)> %s", id, qPrintable(name), iid, qPrintable(s));
	} else {
		qWarning("%s", qPrintable(s));
	}
}

void Server::newClient() {
	forever {
		QSslSocket *sock = qtsServer->nextPendingSSLConnection();
		if (! sock)
			return;

		QHostAddress adr = sock->peerAddress();
		quint32 base = adr.toIPv4Address();

		QPair<quint32,int> ban;

		foreach(ban, qlBans) {
			int mask = 32 - ban.second;
			mask = (1 << mask) - 1;
			if ((base & ~mask) == (ban.first & ~mask)) {
				log(QString("Ignoring connection: %1:%2").arg(sock->peerAddress().toString()).arg(sock->peerPort()));
				sock->disconnectFromHost();
				return;
			}
		}

		sock->setPrivateKey(cert.getKey());
		sock->setLocalCertificate(cert.getCert());

		if (qqIds.isEmpty()) {
			sock->disconnectFromHost();
			return;
		}

		User *u = new User(this, sock);
		u->uiSession = qqIds.dequeue();

		{
			QWriteLocker wl(&qrwlUsers);
			qhUsers.insert(u->uiSession, u);
		}

		connect(u, SIGNAL(connectionClosed(QString)), this, SLOT(connectionClosed(QString)));
		connect(u, SIGNAL(message(QByteArray &)), this, SLOT(message(QByteArray &)));
		connect(u, SIGNAL(handleSslErrors(const QList<QSslError> &)), this, SLOT(sslError(const QList<QSslError> &)));

		log(QString("New connection: %1:%2").arg(sock->peerAddress().toString()).arg(sock->peerPort()), u);

		sock->startServerEncryption();
	}
}

void Server::sslError(const QList<QSslError> &errors) {
	bool ok = true;
	foreach(QSslError e, errors) {
		switch (e.error()) {
			case QSslError::NoPeerCertificate:
				break;
			default:
				ok = false;
		}
	}
	if (ok) {
		Connection *c = dynamic_cast<User *>(sender());
		c->proceedAnyway();
	}
}

void Server::connectionClosed(QString reason) {
	Connection *c = dynamic_cast<Connection *>(sender());
	User *u = static_cast<User *>(c);

	log(QString("Connection closed: %1").arg(reason), c);

	if (u->sState == Player::Authenticated) {
		MessageServerLeave mslMsg;
		mslMsg.uiSession=u->uiSession;
		sendExcept(&mslMsg, c);

		dbus->playerDisconnected(u);
	}

	QWriteLocker wl(&qrwlUsers);

	qhUsers.remove(u->uiSession);
	if (u->cChannel)
		u->cChannel->removePlayer(u);

	qqIds.enqueue(u->uiSession);
	qhUserTextureCache.remove(u->iId);

	u->deleteLater();
}

void Server::message(QByteArray &qbaMsg, Connection *cCon) {
	if (cCon == NULL) {
		cCon = static_cast<Connection *>(sender());
	}
	Message *mMsg = Message::networkToMessage(qbaMsg);

	// Just leftovers from the buffer and we just kicked
	// the user off.
	// FIXME :: We don't have anything like this now.
	//if (cCon->isDeleted())
	//	return;

	if (mMsg) {
		dispatch(cCon, mMsg);
		delete mMsg;
	} else {
		cCon->disconnectSocket();
	}
}


void Server::checkTimeout() {
	QList<User *> qlClose;

	qrwlUsers.lockForRead();
	foreach(User *u, qhUsers) {
		if (u->activityTime() > (g_sp.iTimeout * 1000)) {
			log(QLatin1String("Timeout"), u);
			qlClose.append(u);
		}
	}
	qrwlUsers.unlock();
	foreach(User *u, qlClose)
	u->disconnectSocket();
}

void Server::tcpTransmitData(QByteArray a, unsigned int id) {
	Connection *c = qhUsers.value(id);
	if (c) {
		c->sendMessage(a);
		c->forceFlush();
	}
}

void Server::sendMessage(unsigned int id, Message *mMsg) {
	Connection *c = qhUsers.value(id);
	sendMessage(c, mMsg);
}

void Server::sendMessage(Connection *c, Message *mMsg) {
	c->sendMessage(mMsg);
}

void Server::sendAll(Message *mMsg) {
	sendExcept(mMsg, NULL);
}

void Server::sendExcept(Message *mMsg, Connection *cCon) {
	foreach(User *u, qhUsers)
	if ((static_cast<Connection *>(u) != cCon) && (u->sState == Player::Authenticated))
		u->sendMessage(mMsg);
}

void Server::removeChannel(Channel *chan, Player *src, Channel *dest) {
	Channel *c;
	Player *p;

	if (dest == NULL)
		dest = Channel::get(chan->cParent->iId);

	chan->unlink(NULL);

	foreach(c, chan->qlChannels) {
		removeChannel(c, src, dest);
	}

	foreach(p, chan->qlPlayers) {
		chan->removePlayer(p);

		MessagePlayerMove mpm;
		mpm.uiSession = 0;
		mpm.uiVictim = p->uiSession;
		mpm.iChannelId = dest->iId;
		sendAll(&mpm);

		playerEnterChannel(p, dest);
	}

	MessageChannelRemove mcr;
	mcr.uiSession = src ? src->uiSession : 0;
	mcr.iId = chan->iId;
	sendAll(&mcr);

	removeChannel(chan);
	dbus->channelRemoved(chan);

	if (chan->cParent) {
		QWriteLocker wl(&qrwlUsers);
		chan->cParent->removeChannel(chan);
	}

	delete chan;
}

void Server::playerEnterChannel(Player *p, Channel *c, bool quiet) {
	if (quiet && (p->cChannel == c))
		return;

	{
		QWriteLocker wl(&qrwlUsers);
		c->addPlayer(p);
	}

	if (quiet)
		return;

	setLastChannel(p);
	dbus->playerStateChanged(p);

	bool mayspeak = ChanACL::hasPermission(p, c, ChanACL::Speak);
	bool sup = p->bSuppressed;

	if (! p->bMute) {
		if (mayspeak == sup) {
			// Ok, he can speak and was suppressed, or vice versa
			p->bSuppressed = ! mayspeak;

			MessagePlayerMute mpm;
			mpm.uiSession = 0;
			mpm.uiVictim = p->uiSession;
			mpm.bMute = p->bSuppressed;
			sendAll(&mpm);
		}
	}
}
