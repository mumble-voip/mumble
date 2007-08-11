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

#define MSG_SETUP(st) \
	User *uSource = static_cast<User *>(cCon); \
	MessagePermissionDenied mpd; \
	msg->uiSession = uSource->uiSession; \
	if (uSource->sState != st) \
		return

#define VICTIM_SETUP \
	User *pDstUser = qhUsers.value(msg->uiVictim); \
	if (! pDstUser) \
		return; \
	Q_UNUSED(pDstUser)

#define PERM_DENIED(who, where, what) \
	mpd.qsReason = QString("%1 not allowed to %2 in %3").arg(who->qsName).arg(ChanACL::permName(what)).arg(where->qsName); \
	sendMessage(cCon, &mpd); \
	log(uSource, "%s", qPrintable(mpd.qsReason))
#define PERM_DENIED_TEXT(text) \
	mpd.qsReason = text; \
	sendMessage(cCon, &mpd)


void Server::msgServerAuthenticate(Connection *cCon, MessageServerAuthenticate *msg) {
	MSG_SETUP(Player::Connected);

	Channel *c;

	uSource->qsName = msg->qsUsername;

	MessageServerReject msr;
	bool ok = false;

	QRegExp re("[\\w\\[\\]\\{\\}\\(\\)\\@\\|\\.]+");

	bool nameok = re.exactMatch(msg->qsUsername);
	if (nameok && msg->qsUsername[0] == '@')
		nameok = false;
	if (nameok && msg->qsUsername[0] == '#')
		nameok = false;

	// Fetch ID and stored username.
	// Since this may call DBus, which may recall our dbus messages, this function needs
	// to support re-entrancy, and also to support the fact that sessions may go away.
	int id = authenticate(msg->qsUsername, msg->qsPassword);

	// Did the session go away?
	Player *p = qhUsers.value(msg->uiSession);
	if (p != uSource)
		return;

	uSource->iId = id >= 0 ? id : -1;
	uSource->qsName = msg->qsUsername;

	if (msg->iVersion != MESSAGE_STREAM_VERSION) {
		msr.qsReason = QString("Wrong version of mumble protocol (client: %1, server: %2)").arg(msg->iVersion).arg(MESSAGE_STREAM_VERSION);
		msr.rtType = MessageServerReject::WrongVersion;
	} else if (! nameok) {
		msr.qsReason = "Invalid Username";
		msr.rtType = MessageServerReject::InvalidUsername;
	} else if (id==-1) {
		msr.qsReason = "Wrong password for user";
		msr.rtType = MessageServerReject::WrongUserPW;
	} else if (id==-2 && ! qsPassword.isEmpty() && qsPassword != msg->qsPassword) {
		msr.qsReason = "Invalid server password";
		msr.rtType = MessageServerReject::WrongServerPW;
	} else {
		ok = true;
	}

	User *uOld = NULL;
	foreach(User *u, qhUsers) {
		if (u == uSource)
			continue;
		if (((u->iId>=0) && (u->iId == uSource->iId)) ||
		        (u->qsName == uSource->qsName)) {
			uOld = u;
			break;
		}
	}

	// Allow reuse of name from same IP
	if (ok && uOld && (uSource->iId == -1)) {
		if (uOld->peerAddress() != cCon->peerAddress()) {
			msr.qsReason = "Playername already in use";
			msr.rtType = MessageServerReject::UsernameInUse;
			ok = false;
		}
	}

	if (msg->iMaxBandwidth > iMaxBandwidth) {
		msr.qsReason = QString::fromLatin1("Your maximum bandwidth(%1 kbit/s) above server limit (%2 kbit/s)").arg(msg->iMaxBandwidth/125.0).arg(iMaxBandwidth/125.0);
		msr.rtType = MessageServerReject::BandwidthExceeded;
		ok = false;
	}

	if ((id != 0) && (qhUsers.count() > iMaxUsers)) {
		msr.qsReason = QString::fromLatin1("Server is full (max %1 users)").arg(iMaxUsers);
		msr.rtType = MessageServerReject::ServerFull;
		ok = false;
	}

	if (! ok) {
		log(uSource, "Rejected connection: %s", qPrintable(msr.qsReason));
		sendMessage(cCon, &msr);
		cCon->disconnectSocket();
		return;
	}

	// Kick ghost
	if (uOld) {
		log(uSource, "Disconnecting ghost");
		uOld->disconnectSocket();
	}

	// Setup UDP encryption
	uSource->csCrypt.genKey();
	MessageCryptSetup mcs;
	mcs.uiSession = uSource->uiSession;
	mcs.qbaKey = QByteArray(reinterpret_cast<const char *>(uSource->csCrypt.raw_key), AES_BLOCK_SIZE);
	mcs.qbaServerNonce = QByteArray(reinterpret_cast<const char *>(uSource->csCrypt.encrypt_iv), AES_BLOCK_SIZE);
	mcs.qbaClientNonce = QByteArray(reinterpret_cast<const char *>(uSource->csCrypt.decrypt_iv), AES_BLOCK_SIZE);
	sendMessage(cCon, &mcs);

	int lchan = readLastChannel(uSource);
	Channel *lc = qhChannels.value(lchan);
	if (! lc)
		lc = qhChannels.value(0);
	else if (! hasPermission(uSource, lc, ChanACL::Enter))
		lc = qhChannels.value(0);

	playerEnterChannel(uSource, lc, true);

	QQueue<Channel *> q;
	QSet<Channel *> chans;
	q << qhChannels.value(0);
	while (! q.isEmpty()) {
		c = q.dequeue();

		chans.insert(c);

		MessageChannelAdd mca;
		mca.uiSession = 0;
		mca.iId = c->iId;
		mca.iParent = (c->cParent) ? c->cParent->iId : -1;
		mca.qsName = c->qsName;
		if (c->iId != 0)
			sendMessage(cCon, &mca);

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
			sendMessage(cCon, &mcl);
		}
	}

	MessageServerJoin msjMsg;
	MessagePlayerMove mpm;

	uSource->sState = Player::Authenticated;
	msjMsg.uiSession = uSource->uiSession;
	msjMsg.iId = uSource->iId;
	msjMsg.qsPlayerName = uSource->qsName;
	sendExcept(&msjMsg, cCon);

	mpm.uiSession = 0;
	mpm.uiVictim = uSource->uiSession;
	mpm.iChannelId = uSource->cChannel->iId;
	if (mpm.iChannelId != 0)
		sendExcept(&mpm, cCon);

	foreach(User *u, qhUsers) {
		if (u->sState != Player::Authenticated)
			continue;
		msjMsg.uiSession = u->uiSession;
		msjMsg.iId = u->iId;
		msjMsg.qsPlayerName = u->qsName;
		sendMessage(cCon, &msjMsg);

		if (u->bDeaf) {
			MessagePlayerDeaf mpdMsg;
			mpdMsg.uiSession = 0;
			mpdMsg.uiVictim = u->uiSession;
			mpdMsg.bDeaf = true;
			sendMessage(cCon, &mpdMsg);
		} else if (u->bMute || u->bSuppressed) {
			MessagePlayerMute mpmMsg;
			mpmMsg.uiSession = 0;
			mpmMsg.uiVictim = u->uiSession;
			mpmMsg.bMute = true;
			sendMessage(cCon, &mpmMsg);
		}
		if (u->bSelfDeaf || u->bSelfMute) {
			MessagePlayerSelfMuteDeaf mpsmdMsg;
			mpsmdMsg.uiSession = u->uiSession;
			mpsmdMsg.bDeaf = u->bSelfDeaf;
			mpsmdMsg.bMute = u->bSelfMute;
			sendMessage(cCon, &mpsmdMsg);
		}

		mpm.uiSession = 0;
		mpm.uiVictim = u->uiSession;
		mpm.iChannelId = u->cChannel->iId;
		sendMessage(cCon, &mpm);
	}

	MessageServerSync mssMsg;
	mssMsg.uiSession = uSource->uiSession;
	mssMsg.qsWelcomeText = qsWelcomeText;
	mssMsg.iMaxBandwidth = iMaxBandwidth;
	sendMessage(cCon, &mssMsg);
	log(uSource, "Authenticated");

	dbus->playerConnected(uSource);
	playerEnterChannel(uSource, lc, false);
}

