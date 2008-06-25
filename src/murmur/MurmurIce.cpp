#include <Ice/Ice.h>
#include <IceUtil/IceUtil.h>
#include "Meta.h"
#include "MurmurIce.h"
#include "MurmurI.h"

using namespace std;
using namespace Murmur;

static MurmurIce *mi = NULL;

class MurmurLocker {
  public:
	MurmurLocker() {
		mi->qmEvent.lock();
		mi->count++;
		qApp->postEvent(mi, new QEvent(static_cast<QEvent::Type>(ICE_QEVENT)));
		mi->qwcEvent.wait( & mi->qmEvent);
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

MurmurIce::MurmurIce() {
	communicator = Ice::initialize();
	Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapterWithEndpoints("Murmur", "tcp -p 10000");
	MetaPtr m = new MetaI;
	adapter->add(m, communicator->stringToIdentity("Meta"));
	adapter->activate();
	qWarning("MurmurIce: Running");
}

MurmurIce::~MurmurIce() {
	communicator->waitForShutdown();
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

PlayerList
ServerI::getPlayers(const Ice::Current& current) {
	return PlayerList();
}

ChannelList
ServerI::getChannels(const Ice::Current& current) {
	return ChannelList();
}

void
ServerI::getACL(::Ice::Int channelid,
                ACLList& acls,
                GroupList& groups,
                bool& inherit,
                const Ice::Current& current) {
}

void
ServerI::setACL(::Ice::Int channelid,
                const ACLList& acls,
                const GroupList& groups,
                bool inherit,
                const Ice::Current& current) {
}

BanList
ServerI::getBans(const Ice::Current& current) {
	return BanList();
}

void
ServerI::setBans(const BanList& bans,
                 const Ice::Current& current) {
}

void
ServerI::kickPlayer(::Ice::Int session,
                    const ::std::string& reason,
                    const Ice::Current& current) {
}

::Murmur::Player
ServerI::getState(::Ice::Int session,
                  const Ice::Current& current) {
	return Player();
}

void
ServerI::setState(::Ice::Int session,
                  const ::Murmur::Player& state,
                  const Ice::Current& current) {
}

::Murmur::Channel
ServerI::getChannelState(::Ice::Int channelid,
                         const Ice::Current& current) {
	return Channel();
}

void
ServerI::setChanelState(::Ice::Int channelid,
                        const Channel& state,
                        const Ice::Current& current) {
}

void
ServerI::removeChannel(::Ice::Int channelid,
                       const Ice::Current& current) {
}

::Ice::Int
ServerI::addChannel(const ::std::string& name,
                    ::Ice::Int parent,
                    const Ice::Current& current) {
	return 0;
}

NameList
ServerI::getPlayerNames(const IdList& ids,
                        const Ice::Current& current) {
	return NameList();
}

IdList
ServerI::getPlayerIds(const NameList& names,
                      const Ice::Current& current) {
	return IdList();
}

::Ice::Int
ServerI::registerPlayer(const ::std::string& name,
                        const Ice::Current& current) {
	return 0;
}

void
ServerI::unregisterPlayer(::Ice::Int id,
                          const Ice::Current& current) {
}

void
ServerI::updateregistration(const ::Murmur::RegisteredPlayer& registration,
                            const Ice::Current& current) {
}

::Murmur::RegisteredPlayer
ServerI::getRegistration(::Ice::Int id,
                         const Ice::Current& current) {
	return RegisteredPlayer();
}

RegisteredPlayerList
ServerI::getRegisteredPlayers(const ::std::string& filter,
                              const Ice::Current& current) {
	return RegisteredPlayerList();
}

bool
ServerI::verifyPassword(::Ice::Int id,
                        const ::std::string& pw,
                        const Ice::Current& current) {
	return false;
}

Texture
ServerI::getTexture(::Ice::Int id,
                    const Ice::Current& current) {
	return Texture();
}

void
ServerI::setTexture(::Ice::Int id,
                    const Texture& tex,
                    const Ice::Current& current) {
}

ServerPrx
MetaI::getServer(::Ice::Int id,
                 const Ice::Current& current) {
	return 0;
}

void
MetaI::start(::Ice::Int id,
             const Ice::Current& current) {
}

::Ice::Int
MetaI::newServer(const Ice::Current& current) {
	return 0;
}

void
MetaI::deleteServer(::Ice::Int id,
                    const Ice::Current& current) {
}

ServerList
MetaI::getBootedServers(const Ice::Current& current) {
	MurmurLocker ml;

	const QList<int> servers = meta->qhServers.keys();
	return servers.toVector().toStdVector();
}

ServerList
MetaI::getAllServers(const Ice::Current& current) {
	return ServerList();
}

bool
MetaI::isBooted(::Ice::Int id,
                const Ice::Current& current) {
	return false;
}

::std::string
MetaI::getConf(::Ice::Int id,
               const ::std::string& key,
               const Ice::Current& current) {
	return ::std::string();
}

::Murmur::ConfigMap
MetaI::getAllConf(::Ice::Int id,
                  const Ice::Current& current) {
	return ConfigMap();
}

::Murmur::ConfigMap
MetaI::getDefaultConf(const Ice::Current& current) {
	return ConfigMap();
}

void
MetaI::setConf(::Ice::Int id,
               const ::std::string& key,
               const ::std::string& value,
               const Ice::Current& current) {
}

void
MetaI::setSuperuserPassword(::Ice::Int id,
                            const ::std::string& pw,
                            const Ice::Current& current) {
}

LogList
MetaI::getLog(::Ice::Int id,
              ::Ice::Int min,
              ::Ice::Int max,
              const Ice::Current& current) {
	return LogList();
}
