// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef Q_MOC_RUN
# include <boost/type_traits.hpp>
# include "FiberScheduler.h"
#endif

#include "Mumble.pb.h"

#include "../Message.h"
#include "../Group.h"
#include "MurmurGRPCImpl.h"

#include "ServerDB.h"
#include "ServerUser.h"
#include "Server.h"
#include "Channel.h"
#include "Utils.h"

#include <chrono>

#include <QtCore/QStack>
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QTimer>
#include <QDebug>

// this is needed because some of my templates
// need MurmurGRPCImpl to be fully defined
// and I didn't want to split everything into
// a forward declare and implementation part
#define MUMBLE_MURMUR_GRPC_WRAPPER_IMPL
#ifndef Q_MOC_RUN
# include "GRPCall.h"
#endif
#undef MUMBLE_MURMUR_GRPC_WRAPPER_IMPL

// GRPC system overview
// ====================
//
// Definitions:
//
//  - Completion queue: a mechanism used for handling asynchronous grpc
//    requests. Whenever you want to do something with grpc (e.g. accept a
//    remote call, read/write a message), the completion queue manages that
//    request for you.
//
//    When you wish to perform an asynchronous action, you provide the
//    completion queue with a unique "tag". This tag used to identify the
//    request after it has finished.
//
//    To check the status of asynchronous actions that have been added to the
//    completion queue, you call its "Next" method. The async version is used
//    to allow for calls to yield so that fibers in the gRPC worker thread get
//    to act.
//
//    Next provides several output variables: a static indicating if there was
//    an event found in the timeout, if if there was none found or the server
//    is shutting down, the completed action's tag, and a boolean, which indicates if
//    the action was successful. All tags in this project are are
//    stack-allocated pointers to std::function objects that accept a boolean.
//    After Next returns, the tag pointer is dereferenced, executed with the
//    success variable.
//
//  - Wrapper class:
//    Each service has a wrapper class. The wrapper classes are stateless
//    functor objects which define what type of stream they are providing,
//    and the specifics of the types involved in the messages.
//
//  - RPC templates:
//    The core of the system. By using the configuration information from the
//    wrapper class; it will create a template object that had functions
//    to support the type of stream that is in use. Since they are all allocated
//    on the heap, they use an internal shared_ptr to do reference counting.
//    Containers store weak_ptrs that expire if the primary object is deleted.
//
//  - RPC Object Lifetime: gRPC supports an method to call a callback when the
//    call is over. This starts the deletion processes. All shared/weak pointers
//    that have been given out have a custom deleter that sets a flag letting the
//    object know that there are no more references to it and it was marked to be
//    deleted. The finish method resets the original shared_ptr, then spawns another
//    fiber to check repeadly until the deleter was invoked on the object.
//
//  - Boost Fibers:
//    The magic that makes this all work. By using clever stack manipulation,
//    a single threaded program can turn into a cooperative multi-threaded program.
//    This allows you to use a 'callback' based async system while writing code
//    as if you were working with a syncronous api.
//
//    Fibers also lets you customize the scheduling algorithm by giving the fibers
//    properties. The current algorithm is a very simple one; there is one fiber
//    that lives in the event loop and calls yield every time it finishes (so the
//    other fibers can run). When creating a fiber, you can specify that you want
//    it to run in this thread instead of the originating thread.
//
//    NOTE: THREAD LOCAL STORAGE WILL NOT WORK WITH THIS MODEL
//
//
// The flow of the grpc system is as follows:
//
//  - GRPCStart() is called from murmur's main().
//  - If an address for the grpc has been set in murmur.ini, the grpc service
//    begins listening on that address for grpc client connections.
//  - When an incoming request is recieved if TLS is enabled, client certificate
//    valitation is done. This is done by a simple SHA256 fingerprint defined
//    in the murmur.ini file. Unauthenticated requests never reach any of the
//    Wrapper code.
//  - A new thread is created which handles the grpc completion queue (defined
//    above). This thread continuously calls the completion queue's Next method
//    to process the next completed event.
//  - The wrapper classes' "create" methods are executed, which makes them
//    invokable by grpc clients.
//  - With the completion queue now running in the background, murmur
//    continues starting up.
//  - When a grpc client invokes an RPC method, it can execute in either the
//    polling thread or the main event loop. Realistically, only the impl callback
//    needs to go into the main event loop. Reading/Writing messages can be done in
//    the other loop and then retrieved in the thread that originated them. The 'done'
//    cleanup methods also do not need to be in the main loop as the Boost Multi-Index
//    container wrapper contains internal locking to prevent data races. Any errors
//    in use should cause an easy to reproduce deadlock.
//
//    Additionally, the execution of tags are wrapped with a try-catch. This
//    try-catch catches any grpc::Status that is thrown. If one is caught, the
//    status is automatically sent to the grpc client and the invocation of the
//    current method is stopped.

static MurmurRPCImpl *service;

/// \brief Used to inject boost::fiber::yield() calls into main event loop.
///
/// Started by a single-shot timer in GRPCStart(), this function is used
/// to make sure that yield() is called in every run of the event loop.
///
/// First sets up a static functor to be used in a QTimer object. It then
/// creates a new QTimer object, storing it in a std::unique_ptr with a
/// MurmurRPC::Wrapper::Detail::qtimer_deleter as the deleter.
/// (without the custom deleter murmur will segfault on shutdown)
/// It is a single-shot timer which means it only fires once, but
/// because the timeout is 0, it gets called every time the event
/// loop ends.
///
/// It will then call processEvents() on the main application
/// event loop with the WaitForMoreEvents flag. This will wait
/// until it gets some events, then process them.
///
/// After processing of events, yield() is called which allows
/// any other fibers running in the main event loop to be processed.
///
/// The QTimer is then started again to make sure that on completion
/// of the next event loop that we get called again.
///
void InjectThreadYield() {
	using MurmurRPC::Wrapper::Detail::qtimer_deleter;
	static std::function<void()> thisfunc = InjectThreadYield;
	static std::unique_ptr<QTimer, qtimer_deleter> timer([](){
		auto t = new QTimer(QCoreApplication::instance());
		t->setInterval(0);
		t->setSingleShot(true);
		t->callOnTimeout(QCoreApplication::instance(), std::ref(thisfunc));
		return t;}(), qtimer_deleter());
	QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
	boost::this_fiber::yield();
	timer->start();
}

void GRPCStart() {
	const auto &address = meta->mp.qsGRPCAddress;
	if (address.isEmpty()) {
		return;
	}
	const auto &cert = meta->mp.qsGRPCCert;
	const auto &key = meta->mp.qsGRPCKey;
	std::shared_ptr<::grpc::ServerCredentials> credentials;
	if (cert.isEmpty() || key.isEmpty()) {
		credentials = ::grpc::InsecureServerCredentials();
	} else {
		std::shared_ptr<MurmurRPCAuthenticator> authenticator(new MurmurRPCAuthenticator());
		::grpc::SslServerCredentialsOptions options(GRPC_SSL_REQUEST_AND_REQUIRE_CLIENT_CERTIFICATE_BUT_DONT_VERIFY);
		::grpc::SslServerCredentialsOptions::PemKeyCertPair pair;
		{
			QFile file(cert);
			if (!file.open(QIODevice::ReadOnly)) {
				qFatal("could not open gRPC certificate file: %s", cert.toStdString().c_str());
				return;
			}
			QTextStream stream(&file);
			auto contents = stream.readAll();
			pair.cert_chain = contents.toStdString();
		}
		{
			QFile file(key);
			if (!file.open(QIODevice::ReadOnly)) {
				qFatal("could not open gRPC key file: %s", key.toStdString().c_str());
				return;
			}
			QTextStream stream(&file);
			auto contents = stream.readAll();
			pair.private_key = contents.toStdString();
		}
		options.pem_key_cert_pairs.push_back(pair);
		credentials = ::grpc::SslServerCredentials(options);
		credentials->SetAuthMetadataProcessor(authenticator);
	}

	service = new MurmurRPCImpl(address, credentials);
	qWarning("GRPC: listening on '%s'", qPrintable(address));
	QTimer::singleShot(0, QCoreApplication::instance(), [](){
			boost::fibers::use_scheduling_algorithm<MurmurRPC::Scheduler::grpc_scheduler>(true);
			InjectThreadYield();});
}

void GRPCStop() {
	delete service;
}


