// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "User.h"
#include "Channel.h"
#include "ACL.h"
#include "Group.h"
#include "Message.h"
#include "ServerDB.h"
#include "Connection.h"
#include "Server.h"
#include "ServerUser.h"
#include "Version.h"
#include "CryptState.h"

#include <QtCore/QStack>
#include <QtCore/QtEndian>

#define RATELIMIT(user) \
	if (user->leakyBucket.ratelimit(1)) { \
		return; \
	}

#define MSG_SETUP(st) \
	if (uSource->sState != st) { \
		return; \
	} \
	uSource->bwr.resetIdleSeconds()

#define MSG_SETUP_NO_UNIDLE(st) \
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

#define PERM_DENIED_FALLBACK(type,version,text) \
	{ \
		MumbleProto::PermissionDenied mppd; \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_##type); \
		if (uSource->uiVersion < version) \
			mppd.set_reason(u8(text)); \
		sendMessage(uSource, mppd); \
	}

#define PERM_DENIED_HASH(user) \
	{ \
		MumbleProto::PermissionDenied mppd; \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_MissingCertificate); \
		if (user) \
			mppd.set_session(user->uiSession); \
		sendMessage(uSource, mppd); \
	}

void Server::msgAuthenticate(ServerUser *uSource, MumbleProto::Authenticate &msg) {
	if ((msg.tokens_size() > 0) || (uSource->sState == ServerUser::Authenticated)) {
		QStringList qsl;
		for (int i=0;i<msg.tokens_size();++i)
			qsl << u8(msg.tokens(i));
		{
			QMutexLocker qml(&qmCache);
			uSource->qslAccessTokens = qsl;
		}
		clearACLCache(uSource);
	}
	MSG_SETUP(ServerUser::Connected);

	Channel *root = qhChannels.value(0);
	Channel *c;

	uSource->qsName = u8(msg.username());

	bool ok = false;
	bool nameok = validateUserName(uSource->qsName);
	QString pw = u8(msg.password());

	// Fetch ID and stored username.
	// Since this may call DBus, which may recall our dbus messages, this function needs
	// to support re-entrancy, and also to support the fact that sessions may go away.
	int id = authenticate(uSource->qsName, pw, uSource->uiSession, uSource->qslEmail, uSource->qsHash, uSource->bVerified, uSource->peerCertificateChain());

	uSource->iId = id >= 0 ? id : -1;

	QString reason;
	MumbleProto::Reject_RejectType rtType = MumbleProto::Reject_RejectType_None;

	if (id==-2 && ! nameok) {
		reason = "Invalid username";
		rtType = MumbleProto::Reject_RejectType_InvalidUsername;
	} else if (id==-1) {
		reason = "Wrong certificate or password for existing user";
		rtType = MumbleProto::Reject_RejectType_WrongUserPW;
	} else if (id==-2 && ! qsPassword.isEmpty() && qsPassword != pw) {
		reason = "Invalid server password";
		rtType = MumbleProto::Reject_RejectType_WrongServerPW;
	} else if (id==-3) {
		reason = "Your account information can not be verified currently. Please try again later";
		rtType = MumbleProto::Reject_RejectType_AuthenticatorFail;
	} else {
		ok = true;
	}

	ServerUser *uOld = NULL;
	foreach(ServerUser *u, qhUsers) {
		if (u == uSource)
			continue;
		if (((u->iId>=0) && (u->iId == uSource->iId)) ||
		        (u->qsName.toLower() == uSource->qsName.toLower())) {
			uOld = u;
			break;
		}
	}

	// Allow reuse of name from same IP
	if (ok && uOld && (uSource->iId == -1)) {
		if ((uOld->peerAddress() != uSource->peerAddress()) && (uSource->qsHash.isEmpty() || (uSource->qsHash != uOld->qsHash))) {
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

	if ((id != 0) && (uSource->qsHash.isEmpty() && bCertRequired)) {
		reason = QString::fromLatin1("A certificate is required to connect to this server");
		rtType = MumbleProto::Reject_RejectType_NoCertificate;
		ok = false;
	}

	Channel *lc;
	if (bRememberChan) {
		lc = qhChannels.value(readLastChannel(uSource->iId));
	} else {
		lc = qhChannels.value(iDefaultChan);
	}

	if (! lc || ! hasPermission(uSource, lc, ChanACL::Enter) || isChannelFull(lc, uSource)) {
		lc = qhChannels.value(iDefaultChan);
		if (! lc || ! hasPermission(uSource, lc, ChanACL::Enter) || isChannelFull(lc, uSource)) {
			lc = root;
			if (isChannelFull(lc, uSource)) {
				reason = QString::fromLatin1("Server channels are full");
				rtType = MumbleProto::Reject_RejectType_ServerFull;
				ok = false;
			}
		}
	}

	if (! ok) {
		log(uSource, QString("Rejected connection from %1: %2")
			.arg(addressToString(uSource->peerAddress(), uSource->peerPort()), reason));
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
		MumbleProto::UserRemove mpur;
		mpur.set_session(uOld->uiSession);
		mpur.set_reason("You connected to the server from another device");
		sendMessage(uOld, mpur);
		uOld->forceFlush();
		uOld->disconnectSocket(true);
	}

	// Setup UDP encryption
	{
		QMutexLocker l(&uSource->qmCrypt);

		uSource->csCrypt.genKey();

		MumbleProto::CryptSetup mpcrypt;
		mpcrypt.set_key(std::string(reinterpret_cast<const char *>(uSource->csCrypt.raw_key), AES_KEY_SIZE_BYTES));
		mpcrypt.set_server_nonce(std::string(reinterpret_cast<const char *>(uSource->csCrypt.encrypt_iv), AES_BLOCK_SIZE));
		mpcrypt.set_client_nonce(std::string(reinterpret_cast<const char *>(uSource->csCrypt.decrypt_iv), AES_BLOCK_SIZE));
		sendMessage(uSource, mpcrypt);
	}

	bool fake_celt_support = false;
	if (msg.celt_versions_size() > 0) {
		for (int i=0;i < msg.celt_versions_size(); ++i)
			uSource->qlCodecs.append(msg.celt_versions(i));
	} else {
		uSource->qlCodecs.append(static_cast<qint32>(0x8000000b));
		fake_celt_support = true;
	}
	uSource->bOpus = msg.opus();
	recheckCodecVersions(uSource);

	MumbleProto::CodecVersion mpcv;
	mpcv.set_alpha(iCodecAlpha);
	mpcv.set_beta(iCodecBeta);
	mpcv.set_prefer_alpha(bPreferAlpha);
	mpcv.set_opus(bOpus);
	sendMessage(uSource, mpcv);

	if (!bOpus && uSource->bOpus && fake_celt_support) {
		sendTextMessage(NULL, uSource, false, QLatin1String("<strong>WARNING:</strong> Your client doesn't support the CELT codec, you won't be able to talk to or hear most clients. Please make sure your client was built with CELT support."));
	}

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

		mpcs.set_position(c->iPosition);

		if ((uSource->uiVersion >= 0x010202) && ! c->qbaDescHash.isEmpty())
			mpcs.set_description_hash(blob(c->qbaDescHash));
		else if (! c->qsDesc.isEmpty())
			mpcs.set_description(u8(c->qsDesc));

		mpcs.set_max_users(c->uiMaxUsers);

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

	userEnterChannel(uSource, lc, mpus);

	{
		QWriteLocker wl(&qrwlVoiceThread);
		uSource->sState = ServerUser::Authenticated;
	}

	mpus.set_session(uSource->uiSession);
	mpus.set_name(u8(uSource->qsName));
	if (uSource->iId >= 0) {
		mpus.set_user_id(uSource->iId);

		hashAssign(uSource->qbaTexture, uSource->qbaTextureHash, getUserTexture(uSource->iId));

		if (! uSource->qbaTextureHash.isEmpty())
			mpus.set_texture_hash(blob(uSource->qbaTextureHash));
		else if (! uSource->qbaTexture.isEmpty())
			mpus.set_texture(blob(uSource->qbaTexture));

		const QMap<int, QString> &info = getRegistration(uSource->iId);
		if (info.contains(ServerDB::User_Comment)) {
			hashAssign(uSource->qsComment, uSource->qbaCommentHash, info.value(ServerDB::User_Comment));
			if (! uSource->qbaCommentHash.isEmpty())
				mpus.set_comment_hash(blob(uSource->qbaCommentHash));
			else if (! uSource->qsComment.isEmpty())
				mpus.set_comment(u8(uSource->qsComment));
		}
	}
	if (! uSource->qsHash.isEmpty())
		mpus.set_hash(u8(uSource->qsHash));
	if (uSource->cChannel->iId != 0)
		mpus.set_channel_id(uSource->cChannel->iId);

	sendAll(mpus, 0x010202);

	if ((uSource->qbaTexture.length() >= 4) && (qFromBigEndian<unsigned int>(reinterpret_cast<const unsigned char *>(uSource->qbaTexture.constData())) == 600 * 60 * 4))
		mpus.set_texture(blob(uSource->qbaTexture));
	if (! uSource->qsComment.isEmpty())
		mpus.set_comment(u8(uSource->qsComment));
	sendAll(mpus, ~ 0x010202);

	// Transmit other users profiles
	foreach(ServerUser *u, qhUsers) {
		if (u->sState != ServerUser::Authenticated)
			continue;

		if (u == uSource)
			continue;

		mpus.Clear();
		mpus.set_session(u->uiSession);
		mpus.set_name(u8(u->qsName));
		if (u->iId >= 0)
			mpus.set_user_id(u->iId);
		if (uSource->uiVersion >= 0x010202) {
			if (! u->qbaTextureHash.isEmpty())
				mpus.set_texture_hash(blob(u->qbaTextureHash));
			else if (! u->qbaTexture.isEmpty())
				mpus.set_texture(blob(u->qbaTexture));
		} else if ((uSource->qbaTexture.length() >= 4) && (qFromBigEndian<unsigned int>(reinterpret_cast<const unsigned char *>(uSource->qbaTexture.constData())) == 600 * 60 * 4)) {
			mpus.set_texture(blob(u->qbaTexture));
		}
		if (u->cChannel->iId != 0)
			mpus.set_channel_id(u->cChannel->iId);
		if (u->bDeaf)
			mpus.set_deaf(true);
		else if (u->bMute)
			mpus.set_mute(true);
		if (u->bSuppress)
			mpus.set_suppress(true);
		if (u->bPrioritySpeaker)
			mpus.set_priority_speaker(true);
		if (u->bRecording)
			mpus.set_recording(true);
		if (u->bSelfDeaf)
			mpus.set_self_deaf(true);
		else if (u->bSelfMute)
			mpus.set_self_mute(true);
		if ((uSource->uiVersion >= 0x010202) && ! u->qbaCommentHash.isEmpty())
			mpus.set_comment_hash(blob(u->qbaCommentHash));
		else if (! u->qsComment.isEmpty())
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
		QMutexLocker qml(&qmCache);
		ChanACL::hasPermission(uSource, root, ChanACL::Enter, &acCache);
		mpss.set_permissions(acCache.value(uSource)->value(root));
	}

	sendMessage(uSource, mpss);

	MumbleProto::ServerConfig mpsc;
	mpsc.set_allow_html(bAllowHTML);
	mpsc.set_message_length(iMaxTextMessageLength);
	mpsc.set_image_message_length(iMaxImageMessageLength);
	mpsc.set_max_users(iMaxUsers);
	sendMessage(uSource, mpsc);

	MumbleProto::SuggestConfig mpsug;
	if (! qvSuggestVersion.isNull())
		mpsug.set_version(qvSuggestVersion.toUInt());
	if (! qvSuggestPositional.isNull())
		mpsug.set_positional(qvSuggestPositional.toBool());
	if (! qvSuggestPushToTalk.isNull())
		mpsug.set_push_to_talk(qvSuggestPushToTalk.toBool());
	if (mpsug.ByteSize() > 0) {
		sendMessage(uSource, mpsug);
	}

	log(uSource, "Authenticated");

	emit userConnected(uSource);
}

void Server::msgBanList(ServerUser *uSource, MumbleProto::BanList &msg) {
	MSG_SETUP(ServerUser::Authenticated);

	QSet<Ban> previousBans, newBans;
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
		previousBans = qlBans.toSet();
		qlBans.clear();
		for (int i=0;i < msg.bans_size(); ++i) {
			const MumbleProto::BanList_BanEntry &be = msg.bans(i);

			Ban b;
			b.haAddress = be.address();
			b.iMask = be.mask();
			b.qsUsername = u8(be.name());
			b.qsHash = u8(be.hash());
			b.qsReason = u8(be.reason());
			if (be.has_start()) {
				b.qdtStart = QDateTime::fromString(u8(be.start()), Qt::ISODate);
				b.qdtStart.setTimeSpec(Qt::UTC);
			} else {
				b.qdtStart = QDateTime::currentDateTime().toUTC();
			}
			b.iDuration = be.duration();
			if (b.isValid()) {
				qlBans << b;
			}
		}
		newBans = qlBans.toSet();
		QSet<Ban> removed = previousBans - newBans;
		QSet<Ban> added = newBans - previousBans;
		foreach(const Ban &b, removed) {
			log(uSource, QString("Removed ban: %1").arg(b.toString()));
		}
		foreach(const Ban &b, added) {
			log(uSource, QString("New ban: %1").arg(b.toString()));
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
	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	const std::string &str = msg.packet();
	int len = static_cast<int>(str.length());
	if (len < 1)
		return;
	QReadLocker rl(&qrwlVoiceThread);
	processMsg(uSource, str.data(), len);
}

void Server::msgUserState(ServerUser *uSource, MumbleProto::UserState &msg) {
	MSG_SETUP(ServerUser::Authenticated);
	VICTIM_SETUP;

	Channel *root = qhChannels.value(0);

	/*
		First check all permissions involved
	*/
	if ((pDstServerUser->iId == 0) && (uSource->iId != 0)) {
		PERM_DENIED_TYPE(SuperUser);
		return;
	}

	msg.set_session(pDstServerUser->uiSession);
	msg.set_actor(uSource->uiSession);

	if (msg.has_name()) {
		PERM_DENIED_TYPE(UserName);
		return;
	}

	if (uSource == pDstServerUser) {
		RATELIMIT(uSource);
	}

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
		if (isChannelFull(c, uSource)) {
			PERM_DENIED_FALLBACK(ChannelFull, 0x010201, QLatin1String("Channel is full"));
			return;
		}
	}

	if (msg.has_mute() || msg.has_deaf() || msg.has_suppress() || msg.has_priority_speaker()) {
		if (pDstServerUser->iId == 0) {
			PERM_DENIED_TYPE(SuperUser);
			return;
		}
		if (uSource->cChannel->bTemporary) {
			PERM_DENIED_TYPE(TemporaryChannel);
			return;
		}
		if (! hasPermission(uSource, pDstServerUser->cChannel, ChanACL::MuteDeafen) || msg.suppress()) {
			PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::MuteDeafen);
			return;
		}
	}

	QString comment;

	if (msg.has_comment()) {
		bool changed = false;
		comment = u8(msg.comment());
		if (uSource != pDstServerUser) {
			if (! hasPermission(uSource, root, ChanACL::Move)) {
				PERM_DENIED(uSource, root, ChanACL::Move);
				return;
			}
			if (comment.length() > 0) {
				PERM_DENIED_TYPE(TextTooLong);
				return;
			}
		}


		if (! isTextAllowed(comment, changed)) {
			PERM_DENIED_TYPE(TextTooLong);
			return;
		}
		if (changed)
			msg.set_comment(u8(comment));
	}

	if (msg.has_texture()) {
		if (iMaxImageMessageLength > 0 && (msg.texture().length() > static_cast<unsigned int>(iMaxImageMessageLength))) {
			PERM_DENIED_TYPE(TextTooLong);
			return;
		}
		if (uSource != pDstServerUser) {
			if (! hasPermission(uSource, root, ChanACL::Move)) {
				PERM_DENIED(uSource, root, ChanACL::Move);
				return;
			}
			if (msg.texture().length() > 0) {
				PERM_DENIED_TYPE(TextTooLong);
				return;
			}
		}
	}


	if (msg.has_user_id()) {
		ChanACL::Perm p = (uSource == pDstServerUser) ? ChanACL::SelfRegister : ChanACL::Register;
		if ((pDstServerUser->iId >= 0) || ! hasPermission(uSource, root, p)) {
			PERM_DENIED(uSource, root, p);
			return;
		}
		if (pDstServerUser->qsHash.isEmpty()) {
			PERM_DENIED_HASH(pDstServerUser);
			return;
		}
	}

	// Prevent self-targeting state changes from being applied to others
	if ((pDstServerUser != uSource) && (msg.has_self_deaf() || msg.has_self_mute() || msg.has_plugin_context() || msg.has_plugin_identity() || msg.has_recording())) {
		return;
	}

	/*
		-------------------- Permission checks done. Now act --------------------
	*/
	bool bBroadcast = false;

	if (msg.has_texture()) {
		QByteArray qba = blob(msg.texture());
		if (pDstServerUser->iId > 0) {
			// For registered users store the texture we just received in the database
			if (! setTexture(pDstServerUser->iId, qba)) {
				return;
			}
		} else {
			// For unregistered users or SuperUser only get the hash
			hashAssign(pDstServerUser->qbaTexture, pDstServerUser->qbaTextureHash, qba);
		}

		// The texture will be sent out later in this function
		bBroadcast = true;
	}

	// Writing to bSelfMute, bSelfDeaf and ssContext
	// requires holding a write lock on qrwlVoiceThread.
	{
		QWriteLocker wl(&qrwlVoiceThread);

		if (msg.has_self_deaf()) {
			uSource->bSelfDeaf = msg.self_deaf();
			if (uSource->bSelfDeaf)
				msg.set_self_mute(true);
			bBroadcast = true;
		}

		if (msg.has_self_mute()) {
			uSource->bSelfMute = msg.self_mute();
			if (! uSource->bSelfMute) {
				msg.set_self_deaf(false);
				uSource->bSelfDeaf = false;
			}
			bBroadcast = true;
		}

		if (msg.has_plugin_context()) {
			uSource->ssContext = msg.plugin_context();

			// Make sure to clear this from the packet so we don't broadcast it
			msg.clear_plugin_context();
		}
	}

	if (msg.has_plugin_identity()) {
		uSource->qsIdentity = u8(msg.plugin_identity());
		// Make sure to clear this from the packet so we don't broadcast it
		msg.clear_plugin_identity();
	}

	if (! comment.isNull()) {
		hashAssign(pDstServerUser->qsComment, pDstServerUser->qbaCommentHash, comment);

		if (pDstServerUser->iId >= 0) {
			QMap<int, QString> info;
			info.insert(ServerDB::User_Comment, pDstServerUser->qsComment);
			setInfo(pDstServerUser->iId, info);
		}
		bBroadcast = true;
	}



	if (msg.has_mute() || msg.has_deaf() || msg.has_suppress() || msg.has_priority_speaker()) {
		// Writing to bDeaf, bMute and bSuppress requires
		// holding a write lock on qrwlVoiceThread.
		QWriteLocker wl(&qrwlVoiceThread);

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
		if (msg.has_suppress())
			pDstServerUser->bSuppress = msg.suppress();

		if (msg.has_priority_speaker())
			pDstServerUser->bPrioritySpeaker = msg.priority_speaker();

		log(uSource, QString("Changed speak-state of %1 (%2 %3 %4 %5)").arg(QString(*pDstServerUser),
		        QString::number(pDstServerUser->bMute),
		        QString::number(pDstServerUser->bDeaf),
		        QString::number(pDstServerUser->bSuppress),
		        QString::number(pDstServerUser->bPrioritySpeaker)));

		bBroadcast = true;
	}

	if (msg.has_recording() && (pDstServerUser->bRecording != msg.recording())) {
		pDstServerUser->bRecording = msg.recording();

		MumbleProto::TextMessage mptm;
		mptm.add_tree_id(0);
		if (pDstServerUser->bRecording)
			mptm.set_message(u8(QString(QLatin1String("User '%1' started recording")).arg(pDstServerUser->qsName)));
		else
			mptm.set_message(u8(QString(QLatin1String("User '%1' stopped recording")).arg(pDstServerUser->qsName)));

		sendAll(mptm, ~ 0x010203);

		bBroadcast = true;
	}

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());

		userEnterChannel(pDstServerUser, c, msg);
		log(uSource, QString("Moved %1 to %2").arg(QString(*pDstServerUser), QString(*c)));
		bBroadcast = true;
	}

	bool bDstAclChanged = false;
	if (msg.has_user_id()) {
		// Handle user (Self-)Registration
		QMap<int, QString> info;

		info.insert(ServerDB::User_Name, pDstServerUser->qsName);
		info.insert(ServerDB::User_Hash, pDstServerUser->qsHash);
		if (! pDstServerUser->qslEmail.isEmpty())
			info.insert(ServerDB::User_Email, pDstServerUser->qslEmail.first());
		int id = registerUser(info);
		if (id > 0) {
			pDstServerUser->iId = id;
			setLastChannel(pDstServerUser);
			msg.set_user_id(id);
			bDstAclChanged = true;
		} else {
			// Registration failed
			msg.clear_user_id();
		}
		bBroadcast = true;
	}

	if (bBroadcast) {
		// Texture handling for clients < 1.2.2.
		// Send the texture data in the message.
		if (msg.has_texture() && (pDstServerUser->qbaTexture.length() >= 4) && (qFromBigEndian<unsigned int>(reinterpret_cast<const unsigned char *>(pDstServerUser->qbaTexture.constData())) != 600 * 60 * 4)) {
			// This is a new style texture, don't send it because the client doesn't handle it correctly / crashes.
			msg.clear_texture();
			sendAll(msg, ~ 0x010202);
			msg.set_texture(blob(pDstServerUser->qbaTexture));
		} else {
			// This is an old style texture, empty texture or there was no texture in this packet,
			// send the message unchanged.
			sendAll(msg, ~ 0x010202);
		}

		// Texture / comment handling for clients >= 1.2.2.
		// Send only a hash of the texture / comment text. The client will request the actual data if necessary.
		if (msg.has_texture() && ! pDstServerUser->qbaTextureHash.isEmpty()) {
			msg.clear_texture();
			msg.set_texture_hash(blob(pDstServerUser->qbaTextureHash));
		}
		if (msg.has_comment() && ! pDstServerUser->qbaCommentHash.isEmpty()) {
			msg.clear_comment();
			msg.set_comment_hash(blob(pDstServerUser->qbaCommentHash));
		}

		sendAll(msg, 0x010202);

		if (bDstAclChanged)
			clearACLCache(pDstServerUser);
	}

	emit userStateChanged(pDstServerUser);
}

