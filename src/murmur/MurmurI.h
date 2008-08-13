#ifndef __MurmurI_h__
#define __MurmurI_h__

#include <Murmur.h>

namespace Murmur {

class ServerI : virtual public Server {
	public:

		virtual bool isRunning(const Ice::Current&);

		virtual void start(const Ice::Current&);

		virtual void stop(const Ice::Current&);

		virtual void _cpp_delete(const Ice::Current&);

		virtual ::Ice::Int id(const Ice::Current&);

		virtual ::std::string getConf(const ::std::string&,
		                              const Ice::Current&);

		virtual ::Murmur::ConfigMap getAllConf(const Ice::Current&);

		virtual void setConf(const ::std::string&,
		                     const ::std::string&,
		                     const Ice::Current&);

		virtual void setSuperuserPasssword(const ::std::string&,
		                                   const Ice::Current&);

		virtual ::Murmur::LogList getLog(::Ice::Int,
		                                 ::Ice::Int,
		                                 const Ice::Current&);

		virtual ::Murmur::PlayerMap getPlayers(const Ice::Current&);

		virtual ::Murmur::ChannelMap getChannels(const Ice::Current&);

		virtual ::Murmur::TreePtr getTree(const Ice::Current&);

		virtual ::Murmur::BanList getBans(const Ice::Current&);

		virtual void setBans(const ::Murmur::BanList&,
		                     const Ice::Current&);

		virtual void kickPlayer(::Ice::Int,
		                        const ::std::string&,
		                        const Ice::Current&);

		virtual ::Murmur::Player getState(::Ice::Int,
		                                  const Ice::Current&);

		virtual void setState(const ::Murmur::Player&,
		                      const Ice::Current&);

		virtual ::Murmur::Channel getChannelState(::Ice::Int,
		        const Ice::Current&);

		virtual void setChannelState(const ::Murmur::Channel&,
		                             const Ice::Current&);

		virtual void removeChannel(::Ice::Int,
		                           const Ice::Current&);

		virtual ::Ice::Int addChannel(const ::std::string&,
		                              ::Ice::Int,
		                              const Ice::Current&);

		virtual void getACL(::Ice::Int,
		                    ::Murmur::ACLList&,
		                    ::Murmur::GroupList&,
		                    bool&,
		                    const Ice::Current&);

		virtual void setACL(::Ice::Int,
		                    const ::Murmur::ACLList&,
		                    const ::Murmur::GroupList&,
		                    bool,
		                    const Ice::Current&);

		virtual ::Murmur::NameMap getPlayerNames(const ::Murmur::IdList&,
		        const Ice::Current&);

		virtual ::Murmur::IdMap getPlayerIds(const ::Murmur::NameList&,
		                                     const Ice::Current&);

		virtual ::Ice::Int registerPlayer(const ::std::string&,
		                                  const Ice::Current&);

		virtual void unregisterPlayer(::Ice::Int,
		                              const Ice::Current&);

		virtual void updateregistration(const ::Murmur::RegisteredPlayer&,
		                                const Ice::Current&);

		virtual ::Murmur::RegisteredPlayer getRegistration(::Ice::Int,
		        const Ice::Current&);

		virtual ::Murmur::RegisteredPlayerList getRegisteredPlayers(const ::std::string&,
		        const Ice::Current&);

		virtual int verifyPassword(const ::std::string&,
		                           const ::std::string&,
		                           const Ice::Current&);

		virtual ::Murmur::Texture getTexture(::Ice::Int,
		                                     const Ice::Current&);

		virtual void setTexture(::Ice::Int,
		                        const ::Murmur::Texture&,
		                        const Ice::Current&);

		virtual void ice_ping(const Ice::Current&) const;
};

class MetaI : virtual public Meta {
	public:

		virtual ::Murmur::ServerPrx getServer(::Ice::Int,
		                                      const Ice::Current&);

		virtual ::Murmur::ServerPrx newServer(const Ice::Current&);

		virtual ::Murmur::ServerList getBootedServers(const Ice::Current&);

		virtual ::Murmur::ServerList getAllServers(const Ice::Current&);

		virtual ::Murmur::ConfigMap getDefaultConf(const Ice::Current&);
};

}

#endif