// Check for valid fingerprints from the config and give a warning
// if there aren't any valid ones
MurmurRPCAuthenticator::MurmurRPCAuthenticator() {
	QRegularExpression re("^(?:[[:xdigit:]]{2}:?){32}$");
	const auto &authorized = meta->mp.qsGRPCAuthorized;

	for (auto&& user : authorized.split(' ')) {
		if (!re.match(user).hasMatch()) {
			qWarning("gRPC: %s is not a valid hexadecimal SHA256 digest, ignoring", qUtf8Printable(user));
			continue;
		}
		m_gRPCUsers.insert(QByteArray::fromHex(user.toUtf8()));
	}

	if (m_gRPCUsers.empty()) {
		qWarning("gRPC Security is enabled but no users are authorized to use the interface\n"
			 "Please set grpcauthorized to a list of authorized clients");
	}

	return;
}

// Hash table lookup should be non-blocking
bool MurmurRPCAuthenticator::IsBlocking() const {
	return false;
}

// We don't use any metadata. Just check to see if the certificate fingerprint matches
grpc::Status MurmurRPCAuthenticator::Process(const InputMetadata &authData, ::grpc::AuthContext *ctx, OutputMetadata *used, OutputMetadata* resp) {
	QByteArray fingerprint;
	QString identity;
	QStringList identities;

	(void) used;
	(void) resp;
	(void) authData;

	for (auto&& i : ctx->GetPeerIdentity()) {
		identities.append(QString::fromUtf8(i.data(), i.length()));
	}
	if (identities.empty()) {
		identity = "Undefined";
	} else {
		identity = identities.join(':');
	}

	qDebug("Incoming connection from: %s", qUtf8Printable(identity));

	for (auto&& pem : ctx->FindPropertyValues("x509_pem_cert")) {
		QSslCertificate cert(QByteArray(pem.data(), pem.length()));
		if (cert.isNull()) {
			continue;
		}

		fingerprint = cert.digest(QCryptographicHash::Sha256);

		if (!m_gRPCUsers.contains(fingerprint)) {
			qDebug("Fingerprint %s not found", fingerprint.toHex(':').data());
			continue;
		}

		qDebug("Fingerprint %s found", fingerprint.toHex(':').data());
		return ::grpc::Status::OK;
	}

	qDebug("Connection from %s could not be validated", qUtf8Printable(identity));
	return ::grpc::Status(::grpc::StatusCode::UNAUTHENTICATED, "Certificate invalid or not presented");
}

MurmurRPCImpl::MurmurRPCImpl(const QString &address, std::shared_ptr<::grpc::ServerCredentials> credentials) {
	::grpc::ServerBuilder builder;
	builder.AddListeningPort(u8(address), credentials);
	builder.RegisterService(&m_V1Service);
	m_completionQueue = builder.AddCompletionQueue();
	m_server = builder.BuildAndStart();
	meta->connectListener(this);
	m_isRunning = true;
	start();
}

MurmurRPCImpl::~MurmurRPCImpl() {
	void *ignored_tag;
	bool ignored_ok;
	m_server->Shutdown(std::chrono::system_clock::now() + std::chrono::seconds(5));
	m_server->Wait();
	m_isRunning = false;
	m_completionQueue->Shutdown();
	while (m_completionQueue->Next(&ignored_tag, &ignored_ok)) { }
}

// ToRPC/FromRPC methods convert data to/from grpc protocol buffer messages.
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

	const auto su = static_cast<const ServerUser *>(u);
	ru->set_online_secs(su->bwr.onlineSeconds());
	ru->set_bytes_per_sec(su->bwr.bandwidth());
	ru->mutable_version()->set_version(su->uiVersion);
	ru->mutable_version()->set_release(u8(su->qsRelease));
	ru->mutable_version()->set_os(u8(su->qsOS));
	ru->mutable_version()->set_os_version(u8(su->qsOSVersion));
	ru->set_plugin_identity(u8(su->qsIdentity));
	ru->set_plugin_context(su->ssContext);
	ru->set_idle_secs(su->bwr.idleSeconds());
	ru->set_udp_ping_msecs(su->dUDPPingAvg);
	ru->set_tcp_ping_msecs(su->dTCPPingAvg);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
	ru->set_tcp_only(su->aiUdpFlag.loadRelaxed() == 0);
#else
	// Qt 5.14 introduced QAtomicInteger::loadRelaxed() which deprecates QAtomicInteger::load()
	ru->set_tcp_only(su->aiUdpFlag.load() == 0);
#endif

	ru->set_address(su->haAddress.toStdString());
}

void ToRPC(const ::Server *srv, const QMap<int, QString> &info, const QByteArray &texture, ::MurmurRPC::DatabaseUser *du) {
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
	if (!texture.isNull()) {
		du->set_texture(texture.constData(), texture.size());
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
		ra->mutable_group()->set_name(u8(acl->qsGroup));
	}

	ra->set_allow(MurmurRPC::ACL_Permission(int(acl->pAllow)));
	ra->set_deny(MurmurRPC::ACL_Permission(int(acl->pDeny)));
}

void ToRPC(const ::Server *, const ::Group *g, ::MurmurRPC::ACL_Group *rg) {
	rg->set_name(u8(g->qsName));
	rg->set_inherit(g->bInherit);
	rg->set_inheritable(g->bInheritable);
}

void ToRPC(const ::Server *srv, const ::Ban &ban, ::MurmurRPC::Ban *rb) {
	rb->mutable_server()->set_id(srv->iServerNum);

	rb->set_address(ban.haAddress.toStdString());
	rb->set_bits(ban.iMask);
	rb->set_name(u8(ban.qsUsername));
	rb->set_hash(u8(ban.qsHash));
	rb->set_reason(u8(ban.qsReason));
	rb->set_start(ban.qdtStart.toLocalTime().toTime_t());
	rb->set_duration_secs(ban.iDuration);
}

void FromRPC(const ::Server *, const ::MurmurRPC::Ban &rb, ::Ban &ban) {
	ban.haAddress = HostAddress(rb.address());
	ban.iMask = rb.bits();
	ban.qsUsername = u8(rb.name());
	ban.qsHash = u8(rb.hash());
	ban.qsReason = u8(rb.reason());
	ban.qdtStart = QDateTime::fromTime_t(static_cast<quint32>(rb.start())).toUTC();
	ban.iDuration = rb.duration_secs();
}

void ToRPC(int serverID, const ::ServerDB::LogRecord &log, ::MurmurRPC::Log *rl) {
	rl->mutable_server()->set_id(serverID);

	rl->set_timestamp(log.first);
	rl->set_text(u8(log.second));
}

void ToRPC(const ::Server *srv, const ::User *user, const ::TextMessage &message, ::MurmurRPC::TextMessage *rtm) {
	rtm->mutable_server()->set_id(srv->iServerNum);

	rtm->mutable_actor()->mutable_server()->set_id(srv->iServerNum);
	rtm->mutable_actor()->set_session(user->uiSession);

	foreach(auto session, message.qlSessions) {
		auto target = rtm->add_users();
		target->mutable_server()->set_id(srv->iServerNum);
		target->set_session(session);
	}

	foreach(auto id, message.qlChannels) {
		auto target = rtm->add_channels();
		target->mutable_server()->set_id(srv->iServerNum);
		target->set_id(id);
	}

	foreach(auto id, message.qlTrees) {
		auto target = rtm->add_trees();
		target->mutable_server()->set_id(srv->iServerNum);
		target->set_id(id);
	}

	rtm->set_text(u8(message.qsText));
}

// Sends a meta event to any subscribed listeners.
void MurmurRPCImpl::sendMetaEvent(const ::MurmurRPC::Event &e) {
	qDebug("sending meta events");
	for (auto&& item : m_metaServiceListeners.getLockedIndex(mwc::rpcid{})) {
		auto id = std::get<0>(item);
		auto listener = std::get<1>(item);
		auto cb = [this, id](bool ok) {
				if (!ok) {
					(this->m_metaServiceListeners.getRPCIdPtr())->erase(id);
				}
			};
		qDebug("sending write event for meta event, listenerid %u", id);
		listener->Write(e, cb);
	}
}

// Called when a server starts.
void MurmurRPCImpl::started(::Server *server) {
	server->connectListener(this);
	server->connectAuthenticator(this);
	connect(server, SIGNAL(contextAction(const User *, const QString &, unsigned int, int)), this, SLOT(contextAction(const User *, const QString &, unsigned int, int)));
	connect(server, SIGNAL(textMessageFilterSig(int&, unsigned int, MumbleProto::TextMessage &)), this, SLOT(textMessageFilter(int&, unsigned int, MumbleProto::TextMessage &)));

	::MurmurRPC::Event rpcEvent;
	rpcEvent.set_type(::MurmurRPC::Event_Type_ServerStarted);
	rpcEvent.mutable_server()->set_id(server->iServerNum);
	sendMetaEvent(rpcEvent);
}

