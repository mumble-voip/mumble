// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef Q_MOC_RUN
# include <boost/function.hpp>
#endif

#include "Mumble.pb.h"

#include "../Message.h"
#include "../Group.h"
#include "MurmurGRPCImpl.h"
#include "ServerDB.h"
#include "ServerUser.h"
#include "Server.h"
#include "Channel.h"
#include "Utils.h"

#include <QtCore/QStack>

#include "MurmurRPC.proto.Wrapper.cpp"

// GRPC system overview
// ====================
//
// Definitions:
//
//  - Completion queue: a mechanism used for handling asynchronous grpc
//    requests. Whenever you want to do something with grpc (e.g. accept a
//    remote call, read/write a message), the completion queue manages that
//    request for you.
//
//    When you wish to perform an asynchronous action, you provide the
//    completion queue with a unique "tag". This tag used to identify the
//    request after it has finished.
//
//    To check the status of asynchronous actions that have been added to the
//    completion queue, you call its "Next" method. Next returns when there is
//    a finished action in the completion queue. Next provides two output
//    variables: the completed action's tag, and a boolean, which indicates if
//    the action was successful. All tags in this project are are
//    heap-allocated pointers to "boost::function"s that accept a boolean.
//    After Next returns, the tag pointer is dereferenced, executed with the
//    success variable, and then freed.
//
//  - Wrapper class: each RPC method that is defined in the .proto has had a
//    class generated for it (the generator program is located in scripts/).
//    Each class provides storage for the incoming and outgoing protocol buffer
//    message, as well as helper methods for registering events with the
//    completion queue. Each wrapper class also has an "impl(bool)" method;
//    each one is implemented in this file. This impl method gets executed when
//    the particular RPC method gets invoked by an RPC client.
//
//    Each wrapper class extends RPCCall. Among other things, RPCCall provides
//    reference counting for the object. This is needed for memory management,
//    as the wrapper objects can be used and stored in several locations (this
//    is only really true for streaming grpc calls).
//
// The flow of the grpc system is as follows:
//
//  - GRPCStart() is called from murmur's main().
//  - If an address for the grpc has been set in murmur.ini, the grpc service
//    begins listening on that address for grpc client connections.
//  - A new thread is created which handles the grpc completion queue (defined
//    above). This thread continuously calls the completion queue's Next method
//    to process the next completed event.
//  - The wrapper classes' "create" methods are executed, which makes them
//    invokable by grpc clients.
//  - With the completion queue now running in the background, murmur
//    continues starting up.
//
//  - When a grpc client invokes an RPC method, the previously mentioned thread
//    is notified of this and executes the "tag" (recall, tags are pointers to
//    functions). The wrapper method "impl(bool)" for the corresponding RPC
//    method ends up being called. It is important to note that this impl
//    method gets executed in the main thread. This prevents data corruption
//    of murmur's data structures without the need of locks.
//
//    Additionally, the execution of tags are wrapped with a try-catch. This
//    try-catch catches any grpc::Status that is thrown. If one is caught, the
//    status is automatically sent to the grpc client and the invocation of the
//    current method is stopped.

static MurmurRPCImpl *service;

void GRPCStart() {
	const auto &address = meta->mp.qsGRPCAddress;
	if (address.isEmpty()) {
		return;
	}
	const auto &cert = meta->mp.qsGRPCCert;
	const auto &key = meta->mp.qsGRPCKey;
	std::shared_ptr<::grpc::ServerCredentials> credentials;
	if (cert.isEmpty() || key.isEmpty()) {
		credentials = ::grpc::InsecureServerCredentials();
	} else {
		::grpc::SslServerCredentialsOptions options;
		::grpc::SslServerCredentialsOptions::PemKeyCertPair pair;
		{
			QFile file(cert);
			if (!file.open(QIODevice::ReadOnly)) {
				qFatal("could not open gRPC certificate file: %s", cert.toStdString().c_str());
				return;
			}
			QTextStream stream(&file);
			auto contents = stream.readAll();
			pair.cert_chain = contents.toStdString();
		}
		{
			QFile file(key);
			if (!file.open(QIODevice::ReadOnly)) {
				qFatal("could not open gRPC key file: %s", key.toStdString().c_str());
				return;
			}
			QTextStream stream(&file);
			auto contents = stream.readAll();
			pair.private_key = contents.toStdString();
		}
		options.pem_key_cert_pairs.push_back(pair);
		credentials = ::grpc::SslServerCredentials(options);
	}

	service = new MurmurRPCImpl(address, credentials);

	qWarning("GRPC: listening on '%s'", qPrintable(address));
}

void GRPCStop() {
	delete service;
}

MurmurRPCImpl::MurmurRPCImpl(const QString &address, std::shared_ptr<::grpc::ServerCredentials> credentials) {
	::grpc::ServerBuilder builder;
	builder.AddListeningPort(u8(address), credentials);
	builder.RegisterService(&m_V1Service);
	m_completionQueue = builder.AddCompletionQueue();
	m_server = builder.BuildAndStart();
	meta->connectListener(this);
	start();
}

MurmurRPCImpl::~MurmurRPCImpl() {
}

// ToRPC/FromRPC methods convert data to/from grpc protocol buffer messages.
void ToRPC(const ::Server *srv, const ::Channel *c, ::MurmurRPC::Channel *rc) {
	rc->mutable_server()->set_id(srv->iServerNum);

	rc->set_id(c->iId);
	rc->set_name(u8(c->qsName));
	if (c->cParent) {
		rc->mutable_parent()->mutable_server()->set_id(srv->iServerNum);
		rc->mutable_parent()->set_id(c->cParent->iId);
	}
	rc->set_description(u8(c->qsDesc));
	rc->set_position(c->iPosition);
	foreach(::Channel *chn, c->qsPermLinks) {
		::MurmurRPC::Channel *linked = rc->add_links();
		linked->mutable_server()->set_id(srv->iServerNum);
		linked->set_id(chn->iId);
	}
	rc->set_temporary(c->bTemporary);
}

void ToRPC(const ::Server *srv, const ::User *u, ::MurmurRPC::User *ru) {
	ru->mutable_server()->set_id(srv->iServerNum);

	ru->set_session(u->uiSession);
	if (u->iId >= 0) {
		ru->set_id(u->iId);
	}
	ru->set_name(u8(u->qsName));
	ru->set_mute(u->bMute);
	ru->set_deaf(u->bDeaf);
	ru->set_suppress(u->bSuppress);
	ru->set_recording(u->bRecording);
	ru->set_priority_speaker(u->bPrioritySpeaker);
	ru->set_self_mute(u->bSelfMute);
	ru->set_self_deaf(u->bSelfDeaf);
	ru->mutable_channel()->mutable_server()->set_id(srv->iServerNum);
	ru->mutable_channel()->set_id(u->cChannel->iId);
	ru->set_comment(u8(u->qsComment));

	const auto su = static_cast<const ServerUser *>(u);
	ru->set_online_secs(su->bwr.onlineSeconds());
	ru->set_bytes_per_sec(su->bwr.bandwidth());
	ru->mutable_version()->set_version(su->uiVersion);
	ru->mutable_version()->set_release(u8(su->qsRelease));
	ru->mutable_version()->set_os(u8(su->qsOS));
	ru->mutable_version()->set_os_version(u8(su->qsOSVersion));
	ru->set_plugin_identity(u8(su->qsIdentity));
	ru->set_plugin_context(su->ssContext);
	ru->set_idle_secs(su->bwr.idleSeconds());
	ru->set_udp_ping_msecs(su->dUDPPingAvg);
	ru->set_tcp_ping_msecs(su->dTCPPingAvg);

	ru->set_tcp_only(QAtomicIntLoad(su->aiUdpFlag) == 0);

	ru->set_address(su->haAddress.toStdString());
}