void Server::msgServerBanList(Connection *cCon, MessageServerBanList *msg) {
	MSG_SETUP(Player::Authenticated);

	if (! hasPermission(uSource, qhChannels.value(0), ChanACL::Write)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Write);
		return;
	}
	if (msg->bQuery) {
		MessageServerBanList msbl;
		msbl.uiSession = 0;
		msbl.bQuery = false;
		msbl.qlBans = qlBans;
		sendMessage(cCon, &msbl);
	} else {
		qlBans = msg->qlBans;
		saveBans();
		log(uSource, "Updated banlist");
	}
}

void Server::msgServerLeave(Connection *cCon, MessageServerLeave *) {
	cCon->disconnectSocket();
}

void Server::msgServerJoin(Connection *cCon, MessageServerJoin *) {
	cCon->disconnectSocket();
}

void Server::msgServerReject(Connection *cCon, MessageServerReject *) {
	cCon->disconnectSocket();
}

void Server::msgServerSync(Connection *cCon, MessageServerSync *) {
	cCon->disconnectSocket();
}

void Server::msgPermissionDenied(Connection *cCon, MessagePermissionDenied *) {
	cCon->disconnectSocket();
}

void Server::msgPlayerRename(Connection *cCon, MessagePlayerRename *) {
	cCon->disconnectSocket();
}

