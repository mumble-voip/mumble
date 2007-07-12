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

#ifdef Q_OS_UNIX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#endif

Server *g_sServer;
ServerParams g_sp;

uint qHash(const Peer &p) {
	return p.first ^ p.second;
}

ServerParams::ServerParams() {
	qsPassword = QString();
	iPort = 64738;
	iCommandFrequency = 0;
	iTimeout = 30;
	iMaxBandwidth = 10000;
	iMaxUsers = 1000;
	qsWelcomeText = QString("Welcome to this server");
	qsDatabase = QString();
	iDBPort = 0;
	qsDBDriver = "QSQLITE";
	qsLogfile = "murmur.log";
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

	qsPassword = qs.value("serverpassword", qsPassword).toString();
	iPort = qs.value("port", iPort).toInt();
	iCommandFrequency = qs.value("commandtime", iCommandFrequency).toInt();
	iTimeout = qs.value("timeout", iTimeout).toInt();
	iMaxBandwidth = qs.value("bandwidth", iMaxBandwidth).toInt();
	iMaxUsers = qs.value("users", iMaxUsers).toInt();
	qsWelcomeText = qs.value("welcometext", qsWelcomeText).toString();

	qsDatabase = qs.value("database", qsDatabase).toString();

	qsDBDriver = qs.value("dbDriver", qsDBDriver).toString();
	qsDBUserName = qs.value("dbUsername", qsDBUserName).toString();
	qsDBPassword = qs.value("dbPassword", qsDBPassword).toString();
	qsDBHostName = qs.value("dbHost", qsDBHostName).toString();
	iDBPort = qs.value("dbPort", iDBPort).toInt();

	qsDBus = qs.value("dbus", qsDBus).toString();
	qsLogfile = qs.value("logfile", qsLogfile).toString();

	qsRegName = qs.value("registerName", qsRegName).toString();
	qsRegPassword = qs.value("registerPassword", qsRegPassword).toString();
	qurlRegWeb = QUrl(qs.value("registerUrl", qurlRegWeb.toString()).toString());
}

BandwidthRecord::BandwidthRecord() {
	iRecNum = 0;
	iSum = 0;
	qtFirst.start();
	for(int i=0;i<N_BANDWIDTH_SLOTS;i++)
		a_iBW[i] = 0;
}

void BandwidthRecord::addFrame(int size) {
	iSum -= a_iBW[iRecNum];
	a_iBW[iRecNum] = size;
	iSum += a_iBW[iRecNum];

	a_qtWhen[iRecNum].start();

	iRecNum++;
	if (iRecNum == N_BANDWIDTH_SLOTS)
		iRecNum = 0;
}

int BandwidthRecord::bytesPerSec() {
	// Multiply by 45; give 10% leniency
	unsigned int elapsed = a_qtWhen[iRecNum].elapsed();
	return (iSum * 1000) / elapsed;
}

void UDPThread::run() {
	qDebug("Starting UDP Thread");
	qusUdp = new QUdpSocket();
	if (! qusUdp->bind(g_sp.iPort))
		qFatal("Server: UDP Bind to port %d failed",g_sp.iPort);

#ifdef Q_OS_UNIX
	int val = IPTOS_PREC_FLASHOVERRIDE | IPTOS_LOWDELAY | IPTOS_THROUGHPUT;
	if (setsockopt(qusUdp->socketDescriptor(), SOL_IP, IP_TOS, &val, sizeof(val)))
		qWarning("Server: Failed to set TOS for UDP Socket");
#endif
	connect(this, SIGNAL(tcpTransmit(QByteArray, short)), g_sServer, SLOT(tcpTransmit(QByteArray, short)), Qt::QueuedConnection);

	QHostAddress senderAddr;
	quint16 senderPort;
	qint32 len;
	char buffer[65535];

	quint32 msgType;
	int sPlayerId;

	qDebug("Entering UDP event loop");

	while (qusUdp->waitForReadyRead(-1)) {
		QReadLocker rl(& g_sServer->qrwlConnections);
		while (qusUdp->hasPendingDatagrams()) {

			len=qusUdp->readDatagram(buffer, 65535, &senderAddr, &senderPort);

			PacketDataStream pds(buffer, len);
			pds >> msgType >> sPlayerId;

			if ((msgType != Message::Speex) && (msgType != Message::Ping))
				continue;
			if (! pds.isValid())
				continue;

			Peer p(senderAddr.toIPv4Address(), senderPort);

			if (p != qhPeers.value(sPlayerId)) {
				Connection *source = g_sServer->qmConnections.value(sPlayerId);
				if (! source || ! (source->peerAddress() == senderAddr)) {
					continue;
				}
				// At any point after this, the connection might go away WHILE we're processing. That is "bad".
				rl.unlock();
				{
					QWriteLocker wl(&g_sServer->qrwlConnections);
					if (g_sServer->qmConnections.contains(sPlayerId)) {
						qhHosts[sPlayerId] = senderAddr;
						qhPeers[sPlayerId] = p;
				    	}
				}
				rl.relock();
				if (! g_sServer->qmConnections.contains(sPlayerId)) {
				    continue;
				}
			}

			if (msgType == Message::Ping)
				qusUdp->writeDatagram(buffer, len, senderAddr, senderPort);
			else {
				processMsg(pds, g_sServer->qmConnections.value(sPlayerId));
			}
		}
	}
}

