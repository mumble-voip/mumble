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

#include "Player.h"
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
	User *pDstUser = uSource; \
	if (msg.has_session()) \
		pDstUser = qhUsers.value(msg.session()); \
	if (! pDstUser) \
		return; \
	Q_UNUSED(pDstUser)

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

void Server::msgAuthenticate(User *uSource, MumbleProto::Authenticate &msg) {
	MSG_SETUP(Player::Connected);

	Channel *root = qhChannels.value(0);
	Channel *c;

	uSource->qsName = u8(msg.username());

	bool ok = false;
	bool nameok = validatePlayerName(uSource->qsName);
	QString pw = u8(msg.password());

	// Fetch ID and stored username.
	// Since this may call DBus, which may recall our dbus messages, this function needs
	// to support re-entrancy, and also to support the fact that sessions may go away.
	int id = authenticate(uSource->qsName, pw, uSource->qslEmail, uSource->qsHash, uSource->bVerified);

	uSource->iId = id >= 0 ? id : -1;

	QString reason;
	MumbleProto::Reject_RejectType rtType = MumbleProto::Reject_RejectType_None;

	if (! nameok && (uSource->iId == -1)) {
		reason = "Invalid Username";
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

	User *uOld = NULL;
	foreach(User *u, qhUsers) {
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
			reason = "Playername already in use";
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

	playerEnterChannel(uSource, lc, true);

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

	// Transmit player profile
	MumbleProto::UserState mpus;

	uSource->sState = Player::Authenticated;
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

	// Transmit other players profiles
	foreach(User *u, qhUsers) {
		if (u->sState != Player::Authenticated)
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

	emit playerConnected(uSource);
	playerEnterChannel(uSource, lc, false);
}

void Server::msgBanList(User *uSource, MumbleProto::BanList &msg) {
	MSG_SETUP(Player::Authenticated);

	typedef QPair<quint32, int> ban;
	if (! hasPermission(uSource, qhChannels.value(0), ChanACL::Ban)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Ban);
		return;
	}
	if (msg.query()) {
		msg.clear_query();
		msg.clear_bans();
		foreach(const ban &b, qlBans) {
			MumbleProto::BanList_BanEntry *be = msg.add_bans();
			char buff[4];
			buff[0] = static_cast<char>((b.first >> 24) & 0xFF);
			buff[1] = static_cast<char>((b.first >> 16) & 0xFF);
			buff[2] = static_cast<char>((b.first >> 8) & 0xFF);
			buff[3] = static_cast<char>((b.first >> 0) & 0xFF);
			be->set_address(std::string(buff, 4));
			be->set_mask(b.second);
		}
		sendMessage(uSource, msg);
	} else {
		qlBans.clear();
		for (int i=0;i < msg.bans_size(); ++i) {
			const MumbleProto::BanList_BanEntry &be = msg.bans(i);
			quint32 v = 0;
			std::string s = be.address();
			if (s.length() == 4) {
				const char *data = s.data();
				v += (data[0] << 24);
				v += (data[1] << 16);
				v += (data[2] << 8);
				v += (data[3] << 0);
				qlBans << ban(v, be.mask());
			}
		}
		saveBans();
		log(uSource, "Updated banlist");
	}
}

void Server::msgReject(User *, MumbleProto::Reject &) {
}

void Server::msgServerSync(User *, MumbleProto::ServerSync &) {
}

void Server::msgPermissionDenied(User *, MumbleProto::PermissionDenied &) {
}

void Server::msgUDPTunnel(User *uSource, MumbleProto::UDPTunnel &msg) {
	MSG_SETUP(Player::Authenticated);

	const std::string &str = msg.packet();
	int len = static_cast<int>(str.length());
	if (len < 1)
		return;
	QReadLocker rl(&qrwlUsers);
	processMsg(uSource, str.data(), len);
}

void Server::msgUserState(User *uSource, MumbleProto::UserState &msg) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	bool bNoBroadcast = false;
	Channel *root = qhChannels.value(0);

	if (pDstUser->iId == 0) {
		PERM_DENIED_TYPE(SuperUser);
		return;
	}

	msg.set_session(pDstUser->uiSession);
	msg.set_actor(uSource->uiSession);

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());
		if (!c || (c == pDstUser->cChannel))
			return;

		if ((uSource != pDstUser) && (! hasPermission(uSource, pDstUser->cChannel, ChanACL::Move))) {
			PERM_DENIED(uSource, pDstUser->cChannel, ChanACL::Move);
			return;
		}

		if (! hasPermission(uSource, c, ChanACL::Move) && ! hasPermission(pDstUser, c, ChanACL::Enter)) {
			PERM_DENIED(pDstUser, c, ChanACL::Enter);
			return;
		}
	}

	if (msg.has_mute() || msg.has_deaf() || msg.has_suppressed()) {
		if (! hasPermission(uSource, pDstUser->cChannel, ChanACL::MuteDeafen) || msg.suppressed()) {
			PERM_DENIED(uSource, pDstUser->cChannel, ChanACL::MuteDeafen);
			return;
		}
	}

	if (msg.has_comment() && (uSource != pDstUser)) {
		if (! hasPermission(uSource, root, ChanACL::Move)) {
			PERM_DENIED(uSource, root, ChanACL::Move);
			return;
		}
	}

	if (msg.has_user_id()) {
		if ((pDstUser->iId >= 0) || (pDstUser->qsHash.isEmpty()) || ! hasPermission(uSource, root, ChanACL::Register)) {
			PERM_DENIED(uSource, root, ChanACL::Register);
			return;
		}
	}

	if ((pDstUser != uSource) && (msg.has_self_deaf() || msg.has_self_mute() || msg.has_texture() || msg.has_plugin_context() || msg.has_plugin_identity()))
		return;

	// Permission checks done. Now enact this.

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
		pDstUser->qsComment = u8(msg.comment());

		if (pDstUser->iId >= 0) {
			QMap<QString, QString> info;
			info.insert("comment", pDstUser->qsComment);
			setInfo(pDstUser->iId, info);
		}
	}

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());

		playerEnterChannel(pDstUser, c);
		log(uSource, QString("Moved %1 to %2").arg(*pDstUser).arg(*c));
	}


	if (msg.has_mute() || msg.has_deaf() || msg.has_suppressed()) {
		if (msg.has_deaf()) {
			pDstUser->bDeaf = msg.deaf();
			if (pDstUser->bDeaf)
				msg.set_mute(true);
		}
		if (msg.has_mute()) {
			pDstUser->bMute = msg.mute();
			if (! pDstUser->bMute) {
				msg.set_deaf(false);
				pDstUser->bDeaf = false;
			}
		}
		if (msg.has_suppressed())
			pDstUser->bSuppressed = msg.suppressed();

		log(uSource, QString("Changed speak-state of %1 (%2 %3 %4)").arg(*pDstUser).arg(pDstUser->bMute).arg(pDstUser->bDeaf).arg(pDstUser->bSuppressed));
	}

	if (msg.has_user_id()) {
		QMap<QString, QString> info;

		info.insert("name", pDstUser->qsName);
		info.insert("certhash", pDstUser->qsHash);
		if (! pDstUser->qslEmail.isEmpty())
			info.insert("email", pDstUser->qslEmail.first());
		int id = registerPlayer(info);
		if (id > 0) {
			pDstUser->iId = id;
			msg.set_user_id(id);
		} else {
			msg.clear_user_id();
		}
	}

	if (! bNoBroadcast)
		sendAll(msg);

	emit playerStateChanged(pDstUser);
}

