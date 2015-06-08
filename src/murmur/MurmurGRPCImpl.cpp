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
#include "MurmurGRPCImpl.h"
#include "Meta.h"
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

MurmurRPCImpl::MurmurRPCImpl(const QString &address) {
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
	start();
}

MurmurRPCImpl::~MurmurRPCImpl() {
}

void MurmurRPCImpl::customEvent(QEvent *evt) {
	if (evt->type() == EXEC_QEVENT) {
		auto event = static_cast<RPCExecEvent *>(evt);
		try {
			event->execute();
		} catch (::grpc::Status &ex) {
			(*event->Error)(ex);
		}
		delete event->Error;
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
			auto op = static_cast<boost::function<void()> *>(tag);
			(*op)();
			delete op;
		}
	}
	// TODO(grpc): cleanup allocated memory? not super important, because murmur should be exiting now
}

template <class T>
::ServerUser *MustUser(const ::Server *server, const T *msg) {
	if (!msg->has_user()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing user");
	}
	auto user = server->qhUsers.value(msg->user().session());
	if (!user) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid user");
	}
	return user;
}

template <>
::ServerUser *MustUser(const Server *server, const ::MurmurRPC::User *msg) {
	if (!msg->has_session()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing user session");
	}
	auto user = server->qhUsers.value(msg->session());
	if (!user) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid user");
	}
	return user;
}

template <class T>
::Server *MustServer(const T *msg) {
	if (!msg->has_server()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server");
	}
	auto server = meta->qhServers.value(msg->server().id());
	if (!server) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid server");
	}
	return server;
}

::Channel *MustChannel(const Server *server, const ::MurmurRPC::Channel *msg) {
	if (!msg->has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing channel id");
	}
	auto channel = server->qhChannels.value(msg->id());
	if (!channel) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid channel");
	}
	return channel;
}

