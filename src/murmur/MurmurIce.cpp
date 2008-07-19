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
Ice::ObjectPtr iopServer;

class MurmurLocker {
	public:
		MurmurLocker() {
			mi->qmEvent.lock();
			mi->count++;
			QCoreApplication::instance()->postEvent(mi, new QEvent(static_cast<QEvent::Type>(ICE_QEVENT)));
			mi->qwcEvent.wait(& mi->qmEvent);
		};

		~MurmurLocker() {
			mi->count--;
			mi->qmEvent.unlock();
		}
};

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
			qWarning("MurmurIce: Endpoint \"%s\" running", qPrintable(QString::fromStdString(ep->toString())));
		}
	} catch (Ice::Exception &e) {
		qWarning("MurmurIce: Initialization failed: %s", qPrintable(QString::fromStdString(e.ice_name())));
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
		qmEvent.lock();
		while (count) {
			qwcEvent.wakeAll();
			qmEvent.unlock();
			qmEvent.lock();
		}
		qmEvent.unlock();
	}
}

Ice::ObjectPtr ServerLocator::locate(const Ice::Current &, Ice::LocalObjectPtr &) {
	return iopServer;
}

static ServerPrx idToProxy(int id, const Ice::ObjectAdapterPtr &adapter) {
	Ice::Identity ident;
	ident.category = "s";
	ident.name = QString::number(id).toStdString();

	return ServerPrx::uncheckedCast(adapter->createProxy(ident));
}

static void logToLog(const ServerDB::LogRecord &r, Murmur::LogEntry &le) {
	le.timestamp = r.first;
	le.txt = r.second.toStdString();
}

static void playerToPlayer(const ::Player *p, Murmur::Player &mp) {
	mp.session = p->uiSession;
	mp.playerid = p->iId;
	mp.name = p->qsName.toStdString();
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
	mc.name = c->qsName.toStdString();
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
	ma.group = acl->qsGroup.toStdString();
	ma.allow = acl->pAllow;
	ma.deny = acl->pDeny;
}

