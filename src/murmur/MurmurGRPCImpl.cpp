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
	// TODO(grpc): cleanup any old connections that are stored in our listener
	// lists.
	// TODO(grpc): qhContextActionListeners, qsMetaServiceListeners
}

void MurmurRPCImpl::started(::Server *server) {
	server->connectListener(this);
	connect(server, SIGNAL(contextAction(const User *, const QString &, unsigned int, int)), this, SLOT(contextAction(const User *, const QString &, unsigned int, int)));

	::MurmurRPC::Event rpcEvent;
	rpcEvent.set_type(::MurmurRPC::Event_Type_ServerStarted);
	rpcEvent.mutable_server()->set_id(server->iServerNum);
	foreach(auto listener, qsMetaServiceListeners) {
		// TODO(grpc): remove listener on error
		listener->response.Write(rpcEvent, nullptr);
	}
}

void MurmurRPCImpl::stopped(::Server *server) {
	::MurmurRPC::Event rpcEvent;
	rpcEvent.set_type(::MurmurRPC::Event_Type_ServerStopped);
	rpcEvent.mutable_server()->set_id(server->iServerNum);
	foreach(auto listener, qsMetaServiceListeners) {
		// TODO(grpc): remove listener on error
		listener->response.Write(rpcEvent, nullptr);
	}
}

void MurmurRPCImpl::userStateChanged(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());
}

void MurmurRPCImpl::userTextMessage(const ::User *user, const ::TextMessage &message) {
	::Server *s = qobject_cast< ::Server *> (sender());
}

void MurmurRPCImpl::userConnected(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());
}

void MurmurRPCImpl::userDisconnected(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());
}

void MurmurRPCImpl::channelStateChanged(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());
}

void MurmurRPCImpl::channelCreated(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());
}

void MurmurRPCImpl::channelRemoved(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());
}

void MurmurRPCImpl::contextAction(const ::User *user, const QString &action, unsigned int session, int channel) {
	::Server *s = qobject_cast< ::Server *> (sender());
	::MurmurRPC::ContextAction ca;
	ca.mutable_server()->set_id(s->iServerNum);
	ca.mutable_actor()->mutable_server()->set_id(s->iServerNum);
	ca.mutable_actor()->set_session(user->uiSession);
	ca.set_action(u8(action));
	// TODO(grpc): validate these values?
	ca.mutable_user()->mutable_server()->set_id(s->iServerNum);
	ca.mutable_user()->set_session(session);
	ca.mutable_channel()->mutable_server()->set_id(s->iServerNum);
	ca.mutable_channel()->set_id(channel);

	auto &ref = this->qhContextActionListeners[s->iServerNum];
	auto itr = ref.find(action);
	for ( ; itr != ref.end() && itr.key() == action; ++itr) {
		auto listener = itr.value();
		auto cb = [this, s, action](::MurmurRPC::Wrapper::ContextActionService_Events *listener, bool ok) {
			if (ok) {
				return;
			}
			this->qhContextActionListeners[s->iServerNum].remove(action, listener);
		};
		listener->response.Write(ca, listener->callback(cb));
	}
}

// TODO(grpc): ensure that all implementation methods are using the correct
// Must* kind (Must*, MustExist*, etc.)

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

::Channel *MustChannel(const Server *server, int channel_id) {
	auto channel = server->qhChannels.value(channel_id);
	if (!channel) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid channel");
	}
	return channel;
}

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

	const ServerUser *su=static_cast<const ServerUser *>(u);
	ru->set_online_secs(su->bwr.onlineSeconds());
	ru->set_bytes_per_sec(su->bwr.bandwidth());
	ru->mutable_version()->set_version(su->uiVersion);
	ru->mutable_version()->set_release(u8(su->qsRelease));
	ru->mutable_version()->set_os(u8(su->qsOS));
	ru->mutable_version()->set_os_version(u8(su->qsOSVersion));
	ru->set_plugin_identity(u8(su->qsIdentity));
	ru->set_plugin_context(su->ssContext);
	ru->set_idle_secs(su->bwr.idleSeconds());
	ru->set_udp_ping(su->dUDPPingAvg);
	ru->set_tcp_ping(su->dTCPPingAvg);

	ru->set_tcp_only(!su->bUdp);

	ru->set_address(su->haAddress.toStdString());
}