// Called when a server stops.
void MurmurRPCImpl::stopped(::Server *server) {
	qDebug("got server stopped: %d", server->iServerNum);
	removeActiveContextActions(server);

	::MurmurRPC::Event rpcEvent;
	rpcEvent.set_type(::MurmurRPC::Event_Type_ServerStopped);
	rpcEvent.mutable_server()->set_id(server->iServerNum);
	sendMetaEvent(rpcEvent);
}

// Removes a connected text message filter.
void MurmurRPCImpl::removeTextMessageFilter(int serverNum) {
	auto idx = m_textMessageFilters.getServerPtr();
	auto it = idx->find(serverNum);
	if (it == idx->end()) {
		return;
	}
	auto filter = std::get<2>(*it).lock();
	if (filter != nullptr) {
		filter->error(::grpc::Status(::grpc::CANCELLED, "filter detached"));
	}
	idx->erase(it);
}

// Removes a connected authenticator by server
void MurmurRPCImpl::removeAuthenticator(int serverNum) {
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(serverNum);
	if (it == idx->end()) {
		return;
	}
	auto authenticator = std::get<2>(*it).lock();
	if (authenticator != nullptr) {
		authenticator->error(::grpc::Status(::grpc::CANCELLED, "authenticator detached"));
	}
	idx->erase(it);
}

// Removes a connected authenticator by worker id
void MurmurRPCImpl::removeAuthenticator(uint32_t workerId) {
	auto idx = m_authenticators.getRPCIdPtr();
	auto it = idx->find(workerId);
	if (it == idx->end()) {
		return;
	}
	auto authenticator = std::get<2>(*it).lock();
	if (authenticator != nullptr) {
		authenticator->error(::grpc::Status(::grpc::CANCELLED, "authenticator detached"));
	}
	idx->erase(it);
}

// Called when a connecting user needs to be authenticated.
void MurmurRPCImpl::authenticateSlot(int &res, QString &uname, int sessionId, const QList<QSslCertificate> &certlist, const QString &certhash, bool certstrong, const QString &pw) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto serverNum = s->iServerNum;
	auto it = idx->find(serverNum);
	if (it == idx->end()) {
		return;
	}
	auto authenticator = std::get<2>(*it).lock();
	auto authenticatorId = std::get<1>(*it);
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	bool ok;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	::MurmurRPC::Authenticator_Request request;

	request.mutable_authenticate()->set_name(u8(uname));
	if (!pw.isEmpty()) {
		request.mutable_authenticate()->set_password(u8(pw));
	}
	foreach(const auto &cert, certlist) {
		auto data = cert.toDer();
		request.mutable_authenticate()->add_certificates(data.constData(), data.size());
	}
	if (!certhash.isEmpty()) {
		request.mutable_authenticate()->set_certificate_hash(u8(certhash));
		request.mutable_authenticate()->set_strong_certificate(certstrong);
	}

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authenticatorId << "cancelled";
		return;
	}

	if(!ok) {
		removeAuthenticator(authenticatorId);
		// bad responces should be temp fails
		res = -3;
		return;
	}

	s = meta->qhServers.value(serverNum, nullptr);
	if (s == nullptr) {
		//server deleted during network delays
		res = -3;
		return;
	}


	switch (response->authenticate().status()) {
	case ::MurmurRPC::Authenticator_Response_Status_Success:
		if (!response->authenticate().has_id()) {
			res = -3;
			break;
		}
		res = response->authenticate().id();
		if (response->authenticate().has_name()) {
			uname = u8(response->authenticate().name());
		}
		{
			QStringList qsl;
			for (int i = 0; i < response->authenticate().groups_size(); i++) {
				auto &group = response->authenticate().groups(i);
				if (group.has_name()) {
					qsl << u8(group.name());
				}
			}
			if (!qsl.isEmpty()) {
				s->setTempGroups(res, sessionId, NULL, qsl);
			}
		}
		break;
	case ::MurmurRPC::Authenticator_Response_Status_TemporaryFailure:
		res = -3;
		break;
	case ::MurmurRPC::Authenticator_Response_Status_Failure:
		res = -1;
		break;
	default:
		break;
	}
}

// Called when a user is being registered on the server.
void MurmurRPCImpl::registerUserSlot(int &res, const QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}
	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	ToRPC(s, info, QByteArray(), request.mutable_register_()->mutable_user());

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}

	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	s = meta->qhServers.value(serverNum, nullptr);
	if (s == nullptr) {
		//server deleted during network delays
		res = -3;
		return;
	}

	switch (response->register_().status()) {
	case ::MurmurRPC::Authenticator_Response_Status_Success:
		if (!response->register_().has_user() || !response->register_().user().has_id()) {
			res = -1;
			break;
		}
		res = response->register_().user().id();
		break;
	case ::MurmurRPC::Authenticator_Response_Status_Fallthrough:
		break;
	default:
		res = -1;
		break;
	}
}

// Called when a user is being deregistered on the server.
void MurmurRPCImpl::unregisterUserSlot(int &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}

	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	request.mutable_deregister()->mutable_user()->mutable_server()->set_id(s->iServerNum);
	request.mutable_deregister()->mutable_user()->set_id(id);

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}

	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	if (response->deregister().status() != ::MurmurRPC::Authenticator_Response_Status_Fallthrough) {
		res = 0;
	}
}

// Called when a list of registered users is requested.
void MurmurRPCImpl::getRegisteredUsersSlot(const QString &filter, QMap<int, QString> &res) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}

	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	if (!filter.isEmpty()) {
		request.mutable_query()->set_filter(u8(filter));
	}

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}

	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	for (int i = 0; i < response->query().users_size(); i++) {
		const auto &user = response->query().users(i);
		if (!user.has_id() || !user.has_name()) {
			continue;
		}
		res.insert(user.id(), u8(user.name()));
	}
}

// Called when information about a registered user is requested.
void MurmurRPCImpl::getRegistrationSlot(int &res, int id, QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}

	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	request.mutable_find()->set_id(id);

	res = -1;

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}

	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	if (response->find().has_user()) {
		FromRPC(response->find().user(), info);
		res = 1;
	}
}

// Called when information about a registered user is being updated.
void MurmurRPCImpl::setInfoSlot(int &res, int id, const QMap<int, QString> &info) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}

	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	request.mutable_update()->mutable_user()->set_id(id);
	ToRPC(s, info, QByteArray(), request.mutable_update()->mutable_user());

	res = 0;

	auto future = authenticator->writeRead(request);
	try { 
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}
	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	switch (response->update().status()) {
	case ::MurmurRPC::Authenticator_Response_Status_Success:
		res = 1;
		break;
	case ::MurmurRPC::Authenticator_Response_Status_Fallthrough:
		res = -1;
		break;
	default:
		break;
	}
}

// Called when a texture for a registered user is being updated.
void MurmurRPCImpl::setTextureSlot(int &res, int id, const QByteArray &texture) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}

	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	request.mutable_update()->mutable_user()->set_id(id);
	request.mutable_update()->mutable_user()->set_texture(texture.constData(), texture.size());

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}

	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	if (response->update().status() == ::MurmurRPC::Authenticator_Response_Status_Success) {
		res = 1;
	}
}

// Called when a user name needs to be converted to a user ID.
void MurmurRPCImpl::nameToIdSlot(int &res, const QString &name) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}

	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	request.mutable_find()->set_name(u8(name));

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}

	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	if (response->find().has_user() && response->find().user().has_id()) {
		res = response->find().user().id();
	}
}

// Called when a user ID needs to be converted to a user name.
void MurmurRPCImpl::idToNameSlot(QString &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}

	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	request.mutable_find()->set_id(id);

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}

	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	if (response->find().has_user() && response->find().user().has_name()) {
		res = u8(response->find().user().name());
	}
}