void UDPThread::fakeUdpPacket(Message *msg, Connection *source) {
	char buffer[65535];
	PacketDataStream pds(buffer, 65535);
	msg->messageToNetwork(pds);
	pds.rewind();

	quint32 msgType;
	int sPlayerId;

	pds >> msgType >> sPlayerId;

	QReadLocker rl(&g_sServer->qrwlConnections);
	processMsg(pds, source);
}

void UDPThread::sendMessage(short id, const char *data, int len, QByteArray &cache) {
	if (qhPeers.contains(id)) {
		qusUdp->writeDatagram(data, len, qhHosts[id], qhPeers[id].second);
	} else {
		if (cache.isEmpty())
			cache = QByteArray(data, len);
		emit tcpTransmit(cache,id);
	}
}

void UDPThread::processMsg(PacketDataStream &pds, Connection *cCon) {
	Player *pSrcPlayer = g_sServer->qmPlayers.value(cCon);
	if (!pSrcPlayer || (pSrcPlayer->sState != Player::Authenticated))
		return;

	Player *p;
	int seq, flags;

	if (pSrcPlayer->bMute || pSrcPlayer->bSuppressed)
		return;

	BandwidthRecord *bw = g_sServer->qmBandwidth[cCon];

	pds >> seq;
	pds >> flags;

    	int nframes = ((flags >> 4) & 0x03) + 1;
	int packetsize = (20 + 8 + 3 + pds.left() + nframes - 1) / nframes;
	for(int i = 0; i < nframes; i++)
		bw->addFrame(packetsize);

	if (bw->bytesPerSec() > g_sp.iMaxBandwidth) {
		// Suppress packet.
		return;
	}

	Channel *c = pSrcPlayer->cChannel;

	QByteArray qba;

	pds.rewind();
	const char *data = pds.charPtr();
	int len = pds.left();

	if (flags & MessageSpeex::LoopBack) {
		sendMessage(pSrcPlayer->sId, data, len, qba);
		return;
	}

	foreach(p, c->qlPlayers) {
		if (! p->bDeaf && ! p->bSelfDeaf && (p != pSrcPlayer))
			sendMessage(p->sId, data, len, qba);
	}

	if (! c->qhLinks.isEmpty()) {
		QSet<Channel *> chans = c->allLinks();
		chans.remove(c);

		foreach(Channel *l, chans) {
			if (ChanACL::hasPermission(pSrcPlayer, l, (flags & MessageSpeex::AltSpeak) ? ChanACL::AltSpeak : ChanACL::Speak)) {
				foreach(p, l->qlPlayers) {
					if (! p->bDeaf && ! p->bSelfDeaf)
						sendMessage(p->sId, data, len, qba);
				}
			}
		}
	}
}


Server::Server() {
	qtsServer = new QTcpServer(this);

	connect(qtsServer, SIGNAL(newConnection()), this, SLOT(newClient()));

	if (! qtsServer->listen(QHostAddress::Any, g_sp.iPort))
		qFatal("Server: TCP Listen on port %d failed",g_sp.iPort);

	udp = new UDPThread();
	udp->moveToThread(udp);

	log(QString("Server listening on port %1").arg(g_sp.iPort));

	for(int i=1;i<2000;i++)
		qqIds.enqueue(i);

	qtTimer = new QTimer(this);
	connect(qtTimer, SIGNAL(timeout()), this, SLOT(checkCommands()));
	if (g_sp.iCommandFrequency > 0)
		qtTimer->start(g_sp.iCommandFrequency * 1000);

	qtTimeout = new QTimer(this);
	connect(qtTimeout, SIGNAL(timeout()), this, SLOT(checkTimeout()));
	qtTimeout->start(5500);

	qlBans = ServerDB::getBans();
}

void Server::log(QString s, Connection *c) {
	if (c) {
		Player *p = qmPlayers.value(c);

		int id = 0;
		int iid = -1;
		QString name;
		if (p) {
			id = p->sId;
			iid = p->iId;
			name = p->qsName;
		}
		qWarning("<%d:%s(%d)> %s", id, qPrintable(name), iid, qPrintable(s));
	} else {
		qWarning("%s", qPrintable(s));
	}
}

void Server::newClient() {
	QTcpSocket *sock = qtsServer->nextPendingConnection();

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


	Connection *cCon = new Connection(this, sock);

	short id;

	if (qqIds.isEmpty()) {
		cCon->disconnect();
		return;
	}

	id=qqIds.dequeue();

	Player *pPlayer = Player::add(id);
	qmPlayers[cCon] = pPlayer;
	{
		QWriteLocker wl(&qrwlConnections);
		qmBandwidth[cCon] = new BandwidthRecord();
		qmConnections[id] = cCon;
	}

	connect(cCon, SIGNAL(connectionClosed(QString)), this, SLOT(connectionClosed(QString)));
	connect(cCon, SIGNAL(message(QByteArray &)), this, SLOT(message(QByteArray &)));

	log(QString("New connection: %1:%2").arg(sock->peerAddress().toString()).arg(sock->peerPort()), cCon);
}

