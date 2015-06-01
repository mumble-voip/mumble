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

#include "murmur_pch.h"

#include "Mumble.pb.h"

#include "../Message.h"
#include "MurmurGRPCImpl.h"
#include "Meta.h"
#include "ServerDB.h"
#include "ServerUser.h"
#include "Server.h"
#include "Channel.h"

/*
static MurmurGRPC *mrpc = NULL;
*/

static MurmurRPCImpl service;
static std::unique_ptr<grpc::Server> s;

void RPCStart() {
	QString &address = meta->mp.qsGRPCAddress;
	if (address.isEmpty()) {
		return;
	}

	grpc::ServerBuilder builder;
	builder.AddListeningPort(u8(address), grpc::InsecureServerCredentials());
	builder.RegisterService(&service.sMetaService);
	builder.RegisterService(&service.sTextMessageService);
	builder.RegisterService(&service.sUserService);
	builder.RegisterService(&service.sChannelService);
	// TODO(grpc): single conn? look into this
	s = builder.BuildAndStart();
}

void RPCStop() {
}

MurmurRPCImpl::MurmurRPCImpl() {
}

#define FIND_SERVER(r) \
	if (!r->has_server()) { \
		return grpc::Status(grpc::INVALID_ARGUMENT, "missing server"); \
	} \
	::Server *server = meta->qhServers.value(r->server().id());

#define NEED_SERVER_EXISTS(r) \
	FIND_SERVER(r) \
	if (!server && ! ::ServerDB::serverExists(r->server().id())) { \
		return grpc::Status(grpc::OUT_OF_RANGE, "invalid server ID"); \
	}

