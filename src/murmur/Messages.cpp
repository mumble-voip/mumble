/* Copyright (C) 2005-2009, Thorvald Natvig <thorvald@natvig.com>

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

#include "User.h"
#include "Channel.h"
#include "ACL.h"
#include "Group.h"
#include "Message.h"
#include "ServerDB.h"
#include "Connection.h"
#include "Server.h"
#include "DBus.h"

#define MSG_SETUP(st) \
	if (uSource->sState != st) \
		return

#define VICTIM_SETUP \
	ServerUser *pDstServerUser = uSource; \
	if (msg.has_session()) \
		pDstServerUser = qhUsers.value(msg.session()); \
	if (! pDstServerUser) \
		return; \
	Q_UNUSED(pDstServerUser)

#define PERM_DENIED(who, where, what) \
	{ \
		MumbleProto::PermissionDenied mppd; \
		mppd.set_permission(static_cast<int>(what)); \
		mppd.set_channel_id(where->iId); \
		mppd.set_session(who->uiSession); \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_Permission); \
		sendMessage(uSource, mppd); \
		log(uSource, QString("%1 not allowed to %2 in %3").arg(who->qsName).arg(ChanACL::permName(what)).arg(where->qsName)); \
	}
#define PERM_DENIED_TYPE(type) \
	{ \
		MumbleProto::PermissionDenied mppd; \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_##type); \
		sendMessage(uSource, mppd); \
	}

void Server::msgAuthenticate(ServerUser *uSource, MumbleProto::Authenticate &msg) {
	if ((msg.tokens_size() > 0) || (uSource->sState == User::Authenticated)) {
		QStringList qsl;
		for (int i=0;i<msg.tokens_size();++i)
			qsl << u8(msg.tokens(i));
		{
			QMutexLocker qml(&qmCache);
			uSource->qslAccessTokens = qsl;
		}
		clearACLCache(uSource);
	}
	MSG_SETUP(User::Connected);

	Channel *root = qhChannels.value(0);
	Channel *c;

	uSource->qsName = u8(msg.username());

	bool ok = false;
	bool nameok = validateUserName(uSource->qsName);
	QString pw = u8(msg.password());

	// Fetch ID and stored username.
	// Since this may call DBus, which may recall our dbus messages, this function needs
	// to support re-entrancy, and also to support the fact that sessions may go away.
	int id = authenticate(uSource->qsName, pw, uSource->qslEmail, uSource->qsHash, uSource->bVerified);

	uSource->iId = id >= 0 ? id : -1;

	QString reason;
	MumbleProto::Reject_RejectType rtType = MumbleProto::Reject_RejectType_None;

	if (! nameok && (uSource->iId == -1)) {
		reason = "Invalid ServerUsername";
		rtType = MumbleProto::Reject_RejectType_InvalidUsername;
	} else if (id==-1) {
		reason = "Wrong password for user";
		rtType = MumbleProto::Reject_RejectType_WrongUserPW;
	} else if (id==-2 && ! qsPassword.isEmpty() && qsPassword != pw) {
		reason = "Invalid server password";
		rtType = MumbleProto::Reject_RejectType_WrongServerPW;
	} else {
		ok = true;
	}

	ServerUser *uOld = NULL;
	foreach(ServerUser *u, qhUsers) {
		if (u == uSource)
			continue;
		if (((u->iId>=0) && (u->iId == uSource->iId)) ||
		        (u->qsName == uSource->qsName)) {
			uOld = u;
			break;
		}
	}

	// Allow reuse of name from same IP
	if (ok && uOld && (uSource->iId == -1)) {
		if (uOld->peerAddress() != uSource->peerAddress()) {
			reason = "Username already in use";
			rtType = MumbleProto::Reject_RejectType_UsernameInUse;
			ok = false;
		}
	}

	if ((id != 0) && (qhUsers.count() > iMaxUsers)) {
		reason = QString::fromLatin1("Server is full (max %1 users)").arg(iMaxUsers);
		rtType = MumbleProto::Reject_RejectType_ServerFull;
		ok = false;
	}

	if (! ok) {
		log(uSource, QString("Rejected connection: %1").arg(reason));
		MumbleProto::Reject mpr;
		mpr.set_reason(u8(reason));
		mpr.set_type(rtType);
		sendMessage(uSource, mpr);
		uSource->disconnectSocket();
		return;
	}

	startThread();

	// Kick ghost
	if (uOld) {
		log(uSource, "Disconnecting ghost");
		uOld->disconnectSocket(true);
	}

	// Setup UDP encryption
	uSource->csCrypt.genKey();

	MumbleProto::CryptSetup mpcrypt;
	mpcrypt.set_key(std::string(reinterpret_cast<const char *>(uSource->csCrypt.raw_key), AES_BLOCK_SIZE));
	mpcrypt.set_server_nonce(std::string(reinterpret_cast<const char *>(uSource->csCrypt.encrypt_iv), AES_BLOCK_SIZE));
	mpcrypt.set_client_nonce(std::string(reinterpret_cast<const char *>(uSource->csCrypt.decrypt_iv), AES_BLOCK_SIZE));
	sendMessage(uSource, mpcrypt);

	int lchan = readLastChannel(uSource->iId);
	if (lchan == 0)
		lchan = iDefaultChan;
	Channel *lc = qhChannels.value(lchan);
	if (! lc)
		lc = root;
	else if (! hasPermission(uSource, lc, ChanACL::Enter))
		lc = root;

	userEnterChannel(uSource, lc, true);

	// Transmit channel tree
	QQueue<Channel *> q;
	QSet<Channel *> chans;
	q << root;
	MumbleProto::ChannelState mpcs;
	while (! q.isEmpty()) {
		c = q.dequeue();
		chans.insert(c);

		mpcs.Clear();

		mpcs.set_channel_id(c->iId);
		if (c->cParent)
			mpcs.set_parent(c->cParent->iId);
		if (c->iId == 0)
			mpcs.set_name(u8(qsRegName.isEmpty() ? QLatin1String("Root") : qsRegName));
		else
			mpcs.set_name(u8(c->qsName));
		if (! c->qsDesc.isEmpty())
			mpcs.set_description(u8(c->qsDesc));

		sendMessage(uSource, mpcs);

		foreach(c, c->qlChannels)
			q.enqueue(c);
	}

	// Transmit links
	foreach(c, chans) {
		if (c->qhLinks.count() > 0) {
			mpcs.Clear();
			mpcs.set_channel_id(c->iId);

			foreach(Channel *l, c->qhLinks.keys())
				mpcs.add_links(l->iId);
			sendMessage(uSource, mpcs);
		}
	}

	// Transmit user profile
	MumbleProto::UserState mpus;

	uSource->sState = User::Authenticated;
	mpus.set_session(uSource->uiSession);
	mpus.set_name(u8(uSource->qsName));
	if (uSource->iId >= 0) {
		uSource->qbaTexture = getUserTexture(uSource->iId);
		mpus.set_user_id(uSource->iId);
		if (! uSource->qbaTexture.isEmpty())
			mpus.set_texture(std::string(uSource->qbaTexture.constData(), uSource->qbaTexture.size()));

		const QMap<QString, QString> &info = getRegistration(uSource->iId);
		if (info.contains("comment")) {
			uSource->qsComment = info.value("comment");
			mpus.set_comment(u8(uSource->qsComment));
		}
	}
	if (! uSource->qsHash.isEmpty())
		mpus.set_hash(u8(uSource->qsHash));
	if (uSource->cChannel->iId != 0)
		mpus.set_channel_id(uSource->cChannel->iId);

	sendExcept(uSource, mpus);

	// Transmit other users profiles
	foreach(ServerUser *u, qhUsers) {
		if (u->sState != User::Authenticated)
			continue;

		mpus.Clear();
		mpus.set_session(u->uiSession);
		mpus.set_name(u8(u->qsName));
		if (u->iId >= 0) {
			mpus.set_user_id(u->iId);
			if (! u->qbaTexture.isEmpty())
				mpus.set_texture(std::string(u->qbaTexture.constData(), u->qbaTexture.size()));
		}

		if (u->cChannel->iId != 0)
			mpus.set_channel_id(u->cChannel->iId);
		if (u->bDeaf)
			mpus.set_deaf(true);
		else if (u->bMute)
			mpus.set_mute(true);
		if (u->bSuppressed)
			mpus.set_suppressed(true);
		if (u->bSelfDeaf)
			mpus.set_self_deaf(true);
		else if (u->bSelfMute)
			mpus.set_self_mute(true);
		if (! u->qsComment.isEmpty())
			mpus.set_comment(u8(u->qsComment));
		if (! u->qsHash.isEmpty())
			mpus.set_hash(u8(u->qsHash));

		sendMessage(uSource, mpus);
	}

	// Send syncronisation packet
	MumbleProto::ServerSync mpss;
	mpss.set_session(uSource->uiSession);
	if (! qsWelcomeText.isEmpty())
		mpss.set_welcome_text(u8(qsWelcomeText));
	mpss.set_max_bandwidth(iMaxBandwidth);

	if (uSource->iId == 0) {
		mpss.set_permissions(ChanACL::All);
	} else {
		hasPermission(uSource, root, ChanACL::Enter);
		QMutexLocker qml(&qmCache);
		mpss.set_permissions(acCache.value(uSource)->value(root));
	}

	sendMessage(uSource, mpss);
	log(uSource, "Authenticated");

	emit userConnected(uSource);
	userEnterChannel(uSource, lc, false);
}

void Server::msgBanList(ServerUser *uSource, MumbleProto::BanList &msg) {
	MSG_SETUP(User::Authenticated);

	if (! hasPermission(uSource, qhChannels.value(0), ChanACL::Ban)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Ban);
		return;
	}
	if (msg.query()) {
		msg.clear_query();
		msg.clear_bans();
		foreach(const Ban &b, qlBans) {
			MumbleProto::BanList_BanEntry *be = msg.add_bans();
			be->set_address(b.haAddress.toStdString());
			be->set_mask(b.iMask);
			be->set_name(u8(b.qsUsername));
			be->set_hash(u8(b.qsHash));
			be->set_reason(u8(b.qsReason));
			be->set_start(u8(b.qdtStart.toString(Qt::ISODate)));
			be->set_duration(b.iDuration);
		}
		sendMessage(uSource, msg);
	} else {
		qlBans.clear();
		for (int i=0;i < msg.bans_size(); ++i) {
			const MumbleProto::BanList_BanEntry &be = msg.bans(i);

			Ban b;
			b.haAddress = be.address();
			b.iMask = be.mask();
			b.qsUsername = u8(be.name());
			b.qsHash = u8(be.hash());
			b.qsReason = u8(be.reason());
			if (be.has_start())
				b.qdtStart = QDateTime::fromString(u8(be.start()), Qt::ISODate);
			else
				b.qdtStart = QDateTime::currentDateTime().toUTC();
			b.iDuration = be.duration();
			if (b.isValid())
				qlBans << b;
		}
		saveBans();
		log(uSource, "Updated banlist");
	}
}

void Server::msgReject(ServerUser *, MumbleProto::Reject &) {
}

void Server::msgServerSync(ServerUser *, MumbleProto::ServerSync &) {
}

void Server::msgPermissionDenied(ServerUser *, MumbleProto::PermissionDenied &) {
}

void Server::msgUDPTunnel(ServerUser *uSource, MumbleProto::UDPTunnel &msg) {
	MSG_SETUP(User::Authenticated);

	const std::string &str = msg.packet();
	int len = static_cast<int>(str.length());
	if (len < 1)
		return;
	QReadLocker rl(&qrwlUsers);
	processMsg(uSource, str.data(), len);
}

void Server::msgUserState(ServerUser *uSource, MumbleProto::UserState &msg) {
	MSG_SETUP(User::Authenticated);
	VICTIM_SETUP;

	bool bNoBroadcast = false;
	Channel *root = qhChannels.value(0);

	if (pDstServerUser->iId == 0) {
		PERM_DENIED_TYPE(SuperUser);
		return;
	}

	msg.set_session(pDstServerUser->uiSession);
	msg.set_actor(uSource->uiSession);

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());
		if (!c || (c == pDstServerUser->cChannel))
			return;

		if ((uSource != pDstServerUser) && (! hasPermission(uSource, pDstServerUser->cChannel, ChanACL::Move))) {
			PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::Move);
			return;
		}

		if (! hasPermission(uSource, c, ChanACL::Move) && ! hasPermission(pDstServerUser, c, ChanACL::Enter)) {
			PERM_DENIED(pDstServerUser, c, ChanACL::Enter);
			return;
		}
	}

	if (msg.has_mute() || msg.has_deaf() || msg.has_suppressed()) {
		if (! hasPermission(uSource, pDstServerUser->cChannel, ChanACL::MuteDeafen) || msg.suppressed()) {
			PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::MuteDeafen);
			return;
		}
	}

	if (msg.has_comment() && (uSource != pDstServerUser)) {
		if (! hasPermission(uSource, root, ChanACL::Move)) {
			PERM_DENIED(uSource, root, ChanACL::Move);
			return;
		}
	}

	if (msg.has_user_id()) {
		if ((pDstServerUser->iId >= 0) || (pDstServerUser->qsHash.isEmpty()) || ! hasPermission(uSource, root, ChanACL::Register)) {
			PERM_DENIED(uSource, root, ChanACL::Register);
			return;
		}
	}

	if ((pDstServerUser != uSource) && (msg.has_self_deaf() || msg.has_self_mute() || msg.has_texture() || msg.has_plugin_context() || msg.has_plugin_identity()))
		return;

	// Permission checks done. Now enact this.

	if (msg.has_texture()) {
		if (uSource->iId > 0) {
			QByteArray qba = QByteArray(msg.texture().data(), msg.texture().size());
			if (! setTexture(uSource->iId, qba))
				return;
		}
	}

	if (msg.has_self_deaf()) {
		uSource->bSelfDeaf = msg.self_deaf();
		if (uSource->bSelfDeaf)
			msg.set_self_mute(true);
	}

	if (msg.has_self_mute()) {
		uSource->bSelfMute = msg.self_mute();
		if (! uSource->bSelfMute) {
			msg.set_self_deaf(false);
			uSource->bSelfDeaf = false;
		}
	}

	if (msg.has_plugin_context()) {
		bNoBroadcast = true;
		uSource->ssContext = msg.plugin_context();
	}

	if (msg.has_plugin_identity()) {
		bNoBroadcast = true;
		uSource->qsIdentity = u8(msg.plugin_identity());
	}

	if (msg.has_comment()) {
		pDstServerUser->qsComment = u8(msg.comment());

		if (pDstServerUser->iId >= 0) {
			QMap<QString, QString> info;
			info.insert("comment", pDstServerUser->qsComment);
			setInfo(pDstServerUser->iId, info);
		}
	}

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());

		userEnterChannel(pDstServerUser, c);
		log(uSource, QString("Moved %1 to %2").arg(*pDstServerUser).arg(*c));
	}


	if (msg.has_mute() || msg.has_deaf() || msg.has_suppressed()) {
		if (msg.has_deaf()) {
			pDstServerUser->bDeaf = msg.deaf();
			if (pDstServerUser->bDeaf)
				msg.set_mute(true);
		}
		if (msg.has_mute()) {
			pDstServerUser->bMute = msg.mute();
			if (! pDstServerUser->bMute) {
				msg.set_deaf(false);
				pDstServerUser->bDeaf = false;
			}
		}
		if (msg.has_suppressed())
			pDstServerUser->bSuppressed = msg.suppressed();

		log(uSource, QString("Changed speak-state of %1 (%2 %3 %4)").arg(*pDstServerUser).arg(pDstServerUser->bMute).arg(pDstServerUser->bDeaf).arg(pDstServerUser->bSuppressed));
	}

	if (msg.has_user_id()) {
		QMap<QString, QString> info;

		info.insert("name", pDstServerUser->qsName);
		info.insert("certhash", pDstServerUser->qsHash);
		if (! pDstServerUser->qslEmail.isEmpty())
			info.insert("email", pDstServerUser->qslEmail.first());
		int id = registerUser(info);
		if (id > 0) {
			pDstServerUser->iId = id;
			msg.set_user_id(id);
		} else {
			msg.clear_user_id();
		}
	}

	if (! bNoBroadcast)
		sendAll(msg);

	emit userStateChanged(pDstServerUser);
}

void Server::msgUserRemove(ServerUser *uSource, MumbleProto::UserRemove &msg) {
	MSG_SETUP(User::Authenticated);
	VICTIM_SETUP;

	msg.set_actor(uSource->uiSession);

	bool ban = msg.has_ban() && msg.ban();

	Channel *c = qhChannels.value(0);
	QFlags<ChanACL::Perm> perm = ban ? ChanACL::Ban : (ChanACL::Ban|ChanACL::Kick);

	if ((pDstServerUser->iId ==0) || ! hasPermission(uSource, c, perm)) {
		PERM_DENIED(uSource, c, perm);
		return;
	}

	if (ban) {
		Ban b;
		b.haAddress = pDstServerUser->haAddress;
		b.iMask = 128;
		b.qsReason = u8(msg.reason());
		b.qsUsername = pDstServerUser->qsName;
		b.qsHash = pDstServerUser->qsHash;
		b.qdtStart = QDateTime::currentDateTime().toUTC();
		b.iDuration = 0;
		qlBans << b;
		saveBans();
	}

	sendAll(msg);
	if (ban)
		log(uSource, QString("Kickbanned %1 (%2)").arg(*pDstServerUser).arg(u8(msg.reason())));
	else
		log(uSource, QString("Kicked %1 (%2)").arg(*pDstServerUser).arg(u8(msg.reason())));
	pDstServerUser->disconnectSocket();
}

void Server::msgChannelState(ServerUser *uSource, MumbleProto::ChannelState &msg) {
	MSG_SETUP(User::Authenticated);

	Channel *c = NULL;
	Channel *p = NULL;

	if (msg.has_channel_id()) {
		c = qhChannels.value(msg.channel_id());
		if (! c)
			return;
	}

	if (msg.has_parent()) {
		p = qhChannels.value(msg.parent());
		if (! p)
			return;
	}

	msg.clear_links();

	QString qsName;
	QString qsDesc;
	if (msg.has_description())
		qsDesc = u8(msg.description());
	if (msg.has_name()) {
		qsName = u8(msg.name());

		if (! validateChannelName(qsName)) {
			PERM_DENIED_TYPE(ChannelName);
			return;
		}

		QRegExp re2("\\w");
		if (re2.indexIn(qsName) == -1) {
			PERM_DENIED_TYPE(ChannelName);
			return;
		}

		if (p || c) {
			Channel *cp = p ? p : c->cParent;
			foreach(Channel *sibling, cp->qlChannels) {
				if (sibling->qsName == qsName) {
					PERM_DENIED_TYPE(ChannelName);
					return;
				}
			}
		}
	}

	if (! c) {
		if (! p || qsName.isNull())
			return;

		ChanACL::Perm perm = msg.temporary() ? ChanACL::MakeTempChannel : ChanACL::MakeChannel;
		if (! hasPermission(uSource, p, perm) || ((uSource->iId < 0) && uSource->qsHash.isEmpty())) {
			PERM_DENIED(uSource, p, perm);
			return;
		}

		Channel *tmp = p;
		while (tmp) {
			if (tmp->bTemporary) {
				PERM_DENIED(uSource, tmp, perm);
				return;
			}
			tmp = tmp->cParent;
		}

		c = addChannel(p, qsName, msg.temporary());
		c->qsDesc = qsDesc;
		if (uSource->iId >= 0) {
			Group *g = new Group(c, "admin");
			g->qsAdd << uSource->iId;
		}

		if (! hasPermission(uSource, c, ChanACL::Write)) {
			ChanACL *a = new ChanACL(c);
			a->bApplyHere=true;
			a->bApplySubs=false;
			if (uSource->iId >= 0)
				a->iUserId=uSource->iId;
			else
				a->qsGroup=QLatin1Char('$') + uSource->qsHash;
			a->pDeny=ChanACL::None;
			a->pAllow=ChanACL::Write | ChanACL::Traverse;

			clearACLCache();
		}
		updateChannel(c);

		msg.set_channel_id(c->iId);
		log(uSource, QString("Added channel %1 under %2").arg(*c).arg(*p));
		emit channelCreated(c);
		sendAll(msg);

		if (c->bTemporary) {
			MumbleProto::UserState mpus;
			mpus.set_session(uSource->uiSession);
			mpus.set_channel_id(c->iId);
			sendAll(mpus);
			userEnterChannel(uSource, c);
		}
	} else {
		if (! qsName.isNull()) {
			if (! hasPermission(uSource, c, ChanACL::Write) || (c->iId == 0)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}
		if (! qsDesc.isNull()) {
			if (! hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}
		if (p) {
			if (p == c->cParent)
				return;

			Channel *ip = p;
			while (ip) {
				if (ip == c)
					return;
				ip = ip->cParent;
			}

			if (! hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}

			if (! hasPermission(uSource, p, ChanACL::MakeChannel)) {
				PERM_DENIED(uSource, p, ChanACL::MakeChannel);
				return;
			}

			QString name = qsName.isNull() ? c->qsName : qsName;

			foreach(Channel *sibling, p->qlChannels) {
				if (sibling->qsName == name) {
					PERM_DENIED_TYPE(ChannelName);
					return;
				}
			}
		}
		QList<Channel *> qlAdd;
		QList<Channel *> qlRemove;

		if (msg.links_add_size() || msg.links_remove_size()) {
			if (! hasPermission(uSource, c, ChanACL::LinkChannel)) {
				PERM_DENIED(uSource, c, ChanACL::LinkChannel);
				return;
			}
			if (msg.links_remove_size()) {
				for (int i=0;i < msg.links_remove_size(); ++i) {
					unsigned int link = msg.links_remove(i);
					Channel *l = qhChannels.value(link);
					if (! l)
						return;
					qlRemove << l;
				}
			}
			if (msg.links_add_size()) {
				for (int i=0;i < msg.links_add_size(); ++i) {
					unsigned int link = msg.links_add(i);
					Channel *l = qhChannels.value(link);
					if (! l)
						return;
					if (! hasPermission(uSource, l, ChanACL::LinkChannel)) {
						PERM_DENIED(uSource, l, ChanACL::LinkChannel);
						return;
					}
					qlAdd << l;
				}
			}
		}

		// All permission checks done -- the update is good.

		if (p) {
			log(uSource, QString("Moved channel %1 from %2 to %3").arg(*c).arg(* c->cParent).arg(*p));

			c->cParent->removeChannel(c);
			p->addChannel(c);
		}
		if (! qsName.isNull()) {
			log(uSource, QString("Renamed channel %1 to %2").arg(*c).arg(qsName));
			c->qsName = qsName;
		}
		if (! qsDesc.isNull())
			c->qsDesc = qsDesc;

		foreach(Channel *l, qlAdd) {
			addLink(c, l);
		}
		foreach(Channel *l, qlRemove) {
			removeLink(c, l);
		}

		updateChannel(c);
		emit channelStateChanged(c);
		sendAll(msg);
	}
}

void Server::msgChannelRemove(ServerUser *uSource, MumbleProto::ChannelRemove &msg) {
	MSG_SETUP(User::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	if (! hasPermission(uSource, c, ChanACL::Write) || (c->iId == 0)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	log(uSource, QString("Removed channel %1").arg(*c));

	removeChannel(c);
}

void Server::msgTextMessage(ServerUser *uSource, MumbleProto::TextMessage &msg) {
	MSG_SETUP(User::Authenticated);
	QMutexLocker qml(&qmCache);

	QSet<ServerUser *> users;
	QQueue<Channel *> q;

	msg.set_actor(uSource->uiSession);

	for (int i=0;i<msg.channel_id_size(); ++i) {
		unsigned int id = msg.channel_id(i);

		Channel *c = qhChannels.value(id);
		if (! c)
			return;

		if (! ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, acCache)) {
			PERM_DENIED(uSource, c, ChanACL::TextMessage);
			return;
		}

		foreach(User *p, c->qlUsers)
			users.insert(static_cast<ServerUser *>(p));
	}

	for (int i=0;i<msg.tree_id_size(); ++i) {
		unsigned int id = msg.tree_id(i);

		Channel *c = qhChannels.value(id);
		if (! c)
			return;

		if (! ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, acCache)) {
			PERM_DENIED(uSource, c, ChanACL::TextMessage);
			return;
		}

		q.enqueue(c);
	}

	while (! q.isEmpty()) {
		Channel *c = q.dequeue();
		if (ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, acCache)) {
			foreach(Channel *sub, c->qlChannels)
				q.enqueue(sub);
			foreach(User *p, c->qlUsers)
				users.insert(static_cast<ServerUser *>(p));
		}
	}

	for (int i=0;i < msg.session_size(); ++i) {
		unsigned int session = msg.session(i);
		ServerUser *u = qhUsers.value(session);
		if (! ChanACL::hasPermission(uSource, u->cChannel, ChanACL::TextMessage, acCache)) {
			PERM_DENIED(uSource, u->cChannel, ChanACL::TextMessage);
			return;
		}
		users.insert(u);
	}

	users.remove(uSource);

	foreach(ServerUser *u, users)
		sendMessage(u, msg);
}

void Server::msgACL(ServerUser *uSource, MumbleProto::ACL &msg) {
	MSG_SETUP(User::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	if (! hasPermission(uSource, c, ChanACL::Write) && !(c->cParent && hasPermission(uSource, c->cParent, ChanACL::Write))) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	if (msg.has_query() && msg.query()) {
		QStack<Channel *> chans;
		Channel *p;
		ChanACL *acl;

		QSet<int> qsId;

		msg.clear_groups();
		msg.clear_acls();
		msg.clear_query();
		msg.set_inherit_acls(c->bInheritACL);

		p = c;
		while (p) {
			chans.push(p);
			if ((p==c) || p->bInheritACL)
				p = p->cParent;
			else
				p = NULL;
		}

		while (! chans.isEmpty()) {
			p = chans.pop();
			foreach(acl, p->qlACL) {
				if ((p == c) || (acl->bApplySubs)) {
					MumbleProto::ACL_ChanACL *mpacl = msg.add_acls();

					mpacl->set_inherited(p != c);
					mpacl->set_apply_here(acl->bApplyHere);
					mpacl->set_apply_subs(acl->bApplySubs);
					if (acl->iUserId >= 0) {
						mpacl->set_user_id(acl->iUserId);
						qsId.insert(acl->iUserId);
					} else
						mpacl->set_group(u8(acl->qsGroup));
					mpacl->set_grant(acl->pAllow);
					mpacl->set_deny(acl->pDeny);
				}
			}
		}

		p = c->cParent;
		QSet<QString> allnames=Group::groupNames(c);
		foreach(const QString &name, allnames) {
			Group *g = c->qhGroups.value(name);
			Group *pg = p ? Group::getGroup(p, name) : NULL;

			MumbleProto::ACL_ChanGroup *group = msg.add_groups();
			group->set_name(u8(name));
			group->set_inherit(g ? g->bInherit : true);
			group->set_inheritable(g ? g->bInheritable : true);
			group->set_inherited((pg != NULL) && pg->bInheritable);
			if (g) {
				foreach(int id, g->qsAdd) {
					qsId.insert(id);
					group->add_add(id);
				}
				foreach(int id, g->qsRemove) {
					qsId.insert(id);
					group->add_remove(id);
				}
			}
			if (pg)
				foreach(int id, pg->members()) {
					qsId.insert(id);
					group->add_inherited_members(id);
				}
		}

		sendMessage(uSource, msg);

		MumbleProto::QueryUsers mpqu;
		foreach(int id, qsId) {
			QString uname=getUserName(id);
			if (! uname.isEmpty()) {
				mpqu.add_ids(id);
				mpqu.add_names(u8(uname));
			}
		}
		if (mpqu.ids_size())
			sendMessage(uSource, mpqu);
	} else {
		Group *g;
		ChanACL *a;

		QHash<QString, QSet<int> > hOldTemp;

		foreach(g, c->qhGroups) {
			hOldTemp.insert(g->qsName, g->qsTemporary);
			delete g;
		}

		foreach(a, c->qlACL)
			delete a;

		c->qhGroups.clear();
		c->qlACL.clear();

		c->bInheritACL = msg.inherit_acls();

		for (int i=0;i<msg.groups_size(); ++i) {
			const MumbleProto::ACL_ChanGroup &group = msg.groups(i);
			g = new Group(c, u8(group.name()));
			g->bInherit = group.inherit();
			g->bInheritable = group.inheritable();
			for (int j=0;j<group.add_size();++j)
				g->qsAdd << group.add(j);
			for (int j=0;j<group.remove_size();++j)
				g->qsRemove << group.remove(j);
			g->qsTemporary = hOldTemp.value(g->qsName);
		}

		for (int i=0;i<msg.acls_size(); ++i) {
			const MumbleProto::ACL_ChanACL &mpacl = msg.acls(i);
			a = new ChanACL(c);
			a->bApplyHere=mpacl.apply_here();
			a->bApplySubs=mpacl.apply_subs();
			if (mpacl.has_user_id())
				a->iUserId=mpacl.user_id();
			else
				a->qsGroup=u8(mpacl.group());
			a->pDeny=static_cast<ChanACL::Permissions>(mpacl.deny())  & ChanACL::All;
			a->pAllow=static_cast<ChanACL::Permissions>(mpacl.grant()) & ChanACL::All;
		}

		clearACLCache();

		if (! hasPermission(uSource, c, ChanACL::Write) && ((uSource->iId >= 0) || !uSource->qsHash.isEmpty())) {
			a = new ChanACL(c);
			a->bApplyHere=true;
			a->bApplySubs=false;
			if (uSource->iId >= 0)
				a->iUserId=uSource->iId;
			else
				a->qsGroup=QLatin1Char('$') + uSource->qsHash;
			a->iUserId=uSource->iId;
			a->pDeny=ChanACL::None;
			a->pAllow=ChanACL::Write | ChanACL::Traverse;

			clearACLCache();
		}

		updateChannel(c);
		log(uSource, QString("Updated ACL in channel %1").arg(*c));
	}
}

void Server::msgQueryUsers(ServerUser *uSource, MumbleProto::QueryUsers &msg) {
	MSG_SETUP(User::Authenticated);

	MumbleProto::QueryUsers reply;

	for (int i=0;i<msg.ids_size();++i) {
		int id = msg.ids(i);
		if (id >= 0) {
			const QString &name = getUserName(id);
			if (! name.isEmpty()) {
				reply.add_ids(id);
				reply.add_names(u8(name));
			}
		}
	}

	for (int i=0;i<msg.names_size();++i) {
		QString name = u8(msg.names(i));
		int id = getUserID(name);
		if (id >= 0) {
			name = getUserName(id);
			reply.add_ids(id);
			reply.add_names(u8(name));
		}
	}

	sendMessage(uSource, reply);
}

void Server::msgPing(ServerUser *uSource, MumbleProto::Ping &msg) {
	MSG_SETUP(User::Authenticated);
	CryptState &cs=uSource->csCrypt;

	cs.uiRemoteGood = msg.good();
	cs.uiRemoteLate = msg.late();
	cs.uiRemoteLost = msg.lost();
	cs.uiRemoteResync = msg.resync();

	uSource->dUDPPingAvg = msg.udp_ping_avg();
	uSource->dUDPPingVar = msg.udp_ping_var();
	uSource->uiUDPPackets = msg.udp_packets();
	uSource->dTCPPingAvg = msg.tcp_ping_avg();
	uSource->dTCPPingVar = msg.tcp_ping_var();
	uSource->uiTCPPackets = msg.tcp_packets();

	quint64 ts = msg.timestamp();

	msg.Clear();
	msg.set_timestamp(ts);
	msg.set_good(cs.uiGood);
	msg.set_late(cs.uiLate);
	msg.set_lost(cs.uiLost);
	msg.set_resync(cs.uiResync);

	sendMessage(uSource, msg);
}

void Server::msgCryptSetup(ServerUser *uSource, MumbleProto::CryptSetup &msg) {
	MSG_SETUP(User::Authenticated);
	if (! msg.has_client_nonce()) {
		log(uSource, "Requested crypt-nonce resync");
		msg.set_server_nonce(std::string(reinterpret_cast<const char *>(uSource->csCrypt.encrypt_iv), AES_BLOCK_SIZE));
		sendMessage(uSource, msg);
	} else {
		const std::string &str = msg.client_nonce();
		if (str.size()  == AES_BLOCK_SIZE) {
			uSource->csCrypt.uiResync++;
			memcpy(uSource->csCrypt.decrypt_iv, str.data(), AES_BLOCK_SIZE);
		}
	}
}

void Server::msgContextActionAdd(ServerUser *, MumbleProto::ContextActionAdd &) {
}

void Server::msgContextAction(ServerUser *uSource, MumbleProto::ContextAction &msg) {
	MSG_SETUP(User::Authenticated);

	unsigned int session = msg.has_session() ? msg.session() : 0;
	int id = msg.has_channel_id() ? static_cast<int>(msg.channel_id()) : -1;

	if (session && ! qhUsers.contains(session))
		return;
	if ((id >= 0) && ! qhChannels.contains(id))
		return;
	emit contextAction(uSource, u8(msg.action()), session, id);
}

void Server::msgVersion(ServerUser *uSource, MumbleProto::Version &msg) {
	if (msg.has_version())
		uSource->uiVersion=msg.version();
	if (msg.has_release())
		uSource->qsRelease = u8(msg.release());
	if (msg.has_os()) {
		uSource->qsOS = u8(msg.os());
		if (msg.has_os_version())
			uSource->qsOSVersion = u8(msg.os_version());
	}
}

void Server::msgUserList(ServerUser *uSource, MumbleProto::UserList &msg) {
	MSG_SETUP(User::Authenticated);

	if (! hasPermission(uSource, qhChannels.value(0), ChanACL::Register)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Register);
		return;
	}

	if (msg.users_size() == 0) {
		// Query mode.
		QMap<int, QString> users = getRegisteredUsers();
		QMap<int, QString>::const_iterator i;
		for (i = users.constBegin(); i != users.constEnd(); ++i) {
			::MumbleProto::UserList_User *u = msg.add_users();
			u->set_user_id(i.key());
			u->set_name(u8(i.value()));
		}
		sendMessage(uSource, msg);
	} else {
		for (int i=0;i < msg.users_size(); ++i) {
			const MumbleProto::UserList_User &u = msg.users(i);

			int id = u.user_id();
			if (id == 0)
				continue;

			if (! u.has_name()) {
				log(uSource, QString::fromLatin1("Unregistered user %1").arg(id));
				unregisterUser(id);
			} else {
				const QString &name = u8(u.name());
				log(uSource, QString::fromLatin1("Renamed user %1 to '%2'").arg(id).arg(name));

				QMap<QString, QString> info;
				info.insert("name", name);
				setInfo(id, info);
			}
		}
	}
}

void Server::msgVoiceTarget(ServerUser *uSource, MumbleProto::VoiceTarget &msg) {
	MSG_SETUP(User::Authenticated);

	int target = msg.id();
	if ((target < 1) || (target >= 0x1f))
		return;

	QWriteLocker lock(&qrwlUsers);

	uSource->qmTargetCache.remove(target);

	int count = msg.targets_size();
	if (count == 0) {
		uSource->qmTargets.remove(target);
	} else {
		WhisperTarget wt;
		for (int i=0;i<count;++i) {
			const MumbleProto::VoiceTarget_Target &t = msg.targets(i);
			for (int j=0;j<t.session_size(); ++j) {
				unsigned int s = t.session(j);
				if (qhUsers.contains(s))
					wt.qlSessions << s;
			}
			if (t.has_channel_id()) {
				unsigned int id = t.channel_id();
				if (qhChannels.contains(id)) {
					WhisperTarget::Channel wtc;
					wtc.iId = id;
					wtc.bChildren = t.children();
					wtc.bLinks = t.links();
					if (t.has_group())
						wtc.qsGroup = u8(t.group());
					wt.qlChannels << wtc;
				}
			}
		}
		if (wt.qlSessions.isEmpty() && wt.qlChannels.isEmpty())
			uSource->qmTargets.remove(target);
		else
			uSource->qmTargets.insert(target, wt);
	}
}
