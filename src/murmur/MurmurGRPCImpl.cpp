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
	// TODO(grpc): single conn? look into this
	s = builder.BuildAndStart();
}

void RPCStop() {
}

MurmurRPCImpl::MurmurRPCImpl() {
}

#define FIND_SERVER(r) \
	if (!r->has_server()) { \
		return grpc::Status(grpc::OUT_OF_RANGE, "request does not contain a server"); \
	} \
	::Server *server = meta->qhServers.value(r->server().id());

#define NEED_SERVER_EXISTS(r) \
	FIND_SERVER(r) \
	if (!server && ! ::ServerDB::serverExists(r->server().id())) { \
		return grpc::Status(grpc::OUT_OF_RANGE, "invalid server ID"); \
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

#undef FIND_SERVER
#undef NEED_SERVER_EXISTS
