// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MurmurIce.h"

#include "Channel.h"
#include "Group.h"
#include "Meta.h"
#include "MurmurI.h"
#include "Server.h"
#include "ServerUser.h"
#include "ServerDB.h"
#include "User.h"
#include "Ban.h"
#include "Utils.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QSettings>
#include <QtCore/QStack>

#include <boost/bind.hpp>

#include <openssl/err.h>

#include <Ice/Ice.h>
#include <Ice/SliceChecksums.h>
#include <IceUtil/IceUtil.h>

#include <limits>

using namespace std;
using namespace Murmur;

static MurmurIce *mi = NULL;
static Ice::ObjectPtr iopServer;
static Ice::PropertiesPtr ippProperties;

void IceParse(int &argc, char *argv[]) {
	ippProperties = Ice::createProperties(argc, argv);
}

void IceStart() {
	mi = new MurmurIce();
}

void IceStop() {
	delete mi;
	mi = NULL;
}

/// Remove all NUL bytes from |s|.
static std::string iceRemoveNul(std::string s) {
	std::vector<char> newstr;
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
	if (s.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
		return std::string();
	}

	QByteArray ba(s.data(), static_cast<int>(s.size()));
	QByteArray ba64 = ba.toBase64();

	return std::string(ba64.data(), static_cast<size_t>(ba.size()));
}

static void logToLog(const ServerDB::LogRecord &r, Murmur::LogEntry &le) {
	le.timestamp = r.first;
	le.txt = iceString(r.second);
}

static void userToUser(const ::User *p, Murmur::User &mp) {
	mp.session = p->uiSession;
	mp.userid = p->iId;
	mp.name = iceString(p->qsName);
	mp.mute = p->bMute;
	mp.deaf = p->bDeaf;
	mp.suppress = p->bSuppress;
	mp.recording = p->bRecording;
	mp.prioritySpeaker = p->bPrioritySpeaker;
	mp.selfMute = p->bSelfMute;
	mp.selfDeaf = p->bSelfDeaf;
	mp.channel = p->cChannel->iId;
	mp.comment = iceString(p->qsComment);

	const ServerUser *u=static_cast<const ServerUser *>(p);
	mp.onlinesecs = u->bwr.onlineSeconds();
	mp.bytespersec = u->bwr.bandwidth();
	mp.version = u->uiVersion;
	mp.release = iceString(u->qsRelease);
	mp.os = iceString(u->qsOS);
	mp.osversion = iceString(u->qsOSVersion);
	mp.identity = iceString(u->qsIdentity);
	mp.context = iceBase64(u->ssContext);
	mp.idlesecs = u->bwr.idleSeconds();
	mp.udpPing = u->dUDPPingAvg;
	mp.tcpPing = u->dTCPPingAvg;

	mp.tcponly = QAtomicIntLoad(u->aiUdpFlag) == 0;

	::Murmur::NetAddress addr(16, 0);
	const Q_IPV6ADDR &a = u->haAddress.qip6;
	for (int i=0;i<16;++i)
		addr[i] = a[i];

	mp.address = addr;
}

static void channelToChannel(const ::Channel *c, Murmur::Channel &mc) {
	mc.id = c->iId;
	mc.name = iceString(c->qsName);
	mc.parent = c->cParent ? c->cParent->iId : -1;
	mc.description = iceString(c->qsDesc);
	mc.position = c->iPosition;
	mc.links.clear();
	foreach(::Channel *chn, c->qsPermLinks)
		mc.links.push_back(chn->iId);
	mc.temporary = c->bTemporary;
}

static void ACLtoACL(const ::ChanACL *acl, Murmur::ACL &ma) {
	ma.applyHere = acl->bApplyHere;
	ma.applySubs = acl->bApplySubs;
	ma.inherited = false;
	ma.userid = acl->iUserId;
	ma.group = iceString(acl->qsGroup);
	ma.allow = acl->pAllow;
	ma.deny = acl->pDeny;
}

static void groupToGroup(const ::Group *g, Murmur::Group &mg) {
	mg.name = iceString(g->qsName);
	mg.inherit = g->bInherit;
	mg.inheritable = g->bInheritable;
	mg.add.clear();
	mg.remove.clear();
	mg.members.clear();
}

static void banToBan(const ::Ban &b, Murmur::Ban &mb) {
	::Murmur::NetAddress addr(16, 0);
	const Q_IPV6ADDR &a = b.haAddress.qip6;
	for (int i=0;i<16;++i)
		addr[i] = a[i];

	mb.address = addr;
	mb.bits = b.iMask;
	mb.name = iceString(b.qsUsername);
	mb.hash = iceString(b.qsHash);
	mb.reason = iceString(b.qsReason);
	mb.start = b.qdtStart.toLocalTime().toTime_t();
	mb.duration = b.iDuration;
}

static void banToBan(const ::Murmur::Ban &mb, ::Ban &b) {
	if (mb.address.size() != 16)
		for (int i=0;i<16;++i)
			b.haAddress.qip6[i] = 0;
	else
		for (int i=0;i<16;++i)
			b.haAddress.qip6[i] = mb.address[i];
	b.iMask = mb.bits;
	b.qsUsername = u8(mb.name);
	b.qsHash = u8(mb.hash);
	b.qsReason = u8(mb.reason);
	b.qdtStart = QDateTime::fromTime_t(static_cast<quint32>(mb.start)).toUTC();
	b.iDuration = mb.duration;
}

static void infoToInfo(const QMap<int, QString> &info, Murmur::UserInfoMap &im) {
	QMap<int, QString>::const_iterator i;
	for (i = info.constBegin(); i != info.constEnd(); ++i)
		im[static_cast<Murmur::UserInfo>(i.key())] = iceString(i.value());
}

static void infoToInfo(const Murmur::UserInfoMap &im, QMap<int, QString> &info) {
	Murmur::UserInfoMap::const_iterator i;
	for (i = im.begin(); i != im.end(); ++i)
		info.insert((*i).first, u8((*i).second));
}

static void textmessageToTextmessage(const ::TextMessage &tm, Murmur::TextMessage &tmdst) {
	tmdst.text = iceString(tm.qsText);

	foreach(unsigned int i, tm.qlSessions)
		tmdst.sessions.push_back(i);

	foreach(unsigned int i, tm.qlChannels)
		tmdst.channels.push_back(i);

	foreach(unsigned int i, tm.qlTrees)
		tmdst.trees.push_back(i);
}

class ServerLocator : public virtual Ice::ServantLocator {
	public:
		virtual Ice::ObjectPtr locate(const Ice::Current &, Ice::LocalObjectPtr &);
		virtual void finished(const Ice::Current &, const Ice::ObjectPtr &, const Ice::LocalObjectPtr &) {};
		virtual void deactivate(const std::string &) {};
};