void Server::msgCryptSetup(Connection *cCon, MessageCryptSetup *) {
	cCon->disconnectSocket();
}

void Server::msgSpeex(Connection *cCon, MessageSpeex *msg) {
	MSG_SETUP(Player::Authenticated);
	fakeUdpPacket(msg, cCon);
}

void Server::msgPlayerMute(Connection *cCon, MessagePlayerMute *msg) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if ((pDstUser->iId ==0) || ! hasPermission(uSource, pDstUser->cChannel, ChanACL::MuteDeafen)) {
		PERM_DENIED(uSource, pDstUser->cChannel, ChanACL::MuteDeafen);
		return;
	}

	if (! msg->bMute && pDstUser->bSuppressed) {
		pDstUser->bSuppressed = false;
	} else if (pDstUser->bMute == msg->bMute) {
		return;
	}

	pDstUser->bMute = msg->bMute;
	sendAll(msg);

	if (! msg->bMute && pDstUser->bDeaf) {
		pDstUser->bDeaf = false;
	}

	dbus->playerStateChanged(pDstUser);

	log(uSource, "Muted %s (%d)", qPrintable(pDstUser->qsName), msg->bMute);
}

void Server::msgPlayerDeaf(Connection *cCon, MessagePlayerDeaf *msg) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if ((pDstUser->iId ==0) || ! hasPermission(uSource, pDstUser->cChannel, ChanACL::MuteDeafen)) {
		PERM_DENIED(uSource, pDstUser->cChannel, ChanACL::MuteDeafen);
		return;
	}

	if (pDstUser->bDeaf == msg->bDeaf)
		return;

	pDstUser->bDeaf = msg->bDeaf;
	sendAll(msg);

	if (msg->bDeaf && ! pDstUser->bMute) {
		pDstUser->bMute = true;
	}

	dbus->playerStateChanged(pDstUser);

	log(uSource, "Deafened %s (%d)", qPrintable(pDstUser->qsName),msg->bDeaf);
}

void Server::msgPlayerKick(Connection *cCon, MessagePlayerKick *msg) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if ((pDstUser->iId ==0) || ! hasPermission(uSource, pDstUser->cChannel, ChanACL::MoveKick)) {
		PERM_DENIED(uSource, pDstUser->cChannel, ChanACL::MoveKick);
		return;
	}

	sendAll(msg);
	log(uSource, "Kicked %s (%s)", qPrintable(pDstUser->qsName), qPrintable(msg->qsReason));
	pDstUser->disconnectSocket();
}

void Server::msgPlayerBan(Connection *cCon, MessagePlayerBan *msg) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	if ((pDstUser->iId ==0) || ! hasPermission(uSource, qhChannels.value(0), ChanACL::MoveKick)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::MoveKick);
		return;
	}

	sendAll(msg);
	log(uSource, "Kickbanned %s (%s)", qPrintable(pDstUser->qsName), qPrintable(msg->qsReason));

	QHostAddress adr = pDstUser->peerAddress();
	quint32 base = adr.toIPv4Address();
	qlBans << QPair<quint32,int>(base, 32);
	saveBans();

	pDstUser->disconnectSocket();
}

void Server::msgPlayerSelfMuteDeaf(Connection *cCon, MessagePlayerSelfMuteDeaf *msg) {
	MSG_SETUP(Player::Authenticated);

	uSource->bSelfMute = msg->bMute;
	uSource->bSelfDeaf = msg->bDeaf;
	sendAll(msg);
	dbus->playerStateChanged(uSource);
}