void ToRPC(const ::Server *srv, const QMap<int, QString> &info, const QByteArray &texture, ::MurmurRPC::DatabaseUser *du) {
	du->mutable_server()->set_id(srv->iServerNum);

	if (info.contains(::ServerDB::User_Name)) {
		du->set_name(u8(info[::ServerDB::User_Name]));
	}
	if (info.contains(::ServerDB::User_Email)) {
		du->set_email(u8(info[::ServerDB::User_Email]));
	}
	if (info.contains(::ServerDB::User_Comment)) {
		du->set_comment(u8(info[::ServerDB::User_Comment]));
	}
	if (info.contains(::ServerDB::User_Hash)) {
		du->set_hash(u8(info[::ServerDB::User_Hash]));
	}
	if (info.contains(::ServerDB::User_Password)) {
		du->set_password(u8(info[::ServerDB::User_Password]));
	}
	if (info.contains(::ServerDB::User_LastActive)) {
		du->set_last_active(u8(info[::ServerDB::User_LastActive]));
	}
	if (!texture.isNull()) {
		du->set_texture(texture.constData(), texture.size());
	}
}

void FromRPC(const ::MurmurRPC::DatabaseUser &du, QMap<int, QString> &info) {
	if (du.has_name()) {
		info.insert(::ServerDB::User_Name, u8(du.name()));
	}
	if (du.has_email()) {
		info.insert(::ServerDB::User_Email, u8(du.email()));
	}
	if (du.has_comment()) {
		info.insert(::ServerDB::User_Comment, u8(du.comment()));
	}
	if (du.has_hash()) {
		info.insert(::ServerDB::User_Hash, u8(du.hash()));
	}
	if (du.has_password()) {
		info.insert(::ServerDB::User_Password, u8(du.password()));
	}
	if (du.has_last_active()) {
		info.insert(::ServerDB::User_LastActive, u8(du.last_active()));
	}
}

void ToRPC(const ::Server *srv, const ::ChanACL *acl, ::MurmurRPC::ACL *ra) {
	ra->set_apply_here(acl->bApplyHere);
	ra->set_apply_subs(acl->bApplySubs);
	ra->set_inherited(false);

	if (acl->iUserId >= 0) {
		ra->mutable_user()->mutable_server()->set_id(srv->iServerNum);
		ra->mutable_user()->set_id(acl->iUserId);
	}
	if (!acl->qsGroup.isEmpty()) {
		ra->mutable_group()->set_name(u8(acl->qsGroup));
	}

	ra->set_allow(MurmurRPC::ACL_Permission(int(acl->pAllow)));
	ra->set_deny(MurmurRPC::ACL_Permission(int(acl->pDeny)));
}

void ToRPC(const ::Server *, const ::Group *g, ::MurmurRPC::ACL_Group *rg) {
	rg->set_name(u8(g->qsName));
	rg->set_inherit(g->bInherit);
	rg->set_inheritable(g->bInheritable);
}

void ToRPC(const ::Server *srv, const ::Ban &ban, ::MurmurRPC::Ban *rb) {
	rb->mutable_server()->set_id(srv->iServerNum);

	rb->set_address(ban.haAddress.toStdString());
	rb->set_bits(ban.iMask);
	rb->set_name(u8(ban.qsUsername));
	rb->set_hash(u8(ban.qsHash));
	rb->set_reason(u8(ban.qsReason));
	rb->set_start(ban.qdtStart.toLocalTime().toTime_t());
	rb->set_duration_secs(ban.iDuration);
}

void FromRPC(const ::Server *, const ::MurmurRPC::Ban &rb, ::Ban &ban) {
	ban.haAddress = HostAddress(rb.address());
	ban.iMask = rb.bits();
	ban.qsUsername = u8(rb.name());
	ban.qsHash = u8(rb.hash());
	ban.qsReason = u8(rb.reason());
	ban.qdtStart = QDateTime::fromTime_t(static_cast<quint32>(rb.start())).toUTC();
	ban.iDuration = rb.duration_secs();
}

void ToRPC(int serverID, const ::ServerDB::LogRecord &log, ::MurmurRPC::Log *rl) {
	rl->mutable_server()->set_id(serverID);

	rl->set_timestamp(log.first);
	rl->set_text(u8(log.second));
}

void ToRPC(const ::Server *srv, const ::User *user, const ::TextMessage &message, ::MurmurRPC::TextMessage *rtm) {
	rtm->mutable_server()->set_id(srv->iServerNum);

	rtm->mutable_actor()->mutable_server()->set_id(srv->iServerNum);
	rtm->mutable_actor()->set_session(user->uiSession);

	foreach(auto session, message.qlSessions) {
		auto target = rtm->add_users();
		target->mutable_server()->set_id(srv->iServerNum);
		target->set_session(session);
	}

	foreach(auto id, message.qlChannels) {
		auto target = rtm->add_channels();
		target->mutable_server()->set_id(srv->iServerNum);
		target->set_id(id);
	}

	foreach(auto id, message.qlTrees) {
		auto target = rtm->add_trees();
		target->mutable_server()->set_id(srv->iServerNum);
		target->set_id(id);
	}

	rtm->set_text(u8(message.qsText));
}

// Sends a meta event to any subscribed listeners.
void MurmurRPCImpl::sendMetaEvent(const ::MurmurRPC::Event &e) {
	auto listeners = m_metaServiceListeners;

	for (auto i = listeners.constBegin(); i != listeners.constEnd(); ++i) {
		auto listener = *i;
		listener->ref();
		auto cb = [this, listener] (::MurmurRPC::Wrapper::V1_Events *, bool ok) {
			if (!ok && m_metaServiceListeners.remove(listener)) {
				listener->deref();
			}
			listener->deref();
		};
		listener->write(e, listener->callback(cb));
	}
}

// Called when a server starts.
void MurmurRPCImpl::started(::Server *server) {
	server->connectListener(this);
	server->connectAuthenticator(this);
	connect(server, SIGNAL(contextAction(const User *, const QString &, unsigned int, int)), this, SLOT(contextAction(const User *, const QString &, unsigned int, int)));
	connect(server, SIGNAL(textMessageFilterSig(int &, const User *, MumbleProto::TextMessage &)), this, SLOT(textMessageFilter(int &, const User *, MumbleProto::TextMessage &)));

	::MurmurRPC::Event rpcEvent;
	rpcEvent.set_type(::MurmurRPC::Event_Type_ServerStarted);
	rpcEvent.mutable_server()->set_id(server->iServerNum);
	sendMetaEvent(rpcEvent);
}

