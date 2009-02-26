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

QDBusArgument &operator<<(QDBusArgument &a, const RegisteredPlayer &s) {
	a.beginStructure();
	a << s.id << s.name << s.email << s.pw;
	a.endStructure();
	return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, RegisteredPlayer &s) {
	a.beginStructure();
	a >> s.id >> s.name >> s.email >> s.pw;
	a.endStructure();
	return a;
}


QDBusArgument &operator<<(QDBusArgument &a, const LogEntry &s) {
	a.beginStructure();
	a << s.timestamp << s.txt;
	a.endStructure();
	return a;
}

const QDBusArgument & operator >>(const QDBusArgument &a, LogEntry &s) {
	a.beginStructure();
	a >> s.timestamp >> s.txt;
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
	qDBusRegisterMetaType<RegisteredPlayer>();
	qDBusRegisterMetaType<QList<RegisteredPlayer> >();
	qDBusRegisterMetaType<ConfigMap>();
	qDBusRegisterMetaType<LogEntry>();
	qDBusRegisterMetaType<QList<LogEntry> >();
}

QDBusConnection MurmurDBus::qdbc(QLatin1String("mainbus"));

MurmurDBus::MurmurDBus(Server *srv) : QDBusAbstractAdaptor(srv) {
	server = srv;
	srv->connectListener(this);
}

void MurmurDBus::removeAuthenticator() {
	server->disconnectAuthenticator(this);
	qsAuthPath = QString();
	qsAuthService = QString();
}


void MurmurDBus::idToNameSlot(QString &name, int id) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<QString> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getUserName",id);
	if (reply.isValid())
		name = reply.value();
	else {
		server->log(QString("DBus Authenticator failed getUserName for %1: %2").arg(id).arg(reply.error().message()));
		removeAuthenticator();
	}
}

void MurmurDBus::idToTextureSlot(QByteArray &qba, int id) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<QByteArray> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getUserTexture",id);
	if (reply.isValid()) {
		qba = reply.value();
	}
}

void MurmurDBus::nameToIdSlot(int &id, const QString &name) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getUserId",name);
	if (reply.isValid())
		id = reply.value();
	else {
		server->log(QString("DBus Authenticator failed getUserId for %1: %2").arg(name).arg(reply.error().message()));
		removeAuthenticator();
	}
}

void MurmurDBus::registerPlayerSlot(int &res, const QString &name) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "registerPlayer", name);
	if (reply.isValid())
		res = reply.value();
}

void MurmurDBus::unregisterPlayerSlot(int &res, int id) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "unregisterPlayer", id);
	if (reply.isValid())
		res = reply.value();
}

void MurmurDBus::getRegistrationSlot(int &res, int id, QString &name, QString &email) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<RegisteredPlayer> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getRegistration", id);
	if (reply.isValid()) {
		const RegisteredPlayer &r = reply.value();
		name = r.name;
		email = r.email;
		res = 1;
	}
}

void  MurmurDBus::getRegisteredPlayersSlot(const QString &filter, QMap<int, QPair<QString, QString> > &m) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<QList<RegisteredPlayer> > reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getRegisteredPlayers", filter);
	if (reply.isValid()) {
		const QList<RegisteredPlayer> &r = reply.value();
		foreach(const RegisteredPlayer &p, r) {
			m.insert(p.id, QPair<QString, QString>(p.name, p.email));
		}
	}
}

void MurmurDBus::setNameSlot(int &res, int id, const QString &name) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "setName",id, name);
	if (reply.isValid())
		res = reply.value();
}

void MurmurDBus::setEmailSlot(int &res, int id, const QString &email) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "setEmail",id, email);
	if (reply.isValid())
		res = reply.value();
}

void MurmurDBus::setPwSlot(int &res, int id, const QString &pw) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "setPW",id, pw);
	if (reply.isValid())
		res = reply.value();
}

void MurmurDBus::setTextureSlot(int &res, int id, const QByteArray &texture) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "setTexture",id, texture);
	if (reply.isValid())
		res = reply.value();
}