void Server::msgPlayerMove(Connection *cCon, MessagePlayerMove *msg) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	Channel *c = qhChannels.value(msg->iChannelId);
	if (!c || (c == pDstUser->cChannel))
		return;

	if ((uSource != pDstUser) && ((pDstUser->iId ==0) || ! hasPermission(uSource, pDstUser->cChannel, ChanACL::MoveKick))) {
		PERM_DENIED(uSource, pDstUser->cChannel, ChanACL::MoveKick);
		return;
	}

	if (! hasPermission(uSource, c, ChanACL::MoveKick) && ! hasPermission(pDstUser, c, ChanACL::Enter)) {
		PERM_DENIED(pDstUser, c, ChanACL::Enter);
		return;
	}

	sendAll(msg);
	playerEnterChannel(pDstUser, c);
	log(uSource, "Moved %s to %s",qPrintable(pDstUser->qsName), qPrintable(c->qsName));
}

void Server::msgChannelAdd(Connection *cCon, MessageChannelAdd *msg) {
	MSG_SETUP(Player::Authenticated);

	Channel *p = qhChannels.value(msg->iParent);
	if (!p)
		return;

	if (! hasPermission(uSource, p, ChanACL::MakeChannel)) {
		PERM_DENIED(uSource, p, ChanACL::MakeChannel);
		return;
	}

	QRegExp re("[ \\w\\#\\[\\]\\{\\}\\(\\)\\@\\|]+");

	if (! re.exactMatch(msg->qsName)) {
		PERM_DENIED_TEXT("Illegal channel name");
		return;
	}

	QRegExp re2("\\w");
	if (re2.indexIn(msg->qsName) == -1) {
		PERM_DENIED_TEXT("Must have alphanumeric in name");
		return;
	}

	Channel *c = addChannel(p, msg->qsName);
	if (uSource->iId >= 0) {
		Group *g = new Group(c, "admin");
		g->qsAdd << uSource->iId;
	}
	updateChannel(c);

	dbus->channelCreated(c);

	msg->iId = c->iId;
	sendAll(msg);
	log(uSource, "Added channel %s (%s)", qPrintable(c->qsName), qPrintable(p->qsName));
}

void Server::msgChannelRemove(Connection *cCon, MessageChannelRemove *msg) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = qhChannels.value(msg->iId);
	if (!c)
		return;

	if (! hasPermission(uSource, c, ChanACL::Write) || (msg->iId == 0)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	log(uSource, "Removed channel %s",qPrintable(c->qsName));

	removeChannel(c, uSource);
}