void Server::connectionClosed(QString reason) {
	Connection *c = static_cast<Connection *>(sender());
	Player *pPlayer = qmPlayers.value(c);

	log(QString("Connection closed: %1").arg(reason), c);

	if (pPlayer->sState == Player::Authenticated) {
		MessageServerLeave mslMsg;
		mslMsg.sPlayerId=pPlayer->sId;
		sendExcept(&mslMsg, c);

		ServerDB::conLoggedOff(pPlayer);
		dbus->playerDisconnected(pPlayer);
	}

	QWriteLocker wl(&qrwlConnections);

	qmConnections.remove(pPlayer->sId);
	qmPlayers.remove(c);

	BandwidthRecord *bw = qmBandwidth.take(c);
	delete bw;

	Player::remove(pPlayer);

	qqIds.enqueue(pPlayer->sId);

	udp->qhHosts.remove(pPlayer->sId);
	udp->qhPeers.remove(pPlayer->sId);

	qhUserTextureCache.remove(pPlayer->iId);

	delete pPlayer;
	c->deleteLater();
}

void Server::message(QByteArray &qbaMsg, Connection *cCon) {
        if (cCon == NULL)
		cCon = static_cast<Connection *>(sender());
	Message *mMsg = Message::networkToMessage(qbaMsg);

	  // Just leftovers from the buffer and we just kicked
	  // the user off.
	  if (! qmPlayers.contains(cCon))
	  	return;

	  if (mMsg) {
		mMsg->process(cCon);
		delete mMsg;
	  } else {
		cCon->disconnect();
	  }
}


void Server::checkTimeout() {
    	QList<Connection *> qlClose;

	qrwlConnections.lockForRead();
	foreach(Connection *c, qmConnections) {
	    if (c->activityTime() > (g_sp.iTimeout * 1000)) {
		log(QLatin1String("Timeout"), c);
		qlClose.append(c);
	    }
	}
	qrwlConnections.unlock();
	foreach(Connection *c, qlClose)
		c->disconnect();
}

void Server::tcpTransmit(QByteArray a, short id) {
	Connection *c = qmConnections.value(id);
	if (c) {
		c->sendMessage(a);
		c->forceFlush();
	}
}

void Server::sendMessage(short id, Message *mMsg) {
	Connection *c = qmConnections.value(id);
	sendMessage(c, mMsg);
}

void Server::sendMessage(Connection *c, Message *mMsg) {
	c->sendMessage(mMsg);
}

void Server::sendAll(Message *mMsg) {
	sendExcept(mMsg, NULL);
}

void Server::sendExcept(Message *mMsg, Connection *cCon) {
	QHash<Connection *, Player *>::const_iterator i;
	for(i=qmPlayers.constBegin(); i != qmPlayers.constEnd(); ++i) {
		if ((i.key() != cCon) && (i.value()->sState == Player::Authenticated))
			sendMessage(i.key(), mMsg);
	}
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
		mpm.sPlayerId = 0;
		mpm.sVictim = p->sId;
		mpm.iChannelId = dest->iId;
		sendAll(&mpm);

		playerEnterChannel(p, dest);
	}

	MessageChannelRemove mcr;
	mcr.sPlayerId = src ? src->sId : 0;
	mcr.iId = chan->iId;
	sendAll(&mcr);

	ServerDB::removeChannel(chan);
	dbus->channelRemoved(chan);

	if (chan->cParent) {
		QWriteLocker wl(&qrwlConnections);
		chan->cParent->removeChannel(chan);
	}

	delete chan;
}

void Server::playerEnterChannel(Player *p, Channel *c, bool quiet) {
	if (quiet && (p->cChannel == c))
		return;

	{
		QWriteLocker wl(&qrwlConnections);
		c->addPlayer(p);
	}

	if (quiet)
		return;

	ServerDB::conChangedChannel(p);
	ServerDB::setLastChannel(p);
	dbus->playerStateChanged(p);

	bool mayspeak = ChanACL::hasPermission(p, c, ChanACL::Speak);
	bool sup = p->bSuppressed;

	if (! p->bMute) {
		if (mayspeak == sup) {
			// Ok, he can speak and was suppressed, or vice versa
			p->bSuppressed = ! mayspeak;

			MessagePlayerMute mpm;
			mpm.sPlayerId = 0;
			mpm.sVictim = p->sId;
			mpm.bMute = p->bSuppressed;
			g_sServer->sendAll(&mpm);
		}
	}
}

