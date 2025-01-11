// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MumbleServerIce.h"

#include "Ban.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "Group.h"
#include "Meta.h"
#include "MumbleServer.h"
#include "QtUtils.h"
#include "Server.h"
#include "ServerDB.h"
#include "ServerUser.h"
#include "User.h"
#include "Utils.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QStack>

#include <openssl/err.h>

#include <Ice/Ice.h>
#include <Ice/SliceChecksums.h>
#include <IceUtil/IceUtil.h>

#include <limits>

using namespace std;
using namespace MumbleServer;

static MumbleServerIce *mi = nullptr;
static Ice::ObjectPtr iopServer;
static Ice::PropertiesPtr ippProperties;

void IceParse(int &argc, char *argv[]) {
	ippProperties = Ice::createProperties(argc, argv);
}

void IceStart() {
	mi = new MumbleServerIce();
}

void IceStop() {
	delete mi;
	mi = nullptr;
}

/// Remove all NUL bytes from |s|.
static std::string iceRemoveNul(std::string s) {
	std::vector< char > newstr;
	for (size_t i = 0; i < s.size(); i++) {
		char c = s.at(i);
		if (c == 0) {
			continue;
		}
		newstr.push_back(s.at(i));
	}
	return std::string(newstr.begin(), newstr.end());
}

/// Marshall the QString |s| to be safe for use on
/// the wire in Ice messages, parameters
/// and return values.
///
/// What happens under the hood is that the string
/// is converted to UTF-8, and all NUL bytes are
/// removed.
static std::string iceString(const QString &s) {
	return iceRemoveNul(u8(s));
}

/// Convert the bytes in std::string to base64 using the
/// base64 alphabet from RFC 2045.
///
/// The size of the string may not exceed sizeof(int).
/// If the function is passed a string bigger than that,
/// it will return an empty string.
static std::string iceBase64(const std::string &s) {
	if (s.size() > static_cast< size_t >(std::numeric_limits< int >::max())) {
		return std::string();
	}

	QByteArray ba(s.data(), static_cast< int >(s.size()));
	QByteArray ba64 = ba.toBase64();

	return std::string(ba64.data(), static_cast< size_t >(ba64.size()));
}

static void logToLog(const ServerDB::LogRecord &r, ::MumbleServer::LogEntry &entry) {
	entry.timestamp = static_cast< int >(r.first);
	entry.txt       = iceString(r.second);
}

static void userToUser(const ::User *p, ::MumbleServer::User &mp) {
	mp.session         = static_cast< int >(p->uiSession);
	mp.userid          = p->iId;
	mp.name            = iceString(p->qsName);
	mp.mute            = p->bMute;
	mp.deaf            = p->bDeaf;
	mp.suppress        = p->bSuppress;
	mp.recording       = p->bRecording;
	mp.prioritySpeaker = p->bPrioritySpeaker;
	mp.selfMute        = p->bSelfMute;
	mp.selfDeaf        = p->bSelfDeaf;
	mp.channel         = static_cast< int >(p->cChannel->iId);
	mp.comment         = iceString(p->qsComment);

	const ServerUser *u = static_cast< const ServerUser * >(p);
	mp.onlinesecs       = u->bwr.onlineSeconds();
	mp.bytespersec      = u->bwr.bandwidth();
	mp.version2         = static_cast< long >(u->m_version);
	mp.version          = static_cast< int >(Version::toLegacyVersion(u->m_version));
	mp.release          = iceString(u->qsRelease);
	mp.os               = iceString(u->qsOS);
	mp.osversion        = iceString(u->qsOSVersion);
	mp.identity         = iceString(u->qsIdentity);
	mp.context          = iceBase64(u->ssContext);
	mp.idlesecs         = u->bwr.idleSeconds();
	mp.udpPing          = u->dUDPPingAvg;
	mp.tcpPing          = u->dTCPPingAvg;

	mp.tcponly = u->aiUdpFlag.loadRelaxed() == 0;

	::MumbleServer::NetAddress addr(16, 0);
	for (unsigned int i = 0; i < 16; ++i) {
		addr[i] = u->haAddress.getByteRepresentation()[i];
	}

	mp.address = addr;
}

static void channelToChannel(const ::Channel *c, ::MumbleServer::Channel &mc) {
	mc.id          = static_cast< int >(c->iId);
	mc.name        = iceString(c->qsName);
	mc.parent      = c->cParent ? static_cast< int >(c->cParent->iId) : -1;
	mc.description = iceString(c->qsDesc);
	mc.position    = c->iPosition;
	mc.links.clear();
	foreach (::Channel *chn, c->qsPermLinks)
		mc.links.push_back(static_cast< int >(chn->iId));
	mc.temporary = c->bTemporary;
}

static void ACLtoACL(const ::ChanACL *acl, ::MumbleServer::ACL &ma) {
	ma.applyHere = acl->bApplyHere;
	ma.applySubs = acl->bApplySubs;
	ma.inherited = false;
	ma.userid    = acl->iUserId;
	ma.group     = iceString(acl->qsGroup);
	ma.allow     = static_cast< int >(acl->pAllow);
	ma.deny      = static_cast< int >(acl->pDeny);
}

static void groupToGroup(const ::Group *g, ::MumbleServer::Group &mg) {
	mg.name        = iceString(g->qsName);
	mg.inherit     = g->bInherit;
	mg.inheritable = g->bInheritable;
	mg.add.clear();
	mg.remove.clear();
	mg.members.clear();
}

static void banToBan(const ::Ban &b, ::MumbleServer::Ban &mb) {
	::MumbleServer::NetAddress addr(16, 0);
	for (unsigned int i = 0; i < 16; ++i) {
		addr[i] = b.haAddress.getByteRepresentation()[i];
	}

	mb.address  = addr;
	mb.bits     = b.iMask;
	mb.name     = iceString(b.qsUsername);
	mb.hash     = iceString(b.qsHash);
	mb.reason   = iceString(b.qsReason);
	mb.start    = static_cast< int >(b.qdtStart.toLocalTime().toSecsSinceEpoch());
	mb.duration = static_cast< int >(b.iDuration);
}

static void banToBan(const ::MumbleServer::Ban &mb, ::Ban &b) {
	if (mb.address.size() != 16) {
		b.haAddress.reset();
	} else {
		for (unsigned int i = 0; i < 16; ++i) {
			b.haAddress.setByte(i, mb.address[i]);
		}
	}
	b.iMask      = mb.bits;
	b.qsUsername = u8(mb.name);
	b.qsHash     = u8(mb.hash);
	b.qsReason   = u8(mb.reason);
	b.qdtStart   = QDateTime::fromSecsSinceEpoch(static_cast< quint32 >(mb.start)).toUTC();
	b.iDuration  = static_cast< unsigned int >(mb.duration);
}

static void infoToInfo(const QMap< int, QString > &info, ::MumbleServer::UserInfoMap &im) {
	QMap< int, QString >::const_iterator i;
	for (i = info.constBegin(); i != info.constEnd(); ++i)
		im[static_cast<::MumbleServer::UserInfo >(i.key())] = iceString(i.value());
}

static void infoToInfo(const ::MumbleServer::UserInfoMap &im, QMap< int, QString > &info) {
	::MumbleServer::UserInfoMap::const_iterator i;
	for (i = im.begin(); i != im.end(); ++i)
		info.insert((*i).first, u8((*i).second));
}

static void textmessageToTextmessage(const ::TextMessage &tm, ::MumbleServer::TextMessage &tmdst) {
	tmdst.text = iceString(tm.qsText);

	foreach (unsigned int i, tm.qlSessions)
		tmdst.sessions.push_back(static_cast< int >(i));

	foreach (unsigned int i, tm.qlChannels)
		tmdst.channels.push_back(static_cast< int >(i));

	foreach (unsigned int i, tm.qlTrees)
		tmdst.trees.push_back(static_cast< int >(i));
}

class ServerLocator : public virtual Ice::ServantLocator {
public:
	virtual Ice::ObjectPtr locate(const Ice::Current &, Ice::LocalObjectPtr &);
	virtual void finished(const Ice::Current &, const Ice::ObjectPtr &, const Ice::LocalObjectPtr &){};
	virtual void deactivate(const std::string &){};
};