// Called when a texture for a given registered user is requested.
void MurmurRPCImpl::idToTextureSlot(QByteArray &res, int id) {
	::Server *s = qobject_cast< ::Server *> (sender());
	auto idx = m_authenticators.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}

	int serverNum;
	uint32_t authId;
	std::weak_ptr<RPCCall<::MurmurRPC::Wrapper::V1_AuthenticatorStream>> authWeak;
	std::tie(serverNum, authId, authWeak) = *it;

	auto authenticator = authWeak.lock();
	if (authenticator == nullptr) {
		idx->erase(it);
		return;
	}
	idx = nullptr;

	::MurmurRPC::Authenticator_Request request;
	boost::optional<MurmurRPC::Authenticator_Response> response;
	bool ok;

	request.mutable_find()->set_id(id);

	auto future = authenticator->writeRead(request);
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "authenticator" << authId << "cancelled";
		return;
	}

	if (!ok) {
		removeAuthenticator(authId);
		return;
	}

	if (response->find().has_user() && response->find().user().has_texture()) {
		const auto &texture = response->find().user().texture();
		res = QByteArray(texture.data(), texture.size());
	}
}

// Sends a server event to subscribed listeners.
void MurmurRPCImpl::sendServerEvent(const ::Server *s, const ::MurmurRPC::Server_Event &e) {
	auto serverID = s->iServerNum;
	auto&& attached = m_serverServiceListeners.getLocked(
			[&serverID](const decltype(m_serverServiceListeners)& c){
				const auto& idx = c.getServerIndex();
				return idx.equal_range(serverID);
			});

	for (auto&& item : attached) {
		auto listenerId = std::get<1>(item);
		auto listener = std::get<2>(item);
		auto cb = [this, listenerId](bool ok) -> void {
			if (!ok) {
				(this->m_serverServiceListeners.getRPCIdPtr())->erase(listenerId);
			}
		};
		listener->Write(e, cb);
	}
}

// Called when a user's state changes.
void MurmurRPCImpl::userStateChanged(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserStateChanged);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

// Called when a user sends a text message.
void MurmurRPCImpl::userTextMessage(const ::User *user, const ::TextMessage &message) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserTextMessage);
	ToRPC(s, user, event.mutable_user());
	ToRPC(s, user, message, event.mutable_message());
	sendServerEvent(s, event);
}

// Called when a user successfully connects to a server.
void MurmurRPCImpl::userConnected(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserConnected);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

// Called when a user disconnects from a server.
void MurmurRPCImpl::userDisconnected(const ::User *user) {
	::Server *s = qobject_cast< ::Server *> (sender());

	removeUserActiveContextActions(s, user);

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_UserDisconnected);
	ToRPC(s, user, event.mutable_user());
	sendServerEvent(s, event);
}

// Called when a channel's state changes.
void MurmurRPCImpl::channelStateChanged(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelStateChanged);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

// Called when a channel is created.
void MurmurRPCImpl::channelCreated(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelCreated);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

// Called when a channel is removed.
void MurmurRPCImpl::channelRemoved(const ::Channel *channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	::MurmurRPC::Server_Event event;
	event.mutable_server()->set_id(s->iServerNum);
	event.set_type(::MurmurRPC::Server_Event_Type_ChannelRemoved);
	ToRPC(s, channel, event.mutable_channel());
	sendServerEvent(s, event);
}

// Called when a user sends a text message.
void MurmurRPCImpl::textMessageFilter(int& res, unsigned int userSession, MumbleProto::TextMessage &message) {
	Server *s = qobject_cast< ::Server *> (sender());
	User* user;
	bool ok;

	std::tie(ok, user) = SERVER_USER_ALIVE(s, userSession);
	if (!ok) { // user disconnected before we were called
		return;
	}

	auto idx = m_textMessageFilters.getServerPtr();
	auto it = idx->find(s->iServerNum);
	if (it == idx->end()) {
		return;
	}
	auto filter = std::get<2>(*it).lock();
	if (filter == nullptr) {
		return;
	}
	auto filterId = filter->getId();
	idx = nullptr;

	boost::optional<MurmurRPC::TextMessage_Filter> response;
	::MurmurRPC::TextMessage_Filter request;

	request.mutable_server()->set_id(s->iServerNum);
	auto m = request.mutable_message();
	m->mutable_server()->set_id(s->iServerNum);
	m->mutable_actor()->mutable_server()->set_id(s->iServerNum);
	m->mutable_actor()->set_session(user->uiSession);
	for (int i = 0; i < message.session_size(); i++) {
		auto u = m->add_users();
		u->mutable_server()->set_id(s->iServerNum);
		u->set_session(message.session(i));
	}
	for (int i = 0; i < message.channel_id_size(); i++) {
		auto c = m->add_channels();
		c->mutable_server()->set_id(s->iServerNum);
		c->set_id(message.channel_id(i));
	}
	for (int i = 0; i < message.tree_id_size(); i++) {
		auto c = m->add_trees();
		c->mutable_server()->set_id(s->iServerNum);
		c->set_id(message.tree_id(i));
	}
	m->set_text(message.message());

	auto future = filter->writeRead(request);
	future.wait();

	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "worker" << filterId << "cancelled; accepting message";
		return;
	}

	if (!ok) {
		m_textMessageFilters.getRPCIdPtr()->erase(filterId);
		return;
	}
	res = response->action();
	switch (response->action()) {
	case ::MurmurRPC::TextMessage_Filter_Action_Accept:
		if (response->has_message() && response->message().has_text()) {
			message.set_message(response->message().text());
		}
		break;
	default:
		break;
	}
}

// Has the user been sent the given context action?
bool MurmurRPCImpl::hasActiveContextAction(const ::Server *s, const ::User *u, const QString &action) {
	const auto &m = this->m_activeContextActions;
	auto server = m.find(s->iServerNum);
	if (server == m.end()) {
		return false;
	}
	auto sessions = server->equal_range(u->uiSession);
	if ( sessions.first == server->end()) {
		return false;
	}
	for (auto it = sessions.first; it != sessions.second; ++it) {
		if (it->contains(action)) {
			return true;
		}
	}
	return false;
}

// Add the context action to the user's active context actions.
void MurmurRPCImpl::addActiveContextAction(const ::Server *s, const ::User *u, const QString &action) {
	m_activeContextActions[s->iServerNum][u->uiSession].insert(action);
}

// Remove the context action to the user's active context actions.
void MurmurRPCImpl::removeActiveContextAction(const ::Server *s, const ::User *u, const QString &action) {
	auto &m = m_activeContextActions;
	if (!m.contains(s->iServerNum)) {
		return;
	}
	auto &n = m[s->iServerNum];
	if (!n.contains(u->uiSession)) {
		return;
	}
	auto &o = n[u->uiSession];
	o.remove(action);
}

// Remove all of the user's active context actions.
void MurmurRPCImpl::removeUserActiveContextActions(const ::Server *s, const ::User *u) {
	auto &m = m_activeContextActions;
	if (m.contains(s->iServerNum)) {
		m[s->iServerNum].remove(u->uiSession);
	}
}

// Remove all of the server's active context actions.
void MurmurRPCImpl::removeActiveContextActions(const ::Server *s) {
	auto &m = m_activeContextActions;
	if (m.contains(s->iServerNum)) {
		m.remove(s->iServerNum);
	}
}

// Called when a context action event is triggered.
void MurmurRPCImpl::contextAction(const ::User *user, const QString &action, unsigned int session, int channel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	if (!hasActiveContextAction(s, user, action)) {
		return;
	}

	::MurmurRPC::ContextAction ca;
	ca.mutable_server()->set_id(s->iServerNum);
	ca.mutable_actor()->mutable_server()->set_id(s->iServerNum);
	ca.mutable_actor()->set_session(user->uiSession);
	ca.set_action(u8(action));
	if (session > 0) {
		ca.mutable_user()->mutable_server()->set_id(s->iServerNum);
		ca.mutable_user()->set_session(session);
	}
	if (channel >= 0) {
		ca.mutable_channel()->mutable_server()->set_id(s->iServerNum);
		ca.mutable_channel()->set_id(channel);
	}

	const auto& listeners = m_contextActionListeners.getLocked(
		[&s, &action] (const decltype(m_contextActionListeners)& c) {
			const auto& idx = c.getActionIndex();
			const auto& rng = idx.equal_range(std::forward_as_tuple(s->iServerNum, action.toStdString()));
			return rng;
		}
	);

	for (auto&& item : listeners) {
		auto rpcId = std::get<2>(item);
		auto listener = std::get<3>(item);

		auto cb = [this, rpcId](bool ok) {
			if (!ok) {
				m_contextActionListeners.getRPCIdPtr()->erase(rpcId);
			}
		};
		listener->Write(ca, cb);
	}
}

