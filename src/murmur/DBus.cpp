// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
# include "win.h"
#endif

#include "DBus.h"

#include "Connection.h"
#include "Message.h"
#include "Server.h"
#include "ServerUser.h"
#include "ServerDB.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QStack>
#include <QtDBus/QDBusConnectionInterface>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusMetaType>
#include <QtDBus/QDBusReply>

#ifdef Q_OS_WIN
# include <winsock2.h>
#else
# include <arpa/inet.h>
#endif

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

QDBusConnection *MurmurDBus::qdbc = NULL;

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
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
	QDBusReply<QString> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getUserName",id);
	if (reply.isValid())
		name = reply.value();
	else {
		server->log(QString("DBus Authenticator failed getUserName for %1: %2").arg(QString::number(id), QString(reply.error().message())));
		removeAuthenticator();
	}
}

void MurmurDBus::idToTextureSlot(QByteArray &qba, int id) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
	QDBusReply<QByteArray> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getUserTexture",id);
	if (reply.isValid()) {
		qba = reply.value();
	}
}

void MurmurDBus::nameToIdSlot(int &id, const QString &name) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getUserId",name);
	if (reply.isValid())
		id = reply.value();
	else {
		server->log(QString("DBus Authenticator failed getUserId for %1: %2").arg(name).arg(reply.error().message()));
		removeAuthenticator();
	}
}

void MurmurDBus::registerUserSlot(int &res, const QMap<int, QString> &info) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "registerPlayer", info.value(ServerDB::User_Name));
	if (reply.isValid()) {
		res = reply.value();
		if ((info.count() > 1) && (res > 0))
			setInfoSlot(res, res, info);
	}
}

void MurmurDBus::unregisterUserSlot(int &res, int id) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "unregisterPlayer", id);
	if (reply.isValid())
		res = reply.value();
}

void MurmurDBus::getRegistrationSlot(int &res, int id, QMap<int, QString> &info) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
	QDBusReply<RegisteredPlayer> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getRegistration", id);
	if (reply.isValid()) {
		const RegisteredPlayer &r = reply.value();
		info.insert(ServerDB::User_Name, r.name);
		if (! r.email.isEmpty())
			info.insert(ServerDB::User_Email, r.email);
		res = 1;
	}
}

void  MurmurDBus::getRegisteredUsersSlot(const QString &filter, QMap<int, QString > &m) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
	QDBusReply<QList<RegisteredPlayer> > reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "getRegisteredPlayers", filter);
	if (reply.isValid()) {
		const QList<RegisteredPlayer> &r = reply.value();
		foreach(const RegisteredPlayer &p, r) {
			m.insert(p.id, p.name);
		}
	}
}

void MurmurDBus::setInfoSlot(int &res, int id, const QMap<int, QString> &info) {
	if (info.contains(ServerDB::User_Name)) {
		QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
		QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "setName",id, info.value(ServerDB::User_Name));
		if (reply.isValid())
			res = reply.value();
	}
	if (info.contains(ServerDB::User_Password)) {
		QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
		QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "setPW",id, info.value(ServerDB::User_Password));
		if (reply.isValid())
			res = reply.value();
	}
	if (info.contains(ServerDB::User_Email)) {
		QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
		QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "setEmail",id, info.value(ServerDB::User_Email));
		if (reply.isValid())
			res = reply.value();
	}
}

void MurmurDBus::setTextureSlot(int &res, int id, const QByteArray &texture) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
	QDBusReply<int> reply = remoteApp.call(bReentrant ? QDBus::BlockWithGui : QDBus::Block, "setTexture",id, texture);
	if (reply.isValid())
		res = reply.value();
}