MurmurIce::MurmurIce() {
	count = 0;

	if (meta->mp.qsIceEndpoint.isEmpty())
		return;

	Ice::PropertiesPtr ipp = Ice::createProperties();

	::Meta::mp.qsSettings->beginGroup("Ice");
	foreach(const QString &v, ::Meta::mp.qsSettings->childKeys()) {
		ipp->setProperty(iceString(v), iceString(::Meta::mp.qsSettings->value(v).toString()));
	}
	::Meta::mp.qsSettings->endGroup();

	Ice::PropertyDict props = ippProperties->getPropertiesForPrefix("");
	Ice::PropertyDict::iterator i;
	for (i=props.begin(); i != props.end(); ++i) {
		ipp->setProperty((*i).first, (*i).second);
	}
	ipp->setProperty("Ice.ImplicitContext", "Shared");

	Ice::InitializationData idd;
	idd.properties = ipp;

	try {
		communicator = Ice::initialize(idd);
		if (! meta->mp.qsIceSecretWrite.isEmpty()) {
			::Ice::ImplicitContextPtr impl = communicator->getImplicitContext();
			if (impl)
				impl->put("secret", iceString(meta->mp.qsIceSecretWrite));
		}
		adapter = communicator->createObjectAdapterWithEndpoints("Murmur", qPrintable(meta->mp.qsIceEndpoint));
		MetaPtr m = new MetaI;
#if ICE_INT_VERSION >= 30700
		MetaPrx mprx = MetaPrx::uncheckedCast(adapter->add(m, Ice::stringToIdentity("Meta")));
#else
		MetaPrx mprx = MetaPrx::uncheckedCast(adapter->add(m, communicator->stringToIdentity("Meta")));
#endif
		adapter->addServantLocator(new ServerLocator(), "s");

		iopServer = new ServerI;

		adapter->activate();
		foreach(const Ice::EndpointPtr ep, mprx->ice_getEndpoints()) {
			qWarning("MurmurIce: Endpoint \"%s\" running", qPrintable(u8(ep->toString())));
		}

		meta->connectListener(this);
	} catch (Ice::Exception &e) {
#if ICE_INT_VERSION >= 30700
		qCritical("MurmurIce: Initialization failed: %s", qPrintable(u8(e.ice_id())));
#else
		qCritical("MurmurIce: Initialization failed: %s", qPrintable(u8(e.ice_name())));
#endif
	}
}

MurmurIce::~MurmurIce() {
	if (communicator) {
		communicator->shutdown();
		communicator->waitForShutdown();
		communicator->destroy();
		communicator=NULL;
		qWarning("MurmurIce: Shutdown complete");
	}
	iopServer = NULL;
}

void MurmurIce::customEvent(QEvent *evt) {
	if (evt->type() == EXEC_QEVENT)
		static_cast<ExecEvent *>(evt)->execute();
}

void MurmurIce::badMetaProxy(const ::Murmur::MetaCallbackPrx &prx) {
	qCritical("Ice MetaCallback %s failed", qPrintable(QString::fromStdString(communicator->proxyToString(prx))));
	removeMetaCallback(prx);
}

void MurmurIce::badServerProxy(const ::Murmur::ServerCallbackPrx &prx, const ::Server* server) {
	server->log(QString("Ice ServerCallback %1 failed").arg(QString::fromStdString(communicator->proxyToString(prx))));
	removeServerCallback(server, prx);
}

void MurmurIce::badAuthenticator(::Server *server) {
	server->disconnectAuthenticator(this);
	const ::Murmur::ServerAuthenticatorPrx &prx = qmServerAuthenticator.value(server->iServerNum);
	server->log(QString("Ice Authenticator %1 failed").arg(QString::fromStdString(communicator->proxyToString(prx))));
	removeServerAuthenticator(server);
	removeServerUpdatingAuthenticator(server);
}

void MurmurIce::addMetaCallback(const ::Murmur::MetaCallbackPrx& prx) {
	if (!qlMetaCallbacks.contains(prx)) {
		qWarning("Added Ice MetaCallback %s", qPrintable(QString::fromStdString(communicator->proxyToString(prx))));
		qlMetaCallbacks.append(prx);
	}
}

void MurmurIce::removeMetaCallback(const ::Murmur::MetaCallbackPrx& prx) {
	if (qlMetaCallbacks.removeAll(prx)) {
		qWarning("Removed Ice MetaCallback %s", qPrintable(QString::fromStdString(communicator->proxyToString(prx))));
	}
}

void MurmurIce::addServerCallback(const ::Server* server, const ::Murmur::ServerCallbackPrx& prx) {
	QList< ::Murmur::ServerCallbackPrx >& cbList = qmServerCallbacks[server->iServerNum];

	if (!cbList.contains(prx)) {
		server->log(QString("Added Ice ServerCallback %1").arg(QString::fromStdString(communicator->proxyToString(prx))));
		cbList.append(prx);
	}
}

void MurmurIce::removeServerCallback(const ::Server* server, const ::Murmur::ServerCallbackPrx& prx) {
	if (qmServerCallbacks[server->iServerNum].removeAll(prx)) {
		server->log(QString("Removed Ice ServerCallback %1").arg(QString::fromStdString(communicator->proxyToString(prx))));
	}
}

void MurmurIce::removeServerCallbacks(const ::Server* server) {
	if (qmServerCallbacks.contains(server->iServerNum)) {
		server->log(QString("Removed all Ice ServerCallbacks"));
		qmServerCallbacks.remove(server->iServerNum);
	}
}

void MurmurIce::addServerContextCallback(const ::Server* server, int session_id, const QString& action, const ::Murmur::ServerContextCallbackPrx& prx) {
	QMap<QString, ::Murmur::ServerContextCallbackPrx>& callbacks = qmServerContextCallbacks[server->iServerNum][session_id];

	if (!callbacks.contains(action) || callbacks[action] != prx) {
		server->log(QString("Added Ice ServerContextCallback %1 for session %2, action %3").arg(QString::fromStdString(communicator->proxyToString(prx))).arg(session_id).arg(action));
		callbacks.insert(action, prx);
	}
}

const QMap< int, QMap<QString, ::Murmur::ServerContextCallbackPrx> > MurmurIce::getServerContextCallbacks(const ::Server* server) const {
	return qmServerContextCallbacks[server->iServerNum];
}

void MurmurIce::removeServerContextCallback(const ::Server* server, int session_id, const QString& action) {
	if (qmServerContextCallbacks[server->iServerNum][session_id].remove(action)) {
		server->log(QString("Removed Ice ServerContextCallback for session %1, action %2").arg(session_id).arg(action));
	}
}

void MurmurIce::setServerAuthenticator(const ::Server* server, const ::Murmur::ServerAuthenticatorPrx& prx) {
	if (prx != qmServerAuthenticator[server->iServerNum]) {
		server->log(QString("Set Ice Authenticator to %1").arg(QString::fromStdString(communicator->proxyToString(prx))));
		qmServerAuthenticator[server->iServerNum] = prx;
	}
}

const ::Murmur::ServerAuthenticatorPrx MurmurIce::getServerAuthenticator(const ::Server* server) const {
	return qmServerAuthenticator[server->iServerNum];
}

void MurmurIce::removeServerAuthenticator(const ::Server* server) {
	if (qmServerAuthenticator.remove(server->iServerNum)) {
		server->log(QString("Removed Ice Authenticator %1").arg(QString::fromStdString(communicator->proxyToString(getServerAuthenticator(server)))));
	}
}