// *Must* functions return the requested value, or throw a ::grpc::Status
// exception.
::ServerUser *MustUser(const Server *server, unsigned int session) {
	auto user = server->qhUsers.value(session);
	if (!user || user->sState != ServerUser::Authenticated) {
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

template <class T>
int MustServerID(const T &msg) {
	if (!msg.has_server()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server");
	}
	if (!msg.server().has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server id");
	}
	auto id = msg.server().id();
	if (!ServerDB::serverExists(id)) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid server id");
	}
	return id;
}

int MustServerID(const ::MurmurRPC::Server &msg) {
	if (!msg.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing server id");
	}
	auto id = msg.id();
	if (!ServerDB::serverExists(id)) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid server id");
	}
	return id;
}

::Channel *MustChannel(const Server *server, int channel_id) {
	auto channel = server->qhChannels.value(channel_id);
	if (!channel) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid channel");
	}
	return channel;
}

::Channel *MustChannel(const Server *server, unsigned int channel_id) {
	auto channel = server->qhChannels.value(channel_id);
	if (!channel) {
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid channel");
	}
	return channel;
}

template <class T>
::Channel *MustChannel(const Server *server, const T &msg) {
	if (!msg.has_channel()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing channel");
	}
	return MustChannel(server, msg.channel());
}

template <>
::Channel *MustChannel(const Server *server, const ::MurmurRPC::Channel &msg) {
	if (!msg.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing channel id");
	}
	return MustChannel(server, msg.id());
}

// QThread::run() implementation that runs the grpc event loop and executes
// tags as callback functions.
void MurmurRPCImpl::run() {
	boost::fibers::use_scheduling_algorithm<MurmurRPC::Scheduler::grpc_scheduler>();

	void *tag;
	bool ok;
	grpc::CompletionQueue::NextStatus status;


	MurmurRPC::Wrapper::V1_Init(this, &m_V1Service);
	while (m_isRunning) {
		boost::this_fiber::yield();
		status = m_completionQueue->AsyncNext(&tag, &ok,
				std::chrono::system_clock::now() + std::chrono::milliseconds(200));

		switch(status) {
			case grpc::CompletionQueue::SHUTDOWN:
				return;
			case grpc::CompletionQueue::TIMEOUT:
				continue;
			case grpc::CompletionQueue::GOT_EVENT:
			if (tag != nullptr) {
				auto op = static_cast<std::function<void(bool)> *>(tag);
				(*op)(ok);
			}
		}
	}
}

// The Wrapper implementation methods are below. Implementation methods are
// executed in the main thread when its corresponding grpc method is invoked.
//
// Since the grpc asynchronous API is used, the implementation methods below
// do not have to complete the call during the lifetime of the method (although
// this is only used for streaming calls).

namespace MurmurRPC {
namespace Wrapper {

void V1_ServerCreate::impl(V1_ServerCreate::rpcPtr rpc, V1_ServerCreate::InType&) {
	auto id = ServerDB::addServer();

	::MurmurRPC::Server rpcServer;
	rpcServer.set_id(id);
	rpc->end(rpcServer);
}

void V1_ServerQuery::impl(V1_ServerQuery::rpcPtr rpc, V1_ServerQuery::InType&) {
	::MurmurRPC::Server_List list;

	foreach(int id, ServerDB::getAllServers()) {
		auto rpcServer = list.add_servers();
		rpcServer->set_id(id);
		try {
			auto server = MustServer(id);
			rpcServer->set_running(true);
			rpcServer->mutable_uptime()->set_secs(server->tUptime.elapsed()/1000000LL);
		} catch (::grpc::Status &ex) {
		}
	}

	rpc->end(list);
}

void V1_ServerGet::impl(V1_ServerGet::rpcPtr rpc, V1_ServerGet::InType& request) {
	auto serverID = MustServerID(request);

	::MurmurRPC::Server rpcServer;
	rpcServer.set_id(serverID);
	rpcServer.set_running(false);
	try {
		auto server = MustServer(serverID);
		rpcServer.set_running(true);
		rpcServer.mutable_uptime()->set_secs(server->tUptime.elapsed()/1000000LL);
	} catch (::grpc::Status &ex) {
	}
	rpc->end(rpcServer);
}

void V1_ServerStart::impl(V1_ServerStart::rpcPtr rpc, V1_ServerStart::InType& request) {
	auto serverID = MustServerID(request);

	if (!meta->boot(serverID)) {
		throw ::grpc::Status(::grpc::UNKNOWN, "server could not be started, or is already started");
	}

	rpc->end();
}

void V1_ServerStop::impl(V1_ServerStop::rpcPtr rpc, V1_ServerStop::InType& request) {
	auto server = MustServer(request);
	meta->kill(server->iServerNum);
	rpc->end();
}

void V1_ServerRemove::impl(V1_ServerRemove::rpcPtr rpc, V1_ServerRemove::InType& request) {
	auto serverID = MustServerID(request);

	if (meta->qhServers.value(serverID)) {
		throw ::grpc::Status(::grpc::FAILED_PRECONDITION, "cannot remove started server");
	}

	ServerDB::deleteServer(serverID);
	rpc->end();
}

void V1_ServerEvents::impl(V1_ServerEvents::rpcPtr ptr, V1_ServerEvents::InType& request) {
	auto server = MustServer(request);
	ptr->rpc->m_serverServiceListeners.emplace(
			std::forward_as_tuple(
				server->iServerNum,
				ptr->getId(),
				ptr->getWeakPtr()
			)
		);
}

void V1_ServerEvents::onDone(V1_ServerEvents::rpcPtr ptr, bool) {
	ptr->rpc->m_serverServiceListeners.getRPCIdPtr()->erase(ptr->getId());
}

void V1_GetUptime::impl(V1_GetUptime::rpcPtr rpc, V1_GetUptime::InType&) {
	::MurmurRPC::Uptime uptime;
	uptime.set_secs(meta->tUptime.elapsed()/1000000LL);
	rpc->end(uptime);
}

void V1_GetVersion::impl(V1_GetVersion::rpcPtr rpc,  V1_GetVersion::InType&) {
	::MurmurRPC::Version version;
	int major, minor, patch;
	QString release;
	Meta::getVersion(major, minor, patch, release);
	version.set_version(major << 16 | minor << 8 | patch);
	version.set_release(u8(release));
	rpc->end(version);
}

void V1_Events::impl(V1_Events::rpcPtr ptr, V1_Events::InType&) {
	qDebug("adding meta event listener: %u", ptr->getId());
	ptr->rpc->m_metaServiceListeners.emplace(
			std::forward_as_tuple(
				ptr->getId(),
				ptr->getWeakPtr()
			)
		);
}

void V1_Events::onDone(V1_Events::rpcPtr ptr, bool) {
	qDebug("done meta event listener: %u", ptr->getId());
	auto msls = ptr->rpc->m_metaServiceListeners.getRPCIdPtr();
	if (msls->erase(ptr->getId()) > 0) {
		qDebug("removed meta event listener: %u", ptr->getId());
	}
}

void V1_ContextActionAdd::impl(V1_ContextActionAdd::rpcPtr ptr, V1_ContextActionAdd::InType& request) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	if (!request.has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}
	if (!request.has_text()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing text");
	}
	if (!request.has_context()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing context");
	}

	ptr->rpc->addActiveContextAction(server, user, u8(request.action()));

	::MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(request.action());
	mpcam.set_text(request.text());
	mpcam.set_context(request.context());
	mpcam.set_operation(::MumbleProto::ContextActionModify_Operation_Add);
	server->sendMessage(user, mpcam);