MumbleServerIce::MumbleServerIce() {
	count = 0;

	if (::Meta::mp->qsIceEndpoint.isEmpty())
		return;

	Ice::PropertiesPtr ipp = Ice::createProperties();

	::Meta::mp->qsSettings->beginGroup("Ice");
	foreach (const QString &v, ::Meta::mp->qsSettings->childKeys()) {
		ipp->setProperty(iceString(v), iceString(::Meta::mp->qsSettings->value(v).toString()));
	}
	::Meta::mp->qsSettings->endGroup();

	Ice::PropertyDict props = ippProperties->getPropertiesForPrefix("");
	Ice::PropertyDict::iterator i;
	for (i = props.begin(); i != props.end(); ++i) {
		ipp->setProperty((*i).first, (*i).second);
	}
	ipp->setProperty("Ice.ImplicitContext", "Shared");

	Ice::InitializationData idd;
	idd.properties = ipp;

	try {
		communicator = Ice::initialize(idd);
		if (!::Meta::mp->qsIceSecretWrite.isEmpty()) {
			::Ice::ImplicitContextPtr impl = communicator->getImplicitContext();
			if (impl)
				impl->put("secret", iceString(::Meta::mp->qsIceSecretWrite));
		}
		adapter =
			communicator->createObjectAdapterWithEndpoints("Mumble Server", qPrintable(::Meta::mp->qsIceEndpoint));
		MetaPtr m = new MetaI;
#if ICE_INT_VERSION >= 30700
		MetaPrx mprx = MetaPrx::uncheckedCast(adapter->add(m, Ice::stringToIdentity("Meta")));
#else
		MetaPrx mprx = MetaPrx::uncheckedCast(adapter->add(m, communicator->stringToIdentity("Meta")));
#endif
		adapter->addServantLocator(new ServerLocator(), "s");

		iopServer = new ServerI;

		adapter->activate();
		foreach (const Ice::EndpointPtr ep, mprx->ice_getEndpoints()) {
			qWarning("MumbleServerIce: Endpoint \"%s\" running", qPrintable(u8(ep->toString())));
		}

		meta->connectListener(this);
	} catch (Ice::Exception &e) {
#if ICE_INT_VERSION >= 30700
		qCritical("MumbleServerIce: Initialization failed: %s", qPrintable(u8(e.ice_id())));
#else
		qCritical("MumbleServerIce: Initialization failed: %s", qPrintable(u8(e.ice_name())));
#endif
	}
}

MumbleServerIce::~MumbleServerIce() {
	if (communicator) {
		communicator->shutdown();
		communicator->waitForShutdown();
		communicator->destroy();
		communicator = nullptr;
		qWarning("MumbleServerIce: Shutdown complete");
	}
	iopServer = nullptr;
}

void MumbleServerIce::customEvent(QEvent *evt) {
	if (evt->type() == EXEC_QEVENT)
		static_cast< ExecEvent * >(evt)->execute();
}

void MumbleServerIce::badMetaProxy(const ::MumbleServer::MetaCallbackPrx &prx) {
	qCritical("Ice MetaCallback %s failed", qPrintable(QString::fromStdString(communicator->proxyToString(prx))));
	removeMetaCallback(prx);
}

void MumbleServerIce::badServerProxy(const ::MumbleServer::ServerCallbackPrx &prx, const ::Server *server) {
	server->log(QString("Ice ServerCallback %1 failed").arg(QString::fromStdString(communicator->proxyToString(prx))));
	removeServerCallback(server, prx);
}

void MumbleServerIce::badAuthenticator(::Server *server) {
	server->disconnectAuthenticator(this);
	const ::MumbleServer::ServerAuthenticatorPrx &prx = qmServerAuthenticator.value(server->iServerNum);
	server->log(QString("Ice Authenticator %1 failed").arg(QString::fromStdString(communicator->proxyToString(prx))));
	removeServerAuthenticator(server);
	removeServerUpdatingAuthenticator(server);
}

void MumbleServerIce::addMetaCallback(const ::MumbleServer::MetaCallbackPrx &prx) {
	if (!qlMetaCallbacks.contains(prx)) {
		qWarning("Added Ice MetaCallback %s", qPrintable(QString::fromStdString(communicator->proxyToString(prx))));
		qlMetaCallbacks.append(prx);
	}
}

void MumbleServerIce::removeMetaCallback(const ::MumbleServer::MetaCallbackPrx &prx) {
	if (qlMetaCallbacks.removeAll(prx)) {
		qWarning("Removed Ice MetaCallback %s", qPrintable(QString::fromStdString(communicator->proxyToString(prx))));
	}
}

void MumbleServerIce::addServerCallback(const ::Server *server, const ::MumbleServer::ServerCallbackPrx &prx) {
	QList<::MumbleServer::ServerCallbackPrx > &cbList = qmServerCallbacks[server->iServerNum];

	if (!cbList.contains(prx)) {
		server->log(
			QString("Added Ice ServerCallback %1").arg(QString::fromStdString(communicator->proxyToString(prx))));
		cbList.append(prx);
	}
}

void MumbleServerIce::removeServerCallback(const ::Server *server, const ::MumbleServer::ServerCallbackPrx &prx) {
	if (qmServerCallbacks[server->iServerNum].removeAll(prx)) {
		server->log(
			QString("Removed Ice ServerCallback %1").arg(QString::fromStdString(communicator->proxyToString(prx))));
	}
}

void MumbleServerIce::removeServerCallbacks(const ::Server *server) {
	if (qmServerCallbacks.contains(server->iServerNum)) {
		server->log(QString("Removed all Ice ServerCallbacks"));
		qmServerCallbacks.remove(server->iServerNum);
	}
}

void MumbleServerIce::addServerContextCallback(const ::Server *server, int session_id, const QString &action,
											   const ::MumbleServer::ServerContextCallbackPrx &prx) {
	QMap< QString, ::MumbleServer::ServerContextCallbackPrx > &callbacks =
		qmServerContextCallbacks[server->iServerNum][session_id];

	if (!callbacks.contains(action) || callbacks[action] != prx) {
		server->log(QString("Added Ice ServerContextCallback %1 for session %2, action %3")
						.arg(QString::fromStdString(communicator->proxyToString(prx)))
						.arg(session_id)
						.arg(action));
		callbacks.insert(action, prx);
	}
}

const QMap< int, QMap< QString, ::MumbleServer::ServerContextCallbackPrx > >
	MumbleServerIce::getServerContextCallbacks(const ::Server *server) const {
	return qmServerContextCallbacks[server->iServerNum];
}

void MumbleServerIce::removeServerContextCallback(const ::Server *server, int session_id, const QString &action) {
	if (qmServerContextCallbacks[server->iServerNum][session_id].remove(action)) {
		server->log(QString("Removed Ice ServerContextCallback for session %1, action %2").arg(session_id).arg(action));
	}
}

void MumbleServerIce::setServerAuthenticator(const ::Server *server,
											 const ::MumbleServer::ServerAuthenticatorPrx &prx) {
	if (prx != qmServerAuthenticator[server->iServerNum]) {
		server->log(
			QString("Set Ice Authenticator to %1").arg(QString::fromStdString(communicator->proxyToString(prx))));
		qmServerAuthenticator[server->iServerNum] = prx;
	}
}

const ::MumbleServer::ServerAuthenticatorPrx MumbleServerIce::getServerAuthenticator(const ::Server *server) const {
	return qmServerAuthenticator[server->iServerNum];
}

void MumbleServerIce::removeServerAuthenticator(const ::Server *server) {
	if (qmServerAuthenticator.remove(server->iServerNum)) {
		server->log(QString("Removed Ice Authenticator %1")
						.arg(QString::fromStdString(communicator->proxyToString(getServerAuthenticator(server)))));
	}
}

void MumbleServerIce::setServerUpdatingAuthenticator(const ::Server *server,
													 const ::MumbleServer::ServerUpdatingAuthenticatorPrx &prx) {
	if (prx != qmServerUpdatingAuthenticator[server->iServerNum]) {
		server->log(QString("Set Ice UpdatingAuthenticator to %1")
						.arg(QString::fromStdString(communicator->proxyToString(prx))));
		qmServerUpdatingAuthenticator[server->iServerNum] = prx;
	}
}

const ::MumbleServer::ServerUpdatingAuthenticatorPrx
	MumbleServerIce::getServerUpdatingAuthenticator(const ::Server *server) const {
	return qmServerUpdatingAuthenticator[server->iServerNum];
}

void MumbleServerIce::removeServerUpdatingAuthenticator(const ::Server *server) {
	if (qmServerUpdatingAuthenticator.contains(server->iServerNum)) {
		server->log(
			QString("Removed Ice UpdatingAuthenticator %1")
				.arg(QString::fromStdString(communicator->proxyToString(getServerUpdatingAuthenticator(server)))));
		qmServerUpdatingAuthenticator.remove(server->iServerNum);
	}
}

static ServerPrx idToProxy(int id, const Ice::ObjectAdapterPtr &adapter) {
	Ice::Identity ident;
	ident.category = "s";
	ident.name     = iceString(QString::number(id));

	return ServerPrx::uncheckedCast(adapter->createProxy(ident));
}