void Server::checkCommands() {
    	static bool warned = false;
	QList<ServerDB::qpCommand> cmdlist=ServerDB::getCommands();
	if (cmdlist.count() == 0)
		return;
	foreach(ServerDB::qpCommand cmd, cmdlist) {
	    	if (! warned) {
		    log(QLatin1String("The commands table is deprecated and will be removed in a later release. Please migrate to DBus."), NULL);
		    warned = true;
		}
		QString cmdname = cmd.first;
		QList<QVariant> argv = cmd.second;
		if (cmdname == "moveplayer") {
			Player *p = Player::get(argv[0].toInt());
			Channel *c = Channel::get(argv[1].toInt());

			if (! p || ! c)
				continue;
			playerEnterChannel(p, c);
			MessagePlayerMove mpm;
			mpm.sPlayerId = 0;
			mpm.sVictim = p->sId;
			mpm.iChannelId = c->iId;
			sendAll(&mpm);
		} else if (cmdname == "rename") {
			Player *p = Player::get(argv[0].toInt());
			QString name = argv[1].toString();

			if (! p || name.isEmpty())
				continue;
			MessagePlayerRename mpr;
			mpr.sPlayerId = p->sId;
			mpr.qsName = name;
			sendAll(&mpr);
		} else if (cmdname == "createchannel") {
			Channel *p = Channel::get(argv[0].toInt());
			QString name = argv[1].toString();

			if (! p || name.isEmpty())
				continue;

			Channel *c = ServerDB::addChannel(p, name);
			ServerDB::updateChannel(c);

			MessageChannelAdd mca;
			mca.sPlayerId = 0;
			mca.qsName = name;
			mca.iParent = p->iId;
			mca.iId = c->iId;
			g_sServer->sendAll(&mca);
		} else if (cmdname == "setgroup") {
			Channel *c = Channel::get(argv[0].toInt());
			QString name = argv[1].toString();
			QStringList list = argv[2].toString().split(QRegExp("\\D+"), QString::SkipEmptyParts);
			QList<int> mems;
			foreach(QString m, list) {
				mems.append(m.toInt());
			}
			Group *g = c->qhGroups.value(name);
			if (g)
				delete g;
			g = new Group(c, name);
			g->qsAdd = mems.toSet();
		} else if (cmdname == "quit") {
			QCoreApplication::instance()->quit();
		}
	}
}

#define MSG_SETUP(st) \
	Player *pSrcPlayer = g_sServer->qmPlayers[cCon]; \
	MessagePermissionDenied mpd; \
	sPlayerId = pSrcPlayer->sId; \
	if (pSrcPlayer->sState != st) \
		return

#define VICTIM_SETUP \
	Player *pDstPlayer = Player::get(sVictim); \
	Q_UNUSED(pDstPlayer) \
	Connection *cDst = g_sServer->qmConnections.value(sVictim); \
	Q_UNUSED(cDst)

#define PERM_DENIED(who, where, what) \
	mpd.qsReason = QString("%1 not allowed to %2 in %3").arg(who->qsName).arg(ChanACL::permName(what)).arg(where->qsName); \
	g_sServer->sendMessage(cCon, &mpd); \
	g_sServer->log(mpd.qsReason, cCon)
#define PERM_DENIED_TEXT(text) \
	mpd.qsReason = text; \
	g_sServer->sendMessage(cCon, &mpd)