	ptr->end();
}

void V1_ContextActionRemove::impl(V1_ContextActionRemove::rpcPtr ptr, V1_ContextActionRemove::InType& request) {
	auto server = MustServer(request);

	if (!request.has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}

	auto action = u8(request.action());

	::MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(request.action());
	mpcam.set_operation(::MumbleProto::ContextActionModify_Operation_Remove);

	if (request.has_user()) {
		// Remove context action from specific user
		auto user = MustUser(server, request);
		ptr->rpc->removeActiveContextAction(server, user, action);
		server->sendMessage(user, mpcam);
	} else {
		// Remove context action from all users
		foreach(::ServerUser *user, server->qhUsers) {
			if (user->sState != ServerUser::Authenticated) {
				continue;
			}
			ptr->rpc->removeActiveContextAction(server, user, action);
			server->sendMessage(user, mpcam);
		}
	}

	ptr->end();
}

void V1_ContextActionEvents::impl(V1_ContextActionEvents::rpcPtr ptr, V1_ContextActionEvents::InType& request) {
	auto server = MustServer(request);

	if (!request.has_action()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing action");
	}

	bool ok;
	std::tie(std::ignore, ok) = ptr->rpc->m_contextActionListeners.emplace(
			server->iServerNum, request.action(), ptr->getId(), ptr->getWeakPtr());
	if (!ok) {
		throw ::grpc::Status(::grpc::ABORTED, "failed to add listener");
	}
}

void V1_ContextActionEvents::onDone(V1_ContextActionEvents::rpcPtr ptr, bool) {
	auto idx = ptr->rpc->m_contextActionListeners.getRPCIdPtr();
	idx->erase(ptr->getId());
}

void V1_TextMessageSend::impl(V1_TextMessageSend::rpcPtr rpc, V1_TextMessageSend::InType& request) {
	auto server = MustServer(request);

	::MumbleProto::TextMessage tm;
	tm.set_message(request.text());
	if (request.has_actor() && request.actor().has_session()) {
		tm.set_actor(request.actor().session());
	}
	for (int i = 0; i < request.users_size(); i++) {
		if (request.users(i).has_session()) {
			tm.add_session(request.users(i).session());
		}
	}
	for (int i = 0; i < request.channels_size(); i++) {
		if (request.channels(i).has_id()) {
			tm.add_channel_id(request.channels(i).id());
		}
	}
	for (int i = 0; i < request.trees_size(); i++) {
		if (request.trees(i).has_id()) {
			tm.add_tree_id(request.trees(i).id());
		}
	}

	server->sendTextMessageGRPC(tm);

	rpc->end();
}

void V1_TextMessageFilter::impl(V1_TextMessageFilter::rpcPtr ptr) {
	bool ok;
	boost::optional<MurmurRPC::TextMessage_Filter> request;

	auto future = ptr->read();
	std::tie(ok, request) = future.get();
	if ((!ok) || ptr->m_isCancelled) {
		return;
	}
	auto server = MustServer(request.get());
	ptr->rpc->removeTextMessageFilter(server->iServerNum);
	ptr->rpc->m_textMessageFilters.emplace(
			std::forward_as_tuple(server->iServerNum, ptr->getId(), ptr->getWeakPtr()));
}

void V1_TextMessageFilter::onDone(V1_TextMessageFilter::rpcPtr ptr, bool) {
	auto idx = ptr->rpc->m_textMessageFilters.getRPCIdPtr();

	if( idx->erase( ptr->getId() ) > 0 ) {
		qDebug() << "erased worker" << ptr->getId() << "from filter list";
	}
}

void V1_LogQuery::impl(V1_LogQuery::rpcPtr rpc, V1_LogQuery::InType& request) {
	auto serverID = MustServerID(request);

	int total = ::ServerDB::getLogLen(serverID);
	if (total < 0) {
		throw ::grpc::Status(::grpc::StatusCode::UNAVAILABLE, "could not access database");
	}

	::MurmurRPC::Log_List list;
	list.mutable_server()->set_id(serverID);
	list.set_total(total);

	if (!request.has_min() || !request.has_max()) {
		rpc->end(list);
		return;
	}
	list.set_min(request.min());
	list.set_max(request.max());

	auto dblog = ::ServerDB::getLog(serverID, request.min(), request.max());
	foreach(const ::ServerDB::LogRecord &record, dblog) {
		auto rpcLog = list.add_entries();
		ToRPC(serverID, record, rpcLog);
	}

	rpc->end(list);
}

void V1_ConfigGet::impl(V1_ConfigGet::rpcPtr rpc, V1_ConfigGet::InType& request) {
	auto serverID = MustServerID(request);
	auto config = ServerDB::getAllConf(serverID);

	::MurmurRPC::Config rpcConfig;
	rpcConfig.mutable_server()->set_id(serverID);
	auto &fields = *rpcConfig.mutable_fields();
	for (auto i = config.constBegin(); i != config.constEnd(); ++i) {
		fields[u8(i.key())] = u8(i.value());
	}

	rpc->end(rpcConfig);
}

void V1_ConfigGetField::impl(V1_ConfigGetField::rpcPtr rpc, V1_ConfigGetField::InType& request) {
	auto serverID = MustServerID(request);
	if (!request.has_key()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing key");
	}
	::MurmurRPC::Config_Field rpcField;
	rpcField.mutable_server()->set_id(serverID);
	rpcField.set_key(request.key());
	rpcField.set_value(u8(ServerDB::getConf(serverID, u8(request.key()), QVariant()).toString()));
	rpc->end(rpcField);
}

void V1_ConfigSetField::impl(V1_ConfigSetField::rpcPtr rpc, V1_ConfigSetField::InType& request) {
	auto serverID = MustServerID(request);
	if (!request.has_key()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing key");
	}
	QString key = u8(request.key());
	QString value;
	if (request.has_value()) {
		value = u8(request.value());
	}
	ServerDB::setConf(serverID, key, value);
	try {
		auto server = MustServer(serverID);
		server->setLiveConf(key, value);
	} catch (::grpc::Status &ex) {
	}

	rpc->end();
}

void V1_ConfigGetDefault::impl(V1_ConfigGetDefault::rpcPtr rpc, V1_ConfigGetDefault::InType&) {
	::MurmurRPC::Config rpcConfig;
	auto &fields = *rpcConfig.mutable_fields();
	for (auto i = meta->mp.qmConfig.constBegin(); i != meta->mp.qmConfig.constEnd(); ++i) {
		fields[u8(i.key())] = u8(i.value());
	}

	rpc->end(rpcConfig);
}

void V1_ChannelQuery::impl(V1_ChannelQuery::rpcPtr rpc, V1_ChannelQuery::InType& request) {
	auto server = MustServer(request);

	::MurmurRPC::Channel_List list;
	list.mutable_server()->set_id(server->iServerNum);

	foreach(const ::Channel *channel, server->qhChannels) {
		auto rpcChannel = list.add_channels();
		ToRPC(server, channel, rpcChannel);
	}

	rpc->end(list);
}

void V1_ChannelGet::impl(V1_ChannelGet::rpcPtr rpc, V1_ChannelGet::InType& request) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MurmurRPC::Channel rpcChannel;
	ToRPC(server, channel, &rpcChannel);
	rpc->end(rpcChannel);
}

void V1_ChannelAdd::impl(V1_ChannelAdd::rpcPtr rpc, V1_ChannelAdd::InType& request) {
	auto server = MustServer(request);

	if (!request.has_parent() || !request.has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "parent channel and name required");
	}
	::Channel *parent;
	try {
		parent = MustChannel(server, request.parent());
	} catch (::grpc::Status &ex) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid parent channel");
	}

	if (!server->canNest(parent)) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "cannot nest channel in given parent");
	}

	QString qsName = u8(request.name());

	::Channel *nc;

	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		nc = server->addChannel(parent, qsName, request.temporary(), request.position());
	}

	nc->qsDesc = u8(request.description());

	server->updateChannel(nc);
	int newid = nc->iId;

	::MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(newid);
	mpcs.set_parent(parent->iId);
	mpcs.set_name(request.name());
	mpcs.set_temporary(request.temporary());
	mpcs.set_position(request.position());
	mpcs.set_description(request.description());
	server->sendAll(mpcs);

	::MurmurRPC::Channel resChannel;
	ToRPC(server, nc, &resChannel);
	rpc->end(resChannel);
}

void V1_ChannelRemove::impl(V1_ChannelRemove::rpcPtr rpc, V1_ChannelRemove::InType& request) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	if (!channel->cParent) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "cannot remove the root channel");
	}

	server->removeChannel(channel);

	rpc->end();
}

void V1_ChannelUpdate::impl(V1_ChannelUpdate::rpcPtr rpc, V1_ChannelUpdate::InType& request) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(channel->iId);
	for (int i = 0; i < request.links_size(); i++) {
		if (!request.links(i).has_id()) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid channel link");
		}
		mpcs.add_links(request.links(i).id());
	}
	if (request.has_parent()) {
		if (!request.parent().has_id()) {
			throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid channel parent");
		}
		mpcs.set_parent(request.parent().id());
	}
	if (request.has_name()) {
		mpcs.set_name(request.name());
	}
	if (request.has_position()) {
		mpcs.set_position(request.position());
	}
	if (request.has_description()) {
		mpcs.set_description(request.description());
	}

	QString err;
	if (!server->setChannelStateGRPC(mpcs, err)) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, u8(err));
	}

	::MurmurRPC::Channel rpcChannel;
	ToRPC(server, channel, &rpcChannel);
	rpc->end(rpcChannel);
}