void MurmurIce::setServerUpdatingAuthenticator(const ::Server* server, const ::Murmur::ServerUpdatingAuthenticatorPrx& prx) {
	if (prx != qmServerUpdatingAuthenticator[server->iServerNum]) {
		server->log(QString("Set Ice UpdatingAuthenticator to %1").arg(QString::fromStdString(communicator->proxyToString(prx))));
		qmServerUpdatingAuthenticator[server->iServerNum] = prx;
	}
}

const ::Murmur::ServerUpdatingAuthenticatorPrx MurmurIce::getServerUpdatingAuthenticator(const ::Server* server) const {
	return qmServerUpdatingAuthenticator[server->iServerNum];
}

void MurmurIce::removeServerUpdatingAuthenticator(const ::Server* server) {
	if (qmServerUpdatingAuthenticator.contains(server->iServerNum)) {
		server->log(QString("Removed Ice UpdatingAuthenticator %1").arg(QString::fromStdString(communicator->proxyToString(getServerUpdatingAuthenticator(server)))));
		qmServerUpdatingAuthenticator.remove(server->iServerNum);
	}
}

static ServerPrx idToProxy(int id, const Ice::ObjectAdapterPtr &adapter) {
	Ice::Identity ident;
	ident.category = "s";
	ident.name = iceString(QString::number(id));

	return ServerPrx::uncheckedCast(adapter->createProxy(ident));
}

void MurmurIce::started(::Server *s) {
	s->connectListener(mi);
	connect(s, SIGNAL(contextAction(const User *, const QString &, unsigned int, int)), this, SLOT(contextAction(const User *, const QString &, unsigned int, int)));

	const QList< ::Murmur::MetaCallbackPrx> &qlList = qlMetaCallbacks;

	if (qlList.isEmpty())
		return;

	foreach(const ::Murmur::MetaCallbackPrx &prx, qlList) {
		try {
			prx->started(idToProxy(s->iServerNum, adapter));
		} catch (...) {
			badMetaProxy(prx);
		}
	}
}

void MurmurIce::stopped(::Server *s) {
	removeServerCallbacks(s);
	removeServerAuthenticator(s);
	removeServerUpdatingAuthenticator(s);

	const QList< ::Murmur::MetaCallbackPrx> &qmList = qlMetaCallbacks;

	if (qmList.isEmpty())
		return;

	foreach(const ::Murmur::MetaCallbackPrx &prx, qmList) {
		try {
			prx->stopped(idToProxy(s->iServerNum, adapter));
		} catch (...) {
			badMetaProxy(prx);
		}
	}
}