void MurmurDBus::authenticateSlot(int &res, QString &uname, int sessionId, const QList<QSslCertificate> &, const QString &, bool, const QString &pw) {
	QDBusInterface remoteApp(qsAuthService,qsAuthPath,QString(),*qdbc);
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
			server->setTempGroups(uid, sessionId, NULL, msg.arguments().at(2).toStringList());
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
  ServerUser *pUser = server->qhUsers.value(var); \
  if (! pUser) { \
    qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.session", "Invalid session id")); \
    return; \
  }

#define PLAYER_SETUP PLAYER_SETUP_VAR(session)

#define CHANNEL_SETUP_VAR2(dst,var) \
  Channel *dst = server->qhChannels.value(var); \
  if (! dst) { \
    qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Invalid channel id")); \
    return; \
  }

#define CHANNEL_SETUP_VAR(var) CHANNEL_SETUP_VAR2(cChannel,var)

void MurmurDBus::getPlayers(QList<PlayerInfoExtended> &a) {
	a.clear();
	foreach(ServerUser *p, server->qhUsers) {
		if (static_cast<ServerUser *>(p)->sState == ServerUser::Authenticated)
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

	MumbleProto::UserRemove mpur;
	mpur.set_session(session);
	mpur.set_reason(u8(reason));
	server->sendAll(mpur);
	c->disconnectSocket();
}

void MurmurDBus::getPlayerState(unsigned int session, const QDBusMessage &msg, PlayerInfo &pi) {
	PLAYER_SETUP;
	pi = PlayerInfo(pUser);
}

void MurmurDBus::setPlayerState(const PlayerInfo &npi, const QDBusMessage &msg) {
	PLAYER_SETUP_VAR(npi.session);
	CHANNEL_SETUP_VAR(npi.channel);
	PlayerInfo pi(pUser);

	server->setUserState(pUser, cChannel, npi.mute, npi.deaf, pUser->bPrioritySpeaker, npi.suppressed);
}

void MurmurDBus::sendMessage(unsigned int session, const QString &text, const QDBusMessage &msg) {
	PLAYER_SETUP;

	server->sendTextMessage(NULL, pUser, false, text);
}

void MurmurDBus::sendMessageChannel(int id, bool tree, const QString &text, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(id);

	server->sendTextMessage(cChannel, NULL, tree, text);
}

void MurmurDBus::addChannel(const QString &name, int chanparent, const QDBusMessage &msg, int &newid) {
	CHANNEL_SETUP_VAR(chanparent);

	Channel *nc;

	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		nc = server->addChannel(cChannel, name);
	}

	server->updateChannel(nc);
	newid = nc->iId;

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(nc->iId);
	mpcs.set_parent(cChannel->iId);
	mpcs.set_name(u8(name));
	server->sendAll(mpcs);
}

void MurmurDBus::removeChannel(int id, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(id);
	if (!cChannel->cParent) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Invalid channel id"));
		return;
	}

	QWriteLocker wl(&server->qrwlVoiceThread);
	server->removeChannel(cChannel);
}

void MurmurDBus::getChannelState(int id, const QDBusMessage &msg, ChannelInfo &state) {
	CHANNEL_SETUP_VAR(id);
	state = ChannelInfo(cChannel);
}

void MurmurDBus::setChannelState(const ChannelInfo &nci, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(nci.id);
	CHANNEL_SETUP_VAR2(cParent, nci.parent);

	QSet<Channel *> newset;
	foreach(int id, nci.links) {
		CHANNEL_SETUP_VAR2(cLink, id);
		newset << cLink;
	}

	if (! server->setChannelState(cChannel, cParent, nci.name, newset)) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.channel", "Moving channel to subchannel"));
		return;
	}
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
		a->iUserId = ai.playerid;
		a->qsGroup = ai.group;
		a->pDeny = static_cast<ChanACL::Permissions>(ai.deny) & ChanACL::All;
		a->pAllow = static_cast<ChanACL::Permissions>(ai.allow) & ChanACL::All;
	}

	server->clearACLCache();
	server->updateChannel(cChannel);
}

void MurmurDBus::getBans(QList<BanInfo> &bi) {
	bi.clear();
	foreach(const Ban &b, server->qlBans) {
		if (! b.haAddress.isV6())
			bi << BanInfo(b);
	}
}

void MurmurDBus::setBans(const QList<BanInfo> &, const QDBusMessage &) {
}