void Server::msgChannelRename(Connection *cCon, MessageChannelRename *msg) {
	MSG_SETUP(Player::Authenticated);
	Channel *c = qhChannels.value(msg->iId);

	if (!c)
		return;

	if (! hasPermission(uSource, c, ChanACL::Write)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	log(uSource, "Renamed channel %s to %s", qPrintable(c->qsName), qPrintable(msg->qsName));
	c->qsName = msg->qsName;
	updateChannel(c);
	dbus->channelStateChanged(c);
	sendAll(msg);
}

void Server::msgChannelMove(Connection *cCon, MessageChannelMove *msg) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = qhChannels.value(msg->iId);
	Channel *np = qhChannels.value(msg->iParent);
	if (!c || ! np)
		return;

	if (np == c->cParent)
		return;

	if (! hasPermission(uSource, c, ChanACL::Write)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	if (! hasPermission(uSource, np, ChanACL::MakeChannel)) {
		PERM_DENIED(uSource, np, ChanACL::MakeChannel);
		return;
	}

	// Can't move to a subchannel of itself
	Channel *p = np;
	while (p) {
		if (p == c)
			return;
		p = p->cParent;
	}

	log(uSource, "Moved channel %s from %s to %s", qPrintable(c->qsName), qPrintable(c->cParent->qsName), qPrintable(np->qsName));

	c->cParent->removeChannel(c);
	np->addChannel(c);
	updateChannel(c);
	dbus->channelStateChanged(c);
	sendAll(msg);
}

void Server::msgChannelLink(Connection *cCon, MessageChannelLink *msg) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = qhChannels.value(msg->iId);
	if (!c)
		return;

	if (! hasPermission(uSource, c, ChanACL::LinkChannel)) {
		PERM_DENIED(uSource, c, ChanACL::LinkChannel);
		return;
	}

	Channel *l = (msg->qlTargets.count() == 1) ? qhChannels.value(msg->qlTargets[0]) : NULL;

	switch (msg->ltType) {
		case MessageChannelLink::Link:
			if (!l)
				return;
			if (! hasPermission(uSource, l, ChanACL::LinkChannel)) {
				PERM_DENIED(uSource, l, ChanACL::LinkChannel);
				return;
			}
			break;
		case MessageChannelLink::Unlink:
			if (!l)
				return;
			break;
		case MessageChannelLink::UnlinkAll:
			if (msg->qlTargets.count() > 0)
				return;
			break;
		default:
			if (msg->qlTargets.count() <= 0)
				return;
	}

	QSet<Channel *> oldset = c->qhLinks.keys().toSet();

	switch (msg->ltType) {
		case MessageChannelLink::UnlinkAll:
			c->unlink(NULL);
			removeLink(c, NULL);
			dbus->channelStateChanged(c);
			log(uSource, "Unlinked all from channel %s", qPrintable(c->qsName));
			sendAll(msg);
			return;
		case MessageChannelLink::Link:
			c->link(l);
			addLink(c, l);
			dbus->channelStateChanged(c);
			log(uSource, "Linked channel %s to %s", qPrintable(c->qsName),qPrintable(l->qsName));
			break;
		case MessageChannelLink::Unlink:
			c->unlink(l);
			removeLink(c, l);
			dbus->channelStateChanged(c);
			log(uSource, "Unlinked channel %s from %s", qPrintable(c->qsName), qPrintable(l->qsName));
			break;
		case MessageChannelLink::PushLink:
			foreach(int tid, msg->qlTargets) {
				l=qhChannels.value(tid);
				if (l && hasPermission(uSource, l, ChanACL::LinkChannel))
					c->playerLink(l, uSource);
			}
			break;
		case MessageChannelLink::PushUnlink:
			foreach(int tid, msg->qlTargets) {
				l=qhChannels.value(tid);
				if (l)
					c->playerUnlink(l, uSource);
			}
			break;
	}

	QSet<Channel *> newset = c->qhLinks.keys().toSet();
	QSet<Channel *> changed;

	if (newset.count() == oldset.count())
		return;

	MessageChannelLink mcl;
	mcl.iId = msg->iId;
	mcl.uiSession=msg->uiSession;

	if (newset.count() > oldset.count()) {
		mcl.ltType = MessageChannelLink::Link;
		changed = newset - oldset;
	} else {
		mcl.ltType = MessageChannelLink::Unlink;
		changed = oldset - newset;
	}
	foreach(l, changed)
	mcl.qlTargets << l->iId;
	sendAll(&mcl);
}

void Server::msgTextMessage(Connection *cCon, MessageTextMessage *msg) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;
	sendMessage(pDstUser, msg);
}