void MurmurIce::userConnected(const ::User *p) {
	::Server *s = qobject_cast< ::Server *> (sender());

	const QList< ::Murmur::ServerCallbackPrx> &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::Murmur::User mp;
	userToUser(p, mp);

	foreach(const ::Murmur::ServerCallbackPrx &prx, qmList) {
		try {
			prx->userConnected(mp);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MurmurIce::userDisconnected(const ::User *p) {
	::Server *s = qobject_cast< ::Server *> (sender());

	qmServerContextCallbacks[s->iServerNum].remove(p->uiSession);

	const QList< ::Murmur::ServerCallbackPrx> &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::Murmur::User mp;
	userToUser(p, mp);

	foreach(const ::Murmur::ServerCallbackPrx &prx, qmList) {
		try {
			prx->userDisconnected(mp);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MurmurIce::userStateChanged(const ::User *p) {
	::Server *s = qobject_cast< ::Server *> (sender());

	const QList< ::Murmur::ServerCallbackPrx> &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::Murmur::User mp;
	userToUser(p, mp);

	foreach(const ::Murmur::ServerCallbackPrx &prx, qmList) {
		try {
			prx->userStateChanged(mp);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MurmurIce::userTextMessage(const ::User *p, const ::TextMessage &message) {
	::Server *s = qobject_cast< ::Server *> (sender());

	const QList< ::Murmur::ServerCallbackPrx> &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::Murmur::User mp;
	userToUser(p, mp);

	::Murmur::TextMessage textMessage;
	textmessageToTextmessage(message, textMessage);

	foreach(const ::Murmur::ServerCallbackPrx &prx, qmList) {
		try {
			prx->userTextMessage(mp, textMessage);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MurmurIce::channelCreated(const ::Channel *c) {
	::Server *s = qobject_cast< ::Server *> (sender());

	const QList< ::Murmur::ServerCallbackPrx> &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::Murmur::Channel mc;
	channelToChannel(c, mc);

	foreach(const ::Murmur::ServerCallbackPrx &prx, qmList) {
		try {
			prx->channelCreated(mc);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MurmurIce::channelRemoved(const ::Channel *c) {
	::Server *s = qobject_cast< ::Server *> (sender());

	const QList< ::Murmur::ServerCallbackPrx> &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::Murmur::Channel mc;
	channelToChannel(c, mc);

	foreach(const ::Murmur::ServerCallbackPrx &prx, qmList) {
		try {
			prx->channelRemoved(mc);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MurmurIce::channelStateChanged(const ::Channel *c) {
	::Server *s = qobject_cast< ::Server *> (sender());

	const QList< ::Murmur::ServerCallbackPrx> &qmList = qmServerCallbacks[s->iServerNum];

	if (qmList.isEmpty())
		return;

	::Murmur::Channel mc;
	channelToChannel(c, mc);

	foreach(const ::Murmur::ServerCallbackPrx &prx, qmList) {
		try {
			prx->channelStateChanged(mc);
		} catch (...) {
			badServerProxy(prx, s);
		}
	}
}

void MurmurIce::contextAction(const ::User *pSrc, const QString &action, unsigned int session, int iChannel) {
	::Server *s = qobject_cast< ::Server *> (sender());

	QMap<int, QMap<int, QMap<QString, ::Murmur::ServerContextCallbackPrx> > > &qmAll = qmServerContextCallbacks;
	if (! qmAll.contains(s->iServerNum))
		return;

	QMap<int, QMap<QString, ::Murmur::ServerContextCallbackPrx> > &qmServer = qmAll[s->iServerNum];
	if (! qmServer.contains(pSrc->uiSession))
		return;

	QMap<QString, ::Murmur::ServerContextCallbackPrx> &qmUser = qmServer[pSrc->uiSession];
	if (! qmUser.contains(action))
		return;

	const ::Murmur::ServerContextCallbackPrx &prx = qmUser[action];

	::Murmur::User mp;
	userToUser(pSrc, mp);

	try {
		prx->contextAction(iceString(action), mp, session, iChannel);
	} catch (...) {
		s->log(QString("Ice ServerContextCallback %1 for session %2, action %3 failed").arg(QString::fromStdString(communicator->proxyToString(prx))).arg(pSrc->uiSession).arg(action));
		removeServerContextCallback(s, pSrc->uiSession, action);

		// Remove clientside entry
		MumbleProto::ContextActionModify mpcam;
		mpcam.set_action(iceString(action));
		mpcam.set_operation(MumbleProto::ContextActionModify_Operation_Remove);
		ServerUser *su = s->qhUsers.value(session);
		if (su)
			s->sendMessage(su, mpcam);
	}
}

void MurmurIce::idToNameSlot(QString &name, int id) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerAuthenticatorPrx prx = getServerAuthenticator(server);
	try {
		name = u8(prx->idToName(id));
	} catch (...) {
		badAuthenticator(server);
	}
}
void MurmurIce::idToTextureSlot(QByteArray &qba, int id) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerAuthenticatorPrx prx = getServerAuthenticator(server);
	try {
		const ::Murmur::Texture &tex = prx->idToTexture(id);

		qba.resize(static_cast<int>(tex.size()));
		char *ptr = qba.data();
		for (unsigned int i=0;i<tex.size();++i)
			ptr[i] = tex[i];
	} catch (...) {
		badAuthenticator(server);
	}
}

void MurmurIce::nameToIdSlot(int &id, const QString &name) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerAuthenticatorPrx prx = getServerAuthenticator(server);
	try {
		id = prx->nameToId(iceString(name));
	} catch (...) {
		badAuthenticator(server);
	}
}

void MurmurIce::authenticateSlot(int &res, QString &uname, int sessionId, const QList<QSslCertificate> &certlist, const QString &certhash, bool certstrong, const QString &pw) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerAuthenticatorPrx prx = getServerAuthenticator(server);
	::std::string newname;
	::Murmur::GroupNameList groups;
	::Murmur::CertificateList certs;

	certs.resize(certlist.size());
	for (int i=0;i<certlist.size();++i) {
		::Murmur::CertificateDer der;
		QByteArray qba = certlist.at(i).toDer();
		der.resize(qba.size());
		const char *ptr = qba.constData();
		for (int j=0;j<qba.size();++j)
			der[j] = ptr[j];
		certs[i] = der;
	}

	try {
		res = prx->authenticate(iceString(uname), iceString(pw), certs, iceString(certhash), certstrong, newname, groups);
	} catch (...) {
		badAuthenticator(server);
	}
	if (res >= 0) {
		if (newname.length() > 0)
			uname = u8(newname);
		QStringList qsl;
		foreach(const ::std::string &str, groups) {
			qsl << u8(str);
		}
		if (! qsl.isEmpty())
			server->setTempGroups(res, sessionId, NULL, qsl);
	}
}

void MurmurIce::registerUserSlot(int &res, const QMap<int, QString> &info) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (! prx)
		return;

	::Murmur::UserInfoMap im;

	infoToInfo(info, im);
	try {
		res = prx->registerUser(im);
	} catch (...) {
		badAuthenticator(server);
	}
}

void MurmurIce::unregisterUserSlot(int &res, int id) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (! prx)
		return;
	try {
		res = prx->unregisterUser(id);
	} catch (...) {
		badAuthenticator(server);
	}
}

void MurmurIce::getRegistrationSlot(int &res, int id, QMap<int, QString> &info) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (! prx)
		return;

	Murmur::UserInfoMap im;
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

void  MurmurIce::getRegisteredUsersSlot(const QString &filter, QMap<int, QString> &m) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (! prx)
		return;

	::Murmur::NameMap lst;

	try {
		lst = prx->getRegisteredUsers(iceString(filter));
	} catch (...) {
		badAuthenticator(server);
		return;
	}
	::Murmur::NameMap::const_iterator i;
	for (i=lst.begin(); i != lst.end(); ++i)
		m.insert((*i).first, u8((*i).second));
}

void MurmurIce::setInfoSlot(int &res, int id, const QMap<int, QString> &info) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (! prx)
		return;

	Murmur::UserInfoMap im;
	infoToInfo(info, im);

	try {
		res = prx->setInfo(id, im);
	} catch (...) {
		badAuthenticator(server);
	}
}

void MurmurIce::setTextureSlot(int &res, int id, const QByteArray &texture) {
	::Server *server = qobject_cast< ::Server *> (sender());

	const ServerUpdatingAuthenticatorPrx prx = getServerUpdatingAuthenticator(server);
	if (! prx)
		return;

	::Murmur::Texture tex;
	tex.resize(texture.size());
	const char *ptr = texture.constData();
	for (int i=0;i<texture.size();++i)
		tex[i] = ptr[i];

	try {
		res = prx->setTexture(id, tex);
	} catch (...) {
		badAuthenticator(server);
	}
}

Ice::ObjectPtr ServerLocator::locate(const Ice::Current &, Ice::LocalObjectPtr &) {
	return iopServer;
}

#define FIND_SERVER \
	::Server *server = meta->qhServers.value(server_id);

#define NEED_SERVER_EXISTS \
	FIND_SERVER \
	if (!server && ! ServerDB::serverExists(server_id)) { \
		cb->ice_exception(::Ice::ObjectNotExistException(__FILE__,__LINE__)); \
		return; \
	}

#define NEED_SERVER \
	NEED_SERVER_EXISTS \
	if (! server) { \
		cb->ice_exception(ServerBootedException()); \
		return; \
	}

#define NEED_PLAYER \
	ServerUser *user = server->qhUsers.value(session); \
	if (!user) { \
		cb->ice_exception(::Murmur::InvalidSessionException()); \
		return; \
	}

#define NEED_CHANNEL_VAR(x,y) \
	x = server->qhChannels.value(y); \
	if (!x) { \
		cb->ice_exception(::Murmur::InvalidChannelException()); \
		return; \
	}

#define NEED_CHANNEL \
	::Channel *channel; \
	NEED_CHANNEL_VAR(channel, channelid);

void ServerI::ice_ping(const Ice::Current &current) const {
	// This is executed in the ice thread.
	int server_id = u8(current.id.name).toInt();
	if (! ServerDB::serverExists(server_id))
		throw ::Ice::ObjectNotExistException(__FILE__, __LINE__);
}

#define ACCESS_Server_isRunning_READ
static void impl_Server_isRunning(const ::Murmur::AMD_Server_isRunningPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	cb->ice_response(server != NULL);
}

static void impl_Server_start(const ::Murmur::AMD_Server_startPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	if (server)
		cb->ice_exception(ServerBootedException());
	else if (! meta->boot(server_id))
		cb->ice_exception(ServerFailureException());
	else
		cb->ice_response();
}

static void impl_Server_stop(const ::Murmur::AMD_Server_stopPtr cb, int server_id) {
	NEED_SERVER;
	meta->kill(server_id);
	cb->ice_response();
}

static void impl_Server_delete(const ::Murmur::AMD_Server_deletePtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	if (server) {
		cb->ice_exception(ServerBootedException());
		return;
	}
	ServerDB::deleteServer(server_id);
	cb->ice_response();
}

static void impl_Server_addCallback(const Murmur::AMD_Server_addCallbackPtr cb, int server_id, const Murmur::ServerCallbackPrx& cbptr) {
	NEED_SERVER;

	try {
		const Murmur::ServerCallbackPrx &oneway = Murmur::ServerCallbackPrx::checkedCast(cbptr->ice_oneway()->ice_connectionCached(false));
		mi->addServerCallback(server, oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

static void impl_Server_removeCallback(const Murmur::AMD_Server_removeCallbackPtr cb, int server_id, const Murmur::ServerCallbackPrx& cbptr) {
	NEED_SERVER;

	try {
		const Murmur::ServerCallbackPrx &oneway = Murmur::ServerCallbackPrx::uncheckedCast(cbptr->ice_oneway()->ice_connectionCached(false));
		mi->removeServerCallback(server, oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

static void impl_Server_setAuthenticator(const ::Murmur::AMD_Server_setAuthenticatorPtr& cb, int server_id, const ::Murmur::ServerAuthenticatorPrx &aptr) {
	NEED_SERVER;

	if (mi->getServerAuthenticator(server))
		server->disconnectAuthenticator(mi);

	::Murmur::ServerAuthenticatorPrx prx;

	try {
		prx = ::Murmur::ServerAuthenticatorPrx::checkedCast(aptr->ice_connectionCached(false)->ice_timeout(5000));
		const ::Murmur::ServerUpdatingAuthenticatorPrx uprx = ::Murmur::ServerUpdatingAuthenticatorPrx::checkedCast(prx);

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
static void impl_Server_id(const ::Murmur::AMD_Server_idPtr cb, int server_id) {
	NEED_SERVER_EXISTS;
	cb->ice_response(server_id);
}

#define ACCESS_Server_getConf_READ
static void impl_Server_getConf(const ::Murmur::AMD_Server_getConfPtr cb, int server_id,  const ::std::string& key) {
	NEED_SERVER_EXISTS;
	if (key == "key" || key == "passphrase")
		cb->ice_exception(WriteOnlyException());
	else
		cb->ice_response(iceString(ServerDB::getConf(server_id, u8(key)).toString()));
}

#define ACCESS_Server_getAllConf_READ
static void impl_Server_getAllConf(const ::Murmur::AMD_Server_getAllConfPtr cb, int server_id) {
	NEED_SERVER_EXISTS;

	::Murmur::ConfigMap cm;

	QMap<QString, QString> values = ServerDB::getAllConf(server_id);
	QMap<QString, QString>::const_iterator i;
	for (i=values.constBegin();i != values.constEnd(); ++i) {
		if (i.key() == "key" || i.key() == "passphrase")
			continue;
		cm[iceString(i.key())] = iceString(i.value());
	}
	cb->ice_response(cm);
}

static void impl_Server_setConf(const ::Murmur::AMD_Server_setConfPtr cb, int server_id,  const ::std::string& key,  const ::std::string& value) {
	NEED_SERVER_EXISTS;
	QString k=u8(key);
	QString v=u8(value);
	ServerDB::setConf(server_id, k, v);
	if (server)
		server->setLiveConf(k, v);
	cb->ice_response();
}

static void impl_Server_setSuperuserPassword(const ::Murmur::AMD_Server_setSuperuserPasswordPtr cb, int server_id,  const ::std::string& pw) {
	NEED_SERVER_EXISTS;
	ServerDB::setSUPW(server_id, u8(pw));
	cb->ice_response();
}

#define ACCESS_Server_getLog_READ
static void impl_Server_getLog(const ::Murmur::AMD_Server_getLogPtr cb, int server_id,  ::Ice::Int min,  ::Ice::Int max) {
	NEED_SERVER_EXISTS;

	::Murmur::LogList ll;

	QList<ServerDB::LogRecord> dblog = ServerDB::getLog(server_id, min, max);
	foreach(const ServerDB::LogRecord &e, dblog) {
		::Murmur::LogEntry le;
		logToLog(e, le);
		ll.push_back(le);
	}
	cb->ice_response(ll);
}

#define ACCESS_Server_getLogLen_READ
static void impl_Server_getLogLen(const ::Murmur::AMD_Server_getLogLenPtr cb, int server_id) {
	NEED_SERVER_EXISTS;

	int len = ServerDB::getLogLen(server_id);
	cb->ice_response(len);
}

#define ACCESS_Server_getUsers_READ
static void impl_Server_getUsers(const ::Murmur::AMD_Server_getUsersPtr cb, int server_id) {
	NEED_SERVER;
	::Murmur::UserMap pm;
	foreach(const ::User *p, server->qhUsers) {
		::Murmur::User mp;
		if (static_cast<const ServerUser *>(p)->sState == ::ServerUser::Authenticated) {
			userToUser(p, mp);
			pm[p->uiSession] = mp;
		}
	}
	cb->ice_response(pm);
}

#define ACCESS_Server_getChannels_READ
static void impl_Server_getChannels(const ::Murmur::AMD_Server_getChannelsPtr cb, int server_id) {
	NEED_SERVER;
	::Murmur::ChannelMap cm;
	foreach(const ::Channel *c, server->qhChannels) {
		::Murmur::Channel mc;
		channelToChannel(c, mc);
		cm[c->iId] = mc;
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
	QList< ::User *> users = c->qlUsers;
	std::sort(users.begin(), users.end(), userSort);

	foreach(const ::User *p, users) {
		::Murmur::User mp;
		userToUser(p, mp);
		t->users.push_back(mp);
	}

	QList< ::Channel *> channels = c->qlChannels;
	std::sort(channels.begin(), channels.end(), channelSort);

	foreach(const ::Channel *chn, channels) {
		t->children.push_back(recurseTree(chn));
	}

	return t;
}

#define ACCESS_Server_getTree_READ
static void impl_Server_getTree(const ::Murmur::AMD_Server_getTreePtr cb, int server_id) {
	NEED_SERVER;
	cb->ice_response(recurseTree(server->qhChannels.value(0)));
}

#define ACCESS_Server_getCertificateList_READ
static void impl_Server_getCertificateList(const ::Murmur::AMD_Server_getCertificateListPtr cb, int server_id, ::Ice::Int session) {
	NEED_SERVER;
	NEED_PLAYER;

	::Murmur::CertificateList certs;

	const QList<QSslCertificate> &certlist = user->peerCertificateChain();

	certs.resize(certlist.size());
	for (int i=0;i<certlist.size();++i) {
		::Murmur::CertificateDer der;
		QByteArray qba = certlist.at(i).toDer();
		der.resize(qba.size());
		const char *ptr = qba.constData();
		for (int j=0;j<qba.size();++j)
			der[j] = ptr[j];
		certs[i] = der;
	}
	cb->ice_response(certs);
}

#define ACCESS_Server_getBans_READ
static void impl_Server_getBans(const ::Murmur::AMD_Server_getBansPtr cb, int server_id) {
	NEED_SERVER;
	::Murmur::BanList bl;
	foreach(const ::Ban &ban, server->qlBans) {
		::Murmur::Ban mb;
		banToBan(ban, mb);
		bl.push_back(mb);
	}
	cb->ice_response(bl);
}

static void impl_Server_setBans(const ::Murmur::AMD_Server_setBansPtr cb, int server_id,  const ::Murmur::BanList& bans) {
	NEED_SERVER;
	server->qlBans.clear();
	foreach(const ::Murmur::Ban &mb, bans) {
		::Ban ban;
		banToBan(mb, ban);
		server->qlBans << ban;
	}
	server->saveBans();
	cb->ice_response();
}

static void impl_Server_kickUser(const ::Murmur::AMD_Server_kickUserPtr cb, int server_id,  ::Ice::Int session,  const ::std::string& reason) {
	NEED_SERVER;
	NEED_PLAYER;

	MumbleProto::UserRemove mpur;
	mpur.set_session(session);
	mpur.set_reason(reason);
	server->sendAll(mpur);
	user->disconnectSocket();
	cb->ice_response();
}

static void impl_Server_sendMessage(const ::Murmur::AMD_Server_sendMessagePtr cb, int server_id, ::Ice::Int session, const ::std::string &text) {
	NEED_SERVER;
	NEED_PLAYER;

	server->sendTextMessage(NULL, user, false, u8(text));
	cb->ice_response();
}

#define ACCESS_Server_hasPermission_READ
static void impl_Server_hasPermission(const ::Murmur::AMD_Server_hasPermissionPtr cb, int server_id, ::Ice::Int session, ::Ice::Int channelid, ::Ice::Int perm) {
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL;
	cb->ice_response(server->hasPermission(user, channel, static_cast<ChanACL::Perm>(perm)));
}

#define ACCESS_Server_effectivePermissions_READ
static void impl_Server_effectivePermissions(const ::Murmur::AMD_Server_effectivePermissionsPtr cb, int server_id, ::Ice::Int session, ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL;
	cb->ice_response(server->effectivePermissions(user, channel));
}

static void impl_Server_addContextCallback(const Murmur::AMD_Server_addContextCallbackPtr cb, int server_id, ::Ice::Int session, const ::std::string& action, const ::std::string& text, const ::Murmur::ServerContextCallbackPrx& cbptr, int ctx) {
	NEED_SERVER;
	NEED_PLAYER;

	const QMap<QString, ::Murmur::ServerContextCallbackPrx>& qmPrx = mi->getServerContextCallbacks(server)[session];

	if (!(ctx & (MumbleProto::ContextActionModify_Context_Server | MumbleProto::ContextActionModify_Context_Channel | MumbleProto::ContextActionModify_Context_User))) {
		cb->ice_exception(InvalidCallbackException());
		return;
	}

	try {
		const Murmur::ServerContextCallbackPrx &oneway = Murmur::ServerContextCallbackPrx::checkedCast(cbptr->ice_oneway()->ice_connectionCached(false)->ice_timeout(5000));
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
	mpcam.set_context(ctx);
	mpcam.set_operation(MumbleProto::ContextActionModify_Operation_Add);
	server->sendMessage(user, mpcam);
}

static void impl_Server_removeContextCallback(const Murmur::AMD_Server_removeContextCallbackPtr cb, int server_id, const Murmur::ServerContextCallbackPrx& cbptr) {
	NEED_SERVER;

	const QMap< int, QMap<QString, ::Murmur::ServerContextCallbackPrx> >& qmPrx = mi->getServerContextCallbacks(server);

	try {
		const Murmur::ServerContextCallbackPrx &oneway = Murmur::ServerContextCallbackPrx::uncheckedCast(cbptr->ice_oneway()->ice_connectionCached(false)->ice_timeout(5000));

		foreach(int session, qmPrx.keys()) {
			ServerUser *user = server->qhUsers.value(session);
			const QMap<QString, ::Murmur::ServerContextCallbackPrx> &qm = qmPrx[session];
			foreach(const QString &act, qm.keys(oneway)) {
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
static void impl_Server_getState(const ::Murmur::AMD_Server_getStatePtr cb, int server_id,  ::Ice::Int session) {
	NEED_SERVER;
	NEED_PLAYER;

	::Murmur::User mp;
	userToUser(user, mp);
	cb->ice_response(mp);
}

static void impl_Server_setState(const ::Murmur::AMD_Server_setStatePtr cb, int server_id,  const ::Murmur::User& state) {
	int session = state.session;
	::Channel *channel;
	NEED_SERVER;
	NEED_PLAYER;
	NEED_CHANNEL_VAR(channel, state.channel);

	server->setUserState(user, channel, state.mute, state.deaf, state.suppress, state.prioritySpeaker, u8(state.name), u8(state.comment));
	cb->ice_response();
}

static void impl_Server_sendMessageChannel(const ::Murmur::AMD_Server_sendMessageChannelPtr cb, int server_id, ::Ice::Int channelid, bool tree, const ::std::string &text) {
	NEED_SERVER;
	NEED_CHANNEL;

	server->sendTextMessage(channel, NULL, tree, u8(text));
	cb->ice_response();
}

#define ACCESS_Server_getChannelState_READ
static void impl_Server_getChannelState(const ::Murmur::AMD_Server_getChannelStatePtr cb, int server_id,  ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	::Murmur::Channel mc;
	channelToChannel(channel, mc);
	cb->ice_response(mc);
}

static void impl_Server_setChannelState(const ::Murmur::AMD_Server_setChannelStatePtr cb, int server_id,  const ::Murmur::Channel& state) {
	int channelid = state.id;
	NEED_SERVER;
	NEED_CHANNEL;
	::Channel *np = NULL;
	if (channel->iId != 0) {
		NEED_CHANNEL_VAR(np, state.parent);
	}

	QString qsName = u8(state.name);

	QSet< ::Channel *> newset;
	foreach(int linkid, state.links) {
		::Channel *cLink;
		NEED_CHANNEL_VAR(cLink, linkid);
		newset << cLink;
	}

	if (!server->canNest(np, channel)) {
		cb->ice_exception(::Murmur::NestingLimitException());
		return;
	}

	if (! server->setChannelState(channel, np, qsName, newset, u8(state.description), state.position))
		cb->ice_exception(::Murmur::InvalidChannelException());
	else
		cb->ice_response();
}

static void impl_Server_removeChannel(const ::Murmur::AMD_Server_removeChannelPtr cb, int server_id,  ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	if (!channel->cParent) {
		cb->ice_exception(::Murmur::InvalidChannelException());
	} else {
		server->removeChannel(channel);
		cb->ice_response();
	}
}

static void impl_Server_addChannel(const ::Murmur::AMD_Server_addChannelPtr cb, int server_id,  const ::std::string& name,  ::Ice::Int parent) {
	NEED_SERVER;
	::Channel *p, *nc;
	NEED_CHANNEL_VAR(p, parent);

	if (!server->canNest(p)) {
		cb->ice_exception(::Murmur::NestingLimitException());
		return;
	}

	QString qsName = u8(name);

	nc = server->addChannel(p, qsName);
	server->updateChannel(nc);
	int newid = nc->iId;

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(newid);
	mpcs.set_parent(parent);
	mpcs.set_name(name);
	server->sendAll(mpcs);

	cb->ice_response(newid);
}

#define ACCESS_Server_getACL_READ
static void impl_Server_getACL(const ::Murmur::AMD_Server_getACLPtr cb, int server_id, ::Ice::Int channelid) {
	NEED_SERVER;
	NEED_CHANNEL;

	::Murmur::ACLList acls;
	::Murmur::GroupList groups;

	QStack< ::Channel *> chans;
	::Channel *p;
	ChanACL *acl;
	p = channel;
	while (p) {
		chans.push(p);
		if ((p == channel) || (p->bInheritACL))
			p = p->cParent;
		else
			p = NULL;
	}

	bool inherit = channel->bInheritACL;

	while (! chans.isEmpty()) {
		p = chans.pop();
		foreach(acl, p->qlACL) {
			if ((p==channel) || (acl->bApplySubs)) {
				::Murmur::ACL ma;
				ACLtoACL(acl, ma);
				if (p != channel)
					ma.inherited = true;
				acls.push_back(ma);
			}
		}
	}

	p = channel->cParent;
	const QSet<QString> allnames = ::Group::groupNames(channel);
	foreach(const QString &name, allnames) {
		::Group *g = channel->qhGroups.value(name);
		::Group *pg = p ? ::Group::getGroup(p, name) : NULL;
		if (!g && ! pg)
			continue;
		::Murmur::Group mg;
		groupToGroup(g ? g : pg, mg);
		QSet<int> members;
		if (pg)
			members = pg->members();
		if (g) {
			mg.add = g->qsAdd.toList().toVector().toStdVector();
			mg.remove = g->qsRemove.toList().toVector().toStdVector();
			mg.inherited = false;
			members += g->qsAdd;
			members -= g->qsRemove;
		} else {
			mg.inherited = true;
		}
		mg.members = members.toList().toVector().toStdVector();
		groups.push_back(mg);
	}
	cb->ice_response(acls, groups, inherit);
}

static void impl_Server_setACL(const ::Murmur::AMD_Server_setACLPtr cb, int server_id,  ::Ice::Int channelid,  const ::Murmur::ACLList& acls,  const ::Murmur::GroupList& groups,  bool inherit) {
	NEED_SERVER;
	NEED_CHANNEL;

	::Group *g;
	ChanACL *acl;

	QHash<QString, QSet<int> > hOldTemp;
	foreach(g, channel->qhGroups) {
		hOldTemp.insert(g->qsName, g->qsTemporary);
		delete g;
	}
	foreach(acl, channel->qlACL)
		delete acl;

	channel->qhGroups.clear();
	channel->qlACL.clear();

	channel->bInheritACL = inherit;
	foreach(const ::Murmur::Group &gi, groups) {
		QString name = u8(gi.name);
		g = new ::Group(channel, name);
		g->bInherit = gi.inherit;
		g->bInheritable = gi.inheritable;
		g->qsAdd = QVector<int>::fromStdVector(gi.add).toList().toSet();
		g->qsRemove = QVector<int>::fromStdVector(gi.remove).toList().toSet();
		g->qsTemporary = hOldTemp.value(name);
	}
	foreach(const ::Murmur::ACL &ai, acls) {
		acl = new ChanACL(channel);
		acl->bApplyHere = ai.applyHere;
		acl->bApplySubs = ai.applySubs;
		acl->iUserId = ai.userid;
		acl->qsGroup = u8(ai.group);
		acl->pDeny = static_cast<ChanACL::Permissions>(ai.deny) & ChanACL::All;
		acl->pAllow = static_cast<ChanACL::Permissions>(ai.allow) & ChanACL::All;
	}

	server->clearACLCache();
	server->updateChannel(channel);
	cb->ice_response();
}

#define ACCESS_Server_getUserNames_READ
static void impl_Server_getUserNames(const ::Murmur::AMD_Server_getUserNamesPtr cb, int server_id,  const ::Murmur::IdList& ids) {
	NEED_SERVER;
	::Murmur::NameMap nm;
	foreach(int userid, ids) {
		nm[userid] = iceString(server->getUserName(userid));
	}
	cb->ice_response(nm);
}

#define ACCESS_Server_getUserIds_READ
static void impl_Server_getUserIds(const ::Murmur::AMD_Server_getUserIdsPtr cb, int server_id,  const ::Murmur::NameList& names) {
	NEED_SERVER;
	::Murmur::IdMap im;
	foreach(const string &n, names) {
		QString name = u8(n);
		im[n] = server->getUserID(name);
	}
	cb->ice_response(im);
}

static void impl_Server_registerUser(const ::Murmur::AMD_Server_registerUserPtr cb, int server_id, const ::Murmur::UserInfoMap &im) {
	NEED_SERVER;

	QMap<int, QString> info;
	infoToInfo(im, info);

	int userid = server->registerUser(info);
	if (userid < 0)
		cb->ice_exception(InvalidUserException());
	else
		cb->ice_response(userid);
}

static void impl_Server_unregisterUser(const ::Murmur::AMD_Server_unregisterUserPtr cb, int server_id,  ::Ice::Int userid) {
	NEED_SERVER;
	if (! server->unregisterUser(userid))
		cb->ice_exception(InvalidUserException());
	else
		cb->ice_response();
}

static void impl_Server_updateRegistration(const ::Murmur::AMD_Server_updateRegistrationPtr cb, int server_id,  int id, const ::Murmur::UserInfoMap &im) {
	NEED_SERVER;

	if (! server->isUserId(id)) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	QMap<int, QString> info;
	infoToInfo(im, info);

	if (! server->setInfo(id, info)) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	if (info.contains(ServerDB::User_Comment)) {
		foreach(ServerUser *u, server->qhUsers) {
			if (u->iId == id)
				server->setUserState(u, u->cChannel, u->bMute, u->bDeaf, u->bSuppress, u->bPrioritySpeaker, u->qsName, info.value(ServerDB::User_Comment));
		}
	}

	cb->ice_response();
}

#define ACCESS_Server_getRegistration_READ
static void impl_Server_getRegistration(const ::Murmur::AMD_Server_getRegistrationPtr cb, int server_id,  ::Ice::Int userid) {
	NEED_SERVER;

	QMap<int, QString> info = server->getRegistration(userid);

	if (info.isEmpty()) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	Murmur::UserInfoMap im;
	infoToInfo(info, im);
	cb->ice_response(im);
}

#define ACCESS_Server_getRegisteredUsers_READ
static void impl_Server_getRegisteredUsers(const ::Murmur::AMD_Server_getRegisteredUsersPtr cb, int server_id,  const ::std::string& filter) {
	NEED_SERVER;
	Murmur::NameMap rpl;

	const QMap<int, QString> l = server->getRegisteredUsers(u8(filter));
	QMap<int, QString>::const_iterator i;
	for (i = l.constBegin(); i != l.constEnd(); ++i) {
		rpl[i.key()] = u8(i.value());
	}

	cb->ice_response(rpl);
}

#define ACCESS_Server_verifyPassword_READ
static void impl_Server_verifyPassword(const ::Murmur::AMD_Server_verifyPasswordPtr cb, int server_id,  const ::std::string& name,  const ::std::string& pw) {
	NEED_SERVER;
	QString uname = u8(name);
	cb->ice_response(server->authenticate(uname, u8(pw)));
}

#define ACCESS_Server_getTexture_READ
static void impl_Server_getTexture(const ::Murmur::AMD_Server_getTexturePtr cb, int server_id,  ::Ice::Int userid) {
	NEED_SERVER;

	if (! server->isUserId(userid)) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	const QByteArray &qba = server->getUserTexture(userid);

	::Murmur::Texture tex;
	tex.resize(qba.size());
	const char *ptr = qba.constData();
	for (int i=0;i<qba.size();++i)
		tex[i] = ptr[i];

	cb->ice_response(tex);
}

static void impl_Server_setTexture(const ::Murmur::AMD_Server_setTexturePtr cb, int server_id,  ::Ice::Int userid,  const ::Murmur::Texture& tex) {
	NEED_SERVER;

	if (! server->isUserId(userid)) {
		cb->ice_exception(InvalidUserException());
		return;
	}

	QByteArray qba(static_cast<int>(tex.size()), 0);
	char *ptr = qba.data();
	for (unsigned int i=0;i<tex.size();++i)
		ptr[i] = tex[i];
	if (! server->setTexture(userid, qba)) {
		cb->ice_exception(InvalidTextureException());
	} else {
		ServerUser *user = server->qhUsers.value(userid);
		if (user) {
			MumbleProto::UserState mpus;
			mpus.set_session(user->uiSession);
			mpus.set_texture(blob(user->qbaTexture));

			server->sendAll(mpus, ~0x010202);
			if (! user->qbaTextureHash.isEmpty()) {
				mpus.clear_texture();
				mpus.set_texture_hash(blob(user->qbaTextureHash));
			}
			server->sendAll(mpus, 0x010202);
		}

		cb->ice_response();
	}
}

#define ACCESS_Server_getUptime_READ
static void impl_Server_getUptime(const ::Murmur::AMD_Server_getUptimePtr cb, int server_id) {
	NEED_SERVER;
	cb->ice_response(static_cast<int>(server->tUptime.elapsed()/1000000LL));
}

static void impl_Server_updateCertificate(const ::Murmur::AMD_Server_updateCertificatePtr cb, int server_id, const ::std::string& certificate, const ::std::string& privateKey, const ::std::string& passphrase) {
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
	server->initializeCert();

	cb->ice_response();
}

static void impl_Server_addUserToGroup(const ::Murmur::AMD_Server_addUserToGroupPtr cb, int server_id, ::Ice::Int channelid,  ::Ice::Int session,  const ::std::string& group) {
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
		if (! g)
			g = new ::Group(channel, qsgroup);

		g->qsTemporary.insert(- session);
	}

	server->clearACLCache(user);

	cb->ice_response();
}

static void impl_Server_removeUserFromGroup(const ::Murmur::AMD_Server_removeUserFromGroupPtr cb, int server_id,  ::Ice::Int channelid,  ::Ice::Int session,  const ::std::string& group) {
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

static void impl_Server_redirectWhisperGroup(const ::Murmur::AMD_Server_redirectWhisperGroupPtr cb, int server_id,  ::Ice::Int session,  const ::std::string& source,  const ::std::string& target) {
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
static void impl_Meta_getSliceChecksums(const ::Murmur::AMD_Meta_getSliceChecksumsPtr cb, const Ice::ObjectAdapterPtr) {
	cb->ice_response(::Ice::sliceChecksums());
}

#define ACCESS_Meta_getServer_READ
static void impl_Meta_getServer(const ::Murmur::AMD_Meta_getServerPtr cb, const Ice::ObjectAdapterPtr adapter, ::Ice::Int id) {
	QList<int> server_list = ServerDB::getAllServers();
	if (! server_list.contains(id))
		cb->ice_response(NULL);
	else
		cb->ice_response(idToProxy(id, adapter));
}

static void impl_Meta_newServer(const ::Murmur::AMD_Meta_newServerPtr cb, const Ice::ObjectAdapterPtr adapter) {
	cb->ice_response(idToProxy(ServerDB::addServer(), adapter));
}

#define ACCESS_Meta_getAllServers_READ
static void impl_Meta_getAllServers(const ::Murmur::AMD_Meta_getAllServersPtr cb, const Ice::ObjectAdapterPtr adapter) {
	::Murmur::ServerList sl;

	foreach(int id, ServerDB::getAllServers())
		sl.push_back(idToProxy(id, adapter));
	cb->ice_response(sl);
}

#define ACCESS_Meta_getDefaultConf_READ
static void impl_Meta_getDefaultConf(const ::Murmur::AMD_Meta_getDefaultConfPtr cb, const Ice::ObjectAdapterPtr) {
	::Murmur::ConfigMap cm;
	QMap<QString, QString>::const_iterator i;
	for (i=meta->mp.qmConfig.constBegin();i != meta->mp.qmConfig.constEnd(); ++i) {
		if (i.key() == "key" || i.key() == "passphrase")
			continue;
		cm[iceString(i.key())] = iceString(i.value());
	}
	cb->ice_response(cm);
}

#define ACCESS_Meta_getBootedServers_READ
static void impl_Meta_getBootedServers(const ::Murmur::AMD_Meta_getBootedServersPtr cb, const Ice::ObjectAdapterPtr adapter) {
	::Murmur::ServerList sl;

	foreach(int id, meta->qhServers.keys())
		sl.push_back(idToProxy(id, adapter));
	cb->ice_response(sl);
}

#define ACCESS_Meta_getVersion_ALL
static void impl_Meta_getVersion(const ::Murmur::AMD_Meta_getVersionPtr cb, const Ice::ObjectAdapterPtr) {
	int major, minor, patch;
	QString txt;
	::Meta::getVersion(major, minor, patch, txt);
	cb->ice_response(major, minor, patch, iceString(txt));
}

static void impl_Meta_addCallback(const Murmur::AMD_Meta_addCallbackPtr cb, const Ice::ObjectAdapterPtr, const Murmur::MetaCallbackPrx& cbptr) {
	try {
		const Murmur::MetaCallbackPrx &oneway = Murmur::MetaCallbackPrx::checkedCast(cbptr->ice_oneway()->ice_connectionCached(false)->ice_timeout(5000));
		mi->addMetaCallback(oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

static void impl_Meta_removeCallback(const Murmur::AMD_Meta_removeCallbackPtr cb, const Ice::ObjectAdapterPtr, const Murmur::MetaCallbackPrx& cbptr) {
	try {
		const Murmur::MetaCallbackPrx &oneway = Murmur::MetaCallbackPrx::uncheckedCast(cbptr->ice_oneway()->ice_connectionCached(false)->ice_timeout(5000));
		mi->removeMetaCallback(oneway);
		cb->ice_response();
	} catch (...) {
		cb->ice_exception(InvalidCallbackException());
	}
}

#define ACCESS_Meta_getUptime_ALL
static void impl_Meta_getUptime(const ::Murmur::AMD_Meta_getUptimePtr cb, const Ice::ObjectAdapterPtr) {
	cb->ice_response(static_cast<int>(meta->tUptime.elapsed()/1000000LL));
}

#include "MurmurIceWrapper.cpp"
