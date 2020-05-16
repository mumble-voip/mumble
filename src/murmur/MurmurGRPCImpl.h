// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_GRPC
#ifndef MUMBLE_MURMUR_MURMURRPC_H_
#define MUMBLE_MURMUR_MURMURRPC_H_

#include <QtCore/QCoreApplication>

#include "Meta.h"
#include "Server.h"

#include <boost/config.hpp>
#if !defined BOOST_ATTRIBUTE_NODISCARD
# define BOOST_ATTRIBUTE_NODISCARD
#endif
#if BOOST_VERSION < 106900L
namespace boost {
template<bool B, class T = void>
	using enable_if_t = typename ::std::enable_if<B, T>::type;
}
#endif
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
		grpc::Status Process(const InputMetadata&, ::grpc::AuthContext*, OutputMetadata*, OutputMetadata*) override;
		BOOST_ATTRIBUTE_NODISCARD bool IsBlocking() const override;
	private:
		QSet<QByteArray> m_gRPCUsers;
};

class MurmurRPCImpl : public QThread {
		Q_OBJECT;
		std::unique_ptr<grpc::Server> m_server;
		volatile bool m_isRunning;

		/// \brief how long we wait for an event in the main gRPC
		/// polling loop before we call boost::this_fiber::yield()
		static constexpr std::chrono::milliseconds GRPC_POLL_TIMEOUT{200};

		/// \brief how long we wait before forcefully shutting down gRPC
		static constexpr std::chrono::seconds GRPC_SHUTDOWN_TIMEOUT{5};
	public:
		MurmurRPCImpl(const QString &address, std::shared_ptr<grpc::ServerCredentials> credentials);
		/// \brief non-copy
		MurmurRPCImpl(const MurmurRPCImpl&) = delete;
		/// \brief non-copy
		MurmurRPCImpl& operator=(const MurmurRPCImpl&) = delete;
		/// \brief non-move
		MurmurRPCImpl(MurmurRPCImpl&&) = delete;
		/// \brief non-move
		MurmurRPCImpl& operator=(MurmurRPCImpl&&) = delete;


		~MurmurRPCImpl() override;
		void run() override;
		std::unique_ptr<grpc::ServerCompletionQueue> m_completionQueue; // NOLINT needed by RPCCall

		// Services
		MurmurRPC::V1::AsyncService m_V1Service; // NOLINT needed by RPCCall

		// Listeners
		using contextActionMap_t = mwc::weakContainer<mwc::contextActionConfig<RPCCall<mw::V1_ContextActionEvents>>>;
		contextActionMap_t m_contextActionListeners; // NOLINT needed by RPCCall

		using eventSet_t = mwc::weakContainer<mwc::setConfig<RPCCall<mw::V1_Events> > >;
		eventSet_t m_metaServiceListeners; // NOLINT needed by RPCCall

		using serverServiceListenersMultiMap_t = mwc::weakContainer<mwc::multiMapConfig<RPCCall<mw::V1_ServerEvents>>>;
		serverServiceListenersMultiMap_t m_serverServiceListeners; // NOLINT needed by RPCCall

		using authenticatorMap_t = mwc::weakContainer<mwc::mapConfig<RPCCall<mw::V1_AuthenticatorStream>>>;
		authenticatorMap_t m_authenticators; // NOLINT needed by RPCCall

		using messageFilterMap_t = mwc::weakContainer<mwc::mapConfig<RPCCall<mw::V1_TextMessageFilter>>>;
		messageFilterMap_t m_textMessageFilters; // NOLINT needed by RPCCall

		// Maps server id -> session -> context action
		QMap<int, QMap<unsigned int, QSet<QString> > > m_activeContextActions; // NOLINT needed by RPCCall

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