void Server::msgEditACL(Connection *cCon, MessageEditACL *msg) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = qhChannels.value(msg->iId);
	if (!c)
		return;

	if (! hasPermission(uSource, c, ChanACL::Write)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	MessageEditACL mea;
	mea.iId = msg->iId;

	if (msg->bQuery) {
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

		while (! chans.isEmpty()) {
			p = chans.pop();
			foreach(acl, p->qlACL) {
				if ((p == c) || (acl->bApplySubs)) {
					MessageEditACL::ACLStruct as;
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
			MessageEditACL::GroupStruct gs;
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
		sendMessage(cCon, &mea);
	} else {
		Group *g;
		ChanACL *a;
		MessageEditACL::GroupStruct gs;
		MessageEditACL::ACLStruct as;

		QHash<QString, QSet<int> > hOldTemp;

		foreach(g, c->qhGroups) {
			hOldTemp.insert(g->qsName, g->qsTemporary);
			delete g;
		}

		foreach(a, c->qlACL)
		delete a;

		c->qhGroups.clear();
		c->qlACL.clear();

		c->bInheritACL = msg->bInheritACL;

		foreach(gs, msg->groups) {
			g = new Group(c, gs.qsName);
			g->bInherit = gs.bInherit;
			g->bInheritable = gs.bInheritable;
			g->qsAdd = gs.qsAdd;
			g->qsRemove = gs.qsRemove;
			g->qsTemporary = hOldTemp.value(gs.qsName);
		}

		foreach(as, msg->acls) {
			a = new ChanACL(c);
			a->bApplyHere=as.bApplyHere;
			a->bApplySubs=as.bApplySubs;
			a->iPlayerId=as.iPlayerId;
			a->qsGroup=as.qsGroup;
			a->pDeny=as.pDeny;
			a->pAllow=as.pAllow;
		}

		clearACLCache();

		if (! hasPermission(uSource, c, ChanACL::Write)) {
			a = new ChanACL(c);
			a->bApplyHere=true;
			a->bApplySubs=false;
			a->iPlayerId=uSource->iId;
			a->pDeny=ChanACL::None;
			a->pAllow=ChanACL::Write | ChanACL::Traverse;

			clearACLCache();
		}

		updateChannel(c);
		log(uSource, "Updated ACL in channel %s[%d]", qPrintable(c->qsName), c->iId);
	}
}

void Server::msgQueryUsers(Connection *cCon, MessageQueryUsers *msg) {
	MSG_SETUP(Player::Authenticated);

	int i;
	for (i=0;i<msg->qlIds.count();i++) {
		QString name = msg->qlNames[i];
		int id = msg->qlIds[i];
		if (id < 0) {
			id = qhUserIDCache.value(name);
			if (! qhUserIDCache.contains(name)) {
				id = getUserID(name);
				if (id >= 0)
					qhUserIDCache[name] = id;
			}
			msg->qlIds[i] = (id >= 0) ? id : -1;
		} else {
			if (! qhUserNameCache.contains(id)) {
				name = getUserName(id);
				if (! name.isEmpty())
					qhUserNameCache[id] = name;
			}
			msg->qlNames[i] = qhUserNameCache.value(id);
		}
	}
	// Check if session is alive.
	if (! qhUsers.contains(msg->uiSession))
		return;

	sendMessage(cCon, msg);
}

void Server::msgPing(Connection *cCon, MessagePing *msg) {
	MSG_SETUP(Player::Authenticated);
	sendMessage(cCon, msg);
}

void Server::msgPingStats(Connection *cCon, MessagePingStats *msg) {
	MSG_SETUP(Player::Authenticated);
	CryptState &cs=uSource->csCrypt;
	cs.uiRemoteGood = msg->uiGood;
	cs.uiRemoteLate = msg->uiLate;
	cs.uiRemoteLost = msg->uiLost;
	cs.uiRemoteResync = msg->uiResync;

	uSource->dUDPPingAvg = msg->dUDPPingAvg;
	uSource->dUDPPingVar = msg->dUDPPingVar;
	uSource->uiUDPPackets = msg->uiUDPPackets;
	uSource->dTCPPingAvg = msg->dTCPPingAvg;
	uSource->dTCPPingVar = msg->dTCPPingVar;
	uSource->uiTCPPackets = msg->uiTCPPackets;

	msg->uiGood = cs.uiGood;
	msg->uiLate = cs.uiLate;
	msg->uiLost = cs.uiLost;
	msg->uiResync = cs.uiResync;

	msg->dUDPPingAvg = msg->dUDPPingVar = msg->dTCPPingAvg = msg->dTCPPingVar = 0.0L;
	msg->uiUDPPackets = msg->uiTCPPackets = 0;
	sendMessage(cCon, msg);
}

void Server::msgTexture(Connection *cCon, MessageTexture *msg) {
	MSG_SETUP(Player::Authenticated);
	if (! qhUserTextureCache.contains(msg->iPlayerId)) {
		QByteArray qba = getUserTexture(msg->iPlayerId);
		if (! qba.isEmpty()) {
			qba = qCompress(qba);
		}
		qhUserTextureCache.insert(msg->iPlayerId, qba);
	}

	// Check if session is alive.
	if (! qhUsers.contains(msg->uiSession))
		return;

	msg->qbaTexture = qhUserTextureCache.value(msg->iPlayerId);
	if (! msg->qbaTexture.isEmpty())
		sendMessage(cCon, msg);
}

void Server::msgCryptSync(Connection *cCon, MessageCryptSync *msg) {
	MSG_SETUP(Player::Authenticated);
	if (msg->qbaNonce.isEmpty()) {
		log(uSource, "Requested crypt-nonce resync");
		msg->qbaNonce = QByteArray(reinterpret_cast<const char *>(uSource->csCrypt.encrypt_iv), AES_BLOCK_SIZE);
		sendMessage(cCon, msg);
	} else if (msg->qbaNonce.size() == AES_BLOCK_SIZE) {
		uSource->csCrypt.uiResync++;
		memcpy(uSource->csCrypt.decrypt_iv, msg->qbaNonce.constData(), AES_BLOCK_SIZE);
	} else {
		cCon->disconnectSocket();
	}
}