// Called when a server stops.
void MurmurRPCImpl::stopped(::Server *server) {
	removeActiveContextActions(server);

	::MurmurRPC::Event rpcEvent;
	rpcEvent.set_type(::MurmurRPC::Event_Type_ServerStopped);
	rpcEvent.mutable_server()->set_id(server->iServerNum);
	sendMetaEvent(rpcEvent);
}

// Removes a connected text message filter.
void MurmurRPCImpl::removeTextMessageFilter(const ::Server *s) {
	auto filter = m_textMessageFilters.value(s->iServerNum);
	if (!filter) {
		return;
	}
	filter->error(::grpc::Status(::grpc::CANCELLED, "filter detached"));
	m_textMessageFilters.remove(s->iServerNum);
}

// Removes a connected authenticator.
void MurmurRPCImpl::removeAuthenticator(const ::Server *s) {
	auto authenticator = m_authenticators.value(s->iServerNum);
	if (!authenticator) {
		return;
	}
	authenticator->error(::grpc::Status(::grpc::CANCELLED, "authenticator detached"));
	m_authenticators.remove(s->iServerNum);
}

// Called when a connecting user needs to be authenticated.
void MurmurRPCImpl::authenticateSlot(int &res, QString &uname, int sessionId, const QList<QSslCertificate> &certlist, const QString &certhash, bool certstrong, const QString &pw) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	request.mutable_authenticate()->set_name(u8(uname));
	if (!pw.isEmpty()) {
		request.mutable_authenticate()->set_password(u8(pw));
	}
	foreach(const auto &cert, certlist) {
		auto data = cert.toDer();
		request.mutable_authenticate()->add_certificates(data.constData(), data.size());
	}
	if (!certhash.isEmpty()) {
		request.mutable_authenticate()->set_certificate_hash(u8(certhash));
		request.mutable_authenticate()->set_strong_certificate(certstrong);
	}

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			res = -1;
			return;
		}
	}

	auto &response = authenticator->request;
	switch (response.authenticate().status()) {
	case ::MurmurRPC::Authenticator_Response_Status_Success:
		if (!response.authenticate().has_id()) {
			res = -3;
			break;
		}
		res = response.authenticate().id();
		if (response.authenticate().has_name()) {
			uname = u8(response.authenticate().name());
		}
		{
			QStringList qsl;
			for (int i = 0; i < response.authenticate().groups_size(); i++) {
				auto &group = response.authenticate().groups(i);
				if (group.has_name()) {
					qsl << u8(group.name());
				}
			}
			if (!qsl.isEmpty()) {
				s->setTempGroups(res, sessionId, NULL, qsl);
			}
		}
		break;
	case ::MurmurRPC::Authenticator_Response_Status_TemporaryFailure:
		res = -3;
		break;
	case ::MurmurRPC::Authenticator_Response_Status_Failure:
		res = -1;
		break;
	default:
		break;
	}
}

// Called when a user is being registered on the server.
void MurmurRPCImpl::registerUserSlot(int &res, const QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	ToRPC(s, info, QByteArray(), request.mutable_register_()->mutable_user());

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;
	switch (response.register_().status()) {
	case ::MurmurRPC::Authenticator_Response_Status_Success:
		if (!response.register_().has_user() || !response.register_().user().has_id()) {
			res = -1;
			break;
		}
		res = response.register_().user().id();
		break;
	case ::MurmurRPC::Authenticator_Response_Status_Fallthrough:
		break;
	default:
		res = -1;
		break;
	}
}

// Called when a user is being deregistered on the server.
void MurmurRPCImpl::unregisterUserSlot(int &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	request.mutable_deregister()->mutable_user()->mutable_server()->set_id(s->iServerNum);
	request.mutable_deregister()->mutable_user()->set_id(id);

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;

	if (response.deregister().status() != ::MurmurRPC::Authenticator_Response_Status_Fallthrough) {
		res = 0;
	}
}

// Called when a list of registered users is requested.
void MurmurRPCImpl::getRegisteredUsersSlot(const QString &filter, QMap<int, QString> &res) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	if (!filter.isEmpty()) {
		request.mutable_query()->set_filter(u8(filter));
	}

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;

	for (int i = 0; i < response.query().users_size(); i++) {
		const auto &user = response.query().users(i);
		if (!user.has_id() || !user.has_name()) {
			continue;
		}
		res.insert(user.id(), u8(user.name()));
	}
}

// Called when information about a registered user is requested.
void MurmurRPCImpl::getRegistrationSlot(int &res, int id, QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	request.mutable_find()->set_id(id);

	res = -1;

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;
	if (response.find().has_user()) {
		FromRPC(response.find().user(), info);
		res = 1;
	}
}

// Called when information about a registered user is being updated.
void MurmurRPCImpl::setInfoSlot(int &res, int id, const QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	request.mutable_update()->mutable_user()->set_id(id);
	ToRPC(s, info, QByteArray(), request.mutable_update()->mutable_user());

	res = 0;

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;
	switch (response.update().status()) {
	case ::MurmurRPC::Authenticator_Response_Status_Success:
		res = 1;
		break;
	case ::MurmurRPC::Authenticator_Response_Status_Fallthrough:
		res = -1;
		break;
	default:
		break;
	}
}

// Called when a texture for a registered user is being updated.
void MurmurRPCImpl::setTextureSlot(int &res, int id, const QByteArray &texture) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	request.mutable_update()->mutable_user()->set_id(id);
	request.mutable_update()->mutable_user()->set_texture(texture.constData(), texture.size());

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;
	if (response.update().status() == ::MurmurRPC::Authenticator_Response_Status_Success) {
		res = 1;
	}
}

// Called when a user name needs to be converted to a user ID.
void MurmurRPCImpl::nameToIdSlot(int &res, const QString &name) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	request.mutable_find()->set_name(u8(name));

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;
	if (response.find().has_user() && response.find().user().has_id()) {
		res = response.find().user().id();
	}
}

// Called when a user ID needs to be converted to a user name.
void MurmurRPCImpl::idToNameSlot(QString &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	request.mutable_find()->set_id(id);

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;
	if (response.find().has_user() && response.find().user().has_name()) {
		res = u8(response.find().user().name());
	}
}

// Called when a texture for a given registered user is requested.
void MurmurRPCImpl::idToTextureSlot(QByteArray &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::V1_AuthenticatorStream>(m_authenticators.value(s->iServerNum));
	if (!authenticator) {
		return;
	}

	auto &request = authenticator->response;
	request.Clear();
	request.mutable_find()->set_id(id);

	{
		QMutexLocker l(&qmAuthenticatorsLock);
		if (!authenticator->writeRead()) {
			removeAuthenticator(s);
			return;
		}
	}

	auto &response = authenticator->request;
	if (response.find().has_user() && response.find().user().has_texture()) {
		const auto &texture = response.find().user().texture();
		res = QByteArray(texture.data(), texture.size());
	}
}