static void groupToGroup(const ::Group *g, Murmur::Group &mg) {
	mg.name = g->qsName.toStdString();
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
	int server_id = QString::fromStdString(current.id.name).toInt(); \
	MurmurLocker ml; \
	::Server *server = meta->qhServers.value(server_id);

#define NEED_SERVER_EXISTS \
	FIND_SERVER \
	if (!server && ! ServerDB::serverExists(server_id)) \
		throw ::Ice::ObjectNotExistException(__FILE__,__LINE__);

#define NEED_SERVER \
	NEED_SERVER_EXISTS \
	if (! server) \
		throw ::Murmur::ServerBootedException();

#define NEED_PLAYER \
	User *user = server->qhUsers.value(session); \
	if (!user) \
		throw ::Murmur::InvalidSessionException();

#define NEED_CHANNEL_VAR(x,y) \
	x = server->qhChannels.value(y); \
	if (!x) \
		throw ::Murmur::InvalidChannelException();

#define NEED_CHANNEL \
	::Channel *channel; \
	NEED_CHANNEL_VAR(channel, channelid);

void ServerI::ice_ping(const Ice::Current &current) const {
	NEED_SERVER_EXISTS;
	qWarning("Ping?");
}

bool ServerI::isRunning(const Ice::Current& current) {
	NEED_SERVER_EXISTS;
	return (server != NULL);
}

void ServerI::start(const Ice::Current& current) {
	NEED_SERVER_EXISTS;
	if (server)
		throw ServerBootedException();
	if (! meta->boot(server_id))
		throw ServerFailureException();
}

void ServerI::stop(const Ice::Current& current) {
	NEED_SERVER;
	meta->kill(server_id);
}

void ServerI::_cpp_delete(const Ice::Current& current) {
	NEED_SERVER_EXISTS;
	if (server)
		throw ServerBootedException();
	ServerDB::deleteServer(server_id);
}

int ServerI::id(const Ice::Current& current) {
	NEED_SERVER_EXISTS;
	return server_id;
}

string ServerI::getConf(const string& key, const Ice::Current& current) {
	NEED_SERVER_EXISTS;
	return ServerDB::getConf(server_id, QString::fromStdString(key)).toString().toStdString();
}

::Murmur::ConfigMap ServerI::getAllConf(const Ice::Current& current) {
	NEED_SERVER_EXISTS;

	::Murmur::ConfigMap cm;

	QMap<QString, QString> values = ServerDB::getAllConf(server_id);
	QMap<QString, QString>::const_iterator i;
	for (i=values.constBegin();i != values.constEnd(); ++i) {
		cm[i.key().toStdString()] = i.value().toStdString();
	}
	return cm;
}

void ServerI::setConf(const string& key, const string& value, const Ice::Current& current) {
	NEED_SERVER_EXISTS;
	QString k=QString::fromStdString(key);
	QString v=QString::fromStdString(value);
	ServerDB::setConf(server_id, k, v);
	if (server)
		server->setLiveConf(k, v);
}

void ServerI::setSuperuserPasssword(const string& pw, const Ice::Current& current) {
	NEED_SERVER_EXISTS;
	ServerDB::setSUPW(server_id, QString::fromStdString(pw));
}

::Murmur::LogList ServerI::getLog(int min, int max, const Ice::Current& current) {
	NEED_SERVER_EXISTS;

	::Murmur::LogEntry le;
	::Murmur::LogList ll;

	QList<ServerDB::LogRecord> dblog = ServerDB::getLog(server_id, min, max);
	foreach(const ServerDB::LogRecord &e, dblog) {
		logToLog(e, le);
		ll.push_back(le);
	}
	return ll;
}

::Murmur::PlayerMap ServerI::getPlayers(const Ice::Current& current) {
	NEED_SERVER;
	::Murmur::PlayerMap pm;
	::Murmur::Player mp;
	foreach(const ::Player *p, server->qhUsers) {
		if (p->sState == ::Player::Authenticated) {
			playerToPlayer(p, mp);
			pm[p->uiSession] = mp;
		}
	}
	return pm;
}

::Murmur::ChannelMap ServerI::getChannels(const Ice::Current& current) {
	NEED_SERVER;
	::Murmur::ChannelMap cm;
	::Murmur::Channel mc;
	foreach(const ::Channel *c, server->qhChannels) {
		channelToChannel(c, mc);
		cm[c->iId] = mc;
	}
	return cm;
}

bool playerSort(const ::Player *a, const ::Player *b) {
	return a->qsName < b->qsName;
}

bool channelSort(const ::Channel *a, const ::Channel *b) {
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

::Murmur::TreePtr ServerI::getTree(const Ice::Current& current) {
	NEED_SERVER;
	return recurseTree(server->qhChannels.value(0));
}

::Murmur::BanList ServerI::getBans(const Ice::Current& current) {
	NEED_SERVER;
	::Murmur::BanList bl;
	::Murmur::Ban mb;
	QPair<quint32, int> ban;
	foreach(ban, server->qlBans) {
		banToBan(ban, mb);
		bl.push_back(mb);
	}
	return bl;
}

void ServerI::setBans(const ::Murmur::BanList& bans, const Ice::Current& current) {
	NEED_SERVER;
	server->qlBans.clear();
	foreach(const ::Murmur::Ban &mb, bans) {
		server->qlBans << QPair<quint32,int>(mb.address,mb.bits);
	}
	server->saveBans();
}

void ServerI::kickPlayer(int session, const string& reason, const Ice::Current& current) {
	NEED_SERVER;
	NEED_PLAYER;

	MessagePlayerKick mpk;
	mpk.uiSession = 0;
	mpk.uiVictim = session;
	mpk.qsReason = QString::fromStdString(reason);
	server->sendAll(&mpk);
	user->disconnectSocket();
}

::Murmur::Player ServerI::getState(int session, const Ice::Current& current) {
	NEED_SERVER;
	NEED_PLAYER;

	::Murmur::Player mp;
	playerToPlayer(user, mp);
	return mp;
}

void ServerI::setState(const ::Murmur::Player& state, const Ice::Current& current) {
	int session = state.session;
	::Channel *channel;
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL_VAR(channel, state.channel);

	server->setPlayerState(user, channel, state.mute, state.deaf, state.suppressed);
}

::Murmur::Channel ServerI::getChannelState(int channelid, const Ice::Current& current) {
	NEED_SERVER;
	NEED_CHANNEL;

	::Murmur::Channel mc;
	channelToChannel(channel, mc);
	return mc;
}

void ServerI::setChannelState(const ::Murmur::Channel& state, const Ice::Current& current) {
	int channelid = state.id;
	NEED_SERVER;
	NEED_CHANNEL;
	::Channel *np;
	NEED_CHANNEL_VAR(np, state.parent);

	QSet< ::Channel *> newset;
	foreach(int linkid, state.links) {
		::Channel *cLink;
		NEED_CHANNEL_VAR(cLink, linkid);
		newset << cLink;
	}

	if (! server->setChannelState(channel, np, newset))
		throw ::Murmur::InvalidChannelException();
}

void ServerI::removeChannel(int channelid, const Ice::Current& current) {
	NEED_SERVER;
	NEED_CHANNEL;
	server->removeChannel(channel, NULL);
}

int ServerI::addChannel(const string& name, int parent, const Ice::Current& current) {
	NEED_SERVER;
	::Channel *p, *nc;
	NEED_CHANNEL_VAR(p, parent);

	QString qsName = QString::fromStdString(name);

	nc = server->addChannel(p, qsName);
	server->updateChannel(nc);
	int newid = nc->iId;

	MessageChannelAdd mca;
	mca.uiSession = 0;
	mca.qsName = qsName;
	mca.iParent = parent;
	mca.iId = newid;
	server->sendAll(&mca);

	return newid;
}

void ServerI::getACL(int channelid, ::Murmur::ACLList& acls, ::Murmur::GroupList& groups, bool& inherit, const Ice::Current& current) {
	NEED_SERVER;
	NEED_CHANNEL;

	acls.clear();
	groups.clear();

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

	inherit = channel->bInheritACL;

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
}

void ServerI::setACL(int channelid, const ::Murmur::ACLList& acls, const ::Murmur::GroupList& groups, bool inherit, const Ice::Current& current) {
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
		QString name = QString::fromStdString(gi.name);
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
		acl->qsGroup = QString::fromStdString(ai.group);
		acl->pDeny = static_cast<ChanACL::Permissions>(ai.deny);
		acl->pAllow = static_cast<ChanACL::Permissions>(ai.allow);
	}
	server->clearACLCache();
	server->updateChannel(channel);
}

::Murmur::NameMap ServerI::getPlayerNames(const ::Murmur::IdList& ids, const Ice::Current& current) {
	NEED_SERVER;
	::Murmur::NameMap nm;
	foreach(int playerid, ids) {
		if (! server->qhUserNameCache.contains(playerid)) {
			QString name=server->getUserName(playerid);
			if (! name.isEmpty())
				server->qhUserNameCache.insert(playerid, name);
		}
		nm[playerid] = server->qhUserNameCache.value(playerid).toStdString();
	}
	return nm;
}

::Murmur::IdMap ServerI::getPlayerIds(const ::Murmur::NameList& names, const Ice::Current& current) {
	NEED_SERVER;
	::Murmur::IdMap im;
	foreach(const string &n, names) {
		QString name = QString::fromStdString(n);
		if (! server->qhUserIDCache.contains(name)) {
			int playerid = server->getUserID(name);
			if (playerid != -1)
				server->qhUserIDCache.insert(name, playerid);
		}
		im[n] = server->qhUserIDCache.value(name);
	}
	return im;
}

int ServerI::registerPlayer(const string& name, const Ice::Current& current) {
	NEED_SERVER;
	int playerid = server->registerPlayer(QString::fromStdString(name));
	if (playerid < 0)
		throw InvalidPlayerException();
	return playerid;
}

void ServerI::unregisterPlayer(int playerid, const Ice::Current& current) {
	NEED_SERVER;
	if (! server->unregisterPlayer(playerid))
		throw InvalidPlayerException();
}

void ServerI::updateregistration(const ::Murmur::RegisteredPlayer& registration, const Ice::Current& current) {
	NEED_SERVER;

	QString name, email;
	if (! server->getRegistration(registration.playerid, name, email))
		throw InvalidPlayerException();

	QString newname = QString::fromStdString(registration.name);
	QString newemail = QString::fromStdString(registration.email);
	QString newpw = QString::fromStdString(registration.pw);

	if ((! newname.isEmpty()) && (newname != name))
		if (! server->setName(registration.playerid, newname))
			throw InvalidPlayerException();

	if ((! newemail.isEmpty()) && (newemail != email))
		server->setEmail(registration.playerid, newemail);

	if ((! newpw.isEmpty()))
		server->setPW(registration.playerid, newpw);
}

::Murmur::RegisteredPlayer ServerI::getRegistration(int playerid, const Ice::Current& current) {
	NEED_SERVER;

	QString name, email;
	if (! server->getRegistration(playerid, name, email))
		throw InvalidPlayerException();

	::Murmur::RegisteredPlayer reg;
	reg.playerid = playerid;
	reg.name = name.toStdString();
	reg.email = email.toStdString();
	return reg;
}

::Murmur::RegisteredPlayerList ServerI::getRegisteredPlayers(const string& filter, const Ice::Current& current) {
	NEED_SERVER;
	Murmur::RegisteredPlayerList rpl;
	Murmur::RegisteredPlayer reg;

	const QMap<int, QPair<QString, QString> > l = server->getRegisteredPlayers(QString::fromStdString(filter));
	QMap<int, QPair<QString, QString > >::const_iterator i;
	for(i = l.constBegin(); i != l.constEnd(); ++i) {
		reg.playerid = i.key();
		reg.name = i.value().first.toStdString();
		reg.email = i.value().second.toStdString();
		rpl.push_back(reg);
	}

	return rpl;
}

int ServerI::verifyPassword(const string &name, const string& pw, const Ice::Current& current) {
	NEED_SERVER;
	QString uname = QString::fromStdString(name);
	return server->authenticate(uname, QString::fromStdString(pw));
}

::Murmur::Texture ServerI::getTexture(int playerid, const Ice::Current& current) {
	NEED_SERVER;
	const QByteArray &qba = server->getUserTexture(playerid);

	::Murmur::Texture tex;
	tex.resize(qba.size());
	const char *ptr = qba.constData();
	for(int i=0;i<qba.size();++i)
		tex[i] = ptr[i];

	return tex;
}

void ServerI::setTexture(int playerid, const ::Murmur::Texture& tex, const Ice::Current& current) {
	NEED_SERVER;
	QByteArray qba(tex.size(), 0);
	char *ptr = qba.data();
	for(unsigned int i=0;i<tex.size();++i)
		ptr[i] = tex[i];
	if (! server->setTexture(playerid, qba))
		throw InvalidPlayerException();
}

ServerPrx MetaI::getServer(int id, const Ice::Current&current) {
	MurmurLocker ml;
	QList<int> server_list = ServerDB::getAllServers();
	if (! server_list.contains(id))
		return NULL;
	return idToProxy(id, current.adapter);
}

ServerPrx MetaI::newServer(const Ice::Current& current) {
	MurmurLocker ml;
	return idToProxy(ServerDB::addServer(), current.adapter);
}

::Murmur::ServerList MetaI::getBootedServers(const Ice::Current& current) {
	MurmurLocker ml;

	::Murmur::ServerList sl;

	foreach(int id, meta->qhServers.keys())
	sl.push_back(idToProxy(id, current.adapter));
	return sl;
}

::Murmur::ServerList MetaI::getAllServers(const Ice::Current& current) {
	MurmurLocker ml;

	::Murmur::ServerList sl;

	foreach(int id, ServerDB::getAllServers())
	sl.push_back(idToProxy(id, current.adapter));
	return sl;
}

::Murmur::ConfigMap MetaI::getDefaultConf(const Ice::Current&) {
	::Murmur::ConfigMap cm;
	QMap<QString, QString>::const_iterator i;
	for (i=meta->mp.qmConfig.constBegin();i != meta->mp.qmConfig.constEnd(); ++i) {
		cm[i.key().toStdString()] = i.value().toStdString();
	}
	return cm;
}
