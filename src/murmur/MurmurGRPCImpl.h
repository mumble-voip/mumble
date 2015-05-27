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

#ifdef USE_GRPC
#ifndef MUMBLE_MURMUR_MURMURRPC_H_
#define MUMBLE_MURMUR_MURMURRPC_H_

#include "MurmurRPC.grpc.pb.h"

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/server_credentials.h>
#include <grpc++/status.h>

class MurmurRPCImpl : public QObject {
		Q_OBJECT;
	public:
		MurmurRPCImpl();
		// MetaService
		struct MetaService : public MurmurRPC::MetaService::Service {
			::grpc::Status uptime(::grpc::ServerContext* context, const ::MurmurRPC::Void* request, ::MurmurRPC::Uptime* response);
			::grpc::Status version(::grpc::ServerContext* context, const ::MurmurRPC::Void* request, ::MurmurRPC::Version* response);
			::grpc::Status events(::grpc::ServerContext* context, const ::MurmurRPC::Void* request, ::grpc::ServerWriter< ::MurmurRPC::Event>* writer);
		};
		MurmurRPCImpl::MetaService sMetaService;
		// TextMessageService
		struct TextMessageService : public MurmurRPC::TextMessageService::Service {
			::grpc::Status send(::grpc::ServerContext* context, const ::MurmurRPC::TextMessage* request, ::MurmurRPC::Void* response);
		};
		MurmurRPCImpl::TextMessageService sTextMessageService;
};

#endif
#endif
