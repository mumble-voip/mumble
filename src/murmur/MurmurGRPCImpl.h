// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_GRPC
#ifndef MUMBLE_MURMUR_MURMURRPC_H_
#define MUMBLE_MURMUR_MURMURRPC_H_

#include <QtCore/QCoreApplication>

#include "Server.h"
#include "Meta.h"

#ifndef Q_MOC_RUN
#include "GRPCContainer.h"
#include "GRPCall.h"
#include <boost/optional.hpp>
#endif

#include <atomic>
#include <chrono>
#include <functional>
#include <type_traits>

#include <QMultiHash>
#include <QSet>

#include <grpc++/grpc++.h>
#include <grpc++/security/auth_context.h>

using MurmurRPC::Wrapper::RPCCall;
namespace mwc = MurmurRPC::Wrapper::Container;
namespace mw = MurmurRPC::Wrapper;

class MurmurRPCAuthenticator : public ::grpc::AuthMetadataProcessor {
	public:
		MurmurRPCAuthenticator();
		grpc::Status Process(const InputMetadata&, ::grpc::AuthContext*, OutputMetadata*, OutputMetadata*);
		bool IsBlocking() const;
	protected:
		QSet<QByteArray> m_gRPCUsers;
};

class MurmurRPCImpl : public QThread {
		Q_OBJECT;
		std::unique_ptr<grpc::Server> m_server;
		volatile bool m_isRunning;
	protected:
//		void customEvent(QEvent *evt);
	public:
		MurmurRPCImpl(const QString &address, std::shared_ptr<grpc::ServerCredentials> credentials);
		~MurmurRPCImpl();
		void run();
		std::unique_ptr<grpc::ServerCompletionQueue> m_completionQueue;

		// Services
		MurmurRPC::V1::AsyncService m_V1Service;

		// Listeners
		typedef mwc::weakContainer<mwc::contextActionConfig<RPCCall<mw::V1_ContextActionEvents>>> contextActionMap;
		contextActionMap m_contextActionListeners;

		typedef mwc::weakContainer<mwc::setConfig<RPCCall<mw::V1_Events>>> eventSet_t;
		eventSet_t m_metaServiceListeners;

		typedef mwc::weakContainer<mwc::multiMapConfig<RPCCall<mw::V1_ServerEvents>>> serverServiceListenersMultiMap;
		serverServiceListenersMultiMap m_serverServiceListeners;

		typedef mwc::weakContainer<mwc::mapConfig<RPCCall<mw::V1_AuthenticatorStream>>> authenticatorMap;
		authenticatorMap m_authenticators;

		typedef mwc::weakContainer<mwc::mapConfig<RPCCall<mw::V1_TextMessageFilter>>> messageFilterMap;
		messageFilterMap m_textMessageFilters;

		// Maps server id -> session -> context action
		QMap<int, QMap<unsigned int, QSet<QString> > > m_activeContextActions;

		bool hasActiveContextAction(const ::Server *s, const ::User *u, const QString &action);
		void addActiveContextAction(const ::Server *s, const ::User *u, const QString &action);
		void removeActiveContextAction(const ::Server *s, const ::User *u, const QString &action);
		void removeUserActiveContextActions(const ::Server *s, const ::User *u);
		void removeActiveContextActions(const ::Server *s);

		void removeTextMessageFilter(int);
		void removeAuthenticator(int);
		void removeAuthenticator(uint32_t);
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

		/// \brief reciever for Server::textMessageFilterSig()
		void textMessageFilter(int& res, unsigned int uiSession, MumbleProto::TextMessage &message);

		void contextAction(const User *user, const QString &action, unsigned int session, int channel);
};


#endif
#endif