void ToRPC(const ::Server *srv, const QMap<int, QString> &info, ::MurmurRPC::DatabaseUser *du) {
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
		ra->set_group(u8(acl->qsGroup));
	}

	ra->set_allow(MurmurRPC::ACL_Permission(int(acl->pAllow)));
	ra->set_deny(MurmurRPC::ACL_Permission(int(acl->pDeny)));
}

void ToRPC(const ::Server *srv, const ::Group *g, ::MurmurRPC::ACL_Group *rg) {
	rg->set_name(u8(g->qsName));
	rg->set_inherit(g->bInherit);
	rg->set_inheritable(g->bInheritable);
}

namespace MurmurRPC {
namespace Wrapper {

void ServerService_Create::impl(bool) {
	auto id = ServerDB::addServer();

	::MurmurRPC::Server rpcServer;
	rpcServer.set_id(id);
	response.Finish(rpcServer, ::grpc::Status::OK, done());
}

void ServerService_Query::impl(bool) {
	::MurmurRPC::Server_List list;

	foreach(int id, ServerDB::getAllServers()) {
		auto rpcServer = list.add_servers();
		rpcServer->set_id(id);
		try {
			auto server = MustServer(id);
			rpcServer->set_running(true);
			rpcServer->set_uptime(server->tUptime.elapsed()/1000000LL);
		} catch (::grpc::Status &ex) {
		}
	}

	response.Finish(list, ::grpc::Status::OK, done());
}

void ServerService_Get::impl(bool) {
	// TODO(grpc): fix for non-started servers
	auto server = MustServer(request);

	::MurmurRPC::Server rpcServer;
	rpcServer.set_id(server->iServerNum);
	rpcServer.set_running(true);  // TODO(grpc): fix me
	rpcServer.set_uptime(server->tUptime.elapsed()/1000000LL);
	response.Finish(rpcServer, ::grpc::Status::OK, done());
}

void ServerService_Start::impl(bool) {
	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server id");
	}
	if (!meta->boot(request.id())) {
		throw ::grpc::Status(::grpc::UNKNOWN, "server could not be started, or is already started");
	}

	::MurmurRPC::Void vd;
	response.Finish(vd, ::grpc::Status::OK, done());
}

void ServerService_Stop::impl(bool) {
	auto server = MustServer(request);
	meta->kill(server->iServerNum);

	::MurmurRPC::Void vd;
	response.Finish(vd, ::grpc::Status::OK, done());
}

void ServerService_Remove::impl(bool) {
	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server id");
	}
	if (!ServerDB::serverExists(request.id())) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid server");
	}
	if (meta->qhServers.value(request.id())) {
		throw ::grpc::Status(::grpc::FAILED_PRECONDITION, "cannot remove started server");
	}

	ServerDB::deleteServer(request.id());

	::MurmurRPC::Void vd;
	response.Finish(vd, ::grpc::Status::OK, done());
}

void ServerService_Events::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MetaService_GetUptime::impl(bool) {
	::MurmurRPC::Uptime uptime;
	uptime.set_secs(meta->tUptime.elapsed()/1000000LL);
	response.Finish(uptime, ::grpc::Status::OK, done());
}