// Sends a server event to subscribed listeners.
void MurmurRPCImpl::sendServerEvent(const ::Server *s, const ::MurmurRPC::Server_Event &e) {
	auto listeners = m_serverServiceListeners;
	auto serverID = s->iServerNum;
	auto i = listeners.find(serverID);

	for ( ; i != listeners.end() && i.key() == serverID; ++i) {
		auto listener = i.value();
		listener->ref();
		auto cb = [this, listener, serverID] (::MurmurRPC::Wrapper::V1_ServerEvents *, bool ok) {
			if (!ok && m_serverServiceListeners.remove(serverID, listener) > 0) {
				listener->deref();
			}
			listener->deref();
		};
		listener->write(e, listener->callback(cb));
	}
}

// Called when a user's state changes.
void MurmurRPCImpl::userStateChanged(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserStateChanged);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

// Called when a user sends a text message.
void MurmurRPCImpl::userTextMessage(const ::User *user, const ::TextMessage &message) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserTextMessage);
	ToRPC(s, user, event.mutable_user());
	ToRPC(s, user, message, event.mutable_message());
	sendServerEvent(s, event);
}

// Called when a user successfully connects to a server.
void MurmurRPCImpl::userConnected(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserConnected);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

// Called when a user disconnects from a server.
void MurmurRPCImpl::userDisconnected(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	removeUserActiveContextActions(s, user);

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserDisconnected);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

// Called when a channel's state changes.
void MurmurRPCImpl::channelStateChanged(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelStateChanged);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

// Called when a channel is created.
void MurmurRPCImpl::channelCreated(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelCreated);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

// Called when a channel is removed.
void MurmurRPCImpl::channelRemoved(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelRemoved);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

// Called when a user sends a text message.
void MurmurRPCImpl::textMessageFilter(int &res, const User *user, MumbleProto::TextMessage &message) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto filter = RPCCall::Ref<::MurmurRPC::Wrapper::V1_TextMessageFilter>(m_textMessageFilters.value(s->iServerNum));
	if (!filter) {
		return;
	}

	auto &request = filter->response;
	request.Clear();
	request.mutable_server()->set_id(s->iServerNum);
	auto m = request.mutable_message();
	m->mutable_server()->set_id(s->iServerNum);
	m->mutable_actor()->mutable_server()->set_id(s->iServerNum);
	m->mutable_actor()->set_session(user->uiSession);
	for (int i = 0; i < message.session_size(); i++) {
		auto u = m->add_users();
		u->mutable_server()->set_id(s->iServerNum);
		u->set_session(message.session(i));
	}
	for (int i = 0; i < message.channel_id_size(); i++) {
		auto c = m->add_channels();
		c->mutable_server()->set_id(s->iServerNum);
		c->set_id(message.channel_id(i));
	}
	for (int i = 0; i < message.tree_id_size(); i++) {
		auto c = m->add_trees();
		c->mutable_server()->set_id(s->iServerNum);
		c->set_id(message.tree_id(i));
	}
	m->set_text(message.message());

	{
		QMutexLocker l(&qmTextMessageFilterLock);
		if (!filter->writeRead()) {
			removeTextMessageFilter(s);
			return;
		}
	}

	auto &response = filter->request;
	res = response.action();
	switch (response.action()) {
	case ::MurmurRPC::TextMessage_Filter_Action_Accept:
		if (response.has_message() && response.message().has_text()) {
			message.set_message(response.message().text());
		}
		break;
	default:
		break;
	}
}

// Has the user been sent the given context action?
bool MurmurRPCImpl::hasActiveContextAction(const ::Server *s, const ::User *u, const QString &action) {
	const auto &m = m_activeContextActions;
	if (!m.contains(s->iServerNum)) {
		return false;
	}
	const auto &n = m.value(s->iServerNum);
	if (!n.contains(u->uiSession)) {
		return false;
	}
	const auto &o = n.value(u->uiSession);
	if (!o.contains(action)) {
		return false;
	}
	return true;
}

// Add the context action to the user's active context actions.
void MurmurRPCImpl::addActiveContextAction(const ::Server *s, const ::User *u, const QString &action) {
	m_activeContextActions[s->iServerNum][u->uiSession].insert(action);
}

// Remove the context action to the user's active context actions.
void MurmurRPCImpl::removeActiveContextAction(const ::Server *s, const ::User *u, const QString &action) {
	auto &m = m_activeContextActions;
	if (!m.contains(s->iServerNum)) {
		return;
	}
	auto &n = m[s->iServerNum];
	if (!n.contains(u->uiSession)) {
		return;
	}
	auto &o = n[u->uiSession];
	o.remove(action);
}

// Remove all of the user's active context actions.
void MurmurRPCImpl::removeUserActiveContextActions(const ::Server *s, const ::User *u) {
	auto &m = m_activeContextActions;
	if (m.contains(s->iServerNum)) {
		m[s->iServerNum].remove(u->uiSession);
	}
}

// Remove all of the server's active context actions.
void MurmurRPCImpl::removeActiveContextActions(const ::Server *s) {
	auto &m = m_activeContextActions;
	if (m.contains(s->iServerNum)) {
		m.remove(s->iServerNum);
	}
}

// Called when a context action event is triggered.
void MurmurRPCImpl::contextAction(const ::User *user, const QString &action, unsigned int session, int channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	if (!hasActiveContextAction(s, user, action)) {
		return;
	}

	::MurmurRPC::ContextAction ca;
	ca.mutable_server()->set_id(s->iServerNum);
	ca.mutable_actor()->mutable_server()->set_id(s->iServerNum);
	ca.mutable_actor()->set_session(user->uiSession);
	ca.set_action(u8(action));
	if (session > 0) {
		ca.mutable_user()->mutable_server()->set_id(s->iServerNum);
		ca.mutable_user()->set_session(session);
	}
	if (channel >= 0) {
		ca.mutable_channel()->mutable_server()->set_id(s->iServerNum);
		ca.mutable_channel()->set_id(channel);
	}

	auto serverID = s->iServerNum;
	auto listeners = this->m_contextActionListeners.value(serverID);
	auto i = listeners.find(action);
	for ( ; i != listeners.end() && i.key() == action; ++i) {
		auto listener = i.value();
		listener->ref();
		auto cb = [this, listener, serverID, action] (::MurmurRPC::Wrapper::V1_ContextActionEvents *, bool ok) {
			if (!ok && m_contextActionListeners[serverID].remove(action, listener) > 0) {
				listener->deref();
			}
			listener->deref();
		};
		listener->write(ca, listener->callback(cb));
	}
}

// *Must* functions return the requested value, or throw a ::grpc::Status
// exception.
::ServerUser *MustUser(const Server *server, unsigned int session) {
	auto user = server->qhUsers.value(session);
	if (!user || user->sState != ServerUser::Authenticated) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid user");
	}
	return user;
}

template <class T>
::ServerUser *MustUser(const ::Server *server, const T &msg) {
	if (!msg.has_user()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing user");
	}
	return MustUser(server, msg.user().session());
}

template <>
::ServerUser *MustUser(const Server *server, const ::MurmurRPC::User &msg) {
	if (!msg.has_session()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing user session");
	}
	return MustUser(server, msg.session());
}

::Server *MustServer(unsigned int id) {
	auto server = meta->qhServers.value(id);
	if (!server) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid server");
	}
	return server;
}

::Server *MustServer(int id) {
	auto server = meta->qhServers.value(id);
	if (!server) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid server");
	}
	return server;
}