void MumbleServerIce::started(::Server *s) {
	s->connectListener(mi);
	connect(s, SIGNAL(contextAction(const User *, const QString &, unsigned int, int)), this,
			SLOT(contextAction(const User *, const QString &, unsigned int, int)));

	const QList<::MumbleServer::MetaCallbackPrx > &qlList = qlMetaCallbacks;

	if (qlList.isEmpty())
		return;

	foreach (const ::MumbleServer::MetaCallbackPrx &prx, qlList) {
		try {
			prx->started(idToProxy(s->iServerNum, adapter));
		} catch (...) {
			badMetaProxy(prx);
		}
	}
}

void MumbleServerIce::stopped(::Server *s) {
	removeServerCallbacks(s);
	removeServerAuthenticator(s);
	removeServerUpdatingAuthenticator(s);

	const QList<::MumbleServer::MetaCallbackPrx > &qmList = qlMetaCallbacks;

	if (qmList.isEmpty())
		return;

	foreach (const ::MumbleServer::MetaCallbackPrx &prx, qmList) {
		try {
			prx->stopped(idToProxy(s->iServerNum, adapter));
		} catch (...) {
			badMetaProxy(prx);
		}
	}
}

void MumbleServerIce::userConnected(const ::User *p) {
	::Server *s = qobject_cast<::Server * >(sender());

	const QList<::MumbleServer::ServerCallbackPrx > &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::MumbleServer::User mp;
	userToUser(p, mp);

	foreach (const ::MumbleServer::ServerCallbackPrx &prx, qmList) {
		try {
			prx->userConnected(mp);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MumbleServerIce::userDisconnected(const ::User *p) {
	::Server *s = qobject_cast<::Server * >(sender());

	qmServerContextCallbacks[s->iServerNum].remove(static_cast< int >(p->uiSession));

	const QList<::MumbleServer::ServerCallbackPrx > &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::MumbleServer::User mp;
	userToUser(p, mp);

	foreach (const ::MumbleServer::ServerCallbackPrx &prx, qmList) {
		try {
			prx->userDisconnected(mp);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MumbleServerIce::userStateChanged(const ::User *p) {
	::Server *s = qobject_cast<::Server * >(sender());

	const QList<::MumbleServer::ServerCallbackPrx > &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::MumbleServer::User mp;
	userToUser(p, mp);

	foreach (const ::MumbleServer::ServerCallbackPrx &prx, qmList) {
		try {
			prx->userStateChanged(mp);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MumbleServerIce::userTextMessage(const ::User *p, const ::TextMessage &message) {
	::Server *s = qobject_cast<::Server * >(sender());

	const QList<::MumbleServer::ServerCallbackPrx > &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::MumbleServer::User mp;
	userToUser(p, mp);

	::MumbleServer::TextMessage textMessage;
	textmessageToTextmessage(message, textMessage);

	foreach (const ::MumbleServer::ServerCallbackPrx &prx, qmList) {
		try {
			prx->userTextMessage(mp, textMessage);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MumbleServerIce::channelCreated(const ::Channel *c) {
	::Server *s = qobject_cast<::Server * >(sender());

	const QList<::MumbleServer::ServerCallbackPrx > &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::MumbleServer::Channel mc;
	channelToChannel(c, mc);

	foreach (const ::MumbleServer::ServerCallbackPrx &prx, qmList) {
		try {
			prx->channelCreated(mc);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MumbleServerIce::channelRemoved(const ::Channel *c) {
	::Server *s = qobject_cast<::Server * >(sender());

	const QList<::MumbleServer::ServerCallbackPrx > &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::MumbleServer::Channel mc;
	channelToChannel(c, mc);

	foreach (const ::MumbleServer::ServerCallbackPrx &prx, qmList) {
		try {
			prx->channelRemoved(mc);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MumbleServerIce::channelStateChanged(const ::Channel *c) {
	::Server *s = qobject_cast<::Server * >(sender());

	const QList<::MumbleServer::ServerCallbackPrx > &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::MumbleServer::Channel mc;
	channelToChannel(c, mc);

	foreach (const ::MumbleServer::ServerCallbackPrx &prx, qmList) {
		try {
			prx->channelStateChanged(mc);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MumbleServerIce::contextAction(const ::User *pSrc, const QString &action, unsigned int session, int iChannel) {
	::Server *s = qobject_cast<::Server * >(sender());

	QMap< int, QMap< int, QMap< QString, ::MumbleServer::ServerContextCallbackPrx > > > &qmAll =
		qmServerContextCallbacks;
	if (!qmAll.contains(s->iServerNum))
		return;

	QMap< int, QMap< QString, ::MumbleServer::ServerContextCallbackPrx > > &qmServer = qmAll[s->iServerNum];
	if (!qmServer.contains(static_cast< int >(pSrc->uiSession)))
		return;

	QMap< QString, ::MumbleServer::ServerContextCallbackPrx > &qmUser = qmServer[static_cast< int >(pSrc->uiSession)];
	if (!qmUser.contains(action))
		return;

	const ::MumbleServer::ServerContextCallbackPrx &prx = qmUser[action];

	::MumbleServer::User mp;
	userToUser(pSrc, mp);

	try {
		prx->contextAction(iceString(action), mp, static_cast< int >(session), iChannel);
	} catch (...) {
		s->log(QString("Ice ServerContextCallback %1 for session %2, action %3 failed")
				   .arg(QString::fromStdString(communicator->proxyToString(prx)))
				   .arg(pSrc->uiSession)
				   .arg(action));
		removeServerContextCallback(s, static_cast< int >(pSrc->uiSession), action);

		// Remove clientside entry
		MumbleProto::ContextActionModify mpcam;
		mpcam.set_action(iceString(action));
		mpcam.set_operation(MumbleProto::ContextActionModify_Operation_Remove);
		ServerUser *su = s->qhUsers.value(session);
		if (su)
			s->sendMessage(su, mpcam);
	}
}

void MumbleServerIce::idToNameSlot(QString &name, int id) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerAuthenticatorPrx prx = getServerAuthenticator(server);
	try {
		name = u8(prx->idToName(id));
	} catch (...) {
		badAuthenticator(server);
	}
}
void MumbleServerIce::idToTextureSlot(QByteArray &qba, int id) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerAuthenticatorPrx prx = getServerAuthenticator(server);
	try {
		const ::MumbleServer::Texture &tex = prx->idToTexture(id);

		qba.resize(static_cast< int >(tex.size()));
		char *ptr = qba.data();
		for (unsigned int i = 0; i < tex.size(); ++i)
			ptr[i] = static_cast< char >(tex[i]);
	} catch (...) {
		badAuthenticator(server);
	}
}

void MumbleServerIce::nameToIdSlot(int &id, const QString &name) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerAuthenticatorPrx prx = getServerAuthenticator(server);
	try {
		id = prx->nameToId(iceString(name));
	} catch (...) {
		badAuthenticator(server);
	}
}

void MumbleServerIce::authenticateSlot(int &res, QString &uname, int sessionId,
									   const QList< QSslCertificate > &certlist, const QString &certhash,
									   bool certstrong, const QString &pw) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerAuthenticatorPrx prx = getServerAuthenticator(server);
	::std::string newname;
	::MumbleServer::GroupNameList groups;
	::MumbleServer::CertificateList certs;

	certs.resize(static_cast< std::size_t >(certlist.size()));
	for (int i = 0; i < certlist.size(); ++i) {
		::MumbleServer::CertificateDer der;
		QByteArray qba = certlist.at(i).toDer();
		der.resize(static_cast< std::size_t >(qba.size()));
		const char *ptr = qba.constData();
		for (int j = 0; j < qba.size(); ++j)
			der[static_cast< std::size_t >(j)] = static_cast< unsigned char >(ptr[j]);
		certs[static_cast< std::size_t >(i)] = der;
	}

	try {
		res =
			prx->authenticate(iceString(uname), iceString(pw), certs, iceString(certhash), certstrong, newname, groups);
	} catch (...) {
		badAuthenticator(server);
	}
	if (res >= 0) {
		if (newname.length() > 0)
			uname = u8(newname);
		QStringList qsl;
		foreach (const ::std::string &str, groups) { qsl << u8(str); }
		if (!qsl.isEmpty())
			server->setTempGroups(res, sessionId, nullptr, qsl);
	}
}

void MumbleServerIce::registerUserSlot(int &res, const QMap< int, QString > &info) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (!prx)
		return;

	::MumbleServer::UserInfoMap im;

	infoToInfo(info, im);
	try {
		res = prx->registerUser(im);
	} catch (...) {
		badAuthenticator(server);
	}
}

void MumbleServerIce::unregisterUserSlot(int &res, int id) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (!prx)
		return;
	try {
		res = prx->unregisterUser(id);
	} catch (...) {
		badAuthenticator(server);
	}
}

void MumbleServerIce::getRegistrationSlot(int &res, int id, QMap< int, QString > &info) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (!prx)
		return;

	::MumbleServer::UserInfoMap im;
	try {
		if (prx->getInfo(id, im)) {
			res = 1;
			infoToInfo(im, info);
		}
	} catch (...) {
		badAuthenticator(server);
		return;
	}
}

void MumbleServerIce::getRegisteredUsersSlot(const QString &filter, QMap< int, QString > &m) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (!prx)
		return;

	::MumbleServer::NameMap lst;

	try {
		lst = prx->getRegisteredUsers(iceString(filter));
	} catch (...) {
		badAuthenticator(server);
		return;
	}
	::MumbleServer::NameMap::const_iterator i;
	for (i = lst.begin(); i != lst.end(); ++i)
		m.insert((*i).first, u8((*i).second));
}

void MumbleServerIce::setInfoSlot(int &res, int id, const QMap< int, QString > &info) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (!prx)
		return;

	MumbleServer::UserInfoMap im;
	infoToInfo(info, im);

	try {
		res = prx->setInfo(id, im);
	} catch (...) {
		badAuthenticator(server);
	}
}

void MumbleServerIce::setTextureSlot(int &res, int id, const QByteArray &texture) {
	::Server *server = qobject_cast<::Server * >(sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (!prx)
		return;

	::MumbleServer::Texture tex;
	tex.resize(static_cast< std::size_t >(texture.size()));
	const char *ptr = texture.constData();
	for (unsigned int i = 0; i < static_cast< unsigned int >(texture.size()); ++i)
		tex[i] = static_cast< unsigned char >(ptr[i]);

	try {
		res = prx->setTexture(id, tex);
	} catch (...) {
		badAuthenticator(server);
	}
}

Ice::ObjectPtr ServerLocator::locate(const Ice::Current &, Ice::LocalObjectPtr &) {
	return iopServer;
}

#define FIND_SERVER ::Server *server = meta->qhServers.value(server_id);

#define NEED_SERVER_EXISTS                                                     \
	FIND_SERVER                                                                \
	if (!server && !ServerDB::serverExists(server_id)) {                       \
		cb->ice_exception(::Ice::ObjectNotExistException(__FILE__, __LINE__)); \
		return;                                                                \
	}

#define NEED_SERVER                                 \
	NEED_SERVER_EXISTS                              \
	if (!server) {                                  \
		cb->ice_exception(ServerBootedException()); \
		return;                                     \
	}

#define NEED_PLAYER                                                                 \
	ServerUser *user = server->qhUsers.value(static_cast< unsigned int >(session)); \
	if (!user) {                                                                    \
		cb->ice_exception(::MumbleServer::InvalidSessionException());               \
		return;                                                                     \
	}

#define NEED_CHANNEL_VAR(x, y)                                        \
	x = server->qhChannels.value(static_cast< unsigned int >(y));     \
	if (!x) {                                                         \
		cb->ice_exception(::MumbleServer::InvalidChannelException()); \
		return;                                                       \
	}

#define NEED_CHANNEL    \
	::Channel *channel; \
	NEED_CHANNEL_VAR(channel, channelid);

void ServerI::ice_ping(const Ice::Current &current) const {
	// This is executed in the ice thread.
	int server_id = u8(current.id.name).toInt();
	if (!ServerDB::serverExists(server_id))
		throw ::Ice::ObjectNotExistException(__FILE__, __LINE__);
}

#define ACCESS_Server_isRunning_READ
static void impl_Server_isRunning(const ::MumbleServer::AMD_Server_isRunningPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	cb->ice_response(server != nullptr);
}

static void impl_Server_start(const ::MumbleServer::AMD_Server_startPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	if (server)
		cb->ice_exception(ServerBootedException());
	else if (!meta->boot(server_id))
		cb->ice_exception(ServerFailureException());
	else
		cb->ice_response();
}

static void impl_Server_stop(const ::MumbleServer::AMD_Server_stopPtr cb, int server_id) {
	NEED_SERVER;
	meta->kill(server_id);
	cb->ice_response();
}

static void impl_Server_delete(const ::MumbleServer::AMD_Server_deletePtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	if (server) {
		cb->ice_exception(ServerBootedException());
		return;
	}
	ServerDB::deleteServer(server_id);
	cb->ice_response();
}

static void impl_Server_addCallback(const MumbleServer::AMD_Server_addCallbackPtr cb, int server_id,
									const MumbleServer::ServerCallbackPrx &cbptr) {
	NEED_SERVER;

	try {
		const MumbleServer::ServerCallbackPrx &oneway =
			MumbleServer::ServerCallbackPrx::checkedCast(cbptr->ice_oneway()->ice_connectionCached(false));
		mi->addServerCallback(server, oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

static void impl_Server_removeCallback(const MumbleServer::AMD_Server_removeCallbackPtr cb, int server_id,
									   const MumbleServer::ServerCallbackPrx &cbptr) {
	NEED_SERVER;

	try {
		const MumbleServer::ServerCallbackPrx &oneway =
			MumbleServer::ServerCallbackPrx::uncheckedCast(cbptr->ice_oneway()->ice_connectionCached(false));
		mi->removeServerCallback(server, oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

static void impl_Server_setAuthenticator(const ::MumbleServer::AMD_Server_setAuthenticatorPtr &cb, int server_id,
										 const ::MumbleServer::ServerAuthenticatorPrx &aptr) {
	NEED_SERVER;

	if (mi->getServerAuthenticator(server))
		server->disconnectAuthenticator(mi);

	::MumbleServer::ServerAuthenticatorPrx prx;

	try {
		prx = ::MumbleServer::ServerAuthenticatorPrx::checkedCast(aptr->ice_connectionCached(false)->ice_timeout(5000));
		const ::MumbleServer::ServerUpdatingAuthenticatorPrx uprx =
			::MumbleServer::ServerUpdatingAuthenticatorPrx::checkedCast(prx);

		mi->setServerAuthenticator(server, prx);
		if (uprx)
			mi->setServerUpdatingAuthenticator(server, uprx);
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
		return;
	}

	if (prx)
		server->connectAuthenticator(mi);

	cb->ice_response();
}

#define ACCESS_Server_id_READ
static void impl_Server_id(const ::MumbleServer::AMD_Server_idPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	cb->ice_response(server_id);
}

#define ACCESS_Server_getConf_READ
static void impl_Server_getConf(const ::MumbleServer::AMD_Server_getConfPtr cb, int server_id,
								const ::std::string &key) {
	NEED_SERVER_EXISTS;
	if (key == "key" || key == "passphrase")
		cb->ice_exception(WriteOnlyException());
	else
		cb->ice_response(iceString(ServerDB::getConf(server_id, u8(key)).toString()));
}

#define ACCESS_Server_getAllConf_READ
static void impl_Server_getAllConf(const ::MumbleServer::AMD_Server_getAllConfPtr cb, int server_id) {
	NEED_SERVER_EXISTS;

	::MumbleServer::ConfigMap cm;

	QMap< QString, QString > values = ServerDB::getAllConf(server_id);
	QMap< QString, QString >::const_iterator i;
	for (i = values.constBegin(); i != values.constEnd(); ++i) {
		if (i.key() == "key" || i.key() == "passphrase")
			continue;
		cm[iceString(i.key())] = iceString(i.value());
	}
	cb->ice_response(cm);
}

static void impl_Server_setConf(const ::MumbleServer::AMD_Server_setConfPtr cb, int server_id, const ::std::string &key,
								const ::std::string &value) {
	NEED_SERVER_EXISTS;
	QString k = u8(key);
	QString v = u8(value);
	ServerDB::setConf(server_id, k, v);
	if (server) {
		QWriteLocker wl(&server->qrwlVoiceThread);
		server->setLiveConf(k, v);
	}
	cb->ice_response();
}

static void impl_Server_setSuperuserPassword(const ::MumbleServer::AMD_Server_setSuperuserPasswordPtr cb, int server_id,
											 const ::std::string &pw) {
	NEED_SERVER_EXISTS;
	ServerDB::setSUPW(server_id, u8(pw));
	cb->ice_response();
}

#define ACCESS_Server_getLog_READ
static void impl_Server_getLog(const ::MumbleServer::AMD_Server_getLogPtr cb, int server_id, ::Ice::Int min,
							   ::Ice::Int max) {
	NEED_SERVER_EXISTS;

	::MumbleServer::LogList ll;

	QList< ServerDB::LogRecord > dblog =
		ServerDB::getLog(server_id, static_cast< unsigned int >(min), static_cast< unsigned int >(max));
	foreach (const ServerDB::LogRecord &e, dblog) {
		::MumbleServer::LogEntry entry;
		logToLog(e, entry);
		ll.push_back(std::move(entry));
	}
	cb->ice_response(ll);
}

#define ACCESS_Server_getLogLen_READ
static void impl_Server_getLogLen(const ::MumbleServer::AMD_Server_getLogLenPtr cb, int server_id) {
	NEED_SERVER_EXISTS;

	int len = ServerDB::getLogLen(server_id);
	cb->ice_response(len);
}

#define ACCESS_Server_getUsers_READ
static void impl_Server_getUsers(const ::MumbleServer::AMD_Server_getUsersPtr cb, int server_id) {
	NEED_SERVER;
	::MumbleServer::UserMap pm;
	foreach (const ::User *p, server->qhUsers) {
		::MumbleServer::User mp;
		if (static_cast< const ServerUser * >(p)->sState == ::ServerUser::Authenticated) {
			userToUser(p, mp);
			pm[static_cast< int >(p->uiSession)] = mp;
		}
	}
	cb->ice_response(pm);
}

#define ACCESS_Server_getChannels_READ
static void impl_Server_getChannels(const ::MumbleServer::AMD_Server_getChannelsPtr cb, int server_id) {
	NEED_SERVER;
	::MumbleServer::ChannelMap cm;
	foreach (const ::Channel *c, server->qhChannels) {
		::MumbleServer::Channel mc;
		channelToChannel(c, mc);
		cm[static_cast< int >(c->iId)] = mc;
	}
	cb->ice_response(cm);
}

static bool userSort(const ::User *a, const ::User *b) {
	return ::User::lessThan(a, b);
}

static bool channelSort(const ::Channel *a, const ::Channel *b) {
	return ::Channel::lessThan(a, b);
}

TreePtr recurseTree(const ::Channel *c) {
	TreePtr t = new Tree();
	channelToChannel(c, t->c);
	QList<::User * > users = c->qlUsers;
	std::sort(users.begin(), users.end(), userSort);

	foreach (const ::User *p, users) {
		::MumbleServer::User mp;
		userToUser(p, mp);
		t->users.push_back(mp);
	}

	QList<::Channel * > channels = c->qlChannels;
	std::sort(channels.begin(), channels.end(), channelSort);

	foreach (const ::Channel *chn, channels) { t->children.push_back(recurseTree(chn)); }

	return t;
}

#define ACCESS_Server_getTree_READ
static void impl_Server_getTree(const ::MumbleServer::AMD_Server_getTreePtr cb, int server_id) {
	NEED_SERVER;
	cb->ice_response(recurseTree(server->qhChannels.value(0)));
}

#define ACCESS_Server_getCertificateList_READ
static void impl_Server_getCertificateList(const ::MumbleServer::AMD_Server_getCertificateListPtr cb, int server_id,
										   ::Ice::Int session) {
	NEED_SERVER;
	NEED_PLAYER;

	::MumbleServer::CertificateList certs;

	const QList< QSslCertificate > &certlist = user->peerCertificateChain();

	certs.resize(static_cast< std::size_t >(certlist.size()));
	for (int i = 0; i < certlist.size(); ++i) {
		::MumbleServer::CertificateDer der;
		QByteArray qba = certlist.at(i).toDer();
		der.resize(static_cast< std::size_t >(qba.size()));
		const char *ptr = qba.constData();
		for (unsigned int j = 0; j < static_cast< unsigned int >(qba.size()); ++j)
			der[j] = static_cast< unsigned char >(ptr[j]);
		certs[static_cast< std::size_t >(i)] = der;
	}
	cb->ice_response(certs);
}

#define ACCESS_Server_getBans_READ
static void impl_Server_getBans(const ::MumbleServer::AMD_Server_getBansPtr cb, int server_id) {
	NEED_SERVER;
	::MumbleServer::BanList bl;
	foreach (const ::Ban &ban, server->qlBans) {
		::MumbleServer::Ban mb;
		banToBan(ban, mb);
		bl.push_back(mb);
	}
	cb->ice_response(bl);
}

static void impl_Server_setBans(const ::MumbleServer::AMD_Server_setBansPtr cb, int server_id,
								const ::MumbleServer::BanList &bans) {
	NEED_SERVER;
	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		server->qlBans.clear();
		foreach (const ::MumbleServer::Ban &mb, bans) {
			::Ban ban;
			banToBan(mb, ban);
			server->qlBans << ban;
		}
	}

	server->saveBans();

	cb->ice_response();
}

static void impl_Server_kickUser(const ::MumbleServer::AMD_Server_kickUserPtr cb, int server_id, ::Ice::Int session,
								 const ::std::string &reason) {
	NEED_SERVER;
	NEED_PLAYER;

	MumbleProto::UserRemove mpur;
	mpur.set_session(static_cast< unsigned int >(session));
	mpur.set_reason(reason);
	server->sendAll(mpur);
	user->disconnectSocket();
	cb->ice_response();
}

static void impl_Server_sendMessage(const ::MumbleServer::AMD_Server_sendMessagePtr cb, int server_id,
									::Ice::Int session, const ::std::string &text) {
	NEED_SERVER;
	NEED_PLAYER;

	server->sendTextMessage(nullptr, user, false, u8(text));
	cb->ice_response();
}

#define ACCESS_Server_hasPermission_READ
static void impl_Server_hasPermission(const ::MumbleServer::AMD_Server_hasPermissionPtr cb, int server_id,
									  ::Ice::Int session, ::Ice::Int channelid, ::Ice::Int perm) {
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL;
	cb->ice_response(server->hasPermission(user, channel, static_cast< ChanACL::Perm >(perm)));
}

#define ACCESS_Server_effectivePermissions_READ
static void impl_Server_effectivePermissions(const ::MumbleServer::AMD_Server_effectivePermissionsPtr cb, int server_id,
											 ::Ice::Int session, ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL;
	cb->ice_response(static_cast< int >(server->effectivePermissions(user, channel)));
}

static void impl_Server_addContextCallback(const MumbleServer::AMD_Server_addContextCallbackPtr cb, int server_id,
										   ::Ice::Int session, const ::std::string &action, const ::std::string &text,
										   const ::MumbleServer::ServerContextCallbackPrx &cbptr, int ctx) {
	NEED_SERVER;
	NEED_PLAYER;

	const QMap< QString, ::MumbleServer::ServerContextCallbackPrx > &qmPrx =
		mi->getServerContextCallbacks(server)[session];

	if (!(ctx
		  & (MumbleProto::ContextActionModify_Context_Server | MumbleProto::ContextActionModify_Context_Channel
			 | MumbleProto::ContextActionModify_Context_User))) {
		cb->ice_exception(InvalidCallbackException());
		return;
	}

	try {
		const MumbleServer::ServerContextCallbackPrx &oneway = MumbleServer::ServerContextCallbackPrx::checkedCast(
			cbptr->ice_oneway()->ice_connectionCached(false)->ice_timeout(5000));
		if (qmPrx.contains(u8(action))) {
			// Since the server has no notion of the ctx part of the context action
			// make sure we remove them all clientside when overriding an old callback
			MumbleProto::ContextActionModify mpcam;
			mpcam.set_action(action);
			mpcam.set_operation(MumbleProto::ContextActionModify_Operation_Remove);
			server->sendMessage(user, mpcam);
		}
		mi->addServerContextCallback(server, session, u8(action), oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
		return;
	}

	MumbleProto::ContextActionModify mpcam;
	mpcam.set_action(action);
	mpcam.set_text(text);
	mpcam.set_context(static_cast< unsigned int >(ctx));
	mpcam.set_operation(MumbleProto::ContextActionModify_Operation_Add);
	server->sendMessage(user, mpcam);
}

static void impl_Server_removeContextCallback(const MumbleServer::AMD_Server_removeContextCallbackPtr cb, int server_id,
											  const MumbleServer::ServerContextCallbackPrx &cbptr) {
	NEED_SERVER;

	const QMap< int, QMap< QString, ::MumbleServer::ServerContextCallbackPrx > > &qmPrx =
		mi->getServerContextCallbacks(server);

	try {
		const MumbleServer::ServerContextCallbackPrx &oneway = MumbleServer::ServerContextCallbackPrx::uncheckedCast(
			cbptr->ice_oneway()->ice_connectionCached(false)->ice_timeout(5000));

		foreach (int session, qmPrx.keys()) {
			ServerUser *user = server->qhUsers.value(static_cast< unsigned int >(session));
			const QMap< QString, ::MumbleServer::ServerContextCallbackPrx > &qm = qmPrx[session];
			foreach (const QString &act, qm.keys(oneway)) {
				mi->removeServerContextCallback(server, session, act);

				// Ask clients to remove the clientside callbacks
				if (user) {
					MumbleProto::ContextActionModify mpcam;
					mpcam.set_action(iceString(act));
					mpcam.set_operation(MumbleProto::ContextActionModify_Operation_Remove);
					server->sendMessage(user, mpcam);
				}
			}
		}

		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

#define ACCESS_Server_getState_READ
static void impl_Server_getState(const ::MumbleServer::AMD_Server_getStatePtr cb, int server_id, ::Ice::Int session) {
	NEED_SERVER;
	NEED_PLAYER;

	::MumbleServer::User mp;
	userToUser(user, mp);
	cb->ice_response(mp);
}

static void impl_Server_setState(const ::MumbleServer::AMD_Server_setStatePtr cb, int server_id,
								 const ::MumbleServer::User &state) {
	int session = state.session;
	::Channel *channel;
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL_VAR(channel, state.channel);

	server->setUserState(user, channel, state.mute, state.deaf, state.suppress, state.prioritySpeaker, u8(state.name),
						 u8(state.comment));
	cb->ice_response();
}

static void impl_Server_sendMessageChannel(const ::MumbleServer::AMD_Server_sendMessageChannelPtr cb, int server_id,
										   ::Ice::Int channelid, bool tree, const ::std::string &text) {
	NEED_SERVER;
	NEED_CHANNEL;

	server->sendTextMessage(channel, nullptr, tree, u8(text));
	cb->ice_response();
}

#define ACCESS_Server_getChannelState_READ
static void impl_Server_getChannelState(const ::MumbleServer::AMD_Server_getChannelStatePtr cb, int server_id,
										::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	::MumbleServer::Channel mc;
	channelToChannel(channel, mc);
	cb->ice_response(mc);
}

static void impl_Server_setChannelState(const ::MumbleServer::AMD_Server_setChannelStatePtr cb, int server_id,
										const ::MumbleServer::Channel &state) {
	int channelid = state.id;
	NEED_SERVER;
	NEED_CHANNEL;
	::Channel *np = nullptr;
	if (channel->iId != 0) {
		NEED_CHANNEL_VAR(np, state.parent);
	}

	QString qsName = u8(state.name);

	QSet<::Channel * > newset;
	foreach (int linkid, state.links) {
		::Channel *cLink;
		NEED_CHANNEL_VAR(cLink, linkid);
		newset << cLink;
	}

	if (!server->canNest(np, channel)) {
		cb->ice_exception(::MumbleServer::NestingLimitException());
		return;
	}

	if (!server->setChannelState(channel, np, qsName, newset, u8(state.description), state.position))
		cb->ice_exception(::MumbleServer::InvalidChannelException());
	else
		cb->ice_response();
}

static void impl_Server_removeChannel(const ::MumbleServer::AMD_Server_removeChannelPtr cb, int server_id,
									  ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	if (!channel->cParent) {
		cb->ice_exception(::MumbleServer::InvalidChannelException());
	} else {
		server->removeChannel(channel);
		cb->ice_response();
	}
}

static void impl_Server_addChannel(const ::MumbleServer::AMD_Server_addChannelPtr cb, int server_id,
								   const ::std::string &name, ::Ice::Int parent) {
	NEED_SERVER;
	::Channel *p, *nc;
	NEED_CHANNEL_VAR(p, parent);

	if (!server->canNest(p)) {
		cb->ice_exception(::MumbleServer::NestingLimitException());
		return;
	}

	QString qsName = u8(name);

	nc = server->addChannel(p, qsName);
	server->updateChannel(nc);

	unsigned int newid = nc->iId;

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(newid);
	mpcs.set_parent(static_cast< unsigned int >(parent));
	mpcs.set_name(name);
	server->sendAll(mpcs);

	cb->ice_response(static_cast< int >(newid));
}

#define ACCESS_Server_getACL_READ
static void impl_Server_getACL(const ::MumbleServer::AMD_Server_getACLPtr cb, int server_id, ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	::MumbleServer::ACLList acls;
	::MumbleServer::GroupList groups;

	QStack<::Channel * > chans;
	::Channel *p;
	ChanACL *acl;
	p = channel;
	while (p) {
		chans.push(p);
		if ((p == channel) || (p->bInheritACL))
			p = p->cParent;
		else
			p = nullptr;
	}

	bool inherit = channel->bInheritACL;

	while (!chans.isEmpty()) {
		p = chans.pop();
		foreach (acl, p->qlACL) {
			if ((p == channel) || (acl->bApplySubs)) {
				::MumbleServer::ACL ma;
				ACLtoACL(acl, ma);
				if (p != channel)
					ma.inherited = true;
				acls.push_back(ma);
			}
		}
	}

	p                              = channel->cParent;
	const QSet< QString > allnames = ::Group::groupNames(channel);
	foreach (const QString &name, allnames) {
		::Group *g  = channel->qhGroups.value(name);
		::Group *pg = p ? ::Group::getGroup(p, name) : nullptr;
		if (!g && !pg)
			continue;
		::MumbleServer::Group mg;
		groupToGroup(g ? g : pg, mg);
		QSet< int > members;
		if (pg)
			members = pg->members();
		if (g) {
			QVector< int > addVec    = g->qsAdd.values().toVector();
			QVector< int > removeVec = g->qsRemove.values().toVector();

			mg.add       = std::vector< int >(addVec.begin(), addVec.end());
			mg.remove    = std::vector< int >(removeVec.begin(), removeVec.end());
			mg.inherited = false;
			members += g->qsAdd;
			members -= g->qsRemove;
		} else {
			mg.inherited = true;
		}

		QVector< int > memberVec = members.values().toVector();
		mg.members               = std::vector< int >(memberVec.begin(), memberVec.end());
		groups.push_back(mg);
	}
	cb->ice_response(acls, groups, inherit);
}

static void impl_Server_setACL(const ::MumbleServer::AMD_Server_setACLPtr cb, int server_id, ::Ice::Int channelid,
							   const ::MumbleServer::ACLList &acls, const ::MumbleServer::GroupList &groups,
							   bool inherit) {
	NEED_SERVER;
	NEED_CHANNEL;

	{
		QWriteLocker locker(&server->qrwlVoiceThread);

		::Group *g;
		ChanACL *acl;

		QHash< QString, QSet< int > > hOldTemp;
		foreach (g, channel->qhGroups) {
			hOldTemp.insert(g->qsName, g->qsTemporary);
			delete g;
		}
		foreach (acl, channel->qlACL)
			delete acl;

		channel->qhGroups.clear();
		channel->qlACL.clear();

		channel->bInheritACL = inherit;
		foreach (const ::MumbleServer::Group &gi, groups) {
			QString name    = u8(gi.name);
			g               = new ::Group(channel, name);
			g->bInherit     = gi.inherit;
			g->bInheritable = gi.inheritable;
			QVector< int > addVec(gi.add.begin(), gi.add.end());
			QVector< int > removeVec(gi.remove.begin(), gi.remove.end());

			g->qsAdd       = QSet< int >(addVec.begin(), addVec.end());
			g->qsRemove    = QSet< int >(removeVec.begin(), removeVec.end());
			g->qsTemporary = hOldTemp.value(name);
		}
		foreach (const ::MumbleServer::ACL &ai, acls) {
			acl             = new ChanACL(channel);
			acl->bApplyHere = ai.applyHere;
			acl->bApplySubs = ai.applySubs;
			acl->iUserId    = ai.userid;
			acl->qsGroup    = u8(ai.group);
			acl->pDeny      = static_cast< ChanACL::Permissions >(ai.deny) & ChanACL::All;
			acl->pAllow     = static_cast< ChanACL::Permissions >(ai.allow) & ChanACL::All;
		}
	}

	server->clearACLCache();
	server->updateChannel(channel);
	cb->ice_response();
}

#define ACCESS_Server_getUserNames_READ
static void impl_Server_getUserNames(const ::MumbleServer::AMD_Server_getUserNamesPtr cb, int server_id,
									 const ::MumbleServer::IdList &ids) {
	NEED_SERVER;
	::MumbleServer::NameMap nm;
	foreach (int userid, ids) { nm[userid] = iceString(server->getUserName(userid)); }
	cb->ice_response(nm);
}

#define ACCESS_Server_getUserIds_READ
static void impl_Server_getUserIds(const ::MumbleServer::AMD_Server_getUserIdsPtr cb, int server_id,
								   const ::MumbleServer::NameList &names) {
	NEED_SERVER;
	::MumbleServer::IdMap im;
	foreach (const string &n, names) {
		QString name = u8(n);
		im[n]        = server->getUserID(name);
	}
	cb->ice_response(im);
}

static void impl_Server_registerUser(const ::MumbleServer::AMD_Server_registerUserPtr cb, int server_id,
									 const ::MumbleServer::UserInfoMap &im) {
	NEED_SERVER;

	QMap< int, QString > info;
	infoToInfo(im, info);

	int userid = server->registerUser(info);

	if (userid < 0)
		cb->ice_exception(InvalidUserException());
	else
		cb->ice_response(userid);
}

static void impl_Server_unregisterUser(const ::MumbleServer::AMD_Server_unregisterUserPtr cb, int server_id,
									   ::Ice::Int userid) {
	NEED_SERVER;

	bool success = server->unregisterUser(userid);

	if (!success) {
		cb->ice_exception(InvalidUserException());
	} else {
		cb->ice_response();
	}
}

static void impl_Server_updateRegistration(const ::MumbleServer::AMD_Server_updateRegistrationPtr cb, int server_id,
										   int id, const ::MumbleServer::UserInfoMap &im) {
	NEED_SERVER;

	if (!server->isUserId(id)) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	QMap< int, QString > info;
	infoToInfo(im, info);

	if (!server->setInfo(id, info)) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	if (info.contains(ServerDB::User_Comment)) {
		foreach (ServerUser *u, server->qhUsers) {
			if (u->iId == id)
				server->setUserState(u, u->cChannel, u->bMute, u->bDeaf, u->bSuppress, u->bPrioritySpeaker, u->qsName,
									 info.value(ServerDB::User_Comment));
		}
	}

	cb->ice_response();
}

#define ACCESS_Server_getRegistration_READ
static void impl_Server_getRegistration(const ::MumbleServer::AMD_Server_getRegistrationPtr cb, int server_id,
										::Ice::Int userid) {
	NEED_SERVER;

	QMap< int, QString > info = server->getRegistration(userid);

	if (info.isEmpty()) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	MumbleServer::UserInfoMap im;
	infoToInfo(info, im);
	cb->ice_response(im);
}

#define ACCESS_Server_getRegisteredUsers_READ
static void impl_Server_getRegisteredUsers(const ::MumbleServer::AMD_Server_getRegisteredUsersPtr cb, int server_id,
										   const ::std::string &filter) {
	NEED_SERVER;
	MumbleServer::NameMap rpl;

	const QMap< int, QString > l = server->getRegisteredUsers(u8(filter));
	QMap< int, QString >::const_iterator i;
	for (i = l.constBegin(); i != l.constEnd(); ++i) {
		rpl[i.key()] = u8(i.value());
	}

	cb->ice_response(rpl);
}

#define ACCESS_Server_verifyPassword_READ
static void impl_Server_verifyPassword(const ::MumbleServer::AMD_Server_verifyPasswordPtr cb, int server_id,
									   const ::std::string &name, const ::std::string &pw) {
	NEED_SERVER;
	QString uname = u8(name);
	cb->ice_response(server->authenticate(uname, u8(pw)));
}

#define ACCESS_Server_getTexture_READ
static void impl_Server_getTexture(const ::MumbleServer::AMD_Server_getTexturePtr cb, int server_id,
								   ::Ice::Int userid) {
	NEED_SERVER;

	if (!server->isUserId(userid)) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	const QByteArray &qba = server->getUserTexture(userid);

	::MumbleServer::Texture tex;
	tex.resize(static_cast< std::size_t >(qba.size()));
	const char *ptr = qba.constData();
	for (unsigned int i = 0; i < static_cast< unsigned int >(qba.size()); ++i)
		tex[i] = static_cast< unsigned char >(ptr[i]);

	cb->ice_response(tex);
}

static void impl_Server_setTexture(const ::MumbleServer::AMD_Server_setTexturePtr cb, int server_id, ::Ice::Int userid,
								   const ::MumbleServer::Texture &tex) {
	NEED_SERVER;

	if (!server->isUserId(userid)) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	QByteArray qba(static_cast< int >(tex.size()), 0);
	char *ptr = qba.data();
	for (unsigned int i = 0; i < tex.size(); ++i)
		ptr[i] = static_cast< char >(tex[i]);

	if (!server->setTexture(userid, qba)) {
		cb->ice_exception(InvalidTextureException());
	} else {
		ServerUser *user = server->qhUsers.value(static_cast< unsigned int >(userid));
		if (user) {
			MumbleProto::UserState mpus;
			mpus.set_session(user->uiSession);
			mpus.set_texture(blob(user->qbaTexture));

			server->sendAll(mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
			if (!user->qbaTextureHash.isEmpty()) {
				mpus.clear_texture();
				mpus.set_texture_hash(blob(user->qbaTextureHash));
			}
			server->sendAll(mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
		}

		cb->ice_response();
	}
}

#define ACCESS_Server_getUptime_READ
static void impl_Server_getUptime(const ::MumbleServer::AMD_Server_getUptimePtr cb, int server_id) {
	NEED_SERVER;
	cb->ice_response(static_cast< int >(server->tUptime.elapsed() / 1000000LL));
}

static void impl_Server_updateCertificate(const ::MumbleServer::AMD_Server_updateCertificatePtr cb, int server_id,
										  const ::std::string &certificate, const ::std::string &privateKey,
										  const ::std::string &passphrase) {
	NEED_SERVER;

	QByteArray certPem(certificate.c_str());
	QByteArray privateKeyPem(privateKey.c_str());
	QByteArray passphraseBytes(passphrase.c_str());

	// Verify that we can load the certificate.
	QSslCertificate cert(certPem);
	if (cert.isNull()) {
		ERR_clear_error();
		cb->ice_exception(InvalidInputDataException());
		return;
	}

	// Verify that we can load the private key.
	QSslKey privKey = ::Server::privateKeyFromPEM(privateKeyPem, passphraseBytes);
	if (privKey.isNull()) {
		ERR_clear_error();
		cb->ice_exception(InvalidInputDataException());
		return;
	}

	// Ensure that the private key is usable with the given
	// certificate.
	if (!::Server::isKeyForCert(privKey, cert)) {
		ERR_clear_error();
		cb->ice_exception(InvalidInputDataException());
		return;
	}

	// All our sanity checks passed.
	// The certificate and private key are usable, so
	// update the server to use them.
	server->setConf("certificate", u8(certificate));
	server->setConf("key", u8(privateKey));
	server->setConf("passphrase", u8(passphrase));
	{
		QWriteLocker wl(&server->qrwlVoiceThread);
		server->initializeCert();
	}

	cb->ice_response();
}

static void impl_Server_startListening(const ::MumbleServer::AMD_Server_startListeningPtr cb, int server_id,
									   int session, int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;
	NEED_PLAYER;

	server->startListeningToChannel(user, channel);

	cb->ice_response();
}

static void impl_Server_stopListening(const ::MumbleServer::AMD_Server_stopListeningPtr cb, int server_id, int session,
									  int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;
	NEED_PLAYER;

	server->stopListeningToChannel(user, channel);

	cb->ice_response();
}

static void impl_Server_isListening(const ::MumbleServer::AMD_Server_isListeningPtr cb, int server_id, int session,
									int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;
	NEED_PLAYER;

	cb->ice_response(server->m_channelListenerManager.isListening(user->uiSession, channel->iId));
}

static void impl_Server_getListeningChannels(const ::MumbleServer::AMD_Server_getListeningChannelsPtr cb, int server_id,
											 int session) {
	NEED_SERVER;
	NEED_PLAYER;

	::MumbleServer::IntList channelIDs;
	for (unsigned int currentChannelID : server->m_channelListenerManager.getListenedChannelsForUser(user->uiSession)) {
		channelIDs.push_back(static_cast< int >(currentChannelID));
	}

	cb->ice_response(channelIDs);
}

static void impl_Server_getListeningUsers(const ::MumbleServer::AMD_Server_getListeningUsersPtr cb, int server_id,
										  int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	::MumbleServer::IntList userSessions;
	foreach (unsigned int currentSession, server->m_channelListenerManager.getListenersForChannel(channel->iId)) {
		userSessions.push_back(static_cast< int >(currentSession));
	}

	cb->ice_response(userSessions);
}

static void impl_Server_sendWelcomeMessage(const ::MumbleServer::AMD_Server_sendWelcomeMessagePtr cb, int server_id,
										   ::MumbleServer::IdList receiverUserIDs) {
	NEED_SERVER;

	for (int session : receiverUserIDs) {
		NEED_PLAYER;

		server->sendWelcomeMessageTo(user);
	}

	cb->ice_response();
}

static void impl_Server_getListenerVolumeAdjustment(const ::MumbleServer::AMD_Server_getListenerVolumeAdjustmentPtr cb,
													int server_id, int channelid, int session) {
	NEED_SERVER;
	NEED_CHANNEL;
	NEED_PLAYER;

	cb->ice_response(
		server->m_channelListenerManager.getListenerVolumeAdjustment(user->uiSession, channel->iId).factor);
}

static void impl_Server_setListenerVolumeAdjustment(const ::MumbleServer::AMD_Server_setListenerVolumeAdjustmentPtr cb,
													int server_id, int channelid, int session, float volumeAdjustment) {
	NEED_SERVER;
	NEED_CHANNEL;
	NEED_PLAYER;

	server->setListenerVolumeAdjustment(user, channel, VolumeAdjustment::fromFactor(volumeAdjustment));

	cb->ice_response();
}

static void impl_Server_addUserToGroup(const ::MumbleServer::AMD_Server_addUserToGroupPtr cb, int server_id,
									   ::Ice::Int channelid, ::Ice::Int session, const ::std::string &group) {
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL;

	QString qsgroup = u8(group);
	if (qsgroup.isEmpty()) {
		cb->ice_exception(InvalidChannelException());
		return;
	}

	{
		QWriteLocker wl(&server->qrwlVoiceThread);

		::Group *g = channel->qhGroups.value(qsgroup);
		if (!g)
			g = new ::Group(channel, qsgroup);

		g->qsTemporary.insert(-session);
	}

	server->clearACLCache(user);

	cb->ice_response();
}

static void impl_Server_removeUserFromGroup(const ::MumbleServer::AMD_Server_removeUserFromGroupPtr cb, int server_id,
											::Ice::Int channelid, ::Ice::Int session, const ::std::string &group) {
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL;

	QString qsgroup = u8(group);
	if (qsgroup.isEmpty()) {
		cb->ice_exception(InvalidChannelException());
		return;
	}

	{
		QWriteLocker qrwl(&server->qrwlVoiceThread);

		::Group *g = channel->qhGroups.value(qsgroup);
		if (!g)
			g = new ::Group(channel, qsgroup);

		g->qsTemporary.remove(-session);
	}

	server->clearACLCache(user);

	cb->ice_response();
}

static void impl_Server_redirectWhisperGroup(const ::MumbleServer::AMD_Server_redirectWhisperGroupPtr cb, int server_id,
											 ::Ice::Int session, const ::std::string &source,
											 const ::std::string &target) {
	NEED_SERVER;
	NEED_PLAYER;

	QString qssource = u8(source);
	QString qstarget = u8(target);

	{
		QWriteLocker wl(&server->qrwlVoiceThread);

		if (qstarget.isEmpty())
			user->qmWhisperRedirect.remove(qssource);
		else
			user->qmWhisperRedirect.insert(qssource, qstarget);
	}

	server->clearACLCache(user);

	cb->ice_response();
}

#define ACCESS_Meta_getSliceChecksums_ALL
static void impl_Meta_getSliceChecksums(const ::MumbleServer::AMD_Meta_getSliceChecksumsPtr cb,
										const Ice::ObjectAdapterPtr) {
	cb->ice_response(::Ice::sliceChecksums());
}

#define ACCESS_Meta_getServer_READ
static void impl_Meta_getServer(const ::MumbleServer::AMD_Meta_getServerPtr cb, const Ice::ObjectAdapterPtr adapter,
								::Ice::Int id) {
	QList< int > server_list = ServerDB::getAllServers();
	if (!server_list.contains(id))
		cb->ice_response(nullptr);
	else
		cb->ice_response(idToProxy(id, adapter));
}

static void impl_Meta_newServer(const ::MumbleServer::AMD_Meta_newServerPtr cb, const Ice::ObjectAdapterPtr adapter) {
	cb->ice_response(idToProxy(ServerDB::addServer(), adapter));
}

#define ACCESS_Meta_getAllServers_READ
static void impl_Meta_getAllServers(const ::MumbleServer::AMD_Meta_getAllServersPtr cb,
									const Ice::ObjectAdapterPtr adapter) {
	::MumbleServer::ServerList sl;

	foreach (int id, ServerDB::getAllServers())
		sl.push_back(idToProxy(id, adapter));
	cb->ice_response(sl);
}

#define ACCESS_Meta_getDefaultConf_READ
static void impl_Meta_getDefaultConf(const ::MumbleServer::AMD_Meta_getDefaultConfPtr cb, const Ice::ObjectAdapterPtr) {
	::MumbleServer::ConfigMap cm;
	QMap< QString, QString >::const_iterator i;
	for (i = ::Meta::mp->qmConfig.constBegin(); i != ::Meta::mp->qmConfig.constEnd(); ++i) {
		if (i.key() == "key" || i.key() == "passphrase")
			continue;
		cm[iceString(i.key())] = iceString(i.value());
	}
	cb->ice_response(cm);
}

#define ACCESS_Meta_getBootedServers_READ
static void impl_Meta_getBootedServers(const ::MumbleServer::AMD_Meta_getBootedServersPtr cb,
									   const Ice::ObjectAdapterPtr adapter) {
	::MumbleServer::ServerList sl;

	foreach (int id, meta->qhServers.keys())
		sl.push_back(idToProxy(id, adapter));
	cb->ice_response(sl);
}

#define ACCESS_Meta_getVersion_ALL
static void impl_Meta_getVersion(const ::MumbleServer::AMD_Meta_getVersionPtr cb, const Ice::ObjectAdapterPtr) {
	Version::component_t major, minor, patch;
	QString txt;
	::Meta::getVersion(major, minor, patch, txt);
	cb->ice_response(major, minor, patch, iceString(txt));
}

static void impl_Meta_addCallback(const MumbleServer::AMD_Meta_addCallbackPtr cb, const Ice::ObjectAdapterPtr,
								  const MumbleServer::MetaCallbackPrx &cbptr) {
	try {
		const MumbleServer::MetaCallbackPrx &oneway = MumbleServer::MetaCallbackPrx::checkedCast(
			cbptr->ice_oneway()->ice_connectionCached(false)->ice_timeout(5000));
		mi->addMetaCallback(oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

static void impl_Meta_removeCallback(const MumbleServer::AMD_Meta_removeCallbackPtr cb, const Ice::ObjectAdapterPtr,
									 const MumbleServer::MetaCallbackPrx &cbptr) {
	try {
		const MumbleServer::MetaCallbackPrx &oneway = MumbleServer::MetaCallbackPrx::uncheckedCast(
			cbptr->ice_oneway()->ice_connectionCached(false)->ice_timeout(5000));
		mi->removeMetaCallback(oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

#define ACCESS_Meta_getUptime_ALL
static void impl_Meta_getUptime(const ::MumbleServer::AMD_Meta_getUptimePtr cb, const Ice::ObjectAdapterPtr) {
	cb->ice_response(static_cast< int >(meta->tUptime.elapsed() / 1000000LL));
}

#include "MumbleServerIceWrapper.cpp"

#undef FIND_SERVER
#undef NEED_SERVER_EXISTS
#undef NEED_SERVER
#undef NEED_PLAYER
#undef NEED_CHANNEL_VAR
#undef NEED_CHANNEL
#undef ACCESS_Server_isRunning_READ
#undef ACCESS_Server_id_READ
#undef ACCESS_Server_getConf_READ
#undef ACCESS_Server_getAllConf_READ
#undef ACCESS_Server_getLog_READ
#undef ACCESS_Server_getLogLen_READ
#undef ACCESS_Server_getUsers_READ
#undef ACCESS_Server_getChannels_READ
#undef ACCESS_Server_getTree_READ
#undef ACCESS_Server_getCertificateList_READ
#undef ACCESS_Server_getBans_READ
#undef ACCESS_Server_hasPermission_READ
#undef ACCESS_Server_effectivePermissions_READ
#undef ACCESS_Server_getState_READ
#undef ACCESS_Server_getChannelState_READ
#undef ACCESS_Server_getACL_READ
#undef ACCESS_Server_getUserNames_READ
#undef ACCESS_Server_getUserIds_READ
#undef ACCESS_Server_getRegistration_READ
#undef ACCESS_Server_getRegisteredUsers_READ
#undef ACCESS_Server_verifyPassword_READ
#undef ACCESS_Server_getTexture_READ
#undef ACCESS_Server_getUptime_READ
#undef ACCESS_Meta_getSliceChecksums_ALL
#undef ACCESS_Meta_getServer_READ
#undef ACCESS_Meta_getAllServers_READ
#undef ACCESS_Meta_getDefaultConf_READ
#undef ACCESS_Meta_getBootedServers_READ
#undef ACCESS_Meta_getVersion_ALL
#undef ACCESS_Meta_getUptime_ALL