void Server::msgUserRemove(User *uSource, MumbleProto::UserRemove &msg) {
	MSG_SETUP(Player::Authenticated);
	VICTIM_SETUP;

	msg.set_actor(uSource->uiSession);

	bool ban = msg.has_ban() && msg.ban();

	Channel *c = qhChannels.value(0);
	QFlags<ChanACL::Perm> perm = ban ? ChanACL::Ban : (ChanACL::Ban|ChanACL::Kick);

	if ((pDstUser->iId ==0) || ! hasPermission(uSource, c, perm)) {
		PERM_DENIED(uSource, c, perm);
		return;
	}

	if (ban) {
		QHostAddress adr = pDstUser->peerAddress();
		quint32 base = adr.toIPv4Address();
		qlBans << QPair<quint32,int>(base, 32);
		saveBans();
	}

	sendAll(msg);
	if (ban)
		log(uSource, QString("Kickbanned %1 (%2)").arg(*pDstUser).arg(u8(msg.reason())));
	else
		log(uSource, QString("Kicked %1 (%2)").arg(*pDstUser).arg(u8(msg.reason())));
	pDstUser->disconnectSocket();
}

void Server::msgChannelState(User *uSource, MumbleProto::ChannelState &msg) {
	MSG_SETUP(Player::Authenticated);

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
		if (! hasPermission(uSource, p, ChanACL::MakeChannel)) {
			PERM_DENIED(uSource, p, ChanACL::MakeChannel);
			return;
		}

		c = addChannel(p, qsName);
		c->qsDesc = qsDesc;
		if (uSource->iId >= 0) {
			Group *g = new Group(c, "admin");
			g->qsAdd << uSource->iId;
		}
		updateChannel(c);

		msg.set_channel_id(c->iId);
		log(uSource, QString("Added channel %1 under %2").arg(*c).arg(*p));
		emit channelCreated(c);
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
			c->link(l);
			addLink(c, l);
		}
		foreach(Channel *l, qlRemove) {
			c->unlink(l);
			removeLink(c, l);
		}

		updateChannel(c);
		emit channelStateChanged(c);
	}

	sendAll(msg);
}