void MessageServerAuthenticate::process(Connection *cCon) {
	MSG_SETUP(Player::Connected);

	Channel *c;

	pSrcPlayer->qsName = qsUsername;

	MessageServerReject msr;
	bool ok = false;

	QRegExp re("[\\w\\[\\]\\{\\}\\(\\)\\@\\|]+");

	bool nameok = re.exactMatch(qsUsername);
	if (nameok && qsUsername[0] == '@')
		nameok = false;
	if (nameok && qsUsername[0] == '#')
		nameok = false;

	// Fetch ID and stored username
	int id = ServerDB::authenticate(qsUsername, qsPassword);
	pSrcPlayer->iId = id >= 0 ? id : -1;
	pSrcPlayer->qsName = qsUsername;

	if (iVersion != MESSAGE_STREAM_VERSION) {
	  msr.qsReason = "Wrong version of mumble protocol";
	  msr.rtType = MessageServerReject::WrongVersion;
	} else if (! nameok) {
	  msr.qsReason = "Invalid Username";
	  msr.rtType = MessageServerReject::InvalidUsername;
	} else if (id==-1) {
	  msr.qsReason = "Wrong password for user";
	  msr.rtType = MessageServerReject::WrongUserPW;
	} else if (id==-2 && ! g_sp.qsPassword.isEmpty() && g_sp.qsPassword != qsPassword) {
	  msr.qsReason = "Invalid server password";
	  msr.rtType = MessageServerReject::WrongServerPW;
	} else {
	  ok = true;
	}

	Player *ppOld = Player::match(pSrcPlayer, true);
	Connection *cOld = ppOld ? g_sServer->qmConnections.value(ppOld->sId) : NULL;

	// Allow reuse of name from same IP
	if (ok && ppOld && (pSrcPlayer->iId == -1)) {
	    	if (cOld->peerAddress() != cCon->peerAddress()) {
			msr.qsReason = "Playername already in use";
			msr.rtType = MessageServerReject::UsernameInUse;
			ok = false;
  	        }
	}

	if (iMaxBandwidth > g_sp.iMaxBandwidth) {
		msr.qsReason = QString("Your maximum bandwidth(%1 kbit/s) above server limit (%2 kbit/s)").arg(iMaxBandwidth/125.0).arg(g_sp.iMaxBandwidth/125.0);
		msr.rtType = MessageServerReject::BandwidthExceeded;
		ok = false;
	}

	if ((id != 0) && (g_sServer->qmPlayers.count() > g_sp.iMaxUsers)) {
		msr.qsReason = QString("Server is full (max %1 users)").arg(g_sp.iMaxUsers);
		msr.rtType = MessageServerReject::ServerFull;
		ok = false;
	}

	if (! ok) {
	  g_sServer->log(QString("Rejected connection: %1").arg(msr.qsReason), cCon);
	  g_sServer->sendMessage(cCon, &msr);
	  cCon->disconnect();
	  return;
	}

	// Kick ghost
	if (ppOld) {
		g_sServer->log(QString("Disconnecting ghost"), cOld);
		cOld->disconnect();
	}

	int lchan = ServerDB::readLastChannel(pSrcPlayer);
	Channel *lc = Channel::get(lchan);
	if (! lc)
		lc = Channel::get(0);
	else if (! ChanACL::hasPermission(pSrcPlayer, lc, ChanACL::Enter))
		lc = Channel::get(0);

	g_sServer->playerEnterChannel(pSrcPlayer, lc, true);
	ServerDB::conLoggedOn(pSrcPlayer, cCon);

	QQueue<Channel *> q;
	QSet<Channel *> chans;
	q << Channel::get(0);
	while (! q.isEmpty()) {
		c = q.dequeue();

		chans.insert(c);

		MessageChannelAdd mca;
		mca.sPlayerId = 0;
		mca.iId = c->iId;
		mca.iParent = (c->cParent) ? c->cParent->iId : -1;
		mca.qsName = c->qsName;
		if (c->iId != 0)
			g_sServer->sendMessage(cCon, &mca);

		foreach(c, c->qlChannels)
			q.enqueue(c);
	}

	foreach(c, chans) {
		if (c->qhLinks.count() > 0) {
			MessageChannelLink mcl;
			mcl.iId = c->iId;
			mcl.ltType = MessageChannelLink::Link;
			foreach(Channel *l, c->qhLinks.keys())
				mcl.qlTargets << l->iId;
			g_sServer->sendMessage(cCon, &mcl);
		}
	}

	MessageServerJoin msjMsg;
	MessagePlayerMove mpm;

	pSrcPlayer->sState = Player::Authenticated;
	msjMsg.sPlayerId = pSrcPlayer->sId;
	msjMsg.iId = pSrcPlayer->iId;
	msjMsg.qsPlayerName = pSrcPlayer->qsName;
	g_sServer->sendExcept(&msjMsg, cCon);

	mpm.sPlayerId = 0;
	mpm.sVictim = pSrcPlayer->sId;
	mpm.iChannelId = pSrcPlayer->cChannel->iId;
	if (mpm.iChannelId != 0)
		g_sServer->sendExcept(&mpm, cCon);

	foreach(Player *pPlayer, g_sServer->qmPlayers) {
		if (pPlayer->sState != Player::Authenticated)
			continue;
		msjMsg.sPlayerId = pPlayer->sId;
		msjMsg.iId = pPlayer->iId;
		msjMsg.qsPlayerName = pPlayer->qsName;
		g_sServer->sendMessage(cCon, &msjMsg);

		if (pPlayer->bDeaf) {
			MessagePlayerDeaf mpdMsg;
			mpdMsg.sPlayerId = 0;
			mpdMsg.sVictim = pPlayer->sId;
			mpdMsg.bDeaf = true;
			g_sServer->sendMessage(cCon, &mpdMsg);
		} else if (pPlayer->bMute || pPlayer->bSuppressed) {
			MessagePlayerMute mpmMsg;
			mpmMsg.sPlayerId = 0;
			mpmMsg.sVictim = pPlayer->sId;
			mpmMsg.bMute = true;
			g_sServer->sendMessage(cCon, &mpmMsg);
		}
		if (pPlayer->bSelfDeaf || pPlayer->bSelfMute) {
			MessagePlayerSelfMuteDeaf mpsmdMsg;
			mpsmdMsg.sPlayerId = pPlayer->sId;
			mpsmdMsg.bDeaf = pPlayer->bSelfDeaf;
			mpsmdMsg.bMute = pPlayer->bSelfMute;
			g_sServer->sendMessage(cCon, &mpsmdMsg);
		}

		mpm.sPlayerId = 0;
		mpm.sVictim = pPlayer->sId;
		mpm.iChannelId = pPlayer->cChannel->iId;
		g_sServer->sendMessage(cCon, &mpm);
	}


	MessageServerSync mssMsg;
	mssMsg.sPlayerId = pSrcPlayer->sId;
	mssMsg.qsWelcomeText = g_sp.qsWelcomeText;
	mssMsg.iMaxBandwidth = g_sp.iMaxBandwidth;
	g_sServer->sendMessage(cCon, &mssMsg);
	g_sServer->log(QString("Authenticated: %1").arg(qsUsername), cCon);

	dbus->playerConnected(pSrcPlayer);
	g_sServer->playerEnterChannel(pSrcPlayer, lc, false);
}