void V1_UserQuery::impl(V1_UserQuery::rpcPtr rpc, V1_UserQuery::InType& request) {
	auto server = MustServer(request);

	::MurmurRPC::User_List list;
	list.mutable_server()->set_id(server->iServerNum);

	foreach(const ::ServerUser *user, server->qhUsers) {
		if (user->sState != ServerUser::Authenticated) {
			continue;
		}
		auto rpcUser = list.add_users();
		ToRPC(server, user, rpcUser);
	}

	rpc->end(list);
}

void V1_UserGet::impl(V1_UserGet::rpcPtr rpc, V1_UserGet::InType& request) {
	auto server = MustServer(request);

	::MurmurRPC::User rpcUser;

	if (request.has_session()) {
		// Lookup user by session
		auto user = MustUser(server, request);
		ToRPC(server, user, &rpcUser);
		rpc->end(rpcUser);
		return;
	} else if (request.has_name()) {
		// Lookup user by name
		QString qsName = u8(request.name());
		foreach(const ::ServerUser *user, server->qhUsers) {
			if (user->sState != ServerUser::Authenticated) {
				continue;
			}
			if (user->qsName == qsName) {
				ToRPC(server, user, &rpcUser);
				rpc->end(rpcUser);
				return;
			}
		}
		throw ::grpc::Status(::grpc::NOT_FOUND, "invalid user");
	}

	throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "session or name required");
}

void V1_UserUpdate::impl(V1_UserUpdate::rpcPtr rpc, V1_UserUpdate::InType& request) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	auto channel = user->cChannel;
	if (request.has_channel()) {
		channel = MustChannel(server, request.channel());
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

	server->setUserState(user, channel, mute, deaf, suppress, prioritySpeaker, name, comment);

	::MurmurRPC::User rpcUser;
	ToRPC(server, user, &rpcUser);
	rpc->end(rpcUser);
}

void V1_UserKick::impl(V1_UserKick::rpcPtr rpc, V1_UserKick::InType& request) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	::MumbleProto::UserRemove mpur;
	mpur.set_session(user->uiSession);
	if (request.has_reason()) {
		mpur.set_reason(request.reason());
	}
	if (request.has_actor() && request.actor().has_session()) {
		mpur.set_actor(request.actor().session());
	}
	server->sendAll(mpur);
	user->disconnectSocket();

	rpc->end();
}

void V1_TreeQuery::impl(V1_TreeQuery::rpcPtr rpc, V1_TreeQuery::InType& request) {
	auto server = MustServer(request);

	auto channel = MustChannel(server, 0);
	::MurmurRPC::Tree root;

	QQueue< QPair<const ::Channel *, ::MurmurRPC::Tree *> > qQueue;
	qQueue.enqueue(qMakePair(channel, &root));

	while (!qQueue.isEmpty()) {
		auto current = qQueue.dequeue();
		auto currentChannel = current.first;
		auto currentTree = current.second;

		ToRPC(server, currentChannel, currentTree->mutable_channel());

		QList< ::User *> users = currentChannel->qlUsers;
		std::sort(users.begin(), users.end(), [] (const ::User *a, const ::User *b) -> bool {
			return ::User::lessThan(a, b);
		});
		foreach(const ::User *u, users) {
			auto rpcUser = currentTree->add_users();
			ToRPC(server, u, rpcUser);
		}

		QList< ::Channel *> channels = currentChannel->qlChannels;
		std::sort(channels.begin(), channels.end(), [] (const ::Channel *a, const ::Channel *b) -> bool {
			return ::Channel::lessThan(a, b);
		});
		foreach(const ::Channel *subChannel, channels) {
			auto subTree = currentTree->add_children();
			qQueue.enqueue(qMakePair(subChannel, subTree));
		}
	}

	rpc->end(root);
}

void V1_BansGet::impl(V1_BansGet::rpcPtr rpc, V1_BansGet::InType& request) {
	auto server = MustServer(request);

	::MurmurRPC::Ban_List list;
	list.mutable_server()->set_id(server->iServerNum);
	foreach(const ::Ban &ban, server->qlBans) {
		auto rpcBan = list.add_bans();
		ToRPC(server, ban, rpcBan);
	}

	rpc->end(list);
}

void V1_BansSet::impl(V1_BansSet::rpcPtr rpc, V1_BansSet::InType& request) {
	auto server = MustServer(request);
	server->qlBans.clear();

	for (int i = 0; i < request.bans_size(); i++) {
		const auto &rpcBan = request.bans(i);
		::Ban ban;
		FromRPC(server, rpcBan, ban);
		server->qlBans << ban;
	}
	server->saveBans();

	rpc->end();
}

void V1_ACLGet::impl(V1_ACLGet::rpcPtr rpc, V1_ACLGet::InType& request) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::MurmurRPC::ACL_List list;
	list.set_inherit(channel->bInheritACL);

	QStack< ::Channel *> chans;
	ChanACL *acl;
	::Channel *p = channel;
	while (p) {
		chans.push(p);
		if ((p == channel) || p->bInheritACL) {
			p = p->cParent;
		} else {
			p = NULL;
		}
	}

	while (!chans.isEmpty()) {
		p = chans.pop();
		foreach(acl, p->qlACL) {
			if (p == channel || acl->bApplySubs) {
				auto rpcACL = list.add_acls();
				ToRPC(server, acl, rpcACL);
				if (p != channel) {
					rpcACL->set_inherited(true);
				}
			}
		}
	}

	p = channel->cParent;
	const QSet<QString> allnames = ::Group::groupNames(channel);
	foreach(const QString &name, allnames) {
		::Group *g = channel->qhGroups.value(name);
		::Group *pg = p ? ::Group::getGroup(p, name) : NULL;
		if (!g && ! pg) {
			continue;
		}
		auto aclGroup = list.add_groups();
		ToRPC(server, g ? g : pg, aclGroup);
		QSet<int> members;
		if (pg) {
			members = pg->members();
		}
		if (g) {
			foreach(auto id, g->qsAdd) {
				auto rpcUser = aclGroup->add_users_add();
				rpcUser->mutable_server()->set_id(server->iServerNum);
				rpcUser->set_id(id);
			}
			foreach(auto id, g->qsRemove) {
				auto rpcUser = aclGroup->add_users_remove();
				rpcUser->mutable_server()->set_id(server->iServerNum);
				rpcUser->set_id(id);
			}
			aclGroup->set_inherited(false);
			members += g->qsAdd;
			members -= g->qsRemove;
		} else {
			aclGroup->set_inherited(true);
		}
		foreach(auto id, members) {
			auto rpcUser = aclGroup->add_users();
			rpcUser->mutable_server()->set_id(server->iServerNum);
			rpcUser->set_id(id);
		}
	}

	rpc->end(list);
}

void V1_ACLSet::impl(V1_ACLSet::rpcPtr rpc, V1_ACLSet::InType& request) {
	auto server = MustServer(request);
	auto channel = MustChannel(server, request);

	::Group *g;
	::ChanACL *acl;

	{
		QWriteLocker wl(&server->qrwlVoiceThread);

		QHash<QString, QSet<int> > hOldTemp;
		foreach(g, channel->qhGroups) {
			hOldTemp.insert(g->qsName, g->qsTemporary);
			delete g;
		}
		foreach(acl, channel->qlACL) {
			delete acl;
		}

		channel->qhGroups.clear();
		channel->qlACL.clear();

		channel->bInheritACL = request.inherit();

		for (int i = 0; i < request.groups_size(); i++) {
			auto &rpcGroup = request.groups(i);
			auto name = u8(rpcGroup.name());
			g = new ::Group(channel, name);
			g->bInherit = rpcGroup.inherit();
			g->bInheritable = rpcGroup.inheritable();
			for (int j = 0; j < rpcGroup.users_add_size(); j++) {
				g->qsAdd.insert(rpcGroup.users_add(j).id());
			}
			for (int j = 0; j < rpcGroup.users_remove_size(); j++) {
				g->qsRemove.insert(rpcGroup.users_remove(j).id());
			}
			g->qsTemporary = hOldTemp.value(name);
		}
		for (int i = 0; i < request.acls_size(); i++) {
			auto &rpcACL = request.acls(i);
			acl = new ChanACL(channel);
			acl->bApplyHere = rpcACL.apply_here();
			acl->bApplySubs = rpcACL.apply_subs();
			if (rpcACL.has_user()) {
				acl->iUserId = rpcACL.user().id();
			}
			if (rpcACL.has_group() && rpcACL.group().has_name()) {
				acl->qsGroup = u8(rpcACL.group().name());
			}
			acl->pDeny = static_cast<ChanACL::Permissions>(rpcACL.deny()) & ChanACL::All;
			acl->pAllow = static_cast<ChanACL::Permissions>(rpcACL.allow()) & ChanACL::All;
		}
	}

	server->clearACLCache();
	server->updateChannel(channel);

	rpc->end();
}

