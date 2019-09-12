// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_GRPC
#ifndef MUMBLE_MURMUR_MURMURRPC_H_
#define MUMBLE_MURMUR_MURMURRPC_H_

#include <QtCore/QCoreApplication>

#include <boost/bind.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "MurmurRPC.grpc.pb.h"
#pragma GCC diagnostic pop

#include "Server.h"
#include "Meta.h"

#include <atomic>

#include <QMultiHash>

#include <grpc++/grpc++.h>

class RPCCall;

namespace MurmurRPC {
	namespace Wrapper {
		class V1_ContextActionEvents;
		class V1_Events;
		class V1_ServerEvents;
		class V1_AuthenticatorStream;
		class V1_TextMessageFilter;
	}
}

class MurmurRPCImpl : public QThread {
		Q_OBJECT;
		std::unique_ptr<grpc::Server> m_server;
	protected:
		void customEvent(QEvent *evt);
	public:
		MurmurRPCImpl(const QString &address, std::shared_ptr<::grpc::ServerCredentials> credentials);
		~MurmurRPCImpl();
		void run();
		std::unique_ptr<grpc::ServerCompletionQueue> m_completionQueue;

		// Services
		MurmurRPC::V1::AsyncService m_V1Service;

		// Listeners
		QHash<int, QMultiHash<QString, ::MurmurRPC::Wrapper::V1_ContextActionEvents *> > m_contextActionListeners;

		QSet<::MurmurRPC::Wrapper::V1_Events *> m_metaServiceListeners;

		QMultiHash<int, ::MurmurRPC::Wrapper::V1_ServerEvents *> m_serverServiceListeners;

		QMutex qmAuthenticatorsLock;
		QHash<int, ::MurmurRPC::Wrapper::V1_AuthenticatorStream *> m_authenticators;

		QMutex qmTextMessageFilterLock;
		QHash<int, ::MurmurRPC::Wrapper::V1_TextMessageFilter *> m_textMessageFilters;

		// Maps server id -> session -> context action
		QMap<int, QMap<unsigned int, QSet<QString> > > m_activeContextActions;

		bool hasActiveContextAction(const ::Server *s, const ::User *u, const QString &action);
		void addActiveContextAction(const ::Server *s, const ::User *u, const QString &action);
		void removeActiveContextAction(const ::Server *s, const ::User *u, const QString &action);
		void removeUserActiveContextActions(const ::Server *s, const ::User *u);
		void removeActiveContextActions(const ::Server *s);

		void removeTextMessageFilter(const ::Server *s);
		void removeAuthenticator(const ::Server *s);
		void sendMetaEvent(const ::MurmurRPC::Event &e);
		void sendServerEvent(const ::Server *s, const ::MurmurRPC::Server_Event &e);

	public slots:
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

		void textMessageFilter(int &res, const User *user, MumbleProto::TextMessage &message);

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
	::std::atomic_int m_refs;
public:
	MurmurRPCImpl *rpc;
	::grpc::ServerContext context;

	RPCCall(MurmurRPCImpl *rpcImpl) : m_refs(0), rpc(rpcImpl) {
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
		Q_ASSERT(m_refs > 0);
		if (--m_refs == 0) {
			delete this;
		}
	}
	virtual void ref() {
		m_refs++;
	}

	template<class T>
	class Ref {
		T *m_object;
	public:
		Ref(T *object) : m_object(object) {
			if (object) {
				object->ref();
			}
		}
		~Ref() {
			if (m_object) {
				m_object->deref();
			}
		}
		operator bool() const {
			return m_object != nullptr && !m_object->context.IsCancelled();
		}
		T *operator->() {
			return m_object;
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

/// Base for "single-stream" RPC methods.
///
/// The helper method "write" automatically queues writes to the stream. Without
/// write queuing, the grpc crashes if a stream.Write is called before a
/// previous stream.Write completes.
template <class InType, class OutType>
class RPCSingleStreamCall : public RPCCall {
	QMutex m_writeLock;
	QQueue< QPair<OutType, void *> > m_writeQueue;
public:
	InType request;
	::grpc::ServerAsyncWriter < OutType > stream;
	RPCSingleStreamCall(MurmurRPCImpl *rpcImpl) : RPCCall(rpcImpl), stream(&context) {
	}

	virtual void error(const ::grpc::Status &err) {
		stream.Finish(err, done());
	}

	void write(const OutType &msg, void *tag) {
		QMutexLocker l(&m_writeLock);
		if (m_writeQueue.size() > 0) {
			m_writeQueue.enqueue(qMakePair(msg, tag));
		} else {
			m_writeQueue.enqueue(qMakePair(OutType(), tag));
			stream.Write(msg, writeCB());
		}
	}

private:
	void *writeCB() {
		auto callback = ::boost::bind(&RPCSingleStreamCall<InType, OutType>::writeCallback, this, _1);
		return new ::boost::function<void(bool)>(callback);
	}

	void writeCallback(bool ok) {
		QMutexLocker l(&m_writeLock);
		auto processed = m_writeQueue.dequeue();
		if (processed.second) {
			auto cb = static_cast< ::boost::function<void(bool)> *>(processed.second);
			(*cb)(ok);
			delete cb;
		}
		if (m_writeQueue.size() > 0) {
			stream.Write(m_writeQueue.head().first, writeCB());
		}
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