template <class T>
::Server *MustServer(const T &msg) {
	if (!msg.has_server()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server");
	}
	if (!msg.server().has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server id");
	}
	return MustServer(msg.server().id());
}

template <>
::Server *MustServer(const ::MurmurRPC::Server &msg) {
	if (!msg.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server id");
	}
	return MustServer(msg.id());
}

template <class T>
int MustServerID(const T &msg) {
	if (!msg.has_server()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server");
	}
	if (!msg.server().has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server id");
	}
	auto id = msg.server().id();
	if (!ServerDB::serverExists(id)) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid server id");
	}
	return id;
}

int MustServerID(const ::MurmurRPC::Server &msg) {
	if (!msg.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server id");
	}
	auto id = msg.id();
	if (!ServerDB::serverExists(id)) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid server id");
	}
	return id;
}

::Channel *MustChannel(const Server *server, int channel_id) {
	auto channel = server->qhChannels.value(channel_id);
	if (!channel) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid channel");
	}
	return channel;
}

::Channel *MustChannel(const Server *server, unsigned int channel_id) {
	auto channel = server->qhChannels.value(channel_id);
	if (!channel) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid channel");
	}
	return channel;
}

template <class T>
::Channel *MustChannel(const Server *server, const T &msg) {
	if (!msg.has_channel()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing channel");
	}
	return MustChannel(server, msg.channel());
}

template <>
::Channel *MustChannel(const Server *server, const ::MurmurRPC::Channel &msg) {
	if (!msg.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing channel id");
	}
	return MustChannel(server, msg.id());
}

// Qt event listener for RPCExecEvents.
void MurmurRPCImpl::customEvent(QEvent *evt) {
	if (evt->type() == EXEC_QEVENT) {
		auto event = static_cast<RPCExecEvent *>(evt);
		try {
			event->execute();
		} catch (::grpc::Status &ex) {
			event->call->error(ex);
		}
	}
}

// QThread::run() implementation that runs the grpc event loop and executes
// tags as callback functions.
void MurmurRPCImpl::run() {
	MurmurRPC::Wrapper::V1_Init(this, &m_V1Service);

	while (true) {
		void *tag;
		bool ok;
		if (!m_completionQueue->Next(&tag, &ok)) {
			break;
		}
		if (tag != nullptr) {
			auto op = static_cast<boost::function<void(bool)> *>(tag);
			(*op)(ok);
			delete op;
		}
	}
	// TODO(grpc): cleanup allocated memory? not super important, because murmur
	// should be exiting now.
}

// The Wrapper implementation methods are below. Implementation methods are
// executed in the main thread when its corresponding grpc method is invoked.
//
// Since the grpc asynchronous API is used, the implementation methods below
// do not have to complete the call during the lifetime of the method (although
// this is only used for streaming calls).

namespace MurmurRPC {
namespace Wrapper {

void V1_ServerCreate::impl(bool) {
	auto id = ServerDB::addServer();

	::MurmurRPC::Server rpcServer;
	rpcServer.set_id(id);
	end(rpcServer);
}

void V1_ServerQuery::impl(bool) {
	::MurmurRPC::Server_List list;

	foreach(int id, ServerDB::getAllServers()) {
		auto rpcServer = list.add_servers();
		rpcServer->set_id(id);
		try {
			auto server = MustServer(id);
			rpcServer->set_running(true);
			rpcServer->mutable_uptime()->set_secs(server->tUptime.elapsed()/1000000LL);
		} catch (::grpc::Status &ex) {
		}
	}

	end(list);
}

void V1_ServerGet::impl(bool) {
	auto serverID = MustServerID(request);

	::MurmurRPC::Server rpcServer;
	rpcServer.set_id(serverID);
	rpcServer.set_running(false);
	try {
		auto server = MustServer(serverID);
		rpcServer.set_running(true);
		rpcServer.mutable_uptime()->set_secs(server->tUptime.elapsed()/1000000LL);
	} catch (::grpc::Status &ex) {
	}
	end(rpcServer);
}

void V1_ServerStart::impl(bool) {
	auto serverID = MustServerID(request);

	if (!meta->boot(serverID)) {
		throw ::grpc::Status(::grpc::UNKNOWN, "server could not be started, or is already started");
	}

	end();
}

void V1_ServerStop::impl(bool) {
	auto server = MustServer(request);
	meta->kill(server->iServerNum);
	end();
}

void V1_ServerRemove::impl(bool) {
	auto serverID = MustServerID(request);

	if (meta->qhServers.value(serverID)) {
		throw ::grpc::Status(::grpc::FAILED_PRECONDITION, "cannot remove started server");
	}

	ServerDB::deleteServer(serverID);
	end();
}

void V1_ServerEvents::impl(bool) {
	auto server = MustServer(request);
	rpc->m_serverServiceListeners.insert(server->iServerNum, this);
}

void V1_ServerEvents::done(bool) {
	auto &ssls = rpc->m_serverServiceListeners;
	auto i = std::find(ssls.begin(), ssls.end(), this);
	if (i != ssls.end()) {
		ssls.erase(i);
	}
	deref();
}

void V1_GetUptime::impl(bool) {
	::MurmurRPC::Uptime uptime;
	uptime.set_secs(meta->tUptime.elapsed()/1000000LL);
	end(uptime);
}

void V1_GetVersion::impl(bool) {
	::MurmurRPC::Version version;
	int major, minor, patch;
	QString release;
	Meta::getVersion(major, minor, patch, release);
	version.set_version(major << 16 | minor << 8 | patch);
	version.set_release(u8(release));
	end(version);
}

void V1_Events::impl(bool) {
	rpc->m_metaServiceListeners.insert(this);
}

void V1_Events::done(bool) {
	auto &msls = rpc->m_metaServiceListeners;
	auto i = std::find(msls.begin(), msls.end(), this);
	if (i != msls.end()) {
		msls.erase(i);
	}
	deref();
}

void V1_ContextActionAdd::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	if (!request.has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}
	if (!request.has_text()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing text");
	}
	if (!request.has_context()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing context");
	}

	rpc->addActiveContextAction(server, user, u8(request.action()));

	::MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(request.action());
	mpcam.set_text(request.text());
	mpcam.set_context(request.context());
	mpcam.set_operation(::MumbleProto::ContextActionModify_Operation_Add);
	server->sendMessage(user, mpcam);

	end();
}

void V1_ContextActionRemove::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}

	auto action = u8(request.action());

	::MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(request.action());
	mpcam.set_operation(::MumbleProto::ContextActionModify_Operation_Remove);

	if (request.has_user()) {
		// Remove context action from specific user
		auto user = MustUser(server, request);
		rpc->removeActiveContextAction(server, user, action);
		server->sendMessage(user, mpcam);
	} else {
		// Remove context action from all users
		foreach(::ServerUser *user, server->qhUsers) {
			if (user->sState != ServerUser::Authenticated) {
				continue;
			}
			rpc->removeActiveContextAction(server, user, action);
			server->sendMessage(user, mpcam);
		}
	}

	end();
}

void V1_ContextActionEvents::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}

	rpc->m_contextActionListeners[server->iServerNum].insert(u8(request.action()), this);
}