void MurmurDBus::authenticateSlot(int &res, QString &uname, const QString &pw) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),qdbc);
	QDBusMessage msg = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "authenticate",uname,pw);
	QDBusError err = msg;
	if (! err.isValid()) {
		QString newname;
		int uid = -2;
		bool ok = true;
		if (msg.arguments().count() >= 1) {
			uid = msg.arguments().at(0).toInt(&ok);
		}
		if (ok && (msg.arguments().count() >= 2)) {
			newname = msg.arguments().at(1).toString();
			if (! newname.isEmpty()) {
				uname = newname;
			}
		}
		if (ok && (msg.arguments().count() >= 3)) {
			server->setTempGroups(uid, NULL, msg.arguments().at(2).toStringList());
		}
		if (ok) {
			server->log(QString("DBus Authenticate success for %1: %2").arg(uname).arg(uid));
			res = uid;
		} else {
			server->log(QString("DBus Autenticator failed authenticate for %1: Invalid return type").arg(uname));
		}
	} else {
		server->log(QString("DBus Authenticator failed authenticate for %1: %2").arg(uname).arg(err.message()));
		removeAuthenticator();
	}
}

#define PLAYER_SETUP_VAR(var) \
  User *pPlayer = server->qhUsers.value(var); \
  if (! pPlayer) { \
    qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.session", "Invalid session id")); \
    return; \
  }

#define PLAYER_SETUP PLAYER_SETUP_VAR(session)

#define CHANNEL_SETUP_VAR2(dst,var) \
  Channel *dst = server->qhChannels.value(var); \
  if (! dst) { \
    qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Invalid channel id")); \
    return; \
  }

#define CHANNEL_SETUP_VAR(var) CHANNEL_SETUP_VAR2(cChannel,var)

void MurmurDBus::getPlayers(QList<PlayerInfoExtended> &a) {
	a.clear();
	foreach(Player *p, server->qhUsers) {
		if (p->sState == Player::Authenticated)
			a << PlayerInfoExtended(p);
	}
}

void MurmurDBus::getChannels(QList<ChannelInfo> &a) {
	a.clear();
	QQueue<Channel *> q;
	q << server->qhChannels.value(0);
	while (! q.isEmpty()) {
		Channel *c = q.dequeue();
		a << ChannelInfo(c);
		foreach(c, c->qlChannels)
			q.enqueue(c);
	}
}

void MurmurDBus::kickPlayer(unsigned int session, const QString &reason, const QDBusMessage &msg) {
	PLAYER_SETUP;
	Connection *c = server->qhUsers.value(session);
	if (!c)
		return;

	MessagePlayerKick mpk;
	mpk.uiSession = 0;
	mpk.uiVictim = session;
	mpk.qsReason=reason;
	server->sendAll(&mpk);
	c->disconnectSocket();
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
		server->sendAll(&mpd);
		changed = true;
	} else if ((pi.deaf != deaf) || (pi.mute != mute)) {
		pPlayer->bDeaf = deaf;
		pPlayer->bMute = mute;

		MessagePlayerMute mpm;
		mpm.uiSession = 0;
		mpm.uiVictim=pPlayer->uiSession;
		mpm.bMute=mute;
		server->sendAll(&mpm);
		changed = true;
	}

	if (cChannel->iId != pi.channel) {
		server->playerEnterChannel(pPlayer, cChannel);
		MessagePlayerMove mpm;
		mpm.uiSession = 0;
		mpm.uiVictim = pPlayer->uiSession;
		mpm.iChannelId = cChannel->iId;
		server->sendAll(&mpm);
		changed = true;
	}

	if (changed)
		emit playerStateChanged(PlayerInfo(pPlayer));
}

void MurmurDBus::sendMessage(unsigned int session, const QString &text, const QDBusMessage &msg) {
	PLAYER_SETUP;

	server->sendTextMessage(NULL, pPlayer, false, text);
}

void MurmurDBus::sendMessageChannel(int id, bool tree, const QString &text, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(id);

	server->sendTextMessage(cChannel, NULL, tree, text);
}

void MurmurDBus::addChannel(const QString &name, int chanparent, const QDBusMessage &msg, int &newid) {
	CHANNEL_SETUP_VAR(chanparent);

	Channel *nc = server->addChannel(cChannel, name);
	server->updateChannel(nc);
	newid = nc->iId;

	MessageChannelAdd mca;
	mca.uiSession = 0;
	mca.qsName = name;
	mca.iParent = chanparent;
	mca.iId = nc->iId;
	server->sendAll(&mca);
}

void MurmurDBus::removeChannel(int id, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(id);
	if (!cChannel->cParent) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Invalid channel id"));
		return;
	}
	server->removeChannel(cChannel, NULL);
}

void MurmurDBus::getChannelState(int id, const QDBusMessage &msg, ChannelInfo &state) {
	CHANNEL_SETUP_VAR(id);
	state = ChannelInfo(cChannel);
}

