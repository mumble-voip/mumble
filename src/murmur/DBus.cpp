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

#include "Server.h"
#include "ServerDB.h"
#include "Connection.h"
#include "Message.h"
#include "DBus.h"

QDBusArgument &operator<<(QDBusArgument &a, const PlayerInfo &s) {
	a.beginStructure();
	a << s.session << s.mute << s.deaf << s.suppressed << s.selfMute << s.selfDeaf << s.channel;
	a.endStructure();
	return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, PlayerInfo &s) {
	a.beginStructure();
	a >> s.session >> s.mute >> s.deaf >> s.suppressed >> s.selfMute >> s.selfDeaf >> s.channel;
	a.endStructure();
	return a;
}

QDBusArgument &operator<<(QDBusArgument &a, const PlayerInfoExtended &s) {
	a.beginStructure();
	a << s.session << s.mute << s.deaf << s.suppressed << s.selfMute << s.selfDeaf << s.channel << s.id << s.name << s.onlinesecs << s.bytespersec;
	a.endStructure();
	return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, PlayerInfoExtended &s) {
	a.beginStructure();
	a >> s.session >> s.mute >> s.deaf >> s.suppressed >> s.selfMute >> s.selfDeaf >> s.channel >> s.id >> s.name >> s.onlinesecs >> s.bytespersec;
	a.endStructure();
	return a;
}

QDBusArgument &operator<<(QDBusArgument &a, const ChannelInfo &s) {
	a.beginStructure();
	a << s.id << s.name << s.parent << s.links;
	a.endStructure();
	return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, ChannelInfo &s) {
	a.beginStructure();
	a >> s.id >> s.name >> s.parent >> s.links;
	a.endStructure();
	return a;
}

QDBusArgument &operator<<(QDBusArgument &a, const GroupInfo &s) {
	a.beginStructure();
	a << s.name << s.inherited << s.inherit << s.inheritable;
	a << s.add << s.remove << s.members;
	a.endStructure();
	return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, GroupInfo &s) {
	a.beginStructure();
	a >> s.name >> s.inherited >> s.inherit >> s.inheritable;
	a >> s.add >> s.remove >> s.members;
	a.endStructure();
	return a;
}

QDBusArgument &operator<<(QDBusArgument &a, const ACLInfo &s) {
	a.beginStructure();
	a << s.applyHere << s.applySubs << s.inherited;
	a << s.playerid << s.group;
	a << s.allow << s.deny;
	a.endStructure();
	return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, ACLInfo &s) {
	a.beginStructure();
	a >> s.applyHere >> s.applySubs >> s.inherited;
	a >> s.playerid >> s.group;
	a >> s.allow >> s.deny;
	a.endStructure();
	return a;
}

QDBusArgument &operator<<(QDBusArgument &a, const BanInfo &s) {
	a.beginStructure();
	a << s.address << s.bits;
	a.endStructure();
	return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, BanInfo &s) {
	a.beginStructure();
	a >> s.address >> s.bits;
	a.endStructure();
	return a;
}

void MurmurDBus::registerTypes() {
	qDBusRegisterMetaType<PlayerInfo>();
	qDBusRegisterMetaType<PlayerInfoExtended>();
	qDBusRegisterMetaType<QList<PlayerInfoExtended> >();
	qDBusRegisterMetaType<ChannelInfo>();
	qDBusRegisterMetaType<QList<ChannelInfo> >();
	qDBusRegisterMetaType<GroupInfo>();
	qDBusRegisterMetaType<QList<GroupInfo> >();
	qDBusRegisterMetaType<ACLInfo>();
	qDBusRegisterMetaType<QList<ACLInfo> >();
	qDBusRegisterMetaType<BanInfo>();
	qDBusRegisterMetaType<QList<BanInfo> >();
}

MurmurDBus::MurmurDBus(QCoreApplication &app) : QDBusAbstractAdaptor(&app), qdbc(QLatin1String("mainbus")) {
}