void MessageServerBanList::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	if (! ChanACL::hasPermission(pSrcPlayer, Channel::get(0), ChanACL::Write)) {
		PERM_DENIED(pSrcPlayer, Channel::get(0), ChanACL::Write);
		return;
	}
	if (bQuery) {
		MessageServerBanList msbl;
		msbl.sPlayerId = 0;
		msbl.bQuery = false;
		msbl.qlBans = g_sServer->qlBans;
		g_sServer->sendMessage(cCon, &msbl);
	} else {
		g_sServer->qlBans = qlBans;
		g_sServer->log(QString("Updated banlist"), cCon);
		ServerDB::setBans(qlBans);
	}
}

void MessageServerLeave::process(Connection *cCon) {
  cCon->disconnect();
}

void MessageServerJoin::process(Connection *cCon) {
  cCon->disconnect();
}

void MessageServerReject::process(Connection *cCon) {
  cCon->disconnect();
}

void MessageServerSync::process(Connection *cCon) {
  cCon->disconnect();
}

void MessagePermissionDenied::process(Connection *cCon) {
  cCon->disconnect();
}

void MessagePlayerRename::process(Connection *cCon) {
  cCon->disconnect();
}

void MessageSpeex::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	sPlayerId = pSrcPlayer->sId;
	g_sServer->udp->fakeUdpPacket(this, cCon);
}

void MessagePlayerMute::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MuteDeafen)) {
		PERM_DENIED(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MuteDeafen);
		return;
	}

	if (! bMute && pDstPlayer->bSuppressed) {
		pDstPlayer->bSuppressed = false;
	} else if (pDstPlayer->bMute == bMute) {
		return;
	}

	pDstPlayer->bMute = bMute;
	g_sServer->sendAll(this);

	if (! bMute && pDstPlayer->bDeaf) {
		pDstPlayer->bDeaf = false;
	}

	dbus->playerStateChanged(pDstPlayer);

	g_sServer->log(QString("Muted %1 (%2)").arg(pDstPlayer->qsName).arg(bMute), cCon);
}

void MessagePlayerDeaf::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MuteDeafen)) {
		PERM_DENIED(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MuteDeafen);
		return;
	}

	if (pDstPlayer->bDeaf == bDeaf)
		return;

	pDstPlayer->bDeaf = bDeaf;
	g_sServer->sendAll(this);

	if (bDeaf && ! pDstPlayer->bMute) {
		pDstPlayer->bMute = true;
	}

	dbus->playerStateChanged(pDstPlayer);

	g_sServer->log(QString("Deafened %1 (%2)").arg(pDstPlayer->qsName).arg(bDeaf), cCon);
}

void MessagePlayerKick::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MoveKick)) {
		PERM_DENIED(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MoveKick);
		return;
	}

	g_sServer->sendAll(this);
	g_sServer->log(QString("Kicked %1 (%2)").arg(pDstPlayer->qsName).arg(qsReason), cCon);
	cDst->disconnect();
}

void MessagePlayerBan::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, Channel::get(0), ChanACL::MoveKick)) {
		PERM_DENIED(pSrcPlayer, Channel::get(0), ChanACL::MoveKick);
		return;
	}

	g_sServer->sendAll(this);
	g_sServer->log(QString("Kickbanned %1 (%2)").arg(pDstPlayer->qsName).arg(qsReason), cCon);


	QHostAddress adr = cDst->peerAddress();
	quint32 base = adr.toIPv4Address();
	g_sServer->qlBans << QPair<quint32,int>(base, 32);
	ServerDB::setBans(g_sServer->qlBans);

	cDst->disconnect();
}

void MessagePlayerSelfMuteDeaf::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	pSrcPlayer->bSelfMute = bMute;
	pSrcPlayer->bSelfDeaf = bDeaf;
	g_sServer->sendAll(this);
	dbus->playerStateChanged(pSrcPlayer);
}

void MessagePlayerMove::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if (! pDstPlayer)
		return;

	Channel *c = Channel::get(iChannelId);
	if (!c || (c == pDstPlayer->cChannel))
		return;

	if ((pSrcPlayer != pDstPlayer) && ! ChanACL::hasPermission(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MoveKick)) {
		PERM_DENIED(pSrcPlayer, pDstPlayer->cChannel, ChanACL::MoveKick);
		return;
	}

	if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::MoveKick) && ! ChanACL::hasPermission(pDstPlayer, c, ChanACL::Enter)) {
		PERM_DENIED(pDstPlayer, c, ChanACL::Enter);
		return;
	}


	g_sServer->sendAll(this);
	g_sServer->playerEnterChannel(pDstPlayer, c);
	g_sServer->log(QString("Moved to %1 (%2)").arg(c->qsName).arg(pDstPlayer->qsName), cCon);
}

