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
			qApp->postEvent(mi, new QEvent(static_cast<QEvent::Type>(ICE_QEVENT)));
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
	communicator = Ice::initialize();
	Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("Murmur", "tcp -p 10000");
	MetaPtr m = new MetaI;
	adapter->add(m, communicator->stringToIdentity("Meta"));
	adapter->addServantLocator(new ServerLocator(), "s");

	iopServer = new ServerI;

	adapter->activate();
	qWarning("MurmurIce: Running");
}

MurmurIce::~MurmurIce() {
	communicator->waitForShutdown();
	communicator->destroy();
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
	for (i=meta->mp.qmConfig.constBegin();i != meta->mp.qmConfig.constEnd(); ++i) {
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

	::Murmur::Player ps;
	playerToPlayer(user, ps);

	bool changed = false;
	bool deaf = state.deaf;
	bool mute = state.mute;
	if (deaf)
		mute = true;
	if (! mute)
		deaf = false;

	if ((ps.deaf != deaf) && (deaf || (!deaf && mute))) {
		user->bDeaf = deaf;
		user->bMute = mute;
		MessagePlayerDeaf mpd;
		mpd.uiSession = 0;
		mpd.uiVictim=user->uiSession;
		mpd.bDeaf = deaf;
		server->sendAll(&mpd);
		changed = true;
	} else if ((ps.deaf != deaf) || (ps.mute != mute)) {
		user->bDeaf = deaf;
		user->bMute = mute;
		MessagePlayerMute mpm;
		mpm.uiSession = 0;
		mpm.uiVictim=user->uiSession;
		mpm.bMute=mute;
		server->sendAll(&mpm);
		changed = true;
	}

	if (channel->iId != ps.channel) {
		server->playerEnterChannel(user, channel);
		MessagePlayerMove mpm;
		mpm.uiSession = 0;
		mpm.uiVictim = user->uiSession;
		mpm.iChannelId = channel->iId;
		server->sendAll(&mpm);
		changed = true;
	}

	if (changed)
		server->dbus->playerStateChanged(user);
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
	
	bool changed = false;
	
	// TODO
}

void ServerI::removeChannel(int channelid, const Ice::Current& current) {
}

int ServerI::addChannel(const string& name, int parent, const Ice::Current& current) {
	return 0;
}

void ServerI::getACL(int channelid, ::Murmur::ACLList& acls, ::Murmur::GroupList& groups, bool& inherit, const Ice::Current& current) {
}

void ServerI::setACL(int channelid, const ::Murmur::ACLList& acls, const ::Murmur::GroupList& groups, bool inherit, const Ice::Current& current) {
}

::Murmur::NameMap ServerI::getPlayerNames(const ::Murmur::IdList& ids, const Ice::Current& current) {
	return ::Murmur::NameMap();
}

::Murmur::IdMap ServerI::getPlayerIds(const ::Murmur::NameList& names, const Ice::Current& current) {
	return ::Murmur::IdMap();
}

int ServerI::registerPlayer(const string& name, const Ice::Current& current) {
	return 0;
}

void ServerI::unregisterPlayer(int playerid, const Ice::Current& current) {
}

void ServerI::updateregistration(const ::Murmur::RegisteredPlayer& registration, const Ice::Current& current) {
}

::Murmur::RegisteredPlayer ServerI::getRegistration(int playerid, const Ice::Current& current) {
	return ::Murmur::RegisteredPlayer();
}

::Murmur::RegisteredPlayerList ServerI::getRegisteredPlayers(const string& filter, const Ice::Current& current) {
	return ::Murmur::RegisteredPlayerList();
}

bool ServerI::verifyPassword(int playerid, const string& pw, const Ice::Current& current) {
	return false;
}

::Murmur::Texture ServerI::getTexture(int playerid, const Ice::Current& current) {
	return ::Murmur::Texture();
}

void ServerI::setTexture(int playerid, const ::Murmur::Texture& tex, const Ice::Current& current) {
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

::Murmur::ConfigMap MetaI::getDefaultConf(const Ice::Current& current) {
	::Murmur::ConfigMap cm;
	QMap<QString, QString>::const_iterator i;
	for (i=meta->mp.qmConfig.constBegin();i != meta->mp.qmConfig.constEnd(); ++i) {
		cm[i.key().toStdString()] = i.value().toStdString();
	}
	return cm;
}