QString MurmurDBus::mapIdToName(int id) {
	if (qsAuthPath.isEmpty())
		return QString();

	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<QString> reply = remoteApp.call(QDBus::BlockWithGui, "getUserName",id);
	if (reply.isValid())
		return reply.value();
	else {
		qWarning("Authenticator failed getUserName for %d: %s", id, qPrintable(reply.error().message()));
		qsAuthPath = QString();
		qsAuthService = QString();
		return QString();
	}
}

QByteArray MurmurDBus::mapIdToTexture(int id) {
	if (qsAuthPath.isEmpty())
		return QByteArray();

	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<QByteArray> reply = remoteApp.call(QDBus::BlockWithGui, "getUserTexture",id);
	if (reply.isValid()) {
		return reply.value();
	} else {
		// This is NOT a critical failure, it's an OPTIONAL implementation.
		return QByteArray();
	}
}

int MurmurDBus::mapNameToId(const QString &name) {
	if (qsAuthPath.isEmpty())
		return -2;

	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(QDBus::BlockWithGui, "getUserId",name);
	if (reply.isValid())
		return reply.value();
	else {
		qWarning("Authenticator failed getUserId for %s: %s", qPrintable(name), qPrintable(reply.error().message()));
		qsAuthPath = QString();
		qsAuthService = QString();
		return -2;
	}
}

int MurmurDBus::authenticate(const QString &uname, const QString &pw) {
	if (qsAuthPath.isEmpty())
		return -2;

	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(QDBus::BlockWithGui, "authenticate",uname,pw);
	if (reply.isValid()) {
		qWarning("Authenticate success for %s: %d", qPrintable(uname),reply.value());
		return reply.value();
	} else {
		qWarning("Authenticator failed authenticate for %s: %s", qPrintable(uname), qPrintable(reply.error().message()));
		qsAuthPath = QString();
		qsAuthService = QString();
		return -2;
	}
}

#define PLAYER_SETUP_VAR(var) \
  Player *pPlayer = Player::get(var); \
  if (! pPlayer) { \
    QDBusConnection::sessionBus().send(msg.createErrorReply("net.sourceforge.mumble.Error.session", "Invalid session id")); \
    return; \
  }

#define PLAYER_SETUP PLAYER_SETUP_VAR(session)