static void channelToRPCChannel(const ::Server *srv, const ::Channel *c, ::MurmurRPC::Channel *rc) {
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

static void userToRPCUser(const ::Server *srv, const ::User *u, ::MurmurRPC::User* ru) {
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

namespace MurmurRPC {
namespace Wrapper {

void ServerService_Create_Impl(::grpc::ServerContext *context, ::MurmurRPC::Void *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Server > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ServerService_Get_Impl(::grpc::ServerContext *context, ::MurmurRPC::Server *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Server > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ServerService_Start_Impl(::grpc::ServerContext *context, ::MurmurRPC::Server *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ServerService_Stop_Impl(::grpc::ServerContext *context, ::MurmurRPC::Server *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ServerService_Remove_Impl(::grpc::ServerContext *context, ::MurmurRPC::Server *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void MetaService_GetUptime_Impl(::grpc::ServerContext *context, ::MurmurRPC::Void *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Uptime > *response, ::boost::function<void()> *next) {
	::MurmurRPC::Uptime uptime;
	uptime.set_secs(meta->tUptime.elapsed()/1000000LL);
	response->Finish(uptime, ::grpc::Status::OK, next);
}

void MetaService_GetVersion_Impl(::grpc::ServerContext *context, ::MurmurRPC::Void *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Version > *response, ::boost::function<void()> *next) {
	::MurmurRPC::Version version;
	int major, minor, patch;
	QString release;
	Meta::getVersion(major, minor, patch, release);
	version.set_version(major << 16 | minor << 8 | patch);
	version.set_release(u8(release));
	response->Finish(version, ::grpc::Status::OK, next);
}

void ContextActionService_Add_Impl(::grpc::ServerContext *context, ::MurmurRPC::ContextAction *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	if (!request->has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}
	if (!request->has_text()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing text");
	}
	// TODO(grpc): verify that context is valid value?
	if (!request->has_context()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing context");
	}

	::MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(request->action());
	mpcam.set_text(request->text());
	mpcam.set_context(request->context());
	mpcam.set_operation(::MumbleProto::ContextActionModify_Operation_Add);
	server->sendMessage(user, mpcam);

	::MurmurRPC::Void vd;
	response->Finish(vd, grpc::Status::OK, next);
}

void ContextActionService_Remove_Impl(::grpc::ServerContext *context, ::MurmurRPC::ContextAction *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);

	if (!request->has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}

	::MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(request->action());
	mpcam.set_operation(::MumbleProto::ContextActionModify_Operation_Remove);

	if (request->has_user()) {
		// Remove context action from specific user
		auto user = MustUser(server, request);
		server->sendMessage(user, mpcam);
	} else {
		// Remove context action from all users
		server->sendAll(mpcam);
	}

	::MurmurRPC::Void vd;
	response->Finish(vd, grpc::Status::OK, next);
}

void TextMessageService_Send_Impl(::grpc::ServerContext *context, ::MurmurRPC::TextMessage *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);

	::MumbleProto::TextMessage mptm;
	mptm.set_message(request->text());
	if (request->has_actor()) {
		::ServerUser *actor = server->qhUsers.value(request->actor().session());
		if (actor) {
			// TODO(grpc): verify actor?
			mptm.set_actor(actor->uiSession);
		}
	}

	// Broadcast
	if (!request->users_size() && !request->channels_size() && !request->trees_size()) {
		server->sendAll(mptm);
		::MurmurRPC::Void vd;
		response->Finish(vd, grpc::Status::OK, next);
		return;
	}
	// TODO(grpc): send message to specific users, channels
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ConfigService_GetDefault_Impl(::grpc::ServerContext *context, ::MurmurRPC::Void *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Config > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ConfigService_SetDefault_Impl(::grpc::ServerContext *context, ::MurmurRPC::Config *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ConfigService_Query_Impl(::grpc::ServerContext *context, ::MurmurRPC::Config::Query *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Config > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ChannelService_Get_Impl(::grpc::ServerContext *context, ::MurmurRPC::Channel *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Channel > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MurmurRPC::Channel rpcChannel;
	channelToRPCChannel(server, channel, &rpcChannel);
	response->Finish(rpcChannel, ::grpc::Status::OK, next);
}

void ChannelService_Add_Impl(::grpc::ServerContext *context, ::MurmurRPC::Channel *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Channel > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);

	if (!request->has_parent() || !request->has_name()) {
		response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "parent channel and name required"), next);
		return;
	}
	// TODO(grpc): verify request->parent().server() ?
	if (!request->parent().has_id()) {
		response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "parent channel is missing ID"), next);
		return;
	}
	::Channel *parent = server->qhChannels.value(request->parent().id());
	if (!parent) {
		response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "parent channel does not exist"), next);
		return;
	}

	if (!server->canNest(parent)) {
		response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "cannot nest channel in given parent"), next);
		return;
	}

	QString qsName = u8(request->name());

	::Channel *nc = server->addChannel(parent, qsName);
	server->updateChannel(nc);
	int newid = nc->iId;

	::MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(newid);
	mpcs.set_parent(parent->iId);
	mpcs.set_name(request->name());
	server->sendAll(mpcs);

	::MurmurRPC::Channel resChannel;
	channelToRPCChannel(server, nc, &resChannel);
	response->Finish(resChannel, grpc::Status::OK, next);
}

void ChannelService_Remove_Impl(::grpc::ServerContext *context, ::MurmurRPC::Channel *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	if (!channel->cParent) {
		response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "cannot remove the root channel"), next);
		return;
	}
	server->removeChannel(channel);
	::MurmurRPC::Void vd;
	response->Finish(vd, grpc::Status::OK, next);
}