void V1_ACLGetEffectivePermissions::impl(V1_ACLGetEffectivePermissions::rpcPtr rpc, V1_ACLGetEffectivePermissions::InType& request) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);
	auto channel = MustChannel(server, request);

	auto flags = int(server->effectivePermissions(user, channel));

	::MurmurRPC::ACL rpcACL;
	rpcACL.set_allow(::MurmurRPC::ACL_Permission(flags));
	rpc->end(rpcACL);
}

void V1_ACLAddTemporaryGroup::impl(V1_ACLAddTemporaryGroup::rpcPtr rpc, V1_ACLAddTemporaryGroup::InType& request) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);
	auto channel = MustChannel(server, request);

	if (!request.has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing name");
	}

	QString qsgroup = u8(request.name());
	if (qsgroup.isEmpty()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "empty name");
	}

	{
		QWriteLocker wl(&server->qrwlVoiceThread);

		::Group *g = channel->qhGroups.value(qsgroup);
		if (!g) {
			g = new ::Group(channel, qsgroup);
		}

		g->qsTemporary.insert(-user->uiSession);
	}

	server->clearACLCache(user);

	rpc->end();
}

void V1_ACLRemoveTemporaryGroup::impl(V1_ACLRemoveTemporaryGroup::rpcPtr rpc, V1_ACLRemoveTemporaryGroup::InType& request) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);
	auto channel = MustChannel(server, request);

	if (!request.has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing name");
	}

	QString qsgroup = u8(request.name());
	if (qsgroup.isEmpty()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "empty name");
	}

	{
		QWriteLocker wl(&server->qrwlVoiceThread);

		::Group *g = channel->qhGroups.value(qsgroup);
		if (!g) {
			g = new ::Group(channel, qsgroup);
		}

		g->qsTemporary.remove(-user->uiSession);
	}

	server->clearACLCache(user);

	rpc->end();
}

void V1_AuthenticatorStream::impl(V1_AuthenticatorStream::rpcPtr ptr) {
	bool ok;
	boost::optional<MurmurRPC::Authenticator_Response> response;

	auto future = ptr->read();
	try {
		std::tie(ok, response) = future.get();
	} catch (const boost::fibers::broken_promise& ex) {
		qDebug() << "error reading authenticatorstream request";
		return;
	}

	if (!ok) {
		return;
	}

	if (!response->has_initialize()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing initialize");
	}

	auto server = MustServer(response->initialize());
	ptr->rpc->removeAuthenticator(server->iServerNum);
	std::tie(std::ignore, ok) = ptr->rpc->m_authenticators.emplace(
		std::forward_as_tuple(server->iServerNum, ptr->getId(), ptr->getWeakPtr())
	);

	if (!ok) {
		throw ::grpc::Status(::grpc::ABORTED, "internal update failure");
	}
}

void V1_AuthenticatorStream::onDone(V1_AuthenticatorStream::rpcPtr ptr, bool) {
	auto idx = ptr->rpc->m_authenticators.getRPCIdPtr();
	auto count = idx->erase(ptr->getId());
	if(count > 0) {
		qDebug("erased %lu authenticators from stream with id %u", count, ptr->getId());
	}
}

void V1_DatabaseUserQuery::impl(V1_DatabaseUserQuery::rpcPtr rpc, V1_DatabaseUserQuery::InType& request) {
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

	rpc->end(list);
}

void V1_DatabaseUserGet::impl(V1_DatabaseUserGet::rpcPtr rpc, V1_DatabaseUserGet::InType& request) {
	auto server = MustServer(request);

	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing id");
	}
	auto info = server->getRegistration(request.id());
	if (info.isEmpty()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}
	auto texture = server->getUserTexture(request.id());

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	ToRPC(server, info, texture, &rpcDatabaseUser);
	rpc->end(rpcDatabaseUser);
}

void V1_DatabaseUserUpdate::impl(V1_DatabaseUserUpdate::rpcPtr rpc, V1_DatabaseUserUpdate::InType& request) {
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
	if (request.has_texture()) {
		server->setTexture(request.id(), QByteArray(request.texture().c_str(), request.texture().size()));
	}

	if (info.contains(ServerDB::User_Name) || info.contains(ServerDB::User_Comment)) {
		foreach(::ServerUser *u, server->qhUsers) {
			if (static_cast<unsigned int>(u->iId) == request.id()) {
				QString name = u->qsName;
				QString comment = u->qsComment;
				if (info.contains(ServerDB::User_Name)) {
					name = info.value(ServerDB::User_Name);
				}
				if (info.contains(ServerDB::User_Comment)) {
					comment = info.value(ServerDB::User_Comment);
				}
				server->setUserState(u, u->cChannel, u->bMute, u->bDeaf, u->bSuppress, u->bPrioritySpeaker, name, comment);
				break;
			}
		}
	}

	rpc->end();
}

void V1_DatabaseUserRegister::impl(V1_DatabaseUserRegister::rpcPtr rpc, V1_DatabaseUserRegister::InType& request) {
	auto server = MustServer(request);

	QMap<int, QString> info;
	FromRPC(request, info);

	auto userid = server->registerUser(info);
	if (userid < 0) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}
	QByteArray texture;
	if (request.has_texture()) {
		texture = QByteArray(request.texture().c_str(), request.texture().size());
		server->setTexture(request.id(), texture);
	}

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	rpcDatabaseUser.set_id(userid);
	ToRPC(server, info, texture, &rpcDatabaseUser);
	rpc->end(rpcDatabaseUser);
}

void V1_DatabaseUserDeregister::impl(V1_DatabaseUserDeregister::rpcPtr rpc, V1_DatabaseUserDeregister::InType& request) {
	auto server = MustServer(request);

	if (!request.has_id()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing id");
	}
	if (!server->unregisterUser(request.id())) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid user");
	}

	rpc->end();
}

void V1_DatabaseUserVerify::impl(V1_DatabaseUserVerify::rpcPtr rpc, V1_DatabaseUserVerify::InType& request) {
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
	case -1:
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "invalid username and/or password");
	case -2:
		throw ::grpc::Status(::grpc::NOT_FOUND, "unknown user");
	case -3:
		throw ::grpc::Status(::grpc::UNAVAILABLE, "authenticator temporarily unavailable");
	}

	::MurmurRPC::DatabaseUser rpcDatabaseUser;
	rpcDatabaseUser.mutable_server()->set_id(server->iServerNum);
	rpcDatabaseUser.set_id(ret);
	rpc->end(rpcDatabaseUser);
}

void V1_RedirectWhisperGroupAdd::impl(V1_RedirectWhisperGroupAdd::rpcPtr rpc, V1_RedirectWhisperGroupAdd::InType& request) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	if (!request.source().has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing source name");
	}
	if (!request.target().has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing target name");
	}

	QString qssource = u8(request.source().name());
	QString qstarget = u8(request.target().name());

	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		user->qmWhisperRedirect.insert(qssource, qstarget);
	}

	server->clearACLCache(user);

	rpc->end();
}

void V1_RedirectWhisperGroupRemove::impl(V1_RedirectWhisperGroupRemove::rpcPtr rpc, V1_RedirectWhisperGroupRemove::InType& request) {
	auto server = MustServer(request);
	auto user = MustUser(server, request);

	if (!request.source().has_name()) {
		throw ::grpc::Status(::grpc::INVALID_ARGUMENT, "missing source name");
	}

	QString qssource = u8(request.source().name());

	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		user->qmWhisperRedirect.remove(qssource);
	}

	server->clearACLCache(user);

	rpc->end();
}

}
}

/// \brief QDebug can't handle std::string...
QDebug operator<<(QDebug dbg, const std::string& s) {
	dbg << s.c_str();
	return dbg;
}