#define CHANNEL_SETUP_VAR2(dst,var) \
  Channel *dst = Channel::get(var); \
  if (! dst) { \
    QDBusConnection::sessionBus().send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Invalid channel id")); \
    return; \
  }

#define CHANNEL_SETUP_VAR(var) CHANNEL_SETUP_VAR2(cChannel,var)

void MurmurDBus::getPlayers(QList<PlayerInfoExtended> &a) {
	a.clear();
	foreach(Player *p, g_sServer->qmPlayers) {
		a << PlayerInfoExtended(p);
	}
}

void MurmurDBus::getChannels(QList<ChannelInfo> &a) {
	a.clear();
	QQueue<Channel *> q;
	q << Channel::get(0);
	while (! q.isEmpty()) {
		Channel *c = q.dequeue();
		a << ChannelInfo(c);
		foreach(c, c->qlChannels)
		q.enqueue(c);
	}
}

void MurmurDBus::kickPlayer(unsigned int session, const QString &reason, const QDBusMessage &msg) {
	PLAYER_SETUP;
	Connection *c = g_sServer->qmConnections.value(session);
	if (!c)
		return;

	MessagePlayerKick mpk;
	mpk.uiSession = 0;
	mpk.uiVictim = session;
	mpk.qsReason=reason;
	g_sServer->sendAll(&mpk);
	c->disconnect();
}

void MurmurDBus::getPlayerState(unsigned int session, const QDBusMessage &msg, PlayerInfo &pi) {
	PLAYER_SETUP;
	pi = PlayerInfo(pPlayer);
}

void MurmurDBus::setPlayerState(const PlayerInfo &npi, const QDBusMessage &msg) {
	PLAYER_SETUP_VAR(npi.session);
	CHANNEL_SETUP_VAR(npi.channel);
	PlayerInfo pi(pPlayer);

	bool changed = false;

	bool deaf = npi.deaf;
	bool mute = npi.mute;
	if (deaf)
		mute = true;
	if (! mute)
		deaf = false;

	if ((pi.deaf != deaf) && (deaf || (!deaf && mute))) {
		pPlayer->bDeaf = deaf;
		pPlayer->bMute = mute;
		MessagePlayerDeaf mpd;
		mpd.uiSession = 0;
		mpd.uiVictim=pPlayer->uiSession;
		mpd.bDeaf = deaf;
		g_sServer->sendAll(&mpd);
		changed = true;
	} else if ((pi.deaf != deaf) || (pi.mute != mute)) {
		pPlayer->bDeaf = deaf;
		pPlayer->bMute = mute;

		MessagePlayerMute mpm;
		mpm.uiSession = 0;
		mpm.uiVictim=pPlayer->uiSession;
		mpm.bMute=mute;
		g_sServer->sendAll(&mpm);
		changed = true;
	}

	if (cChannel->iId != pi.channel) {
		g_sServer->playerEnterChannel(pPlayer, cChannel);
		MessagePlayerMove mpm;
		mpm.uiSession = 0;
		mpm.uiVictim = pPlayer->uiSession;
		mpm.iChannelId = cChannel->iId;
		g_sServer->sendAll(&mpm);
		changed = true;
	}

	if (changed)
		emit playerStateChanged(PlayerInfo(pPlayer));
}

void MurmurDBus::addChannel(const QString &name, int chanparent, const QDBusMessage &msg, int &newid) {
	CHANNEL_SETUP_VAR(chanparent);

	Channel *nc = ServerDB::addChannel(cChannel, name);
	ServerDB::updateChannel(nc);
	newid = nc->iId;

	MessageChannelAdd mca;
	mca.uiSession = 0;
	mca.qsName = name;
	mca.iParent = chanparent;
	mca.iId = nc->iId;
	g_sServer->sendAll(&mca);
}

void MurmurDBus::removeChannel(int id, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(id);
	if (!cChannel->cParent) {
		QDBusConnection::sessionBus().send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Invalid channel id"));
		return;
	}
	g_sServer->removeChannel(cChannel, NULL);
}

void MurmurDBus::setChannelState(const ChannelInfo &nci, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(nci.id);
	ChannelInfo ci(cChannel);

	CHANNEL_SETUP_VAR2(cParent, nci.parent);

	bool changed = false;

	QSet<Channel *> newset;
	foreach(int id, nci.links) {
		CHANNEL_SETUP_VAR2(cLink, id);
		newset << cLink;
	}

	if (cParent != cChannel->cParent) {

		Channel *p = cParent;
		while (p) {
			if (p == cChannel) {
				QDBusConnection::sessionBus().send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Moving channel to subchannel"));
				return;
			}
			p = p->cParent;
		}

		cChannel->cParent->removeChannel(cChannel);
		cParent->addChannel(cChannel);
		ServerDB::updateChannel(cChannel);

		MessageChannelMove mcm;
		mcm.uiSession = 0;
		mcm.iId = nci.id;
		mcm.iParent = nci.parent;
		g_sServer->sendAll(&mcm);

		changed = true;
	}

	QSet<Channel *> oldset = cChannel->qsPermLinks;

	if (newset != oldset) {
		// Remove

		foreach(Channel *l, oldset) {
			if (! newset.contains(l)) {
				ServerDB::removeLink(cChannel, l);

				MessageChannelLink mcl;
				mcl.uiSession = 0;
				mcl.iId = nci.id;
				mcl.qlTargets << l->iId;
				mcl.ltType = MessageChannelLink::Unlink;
				g_sServer->sendAll(&mcl);
			}
		}

		// Add
		foreach(Channel *l, newset) {
			if (! oldset.contains(l)) {
				ServerDB::addLink(cChannel, l);

				MessageChannelLink mcl;
				mcl.uiSession = 0;
				mcl.iId = nci.id;
				mcl.qlTargets << l->iId;
				mcl.ltType = MessageChannelLink::Link;
				g_sServer->sendAll(&mcl);
			}
		}

		changed = true;
	}

	if (changed)
		emit channelStateChanged(ChannelInfo(cChannel));
}

void MurmurDBus::getACL(int id, const QDBusMessage &msg, QList<ACLInfo> &acls, QList<GroupInfo> &groups, bool &inherit) {
	CHANNEL_SETUP_VAR(id);

	QStack<Channel *> chans;
	Channel *p;
	ChanACL *acl;

	p = cChannel;
	while (p) {
		chans.push(p);
		if ((p == cChannel) || (p->bInheritACL))
			p = p->cParent;
		else
			p =NULL;
	}

	inherit = cChannel->bInheritACL;

	while (! chans.isEmpty()) {
		p = chans.pop();
		foreach(acl, p->qlACL) {
			if ((p==cChannel) || (acl->bApplySubs)) {
				ACLInfo ai(acl);
				if (p != cChannel)
					ai.inherited = true;
				acls << ai;
			}
		}
	}

	p = cChannel->cParent;
	QSet<QString> allnames = Group::groupNames(cChannel);
	QString name;
	foreach(name, allnames) {
		Group *g = cChannel->qhGroups.value(name);
		Group *pg = p ? Group::getGroup(p, name) : NULL;
		GroupInfo gi(g);
		QSet<int> members;
		if (pg) {
			members = pg->members();
			gi.inherited = pg->bInheritable;
		}
		members+=g->qsAdd;
		members-=g->qsRemove;
		gi.members = members.toList();
		groups << gi;
	}
}

void MurmurDBus::setACL(int id, const QList<ACLInfo> &acls, const QList<GroupInfo> &groups, bool inherit, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(id);

	Group *g;
	ChanACL *a;
	ACLInfo ai;
	GroupInfo gi;

	QHash<QString, QSet<int> > hOldTemp;

	foreach(g, cChannel->qhGroups) {
		hOldTemp.insert(g->qsName, g->qsTemporary);
		delete g;
	}
	foreach(a, cChannel->qlACL)
	delete a;
	cChannel->qhGroups.clear();
	cChannel->qlACL.clear();

	cChannel->bInheritACL = inherit;

	foreach(gi, groups) {
		g = new Group(cChannel, gi.name);
		g->bInherit=gi.inherit;
		g->bInheritable=gi.inheritable;
		g->qsAdd = gi.add.toSet();
		g->qsRemove = gi.remove.toSet();
		g->qsTemporary = hOldTemp.value(gi.name);
	}

	foreach(ai, acls) {
		a = new ChanACL(cChannel);
		a->bApplyHere = ai.applyHere;
		a->bApplySubs = ai.applySubs;
		a->iPlayerId = ai.playerid;
		a->qsGroup = ai.group;
		a->pDeny = static_cast<ChanACL::Permissions>(ai.deny);
		a->pAllow = static_cast<ChanACL::Permissions>(ai.allow);
	}

	ChanACL::clearCache();
	ServerDB::updateChannel(cChannel);
}

void MurmurDBus::getBans(QList<BanInfo> &bi) {
	bi.clear();
	QPair<quint32,int> ban;
	foreach(ban, g_sServer->qlBans) {
		bi << BanInfo(ban);
	}
}

void MurmurDBus::setBans(const QList<BanInfo> &bans, const QDBusMessage &) {
	g_sServer->qlBans.clear();
	foreach(BanInfo bi, bans)
	g_sServer->qlBans << QPair<quint32,int>(bi.address,bi.bits);
	ServerDB::setBans(g_sServer->qlBans);
}

void MurmurDBus::getPlayerNames(const QList<int> &ids, const QDBusMessage &, QList<QString> &names) {
	names.clear();
	foreach(int id, ids) {
		if (! g_sServer->qhUserNameCache.contains(id)) {
			QString name=ServerDB::getUserName(id);
			if (! name.isEmpty()) {
				g_sServer->qhUserNameCache[id]=name;
			}
		}
		names << g_sServer->qhUserNameCache.value(id);
	}
}

void MurmurDBus::getPlayerIds(const QList<QString> &names, const QDBusMessage &, QList<int> &ids) {
	ids.clear();
	foreach(QString name, names) {
		if (! g_sServer->qhUserIDCache.contains(name)) {
			int id=ServerDB::getUserID(name);
			if (id != -1) {
				g_sServer->qhUserIDCache[name]=id;
			}
		}
		ids << g_sServer->qhUserIDCache.value(name);
	}
}

void MurmurDBus::setAuthenticator(const QDBusObjectPath &path, const QDBusMessage &msg) {
	qsAuthPath = path.path();
	qsAuthService = msg.service();
	qWarning("Authenticator set to %s %s",qPrintable(qsAuthService),qPrintable(qsAuthPath));
}

void MurmurDBus::setTemporaryGroups(int channel, int playerid, const QStringList &groups, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(channel);

	Group *g;
	foreach(g, cChannel->qhGroups)
	g->qsTemporary.remove(playerid);

	QString gname;
	foreach(gname, groups) {
		g = cChannel->qhGroups.value(gname);
		if (! g) {
			g = new Group(cChannel, gname);
		}
		g->qsTemporary.insert(playerid);
	}

	ChanACL::clearCache();
}

PlayerInfo::PlayerInfo(Player *p) {
	session = p->uiSession;
	mute = p->bMute;
	deaf = p->bDeaf;
	suppressed = p->bSuppressed;
	selfMute = p->bSelfMute;
	selfDeaf = p->bSelfDeaf;
	channel = p->cChannel->iId;
}

PlayerInfoExtended::PlayerInfoExtended(Player *p) : PlayerInfo(p) {
	id = p->iId;
	name = p->qsName;

	Connection *c = g_sServer->qmConnections[p->uiSession];
	BandwidthRecord *bw= g_sServer->qmBandwidth[c];
	onlinesecs = bw->qtFirst.elapsed() / 1000000LL;

	bytespersec = 0;

	int sincelast = bw->a_qtWhen[bw->iRecNum].elapsed() / 20000LL;
	int todo = N_BANDWIDTH_SLOTS - sincelast;
	if (todo < 0)
		return;

	int sum = 0;
	for (int i=0;i<todo;i++) {
		sum += bw->a_iBW[(bw->iRecNum + N_BANDWIDTH_SLOTS - i) % N_BANDWIDTH_SLOTS];
	}
	bytespersec= (sum * 50) / sincelast;
}

ChannelInfo::ChannelInfo(Channel *c) {
	id = c->iId;
	name = c->qsName;
	parent = c->cParent ? c->cParent->iId : -1;
	foreach(Channel *chn, c->qsPermLinks)
	links << chn->iId;
}

ACLInfo::ACLInfo(ChanACL *acl) {
	applyHere = acl->bApplyHere;
	applySubs = acl->bApplySubs;
	inherited = false;
	playerid = acl->iPlayerId;
	group = acl->qsGroup;
	allow = acl->pDeny;
	deny = acl->pAllow;
}

GroupInfo::GroupInfo(Group *g) {
	name = g->qsName;
	inherit = g->bInherit;
	inheritable = g->bInheritable;
	add = g->qsAdd.toList();
	remove = g->qsRemove.toList();
	members.clear();
}

BanInfo::BanInfo(QPair<quint32,int> b) {
	address = b.first;
	bits = b.second;
}

void MurmurDBus::playerStateChanged(Player *p) {
	emit playerStateChanged(PlayerInfo(p));
}

void MurmurDBus::playerConnected(Player *p) {
	emit playerConnected(PlayerInfo(p));
}

void MurmurDBus::playerDisconnected(Player *p) {
	emit playerDisconnected(PlayerInfo(p));
}

void MurmurDBus::channelStateChanged(Channel *c) {
	emit channelStateChanged(ChannelInfo(c));
}

void MurmurDBus::channelCreated(Channel *c) {
	emit channelCreated(ChannelInfo(c));
}

void MurmurDBus::channelRemoved(Channel *c) {
	emit channelRemoved(ChannelInfo(c));
}
