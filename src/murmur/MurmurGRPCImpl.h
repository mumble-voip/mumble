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

#include "Server.h"
#include "Meta.h"

#include <QMultiHash>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/server_credentials.h>
#include <grpc++/status.h>

struct RPCCall {
	virtual ::boost::function<void(bool)> *done() = 0;
	virtual void error(::grpc::Status&) = 0;
};

class RPCExecEvent : public ExecEvent {
	Q_DISABLE_COPY(RPCExecEvent);
public:
	RPCCall *call;
	RPCExecEvent(::boost::function<void()> fn, RPCCall *call) : ExecEvent(fn), call(call) {
	}
};

namespace MurmurRPC {
namespace Wrapper {
struct ContextActionService_Events;
}
}

class MurmurRPCImpl : public QThread {
		Q_OBJECT;
		std::unique_ptr<grpc::Server> mServer;
	protected:
		void customEvent(QEvent *evt);
	public:
		MurmurRPCImpl(const QString &address);
		~MurmurRPCImpl();
		void run();
		std::unique_ptr<grpc::ServerCompletionQueue> mCQ;

		// Services
		MurmurRPC::ACLService::AsyncService aACLService;
		MurmurRPC::AudioService::AsyncService aAudioService;
		MurmurRPC::AuthenticatorService::AsyncService aAuthenticatorService;
		MurmurRPC::BanService::AsyncService aBanService;
		MurmurRPC::ChannelService::AsyncService aChannelService;
		MurmurRPC::ConfigService::AsyncService aConfigService;
		MurmurRPC::ContextActionService::AsyncService aContextActionService;
		MurmurRPC::DatabaseService::AsyncService aDatabaseService;
		MurmurRPC::LogService::AsyncService aLogService;
		MurmurRPC::MetaService::AsyncService aMetaService;
		MurmurRPC::ServerService::AsyncService aServerService;
		MurmurRPC::TextMessageService::AsyncService aTextMessageService;
		MurmurRPC::TreeService::AsyncService aTreeService;
		MurmurRPC::UserService::AsyncService aUserService;

		// ContextActionService
		QHash<int, QMultiHash<QString, ::MurmurRPC::Wrapper::ContextActionService_Events *> > qhContextActionListeners;
		void contextActionCb(::MurmurRPC::Wrapper::ContextActionService_Events *stream, bool ok);

	public slots:
		void started(Server *server);
		void stopped(Server *server);

		void userStateChanged(const User *user);
		void userTextMessage(const User *user, const TextMessage &message);
		void userConnected(const User *user);
		void userDisconnected(const User *user);
		void channelStateChanged(const Channel *channel);
		void channelCreated(const Channel *channel);
		void channelRemoved(const Channel *channel);

		void contextAction(const User *user, const QString &action, unsigned int session, int channel);
};

#endif
#endif
