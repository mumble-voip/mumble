void ::Murmur::ServerI::isRunning_async(const ::Murmur::AMD_Server_isRunningPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_isRunning, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::start_async(const ::Murmur::AMD_Server_startPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_start, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::stop_async(const ::Murmur::AMD_Server_stopPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_stop, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::delete_async(const ::Murmur::AMD_Server_deletePtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_delete, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::id_async(const ::Murmur::AMD_Server_idPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_id, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getConf_async(const ::Murmur::AMD_Server_getConfPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getConf, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getAllConf_async(const ::Murmur::AMD_Server_getAllConfPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getAllConf, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setConf_async(const ::Murmur::AMD_Server_setConfPtr &cb,  const ::std::string& p1,  const ::std::string& p2, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_setConf, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setSuperuserPasssword_async(const ::Murmur::AMD_Server_setSuperuserPassswordPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_setSuperuserPasssword, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getLog_async(const ::Murmur::AMD_Server_getLogPtr &cb,  ::Ice::Int p1,  ::Ice::Int p2, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getLog, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getPlayers_async(const ::Murmur::AMD_Server_getPlayersPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getPlayers, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getChannels_async(const ::Murmur::AMD_Server_getChannelsPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getChannels, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getTree_async(const ::Murmur::AMD_Server_getTreePtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getTree, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getBans_async(const ::Murmur::AMD_Server_getBansPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getBans, cb, QString::fromStdString(current.id.name).toInt()));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setBans_async(const ::Murmur::AMD_Server_setBansPtr &cb,  const ::Murmur::BanList& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_setBans, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::kickPlayer_async(const ::Murmur::AMD_Server_kickPlayerPtr &cb,  ::Ice::Int p1,  const ::std::string& p2, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_kickPlayer, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getState_async(const ::Murmur::AMD_Server_getStatePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setState_async(const ::Murmur::AMD_Server_setStatePtr &cb,  const ::Murmur::Player& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_setState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::sendMessage_async(const ::Murmur::AMD_Server_sendMessagePtr &cb,  ::Ice::Int p1,  const ::std::string& p2, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_sendMessage, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getChannelState_async(const ::Murmur::AMD_Server_getChannelStatePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getChannelState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setChannelState_async(const ::Murmur::AMD_Server_setChannelStatePtr &cb,  const ::Murmur::Channel& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_setChannelState, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::removeChannel_async(const ::Murmur::AMD_Server_removeChannelPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_removeChannel, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::addChannel_async(const ::Murmur::AMD_Server_addChannelPtr &cb,  const ::std::string& p1,  ::Ice::Int p2, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_addChannel, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::sendMessageChannel_async(const ::Murmur::AMD_Server_sendMessageChannelPtr &cb,  ::Ice::Int p1,  bool p2,  const ::std::string& p3, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_sendMessageChannel, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getACL_async(const ::Murmur::AMD_Server_getACLPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getACL, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setACL_async(const ::Murmur::AMD_Server_setACLPtr &cb,  ::Ice::Int p1,  const ::Murmur::ACLList& p2,  const ::Murmur::GroupList& p3,  bool p4, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_setACL, cb, QString::fromStdString(current.id.name).toInt(), p1, p2, p3, p4));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getPlayerNames_async(const ::Murmur::AMD_Server_getPlayerNamesPtr &cb,  const ::Murmur::IdList& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getPlayerNames, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getPlayerIds_async(const ::Murmur::AMD_Server_getPlayerIdsPtr &cb,  const ::Murmur::NameList& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getPlayerIds, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::registerPlayer_async(const ::Murmur::AMD_Server_registerPlayerPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_registerPlayer, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::unregisterPlayer_async(const ::Murmur::AMD_Server_unregisterPlayerPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_unregisterPlayer, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::updateregistration_async(const ::Murmur::AMD_Server_updateregistrationPtr &cb,  const ::Murmur::RegisteredPlayer& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_updateregistration, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getRegistration_async(const ::Murmur::AMD_Server_getRegistrationPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getRegistration, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getRegisteredPlayers_async(const ::Murmur::AMD_Server_getRegisteredPlayersPtr &cb,  const ::std::string& p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getRegisteredPlayers, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::verifyPassword_async(const ::Murmur::AMD_Server_verifyPasswordPtr &cb,  const ::std::string& p1,  const ::std::string& p2, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_verifyPassword, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::getTexture_async(const ::Murmur::AMD_Server_getTexturePtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_getTexture, cb, QString::fromStdString(current.id.name).toInt(), p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::ServerI::setTexture_async(const ::Murmur::AMD_Server_setTexturePtr &cb,  ::Ice::Int p1,  const ::Murmur::Texture& p2, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Server_setTexture, cb, QString::fromStdString(current.id.name).toInt(), p1, p2));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getServer_async(const ::Murmur::AMD_Meta_getServerPtr &cb,  ::Ice::Int p1, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Meta_getServer, cb, current.adapter, p1));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::newServer_async(const ::Murmur::AMD_Meta_newServerPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Meta_newServer, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getBootedServers_async(const ::Murmur::AMD_Meta_getBootedServersPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Meta_getBootedServers, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getAllServers_async(const ::Murmur::AMD_Meta_getAllServersPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Meta_getAllServers, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getDefaultConf_async(const ::Murmur::AMD_Meta_getDefaultConfPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Meta_getDefaultConf, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
void ::Murmur::MetaI::getVersion_async(const ::Murmur::AMD_Meta_getVersionPtr &cb, const ::Ice::Current &current) {
	IceEvent *ie = new IceEvent(boost::bind(&impl_Meta_getVersion, cb, current.adapter));
	QCoreApplication::instance()->postEvent(mi, ie);
};