void MetaService_GetVersion::impl(bool) {
	::MurmurRPC::Version version;
	int major, minor, patch;
	QString release;
	Meta::getVersion(major, minor, patch, release);
	version.set_version(major << 16 | minor << 8 | patch);
	version.set_release(u8(release));
	response.Finish(version, ::grpc::Status::OK, done());
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
	// TODO(grpc): verify that context is valid value?
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
	response.Finish(vd, grpc::Status::OK, done());
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
	response.Finish(vd, grpc::Status::OK, done());
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

	::MumbleProto::TextMessage mptm;
	mptm.set_message(request.text());
	if (request.has_actor()) {
		::ServerUser *actor = server->qhUsers.value(request.actor().session());
		if (actor) {
			// TODO(grpc): verify actor?
			mptm.set_actor(actor->uiSession);
		}
	}

	// Broadcast
	if (!request.users_size() && !request.channels_size() && !request.trees_size()) {
		server->sendAll(mptm);
		::MurmurRPC::Void vd;
		response.Finish(vd, grpc::Status::OK, done());
		return;
	}

	// Single targets
	for (int i = 0; i < request.users_size(); i++) {
		auto target = request.users(i);
		// TODO(grpc): more validation?
		if (!target.has_session()) {
			continue;
		}
		try {
			auto user = MustUser(server, target.session());
			mptm.add_session(user->uiSession);
			server->sendMessage(user, mptm);
		} catch (::grpc::Status &ex) {
		}
		mptm.clear_session();
	}

	// Channel targets
	QSet<::Channel *> chans;

	for (int i = 0; i < request.channels_size(); i++) {
		// TODO(grpc): more validation?
		auto target = request.channels(i);
		if (!target.has_id()) {
			continue;
		}
		try {
			auto channel = MustChannel(server, target.id());
			chans.insert(channel);
			mptm.add_channel_id(target.id());
		} catch (::grpc::Status &ex) {
		}
	}

	QQueue<::Channel *> chansQ;
	for (int i = 0; i < request.trees_size(); i++) {
		// TODO(grpc): more validation?
		auto target = request.trees(i);
		if (!target.has_id()) {
			continue;
		}
		try {
			auto channel = MustChannel(server, target.id());
			chansQ.enqueue(channel);
			mptm.add_tree_id(target.id());
		} catch (::grpc::Status &ex) {
		}
	}
	while (!chansQ.isEmpty()) {
		auto c = chansQ.dequeue();
		chans.insert(c);
		foreach(c, c->qlChannels) {
			chansQ.enqueue(c);
		}
	}

	foreach(::Channel *c, chans) {
		foreach(::User *p, c->qlUsers) {
			server->sendMessage(static_cast<::ServerUser *>(p), mptm);
		}
	}

	::MurmurRPC::Void vd;
	response.Finish(vd, grpc::Status::OK, done());
}

void ConfigService_GetDefault::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ConfigService_SetDefault::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ConfigService_Query::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ChannelService_Query::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::Channel_List list;
	list.mutable_server()->set_id(server->iServerNum);

	foreach(const ::Channel *channel, server->qhChannels) {
		auto rpcChannel = list.add_channels();
		ToRPC(server, channel, rpcChannel);
	}

	response.Finish(list, ::grpc::Status::OK, done());
}

void ChannelService_Get::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MurmurRPC::Channel rpcChannel;
	ToRPC(server, channel, &rpcChannel);
	response.Finish(rpcChannel, ::grpc::Status::OK, done());
}

void ChannelService_Add::impl(bool) {
	auto server = MustServer(request);

	if (!request.has_parent() || !request.has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "parent channel and name required");
	}
	// TODO(grpc): verify request->parent().server() ?
	if (!request.parent().has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "parent channel is missing ID");
	}
	::Channel *parent = server->qhChannels.value(request.parent().id());
	if (!parent) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "parent channel does not exist");
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
	response.Finish(resChannel, grpc::Status::OK, done());
}

void ChannelService_Remove::impl(bool) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	if (!channel->cParent) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "cannot remove the root channel");
	}
	server->removeChannel(channel);
	::MurmurRPC::Void vd;
	response.Finish(vd, grpc::Status::OK, done());
}

