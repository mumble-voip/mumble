#ifndef __MurmurI_h__
#define __MurmurI_h__

#include <Murmur.h>

namespace Murmur {

	class ServerI : virtual public Server {
		public:

			virtual void isRunning_async(const ::Murmur::AMD_Server_isRunningPtr&,
			                             const Ice::Current&);

			virtual void start_async(const ::Murmur::AMD_Server_startPtr&,
			                         const Ice::Current&);

			virtual void stop_async(const ::Murmur::AMD_Server_stopPtr&,
			                        const Ice::Current&);

			virtual void delete_async(const ::Murmur::AMD_Server_deletePtr&,
			                          const Ice::Current&);

			virtual void addCallback_async(const ::Murmur::AMD_Server_addCallbackPtr&, const ::Murmur::ServerCallbackPrx&, const ::Ice::Current&);
			virtual void removeCallback_async(const ::Murmur::AMD_Server_removeCallbackPtr&, const ::Murmur::ServerCallbackPrx&, const ::Ice::Current&);

			virtual void setAuthenticator_async(const ::Murmur::AMD_Server_setAuthenticatorPtr&, const ::Murmur::ServerAuthenticatorPrx&, const ::Ice::Current&);

			virtual void id_async(const ::Murmur::AMD_Server_idPtr&,
			                      const Ice::Current&);

			virtual void getConf_async(const ::Murmur::AMD_Server_getConfPtr&,
			                           const ::std::string&,
			                           const Ice::Current&);

			virtual void getAllConf_async(const ::Murmur::AMD_Server_getAllConfPtr&,
			                              const Ice::Current&);

			virtual void setConf_async(const ::Murmur::AMD_Server_setConfPtr&,
			                           const ::std::string&,
			                           const ::std::string&,
			                           const Ice::Current&);

			virtual void setSuperuserPassword_async(const ::Murmur::AMD_Server_setSuperuserPasswordPtr&,
			                                        const ::std::string&,
			                                        const Ice::Current&);

			virtual void getLog_async(const ::Murmur::AMD_Server_getLogPtr&,
			                          ::Ice::Int,
			                          ::Ice::Int,
			                          const Ice::Current&);

			virtual void getLogLen_async(const ::Murmur::AMD_Server_getLogLenPtr&,
			                             const Ice::Current&);

			virtual void getUsers_async(const ::Murmur::AMD_Server_getUsersPtr&,
			                            const Ice::Current&);

			virtual void getChannels_async(const ::Murmur::AMD_Server_getChannelsPtr&,
			                               const Ice::Current&);

			virtual void getTree_async(const ::Murmur::AMD_Server_getTreePtr&,
			                           const Ice::Current&);

			virtual void getCertificateList_async(const ::Murmur::AMD_Server_getCertificateListPtr&,
			                                      ::Ice::Int,
			                                      const ::Ice::Current&);

			virtual void getBans_async(const ::Murmur::AMD_Server_getBansPtr&,
			                           const Ice::Current&);

			virtual void setBans_async(const ::Murmur::AMD_Server_setBansPtr&,
			                           const ::Murmur::BanList&,
			                           const Ice::Current&);

			virtual void kickUser_async(const ::Murmur::AMD_Server_kickUserPtr&,
			                            ::Ice::Int,
			                            const ::std::string&,
			                            const Ice::Current&);

			virtual void sendMessage_async(const ::Murmur::AMD_Server_sendMessagePtr&,
			                               ::Ice::Int,
			                               const ::std::string&,
			                               const Ice::Current&);

			virtual void hasPermission_async(const ::Murmur::AMD_Server_hasPermissionPtr&, ::Ice::Int, ::Ice::Int, ::Ice::Int, const ::Ice::Current&);

			virtual void addContextCallback_async(const ::Murmur::AMD_Server_addContextCallbackPtr&, ::Ice::Int, const ::std::string&, const ::std::string&, const ::Murmur::ServerContextCallbackPrx&, int, const ::Ice::Current&);
			virtual void removeContextCallback_async(const ::Murmur::AMD_Server_removeContextCallbackPtr&, const ::Murmur::ServerContextCallbackPrx&, const ::Ice::Current&);