void MurmurDBus::getPlayerNames(const QList<int> &ids, const QDBusMessage &, QStringList &names) {
	names.clear();
	foreach(int id, ids) {
		names << server->getUserName(id);
	}
}

void MurmurDBus::getPlayerIds(const QStringList &names, const QDBusMessage &, QList<int> &ids) {
	ids.clear();
	foreach(QString name, names) {
		ids << server->getUserID(name);
	}
}

void MurmurDBus::registerPlayer(const QString &name, const QDBusMessage &msg, int &id) {
	QMap<int, QString> info;
	info.insert(ServerDB::User_Name, name);
	id = server->registerUser(info);
	if (id < 0) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.playername", "Illegal player name"));
		return;
	}
}

void MurmurDBus::unregisterPlayer(int id, const QDBusMessage &msg) {
	if (! server->unregisterUser(id)) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}
}

void MurmurDBus::getRegistration(int id, const QDBusMessage &msg, RegisteredPlayer &user) {
	QMap<int, QString> info = server->getRegistration(id);
	if (info.isEmpty()) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}

	user.id = id;
	user.name = info.value(ServerDB::User_Name);
	user.email = info.value(ServerDB::User_Email);
}

void MurmurDBus::setRegistration(int id, const QString &name, const QString &email, const QString &pw, const QDBusMessage &msg) {
	RegisteredPlayer user;
	user.id = id;
	user.name = name;
	user.email = email;
	user.pw = pw;
	updateRegistration(user, msg);
}

void MurmurDBus::updateRegistration(const RegisteredPlayer &user, const QDBusMessage &msg) {
	QMap<int, QString> info;

	if (! user.name.isEmpty())
		info.insert(ServerDB::User_Name, user.name);

	if (! user.email.isEmpty())
		info.insert(ServerDB::User_Email, user.email);

	if (! user.pw.isEmpty())
		info.insert(ServerDB::User_Password, user.pw);

	if (info.isEmpty() || !server->setInfo(user.id, info)) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}
}

void MurmurDBus::getTexture(int id, const QDBusMessage &msg, QByteArray &texture) {
	if (! server->isUserId(id)) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}
	texture = server->getUserTexture(id);
}

void MurmurDBus::setTexture(int id, const QByteArray &texture, const QDBusMessage &msg) {
	if (! server->isUserId(id)) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}
	if (! server->setTexture(id, texture)) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.texture", "Invalid texture"));
		return;
	}
}

void MurmurDBus::getRegisteredPlayers(const QString &filter, QList<RegisteredPlayer> &users) {
	users.clear();
	QMap<int, QString > l = server->getRegisteredUsers(filter);
	QMap<int, QString >::const_iterator i;
	for (i = l.constBegin(); i != l.constEnd(); ++i) {
		RegisteredPlayer r;
		r.id = i.key();
		r.name = i.value();
		users << r;
	}
}

void MurmurDBus::verifyPassword(int id, const QString &pw, const QDBusMessage &msg, bool &ok) {
	QList<int> ids;
	ids << id;
	QStringList names;
	getPlayerNames(ids, msg, names);

	if ((names.count() != 1) || (names.at(0).isEmpty())) {
		qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.playerid", "Invalid player id"));
		return;
	}

	QString name = names.at(0);
	int nid = server->authenticate(name, pw);
	if (nid == id)
		ok = true;
	else
		ok = false;
}

#ifdef Q_OS_WIN
#undef interface
#endif

void MurmurDBus::setAuthenticator(const QDBusObjectPath &path, bool reentrant, const QDBusMessage &msg) {
	if (! qsAuthPath.isNull() || ! qsAuthService.isNull())
		removeAuthenticator();
	server->connectAuthenticator(this);
	qsAuthPath = path.path();
	qsAuthService = msg.service();

	QDBusConnectionInterface *interface = qdbc->interface();
	QDBusReply<QStringList> names = interface->registeredServiceNames();
	if (names.isValid()) {
		foreach(const QString &name, names.value()) {
			QDBusReply<QString> owner = interface->serviceOwner(name);
			if (owner.isValid() && (owner.value() != name) && (owner.value() == qsAuthService)) {
				qsAuthService = name;
				break;
			}
		}
	}

	bReentrant = reentrant;
	server->log(QString("DBus Authenticator set to %1 %2 (%3)").arg(qsAuthService, qsAuthPath, QString::number(reentrant)));
}