void MessageChannelAdd::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	Channel *p = Channel::get(iParent);
	if (!p)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, p, ChanACL::MakeChannel)) {
		PERM_DENIED(pSrcPlayer, p, ChanACL::MakeChannel);
		return;
	}

	QRegExp re("[ \\w\\#\\[\\]\\{\\}\\(\\)\\@\\|]+");

	if (! re.exactMatch(qsName)) {
		PERM_DENIED_TEXT("Illegal channel name");
		return;
	}

	QRegExp re2("\\w");
	if (re2.indexIn(qsName) == -1) {
		PERM_DENIED_TEXT("Must have alphanumeric in name");
		return;
	}

	Channel *c = ServerDB::addChannel(p, qsName);
	if (pSrcPlayer->iId >= 0) {
		Group *g = new Group(c, "admin");
		g->qsAdd << pSrcPlayer->iId;
	}
	ServerDB::updateChannel(c);

	dbus->channelCreated(c);

	iId = c->iId;
	g_sServer->sendAll(this);
	g_sServer->log(QString("Added channel %1 (%2)").arg(qsName).arg(p->qsName), cCon);
}

void MessageChannelRemove::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = Channel::get(iId);
	if (!c)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::Write) || (iId == 0)) {
		PERM_DENIED(pSrcPlayer, c, ChanACL::Write);
		return;
	}

	g_sServer->log(QString("Removed channel %1").arg(c->qsName), cCon);

	g_sServer->removeChannel(c, pSrcPlayer);
}

void MessageChannelMove::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = Channel::get(iId);
	Channel *np = Channel::get(iParent);
	if (!c || ! np)
		return;

	if (np == c->cParent)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::Write)) {
		PERM_DENIED(pSrcPlayer, c, ChanACL::Write);
		return;
	}

	if (! ChanACL::hasPermission(pSrcPlayer, np, ChanACL::MakeChannel)) {
		PERM_DENIED(pSrcPlayer, np, ChanACL::MakeChannel);
		return;
	}

	// Can't move to a subchannel of itself
	Channel *p = np;
	while (p) {
		if (p == c)
			return;
		p = p->cParent;
	}

	g_sServer->log(QString("Moved channel %1 (%2 -> %3)").arg(c->qsName).arg(c->cParent->qsName).arg(np->qsName), cCon);

	c->cParent->removeChannel(c);
	np->addChannel(c);
	ServerDB::updateChannel(c);
	dbus->channelStateChanged(c);
	g_sServer->sendAll(this);
}

void MessageChannelLink::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = Channel::get(iId);
	if (!c)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::LinkChannel)) {
		PERM_DENIED(pSrcPlayer, c, ChanACL::LinkChannel);
		return;
	}

	Channel *l = (qlTargets.count() == 1 ) ? Channel::get(qlTargets[0]) : NULL;

	switch (ltType) {
		case Link:
			if (!l)
				return;
			if (! ChanACL::hasPermission(pSrcPlayer, l, ChanACL::LinkChannel)) {
				PERM_DENIED(pSrcPlayer, l, ChanACL::LinkChannel);
				return;
			}
			break;
		case Unlink:
			if (!l)
				return;
			break;
		case UnlinkAll:
			if (qlTargets.count() > 0)
				return;
			break;
		default:
			if (qlTargets.count() <= 0)
				return;
	}

	QSet<Channel *> oldset = c->qhLinks.keys().toSet();

	switch(ltType) {
		case UnlinkAll:
			c->unlink(NULL);
			ServerDB::removeLink(c, NULL);
			dbus->channelStateChanged(c);
			g_sServer->log(QString("Unlinked all from channel %1").arg(c->qsName), cCon);
			g_sServer->sendAll(this);
			return;
		case Link:
			c->link(l);
			ServerDB::addLink(c, l);
			dbus->channelStateChanged(c);
			g_sServer->log(QString("Linked channel %1 and %2").arg(c->qsName).arg(l->qsName), cCon);
			break;
		case Unlink:
			c->unlink(l);
			ServerDB::removeLink(c, l);
			dbus->channelStateChanged(c);
			g_sServer->log(QString("Unlinked channel %1 and %2").arg(c->qsName).arg(l->qsName), cCon);
			break;
		case PushLink:
			foreach(int tid, qlTargets) {
				l=Channel::get(tid);
				if (l && ChanACL::hasPermission(pSrcPlayer, l, ChanACL::LinkChannel))
					c->playerLink(l, pSrcPlayer);
			}
			break;
		case PushUnlink:
			foreach(int tid, qlTargets) {
				l=Channel::get(tid);
				if (l)
					c->playerUnlink(l, pSrcPlayer);
			}
			break;
	}

	QSet<Channel *> newset = c->qhLinks.keys().toSet();
	QSet<Channel *> changed;

	if (newset.count() == oldset.count())
		return;

	MessageChannelLink mcl;
	mcl.iId = iId;
	mcl.sPlayerId=sPlayerId;

	if (newset.count() > oldset.count()) {
		mcl.ltType = Link;
		changed = newset - oldset;
	} else {
		mcl.ltType = Unlink;
		changed = oldset - newset;
	}
	foreach(l, changed)
		mcl.qlTargets << l->iId;
	g_sServer->sendAll(&mcl);
}