#define NEED_USER(r) \
	if (!r->has_user()) { \
		return grpc::Status(grpc::INVALID_ARGUMENT, "missing user"); \
	} \
	::ServerUser *user = server->qhUsers.value(r->user().session()); \
	if (!user) { \
		return grpc::Status(grpc::NOT_FOUND, "invalid user"); \
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

// MetaService

::grpc::Status MurmurRPCImpl::MetaService::uptime(::grpc::ServerContext*, const ::MurmurRPC::Void*, ::MurmurRPC::Uptime* response) {
	response->set_secs(meta->tUptime.elapsed()/1000000LL);
	return grpc::Status::OK;
}

::grpc::Status MurmurRPCImpl::MetaService::version(::grpc::ServerContext*, const ::MurmurRPC::Void*, ::MurmurRPC::Version* response) {
	int major, minor, patch;
	QString release;
	Meta::getVersion(major, minor, patch, release);
	response->set_version(major << 16 | minor << 8 | patch);
	response->set_release(u8(release));
	return grpc::Status::OK;
}

::grpc::Status MurmurRPCImpl::MetaService::events(::grpc::ServerContext*, const ::MurmurRPC::Void* request, ::grpc::ServerWriter< ::MurmurRPC::Event>* writer) {
	return grpc::Status(grpc::UNIMPLEMENTED);
}

// TextMessageService

::grpc::Status MurmurRPCImpl::TextMessageService::send(::grpc::ServerContext*, const ::MurmurRPC::TextMessage* request, ::MurmurRPC::Void*) {
	NEED_SERVER_EXISTS(request)

	::MumbleProto::TextMessage mptm;
	mptm.set_message(request->text());
	if (request->has_actor()) {
		::ServerUser *actor = server->qhUsers.value(request->actor().session());
		if (actor) {
			mptm.set_actor(actor->uiSession);
		}
	}

	// Broadcast
	if (!request->users_size() && !request->channels_size() && !request->trees_size()) {
		server->sendAll(mptm);
		return grpc::Status::OK;
	}
	// TODO(grpc): send message to specific users, channels
	return grpc::Status(grpc::UNIMPLEMENTED);
}

// UserService

::grpc::Status MurmurRPCImpl::UserService::query(::grpc::ServerContext*, const ::MurmurRPC::User_Query* request, ::grpc::ServerWriter< ::MurmurRPC::User>* writer) {
	NEED_SERVER_EXISTS(request)

	foreach(const ::ServerUser *user, server->qhUsers) {
		::MurmurRPC::User rpcUser;
		userToRPCUser(server, user, &rpcUser);
		writer->Write(rpcUser);
	}
	return grpc::Status::OK;
}

::grpc::Status MurmurRPCImpl::UserService::get(::grpc::ServerContext*, const ::MurmurRPC::User* request, ::MurmurRPC::User* response) {
	NEED_SERVER_EXISTS(request)

	if (request->has_session()) {
		// Lookup user by session
		::ServerUser *user = server->qhUsers.value(request->session());
		if (!user) {
			return grpc::Status(grpc::NOT_FOUND, "invalid user");
		}
		userToRPCUser(server, user, response);
		return grpc::Status::OK;
	} else if (request->has_name()) {
		// Lookup user by name
		QString qsName = u8(request->name());
		foreach(const ::ServerUser *user, server->qhUsers) {
			if (user->qsName == qsName) {
				userToRPCUser(server, user, response);
				return grpc::Status::OK;
			}
		}
		return grpc::Status(grpc::NOT_FOUND, "invalid user");
	}

	return grpc::Status(grpc::INVALID_ARGUMENT, "session or name required");
}

::grpc::Status MurmurRPCImpl::UserService::update(::grpc::ServerContext* context, const ::MurmurRPC::User* request, ::MurmurRPC::User* response) {
	return grpc::Status(grpc::UNIMPLEMENTED);
}

::grpc::Status MurmurRPCImpl::UserService::kick(::grpc::ServerContext*, const ::MurmurRPC::User_Kick* request, ::MurmurRPC::Void*) {
	NEED_SERVER_EXISTS(request)
	NEED_USER(request)

	MumbleProto::UserRemove mpur;
	mpur.set_session(user->uiSession);
	if (request->has_reason()) {
		mpur.set_reason(request->reason());
	}
	server->sendAll(mpur);
	user->disconnectSocket();
	return grpc::Status::OK;
}

// ChannelService

::grpc::Status MurmurRPCImpl::ChannelService::query(::grpc::ServerContext* context, const ::MurmurRPC::Channel_Query* request, ::grpc::ServerWriter< ::MurmurRPC::Channel>* writer) {
	return grpc::Status(grpc::UNIMPLEMENTED);
}

::grpc::Status MurmurRPCImpl::ChannelService::get(::grpc::ServerContext*, const ::MurmurRPC::Channel* request, ::MurmurRPC::Channel* response) {
	// TODO(grpc): convert to "NEED_SERVER"?
	NEED_SERVER_EXISTS(request)

	if (!request->has_id()) {
		return grpc::Status(grpc::INVALID_ARGUMENT, "id required");
	}
	Channel *channel = server->qhChannels.value(request->id());
	if (!channel) {
		return grpc::Status(grpc::NOT_FOUND, "invalid channel");
	}
	channelToRPCChannel(server, channel, response);
	return grpc::Status::OK;
}

::grpc::Status MurmurRPCImpl::ChannelService::add(::grpc::ServerContext*, const ::MurmurRPC::Channel* request, ::MurmurRPC::Channel* response) {
	// TODO(grpc): convert to "NEED_SERVER"?
	NEED_SERVER_EXISTS(request)

	if (!request->has_parent() || !request->has_name()) {
		return grpc::Status(grpc::INVALID_ARGUMENT, "parent channel and name required");
	}
	// TODO(grpc): verify request->parent().server() ?
	if (!request->parent().has_id()) {
		return grpc::Status(grpc::INVALID_ARGUMENT, "parent channel is missing ID");
	}
	Channel *parent = server->qhChannels.value(request->parent().id());
	if (!parent) {
		return grpc::Status(grpc::INVALID_ARGUMENT, "parent channel does not exist");
	}

	if (!server->canNest(parent)) {
		return grpc::Status(grpc::INVALID_ARGUMENT, "cannot nest channel in given parent");
	}

	QString qsName = u8(request->name());

	Channel *nc = server->addChannel(parent, qsName);
	server->updateChannel(nc);
	int newid = nc->iId;

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(newid);
	mpcs.set_parent(parent->iId);
	mpcs.set_name(request->name());
	server->sendAll(mpcs);

	channelToRPCChannel(server, nc, response);
	return grpc::Status::OK;
}

::grpc::Status MurmurRPCImpl::ChannelService::remove(::grpc::ServerContext*, const ::MurmurRPC::Channel* request, ::MurmurRPC::Void*) {
	// TODO(grpc): convert to "NEED_SERVER"?
	NEED_SERVER_EXISTS(request)

	if (!request->has_id()) {
		return grpc::Status(grpc::INVALID_ARGUMENT, "id required");
	}
	Channel *channel = server->qhChannels.value(request->id());
	if (!channel) {
		return grpc::Status(grpc::NOT_FOUND, "invalid channel");
	}
	if (!channel->cParent) {
		return grpc::Status(grpc::INVALID_ARGUMENT, "cannot remove the root channel");
	}
	server->removeChannel(channel);
	return grpc::Status::OK;
}

::grpc::Status MurmurRPCImpl::ChannelService::update(::grpc::ServerContext*, const ::MurmurRPC::Channel* request, ::MurmurRPC::Channel* response) {
	// TODO(grpc): convert to "NEED_SERVER"?
	NEED_SERVER_EXISTS(request)

	bool changed = false;
	bool updated = false;

	if (!request->has_id()) {
		return grpc::Status(grpc::INVALID_ARGUMENT, "id required");
	}
	Channel *channel = server->qhChannels.value(request->id());
	if (!channel) {
		return grpc::Status(grpc::NOT_FOUND, "invalid channel");
	}

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(channel->iId);


	// Links and parent channel are processed first, because they can return
	// errors. Without doing this, the server state can be changed without
	// notifying users.
	QSet<Channel *> newLinksSet;
	for (int i = 0; i < request->links_size(); i++) {
		const MurmurRPC::Channel &linkRef = request->links(i);
		// TODO(grpc): verify linkRef.server() ?
		if (!linkRef.has_id()) {
			return grpc::Status(grpc::INVALID_ARGUMENT, "link channel is missing ID");
		}
		Channel *link = server->qhChannels.value(linkRef.id());
		if (!link) {
			return grpc::Status(grpc::INVALID_ARGUMENT, "link channel does not exist");
		}
		newLinksSet.insert(link);
	}

	if (request->has_parent()) {
		// TODO(grpc): verify request->parent().server() ?
		if (!request->parent().has_id()) {
			return grpc::Status(grpc::INVALID_ARGUMENT, "parent channel is missing ID");
		}
		Channel *parent = server->qhChannels.value(request->parent().id());
		if (!parent) {
			return grpc::Status(grpc::INVALID_ARGUMENT, "parent channel does not exist");
		}
		if (parent != channel->cParent) {
			Channel *p = parent;
			while (p) {
				if (p == channel) {
					return grpc::Status(grpc::INVALID_ARGUMENT, "parent channel cannot be a descendant of channel");
				}
				p = p->cParent;
			}
			if (!server->canNest(parent, channel)) {
				return grpc::Status(grpc::INVALID_ARGUMENT, "channel cannot be nested in the given parent");
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

	const QSet<Channel *> &oldLinksSet = channel->qsPermLinks;

	if (newLinksSet != oldLinksSet) {
		// Remove
		foreach(Channel *l, oldLinksSet) {
			if (!newLinksSet.contains(l)) {
				server->removeLink(channel, l);
				mpcs.add_links_remove(l->iId);
			}
		}
		// Add
		foreach(Channel *l, newLinksSet) {
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

	channelToRPCChannel(server, channel, response);
	return grpc::Status::OK;
}

#undef FIND_SERVER
#undef NEED_SERVER_EXISTS
#undef NEED_USER
