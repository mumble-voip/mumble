#ifndef __MurmurI_h__
#define __MurmurI_h__

#include <Murmur.h>

namespace Murmur
{

class ServerI : virtual public Server
{
public:

    virtual void isRunning_async(const ::Murmur::AMD_Server_isRunningPtr&,
                                 const Ice::Current&);

    virtual void start_async(const ::Murmur::AMD_Server_startPtr&,
                             const Ice::Current&);

    virtual void stop_async(const ::Murmur::AMD_Server_stopPtr&,
                            const Ice::Current&);

    virtual void delete_async(const ::Murmur::AMD_Server_deletePtr&,
                              const Ice::Current&);

    virtual void id_async(const ::Murmur::AMD_Server_idPtr&,
                          const Ice::Current&);

    virtual void getConf_async(const ::Murmur::AMD_Server_getConfPtr&,
                               const ::std::wstring&,
                               const Ice::Current&);

    virtual void getAllConf_async(const ::Murmur::AMD_Server_getAllConfPtr&,
                                  const Ice::Current&);

    virtual void setConf_async(const ::Murmur::AMD_Server_setConfPtr&,
                               const ::std::wstring&,
                               const ::std::wstring&,
                               const Ice::Current&);

    virtual void setSuperuserPasssword_async(const ::Murmur::AMD_Server_setSuperuserPassswordPtr&,
                                             const ::std::wstring&,
                                             const Ice::Current&);

    virtual void getLog_async(const ::Murmur::AMD_Server_getLogPtr&,
                              ::Ice::Int,
                              ::Ice::Int,
                              const Ice::Current&);

    virtual void getPlayers_async(const ::Murmur::AMD_Server_getPlayersPtr&,
                                  const Ice::Current&);

    virtual void getChannels_async(const ::Murmur::AMD_Server_getChannelsPtr&,
                                   const Ice::Current&);

    virtual void getTree_async(const ::Murmur::AMD_Server_getTreePtr&,
                               const Ice::Current&);

    virtual void getBans_async(const ::Murmur::AMD_Server_getBansPtr&,
                               const Ice::Current&);

    virtual void setBans_async(const ::Murmur::AMD_Server_setBansPtr&,
                               const ::Murmur::BanList&,
                               const Ice::Current&);

    virtual void kickPlayer_async(const ::Murmur::AMD_Server_kickPlayerPtr&,
                                  ::Ice::Int,
                                  const ::std::wstring&,
                                  const Ice::Current&);

    virtual void getState_async(const ::Murmur::AMD_Server_getStatePtr&,
                                ::Ice::Int,
                                const Ice::Current&);

    virtual void setState_async(const ::Murmur::AMD_Server_setStatePtr&,
                                const ::Murmur::Player&,
                                const Ice::Current&);

    virtual void getChannelState_async(const ::Murmur::AMD_Server_getChannelStatePtr&,
                                       ::Ice::Int,
                                       const Ice::Current&);

    virtual void setChannelState_async(const ::Murmur::AMD_Server_setChannelStatePtr&,
                                       const ::Murmur::Channel&,
                                       const Ice::Current&);

    virtual void removeChannel_async(const ::Murmur::AMD_Server_removeChannelPtr&,
                                     ::Ice::Int,
                                     const Ice::Current&);

    virtual void addChannel_async(const ::Murmur::AMD_Server_addChannelPtr&,
                                  const ::std::wstring&,
                                  ::Ice::Int,
                                  const Ice::Current&);

    virtual void getACL_async(const ::Murmur::AMD_Server_getACLPtr&,
                              ::Ice::Int,
                              const Ice::Current&);

    virtual void setACL_async(const ::Murmur::AMD_Server_setACLPtr&,
                              ::Ice::Int,
                              const ::Murmur::ACLList&,
                              const ::Murmur::GroupList&,
                              bool,
                              const Ice::Current&);

    virtual void getPlayerNames_async(const ::Murmur::AMD_Server_getPlayerNamesPtr&,
                                      const ::Murmur::IdList&,
                                      const Ice::Current&);

    virtual void getPlayerIds_async(const ::Murmur::AMD_Server_getPlayerIdsPtr&,
                                    const ::Murmur::NameList&,
                                    const Ice::Current&);

    virtual void registerPlayer_async(const ::Murmur::AMD_Server_registerPlayerPtr&,
                                      const ::std::wstring&,
                                      const Ice::Current&);

    virtual void unregisterPlayer_async(const ::Murmur::AMD_Server_unregisterPlayerPtr&,
                                        ::Ice::Int,
                                        const Ice::Current&);

    virtual void updateregistration_async(const ::Murmur::AMD_Server_updateregistrationPtr&,
                                          const ::Murmur::RegisteredPlayer&,
                                          const Ice::Current&);

    virtual void getRegistration_async(const ::Murmur::AMD_Server_getRegistrationPtr&,
                                       ::Ice::Int,
                                       const Ice::Current&);

    virtual void getRegisteredPlayers_async(const ::Murmur::AMD_Server_getRegisteredPlayersPtr&,
                                            const ::std::wstring&,
                                            const Ice::Current&);

    virtual void verifyPassword_async(const ::Murmur::AMD_Server_verifyPasswordPtr&,
                                      const ::std::wstring&,
                                      const ::std::wstring&,
                                      const Ice::Current&);

    virtual void getTexture_async(const ::Murmur::AMD_Server_getTexturePtr&,
                                  ::Ice::Int,
                                  const Ice::Current&);

    virtual void setTexture_async(const ::Murmur::AMD_Server_setTexturePtr&,
                                  ::Ice::Int,
                                  const ::Murmur::Texture&,
                                  const Ice::Current&);

    virtual void ice_ping(const Ice::Current&) const;
};

class MetaI : virtual public Meta
{
public:

    virtual void getServer_async(const ::Murmur::AMD_Meta_getServerPtr&,
                                 ::Ice::Int,
                                 const Ice::Current&);

    virtual void newServer_async(const ::Murmur::AMD_Meta_newServerPtr&,
                                 const Ice::Current&);

    virtual void getBootedServers_async(const ::Murmur::AMD_Meta_getBootedServersPtr&,
                                        const Ice::Current&);

    virtual void getAllServers_async(const ::Murmur::AMD_Meta_getAllServersPtr&,
                                     const Ice::Current&);

    virtual void getDefaultConf_async(const ::Murmur::AMD_Meta_getDefaultConfPtr&,
                                      const Ice::Current&);
};

}

#endif