void MessageTextMessage::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;
	g_sServer->sendMessage(cDst, this);
}

void MessageEditACL::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = Channel::get(iId);
	if (!c)
		return;

	if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::Write)) {
		PERM_DENIED(pSrcPlayer, c, ChanACL::Write);
		return;
	}

	MessageEditACL mea;
	mea.iId = iId;

	if (bQuery) {
		QStack<Channel *> chans;
		Channel *p;
		ChanACL *acl;

		p = c;
		while (p) {
			chans.push(p);
			if ((p==c) || p->bInheritACL)
				p = p->cParent;
			else
				p = NULL;
		}

		mea.bQuery = false;
		mea.bInheritACL = c->bInheritACL;

		while(! chans.isEmpty()) {
			p = chans.pop();
			foreach(acl, p->qlACL) {
				if ((p == c) || (acl->bApplySubs)) {
					ACLStruct as;
					as.bInherited = (p != c);
					as.bApplyHere = acl->bApplyHere;
					as.bApplySubs = acl->bApplySubs;
					as.iPlayerId = acl->iPlayerId;
					as.qsGroup = acl->qsGroup;
					as.pDeny = acl->pDeny;
					as.pAllow = acl->pAllow;
					mea.acls << as;
				}
			}
		}

		p = c->cParent;
		QSet<QString> allnames=Group::groupNames(c);
		QString name;
		foreach(name, allnames) {
			Group *g = c->qhGroups.value(name);
			Group *pg = p ? Group::getGroup(p, name) : NULL;
			GroupStruct gs;
			gs.qsName = name;
			gs.bInherit = g ? g->bInherit : true;
			gs.bInheritable = g ? g->bInheritable : true;
			gs.bInherited = ((pg != NULL) && pg->bInheritable);
			if (g) {
				gs.qsAdd = g->qsAdd;
				gs.qsRemove = g->qsRemove;
			}
			if (pg)
				gs.qsInheritedMembers = pg->members();
			mea.groups << gs;
		}
		g_sServer->sendMessage(cCon, &mea);
	} else {
		Group *g;
		ChanACL *a;
		GroupStruct gs;
		ACLStruct as;

		foreach(g, c->qhGroups)
			delete g;
		foreach(a, c->qlACL)
			delete a;

		c->qhGroups.clear();
		c->qlACL.clear();

		c->bInheritACL = bInheritACL;

		foreach(gs, groups) {
			g = new Group(c, gs.qsName);
			g->bInherit = gs.bInherit;
			g->bInheritable = gs.bInheritable;
			g->qsAdd = gs.qsAdd;
			g->qsRemove = gs.qsRemove;
		}

		foreach(as, acls) {
			a = new ChanACL(c);
			a->bApplyHere=as.bApplyHere;
			a->bApplySubs=as.bApplySubs;
			a->iPlayerId=as.iPlayerId;
			a->qsGroup=as.qsGroup;
			a->pDeny=as.pDeny;
			a->pAllow=as.pAllow;
		}

		ChanACL::clearCache();

		if (! ChanACL::hasPermission(pSrcPlayer, c, ChanACL::Write)) {
			a = new ChanACL(c);
			a->bApplyHere=true;
			a->bApplySubs=false;
			a->iPlayerId=pSrcPlayer->iId;
			a->pDeny=ChanACL::None;
			a->pAllow=ChanACL::Write | ChanACL::Traverse;

			ChanACL::clearCache();
		}

		ServerDB::updateChannel(c);
		g_sServer->log(QString("Updated ACL in channel %1(%2)").arg(c->qsName).arg(c->iId), cCon);
	}
}

void MessageQueryUsers::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);

	int i;
	for(i=0;i<qlIds.count();i++) {
		QString name = qlNames[i];
		int id = qlIds[i];
		if (id == -1) {
			id = g_sServer->qhUserIDCache.value(name);
			if (! g_sServer->qhUserIDCache.contains(name)) {
				id = ServerDB::getUserID(name);
				if (id != -1)
					g_sServer->qhUserIDCache[name] = id;
			}
			qlIds[i] = id;
		} else {
			if (! g_sServer->qhUserNameCache.contains(id)) {
				name = ServerDB::getUserName(id);
				if (! name.isEmpty())
					g_sServer->qhUserNameCache[id] = name;
			}
			qlNames[i] = g_sServer->qhUserNameCache.value(id);
		}
	}
	g_sServer->sendMessage(cCon, this);
}

void MessagePing::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	g_sServer->sendMessage(cCon, this);
}

void MessageTexture::process(Connection *cCon) {
	MSG_SETUP(Player::Authenticated);
	if (! g_sServer->qhUserTextureCache.contains(iPlayerId)) {
		QByteArray qba = ServerDB::getUserTexture(iPlayerId);
		if (! qba.isEmpty()) {
			qba = qCompress(qba);
		}
		g_sServer->qhUserTextureCache.insert(iPlayerId, qba);
	}
	qbaTexture = g_sServer->qhUserTextureCache.value(iPlayerId);
	if (! qbaTexture.isEmpty())
		g_sServer->sendMessage(cCon, this);
}
