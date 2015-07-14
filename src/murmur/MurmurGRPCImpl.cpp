/* Copyright (C) 2015, Tim Cooper <tim.cooper@layeh.com>

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

#ifndef Q_MOC_RUN
# include <boost/function.hpp>
#endif

#include "murmur_pch.h"

#include "Mumble.pb.h"

#include "../Message.h"
#include "../Group.h"
#include "MurmurGRPCImpl.h"
#include "ServerDB.h"
#include "ServerUser.h"
#include "Server.h"
#include "Channel.h"

#include "MurmurRPC.proto.Wrapper.cpp"

static MurmurRPCImpl *service;

void RPCStart() {
	const QString &address = meta->mp.qsGRPCAddress;
	if (address.isEmpty()) {
		return;
	}
	service = new MurmurRPCImpl(address);
}

void RPCStop() {
	if (service) {
		delete service;
	}
}

MurmurRPCImpl::MurmurRPCImpl(const QString &address) : qtCleanup(this) {
	::grpc::ServerBuilder builder;
	builder.AddListeningPort(u8(address), grpc::InsecureServerCredentials());
	builder.RegisterAsyncService(&aACLService);
	builder.RegisterAsyncService(&aAudioService);
	builder.RegisterAsyncService(&aAuthenticatorService);
	builder.RegisterAsyncService(&aBanService);
	builder.RegisterAsyncService(&aChannelService);
	builder.RegisterAsyncService(&aConfigService);
	builder.RegisterAsyncService(&aContextActionService);
	builder.RegisterAsyncService(&aDatabaseService);
	builder.RegisterAsyncService(&aLogService);
	builder.RegisterAsyncService(&aMetaService);
	builder.RegisterAsyncService(&aServerService);
	builder.RegisterAsyncService(&aTextMessageService);
	builder.RegisterAsyncService(&aTreeService);
	builder.RegisterAsyncService(&aUserService);
	mCQ = builder.AddCompletionQueue();
	mServer = builder.BuildAndStart();
	meta->connectListener(this);
	connect(&qtCleanup, SIGNAL(timeout()), this, SLOT(cleanup()));
	qtCleanup.setSingleShot(false);
	qtCleanup.start(1000 * 60);
	start();
}

MurmurRPCImpl::~MurmurRPCImpl() {
}

void MurmurRPCImpl::cleanup() {
	for (auto i = qsMetaServiceListeners.begin(); i != qsMetaServiceListeners.end(); ) {
		auto listener = *i;
		if (listener->context.IsCancelled()) {
			qsMetaServiceListeners.erase(i);
			listener->deref();
		} else {
			++i;
		}
	}

	for (auto i = qhContextActionListeners.begin(); i != qhContextActionListeners.end(); ) {
		auto &ref = i.value();
		for (auto j = ref.begin(); j != ref.end(); ) {
			auto listener = j.value();
			if (listener->context.IsCancelled()) {
				ref.erase(j);
				listener->deref();
			} else {
				++j;
			}
		}

		if (ref.isEmpty()) {
			qhContextActionListeners.erase(i);
		} else {
			++i;
		}
	}

	for (auto i = qmhServerServiceListeners.begin(); i != qmhServerServiceListeners.end(); ) {
		auto listener = i.value();
		if (listener->context.IsCancelled()) {
			qmhServerServiceListeners.erase(i);
			listener->deref();
		} else {
			++i;
		}
	}

	for (auto i = qhAuthenticators.begin(); i != qhAuthenticators.end(); ) {
		auto listener = i.value();
		if (listener->context.IsCancelled()) {
			qhAuthenticators.erase(i);
			listener->deref();
		} else {
			++i;
		}
	}
}

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

	ru->set_tcp_only(!su->bUdp);

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

void MurmurRPCImpl::sendMetaEvent(const ::MurmurRPC::Event &e) {
	auto listeners = qsMetaServiceListeners;

	for (auto i = listeners.constBegin(); i != listeners.constEnd(); ++i) {
		auto listener = *i;
		auto cb = [this, listener] (::MurmurRPC::Wrapper::MetaService_Events *, bool ok) {
			if (ok) {
				return;
			}
			this->qsMetaServiceListeners.remove(listener);
			listener->deref();
		};
		listener->stream.Write(e, listener->callback(cb));
	}
}

void MurmurRPCImpl::started(::Server *server) {
	server->connectListener(this);
	server->connectAuthenticator(this);
	connect(server, SIGNAL(contextAction(const User *, const QString &, unsigned int, int)), this, SLOT(contextAction(const User *, const QString &, unsigned int, int)));

	::MurmurRPC::Event rpcEvent;
	rpcEvent.set_type(::MurmurRPC::Event_Type_ServerStarted);
	rpcEvent.mutable_server()->set_id(server->iServerNum);
	sendMetaEvent(rpcEvent);
}

void MurmurRPCImpl::stopped(::Server *server) {
	::MurmurRPC::Event rpcEvent;
	rpcEvent.set_type(::MurmurRPC::Event_Type_ServerStopped);
	rpcEvent.mutable_server()->set_id(server->iServerNum);
	sendMetaEvent(rpcEvent);
}

void MurmurRPCImpl::removeAuthenticator(const ::Server *s) {
	auto authenticator = qhAuthenticators.value(s->iServerNum);
	if (!authenticator) {
		return;
	}
	if (!authenticator->context.IsCancelled()) {
		authenticator->error(::grpc::Status(::grpc::CANCELLED, "authenticator detached"));
	}
	qhAuthenticators.remove(s->iServerNum);
	authenticator->deref();
}

void MurmurRPCImpl::authenticateSlot(int &res, QString &uname, int sessionId, const QList<QSslCertificate> &certlist, const QString &certhash, bool certstrong, const QString &pw) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::registerUserSlot(int &res, const QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::unregisterUserSlot(int &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::getRegisteredUsersSlot(const QString &filter, QMap<int, QString> &res) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::getRegistrationSlot(int &res, int id, QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::setInfoSlot(int &res, int id, const QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::setTextureSlot(int &res, int id, const QByteArray &texture) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::nameToIdSlot(int &res, const QString &name) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::idToNameSlot(QString &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::idToTextureSlot(QByteArray &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto authenticator = RPCCall::Ref<::MurmurRPC::Wrapper::AuthenticatorService_Stream>(qhAuthenticators.value(s->iServerNum));
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

void MurmurRPCImpl::sendServerEvent(const ::Server *s, const ::MurmurRPC::Server_Event &e) {
	auto listeners = qmhServerServiceListeners;
	auto serverID = s->iServerNum;
	auto i = listeners.find(serverID);

	for ( ; i != listeners.end() && i.key() == serverID; ++i) {
		auto listener = i.value();
		auto cb = [this, listener, serverID] (::MurmurRPC::Wrapper::ServerService_Events *, bool ok) {
			if (ok) {
				return;
			}
			this->qmhServerServiceListeners.remove(serverID, listener);
			listener->deref();
		};
		listener->stream.Write(e, listener->callback(cb));
	}
}

void MurmurRPCImpl::userStateChanged(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserStateChanged);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

void MurmurRPCImpl::userTextMessage(const ::User *user, const ::TextMessage &message) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserTextMessage);
	ToRPC(s, user, event.mutable_user());
	ToRPC(s, user, message, event.mutable_message());
	sendServerEvent(s, event);
}

void MurmurRPCImpl::userConnected(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserConnected);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

void MurmurRPCImpl::userDisconnected(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserDisconnected);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

void MurmurRPCImpl::channelStateChanged(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelStateChanged);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

void MurmurRPCImpl::channelCreated(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelCreated);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

void MurmurRPCImpl::channelRemoved(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelRemoved);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

void MurmurRPCImpl::contextAction(const ::User *user, const QString &action, unsigned int session, int channel) {
	::Server *s = qobject_cast< ::Server *> (sender());
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
	auto listeners = this->qhContextActionListeners.value(serverID);
	auto i = listeners.find(action);
	for ( ; i != listeners.end() && i.key() == action; ++i) {
		auto listener = i.value();
		auto cb = [this, listener, serverID, action] (::MurmurRPC::Wrapper::ContextActionService_Events *, bool ok) {
			if (ok) {
				return;
			}
			if (this->qhContextActionListeners.contains(serverID)) {
				this->qhContextActionListeners[serverID].remove(action, listener);
				listener->deref();
			}
		};
		listener->stream.Write(ca, listener->callback(cb));
	}
}

::ServerUser *MustUser(const Server *server, unsigned int session) {
	auto user = server->qhUsers.value(session);
	if (!user) {
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

void MurmurRPCImpl::run() {
	MurmurRPC::Wrapper::ACLService_Init(this, &aACLService);
	MurmurRPC::Wrapper::AudioService_Init(this, &aAudioService);
	MurmurRPC::Wrapper::AuthenticatorService_Init(this, &aAuthenticatorService);
	MurmurRPC::Wrapper::BanService_Init(this, &aBanService);
	MurmurRPC::Wrapper::ChannelService_Init(this, &aChannelService);
	MurmurRPC::Wrapper::ConfigService_Init(this, &aConfigService);
	MurmurRPC::Wrapper::ContextActionService_Init(this, &aContextActionService);
	MurmurRPC::Wrapper::DatabaseService_Init(this, &aDatabaseService);
	MurmurRPC::Wrapper::LogService_Init(this, &aLogService);
	MurmurRPC::Wrapper::MetaService_Init(this, &aMetaService);
	MurmurRPC::Wrapper::ServerService_Init(this, &aServerService);
	MurmurRPC::Wrapper::TextMessageService_Init(this, &aTextMessageService);
	MurmurRPC::Wrapper::TreeService_Init(this, &aTreeService);
	MurmurRPC::Wrapper::UserService_Init(this, &aUserService);

	void *tag;
	bool ok;
	while (true) {
		if (!mCQ->Next(&tag, &ok)) {
			break;
		}
		if (tag != nullptr) {
			auto op = static_cast<boost::function<void(bool)> *>(tag);
			(*op)(ok);
			delete op;
		}
	}
	// TODO(grpc): cleanup allocated memory? not super important, because murmur should be exiting now
}

namespace MurmurRPC {
namespace Wrapper {

void ServerService_Create::impl(bool) {
	auto id = ServerDB::addServer();

	::MurmurRPC::Server rpcServer;
	rpcServer.set_id(id);
	stream.Finish(rpcServer, ::grpc::Status::OK, done());
}

void ServerService_Query::impl(bool) {
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

	stream.Finish(list, ::grpc::Status::OK, done());
}

void ServerService_Get::impl(bool) {
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
	stream.Finish(rpcServer, ::grpc::Status::OK, done());
}

void ServerService_Start::impl(bool) {
	auto serverID = MustServerID(request);

	if (!meta->boot(serverID)) {
		throw ::grpc::Status(::grpc::UNKNOWN, "server could not be started, or is already started");
	}

	::MurmurRPC::Void vd;
	stream.Finish(vd, ::grpc::Status::OK, done());
}

void ServerService_Stop::impl(bool) {
	auto server = MustServer(request);
	meta->kill(server->iServerNum);

	::MurmurRPC::Void vd;
	stream.Finish(vd, ::grpc::Status::OK, done());
}

void ServerService_Remove::impl(bool) {
	auto serverID = MustServerID(request);

	if (meta->qhServers.value(serverID)) {
		throw ::grpc::Status(::grpc::FAILED_PRECONDITION, "cannot remove started server");
	}

	ServerDB::deleteServer(serverID);

	::MurmurRPC::Void vd;
	stream.Finish(vd, ::grpc::Status::OK, done());
}

void ServerService_Events::impl(bool) {
	auto server = MustServer(request);
	rpc->qmhServerServiceListeners.insert(server->iServerNum, this);
}

void MetaService_GetUptime::impl(bool) {
	::MurmurRPC::Uptime uptime;
	uptime.set_secs(meta->tUptime.elapsed()/1000000LL);
	stream.Finish(uptime, ::grpc::Status::OK, done());
}

void MetaService_GetVersion::impl(bool) {
	::MurmurRPC::Version version;
	int major, minor, patch;
	QString release;
	Meta::getVersion(major, minor, patch, release);
	version.set_version(major << 16 | minor << 8 | patch);
	version.set_release(u8(release));
	stream.Finish(version, ::grpc::Status::OK, done());
}

void MetaService_Events::impl(bool) {
	rpc->qsMetaServiceListeners.insert(this);
}

void ContextActionService_Add::impl(bool) {
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

	::MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(request.action());
	mpcam.set_text(request.text());
	mpcam.set_context(request.context());
	mpcam.set_operation(::MumbleProto::ContextActionModify_Operation_Add);
	server->sendMessage(user, mpcam);

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void ContextActionService_Remove::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}

	::MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(request.action());
	mpcam.set_operation(::MumbleProto::ContextActionModify_Operation_Remove);

	if (request.has_user()) {
		// Remove context action from specific user
		auto user = MustUser(server, request);
		server->sendMessage(user, mpcam);
	} else {
		// Remove context action from all users
		server->sendAll(mpcam);
	}

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void ContextActionService_Events::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}

	rpc->qhContextActionListeners[server->iServerNum].insert(u8(request.action()), this);
}

void TextMessageService_Send::impl(bool) {
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

	server->sendTextMessage(tm);

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void LogService_Query::impl(bool) {
	auto serverID = MustServerID(request);

	int total = ::ServerDB::getLogLen(serverID);
	if (total < 0) {
		throw ::grpc::Status(::grpc::StatusCode::UNAVAILABLE, "could not access database");
	}

	::MurmurRPC::Log_List list;
	list.mutable_server()->set_id(serverID);
	list.set_total(total);

	if (!request.has_min() || !request.has_max()) {
		stream.Finish(list, ::grpc::Status::OK, done());
		return;
	}
	list.set_min(request.min());
	list.set_max(request.max());

	auto dblog = ::ServerDB::getLog(serverID, request.min(), request.max());
	foreach(const ::ServerDB::LogRecord &record, dblog) {
		auto rpcLog = list.add_entries();
		ToRPC(serverID, record, rpcLog);
	}

	stream.Finish(list, ::grpc::Status::OK, done());
}

void ConfigService_Get::impl(bool) {
	auto serverID = MustServerID(request);
	auto config = ServerDB::getAllConf(serverID);

	::MurmurRPC::Config rpcConfig;
	rpcConfig.mutable_server()->set_id(serverID);
	auto &fields = *rpcConfig.mutable_fields();
	for (auto i = config.constBegin(); i != config.constEnd(); ++i) {
		fields[u8(i.key())] = u8(i.value());
	}

	stream.Finish(rpcConfig, ::grpc::Status::OK, done());
}

void ConfigService_GetField::impl(bool) {
	auto serverID = MustServerID(request);
	if (!request.has_key()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing key");
	}
	::MurmurRPC::Config_Field rpcField;
	rpcField.mutable_server()->set_id(serverID);
	rpcField.set_key(request.key());
	rpcField.set_value(u8(ServerDB::getConf(serverID, u8(request.key()), QVariant()).toString()));
	stream.Finish(rpcField, ::grpc::Status::OK, done());
}

void ConfigService_SetField::impl(bool) {
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

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void ConfigService_GetDefaults::impl(bool) {
	::MurmurRPC::Config rpcConfig;
	auto &fields = *rpcConfig.mutable_fields();
	for (auto i = meta->mp.qmConfig.constBegin(); i != meta->mp.qmConfig.constEnd(); ++i) {
		fields[u8(i.key())] = u8(i.value());
	}

	stream.Finish(rpcConfig, ::grpc::Status::OK, done());
}

void ChannelService_Query::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::Channel_List list;
	list.mutable_server()->set_id(server->iServerNum);

	foreach(const ::Channel *channel, server->qhChannels) {
		auto rpcChannel = list.add_channels();
		ToRPC(server, channel, rpcChannel);
	}

	stream.Finish(list, ::grpc::Status::OK, done());
}

void ChannelService_Get::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MurmurRPC::Channel rpcChannel;
	ToRPC(server, channel, &rpcChannel);
	stream.Finish(rpcChannel, ::grpc::Status::OK, done());
}

void ChannelService_Add::impl(bool) {
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

	::Channel *nc = server->addChannel(parent, qsName);
	server->updateChannel(nc);
	int newid = nc->iId;

	::MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(newid);
	mpcs.set_parent(parent->iId);
	mpcs.set_name(request.name());
	server->sendAll(mpcs);

	::MurmurRPC::Channel resChannel;
	ToRPC(server, nc, &resChannel);
	stream.Finish(resChannel, grpc::Status::OK, done());
}

void ChannelService_Remove::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	if (!channel->cParent) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "cannot remove the root channel");
	}
	server->removeChannel(channel);
	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void ChannelService_Update::impl(bool) {
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
	if (!server->setChannelState(mpcs, err)) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, u8(err));
	}

	::MurmurRPC::Channel rpcChannel;
	ToRPC(server, channel, &rpcChannel);
	stream.Finish(rpcChannel, grpc::Status::OK, done());
}

void UserService_Query::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::User_List list;
	list.mutable_server()->set_id(server->iServerNum);

	foreach(const ::ServerUser *user, server->qhUsers) {
		auto rpcUser = list.add_users();
		ToRPC(server, user, rpcUser);
	}

	stream.Finish(list, grpc::Status::OK, done());
}

void UserService_Get::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::User rpcUser;

	if (request.has_session()) {
		// Lookup user by session
		auto user = MustUser(server, request);
		ToRPC(server, user, &rpcUser);
		stream.Finish(rpcUser, grpc::Status::OK, done());
		return;
	} else if (request.has_name()) {
		// Lookup user by name
		QString qsName = u8(request.name());
		foreach(const ::ServerUser *user, server->qhUsers) {
			if (user->qsName == qsName) {
				ToRPC(server, user, &rpcUser);
				stream.Finish(rpcUser, grpc::Status::OK, done());
				return;
			}
		}
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid user");
	}

	throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "session or name required");
}

void UserService_Update::impl(bool) {
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
	stream.Finish(rpcUser, grpc::Status::OK, done());
}

void UserService_Kick::impl(bool) {
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

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void TreeService_Query::impl(bool) {
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
		qSort(users.begin(), users.end(), [] (const ::User *a, const ::User *b) -> bool {
			return ::User::lessThan(a, b);
		});
		foreach(const ::User *u, users) {
			auto rpcUser = currentTree->add_users();
			ToRPC(server, u, rpcUser);
		}

		QList< ::Channel *> channels = currentChannel->qlChannels;
		qSort(channels.begin(), channels.end(), [] (const ::Channel *a, const ::Channel *b) -> bool {
			return ::Channel::lessThan(a, b);
		});
		foreach(const ::Channel *subChannel, channels) {
			auto subTree = currentTree->add_children();
			qQueue.enqueue(qMakePair(subChannel, subTree));
		}
	}

	stream.Finish(root, grpc::Status::OK, done());
}

void BanService_Get::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::Ban_List list;
	list.mutable_server()->set_id(server->iServerNum);
	foreach(const ::Ban &ban, server->qlBans) {
		auto rpcBan = list.add_bans();
		ToRPC(server, ban, rpcBan);
	}

	stream.Finish(list, grpc::Status::OK, done());
}

void BanService_Set::impl(bool) {
	auto server = MustServer(request);
	server->qlBans.clear();

	for (int i = 0; i < request.bans_size(); i++) {
		const auto &rpcBan = request.bans(i);
		::Ban ban;
		FromRPC(server, rpcBan, ban);
		server->qlBans << ban;
	}
	server->saveBans();

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void ACLService_Get::impl(bool) {
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

	stream.Finish(list, grpc::Status::OK, done());
}

void ACLService_Set::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::Group *g;
	::ChanACL *acl;

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

	server->clearACLCache();
	server->updateChannel(channel);

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void ACLService_GetEffectivePermissions::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);
	auto channel = MustChannel(server, request);

	auto flags = int(server->effectivePermissions(user, channel));

	::MurmurRPC::ACL rpcACL;
	rpcACL.set_allow(::MurmurRPC::ACL_Permission(flags));
	stream.Finish(rpcACL, grpc::Status::OK, done());
}

void ACLService_AddTemporaryGroup::impl(bool) {
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

	::Group *g = channel->qhGroups.value(qsgroup);
	if (!g) {
		g = new ::Group(channel, qsgroup);
	}

	g->qsTemporary.insert(-user->uiSession);
	server->clearACLCache(user);

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void ACLService_RemoveTemporaryGroup::impl(bool) {
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

	::Group *g = channel->qhGroups.value(qsgroup);
	if (!g) {
		g = new ::Group(channel, qsgroup);
	}

	g->qsTemporary.remove(-user->uiSession);
	server->clearACLCache(user);

	::MurmurRPC::Void vd;
	stream.Finish(vd, ::grpc::Status::OK, done());
}

void AuthenticatorService_Stream::impl(bool) {
	auto onInitialize = [this] (AuthenticatorService_Stream *, bool ok) {
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
		rpc->qhAuthenticators.insert(server->iServerNum, this);
	};
	stream.Read(&request, callback(onInitialize));
}

void DatabaseService_Query::impl(bool) {
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

	stream.Finish(list, grpc::Status::OK, done());
}

void DatabaseService_Get::impl(bool) {
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
	stream.Finish(rpcDatabaseUser, grpc::Status::OK, done());
}

void DatabaseService_Update::impl(bool) {
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
			if (u->iId == request.id()) {
				QString name = u->qsName;
				QString comment = u->qsComment;
				if (info.contains(ServerDB::User_Name)) {
					comment = info.value(ServerDB::User_Name);
				}
				if (info.contains(ServerDB::User_Comment)) {
					comment = info.value(ServerDB::User_Comment);
				}
				server->setUserState(u, u->cChannel, u->bMute, u->bDeaf, u->bSuppress, u->bPrioritySpeaker, name, comment);
				break;
			}
		}
	}

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void DatabaseService_Register::impl(bool) {
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
	stream.Finish(rpcDatabaseUser, grpc::Status::OK, done());
}

void DatabaseService_Deregister::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing id");
	}
	if (!server->unregisterUser(request.id())) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void DatabaseService_Verify::impl(bool) {
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
	stream.Finish(rpcDatabaseUser, grpc::Status::OK, done());
}

void AudioService_AddRedirectWhisperGroup::impl(bool) {
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
	user->qmWhisperRedirect.insert(qssource, qstarget);

	server->clearACLCache(user);

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

void AudioService_RemoveRedirectWhisperGroup::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	if (!request.source().has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing source name");
	}

	QString qssource = u8(request.source().name());
	user->qmWhisperRedirect.remove(qssource);

	server->clearACLCache(user);

	::MurmurRPC::Void vd;
	stream.Finish(vd, grpc::Status::OK, done());
}

}
}