void ChannelService_Update::impl(bool) {
	auto server = MustServer(request);

	bool changed = false;
	bool updated = false;

	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "id required");
	}
	// TODO(grpc): replace with MustChannel 
	::Channel *channel = server->qhChannels.value(request.id());
	if (!channel) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid channel");
	}

	::MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(channel->iId);


	// Links and parent channel are processed first, because they can return
	// errors. Without doing this, the server state can be changed without
	// notifying users.
	::QSet<::Channel *> newLinksSet;
	for (int i = 0; i < request.links_size(); i++) {
		const ::MurmurRPC::Channel &linkRef = request.links(i);
		// TODO(grpc): verify linkRef.server() ?
		if (!linkRef.has_id()) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "link channel is missing ID");
		}
		::Channel *link = server->qhChannels.value(linkRef.id());
		if (!link) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "link channel does not exist");
		}
		newLinksSet.insert(link);
	}

	if (request.has_parent()) {
		// TODO(grpc): verify request->parent().server() ?
		if (!request.parent().has_id()) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "parent channel is missing ID");
		}
		::Channel *parent = server->qhChannels.value(request.parent().id());
		if (!parent) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "parent channel does not exist");
		}
		if (parent != channel->cParent) {
			::Channel *p = parent;
			while (p) {
				if (p == channel) {
					throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "parent channel cannot be a descendant of channel");
				}
				p = p->cParent;
			}
			if (!server->canNest(parent, channel)) {
				throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "channel cannot be nested in the given parent");
			}
			channel->cParent->removeChannel(channel);
			parent->addChannel(channel);
			mpcs.set_parent(parent->iId);

			changed = true;
			updated = true;
		}
	}

	if (request.has_name()) {
		QString qsName = u8(request.name());
		if (channel->qsName != qsName) {
			channel->qsName = qsName;
			mpcs.set_name(request.name());

			changed = true;
			updated = true;
		}
	}

	const QSet<::Channel *> &oldLinksSet = channel->qsPermLinks;

	if (newLinksSet != oldLinksSet) {
		// Remove
		foreach(::Channel *l, oldLinksSet) {
			if (!newLinksSet.contains(l)) {
				server->removeLink(channel, l);
				mpcs.add_links_remove(l->iId);
			}
		}
		// Add
		foreach(::Channel *l, newLinksSet) {
			if (! oldLinksSet.contains(l)) {
				server->addLink(channel, l);
				mpcs.add_links_add(l->iId);
			}
		}

		changed = true;
	}

	if (request.has_position() && request.position() != channel->iPosition) {
		channel->iPosition = request.position();
		mpcs.set_position(request.position());

		changed = true;
		updated = true;
	}

	if (request.has_description()) {
		QString qsDescription = u8(request.description());
		if (qsDescription != channel->qsDesc) {
			server->hashAssign(channel->qsDesc, channel->qbaDescHash, qsDescription);
			mpcs.set_description(request.description());

			changed = true;
			updated = true;
		}
	}

	if (updated) {
		server->updateChannel(channel);
	}
	if (changed) {
		server->sendAll(mpcs, ~ 0x010202);
		if (mpcs.has_description() && !channel->qbaDescHash.isEmpty()) {
			mpcs.clear_description();
			mpcs.set_description_hash(blob(channel->qbaDescHash));
		}
		server->sendAll(mpcs, 0x010202);
		emit server->channelStateChanged(channel);
	}

	::MurmurRPC::Channel rpcChannel;
	ToRPC(server, channel, &rpcChannel);
	response.Finish(rpcChannel, grpc::Status::OK, done());
}

void UserService_Query::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::User_List list;
	list.mutable_server()->set_id(server->iServerNum);

	foreach(const ::ServerUser *user, server->qhUsers) {
		auto rpcUser = list.add_users();
		ToRPC(server, user, rpcUser);
	}

	response.Finish(list, grpc::Status::OK, done());
}