			virtual void getState_async(const ::Murmur::AMD_Server_getStatePtr&,
			                            ::Ice::Int,
			                            const Ice::Current&);

			virtual void setState_async(const ::Murmur::AMD_Server_setStatePtr&,
			                            const ::Murmur::User&,
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
			                              const ::std::string&,
			                              ::Ice::Int,
			                              const Ice::Current&);

			virtual void sendMessageChannel_async(const ::Murmur::AMD_Server_sendMessageChannelPtr&,
			                                      ::Ice::Int,
			                                      bool,
			                                      const ::std::string&,
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

			virtual void removeUserFromGroup_async(const ::Murmur::AMD_Server_removeUserFromGroupPtr&, ::Ice::Int, ::Ice::Int, const ::std::string&, const ::Ice::Current&);

			virtual void addUserToGroup_async(const ::Murmur::AMD_Server_addUserToGroupPtr&, ::Ice::Int, ::Ice::Int, const ::std::string&, const ::Ice::Current&);

			virtual void redirectWhisperGroup_async(const ::Murmur::AMD_Server_redirectWhisperGroupPtr&, ::Ice::Int, const ::std::string&, const ::std::string&, const ::Ice::Current&);

			virtual void getUserNames_async(const ::Murmur::AMD_Server_getUserNamesPtr&,
			                                const ::Murmur::IdList&,
			                                const Ice::Current&);

			virtual void getUserIds_async(const ::Murmur::AMD_Server_getUserIdsPtr&,
			                              const ::Murmur::NameList&,
			                              const Ice::Current&);

			virtual void registerUser_async(const ::Murmur::AMD_Server_registerUserPtr&,
			                                const Murmur::UserInfoMap&,
			                                const Ice::Current&);

			virtual void unregisterUser_async(const ::Murmur::AMD_Server_unregisterUserPtr&,
			                                  ::Ice::Int,
			                                  const Ice::Current&);

			virtual void updateRegistration_async(const ::Murmur::AMD_Server_updateRegistrationPtr&,
			                                      Ice::Int,
			                                      const Murmur::UserInfoMap&,
			                                      const Ice::Current&);

			virtual void getRegistration_async(const ::Murmur::AMD_Server_getRegistrationPtr&,
			                                   ::Ice::Int,
			                                   const Ice::Current&);

			virtual void getRegisteredUsers_async(const ::Murmur::AMD_Server_getRegisteredUsersPtr&,
			                                      const ::std::string&,
			                                      const Ice::Current&);

			virtual void verifyPassword_async(const ::Murmur::AMD_Server_verifyPasswordPtr&,
			                                  const ::std::string&,
			                                  const ::std::string&,
			                                  const Ice::Current&);

			virtual void getTexture_async(const ::Murmur::AMD_Server_getTexturePtr&,
			                              ::Ice::Int,
			                              const Ice::Current&);

			virtual void setTexture_async(const ::Murmur::AMD_Server_setTexturePtr&,
			                              ::Ice::Int,
			                              const ::Murmur::Texture&,
			                              const Ice::Current&);

			virtual void getUptime_async(const ::Murmur::AMD_Server_getUptimePtr&,
			                             const Ice::Current&);

			virtual void ice_ping(const Ice::Current&) const;
	};

	class MetaI : virtual public Meta {
		public:
			virtual void getSliceChecksums_async(const ::Murmur::AMD_Meta_getSliceChecksumsPtr&,
			                                     const ::Ice::Current&);

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

			virtual void getVersion_async(const ::Murmur::AMD_Meta_getVersionPtr&,
			                              const Ice::Current&);


			virtual void addCallback_async(const ::Murmur::AMD_Meta_addCallbackPtr&, const ::Murmur::MetaCallbackPrx&, const ::Ice::Current& = ::Ice::Current());
			virtual void removeCallback_async(const ::Murmur::AMD_Meta_removeCallbackPtr&, const ::Murmur::MetaCallbackPrx&, const ::Ice::Current& = ::Ice::Current());

			virtual void getUptime_async(const ::Murmur::AMD_Meta_getUptimePtr&,
			                             const Ice::Current&);

			virtual void getSlice_async(const ::Murmur::AMD_Meta_getSlicePtr&,
			                            const Ice::Current&);
	};

}

#endif
