/* Copyright (C) 2005-2008, Thorvald Natvig <thorvald@natvig.com>

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


#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "Meta.h"
#include "MurmurIce.h"
#include "MurmurI.h"

using namespace std;
using namespace Murmur;

static MurmurIce *mi = NULL;
static Ice::ObjectPtr iopServer;

class IceEvent : public QEvent {
	protected:
		boost::function<void ()> func;
	public:
		IceEvent(boost::function<void ()> f) : QEvent(static_cast<QEvent::Type>(ICE_QEVENT)), func(f) {};
		void execute() {
			func();
		};
};

static inline QString fromStdUtf8String(const ::std::string &str) {
	return QString::fromUtf8(str.data(), str.length());
}

static inline ::std::string toStdUtf8String(const QString &str) {
	const QByteArray &qba = str.toUtf8();
	return ::std::string(qba.constData(), qba.length());
}

void IceStart() {
	mi = new MurmurIce();
}

void IceStop() {
	if (mi)
		delete mi;
	mi = NULL;
}

class ServerLocator : public virtual Ice::ServantLocator {
	public:
		virtual Ice::ObjectPtr locate(const Ice::Current &, Ice::LocalObjectPtr &);
		virtual void finished(const Ice::Current &, const Ice::ObjectPtr &, const Ice::LocalObjectPtr &) {};
		virtual void deactivate(const std::string &) {};
};

MurmurIce::MurmurIce() {
	count = 0;

	if (meta->mp.qsIceEndpoint.isEmpty())
		return;

	try {
		communicator = Ice::initialize();
		Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("Murmur", qPrintable(meta->mp.qsIceEndpoint));
		MetaPtr m = new MetaI;
		MetaPrx mprx = MetaPrx::uncheckedCast(adapter->add(m, communicator->stringToIdentity("Meta")));
		adapter->addServantLocator(new ServerLocator(), "s");

		iopServer = new ServerI;

		adapter->activate();
		foreach(const Ice::EndpointPtr ep, mprx->ice_getEndpoints()) {
			qWarning("MurmurIce: Endpoint \"%s\" running", qPrintable(fromStdUtf8String(ep->toString())));
		}
	} catch (Ice::Exception &e) {
		qWarning("MurmurIce: Initialization failed: %s", qPrintable(fromStdUtf8String(e.ice_name())));
	}
}

MurmurIce::~MurmurIce() {
	if (communicator) {
		communicator->shutdown();
		communicator->waitForShutdown();
		communicator->destroy();
		communicator=NULL;
		qWarning("MurmurIce: Shutdown complete");
	}
	iopServer = NULL;
}

void MurmurIce::customEvent(QEvent *evt) {
	if (evt->type() == ICE_QEVENT) {
		IceEvent *ie = static_cast<IceEvent *>(evt);
		ie->execute();
	}
}

Ice::ObjectPtr ServerLocator::locate(const Ice::Current &, Ice::LocalObjectPtr &) {
	return iopServer;
}

static ServerPrx idToProxy(int id, const Ice::ObjectAdapterPtr &adapter) {
	Ice::Identity ident;
	ident.category = "s";
	ident.name = toStdUtf8String(QString::number(id));

	return ServerPrx::uncheckedCast(adapter->createProxy(ident));
}

static void logToLog(const ServerDB::LogRecord &r, Murmur::LogEntry &le) {
	le.timestamp = r.first;
	le.txt = toStdUtf8String(r.second);
}

static void playerToPlayer(const ::Player *p, Murmur::Player &mp) {
	mp.session = p->uiSession;
	mp.playerid = p->iId;
	mp.name = toStdUtf8String(p->qsName);
	mp.mute = p->bMute;
	mp.deaf = p->bDeaf;
	mp.suppressed = p->bSuppressed;
	mp.selfMute = p->bSelfMute;
	mp.selfDeaf = p->bSelfDeaf;
	mp.channel = p->cChannel->iId;

	const User *u=static_cast<const User *>(p);
	mp.onlinesecs = u->bwr.onlineSeconds();
	mp.bytespersec = u->bwr.bandwidth();
}

static void channelToChannel(const ::Channel *c, Murmur::Channel &mc) {
	mc.id = c->iId;
	mc.name = toStdUtf8String(c->qsName);
	mc.parent = c->cParent ? c->cParent->iId : -1;
	mc.links.clear();
	foreach(::Channel *chn, c->qsPermLinks)
	mc.links.push_back(chn->iId);
}

static void ACLtoACL(const ::ChanACL *acl, Murmur::ACL &ma) {
	ma.applyHere = acl->bApplyHere;
	ma.applySubs = acl->bApplySubs;
	ma.inherited = false;
	ma.playerid = acl->iPlayerId;
	ma.group = toStdUtf8String(acl->qsGroup);
	ma.allow = acl->pAllow;
	ma.deny = acl->pDeny;
}

static void groupToGroup(const ::Group *g, Murmur::Group &mg) {
	mg.name = toStdUtf8String(g->qsName);
	mg.inherit = g->bInherit;
	mg.inheritable = g->bInheritable;
	mg.add.clear();
	mg.remove.clear();
	mg.members.clear();
}

static void banToBan(const QPair<quint32,int> b, Murmur::Ban &mb) {
	mb.address = b.first;
	mb.bits = b.second;
}

#define FIND_SERVER \
	::Server *server = meta->qhServers.value(server_id);

#define NEED_SERVER_EXISTS \
	FIND_SERVER \
	if (!server && ! ServerDB::serverExists(server_id)) { \
		cb->ice_exception(::Ice::ObjectNotExistException(__FILE__,__LINE__)); \
		return; \
	}

#define NEED_SERVER \
	NEED_SERVER_EXISTS \
	if (! server) { \
		cb->ice_exception(ServerBootedException()); \
		return; \
	}

#define NEED_PLAYER \
	User *user = server->qhUsers.value(session); \
	if (!user) { \
		cb->ice_exception(::Murmur::InvalidSessionException()); \
		return; \
	}

#define NEED_CHANNEL_VAR(x,y) \
	x = server->qhChannels.value(y); \
	if (!x) { \
		cb->ice_exception(::Murmur::InvalidChannelException()); \
		return; \
	}

#define NEED_CHANNEL \
	::Channel *channel; \
	NEED_CHANNEL_VAR(channel, channelid);

void ServerI::ice_ping(const Ice::Current &current) const {
	// This is executed in the ice thread.
	int server_id = fromStdUtf8String(current.id.name).toInt();
	if (! ServerDB::serverExists(server_id))
		throw ::Ice::ObjectNotExistException(__FILE__, __LINE__);
}

static void impl_Server_isRunning(const ::Murmur::AMD_Server_isRunningPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	cb->ice_response(server != NULL);
}

static void impl_Server_start(const ::Murmur::AMD_Server_startPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	if (server)
		cb->ice_exception(ServerBootedException());
	else if (! meta->boot(server_id))
		cb->ice_exception(ServerFailureException());
	else
		cb->ice_response();
}

static void impl_Server_stop(const ::Murmur::AMD_Server_stopPtr cb, int server_id) {
	NEED_SERVER;
	meta->kill(server_id);
	cb->ice_response();
}

static void impl_Server_delete(const ::Murmur::AMD_Server_deletePtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	if (server) {
		cb->ice_exception(ServerBootedException());
		return;
	}
	ServerDB::deleteServer(server_id);
	cb->ice_response();
}

static void impl_Server_id(const ::Murmur::AMD_Server_idPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	cb->ice_response(server_id);
}

static void impl_Server_getConf(const ::Murmur::AMD_Server_getConfPtr cb, int server_id,  const ::std::string& key) {
	NEED_SERVER_EXISTS;
	cb->ice_response(toStdUtf8String(ServerDB::getConf(server_id, fromStdUtf8String(key)).toString()));
}

static void impl_Server_getAllConf(const ::Murmur::AMD_Server_getAllConfPtr cb, int server_id) {
	NEED_SERVER_EXISTS;

	::Murmur::ConfigMap cm;

	QMap<QString, QString> values = ServerDB::getAllConf(server_id);
	QMap<QString, QString>::const_iterator i;
	for (i=values.constBegin();i != values.constEnd(); ++i) {
		cm[toStdUtf8String(i.key())] = toStdUtf8String(i.value());
	}
	cb->ice_response(cm);
}

static void impl_Server_setConf(const ::Murmur::AMD_Server_setConfPtr cb, int server_id,  const ::std::string& key,  const ::std::string& value) {
	NEED_SERVER_EXISTS;
	QString k=fromStdUtf8String(key);
	QString v=fromStdUtf8String(value);
	ServerDB::setConf(server_id, k, v);
	if (server)
		server->setLiveConf(k, v);
	cb->ice_response();
}

static void impl_Server_setSuperuserPasssword(const ::Murmur::AMD_Server_setSuperuserPassswordPtr cb, int server_id,  const ::std::string& pw) {
	NEED_SERVER_EXISTS;
	ServerDB::setSUPW(server_id, fromStdUtf8String(pw));
	cb->ice_response();
}

static void impl_Server_getLog(const ::Murmur::AMD_Server_getLogPtr cb, int server_id,  ::Ice::Int min,  ::Ice::Int max) {
	NEED_SERVER_EXISTS;

	::Murmur::LogEntry le;
	::Murmur::LogList ll;

	QList<ServerDB::LogRecord> dblog = ServerDB::getLog(server_id, min, max);
	foreach(const ServerDB::LogRecord &e, dblog) {
		logToLog(e, le);
		ll.push_back(le);
	}
	cb->ice_response(ll);
}

static void impl_Server_getPlayers(const ::Murmur::AMD_Server_getPlayersPtr cb, int server_id) {
	NEED_SERVER;
	::Murmur::PlayerMap pm;
	::Murmur::Player mp;
	foreach(const ::Player *p, server->qhUsers) {
		if (p->sState == ::Player::Authenticated) {
			playerToPlayer(p, mp);
			pm[p->uiSession] = mp;
		}
	}
	cb->ice_response(pm);
}

static void impl_Server_getChannels(const ::Murmur::AMD_Server_getChannelsPtr cb, int server_id) {
	NEED_SERVER;
	::Murmur::ChannelMap cm;
	::Murmur::Channel mc;
	foreach(const ::Channel *c, server->qhChannels) {
		channelToChannel(c, mc);
		cm[c->iId] = mc;
	}
	cb->ice_response(cm);
}

static bool playerSort(const ::Player *a, const ::Player *b) {
	return a->qsName < b->qsName;
}

static bool channelSort(const ::Channel *a, const ::Channel *b) {
	return a->qsName < b->qsName;
}

TreePtr recurseTree(const ::Channel *c) {
	TreePtr t = new Tree();
	channelToChannel(c, t->c);
	QList< ::Player *> players = c->qlPlayers;
	qSort(players.begin(), players.end(), playerSort);

	foreach(const ::Player *p, players) {
		::Murmur::Player mp;
		playerToPlayer(p, mp);
		t->players.push_back(mp);
	}

	QList< ::Channel *> channels = c->qlChannels;
	qSort(channels.begin(), channels.end(), channelSort);

	foreach(const ::Channel *chn, channels) {
		t->children.push_back(recurseTree(chn));
	}

	return t;
}

static void impl_Server_getTree(const ::Murmur::AMD_Server_getTreePtr cb, int server_id) {
	NEED_SERVER;
	cb->ice_response(recurseTree(server->qhChannels.value(0)));
}

static void impl_Server_getBans(const ::Murmur::AMD_Server_getBansPtr cb, int server_id) {
	NEED_SERVER;
	::Murmur::BanList bl;
	::Murmur::Ban mb;
	QPair<quint32, int> ban;
	foreach(ban, server->qlBans) {
		banToBan(ban, mb);
		bl.push_back(mb);
	}
	cb->ice_response(bl);
}

static void impl_Server_setBans(const ::Murmur::AMD_Server_setBansPtr cb, int server_id,  const ::Murmur::BanList& bans) {
	NEED_SERVER;
	server->qlBans.clear();
	foreach(const ::Murmur::Ban &mb, bans) {
		server->qlBans << QPair<quint32,int>(mb.address,mb.bits);
	}
	server->saveBans();
	cb->ice_response();
}

static void impl_Server_kickPlayer(const ::Murmur::AMD_Server_kickPlayerPtr cb, int server_id,  ::Ice::Int session,  const ::std::string& reason) {
	NEED_SERVER;
	NEED_PLAYER;

	MessagePlayerKick mpk;
	mpk.uiSession = 0;
	mpk.uiVictim = session;
	mpk.qsReason = fromStdUtf8String(reason);
	server->sendAll(&mpk);
	user->disconnectSocket();
	cb->ice_response();
}

static void impl_Server_getState(const ::Murmur::AMD_Server_getStatePtr cb, int server_id,  ::Ice::Int session) {
	NEED_SERVER;
	NEED_PLAYER;

	::Murmur::Player mp;
	playerToPlayer(user, mp);
	cb->ice_response(mp);
}

static void impl_Server_setState(const ::Murmur::AMD_Server_setStatePtr cb, int server_id,  const ::Murmur::Player& state) {
	int session = state.session;
	::Channel *channel;
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL_VAR(channel, state.channel);

	server->setPlayerState(user, channel, state.mute, state.deaf, state.suppressed);
	cb->ice_response();
}

static void impl_Server_getChannelState(const ::Murmur::AMD_Server_getChannelStatePtr cb, int server_id,  ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	::Murmur::Channel mc;
	channelToChannel(channel, mc);
	cb->ice_response(mc);
}

static void impl_Server_setChannelState(const ::Murmur::AMD_Server_setChannelStatePtr cb, int server_id,  const ::Murmur::Channel& state) {
	int channelid = state.id;
	NEED_SERVER;
	NEED_CHANNEL;
	::Channel *np;
	NEED_CHANNEL_VAR(np, state.parent);

	QString qsName = fromStdUtf8String(state.name);

	QSet< ::Channel *> newset;
	foreach(int linkid, state.links) {
		::Channel *cLink;
		NEED_CHANNEL_VAR(cLink, linkid);
		newset << cLink;
	}

	if (! server->setChannelState(channel, np, qsName, newset))
		cb->ice_exception(::Murmur::InvalidChannelException());
	else
		cb->ice_response();
}

static void impl_Server_removeChannel(const ::Murmur::AMD_Server_removeChannelPtr cb, int server_id,  ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;
	server->removeChannel(channel, NULL);
	cb->ice_response();
}

static void impl_Server_addChannel(const ::Murmur::AMD_Server_addChannelPtr cb, int server_id,  const ::std::string& name,  ::Ice::Int parent) {
	NEED_SERVER;
	::Channel *p, *nc;
	NEED_CHANNEL_VAR(p, parent);

	QString qsName = fromStdUtf8String(name);

	nc = server->addChannel(p, qsName);
	server->updateChannel(nc);
	int newid = nc->iId;

	MessageChannelAdd mca;
	mca.uiSession = 0;
	mca.qsName = qsName;
	mca.iParent = parent;
	mca.iId = newid;
	server->sendAll(&mca);

	cb->ice_response(newid);
}

static void impl_Server_getACL(const ::Murmur::AMD_Server_getACLPtr cb, int server_id, ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	::Murmur::ACLList acls;
	::Murmur::GroupList groups;

	QStack< ::Channel *> chans;
	::Channel *p;
	ChanACL *acl;
	p = channel;
	while (p) {
		chans.push(p);
		if ((p == channel) || (p->bInheritACL))
			p = p->cParent;
		else
			p = NULL;
	}

	bool inherit = channel->bInheritACL;

	while (! chans.isEmpty()) {
		p = chans.pop();
		foreach(acl, p->qlACL) {
			if ((p==channel) || (acl->bApplySubs)) {
				::Murmur::ACL ma;
				ACLtoACL(acl, ma);
				if (p != channel)
					ma.inherited = true;
				acls.push_back(ma);
			}
		}
	}

	p = channel->cParent;
	const QSet<QString> allnames = ::Group::groupNames(channel);
	foreach(const QString &name, allnames) {
		::Group *g = channel->qhGroups.value(name);
		::Group *pg = p ? ::Group::getGroup(p, name) : NULL;
		if (!g && ! pg)
			continue;
		::Murmur::Group mg;
		groupToGroup(g ? g : pg, mg);
		QSet<int> members;
		if (pg)
			members = pg->members();
		if (g) {
			mg.add = g->qsAdd.toList().toVector().toStdVector();
			mg.remove = g->qsRemove.toList().toVector().toStdVector();
			mg.inherited = false;
			members += g->qsAdd;
			members -= g->qsRemove;
		} else {
			mg.inherited = true;
		}
		mg.members = members.toList().toVector().toStdVector();
		groups.push_back(mg);
	}
	cb->ice_response(acls, groups, inherit);
}

static void impl_Server_setACL(const ::Murmur::AMD_Server_setACLPtr cb, int server_id,  ::Ice::Int channelid,  const ::Murmur::ACLList& acls,  const ::Murmur::GroupList& groups,  bool inherit) {
	NEED_SERVER;
	NEED_CHANNEL;

	::Group *g;
	ChanACL *acl;

	QHash<QString, QSet<int> > hOldTemp;
	foreach(g, channel->qhGroups) {
		hOldTemp.insert(g->qsName, g->qsTemporary);
		delete g;
	}
	foreach(acl, channel->qlACL)
	delete acl;

	channel->qhGroups.clear();
	channel->qlACL.clear();

	channel->bInheritACL = inherit;
	foreach(const ::Murmur::Group &gi, groups) {
		QString name = fromStdUtf8String(gi.name);
		g = new ::Group(channel, name);
		g->bInherit = gi.inherit;
		g->bInheritable = gi.inheritable;
		g->qsAdd = QVector<int>::fromStdVector(gi.add).toList().toSet();
		g->qsRemove = QVector<int>::fromStdVector(gi.remove).toList().toSet();
		g->qsTemporary = hOldTemp.value(name);
	}
	foreach(const ::Murmur::ACL &ai, acls) {
		acl = new ChanACL(channel);
		acl->bApplyHere = ai.applyHere;
		acl->bApplySubs = ai.applySubs;
		acl->iPlayerId = ai.playerid;
		acl->qsGroup = fromStdUtf8String(ai.group);
		acl->pDeny = static_cast<ChanACL::Permissions>(ai.deny);
		acl->pAllow = static_cast<ChanACL::Permissions>(ai.allow);
	}
	server->clearACLCache();
	server->updateChannel(channel);
	cb->ice_response();
}

static void impl_Server_getPlayerNames(const ::Murmur::AMD_Server_getPlayerNamesPtr cb, int server_id,  const ::Murmur::IdList& ids) {
	NEED_SERVER;
	::Murmur::NameMap nm;
	foreach(int playerid, ids) {
		if (! server->qhUserNameCache.contains(playerid)) {
			QString name=server->getUserName(playerid);
			if (! name.isEmpty())
				server->qhUserNameCache.insert(playerid, name);
		}
		nm[playerid] = toStdUtf8String(server->qhUserNameCache.value(playerid));
	}
	cb->ice_response(nm);
}

static void impl_Server_getPlayerIds(const ::Murmur::AMD_Server_getPlayerIdsPtr cb, int server_id,  const ::Murmur::NameList& names) {
	NEED_SERVER;
	::Murmur::IdMap im;
	foreach(const string &n, names) {
		QString name = fromStdUtf8String(n);
		if (! server->qhUserIDCache.contains(name)) {
			int playerid = server->getUserID(name);
			if (playerid != -1)
				server->qhUserIDCache.insert(name, playerid);
		}
		im[n] = server->qhUserIDCache.value(name);
	}
	cb->ice_response(im);
}

static void impl_Server_registerPlayer(const ::Murmur::AMD_Server_registerPlayerPtr cb, int server_id,  const ::std::string& name) {
	NEED_SERVER;
	int playerid = server->registerPlayer(fromStdUtf8String(name));
	if (playerid < 0)
		cb->ice_exception(InvalidPlayerException());
	else
		cb->ice_response(playerid);
}

static void impl_Server_unregisterPlayer(const ::Murmur::AMD_Server_unregisterPlayerPtr cb, int server_id,  ::Ice::Int playerid) {
	NEED_SERVER;
	if (! server->unregisterPlayer(playerid))
		cb->ice_exception(InvalidPlayerException());
	else
		cb->ice_response();
}

static void impl_Server_updateregistration(const ::Murmur::AMD_Server_updateregistrationPtr cb, int server_id,  const ::Murmur::RegisteredPlayer& registration) {
	NEED_SERVER;

	QString name, email;
	if (! server->getRegistration(registration.playerid, name, email)) {
		cb->ice_exception(InvalidPlayerException());
		return;
	}

	QString newname = fromStdUtf8String(registration.name);
	QString newemail = fromStdUtf8String(registration.email);
	QString newpw = fromStdUtf8String(registration.pw);

	if ((! newname.isEmpty()) && (newname != name))
		if (! server->setName(registration.playerid, newname)) {
			cb->ice_exception(InvalidPlayerException());
			return;
		}

	if ((! newemail.isEmpty()) && (newemail != email))
		server->setEmail(registration.playerid, newemail);

	if ((! newpw.isEmpty()))
		server->setPW(registration.playerid, newpw);

	cb->ice_response();
}

static void impl_Server_getRegistration(const ::Murmur::AMD_Server_getRegistrationPtr cb, int server_id,  ::Ice::Int playerid) {
	NEED_SERVER;

	QString name, email;
	if (! server->getRegistration(playerid, name, email)) {
		cb->ice_exception(InvalidPlayerException());
		return;
	}

	::Murmur::RegisteredPlayer reg;
	reg.playerid = playerid;
	reg.name = toStdUtf8String(name);
	reg.email = toStdUtf8String(email);
	cb->ice_response(reg);
}

static void impl_Server_getRegisteredPlayers(const ::Murmur::AMD_Server_getRegisteredPlayersPtr cb, int server_id,  const ::std::string& filter) {
	NEED_SERVER;
	Murmur::RegisteredPlayerList rpl;
	Murmur::RegisteredPlayer reg;

	const QMap<int, QPair<QString, QString> > l = server->getRegisteredPlayers(fromStdUtf8String(filter));
	QMap<int, QPair<QString, QString > >::const_iterator i;
	for (i = l.constBegin(); i != l.constEnd(); ++i) {
		reg.playerid = i.key();
		reg.name = toStdUtf8String(i.value().first);
		reg.email = toStdUtf8String(i.value().second);
		rpl.push_back(reg);
	}

	cb->ice_response(rpl);
}

static void impl_Server_verifyPassword(const ::Murmur::AMD_Server_verifyPasswordPtr cb, int server_id,  const ::std::string& name,  const ::std::string& pw) {
	NEED_SERVER;
	QString uname = fromStdUtf8String(name);
	cb->ice_response(server->authenticate(uname, fromStdUtf8String(pw)));
}

static void impl_Server_getTexture(const ::Murmur::AMD_Server_getTexturePtr cb, int server_id,  ::Ice::Int playerid) {
	NEED_SERVER;
	const QByteArray &qba = server->getUserTexture(playerid);

	::Murmur::Texture tex;
	tex.resize(qba.size());
	const char *ptr = qba.constData();
	for (int i=0;i<qba.size();++i)
		tex[i] = ptr[i];

	cb->ice_response(tex);
}

static void impl_Server_setTexture(const ::Murmur::AMD_Server_setTexturePtr cb, int server_id,  ::Ice::Int playerid,  const ::Murmur::Texture& tex) {
	NEED_SERVER;
	QByteArray qba(tex.size(), 0);
	char *ptr = qba.data();
	for (unsigned int i=0;i<tex.size();++i)
		ptr[i] = tex[i];
	if (! server->setTexture(playerid, qba))
		cb->ice_exception(InvalidPlayerException());
	else
		cb->ice_response();
}

static void impl_Meta_getServer(const ::Murmur::AMD_Meta_getServerPtr cb, const Ice::ObjectAdapterPtr adapter, ::Ice::Int id) {
	QList<int> server_list = ServerDB::getAllServers();
	if (! server_list.contains(id))
		cb->ice_response(NULL);
	else
		cb->ice_response(idToProxy(id, adapter));
}

static void impl_Meta_newServer(const ::Murmur::AMD_Meta_newServerPtr cb, const Ice::ObjectAdapterPtr adapter) {
	cb->ice_response(idToProxy(ServerDB::addServer(), adapter));
}

static void impl_Meta_getAllServers(const ::Murmur::AMD_Meta_getAllServersPtr cb, const Ice::ObjectAdapterPtr adapter) {
	::Murmur::ServerList sl;

	foreach(int id, ServerDB::getAllServers())
	sl.push_back(idToProxy(id, adapter));
	cb->ice_response(sl);
}

static void impl_Meta_getDefaultConf(const ::Murmur::AMD_Meta_getDefaultConfPtr cb, const Ice::ObjectAdapterPtr) {
	::Murmur::ConfigMap cm;
	QMap<QString, QString>::const_iterator i;
	for (i=meta->mp.qmConfig.constBegin();i != meta->mp.qmConfig.constEnd(); ++i) {
		cm[toStdUtf8String(i.key())] = toStdUtf8String(i.value());
	}
	cb->ice_response(cm);
}

static void impl_Meta_getBootedServers(const ::Murmur::AMD_Meta_getBootedServersPtr cb, const Ice::ObjectAdapterPtr adapter) {
	::Murmur::ServerList sl;

	foreach(int id, meta->qhServers.keys())
	sl.push_back(idToProxy(id, adapter));
	cb->ice_response(sl);
}

#include "MurmurIceWrapper.cpp"