void UserService_Get::impl(bool) {
	auto server = MustServer(request);

	::MurmurRPC::User rpcUser;

	if (request.has_session()) {
		// Lookup user by session
		auto user = MustUser(server, request);
		ToRPC(server, user, &rpcUser);
		response.Finish(rpcUser, grpc::Status::OK, done());
		return;
	} else if (request.has_name()) {
		// Lookup user by name
		QString qsName = u8(request.name());
		foreach(const ::ServerUser *user, server->qhUsers) {
			if (user->qsName == qsName) {
				ToRPC(server, user, &rpcUser);
				response.Finish(rpcUser, grpc::Status::OK, done());
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
		// TODO(grpc): more validation?
		channel = MustChannel(server, request.channel().id());
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
	// TODO(grpc): support texture updating

	server->setUserState(user, channel, mute, deaf, suppress, prioritySpeaker, name, comment);

	::MurmurRPC::User rpcUser;
	ToRPC(server, user, &rpcUser);
	response.Finish(rpcUser, grpc::Status::OK, done());
}

void UserService_Kick::impl(bool) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	::MumbleProto::UserRemove mpur;
	mpur.set_session(user->uiSession);
	if (request.has_reason()) {
		mpur.set_reason(request.reason());
	}
	server->sendAll(mpur);
	user->disconnectSocket();

	::MurmurRPC::Void vd;
	response.Finish(vd, grpc::Status::OK, done());
}

void TreeService_Get::impl(bool) {
	auto server = MustServer(request);

	auto channel = MustChannel(server, 0);
	::MurmurRPC::Tree root;

	// TODO(grpc): convert to a container of pairs?
	QQueue<const ::Channel *> qChan;
	QQueue<::MurmurRPC::Tree *> qTree;

	qChan.enqueue(channel);
	qTree.enqueue(&root);

	while (!qChan.isEmpty()) {
		auto currentChannel = qChan.dequeue();
		auto currentTree = qTree.dequeue();

		ToRPC(server, currentChannel, currentTree->mutable_channel());
		// TODO(grpc): sort users?
		foreach(const ::User *u, currentChannel->qlUsers) {
			auto rpcUser = currentTree->add_users();
			// TODO(grpc): don't include every user field
			ToRPC(server, u, rpcUser);
		}
		// TODO(grpc): sort channels?
		foreach(const ::Channel *subChannel, currentChannel->qlChannels) {
			auto subTree = currentTree->add_children();
			qChan.enqueue(subChannel);
			qTree.enqueue(subTree);
		}
	}

	response.Finish(root, grpc::Status::OK, done());
}

void ACLService_Get::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_Set::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_GetEffectivePermissions::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_AddTemporaryGroup::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_RemoveTemporaryGroup::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void AuthenticatorService_Stream::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void AuthenticatorService_RegistrationStream::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
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

	response.Finish(list, grpc::Status::OK, done());
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

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	// TODO(grpc): support rpcDatabaseUser.Texture in all DatabaseMethods
	ToRPC(server, info, &rpcDatabaseUser);
	response.Finish(rpcDatabaseUser, grpc::Status::OK, done());
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
	response.Finish(vd, grpc::Status::OK, done());
}

void DatabaseService_Register::impl(bool) {
	auto server = MustServer(request);

	QMap<int, QString> info;
	FromRPC(request, info);

	auto userid = server->registerUser(info);
	if (userid < 0) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	rpcDatabaseUser.set_id(userid);
	ToRPC(server, info, &rpcDatabaseUser);
	response.Finish(rpcDatabaseUser, grpc::Status::OK, done());
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
	response.Finish(vd, grpc::Status::OK, done());
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
	case -1: // authentication failures
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid username and/or password");
	case -2: // unknown user
		throw ::grpc::Status(::grpc::NOT_FOUND, "unknown user");
	case -3: // authentication failures where the data could (temporarily) not be verified
		throw ::grpc::Status(::grpc::UNAVAILABLE);
	}

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	rpcDatabaseUser.mutable_server()->set_id(server->iServerNum);
	rpcDatabaseUser.set_id(ret);
	response.Finish(rpcDatabaseUser, grpc::Status::OK, done());
}

void AudioService_SetRedirectWhisperGroup::impl(bool) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

}
}