void ChannelService_Update_Impl(::grpc::ServerContext *context, ::MurmurRPC::Channel *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Channel > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);

	bool changed = false;
	bool updated = false;

	if (!request->has_id()) {
		response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "id required"), next);
		return;
	}
	::Channel *channel = server->qhChannels.value(request->id());
	if (!channel) {
		response->FinishWithError(grpc::Status(grpc::NOT_FOUND, "invalid channel"), next);
		return;
	}

	::MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(channel->iId);


	// Links and parent channel are processed first, because they can return
	// errors. Without doing this, the server state can be changed without
	// notifying users.
	::QSet<::Channel *> newLinksSet;
	for (int i = 0; i < request->links_size(); i++) {
		const ::MurmurRPC::Channel &linkRef = request->links(i);
		// TODO(grpc): verify linkRef.server() ?
		if (!linkRef.has_id()) {
			response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "link channel is missing ID"), next);
			return;
		}
		::Channel *link = server->qhChannels.value(linkRef.id());
		if (!link) {
			response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "link channel does not exist"), next);
			return;
		}
		newLinksSet.insert(link);
	}

	if (request->has_parent()) {
		// TODO(grpc): verify request->parent().server() ?
		if (!request->parent().has_id()) {
			response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "parent channel is missing ID"), next);
			return;
		}
		::Channel *parent = server->qhChannels.value(request->parent().id());
		if (!parent) {
			response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "parent channel does not exist"), next);
			return;
		}
		if (parent != channel->cParent) {
			::Channel *p = parent;
			while (p) {
				if (p == channel) {
					response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "parent channel cannot be a descendant of channel"), next);
					return;
				}
				p = p->cParent;
			}
			if (!server->canNest(parent, channel)) {
				response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "channel cannot be nested in the given parent"), next);
				return;
			}
			channel->cParent->removeChannel(channel);
			parent->addChannel(channel);
			mpcs.set_parent(parent->iId);

			changed = true;
			updated = true;
		}
	}

	if (request->has_name()) {
		QString qsName = u8(request->name());
		if (channel->qsName != qsName) {
			channel->qsName = qsName;
			mpcs.set_name(request->name());

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

	if (request->has_position() && request->position() != channel->iPosition) {
		channel->iPosition = request->position();
		mpcs.set_position(request->position());

		changed = true;
		updated = true;
	}

	if (request->has_description()) {
		QString qsDescription = u8(request->description());
		if (qsDescription != channel->qsDesc) {
			server->hashAssign(channel->qsDesc, channel->qbaDescHash, qsDescription);
			mpcs.set_description(request->description());

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
	channelToRPCChannel(server, channel, &rpcChannel);
	response->Finish(rpcChannel, grpc::Status::OK, next);
}

void UserService_Get_Impl(::grpc::ServerContext *context, ::MurmurRPC::User *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::User > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);

	::MurmurRPC::User rpcUser;

	if (request->has_session()) {
		// Lookup user by session
		auto user = MustUser(server, request);
		userToRPCUser(server, user, &rpcUser);
		response->Finish(rpcUser, grpc::Status::OK, next);
		return;
	} else if (request->has_name()) {
		// Lookup user by name
		QString qsName = u8(request->name());
		foreach(const ::ServerUser *user, server->qhUsers) {
			if (user->qsName == qsName) {
				userToRPCUser(server, user, &rpcUser);
				response->Finish(rpcUser, grpc::Status::OK, next);
				return;
			}
		}
		response->FinishWithError(grpc::Status(grpc::NOT_FOUND, "invalid user"), next);
		return;
	}

	response->FinishWithError(grpc::Status(grpc::INVALID_ARGUMENT, "session or name required"), next);
}

void UserService_Update_Impl(::grpc::ServerContext *context, ::MurmurRPC::User *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::User > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void UserService_Kick_Impl(::grpc::ServerContext *context, ::MurmurRPC::User::Kick *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	::MumbleProto::UserRemove mpur;
	mpur.set_session(user->uiSession);
	if (request->has_reason()) {
		mpur.set_reason(request->reason());
	}
	server->sendAll(mpur);
	user->disconnectSocket();

	::MurmurRPC::Void vd;
	response->Finish(vd, grpc::Status::OK, next);
}

void TreeService_Get_Impl(::grpc::ServerContext *context, ::MurmurRPC::Tree *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Tree > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_Get_Impl(::grpc::ServerContext *context, ::MurmurRPC::Channel *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::ACL::List > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_Set_Impl(::grpc::ServerContext *context, ::MurmurRPC::ACL::List *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_GetEffectivePermissions_Impl(::grpc::ServerContext *context, ::MurmurRPC::User *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::ACL > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_AddTemporaryGroup_Impl(::grpc::ServerContext *context, ::MurmurRPC::ACL::TemporaryGroup *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void ACLService_RemoveTemporaryGroup_Impl(::grpc::ServerContext *context, ::MurmurRPC::ACL::TemporaryGroup *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void DatabaseService_Get_Impl(::grpc::ServerContext *context, ::MurmurRPC::Database::User *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Database::User > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void DatabaseService_Update_Impl(::grpc::ServerContext *context, ::MurmurRPC::Database::User *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Database::User > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void DatabaseService_Register_Impl(::grpc::ServerContext *context, ::MurmurRPC::Database::User *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Database::User > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void DatabaseService_Deregister_Impl(::grpc::ServerContext *context, ::MurmurRPC::Database::User *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void DatabaseService_VerifyPassword_Impl(::grpc::ServerContext *context, ::MurmurRPC::Database::VerifyPassword *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Database::User > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

void AudioService_SetRedirectWhisperGroup_Impl(::grpc::ServerContext *context, ::MurmurRPC::RedirectWhisperGroup *request, ::grpc::ServerAsyncResponseWriter< ::MurmurRPC::Void > *response, ::boost::function<void()> *next) {
	throw ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED);
}

}
}