void Server::msgUserRemove(ServerUser *uSource, MumbleProto::UserRemove &msg) {
	MSG_SETUP(ServerUser::Authenticated);
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
		log(uSource, QString("Kickbanned %1 (%2)").arg(QString(*pDstServerUser), u8(msg.reason())));
	else
		log(uSource, QString("Kicked %1 (%2)").arg(QString(*pDstServerUser), u8(msg.reason())));
	pDstServerUser->disconnectSocket();
}

void Server::msgChannelState(ServerUser *uSource, MumbleProto::ChannelState &msg) {
	MSG_SETUP(ServerUser::Authenticated);

	Channel *c = NULL;
	Channel *p = NULL;

	// If this message relates to an existing channel check if the id is really valid
	if (msg.has_channel_id()) {
		c = qhChannels.value(msg.channel_id());
		if (! c)
			return;
	} else {
		RATELIMIT(uSource);
	}

	// Check if the parent exists
	if (msg.has_parent()) {
		p = qhChannels.value(msg.parent());
		if (! p)
			return;
	}

	msg.clear_links();

	QString qsName;
	QString qsDesc;
	if (msg.has_description()) {
		qsDesc = u8(msg.description());
		bool changed = false;
		if (! isTextAllowed(qsDesc, changed)) {
			PERM_DENIED_TYPE(TextTooLong);
			return;
		}
		if (changed)
			msg.set_description(u8(qsDesc));
	}

	if (msg.has_name()) {
		// If we are sent a channel name this means we want to create this channel so
		// check if the name is valid and not already in use.
		qsName = u8(msg.name());

		if (! validateChannelName(qsName)) {
			PERM_DENIED_TYPE(ChannelName);
			return;
		}

		if (qsName.length() == 0) {
			PERM_DENIED_TYPE(ChannelName);
			return;
		}

		if (p || (c && c->iId != 0)) {
			Channel *cp = p ? p : c->cParent;
			foreach(Channel *sibling, cp->qlChannels) {
				if (sibling->qsName == qsName) {
					PERM_DENIED_TYPE(ChannelName);
					return;
				}
			}
		}
	}

	if(p) {
		// Having a parent channel given means we either want to create
		// a channel in or move a channel into this parent.

		if (!canNest(p, c)) {
			PERM_DENIED_FALLBACK(NestingLimit, 0x010204, QLatin1String("Channel nesting limit reached"));
			return;
		}
	}

	if (! c) {
		// If we don't have a channel handle up to now we want to create a new channel
		// so check if the user has enough rights and we got everything we need.
		if (! p || qsName.isNull())
			return;

		if (iChannelCountLimit != 0 && qhChannels.count() >= iChannelCountLimit) {
			PERM_DENIED_FALLBACK(ChannelCountLimit, 0x010300, QLatin1String("Channel count limit reached"));
			return;
		}
		
		ChanACL::Perm perm = msg.temporary() ? ChanACL::MakeTempChannel : ChanACL::MakeChannel;
		if (! hasPermission(uSource, p, perm)) {
			PERM_DENIED(uSource, p, perm);
			return;
		}

		if ((uSource->iId < 0) && uSource->qsHash.isEmpty()) {
			PERM_DENIED_HASH(uSource);
			return;
		}

		if (p->bTemporary) {
			PERM_DENIED_TYPE(TemporaryChannel);
			return;
		}

		c = addChannel(p, qsName, msg.temporary(), msg.position(), msg.max_users());
		hashAssign(c->qsDesc, c->qbaDescHash, qsDesc);

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
		log(uSource, QString("Added channel %1 under %2").arg(QString(*c), QString(*p)));
		emit channelCreated(c);

		sendAll(msg, ~ 0x010202);
		if (! c->qbaDescHash.isEmpty()) {
			msg.clear_description();
			msg.set_description_hash(blob(c->qbaDescHash));
		}
		sendAll(msg, 0x010202);

		if (c->bTemporary) {
			// If a temporary channel has been created move the creator right in there
			MumbleProto::UserState mpus;
			mpus.set_session(uSource->uiSession);
			mpus.set_channel_id(c->iId);
			userEnterChannel(uSource, c, mpus);
			sendAll(mpus);
			emit userStateChanged(uSource);
		}
	} else {
		// The message is related to an existing channel c so check if the user is allowed to modify it
		// and perform the modifications
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
		if (msg.has_position()) {
			if (! hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}
		if (p) {
			// If we received a parent channel check if it differs from the old one and is not
			// Temporary. If that is the case check if the user has enough rights and if the
			// channel name is not used in the target location. Abort otherwise.
			if (p == c->cParent)
				return;

			Channel *ip = p;
			while (ip) {
				if (ip == c)
					return;
				ip = ip->cParent;
			}

			if (p->bTemporary) {
				PERM_DENIED_TYPE(TemporaryChannel);
				return;
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

		if (msg.has_max_users()) {
			if (! hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}

		// All permission checks done -- the update is good.

		if (p) {
			log(uSource, QString("Moved channel %1 from %2 to %3").arg(QString(*c),
			        QString(* c->cParent),
			        QString(*p)));

			{
				QWriteLocker wl(&qrwlVoiceThread);
				c->cParent->removeChannel(c);
				p->addChannel(c);
			}
		}
		if (! qsName.isNull()) {
			log(uSource, QString("Renamed channel %1 to %2").arg(QString(*c),
			        QString(qsName)));
			c->qsName = qsName;
		}
		if (! qsDesc.isNull())
			hashAssign(c->qsDesc, c->qbaDescHash, qsDesc);

		if (msg.has_position())
			c->iPosition = msg.position();

		foreach(Channel *l, qlAdd) {
			addLink(c, l);
		}
		foreach(Channel *l, qlRemove) {
			removeLink(c, l);
		}

		if (msg.has_max_users())
			c->uiMaxUsers = msg.max_users();

		updateChannel(c);
		emit channelStateChanged(c);

		sendAll(msg, ~ 0x010202);
		if (msg.has_description() && ! c->qbaDescHash.isEmpty()) {
			msg.clear_description();
			msg.set_description_hash(blob(c->qbaDescHash));
		}
		sendAll(msg, 0x010202);
	}
}

void Server::msgChannelRemove(ServerUser *uSource, MumbleProto::ChannelRemove &msg) {
	MSG_SETUP(ServerUser::Authenticated);

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
	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	TextMessage tm; // for signal userTextMessage

	QSet<ServerUser *> users;
	QQueue<Channel *> q;

	RATELIMIT(uSource);

	int res = 0;
	emit textMessageFilterSig(res, uSource, msg);
	switch (res) {
		// Accept
		case 0:
			// No-op.
			break;
		// Reject
		case 1:
			PERM_DENIED(uSource, uSource->cChannel, ChanACL::TextMessage);
			return;
		// Drop
		case 2:
			return;
	}

	QString text = u8(msg.message());
	bool changed = false;

	if (! isTextAllowed(text, changed)) {
		PERM_DENIED_TYPE(TextTooLong);
		return;
	}
	if (text.isEmpty())
		return;
	if (changed)
		msg.set_message(u8(text));

	tm.qsText = text;

	{ // Happy easter
		char m[29] = {0117, 0160, 0145, 0156, 040, 0164, 0150, 0145, 040, 0160, 0157, 0144, 040, 0142, 0141, 0171, 040, 0144, 0157, 0157, 0162, 0163, 054, 040, 0110, 0101, 0114, 056, 0};
		if (msg.channel_id_size() == 1 && msg.channel_id(0) == 0 && msg.message() == m) {
			PERM_DENIED_TYPE(H9K);
			return;
		}
	}

	msg.set_actor(uSource->uiSession);
	for (int i=0;i<msg.channel_id_size(); ++i) {
		unsigned int id = msg.channel_id(i);

		Channel *c = qhChannels.value(id);
		if (! c)
			return;

		if (! ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			PERM_DENIED(uSource, c, ChanACL::TextMessage);
			return;
		}

		foreach(User *p, c->qlUsers)
			users.insert(static_cast<ServerUser *>(p));

		tm.qlChannels.append(id);
	}

	for (int i=0;i<msg.tree_id_size(); ++i) {
		unsigned int id = msg.tree_id(i);

		Channel *c = qhChannels.value(id);
		if (! c)
			return;

		if (! ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			PERM_DENIED(uSource, c, ChanACL::TextMessage);
			return;
		}

		q.enqueue(c);

		tm.qlTrees.append(id);
	}

	while (! q.isEmpty()) {
		Channel *c = q.dequeue();
		if (ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			foreach(Channel *sub, c->qlChannels)
				q.enqueue(sub);
			foreach(User *p, c->qlUsers)
				users.insert(static_cast<ServerUser *>(p));
		}
	}

	for (int i=0;i < msg.session_size(); ++i) {
		unsigned int session = msg.session(i);
		ServerUser *u = qhUsers.value(session);
		if (u) {
			if (! ChanACL::hasPermission(uSource, u->cChannel, ChanACL::TextMessage, &acCache)) {
				PERM_DENIED(uSource, u->cChannel, ChanACL::TextMessage);
				return;
			}
			users.insert(u);
		}

		tm.qlSessions.append(session);
	}

	users.remove(uSource);

	foreach(ServerUser *u, users)
		sendMessage(u, msg);

	emit userTextMessage(uSource, tm);
}

void Server::msgACL(ServerUser *uSource, MumbleProto::ACL &msg) {
	MSG_SETUP(ServerUser::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	if (! hasPermission(uSource, c, ChanACL::Write) && !(c->cParent && hasPermission(uSource, c->cParent, ChanACL::Write))) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	RATELIMIT(uSource);

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
			if (pg) {
				foreach(int id, pg->members()) {
					qsId.insert(id);
					group->add_inherited_members(id);
				}
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

		{
			QWriteLocker wl(&qrwlVoiceThread);

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

			for (int i = 0; i < msg.groups_size(); ++i) {
				const MumbleProto::ACL_ChanGroup &group = msg.groups(i);
				g = new Group(c, u8(group.name()));
				g->bInherit = group.inherit();
				g->bInheritable = group.inheritable();
				for (int j = 0; j < group.add_size(); ++j)
					if (!getUserName(group.add(j)).isEmpty())
						g->qsAdd << group.add(j);
				for (int j = 0; j < group.remove_size(); ++j)
					if (!getUserName(group.remove(j)).isEmpty())
						g->qsRemove << group.remove(j);
				g->qsTemporary = hOldTemp.value(g->qsName);
			}

			for (int i = 0; i < msg.acls_size(); ++i) {
				const MumbleProto::ACL_ChanACL &mpacl = msg.acls(i);
				if (mpacl.has_user_id() && getUserName(mpacl.user_id()).isEmpty())
					continue;

				a = new ChanACL(c);
				a->bApplyHere = mpacl.apply_here();
				a->bApplySubs = mpacl.apply_subs();
				if (mpacl.has_user_id())
					a->iUserId = mpacl.user_id();
				else
					a->qsGroup = u8(mpacl.group());
				a->pDeny = static_cast<ChanACL::Permissions>(mpacl.deny()) & ChanACL::All;
				a->pAllow = static_cast<ChanACL::Permissions>(mpacl.grant()) & ChanACL::All;
			}
		}

		clearACLCache();

		if (! hasPermission(uSource, c, ChanACL::Write) && ((uSource->iId >= 0) || !uSource->qsHash.isEmpty())) {
			{
				QWriteLocker wl(&qrwlVoiceThread);

				a = new ChanACL(c);
				a->bApplyHere = true;
				a->bApplySubs = false;
				if (uSource->iId >= 0)
					a->iUserId = uSource->iId;
				else
					a->qsGroup = QLatin1Char('$') + uSource->qsHash;
				a->iUserId = uSource->iId;
				a->pDeny = ChanACL::None;
				a->pAllow = ChanACL::Write | ChanACL::Traverse;
			}

			clearACLCache();
		}


		updateChannel(c);
		log(uSource, QString("Updated ACL in channel %1").arg(*c));
	}
}

void Server::msgQueryUsers(ServerUser *uSource, MumbleProto::QueryUsers &msg) {
	MSG_SETUP(ServerUser::Authenticated);

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
	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	QMutexLocker l(&uSource->qmCrypt);

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
	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	QMutexLocker l(&uSource->qmCrypt);

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

void Server::msgContextActionModify(ServerUser *, MumbleProto::ContextActionModify &) {
}

void Server::msgContextAction(ServerUser *uSource, MumbleProto::ContextAction &msg) {
	MSG_SETUP(ServerUser::Authenticated);

	unsigned int session = msg.has_session() ? msg.session() : 0;
	int id = msg.has_channel_id() ? static_cast<int>(msg.channel_id()) : -1;

	if (session && ! qhUsers.contains(session))
		return;
	if ((id >= 0) && ! qhChannels.contains(id))
		return;
	emit contextAction(uSource, u8(msg.action()), session, id);
}

void Server::msgVersion(ServerUser *uSource, MumbleProto::Version &msg) {
	RATELIMIT(uSource);

	if (msg.has_version())
		uSource->uiVersion=msg.version();
	if (msg.has_release())
		uSource->qsRelease = u8(msg.release());
	if (msg.has_os()) {
		uSource->qsOS = u8(msg.os());
		if (msg.has_os_version())
			uSource->qsOSVersion = u8(msg.os_version());
	}

	log(uSource, QString("Client version %1 (%2: %3)").arg(MumbleVersion::toString(uSource->uiVersion)).arg(uSource->qsOS).arg(uSource->qsRelease));
}

void Server::msgUserList(ServerUser *uSource, MumbleProto::UserList &msg) {
	MSG_SETUP(ServerUser::Authenticated);

	// The register permission is required on the root channel to be allowed to
	// view the registered users.
	if (! hasPermission(uSource, qhChannels.value(0), ChanACL::Register)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Register);
		return;
	}

	if (msg.users_size() == 0) {
		// Query mode.
		QList<UserInfo> users = getRegisteredUsersEx();
		QList<UserInfo>::const_iterator it = users.constBegin();
		for (; it != users.constEnd(); ++it) {
			// Skip the SuperUser
			if (it->user_id > 0) {
				::MumbleProto::UserList_User *user = msg.add_users();
				user->set_user_id(it->user_id);
				user->set_name(u8(it->name));
				if (it->last_channel) {
					user->set_last_channel(*it->last_channel);
				}
				user->set_last_seen(u8(it->last_active.toString(Qt::ISODate)));
			}
		}
		sendMessage(uSource, msg);
	} else {
		// Update mode
		for (int i=0; i < msg.users_size(); ++i) {
			const MumbleProto::UserList_User &user = msg.users(i);

			int id = user.user_id();
			if (id == 0)
				continue;

			if (! user.has_name()) {
				log(uSource, QString::fromLatin1("Unregistered user %1").arg(id));
				unregisterUser(id);
			} else {
				const QString &name = u8(user.name());
				if (validateUserName(name)) {
					log(uSource, QString::fromLatin1("Renamed user %1 to '%2'").arg(QString::number(id), name));

					QMap<int, QString> info;
					info.insert(ServerDB::User_Name, name);
					setInfo(id, info);

					MumbleProto::UserState mpus;
					foreach(ServerUser *serverUser, qhUsers) {
						if (serverUser->iId == id) {
							serverUser->qsName = name;
							mpus.set_session(serverUser->uiSession);
							break;
						}
					}
					if (mpus.has_session()) {
						mpus.set_actor(uSource->uiSession);
						mpus.set_name(u8(name));
						sendAll(mpus);
					}
				} else {
					MumbleProto::PermissionDenied mppd;
					mppd.set_type(MumbleProto::PermissionDenied_DenyType_UserName);
					if (uSource->uiVersion < 0x010201)
						mppd.set_reason(u8(QString::fromLatin1("%1 is not a valid username").arg(name)));
					else
						mppd.set_name(u8(name));
					sendMessage(uSource, mppd);
				}
			}
		}
	}
}

void Server::msgVoiceTarget(ServerUser *uSource, MumbleProto::VoiceTarget &msg) {
	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	int target = msg.id();
	if ((target < 1) || (target >= 0x1f))
		return;

	QWriteLocker lock(&qrwlVoiceThread);

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

void Server::msgPermissionQuery(ServerUser *uSource, MumbleProto::PermissionQuery &msg) {
	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	sendClientPermission(uSource, c, true);
}

void Server::msgCodecVersion(ServerUser *, MumbleProto::CodecVersion &) {
}

void Server::msgUserStats(ServerUser*uSource, MumbleProto::UserStats &msg) {
	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);
	VICTIM_SETUP;
	const BandwidthRecord &bwr = pDstServerUser->bwr;
	const QList<QSslCertificate> &certs = pDstServerUser->peerCertificateChain();

	bool extend = (uSource == pDstServerUser) || hasPermission(uSource, qhChannels.value(0), ChanACL::Register);

	if (! extend && ! hasPermission(uSource, pDstServerUser->cChannel, ChanACL::Enter)) {
		PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::Enter);
		return;
	}

	bool details = extend;
	bool local = extend || (pDstServerUser->cChannel == uSource->cChannel);

	if (msg.stats_only())
		details = false;

	msg.Clear();
	msg.set_session(pDstServerUser->uiSession);

	if (details) {
		foreach(const QSslCertificate &cert, certs) {
			const QByteArray &der = cert.toDer();
			msg.add_certificates(blob(der));
		}
		msg.set_strong_certificate(pDstServerUser->bVerified);
	}

	if (local) {
		MumbleProto::UserStats_Stats *mpusss;

		QMutexLocker l(&pDstServerUser->qmCrypt);
		const CryptState &cs = pDstServerUser->csCrypt;

		mpusss = msg.mutable_from_client();
		mpusss->set_good(cs.uiGood);
		mpusss->set_late(cs.uiLate);
		mpusss->set_lost(cs.uiLost);
		mpusss->set_resync(cs.uiResync);

		mpusss = msg.mutable_from_server();
		mpusss->set_good(cs.uiRemoteGood);
		mpusss->set_late(cs.uiRemoteLate);
		mpusss->set_lost(cs.uiRemoteLost);
		mpusss->set_resync(cs.uiRemoteResync);
	}

	msg.set_udp_packets(pDstServerUser->uiUDPPackets);
	msg.set_tcp_packets(pDstServerUser->uiTCPPackets);
	msg.set_udp_ping_avg(pDstServerUser->dUDPPingAvg);
	msg.set_udp_ping_var(pDstServerUser->dUDPPingVar);
	msg.set_tcp_ping_avg(pDstServerUser->dTCPPingAvg);
	msg.set_tcp_ping_var(pDstServerUser->dTCPPingVar);

	if (details) {
		MumbleProto::Version *mpv;

		mpv = msg.mutable_version();
		if (pDstServerUser->uiVersion)
			mpv->set_version(pDstServerUser->uiVersion);
		if (! pDstServerUser->qsRelease.isEmpty())
			mpv->set_release(u8(pDstServerUser->qsRelease));
		if (! pDstServerUser->qsOS.isEmpty()) {
			mpv->set_os(u8(pDstServerUser->qsOS));
			if (! pDstServerUser->qsOSVersion.isEmpty())
				mpv->set_os_version(u8(pDstServerUser->qsOSVersion));
		}

		foreach(int v, pDstServerUser->qlCodecs)
			msg.add_celt_versions(v);
		msg.set_opus(pDstServerUser->bOpus);

		msg.set_address(pDstServerUser->haAddress.toStdString());
	}

	if (local)
		msg.set_bandwidth(bwr.bandwidth());
	msg.set_onlinesecs(bwr.onlineSeconds());
	if (local)
		msg.set_idlesecs(bwr.idleSeconds());

	sendMessage(uSource, msg);
}

void Server::msgRequestBlob(ServerUser *uSource, MumbleProto::RequestBlob &msg) {
	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	int ntextures = msg.session_texture_size();
	int ncomments = msg.session_comment_size();
	int ndescriptions = msg.channel_description_size();

	if (ndescriptions) {
		MumbleProto::ChannelState mpcs;
		for (int i=0;i<ndescriptions;++i) {
			int id = msg.channel_description(i);
			Channel *c = qhChannels.value(id);
			if (c && ! c->qsDesc.isEmpty()) {
				mpcs.set_channel_id(id);
				mpcs.set_description(u8(c->qsDesc));
				sendMessage(uSource, mpcs);
			}
		}
	}
	if (ntextures || ncomments) {
		MumbleProto::UserState mpus;
		for (int i=0;i<ntextures;++i) {
			int session = msg.session_texture(i);
			ServerUser *su = qhUsers.value(session);
			if (su && ! su->qbaTexture.isEmpty()) {
				mpus.set_session(session);
				mpus.set_texture(blob(su->qbaTexture));
				sendMessage(uSource, mpus);
			}
		}
		if (ntextures)
			mpus.clear_texture();
		for (int i=0;i<ncomments;++i) {
			int session = msg.session_comment(i);
			ServerUser *su = qhUsers.value(session);
			if (su && ! su->qsComment.isEmpty()) {
				mpus.set_session(session);
				mpus.set_comment(u8(su->qsComment));
				sendMessage(uSource, mpus);
			}
		}
	}
}

void Server::msgServerConfig(ServerUser *, MumbleProto::ServerConfig &) {
}

void Server::msgSuggestConfig(ServerUser *, MumbleProto::SuggestConfig &) {
}