void MurmurDBus::setChannelState(const ChannelInfo &nci, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(nci.id);
	ChannelInfo ci(cChannel);

	CHANNEL_SETUP_VAR2(cParent, nci.parent);

	bool changed = false;
	bool update = false;

	QSet<Channel *> newset;
	foreach(int id, nci.links) {
		CHANNEL_SETUP_VAR2(cLink, id);
		newset << cLink;
	}

	if (cChannel->qsName != nci.name) {
		cChannel->qsName = nci.name;
		MessageChannelRename mcr;
		mcr.uiSession = 0;
		mcr.iId = cChannel->iId;
		mcr.qsName = nci.name;
		server->sendAll(&mcr);
		update = true;
		changed = true;
	}

	if ((cParent != cChannel) && (cParent != cChannel->cParent)) {

		Channel *p = cParent;
		while (p) {
			if (p == cChannel) {
				qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Moving channel to subchannel"));
				return;
			}
			p = p->cParent;
		}

		cChannel->cParent->removeChannel(cChannel);
		cParent->addChannel(cChannel);

		MessageChannelMove mcm;
		mcm.uiSession = 0;
		mcm.iId = nci.id;
		mcm.iParent = nci.parent;
		server->sendAll(&mcm);

		update = true;
		changed = true;
	}

	QSet<Channel *> oldset = cChannel->qsPermLinks;

	if (newset != oldset) {
		// Remove

		foreach(Channel *l, oldset) {
			if (! newset.contains(l)) {
				server->removeLink(cChannel, l);

				MessageChannelLink mcl;
				mcl.uiSession = 0;
				mcl.iId = nci.id;
				mcl.qlTargets << l->iId;
				mcl.ltType = MessageChannelLink::Unlink;
				server->sendAll(&mcl);
			}
		}

		// Add
		foreach(Channel *l, newset) {
			if (! oldset.contains(l)) {
				server->addLink(cChannel, l);

				MessageChannelLink mcl;
				mcl.uiSession = 0;
				mcl.iId = nci.id;
				mcl.qlTargets << l->iId;
				mcl.ltType = MessageChannelLink::Link;
				server->sendAll(&mcl);
			}
		}

		changed = true;
	}

	if (update)
		server->updateChannel(cChannel);
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
		if (!g && ! pg)
			continue;
		GroupInfo gi(g ? g : pg);
		QSet<int> members;
		if (pg)
			members = pg->members();
		if (g) {
			gi.add = g->qsAdd.toList();
			gi.remove = g->qsRemove.toList();
			gi.inherited = false;
			members+=g->qsAdd;
			members-=g->qsRemove;
		} else {
			gi.inherited = true;
		}
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

	server->clearACLCache();
	server->updateChannel(cChannel);
}

void MurmurDBus::getBans(QList<BanInfo> &bi) {
	bi.clear();
	QPair<quint32,int> ban;
	foreach(ban, server->qlBans) {
		bi << BanInfo(ban);
	}
}

void MurmurDBus::setBans(const QList<BanInfo> &bans, const QDBusMessage &) {
	server->qlBans.clear();
	foreach(BanInfo bi, bans)
		server->qlBans << QPair<quint32,int>(bi.address,bi.bits);
	server->saveBans();
}

void MurmurDBus::getPlayerNames(const QList<int> &ids, const QDBusMessage &, QStringList &names) {
	names.clear();
	foreach(int id, ids) {
		if (! server->qhUserNameCache.contains(id)) {
			QString name=server->getUserName(id);
			if (! name.isEmpty()) {
				server->qhUserNameCache[id]=name;
			}
		}
		names << server->qhUserNameCache.value(id);
	}
}

void MurmurDBus::getPlayerIds(const QStringList &names, const QDBusMessage &, QList<int> &ids) {
	ids.clear();
	foreach(QString name, names) {
		if (! server->qhUserIDCache.contains(name)) {
			int id=server->getUserID(name);
			if (id != -1) {
				server->qhUserIDCache[name]=id;
			}
		}
		ids << server->qhUserIDCache.value(name);
	}
}

void MurmurDBus::registerPlayer(const QString &name, const QDBusMessage &msg, int &id) {
	id = server->registerPlayer(name);
	if (id < 0) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.playername", "Illegal player name"));
		return;
	}
}

void MurmurDBus::unregisterPlayer(int id, const QDBusMessage &msg) {
	if (! server->unregisterPlayer(id)) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}
}