void V1_ContextActionEvents::done(bool) {
	auto server = MustServer(request);
	auto &cals = rpc->m_contextActionListeners;
	auto &scals = cals[server->iServerNum];

	auto i = std::find(scals.begin(), scals.end(), this);
	if (i != scals.end()) {
		scals.erase(i);
	}
	deref();
	if (scals.isEmpty()) {
		auto i = std::find(cals.begin(), cals.end(), scals);
		if (i != cals.end()) {
			cals.erase(i);
		}
	}
}

void V1_TextMessageSend::impl(bool) {
	auto server = MustServer(request);

	::MumbleProto::TextMessage tm;
	tm.set_message(request.text());
	if (request.has_actor() && request.actor().has_session()) {
		tm.set_actor(request.actor().session());
	}
	for (int i = 0; i < request.users_size(); i++) {
		if (request.users(i).has_session()) {
			tm.add_session(request.users(i).session());
		}
	}
	for (int i = 0; i < request.channels_size(); i++) {
		if (request.channels(i).has_id()) {
			tm.add_channel_id(request.channels(i).id());
		}
	}
	for (int i = 0; i < request.trees_size(); i++) {
		if (request.trees(i).has_id()) {
			tm.add_tree_id(request.trees(i).id());
		}
	}

	server->sendTextMessageGRPC(tm);

	end();
}

void V1_TextMessageFilter::impl(bool) {
	auto onInitialize = [this] (V1_TextMessageFilter *, bool ok) {
		if (!ok) {
			finish(ok);
			return;
		}
		auto server = MustServer(request);
		QMutexLocker l(&rpc->qmTextMessageFilterLock);
		rpc->removeTextMessageFilter(server);
		rpc->m_textMessageFilters.insert(server->iServerNum, this);
	};
	stream.Read(&request, callback(onInitialize));
}

void V1_TextMessageFilter::done(bool) {
	auto server = MustServer(request);
	auto filter = rpc->m_textMessageFilters.value(server->iServerNum);
	if (filter == this) {
		rpc->m_textMessageFilters.remove(server->iServerNum);
	}
	deref();
}

void V1_LogQuery::impl(bool) {
	auto serverID = MustServerID(request);

	int total = ::ServerDB::getLogLen(serverID);
	if (total < 0) {
		throw ::grpc::Status(::grpc::StatusCode::UNAVAILABLE, "could not access database");
	}

	::MurmurRPC::Log_List list;
	list.mutable_server()->set_id(serverID);
	list.set_total(total);

	if (!request.has_min() || !request.has_max()) {
		end(list);
		return;
	}
	list.set_min(request.min());
	list.set_max(request.max());

	auto dblog = ::ServerDB::getLog(serverID, request.min(), request.max());
	foreach(const ::ServerDB::LogRecord &record, dblog) {
		auto rpcLog = list.add_entries();
		ToRPC(serverID, record, rpcLog);
	}

	end(list);
}

void V1_ConfigGet::impl(bool) {
	auto serverID = MustServerID(request);
	auto config = ServerDB::getAllConf(serverID);

	::MurmurRPC::Config rpcConfig;
	rpcConfig.mutable_server()->set_id(serverID);
	auto &fields = *rpcConfig.mutable_fields();
	for (auto i = config.constBegin(); i != config.constEnd(); ++i) {
		fields[u8(i.key())] = u8(i.value());
	}

	end(rpcConfig);
}

void V1_ConfigGetField::impl(bool) {
	auto serverID = MustServerID(request);
	if (!request.has_key()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing key");
	}
	::MurmurRPC::Config_Field rpcField;
	rpcField.mutable_server()->set_id(serverID);
	rpcField.set_key(request.key());
	rpcField.set_value(u8(ServerDB::getConf(serverID, u8(request.key()), QVariant()).toString()));
	end(rpcField);
}

void V1_ConfigSetField::impl(bool) {
	auto serverID = MustServerID(request);
	if (!request.has_key()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing key");
	}
	QString key = u8(request.key());
	QString value;
	if (request.has_value()) {
		value = u8(request.value());
	}
	ServerDB::setConf(serverID, key, value);
	try {
		auto server = MustServer(serverID);
		server->setLiveConf(key, value);
	} catch (::grpc::Status &ex) {
	}

	end();
}

void V1_ConfigGetDefault::impl(bool) {
	::MurmurRPC::Config rpcConfig;
	auto &fields = *rpcConfig.mutable_fields();
	for (auto i = meta->mp.qmConfig.constBegin(); i != meta->mp.qmConfig.constEnd(); ++i) {
		fields[u8(i.key())] = u8(i.value());
	}

	end(rpcConfig);
}

void V1_ChannelQuery::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::Channel_List list;
	list.mutable_server()->set_id(server->iServerNum);

	foreach(const ::Channel *channel, server->qhChannels) {
		auto rpcChannel = list.add_channels();
		ToRPC(server, channel, rpcChannel);
	}

	end(list);
}

void V1_ChannelGet::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MurmurRPC::Channel rpcChannel;
	ToRPC(server, channel, &rpcChannel);
	end(rpcChannel);
}

void V1_ChannelAdd::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_parent() || !request.has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "parent channel and name required");
	}
	::Channel *parent;
	try {
		parent = MustChannel(server, request.parent());
	} catch (::grpc::Status &ex) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid parent channel");
	}

	if (!server->canNest(parent)) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "cannot nest channel in given parent");
	}

	QString qsName = u8(request.name());

	::Channel *nc;

	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		nc = server->addChannel(parent, qsName, request.temporary(), request.position());
	}

	nc->qsDesc = u8(request.description());

	server->updateChannel(nc);
	int newid = nc->iId;

	::MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(newid);
	mpcs.set_parent(parent->iId);
	mpcs.set_name(request.name());
	mpcs.set_temporary(request.temporary());
	mpcs.set_position(request.position());
	mpcs.set_description(request.description());
	server->sendAll(mpcs);

	::MurmurRPC::Channel resChannel;
	ToRPC(server, nc, &resChannel);
	end(resChannel);
}

void V1_ChannelRemove::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	if (!channel->cParent) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "cannot remove the root channel");
	}

	server->removeChannel(channel);

	end();
}

void V1_ChannelUpdate::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(channel->iId);
	for (int i = 0; i < request.links_size(); i++) {
		if (!request.links(i).has_id()) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid channel link");
		}
		mpcs.add_links(request.links(i).id());
	}
	if (request.has_parent()) {
		if (!request.parent().has_id()) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid channel parent");
		}
		mpcs.set_parent(request.parent().id());
	}
	if (request.has_name()) {
		mpcs.set_name(request.name());
	}
	if (request.has_position()) {
		mpcs.set_position(request.position());
	}
	if (request.has_description()) {
		mpcs.set_description(request.description());
	}

	QString err;
	if (!server->setChannelStateGRPC(mpcs, err)) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, u8(err));
	}

	::MurmurRPC::Channel rpcChannel;
	ToRPC(server, channel, &rpcChannel);
	end(rpcChannel);
}

void V1_UserQuery::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::User_List list;
	list.mutable_server()->set_id(server->iServerNum);

	foreach(const ::ServerUser *user, server->qhUsers) {
		if (user->sState != ServerUser::Authenticated) {
			continue;
		}
		auto rpcUser = list.add_users();
		ToRPC(server, user, rpcUser);
	}

	end(list);
}