void MurmurDBus::setTemporaryGroups(int channel, int userid, const QStringList &groups, const QDBusMessage &msg) {
	CHANNEL_SETUP_VAR(channel);

	server->setTempGroups(userid, 0, cChannel, groups);
}

PlayerInfo::PlayerInfo(const User *p) {
	session = p->uiSession;
	mute = p->bMute;
	deaf = p->bDeaf;
	suppressed = p->bSuppress;
	selfMute = p->bSelfMute;
	selfDeaf = p->bSelfDeaf;
	channel = p->cChannel->iId;
}

PlayerInfoExtended::PlayerInfoExtended(const User *p) : PlayerInfo(p) {
	id = p->iId;
	name = p->qsName;

	const ServerUser *u = static_cast<const ServerUser *>(p);
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
	playerid = acl->iUserId;
	group = acl->qsGroup;
	allow = acl->pAllow;
	deny = acl->pDeny;
}

GroupInfo::GroupInfo(const Group *g) : inherited(false) {
	name = g->qsName;
	inherit = g->bInherit;
	inheritable = g->bInheritable;
	members.clear();
}

BanInfo::BanInfo(const Ban &b) {
	address = ntohl(b.haAddress.hash[3]);
	bits = b.iMask;
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

void MurmurDBus::userStateChanged(const User *p) {
	emit playerStateChanged(PlayerInfo(p));
}

void MurmurDBus::userTextMessage(const User *, const TextMessage &) {
	// Not implemented for DBus.
}

void MurmurDBus::userConnected(const User *p) {
	emit playerConnected(PlayerInfo(p));
}

void MurmurDBus::userDisconnected(const User *p) {
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
	if (MurmurDBus::qdbc->isConnected())
		MurmurDBus::qdbc->registerObject(QString::fromLatin1("/%1").arg(s->iServerNum), s);

	emit started(s->iServerNum);
}

void MetaDBus::stopped(Server *s) {
	emit stopped(s->iServerNum);
}

void MetaDBus::start(int server_id, const QDBusMessage &msg) {
	if (meta->qhServers.contains(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.booted", "Server already booted"));
	} else if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else if (! meta->boot(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.bootfail", "Booting server failed"));
	}
}

void MetaDBus::stop(int server_id, const QDBusMessage &msg) {
	if (! meta->qhServers.contains(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.booted", "Server not booted"));
	} else {
		meta->kill(server_id);
	}
}

void MetaDBus::newServer(int &server_id) {
	server_id = ServerDB::addServer();
}

void MetaDBus::deleteServer(int server_id, const QDBusMessage &msg) {
	if (meta->qhServers.contains(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.booted", "Server is running"));
	} else if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
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
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		if (key == "key" || key == "passphrase")
			MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.writeonly", "Requested read of write-only field."));
		else
			value = ServerDB::getConf(server_id, key).toString();
	}
}

void MetaDBus::setConf(int server_id, const QString &key, const QString &value, const QDBusMessage &msg) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		ServerDB::setConf(server_id, key, value);
		Server *s = meta->qhServers.value(server_id);
		if (s)
			s->setLiveConf(key, value);
	}
}

void MetaDBus::getAllConf(int server_id, const QDBusMessage &msg, ConfigMap &values) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
	} else {
		values = ServerDB::getAllConf(server_id);

		values.remove("key");
		values.remove("passphrase");
	}
}

void MetaDBus::getLog(int server_id, int min_offset, int max_offset, const QDBusMessage &msg, QList<LogEntry> &entries) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
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

	values.remove("key");
	values.remove("passphrase");
}

void MetaDBus::setSuperUserPassword(int server_id, const QString &pw, const QDBusMessage &msg) {
	if (! ServerDB::serverExists(server_id)) {
		MurmurDBus::qdbc->send(msg.createErrorReply("net.sourceforge.mumble.Error.server", "Invalid server id"));
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