void MurmurDBus::getRegistration(int id, const QDBusMessage &msg, RegisteredPlayer &player) {
	QString name, email;
	if (! server->getRegistration(id, name, email)) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}

	player.id = id;
	player.name = name;
	player.email = email;
}

void MurmurDBus::setRegistration(int id, const QString &name, const QString &email, const QString &pw, const QDBusMessage &msg) {
	RegisteredPlayer player;
	player.id = id;
	player.name = name;
	player.email = email;
	player.pw = pw;
	updateRegistration(player, msg);
}

void MurmurDBus::updateRegistration(const RegisteredPlayer &player, const QDBusMessage &msg) {
	QString name, email;
	if (! server->getRegistration(player.id, name, email)) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}

	if ((! player.name.isEmpty()) && (name != player.name)) {
		if (! server->setName(player.id, player.name)) {
			qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.playername", "Illegal player name"));
			return;
		}
	}

	if ((! player.email.isEmpty()) && (email != player.email)) {
		server->setEmail(player.id, player.email);
	}

	if (! player.pw.isEmpty()) {
		server->setPW(player.id, player.pw);
	}
}

void MurmurDBus::getTexture(int id, const QDBusMessage &msg, QByteArray &texture) {
	if (! server->isPlayerId(id)) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}
	texture = server->getUserTexture(id);
}

void MurmurDBus::setTexture(int id, const QByteArray &texture, const QDBusMessage &msg) {
	if (! server->isPlayerId(id)) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}
	if (! server->setTexture(id, texture)) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.texture", "Invalid texture"));
		return;
	}
}

void MurmurDBus::getRegisteredPlayers(const QString &filter, QList<RegisteredPlayer> &players) {
	players.clear();
	QMap<int, QPair<QString, QString> > l = server->getRegisteredPlayers(filter);
	QMap<int, QPair<QString, QString> >::const_iterator i;
	for (i = l.constBegin(); i != l.constEnd(); ++i) {
		RegisteredPlayer r;
		r.id = i.key();
		r.name = i.value().first;
		r.email = i.value().second;
		players << r;
	}
}

void MurmurDBus::verifyPassword(int id, const QString &pw, const QDBusMessage &msg, bool &ok) {
	QList<int> ids;
	ids << id;
	QStringList names;
	getPlayerNames(ids, msg, names);

	if ((names.count() != 1) || (names.at(0).isEmpty())) {
		qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}

	QString name = names.at(0);
	int nid = server->authenticate(name, pw);
	if (nid == id)
		ok = true;
	else
		ok = false;
}

void MurmurDBus::setAuthenticator(const QDBusObjectPath &path, bool reentrant, const QDBusMessage &msg) {
	if (! qsAuthPath.isNull() || ! qsAuthService.isNull())
		removeAuthenticator();
	server->connectAuthenticator(this);
	qsAuthPath = path.path();
	qsAuthService = msg.service();
	bReentrant = reentrant;
	server->log(QString("DBus Authenticator set to %1 %2 (%3)").arg(qsAuthService).arg(qsAuthPath).arg(reentrant));
}

void MurmurDBus::setTemporaryGroups(int channel, int playerid, const QStringList &groups, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(channel);

	server->setTempGroups(playerid, cChannel, groups);
}

PlayerInfo::PlayerInfo(const Player *p) {
	session = p->uiSession;
	mute = p->bMute;
	deaf = p->bDeaf;
	suppressed = p->bSuppressed;
	selfMute = p->bSelfMute;
	selfDeaf = p->bSelfDeaf;
	channel = p->cChannel->iId;
}

PlayerInfoExtended::PlayerInfoExtended(const Player *p) : PlayerInfo(p) {
	id = p->iId;
	name = p->qsName;

	const User *u = static_cast<const User *>(p);
	onlinesecs = u->bwr.onlineSeconds();
	bytespersec = u->bwr.bandwidth();
}

ChannelInfo::ChannelInfo(const Channel *c) {
	id = c->iId;
	name = c->qsName;
	parent = c->cParent ? c->cParent->iId : -1;
	foreach(Channel *chn, c->qsPermLinks)
		links << chn->iId;
}

ACLInfo::ACLInfo(const ChanACL *acl) {
	applyHere = acl->bApplyHere;
	applySubs = acl->bApplySubs;
	inherited = false;
	playerid = acl->iPlayerId;
	group = acl->qsGroup;
	allow = acl->pAllow;
	deny = acl->pDeny;
}