void Server::msgChannelRemove(User *uSource, MumbleProto::ChannelRemove &msg) {
	MSG_SETUP(Player::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	if (! hasPermission(uSource, c, ChanACL::Write) || (c->iId == 0)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	log(uSource, QString("Removed channel %1").arg(*c));

	removeChannel(c, uSource);
}

void Server::msgTextMessage(User *uSource, MumbleProto::TextMessage &msg) {
	MSG_SETUP(Player::Authenticated);
	QMutexLocker qml(&qmCache);

	QSet<User *> users;
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

		foreach(Player *p, c->qlPlayers)
			users.insert(static_cast<User *>(p));
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
			foreach(Player *p, c->qlPlayers)
				users.insert(static_cast<User *>(p));
		}
	}

	for (int i=0;i < msg.session_size(); ++i) {
		unsigned int session = msg.session(i);
		User *u = qhUsers.value(session);
		if (! ChanACL::hasPermission(uSource, u->cChannel, ChanACL::TextMessage, acCache)) {
			PERM_DENIED(uSource, u->cChannel, ChanACL::TextMessage);
			return;
		}
		users.insert(u);
	}

	users.remove(uSource);

	foreach(User *u, users)
		sendMessage(u, msg);
}

void Server::msgACL(User *uSource, MumbleProto::ACL &msg) {
	MSG_SETUP(Player::Authenticated);

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
					if (acl->iPlayerId >= 0) {
						mpacl->set_user_id(acl->iPlayerId);
						qsId.insert(acl->iPlayerId);
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
				a->iPlayerId=mpacl.user_id();
			else
				a->qsGroup=u8(mpacl.group());
			a->pDeny=static_cast<ChanACL::Permissions>(mpacl.deny())  & ChanACL::All;
			a->pAllow=static_cast<ChanACL::Permissions>(mpacl.grant()) & ChanACL::All;
		}

		clearACLCache();

		if (! hasPermission(uSource, c, ChanACL::Write)) {
			a = new ChanACL(c);
			a->bApplyHere=true;
			a->bApplySubs=false;
			a->iPlayerId=uSource->iId;
			a->pDeny=ChanACL::None;
			a->pAllow=ChanACL::Write | ChanACL::Traverse;

			clearACLCache();
		}

		updateChannel(c);
		log(uSource, QString("Updated ACL in channel %1").arg(*c));
	}
}

void Server::msgQueryUsers(User *uSource, MumbleProto::QueryUsers &msg) {
	MSG_SETUP(Player::Authenticated);

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

void Server::msgPing(User *uSource, MumbleProto::Ping &msg) {
	MSG_SETUP(Player::Authenticated);
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

void Server::msgCryptSetup(User *uSource, MumbleProto::CryptSetup &msg) {
	MSG_SETUP(Player::Authenticated);
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

void Server::msgContextActionAdd(User *, MumbleProto::ContextActionAdd &) {
}

void Server::msgContextAction(User *uSource, MumbleProto::ContextAction &msg) {
	MSG_SETUP(Player::Authenticated);

	unsigned int session = msg.has_session() ? msg.session() : 0;
	int id = msg.has_channel_id() ? static_cast<int>(msg.channel_id()) : -1;

	if (session && ! qhUsers.contains(session))
		return;
	if ((id >= 0) && ! qhChannels.contains(id))
		return;
	emit contextAction(uSource, u8(msg.action()), session, id);
}

void Server::msgVersion(User *uSource, MumbleProto::Version &msg) {
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