void V1_UserGet::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::User rpcUser;

	if (request.has_session()) {
		// Lookup user by session
		auto user = MustUser(server, request);
		ToRPC(server, user, &rpcUser);
		end(rpcUser);
		return;
	} else if (request.has_name()) {
		// Lookup user by name
		QString qsName = u8(request.name());
		foreach(const ::ServerUser *user, server->qhUsers) {
			if (user->sState != ServerUser::Authenticated) {
				continue;
			}
			if (user->qsName == qsName) {
				ToRPC(server, user, &rpcUser);
				end(rpcUser);
				return;
			}
		}
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid user");
	}

	throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "session or name required");
}

void V1_UserUpdate::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	auto channel = user->cChannel;
	if (request.has_channel()) {
		channel = MustChannel(server, request.channel());
	}
	auto mute = user->bMute;
	if (request.has_mute()) {
		mute = request.mute();
	}
	auto deaf = user->bDeaf;
	if (request.has_deaf()) {
		deaf = request.deaf();
	}
	auto suppress = user->bSuppress;
	if (request.has_suppress()) {
		suppress = request.suppress();
	}
	auto prioritySpeaker = user->bPrioritySpeaker;
	if (request.has_priority_speaker()) {
		prioritySpeaker = request.priority_speaker();
	}
	auto name = user->qsName;
	if (request.has_name()) {
		name = u8(request.name());
	}
	auto comment = user->qsComment;
	if (request.has_comment()) {
		comment = u8(request.comment());
	}

	server->setUserState(user, channel, mute, deaf, suppress, prioritySpeaker, name, comment);

	::MurmurRPC::User rpcUser;
	ToRPC(server, user, &rpcUser);
	end(rpcUser);
}

void V1_UserKick::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	::MumbleProto::UserRemove mpur;
	mpur.set_session(user->uiSession);
	if (request.has_reason()) {
		mpur.set_reason(request.reason());
	}
	if (request.has_actor() && request.actor().has_session()) {
		mpur.set_actor(request.actor().session());
	}
	server->sendAll(mpur);
	user->disconnectSocket();

	end();
}

void V1_TreeQuery::impl(bool) {
	auto server = MustServer(request);

	auto channel = MustChannel(server, 0);
	::MurmurRPC::Tree root;

	QQueue< QPair<const ::Channel *, ::MurmurRPC::Tree *> > qQueue;
	qQueue.enqueue(qMakePair(channel, &root));

	while (!qQueue.isEmpty()) {
		auto current = qQueue.dequeue();
		auto currentChannel = current.first;
		auto currentTree = current.second;

		ToRPC(server, currentChannel, currentTree->mutable_channel());

		QList< ::User *> users = currentChannel->qlUsers;
		std::sort(users.begin(), users.end(), [] (const ::User *a, const ::User *b) -> bool {
			return ::User::lessThan(a, b);
		});
		foreach(const ::User *u, users) {
			auto rpcUser = currentTree->add_users();
			ToRPC(server, u, rpcUser);
		}

		QList< ::Channel *> channels = currentChannel->qlChannels;
		std::sort(channels.begin(), channels.end(), [] (const ::Channel *a, const ::Channel *b) -> bool {
			return ::Channel::lessThan(a, b);
		});
		foreach(const ::Channel *subChannel, channels) {
			auto subTree = currentTree->add_children();
			qQueue.enqueue(qMakePair(subChannel, subTree));
		}
	}

	end(root);
}

void V1_BansGet::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::Ban_List list;
	list.mutable_server()->set_id(server->iServerNum);
	foreach(const ::Ban &ban, server->qlBans) {
		auto rpcBan = list.add_bans();
		ToRPC(server, ban, rpcBan);
	}

	end(list);
}

void V1_BansSet::impl(bool) {
	auto server = MustServer(request);
	server->qlBans.clear();

	for (int i = 0; i < request.bans_size(); i++) {
		const auto &rpcBan = request.bans(i);
		::Ban ban;
		FromRPC(server, rpcBan, ban);
		server->qlBans << ban;
	}
	server->saveBans();

	end();
}

void V1_ACLGet::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MurmurRPC::ACL_List list;
	list.set_inherit(channel->bInheritACL);

	QStack< ::Channel *> chans;
	ChanACL *acl;
	::Channel *p = channel;
	while (p) {
		chans.push(p);
		if ((p == channel) || p->bInheritACL) {
			p = p->cParent;
		} else {
			p = NULL;
		}
	}

	while (!chans.isEmpty()) {
		p = chans.pop();
		foreach(acl, p->qlACL) {
			if (p == channel || acl->bApplySubs) {
				auto rpcACL = list.add_acls();
				ToRPC(server, acl, rpcACL);
				if (p != channel) {
					rpcACL->set_inherited(true);
				}
			}
		}
	}

	p = channel->cParent;
	const QSet<QString> allnames = ::Group::groupNames(channel);
	foreach(const QString &name, allnames) {
		::Group *g = channel->qhGroups.value(name);
		::Group *pg = p ? ::Group::getGroup(p, name) : NULL;
		if (!g && ! pg) {
			continue;
		}
		auto aclGroup = list.add_groups();
		ToRPC(server, g ? g : pg, aclGroup);
		QSet<int> members;
		if (pg) {
			members = pg->members();
		}
		if (g) {
			foreach(auto id, g->qsAdd) {
				auto rpcUser = aclGroup->add_users_add();
				rpcUser->mutable_server()->set_id(server->iServerNum);
				rpcUser->set_id(id);
			}
			foreach(auto id, g->qsRemove) {
				auto rpcUser = aclGroup->add_users_remove();
				rpcUser->mutable_server()->set_id(server->iServerNum);
				rpcUser->set_id(id);
			}
			aclGroup->set_inherited(false);
			members += g->qsAdd;
			members -= g->qsRemove;
		} else {
			aclGroup->set_inherited(true);
		}
		foreach(auto id, members) {
			auto rpcUser = aclGroup->add_users();
			rpcUser->mutable_server()->set_id(server->iServerNum);
			rpcUser->set_id(id);
		}
	}

	end(list);
}

void V1_ACLSet::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::Group *g;
	::ChanACL *acl;

	{
		QWriteLocker wl(&server->qrwlVoiceThread);

		QHash<QString, QSet<int> > hOldTemp;
		foreach(g, channel->qhGroups) {
			hOldTemp.insert(g->qsName, g->qsTemporary);
			delete g;
		}
		foreach(acl, channel->qlACL) {
			delete acl;
		}

		channel->qhGroups.clear();
		channel->qlACL.clear();

		channel->bInheritACL = request.inherit();

		for (int i = 0; i < request.groups_size(); i++) {
			auto &rpcGroup = request.groups(i);
			auto name = u8(rpcGroup.name());
			g = new ::Group(channel, name);
			g->bInherit = rpcGroup.inherit();
			g->bInheritable = rpcGroup.inheritable();
			for (int j = 0; j < rpcGroup.users_add_size(); j++) {
				g->qsAdd.insert(rpcGroup.users_add(j).id());
			}
			for (int j = 0; j < rpcGroup.users_remove_size(); j++) {
				g->qsRemove.insert(rpcGroup.users_remove(j).id());
			}
			g->qsTemporary = hOldTemp.value(name);
		}
		for (int i = 0; i < request.acls_size(); i++) {
			auto &rpcACL = request.acls(i);
			acl = new ChanACL(channel);
			acl->bApplyHere = rpcACL.apply_here();
			acl->bApplySubs = rpcACL.apply_subs();
			if (rpcACL.has_user()) {
				acl->iUserId = rpcACL.user().id();
			}
			if (rpcACL.has_group() && rpcACL.group().has_name()) {
				acl->qsGroup = u8(rpcACL.group().name());
			}
			acl->pDeny = static_cast<ChanACL::Permissions>(rpcACL.deny()) & ChanACL::All;
			acl->pAllow = static_cast<ChanACL::Permissions>(rpcACL.allow()) & ChanACL::All;
		}
	}

	server->clearACLCache();
	server->updateChannel(channel);

	end();
}