GroupInfo::GroupInfo(const Group *g) {
	name = g->qsName;
	inherit = g->bInherit;
	inheritable = g->bInheritable;
	members.clear();
}

BanInfo::BanInfo(QPair<quint32,int> b) {
	address = b.first;
	bits = b.second;
}

RegisteredPlayer::RegisteredPlayer() {
	id = -1;
}

LogEntry::LogEntry() {
	timestamp = 0;
}

LogEntry::LogEntry(const ServerDB::LogRecord &r) {
	timestamp = r.first;
	txt = r.second;
}

void MurmurDBus::playerStateChanged(const Player *p) {
	emit playerStateChanged(PlayerInfo(p));
}

void MurmurDBus::playerConnected(const Player *p) {
	emit playerConnected(PlayerInfo(p));
}

void MurmurDBus::playerDisconnected(const Player *p) {
	emit playerDisconnected(PlayerInfo(p));
}

void MurmurDBus::channelStateChanged(const Channel *c) {
	emit channelStateChanged(ChannelInfo(c));
}

void MurmurDBus::channelCreated(const Channel *c) {
	emit channelCreated(ChannelInfo(c));
}

void MurmurDBus::channelRemoved(const Channel *c) {
	emit channelRemoved(ChannelInfo(c));
}


MetaDBus::MetaDBus(Meta *m) : QDBusAbstractAdaptor(m) {
	meta = m;
	m->connectListener(this);
}

void MetaDBus::started(Server *s) {
	new MurmurDBus(s);
	if (MurmurDBus::qdbc.isConnected())
		MurmurDBus::qdbc.registerObject(QString::fromLatin1("/%1").arg(s->iServerNum), s);

	emit started(s->iServerNum);
}

void MetaDBus::stopped(Server *s) {
	emit stopped(s->iServerNum);
}

void MetaDBus::start(int server_id, const QDBusMessage &msg) {
	if (meta->qhServers.contains(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.booted", "Server already booted"));
	} else if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else if (! meta->boot(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.bootfail", "Booting server failed"));
	}
}

void MetaDBus::stop(int server_id, const QDBusMessage &msg) {
	if (! meta->qhServers.contains(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.booted", "Server not booted"));
	} else {
		meta->kill(server_id);
	}
}

void MetaDBus::newServer(int &server_id) {
	server_id = ServerDB::addServer();
}

void MetaDBus::deleteServer(int server_id, const QDBusMessage &msg) {
	if (meta->qhServers.contains(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.booted", "Server is running"));
	} else if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		ServerDB::deleteServer(server_id);
	}
}

void MetaDBus::getBootedServers(QList<int> &server_list) {
	server_list = meta->qhServers.keys();
}

void MetaDBus::getAllServers(QList<int> &server_list) {
	server_list = ServerDB::getAllServers();
}

void MetaDBus::isBooted(int server_id, bool &booted) {
	booted = meta->qhServers.contains(server_id);
}

void MetaDBus::getConf(int server_id, const QString &key, const QDBusMessage &msg, QString &value) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		value = ServerDB::getConf(server_id, key).toString();
	}
}

void MetaDBus::setConf(int server_id, const QString &key, const QString &value, const QDBusMessage &msg) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		ServerDB::setConf(server_id, key, value);
		Server *s = meta->qhServers.value(server_id);
		if (s)
			s->setLiveConf(key, value);
	}
}

void MetaDBus::getAllConf(int server_id, const QDBusMessage &msg, ConfigMap &values) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		values = ServerDB::getAllConf(server_id);
	}
}

void MetaDBus::getLog(int server_id, int min_offset, int max_offset, const QDBusMessage &msg, QList<LogEntry> &entries) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		entries.clear();
		QList<ServerDB::LogRecord> dblog = ServerDB::getLog(server_id, min_offset, max_offset);
		foreach(const ServerDB::LogRecord &e, dblog) {
			entries << LogEntry(e);
		}
	}
}

void MetaDBus::getDefaultConf(ConfigMap &values) {
	values = Meta::mp.qmConfig;
}

void MetaDBus::setSuperUserPassword(int server_id, const QString &pw, const QDBusMessage &msg) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc.send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		ServerDB::setSUPW(server_id, pw);
	}
}

void MetaDBus::quit() {
	qCritical("Quit requested from D-Bus");
	QCoreApplication::instance()->quit();
}

void MetaDBus::getVersion(int &major, int &minor, int &patch, QString &text) {
	Meta::getVersion(major, minor, patch, text);
}
