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

#include <atomic>

#include <QMultiHash>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/server_credentials.h>
#include <grpc++/status.h>

class RPCCall;

namespace MurmurRPC {
namespace Wrapper {
class V1_ContextActionEvents;
class V1_Events;
class V1_ServerEvents;
class V1_AuthenticatorStream;
}
}

class MurmurRPCImpl : public QThread {
		Q_OBJECT;
		std::unique_ptr<grpc::Server> mServer;
		QTimer qtCleanup;
	protected:
		void customEvent(QEvent *evt);
	public:
		MurmurRPCImpl(const QString &address);
		~MurmurRPCImpl();
		void run();
		std::unique_ptr<grpc::ServerCompletionQueue> mCQ;

		// Services
		MurmurRPC::V1::AsyncService aV1Service;

		// Listeners
		QHash<int, QMultiHash<QString, ::MurmurRPC::Wrapper::V1_ContextActionEvents *> > qhContextActionListeners;
		QSet<::MurmurRPC::Wrapper::V1_Events *> qsMetaServiceListeners;
		QMultiHash<int, ::MurmurRPC::Wrapper::V1_ServerEvents *> qmhServerServiceListeners;
		QMutex qmAuthenticatorsLock;
		QHash<int, ::MurmurRPC::Wrapper::V1_AuthenticatorStream *> qhAuthenticators;

		void removeAuthenticator(const ::Server *);
		void sendMetaEvent(const ::MurmurRPC::Event &e);
		void sendServerEvent(const ::Server *s, const ::MurmurRPC::Server_Event &e);

	public slots:
		void cleanup();

		void started(Server *server);
		void stopped(Server *server);

		void authenticateSlot(int &res, QString &uname, int sessionId, const QList<QSslCertificate> &certlist, const QString &certhash, bool certstrong, const QString &pw);
		void registerUserSlot(int &res, const QMap<int, QString> &);
		void unregisterUserSlot(int &res, int id);
		void getRegisteredUsersSlot(const QString &filter, QMap<int, QString> &res);
		void getRegistrationSlot(int &, int, QMap<int, QString> &);
		void setInfoSlot(int &, int, const QMap<int, QString> &);
		void setTextureSlot(int &res, int id, const QByteArray &texture);
		void nameToIdSlot(int &res, const QString &name);
		void idToNameSlot(QString &res, int id);
		void idToTextureSlot(QByteArray &res, int id);

		void userStateChanged(const User *user);
		void userTextMessage(const User *user, const TextMessage &message);
		void userConnected(const User *user);
		void userDisconnected(const User *user);
		void channelStateChanged(const Channel *channel);
		void channelCreated(const Channel *channel);
		void channelRemoved(const Channel *channel);

		void contextAction(const User *user, const QString &action, unsigned int session, int channel);
};

class RPCExecEvent : public ExecEvent {
	Q_DISABLE_COPY(RPCExecEvent);
public:
	RPCCall *call;
	RPCExecEvent(::boost::function<void()> fn, RPCCall *rpc_call) : ExecEvent(fn), call(rpc_call) {
	}
};

class RPCCall {
	::std::atomic_int mRefs;
public:
	MurmurRPCImpl *rpc;
	::grpc::ServerContext context;

	RPCCall(MurmurRPCImpl *rpcImpl) : rpc(rpcImpl) {
		ref();
	}
	virtual ~RPCCall() {
	}
	virtual ::boost::function<void(bool)> *done() {
		auto done_fn = ::boost::bind(&RPCCall::finish, this, _1);
		return new ::boost::function<void(bool)>(done_fn);
	}

	virtual void error(const ::grpc::Status&) = 0;

	virtual void finish(bool) {
		deref();
	}

	virtual void deref() {
		if (--mRefs <= 0) {
			delete this;
		}
	}
	virtual void ref() {
		mRefs++;
	}

	template<class T>
	class Ref {
		T *mO;
	public:
		Ref(T *o) : mO(o) {
			if (o) {
				o->ref();
			}
		}
		~Ref() {
			if (mO) {
				mO->deref();
			}
		}
		operator bool() const {
			return mO != nullptr && !mO->context.IsCancelled();
		}
		T *operator->() {
			return mO;
		}
	};
};

template <class InType, class OutType>
class RPCSingleSingleCall : public RPCCall {
public:
	InType request;
	::grpc::ServerAsyncResponseWriter < OutType > stream;

	RPCSingleSingleCall(MurmurRPCImpl *rpcImpl) : RPCCall(rpcImpl), stream(&context) {
	}

	virtual void error(const ::grpc::Status &err) {
		stream.FinishWithError(err, done());
	}

	virtual void end(const OutType &msg = OutType()) {
		stream.Finish(msg, ::grpc::Status::OK, done());
	}
};

template <class InType, class OutType>
class RPCStreamStreamCall : public RPCCall {
public:
	InType request;
	OutType response;
	::grpc::ServerAsyncReaderWriter< OutType, InType > stream;

	RPCStreamStreamCall(MurmurRPCImpl *rpcImpl) : RPCCall(rpcImpl), stream(&context) {
	}

	virtual void error(const ::grpc::Status &err) {
		stream.Finish(err, done());
	}
};


#endif
#endif