void V1_ACLGetEffectivePermissions::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);
	auto channel = MustChannel(server, request);

	auto flags = int(server->effectivePermissions(user, channel));

	::MurmurRPC::ACL rpcACL;
	rpcACL.set_allow(::MurmurRPC::ACL_Permission(flags));
	end(rpcACL);
}

void V1_ACLAddTemporaryGroup::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);
	auto channel = MustChannel(server, request);

	if (!request.has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing name");
	}

	QString qsgroup = u8(request.name());
	if (qsgroup.isEmpty()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "empty name");
	}

	{
		QWriteLocker wl(&server->qrwlVoiceThread);

		::Group *g = channel->qhGroups.value(qsgroup);
		if (!g) {
			g = new ::Group(channel, qsgroup);
		}

		g->qsTemporary.insert(-user->uiSession);
	}

	server->clearACLCache(user);

	end();
}

void V1_ACLRemoveTemporaryGroup::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);
	auto channel = MustChannel(server, request);

	if (!request.has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing name");
	}

	QString qsgroup = u8(request.name());
	if (qsgroup.isEmpty()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "empty name");
	}

	{
		QWriteLocker wl(&server->qrwlVoiceThread);

		::Group *g = channel->qhGroups.value(qsgroup);
		if (!g) {
			g = new ::Group(channel, qsgroup);
		}

		g->qsTemporary.remove(-user->uiSession);
	}

	server->clearACLCache(user);

	end();
}

void V1_AuthenticatorStream::impl(bool) {
	auto onInitialize = [this] (V1_AuthenticatorStream *, bool ok) {
		if (!ok) {
			finish(ok);
			return;
		}
		if (!request.has_initialize()) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing initialize");
		}
		auto server = MustServer(request.initialize());
		QMutexLocker l(&rpc->qmAuthenticatorsLock);
		rpc->removeAuthenticator(server);
		rpc->m_authenticators.insert(server->iServerNum, this);
	};
	stream.Read(&request, callback(onInitialize));
}

void V1_AuthenticatorStream::done(bool) {
	auto i = std::find(rpc->m_authenticators.begin(), rpc->m_authenticators.end(), this);
	if (i != rpc->m_authenticators.end()) {
		rpc->m_authenticators.erase(i);
	}
	deref();
}

void V1_DatabaseUserQuery::impl(bool) {
	auto server = MustServer(request);

	QString filter;
	if (request.has_filter()) {
		filter = u8(request.filter());
	}
	auto users = server->getRegisteredUsers(filter);

	::MurmurRPC::DatabaseUser_List list;
	list.mutable_server()->set_id(server->iServerNum);

	for (auto itr = users.constBegin(); itr != users.constEnd(); ++itr) {
		auto user = list.add_users();
		user->mutable_server()->set_id(server->iServerNum);
		user->set_id(itr.key());
		user->set_name(u8(itr.value()));
	}

	end(list);
}

void V1_DatabaseUserGet::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing id");
	}
	auto info = server->getRegistration(request.id());
	if (info.isEmpty()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}
	auto texture = server->getUserTexture(request.id());

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	ToRPC(server, info, texture, &rpcDatabaseUser);
	end(rpcDatabaseUser);
}

void V1_DatabaseUserUpdate::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing id");
	}
	if (!server->isUserId(request.id())) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}

	QMap<int, QString> info;
	FromRPC(request, info);

	if (!server->setInfo(request.id(), info)) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}
	if (request.has_texture()) {
		server->setTexture(request.id(), QByteArray(request.texture().c_str(), request.texture().size()));
	}

	if (info.contains(ServerDB::User_Name) || info.contains(ServerDB::User_Comment)) {
		foreach(::ServerUser *u, server->qhUsers) {
			if (static_cast<unsigned int>(u->iId) == request.id()) {
				QString name = u->qsName;
				QString comment = u->qsComment;
				if (info.contains(ServerDB::User_Name)) {
					name = info.value(ServerDB::User_Name);
				}
				if (info.contains(ServerDB::User_Comment)) {
					comment = info.value(ServerDB::User_Comment);
				}
				server->setUserState(u, u->cChannel, u->bMute, u->bDeaf, u->bSuppress, u->bPrioritySpeaker, name, comment);
				break;
			}
		}
	}

	end();
}

void V1_DatabaseUserRegister::impl(bool) {
	auto server = MustServer(request);

	QMap<int, QString> info;
	FromRPC(request, info);

	auto userid = server->registerUser(info);
	if (userid < 0) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}
	QByteArray texture;
	if (request.has_texture()) {
		texture = QByteArray(request.texture().c_str(), request.texture().size());
		server->setTexture(request.id(), texture);
	}

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	rpcDatabaseUser.set_id(userid);
	ToRPC(server, info, texture, &rpcDatabaseUser);
	end(rpcDatabaseUser);
}

void V1_DatabaseUserDeregister::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing id");
	}
	if (!server->unregisterUser(request.id())) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}

	end();
}

void V1_DatabaseUserVerify::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing name");
	}
	if (!request.has_password()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing password");
	}

	auto name = u8(request.name());
	auto password = u8(request.password());

	auto ret = server->authenticate(name, password);
	switch (ret) {
	case -1:
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid username and/or password");
	case -2:
		throw ::grpc::Status(::grpc::NOT_FOUND, "unknown user");
	case -3:
		throw ::grpc::Status(::grpc::UNAVAILABLE, "authenticator temporarily unavailable");
	}

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	rpcDatabaseUser.mutable_server()->set_id(server->iServerNum);
	rpcDatabaseUser.set_id(ret);
	end(rpcDatabaseUser);
}

void V1_RedirectWhisperGroupAdd::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	if (!request.source().has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing source name");
	}
	if (!request.target().has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing target name");
	}

	QString qssource = u8(request.source().name());
	QString qstarget = u8(request.target().name());

	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		user->qmWhisperRedirect.insert(qssource, qstarget);
	}

	server->clearACLCache(user);

	end();
}

void V1_RedirectWhisperGroupRemove::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	if (!request.source().has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing source name");
	}

	QString qssource = u8(request.source().name());

	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		user->qmWhisperRedirect.remove(qssource);
	}

	server->clearACLCache(user);

	end();
}

}
}
