void ::Murmur::ServerI::isRunning_async(const ::Murmur::AMD_Server_isRunningPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_isRunning, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::start_async(const ::Murmur::AMD_Server_startPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_start, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::stop_async(const ::Murmur::AMD_Server_stopPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_stop, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::delete_async(const ::Murmur::AMD_Server_deletePtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_delete, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::id_async(const ::Murmur::AMD_Server_idPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_id, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::addCallback_async(const ::Murmur::AMD_Server_addCallbackPtr &cb,  const ::Murmur::ServerCallbackPrx& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_addCallback, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::removeCallback_async(const ::Murmur::AMD_Server_removeCallbackPtr &cb,  const ::Murmur::ServerCallbackPrx& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_removeCallback, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setAuthenticator_async(const ::Murmur::AMD_Server_setAuthenticatorPtr &cb,  const ::Murmur::ServerAuthenticatorPrx& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setAuthenticator, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getConf_async(const ::Murmur::AMD_Server_getConfPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getConf, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getAllConf_async(const ::Murmur::AMD_Server_getAllConfPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getAllConf, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setConf_async(const ::Murmur::AMD_Server_setConfPtr &cb,  const ::std::string& p1,  const ::std::string& p2, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setConf, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setSuperuserPassword_async(const ::Murmur::AMD_Server_setSuperuserPasswordPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setSuperuserPassword, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getLog_async(const ::Murmur::AMD_Server_getLogPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getLog, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getUsers_async(const ::Murmur::AMD_Server_getUsersPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getUsers, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getChannels_async(const ::Murmur::AMD_Server_getChannelsPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getChannels, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getCertificateList_async(const ::Murmur::AMD_Server_getCertificateListPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getCertificateList, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getTree_async(const ::Murmur::AMD_Server_getTreePtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getTree, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getBans_async(const ::Murmur::AMD_Server_getBansPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getBans, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setBans_async(const ::Murmur::AMD_Server_setBansPtr &cb,  const ::Murmur::BanList& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setBans, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::kickUser_async(const ::Murmur::AMD_Server_kickUserPtr &cb,  ::Ice::Int p1,  const ::std::string& p2, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_kickUser, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getState_async(const ::Murmur::AMD_Server_getStatePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setState_async(const ::Murmur::AMD_Server_setStatePtr &cb,  const ::Murmur::User& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::sendMessage_async(const ::Murmur::AMD_Server_sendMessagePtr &cb,  ::Ice::Int p1,  const ::std::string& p2, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_sendMessage, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::hasPermission_async(const ::Murmur::AMD_Server_hasPermissionPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2,  ::Ice::Int p3, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_hasPermission, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::addContextCallback_async(const ::Murmur::AMD_Server_addContextCallbackPtr &cb,  ::Ice::Int p1,  const ::std::string& p2,  const ::std::string& p3,  const ::Murmur::ServerContextCallbackPrx& p4,  ::Ice::Int p5, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_addContextCallback, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3, p4, p5));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::removeContextCallback_async(const ::Murmur::AMD_Server_removeContextCallbackPtr &cb,  const ::Murmur::ServerContextCallbackPrx& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_removeContextCallback, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getChannelState_async(const ::Murmur::AMD_Server_getChannelStatePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getChannelState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setChannelState_async(const ::Murmur::AMD_Server_setChannelStatePtr &cb,  const ::Murmur::Channel& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setChannelState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::removeChannel_async(const ::Murmur::AMD_Server_removeChannelPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_removeChannel, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::addChannel_async(const ::Murmur::AMD_Server_addChannelPtr &cb,  const ::std::string& p1,  ::Ice::Int p2, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_addChannel, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::sendMessageChannel_async(const ::Murmur::AMD_Server_sendMessageChannelPtr &cb,  ::Ice::Int p1,  bool p2,  const ::std::string& p3, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_sendMessageChannel, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getACL_async(const ::Murmur::AMD_Server_getACLPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getACL, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setACL_async(const ::Murmur::AMD_Server_setACLPtr &cb,  ::Ice::Int p1,  const ::Murmur::ACLList& p2,  const ::Murmur::GroupList& p3,  bool p4, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setACL, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3, p4));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::addUserToGroup_async(const ::Murmur::AMD_Server_addUserToGroupPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2,  const ::std::string& p3, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_addUserToGroup, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::removeUserFromGroup_async(const ::Murmur::AMD_Server_removeUserFromGroupPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2,  const ::std::string& p3, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_removeUserFromGroup, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::redirectWhisperGroup_async(const ::Murmur::AMD_Server_redirectWhisperGroupPtr &cb,  ::Ice::Int p1,  const ::std::string& p2,  const ::std::string& p3, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_redirectWhisperGroup, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getUserNames_async(const ::Murmur::AMD_Server_getUserNamesPtr &cb,  const ::Murmur::IdList& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getUserNames, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getUserIds_async(const ::Murmur::AMD_Server_getUserIdsPtr &cb,  const ::Murmur::NameList& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getUserIds, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::registerUser_async(const ::Murmur::AMD_Server_registerUserPtr &cb,  const ::Murmur::UserInfoMap& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_registerUser, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::unregisterUser_async(const ::Murmur::AMD_Server_unregisterUserPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_unregisterUser, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::updateRegistration_async(const ::Murmur::AMD_Server_updateRegistrationPtr &cb,  ::Ice::Int p1,  const ::Murmur::UserInfoMap& p2, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_updateRegistration, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getRegistration_async(const ::Murmur::AMD_Server_getRegistrationPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getRegistration, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getRegisteredUsers_async(const ::Murmur::AMD_Server_getRegisteredUsersPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getRegisteredUsers, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::verifyPassword_async(const ::Murmur::AMD_Server_verifyPasswordPtr &cb,  const ::std::string& p1,  const ::std::string& p2, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_verifyPassword, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getTexture_async(const ::Murmur::AMD_Server_getTexturePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_getTexture, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setTexture_async(const ::Murmur::AMD_Server_setTexturePtr &cb,  ::Ice::Int p1,  const ::Murmur::Texture& p2, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Server_setTexture, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getServer_async(const ::Murmur::AMD_Meta_getServerPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getServer, cb, current.adapter, p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::newServer_async(const ::Murmur::AMD_Meta_newServerPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_newServer, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getBootedServers_async(const ::Murmur::AMD_Meta_getBootedServersPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getBootedServers, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getAllServers_async(const ::Murmur::AMD_Meta_getAllServersPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getAllServers, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getDefaultConf_async(const ::Murmur::AMD_Meta_getDefaultConfPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getDefaultConf, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getVersion_async(const ::Murmur::AMD_Meta_getVersionPtr &cb, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_getVersion, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::addCallback_async(const ::Murmur::AMD_Meta_addCallbackPtr &cb,  const ::Murmur::MetaCallbackPrx& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_addCallback, cb, current.adapter, p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::removeCallback_async(const ::Murmur::AMD_Meta_removeCallbackPtr &cb,  const ::Murmur::MetaCallbackPrx& p1, const ::Ice::Current &current) {
	ExecEvent *ie = new ExecEvent(boost::bind(&impl_Meta_removeCallback, cb, current.adapter, p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
