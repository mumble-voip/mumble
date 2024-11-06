// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ACL.h"
#include "Channel.h"
#include "ChannelListenerManager.h"
#include "ClientType.h"
#include "Connection.h"
#include "Group.h"
#include "Meta.h"
#include "MumbleConstants.h"
#include "ProtoUtils.h"
#include "QtUtils.h"
#include "Server.h"
#include "ServerDB.h"
#include "ServerUser.h"
#include "User.h"
#include "Version.h"
#include "crypto/CryptState.h"

#include <cassert>
#include <unordered_map>

#include <QtCore/QStack>
#include <QtCore/QTimeZone>
#include <QtCore/QtEndian>

#include <tracy/Tracy.hpp>

#define RATELIMIT(user)                   \
	if (user->leakyBucket.ratelimit(1)) { \
		return;                           \
	}

#define MSG_SETUP(st)            \
	if (uSource->sState != st) { \
		return;                  \
	}                            \
	uSource->bwr.resetIdleSeconds()

#define MSG_SETUP_NO_UNIDLE(st) \
	if (uSource->sState != st)  \
	return

#define VICTIM_SETUP                                   \
	ServerUser *pDstServerUser = uSource;              \
	if (msg.has_session())                             \
		pDstServerUser = qhUsers.value(msg.session()); \
	if (!pDstServerUser)                               \
		return;                                        \
	Q_UNUSED(pDstServerUser)

#define PERM_DENIED(who, where, what)                                                                                \
	{                                                                                                                \
		MumbleProto::PermissionDenied mppd;                                                                          \
		mppd.set_permission(static_cast< unsigned int >(what));                                                      \
		mppd.set_channel_id(where->iId);                                                                             \
		mppd.set_session(who->uiSession);                                                                            \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_Permission);                                            \
		sendMessage(uSource, mppd);                                                                                  \
		log(uSource,                                                                                                 \
			QString("%1 not allowed to %2 in %3").arg(who->qsName).arg(ChanACL::permName(what)).arg(where->qsName)); \
	}

#define PERM_DENIED_TYPE(type)                                        \
	{                                                                 \
		MumbleProto::PermissionDenied mppd;                           \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_##type); \
		sendMessage(uSource, mppd);                                   \
	}

#define PERM_DENIED_FALLBACK(type, version, text)                     \
	{                                                                 \
		MumbleProto::PermissionDenied mppd;                           \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_##type); \
		if (uSource->m_version < version)                             \
			mppd.set_reason(u8(text));                                \
		sendMessage(uSource, mppd);                                   \
	}

#define PERM_DENIED_HASH(user)                                                    \
	{                                                                             \
		MumbleProto::PermissionDenied mppd;                                       \
		mppd.set_type(MumbleProto::PermissionDenied_DenyType_MissingCertificate); \
		if (user)                                                                 \
			mppd.set_session(user->uiSession);                                    \
		sendMessage(uSource, mppd);                                               \
	}

/// A helper class for managing temporary access tokens.
/// It will add the tokens in the comstructor and remove them again in the destructor effectively
/// turning the tokens into a scope-based property.
class TemporaryAccessTokenHelper {
protected:
	ServerUser *affectedUser;
	QStringList qslTemporaryTokens;
	Server *server;

public:
	TemporaryAccessTokenHelper(ServerUser *affectedUser, const QStringList &tokens, Server *server)
		: affectedUser(affectedUser), qslTemporaryTokens(tokens), server(server) {
		// Add the temporary tokens
		QMutableStringListIterator it(this->qslTemporaryTokens);

		{
			QMutexLocker qml(&server->qmCache);

			while (it.hasNext()) {
				QString &token = it.next();

				// If tokens are treated case-insensitively, transform all temp. tokens to lowercase first
				if (Group::accessTokenCaseSensitivity == Qt::CaseInsensitive) {
					token = token.toLower();
				}

				if (!this->affectedUser->qslAccessTokens.contains(token, Group::accessTokenCaseSensitivity)) {
					// Add token
					this->affectedUser->qslAccessTokens << token;
				} else {
					// It appears, as if the user already has this token set -> it's not a temporary one or a duplicate
					it.remove();
				}
			}
		}

		if (!this->qslTemporaryTokens.isEmpty()) {
			// Clear the cache in order for tokens to take effect
			server->clearACLCache(this->affectedUser);
		}
	}

	~TemporaryAccessTokenHelper() {
		if (!this->qslTemporaryTokens.isEmpty()) {
			{
				QMutexLocker qml(&server->qmCache);

				// remove the temporary tokens
				foreach (const QString &token, this->qslTemporaryTokens) {
					this->affectedUser->qslAccessTokens.removeOne(token);
				}
			}

			// Clear cache to actually get rid of the temporary tokens
			server->clearACLCache(this->affectedUser);
		}
	}
};

/// Checks whether the given channel has restrictions affecting the ENTER privilege
///
/// @param c A pointer to the Channel that should be checked
/// @return Whether the provided channel has an ACL denying ENTER
bool isChannelEnterRestricted(Channel *c) {
	// A channel is enter restricted if there's an ACL denying enter privileges
	foreach (ChanACL *acl, c->qlACL) {
		if (acl->pDeny & ChanACL::Enter) {
			return true;
		}
	}

	return false;
}

void Server::msgAuthenticate(ServerUser *uSource, MumbleProto::Authenticate &msg) {
	ZoneScoped;

	if (uSource->sState == ServerUser::Authenticated && (msg.tokens_size() > 0 || !uSource->qslAccessTokens.empty())) {
		// Process a change in access tokens for already authenticated users
		QStringList qsl;

		for (int i = 0; i < msg.tokens_size(); ++i) {
			qsl << u8(msg.tokens(i));
		}

		{
			QMutexLocker qml(&qmCache);
			uSource->qslAccessTokens = qsl;
		}

		clearACLCache(uSource);

		// Send back updated enter states of all channels
		MumbleProto::ChannelState mpcs;

		for (Channel *chan : qhChannels) {
			mpcs.set_channel_id(static_cast< unsigned int >(chan->iId));
			mpcs.set_can_enter(hasPermission(uSource, chan, ChanACL::Enter));
			// As no ACLs have changed, we don't need to update the is_access_restricted message field

			sendMessage(uSource, mpcs);
		}
	}
	MSG_SETUP(ServerUser::Connected);

	// As the first thing, assign a session ID to this client. Given that the client initiated
	// the authentication procedure we can be sure that this is not just a random TCP connection.
	// Thus it is about time we assign the ID to this client in order to be able to reference it
	// in the following.
	{
		QWriteLocker wl(&qrwlVoiceThread);
		uSource->uiSession = qqIds.dequeue();
		qhUsers.insert(uSource->uiSession, uSource);
		qhHostUsers[uSource->haAddress].insert(uSource);
	}

	Channel *root = qhChannels.value(0);
	Channel *c;

	uSource->qsName = u8(msg.username()).trimmed();

	bool ok     = false;
	bool nameok = validateUserName(uSource->qsName);
	QString pw  = u8(msg.password());

	// Fetch ID and stored username.
	// This function needs to support the fact that sessions may go away.
	int id = authenticate(uSource->qsName, pw, static_cast< int >(uSource->uiSession), uSource->qslEmail,
						  uSource->qsHash, uSource->bVerified, uSource->peerCertificateChain());

	uSource->iId = id >= 0 ? id : -1;

	QString reason;
	MumbleProto::Reject_RejectType rtType = MumbleProto::Reject_RejectType_None;

	if (id == -2 && !nameok) {
		reason = "Invalid username";
		rtType = MumbleProto::Reject_RejectType_InvalidUsername;
	} else if (id == -1) {
		reason = "Wrong certificate or password for existing user";
		rtType = MumbleProto::Reject_RejectType_WrongUserPW;
	} else if (id == -2 && !qsPassword.isEmpty() && qsPassword != pw) {
		reason = "Invalid server password";
		rtType = MumbleProto::Reject_RejectType_WrongServerPW;
	} else if (id == -3) {
		reason = "Your account information can not be verified currently. Please try again later";
		rtType = MumbleProto::Reject_RejectType_AuthenticatorFail;
	} else {
		ok = true;
	}

	ServerUser *uOld = nullptr;
	foreach (ServerUser *u, qhUsers) {
		if (u == uSource)
			continue;
		if (((u->iId >= 0) && (u->iId == uSource->iId)) || (u->qsName.toLower() == uSource->qsName.toLower())) {
			uOld = u;
			break;
		}
	}

	// Allow reuse of name from same IP
	if (ok && uOld && (uSource->iId == -1)) {
		if ((uOld->peerAddress() != uSource->peerAddress())
			&& (uSource->qsHash.isEmpty() || (uSource->qsHash != uOld->qsHash))) {
			reason = "Username already in use";
			rtType = MumbleProto::Reject_RejectType_UsernameInUse;
			ok     = false;
		}
	}

	if ((id != 0) && (static_cast< unsigned int >(qhUsers.count()) > iMaxUsers)) {
		reason = QString::fromLatin1("Server is full (max %1 users)").arg(iMaxUsers);
		rtType = MumbleProto::Reject_RejectType_ServerFull;
		ok     = false;
	}

	if ((id != 0) && (uSource->qsHash.isEmpty() && bCertRequired)) {
		reason = QString::fromLatin1("A certificate is required to connect to this server");
		rtType = MumbleProto::Reject_RejectType_NoCertificate;
		ok     = false;
	}

	if (ok) {
		if (msg.tokens_size() > 0) {
			// Set the access tokens for the newly connected user
			QStringList qsl;

			for (int i = 0; i < msg.tokens_size(); ++i) {
				qsl << u8(msg.tokens(i));
			}

			{
				QMutexLocker qml(&qmCache);
				uSource->qslAccessTokens = qsl;
			}
		}

		// Clear cache as the "auth" ACL depends on the user id
		// and any cached ACL won't have taken that into account yet
		// Also, if we set access tokens above, we have to reset
		// the ACL cache anyway.
		clearACLCache(uSource);
	}

	Channel *lc       = nullptr;
	int lastChannelID = readLastChannel(uSource->iId);
	if (lastChannelID >= 0) {
		lc = qhChannels.value(static_cast< unsigned int >(lastChannelID));
	}

	if (!lc || !hasPermission(uSource, lc, ChanACL::Enter) || isChannelFull(lc, uSource)) {
		lc = qhChannels.value(iDefaultChan);
		if (!lc || !hasPermission(uSource, lc, ChanACL::Enter) || isChannelFull(lc, uSource)) {
			lc = root;
			if (isChannelFull(lc, uSource)) {
				reason = QString::fromLatin1("Server channels are full");
				rtType = MumbleProto::Reject_RejectType_ServerFull;
				ok     = false;
			}
		}
	}

	if (!ok) {
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

		uSource->csCrypt->genKey();

		MumbleProto::CryptSetup mpcrypt;
		mpcrypt.set_key(uSource->csCrypt->getRawKey());
		mpcrypt.set_server_nonce(uSource->csCrypt->getEncryptIV());
		mpcrypt.set_client_nonce(uSource->csCrypt->getDecryptIV());
		sendMessage(uSource, mpcrypt);
	}

	bool fake_celt_support = false;
	if (msg.celt_versions_size() > 0) {
		for (int i = 0; i < msg.celt_versions_size(); ++i)
			uSource->qlCodecs.append(msg.celt_versions(i));
	} else {
		uSource->qlCodecs.append(static_cast< qint32 >(0x8000000b));
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
		sendTextMessage(
			nullptr, uSource, false,
			QLatin1String("<strong>WARNING:</strong> Your client doesn't support the CELT codec, you won't be able to "
						  "talk to or hear most clients. Please make sure your client was built with CELT support."));
	}

	// Transmit channel tree
	QQueue< Channel * > q;
	QSet< Channel * > chans;
	q << root;
	MumbleProto::ChannelState mpcs;

	while (!q.isEmpty()) {
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

		if ((uSource->m_version >= Version::fromComponents(1, 2, 2)) && !c->qbaDescHash.isEmpty())
			mpcs.set_description_hash(blob(c->qbaDescHash));
		else if (!c->qsDesc.isEmpty())
			mpcs.set_description(u8(c->qsDesc));

		mpcs.set_max_users(c->uiMaxUsers);

		// Include info about enter restrictions of this channel
		mpcs.set_is_enter_restricted(isChannelEnterRestricted(c));
		mpcs.set_can_enter(hasPermission(uSource, c, ChanACL::Enter));

		sendMessage(uSource, mpcs);

		foreach (c, c->qlChannels)
			q.enqueue(c);
	}

	// Transmit links
	foreach (c, chans) {
		if (c->qhLinks.count() > 0) {
			mpcs.Clear();
			mpcs.set_channel_id(c->iId);

			foreach (Channel *l, c->qhLinks.keys())
				mpcs.add_links(l->iId);
			sendMessage(uSource, mpcs);
		}
	}

	loadChannelListenersOf(*uSource);

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
		mpus.set_user_id(static_cast< unsigned int >(uSource->iId));

		hashAssign(uSource->qbaTexture, uSource->qbaTextureHash, getUserTexture(uSource->iId));

		if (!uSource->qbaTextureHash.isEmpty())
			mpus.set_texture_hash(blob(uSource->qbaTextureHash));
		else if (!uSource->qbaTexture.isEmpty())
			mpus.set_texture(blob(uSource->qbaTexture));

		const QMap< int, QString > &info = getRegistration(uSource->iId);
		if (info.contains(ServerDB::User_Comment)) {
			hashAssign(uSource->qsComment, uSource->qbaCommentHash, info.value(ServerDB::User_Comment));
			if (!uSource->qbaCommentHash.isEmpty())
				mpus.set_comment_hash(blob(uSource->qbaCommentHash));
			else if (!uSource->qsComment.isEmpty())
				mpus.set_comment(u8(uSource->qsComment));
		}
	}
	if (!uSource->qsHash.isEmpty())
		mpus.set_hash(u8(uSource->qsHash));

	mpus.set_channel_id(uSource->cChannel->iId);

	sendAll(mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);

	if ((uSource->qbaTexture.length() >= 4)
		&& (qFromBigEndian< unsigned int >(reinterpret_cast< const unsigned char * >(uSource->qbaTexture.constData()))
			== 600 * 60 * 4))
		mpus.set_texture(blob(uSource->qbaTexture));
	if (!uSource->qsComment.isEmpty())
		mpus.set_comment(u8(uSource->qsComment));
	sendAll(mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);

	// Transmit other users profiles
	foreach (ServerUser *u, qhUsers) {
		if (u->sState != ServerUser::Authenticated)
			continue;

		if (u == uSource)
			continue;

		mpus.Clear();
		mpus.set_session(u->uiSession);
		mpus.set_name(u8(u->qsName));
		if (u->iId >= 0)
			mpus.set_user_id(static_cast< unsigned int >(u->iId));
		if (uSource->m_version >= Version::fromComponents(1, 2, 2)) {
			if (!u->qbaTextureHash.isEmpty())
				mpus.set_texture_hash(blob(u->qbaTextureHash));
			else if (!u->qbaTexture.isEmpty())
				mpus.set_texture(blob(u->qbaTexture));
		} else if ((uSource->qbaTexture.length() >= 4)
				   && (qFromBigEndian< unsigned int >(
						   reinterpret_cast< const unsigned char * >(uSource->qbaTexture.constData()))
					   == 600 * 60 * 4)) {
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
		if ((uSource->m_version >= Version::fromComponents(1, 2, 2)) && !u->qbaCommentHash.isEmpty())
			mpus.set_comment_hash(blob(u->qbaCommentHash));
		else if (!u->qsComment.isEmpty())
			mpus.set_comment(u8(u->qsComment));
		if (!u->qsHash.isEmpty())
			mpus.set_hash(u8(u->qsHash));


		for (unsigned int channelID : m_channelListenerManager.getListenedChannelsForUser(u->uiSession)) {
			mpus.add_listening_channel_add(channelID);

			if (broadcastListenerVolumeAdjustments) {
				VolumeAdjustment volume = m_channelListenerManager.getListenerVolumeAdjustment(u->uiSession, channelID);
				MumbleProto::UserState::VolumeAdjustment *adjustment = mpus.add_listening_volume_adjustment();
				adjustment->set_listening_channel(channelID);
				adjustment->set_volume_adjustment(volume.factor);
			}
		}

		sendMessage(uSource, mpus);
	}

	// Send synchronisation packet
	MumbleProto::ServerSync mpss;
	mpss.set_session(uSource->uiSession);
	if (!qsWelcomeText.isEmpty())
		mpss.set_welcome_text(u8(qsWelcomeText));
	mpss.set_max_bandwidth(static_cast< unsigned int >(iMaxBandwidth));

	if (uSource->iId == 0) {
		mpss.set_permissions(ChanACL::All);
	} else {
		QMutexLocker qml(&qmCache);
		ChanACL::hasPermission(uSource, root, ChanACL::Enter, &acCache);
		mpss.set_permissions(acCache.value(uSource)->value(root));
	}

	sendMessage(uSource, mpss);

	// Transmit user's listeners - this has to be done AFTER the server-sync message has been sent to uSource as the
	// client may require its own session ID for processing the listeners properly.
	mpus.Clear();
	mpus.set_session(uSource->uiSession);
	for (unsigned int channelID : m_channelListenerManager.getListenedChannelsForUser(uSource->uiSession)) {
		mpus.add_listening_channel_add(channelID);
	}

	// If we are not intending to broadcast the volume adjustments to everyone, we have to send the message to all but
	// uSource without the volume adjustments. Then append the adjustments, but only send them to uSource. If we are in
	// fact broadcasting, just append the adjustments and send to everyone.
	if (!broadcastListenerVolumeAdjustments && mpus.listening_channel_add_size() > 0) {
		sendExcept(uSource, mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
	}

	std::unordered_map< unsigned int, VolumeAdjustment > volumeAdjustments =
		m_channelListenerManager.getAllListenerVolumeAdjustments(uSource->uiSession);
	for (auto it = volumeAdjustments.begin(); it != volumeAdjustments.end(); ++it) {
		MumbleProto::UserState::VolumeAdjustment *adjustment = mpus.add_listening_volume_adjustment();
		adjustment->set_listening_channel(it->first);
		adjustment->set_volume_adjustment(it->second.factor);
	}

	if (mpus.listening_channel_add_size() > 0 || mpus.listening_volume_adjustment_size() > 0) {
		if (!broadcastListenerVolumeAdjustments) {
			if (uSource->m_version >= Version::fromComponents(1, 2, 2)) {
				sendMessage(uSource, mpus);
			}
		} else {
			sendAll(mpus, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
		}
	}

	MumbleProto::ServerConfig mpsc;
	mpsc.set_allow_html(bAllowHTML);
	mpsc.set_message_length(static_cast< unsigned int >(iMaxTextMessageLength));
	mpsc.set_image_message_length(static_cast< unsigned int >(iMaxImageMessageLength));
	mpsc.set_max_users(static_cast< unsigned int >(iMaxUsers));
	mpsc.set_recording_allowed(allowRecording);
	sendMessage(uSource, mpsc);

	MumbleProto::SuggestConfig mpsug;
	if (m_suggestVersion != Version::UNKNOWN) {
		MumbleProto::setSuggestedVersion(mpsug, m_suggestVersion);
	}
	if (!qvSuggestPositional.isNull())
		mpsug.set_positional(qvSuggestPositional.toBool());
	if (!qvSuggestPushToTalk.isNull())
		mpsug.set_push_to_talk(qvSuggestPushToTalk.toBool());
#if GOOGLE_PROTOBUF_VERSION >= 3004000
	if (mpsug.ByteSizeLong() > 0) {
#else
	// ByteSize() has been deprecated as of protobuf v3.4
	if (mpsug.ByteSize() > 0) {
#endif
		sendMessage(uSource, mpsug);
	}

	if (uSource->m_version < Version::fromComponents(1, 4, 0) && Meta::mp.iMaxListenersPerChannel != 0
		&& Meta::mp.iMaxListenerProxiesPerUser != 0) {
		// The server has the ChannelListener feature enabled but the client that connects doesn't have version 1.4.0 or
		// newer meaning that this client doesn't know what ChannelListeners are. Thus we'll send that user a
		// text-message informing about this.
		MumbleProto::TextMessage mptm;

		if (Meta::mp.bAllowHTML) {
			mptm.set_message("<b>[WARNING]</b>: This server has the <b>ChannelListener</b> feature enabled but your "
							 "client version does not support it. "
							 "This means that users <b>might be listening to what you are saying in your channel "
							 "without you noticing!</b> "
							 "You can solve this issue by upgrading to Mumble 1.4.0 or newer.");
		} else {
			mptm.set_message(
				"[WARNING]: This server has the ChannelListener feature enabled but your client version does not "
				"support it. "
				"This means that users might be listening to what you are saying in your channel without you noticing! "
				"You can solve this issue by upgrading to Mumble 1.4.0 or newer.");
		}

		sendMessage(uSource, mptm);
	}

	switch (msg.client_type()) {
		case static_cast< int >(ClientType::BOT):
			uSource->m_clientType = ClientType::BOT;
			m_botCount++;
			break;
		case static_cast< int >(ClientType::REGULAR):
			// No-op (also applies to unknown values of msg.client_type())
			// (The default client type is regular anyway, so we don't need to change anything here)
			break;
	}

	log(uSource, "Authenticated");

	emit userConnected(uSource);
}

void Server::msgBanList(ServerUser *uSource, MumbleProto::BanList &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	QSet< Ban > previousBans, newBans;
	if (!hasPermission(uSource, qhChannels.value(0), ChanACL::Ban)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Ban);
		return;
	}
	if (msg.query()) {
		msg.clear_query();
		msg.clear_bans();
		foreach (const Ban &b, qlBans) {
			MumbleProto::BanList_BanEntry *be = msg.add_bans();
			be->set_address(b.haAddress.toStdString());
			be->set_mask(static_cast< unsigned int >(b.iMask));
			be->set_name(u8(b.qsUsername));
			be->set_hash(u8(b.qsHash));
			be->set_reason(u8(b.qsReason));
			be->set_start(u8(b.qdtStart.toString(Qt::ISODate)));
			be->set_duration(b.iDuration);
		}
		sendMessage(uSource, msg);
	} else {
		previousBans = QSet< Ban >(qlBans.begin(), qlBans.end());
		qlBans.clear();
		for (int i = 0; i < msg.bans_size(); ++i) {
			const MumbleProto::BanList_BanEntry &be = msg.bans(i);

			Ban b;
			b.haAddress  = be.address();
			b.iMask      = static_cast< int >(be.mask());
			b.qsUsername = u8(be.name());
			b.qsHash     = u8(be.hash());
			b.qsReason   = u8(be.reason());
			if (be.has_start()) {
				b.qdtStart = QDateTime::fromString(u8(be.start()), Qt::ISODate);
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
				b.qdtStart.setTimeZone(QTimeZone::UTC);
#else
				b.qdtStart.setTimeSpec(Qt::UTC);
#endif
			} else {
				b.qdtStart = QDateTime::currentDateTime().toUTC();
			}
			b.iDuration = be.duration();
			if (b.isValid()) {
				qlBans << b;
			}
		}
		newBans             = QSet< Ban >(qlBans.begin(), qlBans.end());
		QSet< Ban > removed = previousBans - newBans;
		QSet< Ban > added   = newBans - previousBans;
		foreach (const Ban &b, removed) { log(uSource, QString("Removed ban: %1").arg(b.toString())); }
		foreach (const Ban &b, added) { log(uSource, QString("New ban: %1").arg(b.toString())); }
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

void Server::msgUDPTunnel(ServerUser *, MumbleProto::UDPTunnel &) {
	// This code should be unreachable
	assert(false);
	qWarning("Messages: Reached theoretically unreachable function msgUDPTunnel");
}

void Server::msgUserState(ServerUser *uSource, MumbleProto::UserState &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	VICTIM_SETUP;

	Channel *root = qhChannels.value(0);

	/*
		First check all permissions involved
	*/
	if ((pDstServerUser->iId == 0) && (uSource->iId != 0)) {
		// Don't allow any action on the SuperUser except initiated directly by the SuperUser himself/herself
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

	// Handle potential temporary access tokens
	QStringList temporaryAccessTokens;
	for (int i = 0; i < msg.temporary_access_tokens_size(); i++) {
		temporaryAccessTokens << u8(msg.temporary_access_tokens(i));
	}
	TemporaryAccessTokenHelper tempTokenHelper(uSource, temporaryAccessTokens, this);

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());
		if (!c || (c == pDstServerUser->cChannel))
			return;

		if ((uSource != pDstServerUser) && (!hasPermission(uSource, pDstServerUser->cChannel, ChanACL::Move))) {
			PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::Move);
			return;
		}

		if (!hasPermission(uSource, c, ChanACL::Move) && !hasPermission(pDstServerUser, c, ChanACL::Enter)) {
			PERM_DENIED(pDstServerUser, c, ChanACL::Enter);
			return;
		}
		if (isChannelFull(c, uSource)) {
			PERM_DENIED_FALLBACK(ChannelFull, Version::fromComponents(1, 2, 1), QLatin1String("Channel is full"));
			return;
		}
	}

	QList< Channel * > listeningChannelsAdd;
	int passedChannelListener = 0;
	// Check permission for each channel
	for (int i = 0; i < msg.listening_channel_add_size(); i++) {
		Channel *c = qhChannels.value(msg.listening_channel_add(i));

		if (!c) {
			continue;
		}

		if (!hasPermission(pDstServerUser, c, ChanACL::Listen)) {
			PERM_DENIED(pDstServerUser, c, ChanACL::Listen);
			continue;
		}

		if (Meta::mp.iMaxListenersPerChannel >= 0
			&& Meta::mp.iMaxListenersPerChannel - m_channelListenerManager.getListenerCountForChannel(c->iId) - 1 < 0) {
			// A limit for the amount of listener proxies per channel is set and it has been reached already
			PERM_DENIED_FALLBACK(ChannelListenerLimit, Version::fromComponents(1, 4, 0),
								 QLatin1String("No more listeners allowed in this channel"));
			continue;
		}

		if (Meta::mp.iMaxListenerProxiesPerUser >= 0
			&& Meta::mp.iMaxListenerProxiesPerUser
					   - m_channelListenerManager.getListenedChannelCountForUser(uSource->uiSession)
					   - passedChannelListener - 1
				   < 0) {
			// A limit for the amount of listener proxies per user is set and it has been reached already
			PERM_DENIED_FALLBACK(UserListenerLimit, Version::fromComponents(1, 4, 0),
								 QLatin1String("No more listeners allowed in this channel"));
			continue;
		}

		passedChannelListener++;

		listeningChannelsAdd << c;
	}

	if (msg.has_mute() || msg.has_deaf() || msg.has_suppress() || msg.has_priority_speaker()) {
		if (pDstServerUser->iId == 0) {
			PERM_DENIED_TYPE(SuperUser);
			return;
		}
		if (!hasPermission(uSource, pDstServerUser->cChannel, ChanACL::MuteDeafen) || msg.suppress()) {
			PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::MuteDeafen);
			return;
		}
	}

	if (msg.has_mute() || msg.has_deaf() || msg.has_suppress()) {
		if (pDstServerUser->cChannel->bTemporary) {
			// If the destination user is inside a temporary channel,
			// the source user needs to have the MuteDeafen ACL in the first
			// non-temporary parent channel.

			Channel *c = pDstServerUser->cChannel;
			while (c && c->bTemporary) {
				c = c->cParent;
			}

			if (!c || !hasPermission(uSource, c, ChanACL::MuteDeafen)) {
				PERM_DENIED_TYPE(TemporaryChannel);
				return;
			}
		}
	}

	QString comment;

	if (msg.has_comment()) {
		bool changed = false;
		comment      = u8(msg.comment());
		if (uSource != pDstServerUser) {
			if (!hasPermission(uSource, root, ChanACL::ResetUserContent)) {
				PERM_DENIED(uSource, root, ChanACL::ResetUserContent);
				return;
			}
			if (comment.length() > 0) {
				PERM_DENIED_TYPE(TextTooLong);
				return;
			}
		}


		if (!isTextAllowed(comment, changed)) {
			PERM_DENIED_TYPE(TextTooLong);
			return;
		}
		if (changed)
			msg.set_comment(u8(comment));
	}

	if (msg.has_texture()) {
		if (iMaxImageMessageLength > 0
			&& (msg.texture().length() > static_cast< unsigned int >(iMaxImageMessageLength))) {
			PERM_DENIED_TYPE(TextTooLong);
			return;
		}
		if (uSource != pDstServerUser) {
			if (!hasPermission(uSource, root, ChanACL::ResetUserContent)) {
				PERM_DENIED(uSource, root, ChanACL::ResetUserContent);
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
		if ((pDstServerUser->iId >= 0) || !hasPermission(uSource, root, p)) {
			PERM_DENIED(uSource, root, p);
			return;
		}
		if (pDstServerUser->qsHash.isEmpty()) {
			PERM_DENIED_HASH(pDstServerUser);
			return;
		}
	}

	// Prevent self-targeting state changes from being applied to others
	if ((pDstServerUser != uSource)
		&& (msg.has_self_deaf() || msg.has_self_mute() || msg.has_plugin_context() || msg.has_plugin_identity()
			|| msg.has_recording() || msg.listening_channel_add_size() > 0
			|| msg.listening_channel_remove_size() > 0)) {
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
			if (!setTexture(pDstServerUser->iId, qba)) {
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
			pDstServerUser->bSelfDeaf = msg.self_deaf();
			if (pDstServerUser->bSelfDeaf)
				msg.set_self_mute(true);
			bBroadcast = true;
		}

		if (msg.has_self_mute()) {
			pDstServerUser->bSelfMute = msg.self_mute();
			if (!pDstServerUser->bSelfMute) {
				msg.set_self_deaf(false);
				pDstServerUser->bSelfDeaf = false;
			}
			bBroadcast = true;
		}

		if (msg.has_plugin_context()) {
			pDstServerUser->ssContext = msg.plugin_context();

			// Make sure to clear this from the packet so we don't broadcast it
			msg.clear_plugin_context();
		}
	}

	if (msg.has_plugin_identity()) {
		pDstServerUser->qsIdentity = u8(msg.plugin_identity());
		// Make sure to clear this from the packet so we don't broadcast it
		msg.clear_plugin_identity();
	}

	if (!comment.isNull()) {
		hashAssign(pDstServerUser->qsComment, pDstServerUser->qbaCommentHash, comment);

		if (pDstServerUser->iId >= 0) {
			QMap< int, QString > info;
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
			if (!pDstServerUser->bMute) {
				msg.set_deaf(false);
				pDstServerUser->bDeaf = false;
			}
		}
		if (msg.has_suppress())
			pDstServerUser->bSuppress = msg.suppress();

		if (msg.has_priority_speaker())
			pDstServerUser->bPrioritySpeaker = msg.priority_speaker();

		log(uSource, QString("Changed speak-state of %1 (%2 %3 %4 %5)")
						 .arg(QString(*pDstServerUser), QString::number(pDstServerUser->bMute),
							  QString::number(pDstServerUser->bDeaf), QString::number(pDstServerUser->bSuppress),
							  QString::number(pDstServerUser->bPrioritySpeaker)));

		bBroadcast = true;
	}

	if (msg.has_recording() && (pDstServerUser->bRecording != msg.recording())) {
		assert(uSource == pDstServerUser);

		pDstServerUser->bRecording = msg.recording();

		MumbleProto::TextMessage mptm;
		mptm.add_tree_id(0);
		if (pDstServerUser->bRecording) {
			if (!allowRecording) {
				// User tried to start recording even though this server forbids it
				// -> Kick user
				MumbleProto::UserRemove mpur;
				mpur.set_session(uSource->uiSession);
				mpur.set_reason("Recording is not allowed on this server");
				sendMessage(uSource, mpur);
				uSource->forceFlush();
				uSource->disconnectSocket(true);

				// We just kicked this user, so there is no point in further processing his/her message
				return;
			} else {
				mptm.set_message(u8(QString(QLatin1String("User '%1' started recording")).arg(pDstServerUser->qsName)));
			}
		} else {
			mptm.set_message(u8(QString(QLatin1String("User '%1' stopped recording")).arg(pDstServerUser->qsName)));
		}

		sendAll(mptm, Version::fromComponents(1, 2, 3), Version::CompareMode::LessThan);

		bBroadcast = true;
	}

	if (msg.has_channel_id()) {
		Channel *c = qhChannels.value(msg.channel_id());

		userEnterChannel(pDstServerUser, c, msg);
		log(uSource, QString("Moved %1 to %2").arg(QString(*pDstServerUser), QString(*c)));
		bBroadcast = true;
	}

	// Handle channel listening
	// Note that it is important to handle the listening channels after channel-joins
	QSet< unsigned int > volumeAdjustedChannels;
	for (int i = 0; i < msg.listening_volume_adjustment_size(); i++) {
		const MumbleProto::UserState::VolumeAdjustment &adjustment = msg.listening_volume_adjustment(i);

		const Channel *channel = qhChannels.value(adjustment.listening_channel());

		if (channel) {
			setChannelListenerVolume(*pDstServerUser, *channel, adjustment.volume_adjustment());

			volumeAdjustedChannels << channel->iId;
		} else {
			log(uSource, QString::fromLatin1("Invalid channel ID \"%1\" in volume adjustment")
							 .arg(adjustment.listening_channel()));
		}
	}
	for (Channel *c : listeningChannelsAdd) {
		addChannelListener(*pDstServerUser, *c);

		log(QString::fromLatin1("\"%1\" is now listening to channel \"%2\"")
				.arg(QString(*pDstServerUser))
				.arg(QString(*c)));

		float volumeAdjustment =
			m_channelListenerManager.getListenerVolumeAdjustment(pDstServerUser->uiSession, c->iId).factor;

		if (volumeAdjustment != 1.0f && !volumeAdjustedChannels.contains(c->iId)) {
			MumbleProto::UserState::VolumeAdjustment *adjustment = msg.add_listening_volume_adjustment();
			adjustment->set_listening_channel(c->iId);
			adjustment->set_volume_adjustment(volumeAdjustment);
		}
	}
	for (int i = 0; i < msg.listening_channel_remove_size(); i++) {
		Channel *c = qhChannels.value(msg.listening_channel_remove(i));

		if (c) {
			disableChannelListener(*pDstServerUser, *c);

			log(QString::fromLatin1("\"%1\" is no longer listening to \"%2\"")
					.arg(QString(*pDstServerUser))
					.arg(QString(*c)));
		}
	}

	bool listenerVolumeChanged = msg.listening_volume_adjustment_size() > 0;
	bool listenerChanged       = !listeningChannelsAdd.isEmpty() || msg.listening_channel_remove_size() > 0;

	bool broadcastingBecauseOfVolumeChange = !bBroadcast && listenerVolumeChanged;
	bBroadcast                             = bBroadcast || listenerChanged || listenerVolumeChanged;

	if (listenerChanged || listenerVolumeChanged) {
		// As whisper targets also contain information about ChannelListeners and
		// their associated volume adjustment, we have to clear the target cache
		clearWhisperTargetCache();
	}


	bool bDstAclChanged = false;
	if (msg.has_user_id()) {
		// Handle user (Self-)Registration
		QMap< int, QString > info;

		info.insert(ServerDB::User_Name, pDstServerUser->qsName);
		info.insert(ServerDB::User_Hash, pDstServerUser->qsHash);
		if (!pDstServerUser->qslEmail.isEmpty())
			info.insert(ServerDB::User_Email, pDstServerUser->qslEmail.first());
		int id = registerUser(info);
		if (id > 0) {
			pDstServerUser->iId = id;
			setLastChannel(pDstServerUser);
			msg.set_user_id(static_cast< unsigned int >(id));
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
		if (msg.has_texture() && (pDstServerUser->qbaTexture.length() >= 4)
			&& (qFromBigEndian< unsigned int >(
					reinterpret_cast< const unsigned char * >(pDstServerUser->qbaTexture.constData()))
				!= 600 * 60 * 4)) {
			// This is a new style texture, don't send it because the client doesn't handle it correctly / crashes.
			msg.clear_texture();
			sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
			msg.set_texture(blob(pDstServerUser->qbaTexture));
		} else {
			// This is an old style texture, empty texture or there was no texture in this packet,
			// send the message unchanged.
			sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
		}

		// Texture / comment handling for clients >= 1.2.2.
		// Send only a hash of the texture / comment text. The client will request the actual data if necessary.
		if (msg.has_texture() && !pDstServerUser->qbaTextureHash.isEmpty()) {
			msg.clear_texture();
			msg.set_texture_hash(blob(pDstServerUser->qbaTextureHash));
		}
		if (msg.has_comment() && !pDstServerUser->qbaCommentHash.isEmpty()) {
			msg.clear_comment();
			msg.set_comment_hash(blob(pDstServerUser->qbaCommentHash));
		}

		if (uSource->m_version >= Version::fromComponents(1, 2, 2)) {
			sendMessage(uSource, msg);
		}
		if (!broadcastListenerVolumeAdjustments) {
			// Don't broadcast the volume adjustments to everyone
			msg.clear_listening_volume_adjustment();
		}

		if (broadcastListenerVolumeAdjustments || !broadcastingBecauseOfVolumeChange) {
			sendExcept(uSource, msg, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
		}

		if (bDstAclChanged) {
			clearACLCache(pDstServerUser);
		} else if (listenerChanged || listenerVolumeChanged) {
			// We only have to do this if the ACLs didn't change as
			// clearACLCache calls clearWhisperTargetChache anyways
			clearWhisperTargetCache();
		}
	}

	emit userStateChanged(pDstServerUser);
}

void Server::msgUserRemove(ServerUser *uSource, MumbleProto::UserRemove &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	VICTIM_SETUP;

	msg.set_actor(uSource->uiSession);

	bool ban = msg.has_ban() && msg.ban();

	Channel *c                   = qhChannels.value(0);
	QFlags< ChanACL::Perm > perm = ban ? ChanACL::Ban : (ChanACL::Ban | ChanACL::Kick);

	if ((pDstServerUser->iId == 0) || !hasPermission(uSource, c, perm)) {
		PERM_DENIED(uSource, c, perm);
		return;
	}

	if (ban) {
		Ban b;
		b.haAddress  = pDstServerUser->haAddress;
		b.iMask      = 128;
		b.qsReason   = u8(msg.reason());
		b.qsUsername = pDstServerUser->qsName;
		b.qsHash     = pDstServerUser->qsHash;
		b.qdtStart   = QDateTime::currentDateTime().toUTC();
		b.iDuration  = 0;
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
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	Channel *c = nullptr;
	Channel *p = nullptr;

	// If this message relates to an existing channel check if the id is really valid
	if (msg.has_channel_id()) {
		c = qhChannels.value(msg.channel_id());
		if (!c)
			return;
	} else {
		RATELIMIT(uSource);
	}

	// Check if the parent exists
	if (msg.has_parent()) {
		p = qhChannels.value(msg.parent());
		if (!p)
			return;
	}

	msg.clear_links();

	QString qsName;
	QString qsDesc;
	if (msg.has_description()) {
		qsDesc       = u8(msg.description());
		bool changed = false;
		if (!isTextAllowed(qsDesc, changed)) {
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

		if (!validateChannelName(qsName)) {
			PERM_DENIED_TYPE(ChannelName);
			return;
		}

		if (qsName.length() == 0) {
			PERM_DENIED_TYPE(ChannelName);
			return;
		}

		if (p || (c && c->iId != 0)) {
			Channel *cp = p ? p : c->cParent;
			foreach (Channel *sibling, cp->qlChannels) {
				if (sibling->qsName == qsName) {
					PERM_DENIED_TYPE(ChannelName);
					return;
				}
			}
		}
	}

	if (p) {
		// Having a parent channel given means we either want to create
		// a channel in or move a channel into this parent.

		if (!canNest(p, c)) {
			PERM_DENIED_FALLBACK(NestingLimit, Version::fromComponents(1, 2, 4),
								 QLatin1String("Channel nesting limit reached"));
			return;
		}
	}

	if (!c) {
		// If we don't have a channel handle up to now we want to create a new channel
		// so check if the user has enough rights and we got everything we need.
		if (!p || qsName.isNull())
			return;

		if (iChannelCountLimit != 0 && qhChannels.count() >= iChannelCountLimit) {
			PERM_DENIED_FALLBACK(ChannelCountLimit, Version::fromComponents(1, 3, 0),
								 QLatin1String("Channel count limit reached"));
			return;
		}

		ChanACL::Perm perm = msg.temporary() ? ChanACL::MakeTempChannel : ChanACL::MakeChannel;
		if (!hasPermission(uSource, p, perm)) {
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

		if (!hasPermission(uSource, c, ChanACL::Write)) {
			ChanACL *a    = new ChanACL(c);
			a->bApplyHere = true;
			a->bApplySubs = false;
			if (uSource->iId >= 0)
				a->iUserId = uSource->iId;
			else
				a->qsGroup = QLatin1Char('$') + uSource->qsHash;
			a->pDeny  = ChanACL::None;
			a->pAllow = ChanACL::Write | ChanACL::Traverse;

			clearACLCache();
		}
		updateChannel(c);

		msg.set_channel_id(c->iId);
		log(uSource, QString("Added channel %1 under %2").arg(QString(*c), QString(*p)));
		emit channelCreated(c);

		sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
		if (!c->qbaDescHash.isEmpty()) {
			msg.clear_description();
			msg.set_description_hash(blob(c->qbaDescHash));
		}
		sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);

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
		if (!qsName.isNull()) {
			if (!hasPermission(uSource, c, ChanACL::Write) || (c->iId == 0)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}
		if (!qsDesc.isNull()) {
			if (!hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}
		if (msg.has_position()) {
			if (!hasPermission(uSource, c, ChanACL::Write)) {
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

			if (!hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}

			QFlags< ChanACL::Perm > parentMakePermission =
				c->bTemporary ? ChanACL::MakeTempChannel : ChanACL::MakeChannel;
			if (!hasPermission(uSource, p, parentMakePermission)) {
				PERM_DENIED(uSource, p, parentMakePermission);
				return;
			}

			QString name = qsName.isNull() ? c->qsName : qsName;

			foreach (Channel *sibling, p->qlChannels) {
				if (sibling->qsName == name) {
					PERM_DENIED_TYPE(ChannelName);
					return;
				}
			}
		}
		QList< Channel * > qlAdd;
		QList< Channel * > qlRemove;

		if (msg.links_add_size() || msg.links_remove_size()) {
			if (!hasPermission(uSource, c, ChanACL::LinkChannel)) {
				PERM_DENIED(uSource, c, ChanACL::LinkChannel);
				return;
			}
			if (msg.links_remove_size()) {
				for (int i = 0; i < msg.links_remove_size(); ++i) {
					unsigned int link = msg.links_remove(i);
					Channel *l        = qhChannels.value(link);
					if (!l)
						return;
					qlRemove << l;
				}
			}
			if (msg.links_add_size()) {
				for (int i = 0; i < msg.links_add_size(); ++i) {
					unsigned int link = msg.links_add(i);
					Channel *l        = qhChannels.value(link);
					if (!l)
						return;
					if (!hasPermission(uSource, l, ChanACL::LinkChannel)) {
						PERM_DENIED(uSource, l, ChanACL::LinkChannel);
						return;
					}
					qlAdd << l;
				}
			}
		}

		if (msg.has_max_users()) {
			if (!hasPermission(uSource, c, ChanACL::Write)) {
				PERM_DENIED(uSource, c, ChanACL::Write);
				return;
			}
		}

		// All permission checks done -- the update is good.

		if (p) {
			log(uSource, QString("Moved channel %1 from %2 to %3").arg(QString(*c), QString(*c->cParent), QString(*p)));

			{
				QWriteLocker wl(&qrwlVoiceThread);
				c->cParent->removeChannel(c);
				p->addChannel(c);
			}
		}
		if (!qsName.isNull()) {
			log(uSource, QString("Renamed channel %1 to %2").arg(QString(*c), QString(qsName)));
			c->qsName = qsName;
		}
		if (!qsDesc.isNull())
			hashAssign(c->qsDesc, c->qbaDescHash, qsDesc);

		if (msg.has_position())
			c->iPosition = msg.position();

		foreach (Channel *l, qlAdd) { addLink(c, l); }
		foreach (Channel *l, qlRemove) { removeLink(c, l); }

		if (msg.has_max_users())
			c->uiMaxUsers = msg.max_users();

		updateChannel(c);
		emit channelStateChanged(c);

		sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::LessThan);
		if (msg.has_description() && !c->qbaDescHash.isEmpty()) {
			msg.clear_description();
			msg.set_description_hash(blob(c->qbaDescHash));
		}
		sendAll(msg, Version::fromComponents(1, 2, 2), Version::CompareMode::AtLeast);
	}
}

void Server::msgChannelRemove(ServerUser *uSource, MumbleProto::ChannelRemove &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	if (!hasPermission(uSource, c, ChanACL::Write) || (c->iId == 0)) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	log(uSource, QString("Removed channel %1").arg(*c));

	removeChannel(c);
}

void Server::msgTextMessage(ServerUser *uSource, MumbleProto::TextMessage &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);
	QMutexLocker qml(&qmCache);

	// For signal userTextMessage (RPC consumers)
	TextMessage tm;

	// List of users to route the message to
	QSet< ServerUser * > users;
	// List of channels used if dest is a tree of channels
	QQueue< Channel * > q;

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

	if (!isTextAllowed(text, changed)) {
		PERM_DENIED_TYPE(TextTooLong);
		return;
	}
	if (text.isEmpty()) {
		return;
	}
	if (changed) {
		msg.set_message(u8(text));
	}

	tm.qsText = text;

	{ // Happy easter
		char m[29] = { 0117, 0160, 0145, 0156, 040,  0164, 0150, 0145, 040, 0160, 0157, 0144, 040, 0142, 0141,
					   0171, 040,  0144, 0157, 0157, 0162, 0163, 054,  040, 0110, 0101, 0114, 056, 0 };
		if (msg.channel_id_size() == 1 && msg.channel_id(0) == 0 && msg.message() == m) {
			PERM_DENIED_TYPE(H9K);
			return;
		}
	}

	msg.set_actor(uSource->uiSession);

	// Send the message to all users that are in (= have joined) OR are
	// "listening" to channels to which the message has been directed to
	for (int i = 0; i < msg.channel_id_size(); ++i) {
		unsigned int id = msg.channel_id(i);

		Channel *c = qhChannels.value(id);
		if (!c) {
			return;
		}

		if (!ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			PERM_DENIED(uSource, c, ChanACL::TextMessage);
			return;
		}

		// Users directly in that channel
		foreach (User *p, c->qlUsers) { users.insert(static_cast< ServerUser * >(p)); }

		// Users only listening in that channel
		foreach (unsigned int session, m_channelListenerManager.getListenersForChannel(c->iId)) {
			ServerUser *currentUser = qhUsers.value(session);
			if (currentUser) {
				users.insert(currentUser);
			}
		}

		tm.qlChannels.append(id);
	}

	// If the message is sent to trees of channels, find all affected channels
	// and append them to q
	for (int i = 0; i < msg.tree_id_size(); ++i) {
		unsigned int id = msg.tree_id(i);

		Channel *c = qhChannels.value(id);
		if (!c) {
			return;
		}

		if (!ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			PERM_DENIED(uSource, c, ChanACL::TextMessage);
			return;
		}

		q.enqueue(c);

		tm.qlTrees.append(id);
	}

	// Go through all channels in q and append all users in those channels
	// to the list of recipients
	// Sub-channels are enqued so they are also checked by a later loop-iteration
	while (!q.isEmpty()) {
		Channel *c = q.dequeue();
		if (ChanACL::hasPermission(uSource, c, ChanACL::TextMessage, &acCache)) {
			foreach (Channel *sub, c->qlChannels) { q.enqueue(sub); }
			// Users directly in that channel
			foreach (User *p, c->qlUsers) { users.insert(static_cast< ServerUser * >(p)); }
			// Users only listening in that channel
			foreach (unsigned int session, m_channelListenerManager.getListenersForChannel(c->iId)) {
				ServerUser *currentUser = qhUsers.value(session);
				if (currentUser) {
					users.insert(currentUser);
				}
			}
		}
	}

	// Go through all users the message is sent to directly
	for (int i = 0; i < msg.session_size(); ++i) {
		unsigned int session = msg.session(i);
		ServerUser *u        = qhUsers.value(session);
		if (u) {
			if (!ChanACL::hasPermission(uSource, u->cChannel, ChanACL::TextMessage, &acCache)) {
				PERM_DENIED(uSource, u->cChannel, ChanACL::TextMessage);
				return;
			}
			users.insert(u);
		}

		tm.qlSessions.append(session);
	}

	// Remove the message sender from the list of users to send the message to
	users.remove(uSource);

	// Actually send the original message to the affected users
	foreach (ServerUser *u, users) { sendMessage(u, msg); }

	// Emit the signal for RPC consumers
	emit userTextMessage(uSource, tm);
}

/// Helper function to log the groups of the given channel.
///
/// @param server A pointer to the server object the provided channel lives on
/// @param c A pointer to the channel the groups should be logged for
/// @param prefix An optional QString that is being printed before the groups
void logGroups(Server *server, const Channel *c, QString prefix = QString()) {
	if (!prefix.isEmpty()) {
		server->log(prefix);
	}

	if (c->qhGroups.isEmpty()) {
		server->log(QString::fromLatin1("Channel %1 (%2) has no groups set").arg(c->qsName).arg(c->iId));
		return;
	} else {
		server->log(QString::fromLatin1("%1Listing groups specified for channel \"%2\" (%3)...")
						.arg(prefix.isEmpty() ? QLatin1String("") : QLatin1String("\t"))
						.arg(c->qsName)
						.arg(c->iId));
	}

	foreach (Group *currentGroup, c->qhGroups) {
		QString memberList;
		foreach (int m, currentGroup->members()) {
			memberList += QString::fromLatin1("\"%1\"").arg(server->getUserName(m));
			memberList += ", ";
		}

		if (currentGroup->members().size() > 0) {
			memberList.remove(memberList.length() - 2, 2);
			server->log(QString::fromLatin1("%1Group: \"%2\" contains following users: %3")
							.arg(prefix.isEmpty() ? QLatin1String("\t") : QLatin1String("\t\t"))
							.arg(currentGroup->qsName)
							.arg(memberList));
		} else {
			server->log(QString::fromLatin1("%1Group \"%2\" doesn't contain any users")
							.arg(prefix.isEmpty() ? QLatin1String("\t") : QLatin1String("\t\t"))
							.arg(currentGroup->qsName));
		}
	}
}

/// Helper function to log the ACLs of the given channel.
///
/// @param server A pointer to the server object the provided channel lives on
/// @param c A pointer to the channel the ACLs should be logged for
/// @param prefix An optional QString that is being printed before the ACLs
void logACLs(Server *server, const Channel *c, QString prefix = QString()) {
	if (!prefix.isEmpty()) {
		server->log(prefix);
	}

	foreach (const ChanACL *a, c->qlACL) {
		server->log(QString::fromLatin1("%1%2")
						.arg(prefix.isEmpty() ? QLatin1String("") : QLatin1String("\t"))
						.arg(static_cast< QString >(*a)));
	}
}


void Server::msgACL(ServerUser *uSource, MumbleProto::ACL &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	if (!hasPermission(uSource, c, ChanACL::Write)
		&& !(c->cParent && hasPermission(uSource, c->cParent, ChanACL::Write))) {
		PERM_DENIED(uSource, c, ChanACL::Write);
		return;
	}

	RATELIMIT(uSource);

	if (msg.has_query() && msg.query()) {
		QStack< Channel * > chans;
		Channel *p;
		ChanACL *acl;

		QSet< unsigned int > qsId;

		msg.clear_groups();
		msg.clear_acls();
		msg.clear_query();
		msg.set_inherit_acls(c->bInheritACL);

		p = c;
		while (p) {
			chans.push(p);
			if ((p == c) || p->bInheritACL)
				p = p->cParent;
			else
				p = nullptr;
		}

		while (!chans.isEmpty()) {
			p = chans.pop();
			foreach (acl, p->qlACL) {
				if ((p == c) || (acl->bApplySubs)) {
					MumbleProto::ACL_ChanACL *mpacl = msg.add_acls();

					mpacl->set_inherited(p != c);
					mpacl->set_apply_here(acl->bApplyHere);
					mpacl->set_apply_subs(acl->bApplySubs);
					if (acl->iUserId >= 0) {
						mpacl->set_user_id(static_cast< unsigned int >(acl->iUserId));
						qsId.insert(static_cast< unsigned int >(acl->iUserId));
					} else
						mpacl->set_group(u8(acl->qsGroup));
					mpacl->set_grant(acl->pAllow);
					mpacl->set_deny(acl->pDeny);
				}
			}
		}

		p                        = c->cParent;
		QSet< QString > allnames = Group::groupNames(c);
		foreach (const QString &name, allnames) {
			Group *g  = c->qhGroups.value(name);
			Group *pg = p ? Group::getGroup(p, name) : nullptr;

			MumbleProto::ACL_ChanGroup *group = msg.add_groups();
			group->set_name(u8(name));
			group->set_inherit(g ? g->bInherit : true);
			group->set_inheritable(g ? g->bInheritable : true);
			group->set_inherited(pg && pg->bInheritable);
			if (g) {
				foreach (int id, g->qsAdd) {
					qsId.insert(static_cast< unsigned int >(id));
					group->add_add(static_cast< unsigned int >(id));
				}
				foreach (int id, g->qsRemove) {
					qsId.insert(static_cast< unsigned int >(id));
					group->add_remove(static_cast< unsigned int >(id));
				}
			}
			if (pg) {
				foreach (int id, pg->members()) {
					qsId.insert(static_cast< unsigned int >(id));
					group->add_inherited_members(static_cast< unsigned int >(id));
				}
			}
		}

		sendMessage(uSource, msg);

		MumbleProto::QueryUsers mpqu;
		foreach (unsigned int id, qsId) {
			QString uname = getUserName(static_cast< int >(id));
			if (!uname.isEmpty()) {
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

			QHash< QString, QSet< int > > hOldTemp;

			if (Meta::mp.bLogGroupChanges || Meta::mp.bLogACLChanges) {
				log(uSource, QString::fromLatin1("Updating ACL in channel %1").arg(*c));
			}

			if (Meta::mp.bLogGroupChanges) {
				logGroups(this, c, QLatin1String("These are the groups before applying the change:"));
			}

			foreach (g, c->qhGroups) {
				hOldTemp.insert(g->qsName, g->qsTemporary);
				delete g;
			}

			if (Meta::mp.bLogACLChanges) {
				logACLs(this, c, QLatin1String("These are the ACLs before applying the changed:"));
			}

			// Clear old ACLs
			foreach (a, c->qlACL) { delete a; }

			c->qhGroups.clear();
			c->qlACL.clear();

			c->bInheritACL = msg.inherit_acls();

			// Add new groups
			for (int i = 0; i < msg.groups_size(); ++i) {
				const MumbleProto::ACL_ChanGroup &group = msg.groups(i);
				g                                       = new Group(c, u8(group.name()));
				g->bInherit                             = group.inherit();
				g->bInheritable                         = group.inheritable();
				for (int j = 0; j < group.add_size(); ++j)
					if (!getUserName(static_cast< int >(group.add(j))).isEmpty())
						g->qsAdd << static_cast< int >(group.add(j));
				for (int j = 0; j < group.remove_size(); ++j)
					if (!getUserName(static_cast< int >(group.remove(j))).isEmpty())
						g->qsRemove << static_cast< int >(group.remove(j));

				g->qsTemporary = hOldTemp.value(g->qsName);
			}

			if (Meta::mp.bLogGroupChanges) {
				logGroups(this, c, QLatin1String("And these are the new groups:"));
			}

			// Add new ACLs
			for (int i = 0; i < msg.acls_size(); ++i) {
				const MumbleProto::ACL_ChanACL &mpacl = msg.acls(i);
				if (mpacl.has_user_id() && getUserName(static_cast< int >(mpacl.user_id())).isEmpty())
					continue;

				a             = new ChanACL(c);
				a->bApplyHere = mpacl.apply_here();
				a->bApplySubs = mpacl.apply_subs();
				if (mpacl.has_user_id())
					a->iUserId = static_cast< int >(mpacl.user_id());
				else
					a->qsGroup = u8(mpacl.group());
				a->pDeny  = static_cast< ChanACL::Permissions >(mpacl.deny()) & ChanACL::All;
				a->pAllow = static_cast< ChanACL::Permissions >(mpacl.grant()) & ChanACL::All;
			}

			if (Meta::mp.bLogACLChanges) {
				logACLs(this, c, QLatin1String("And these are the new ACLs:"));
			}
		}

		clearACLCache();

		if (!hasPermission(uSource, c, ChanACL::Write) && ((uSource->iId >= 0) || !uSource->qsHash.isEmpty())) {
			{
				QWriteLocker wl(&qrwlVoiceThread);

				a             = new ChanACL(c);
				a->bApplyHere = true;
				a->bApplySubs = false;
				if (uSource->iId >= 0)
					a->iUserId = uSource->iId;
				else
					a->qsGroup = QLatin1Char('$') + uSource->qsHash;
				a->iUserId = uSource->iId;
				a->pDeny   = ChanACL::None;
				a->pAllow  = ChanACL::Write | ChanACL::Traverse;
			}

			clearACLCache();
		}


		updateChannel(c);
		log(uSource, QString("Updated ACL in channel %1").arg(*c));

		// Send refreshed enter states of this channel to all clients
		MumbleProto::ChannelState mpcs;
		mpcs.set_channel_id(c->iId);

		for (ServerUser *user : qhUsers) {
			mpcs.set_is_enter_restricted(isChannelEnterRestricted(c));
			mpcs.set_can_enter(hasPermission(user, c, ChanACL::Enter));

			sendMessage(user, mpcs);
		}
	}
}

void Server::msgQueryUsers(ServerUser *uSource, MumbleProto::QueryUsers &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	MumbleProto::QueryUsers reply;

	for (int i = 0; i < msg.ids_size(); ++i) {
		unsigned int id     = msg.ids(i);
		const QString &name = getUserName(static_cast< int >(id));
		if (!name.isEmpty()) {
			reply.add_ids(id);
			reply.add_names(u8(name));
		}
	}

	for (int i = 0; i < msg.names_size(); ++i) {
		QString name = u8(msg.names(i));
		int id       = getUserID(name);
		if (id >= 0) {
			name = getUserName(id);
			reply.add_ids(static_cast< unsigned int >(id));
			reply.add_names(u8(name));
		}
	}

	sendMessage(uSource, reply);
}

void Server::msgPing(ServerUser *uSource, MumbleProto::Ping &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	QMutexLocker l(&uSource->qmCrypt);

	uSource->csCrypt->uiRemoteGood   = msg.good();
	uSource->csCrypt->uiRemoteLate   = msg.late();
	uSource->csCrypt->uiRemoteLost   = msg.lost();
	uSource->csCrypt->uiRemoteResync = msg.resync();

	uSource->dUDPPingAvg  = msg.udp_ping_avg();
	uSource->dUDPPingVar  = msg.udp_ping_var();
	uSource->uiUDPPackets = msg.udp_packets();
	uSource->dTCPPingAvg  = msg.tcp_ping_avg();
	uSource->dTCPPingVar  = msg.tcp_ping_var();
	uSource->uiTCPPackets = msg.tcp_packets();

	quint64 ts = msg.timestamp();

	msg.Clear();
	msg.set_timestamp(ts);
	msg.set_good(uSource->csCrypt->uiGood);
	msg.set_late(uSource->csCrypt->uiLate);
	msg.set_lost(uSource->csCrypt->uiLost);
	msg.set_resync(uSource->csCrypt->uiResync);

	sendMessage(uSource, msg);
}

void Server::msgCryptSetup(ServerUser *uSource, MumbleProto::CryptSetup &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	QMutexLocker l(&uSource->qmCrypt);

	if (!msg.has_client_nonce()) {
		log(uSource, "Requested crypt-nonce resync");
		msg.set_server_nonce(uSource->csCrypt->getEncryptIV());
		sendMessage(uSource, msg);
	} else {
		const std::string &str = msg.client_nonce();
		uSource->csCrypt->uiResync++;
		if (!uSource->csCrypt->setDecryptIV(str)) {
			qWarning("Messages: Cipher resync failed: Invalid nonce from the client!");
		}
	}
}

void Server::msgContextActionModify(ServerUser *, MumbleProto::ContextActionModify &) {
}

void Server::msgContextAction(ServerUser *uSource, MumbleProto::ContextAction &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	unsigned int session = msg.has_session() ? msg.session() : 0;
	int id               = msg.has_channel_id() ? static_cast< int >(msg.channel_id()) : -1;

	if (session && !qhUsers.contains(session))
		return;
	if ((id >= 0) && !qhChannels.contains(static_cast< unsigned int >(id)))
		return;
	emit contextAction(uSource, u8(msg.action()), session, id);
}

/// @param str The std::string to convert
/// @param maxSize The maximum allowed size for this string
/// @returns The given std::string converted to a QString, if its size is less
///  	than or equal to the given maxSize. If it is bigger, "[[Invalid]]"
///  	is returned.
QString convertWithSizeRestriction(const std::string &str, size_t maxSize) {
	if (str.size() > maxSize) {
		return QLatin1String("[[Invalid]]");
	}

	return QString::fromStdString(str);
}

void Server::msgVersion(ServerUser *uSource, MumbleProto::Version &msg) {
	ZoneScoped;

	RATELIMIT(uSource);

	uSource->m_version = MumbleProto::getVersion(msg);
	if (msg.has_release()) {
		uSource->qsRelease = convertWithSizeRestriction(msg.release(), 100);
	}
	if (msg.has_os()) {
		uSource->qsOS = convertWithSizeRestriction(msg.os(), 40);

		if (msg.has_os_version()) {
			uSource->qsOSVersion = convertWithSizeRestriction(msg.os_version(), 60);
		}
	}

	log(uSource, QString("Client version %1 (%2 %3: %4)")
					 .arg(Version::toString(uSource->m_version))
					 .arg(uSource->qsOS)
					 .arg(uSource->qsOSVersion)
					 .arg(uSource->qsRelease));
}

void Server::msgUserList(ServerUser *uSource, MumbleProto::UserList &msg) {
	ZoneScoped;

	MSG_SETUP(ServerUser::Authenticated);

	// The register permission is required on the root channel to be allowed to
	// view the registered users.
	if (!hasPermission(uSource, qhChannels.value(0), ChanACL::Register)) {
		PERM_DENIED(uSource, qhChannels.value(0), ChanACL::Register);
		return;
	}

	if (msg.users_size() == 0) {
		// Query mode.
		QList< UserInfo > users              = getRegisteredUsersEx();
		QList< UserInfo >::const_iterator it = users.constBegin();
		for (; it != users.constEnd(); ++it) {
			// Skip the SuperUser
			if (it->user_id > 0) {
				::MumbleProto::UserList_User *user = msg.add_users();
				user->set_user_id(static_cast< unsigned int >(it->user_id));
				user->set_name(u8(it->name));
				if (it->last_channel >= 0) {
					user->set_last_channel(static_cast< unsigned int >(*it->last_channel));
				}
				user->set_last_seen(u8(it->last_active.toString(Qt::ISODate)));
			}
		}
		sendMessage(uSource, msg);
	} else {
		// Update mode
		for (int i = 0; i < msg.users_size(); ++i) {
			const MumbleProto::UserList_User &user = msg.users(i);

			unsigned int id = user.user_id();
			if (id == 0)
				continue;

			if (!user.has_name()) {
				log(uSource, QString::fromLatin1("Unregistered user %1").arg(id));
				unregisterUser(static_cast< int >(id));
			} else {
				const QString &name = u8(user.name()).trimmed();
				if (validateUserName(name)) {
					log(uSource, QString::fromLatin1("Renamed user %1 to '%2'").arg(QString::number(id), name));

					QMap< int, QString > info;
					info.insert(ServerDB::User_Name, name);
					setInfo(static_cast< int >(id), info);

					MumbleProto::UserState mpus;
					foreach (ServerUser *serverUser, qhUsers) {
						if (serverUser->iId == static_cast< int >(id)) {
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
					if (uSource->m_version < Version::fromComponents(1, 2, 1))
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
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	int target = static_cast< int >(msg.id());
	if ((target < 1) || (target >= 0x1f))
		return;

	QWriteLocker lock(&qrwlVoiceThread);

	uSource->qmTargetCache.remove(target);

	int count = msg.targets_size();
	if (count == 0) {
		uSource->qmTargets.remove(target);
	} else {
		WhisperTarget wt;
		for (int i = 0; i < count; ++i) {
			const MumbleProto::VoiceTarget_Target &t = msg.targets(i);
			for (int j = 0; j < t.session_size(); ++j) {
				unsigned int s = t.session(j);
				if (qhUsers.contains(s)) {
					wt.sessions.push_back(s);
				}
			}
			if (t.has_channel_id()) {
				unsigned int id = t.channel_id();
				if (qhChannels.contains(id)) {
					WhisperTarget::Channel wtc;
					wtc.id              = id;
					wtc.includeChildren = t.children();
					wtc.includeLinks    = t.links();
					if (t.has_group()) {
						wtc.targetGroup = u8(t.group());
					}

					wt.channels.push_back(wtc);
				}
			}
		}
		if (wt.sessions.empty() && wt.channels.empty()) {
			uSource->qmTargets.remove(target);
		} else {
			uSource->qmTargets.insert(target, std::move(wt));
		}
	}
}

void Server::msgPermissionQuery(ServerUser *uSource, MumbleProto::PermissionQuery &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	Channel *c = qhChannels.value(msg.channel_id());
	if (!c)
		return;

	sendClientPermission(uSource, c, true);
}

void Server::msgCodecVersion(ServerUser *, MumbleProto::CodecVersion &) {
}

void Server::msgUserStats(ServerUser *uSource, MumbleProto::UserStats &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);
	VICTIM_SETUP;
	const BandwidthRecord &bwr            = pDstServerUser->bwr;
	const QList< QSslCertificate > &certs = pDstServerUser->peerCertificateChain();

	bool extend = (uSource == pDstServerUser) || hasPermission(uSource, qhChannels.value(0), ChanACL::Register);

	if (!extend && !hasPermission(uSource, pDstServerUser->cChannel, ChanACL::Enter)) {
		PERM_DENIED(uSource, pDstServerUser->cChannel, ChanACL::Enter);
		return;
	}

	bool details = extend;
	bool local   = extend || (pDstServerUser->cChannel == uSource->cChannel);

	if (msg.stats_only())
		details = false;

	msg.Clear();
	msg.set_session(pDstServerUser->uiSession);

	if (details) {
		foreach (const QSslCertificate &cert, certs) {
			const QByteArray &der = cert.toDer();
			msg.add_certificates(blob(der));
		}
		msg.set_strong_certificate(pDstServerUser->bVerified);
	}

	if (local) {
		MumbleProto::UserStats_Stats *mpusss;

		QMutexLocker l(&pDstServerUser->qmCrypt);

		mpusss = msg.mutable_from_client();
		mpusss->set_good(pDstServerUser->csCrypt->uiGood);
		mpusss->set_late(pDstServerUser->csCrypt->uiLate);
		mpusss->set_lost(pDstServerUser->csCrypt->uiLost);
		mpusss->set_resync(pDstServerUser->csCrypt->uiResync);

		mpusss = msg.mutable_from_server();
		mpusss->set_good(pDstServerUser->csCrypt->uiRemoteGood);
		mpusss->set_late(pDstServerUser->csCrypt->uiRemoteLate);
		mpusss->set_lost(pDstServerUser->csCrypt->uiRemoteLost);
		mpusss->set_resync(pDstServerUser->csCrypt->uiRemoteResync);
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
		if (pDstServerUser->m_version != Version::UNKNOWN) {
			MumbleProto::setVersion(*mpv, pDstServerUser->m_version);
		}
		if (!pDstServerUser->qsRelease.isEmpty()) {
			mpv->set_release(u8(pDstServerUser->qsRelease));
		}
		if (!pDstServerUser->qsOS.isEmpty()) {
			mpv->set_os(u8(pDstServerUser->qsOS));
			if (!pDstServerUser->qsOSVersion.isEmpty())
				mpv->set_os_version(u8(pDstServerUser->qsOSVersion));
		}

		foreach (int v, pDstServerUser->qlCodecs)
			msg.add_celt_versions(v);
		msg.set_opus(pDstServerUser->bOpus);

		msg.set_address(pDstServerUser->haAddress.toStdString());
	}

	if (local)
		msg.set_bandwidth(static_cast< unsigned int >(bwr.bandwidth()));
	msg.set_onlinesecs(static_cast< unsigned int >(bwr.onlineSeconds()));
	if (local)
		msg.set_idlesecs(static_cast< unsigned int >(bwr.idleSeconds()));

	sendMessage(uSource, msg);
}

void Server::msgRequestBlob(ServerUser *uSource, MumbleProto::RequestBlob &msg) {
	ZoneScoped;

	MSG_SETUP_NO_UNIDLE(ServerUser::Authenticated);

	int ntextures     = msg.session_texture_size();
	int ncomments     = msg.session_comment_size();
	int ndescriptions = msg.channel_description_size();

	if (ndescriptions) {
		MumbleProto::ChannelState mpcs;
		for (int i = 0; i < ndescriptions; ++i) {
			unsigned int id = msg.channel_description(i);
			Channel *c      = qhChannels.value(id);
			if (c && !c->qsDesc.isEmpty()) {
				mpcs.set_channel_id(id);
				mpcs.set_description(u8(c->qsDesc));
				sendMessage(uSource, mpcs);
			}
		}
	}
	if (ntextures || ncomments) {
		MumbleProto::UserState mpus;
		for (int i = 0; i < ntextures; ++i) {
			unsigned int session = msg.session_texture(i);
			ServerUser *su       = qhUsers.value(session);
			if (su && !su->qbaTexture.isEmpty()) {
				mpus.set_session(session);
				mpus.set_texture(blob(su->qbaTexture));
				sendMessage(uSource, mpus);
			}
		}
		if (ntextures)
			mpus.clear_texture();
		for (int i = 0; i < ncomments; ++i) {
			unsigned int session = msg.session_comment(i);
			ServerUser *su       = qhUsers.value(session);
			if (su && !su->qsComment.isEmpty()) {
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

void Server::msgPluginDataTransmission(ServerUser *sender, MumbleProto::PluginDataTransmission &msg) {
	ZoneScoped;

	// A client's plugin has sent us a message that we shall delegate to its receivers

	if (sender->m_pluginMessageBucket.ratelimit(1)) {
		qWarning("Dropping plugin message sent from \"%s\" (%d)", qUtf8Printable(sender->qsName), sender->uiSession);
		return;
	}

	if (!msg.has_data() || !msg.has_dataid()) {
		// Messages without data and/or without a data ID can't be used by the clients. Thus we don't even have to send
		// them
		return;
	}

	if (msg.data().size() > Mumble::Plugins::PluginMessage::MAX_DATA_LENGTH) {
		qWarning("Dropping plugin message sent from \"%s\" (%d) - data too large", qUtf8Printable(sender->qsName),
				 sender->uiSession);
		return;
	}
	if (msg.dataid().size() > Mumble::Plugins::PluginMessage::MAX_DATA_ID_LENGTH) {
		qWarning("Dropping plugin message sent from \"%s\" (%d) - data ID too long", qUtf8Printable(sender->qsName),
				 sender->uiSession);
		return;
	}

	// Always set the sender's session and don't rely on it being set correctly (would
	// allow spoofing the sender's session)
	msg.set_sendersession(sender->uiSession);

	// Copy needed data from message in order to be able to remove info about receivers from the message as this doesn't
	// matter for the client
	size_t receiverAmount = static_cast< std::size_t >(msg.receiversessions_size());
	const ::google::protobuf::RepeatedField<::google::protobuf::uint32 > receiverSessions = msg.receiversessions();

	msg.clear_receiversessions();

	QSet< uint32_t > uniqueReceivers;
	uniqueReceivers.reserve(receiverSessions.size());

	for (int i = 0; static_cast< size_t >(i) < receiverAmount; i++) {
		uint32_t userSession = receiverSessions.Get(i);

		if (!uniqueReceivers.contains(userSession)) {
			uniqueReceivers.insert(userSession);
		} else {
			// Duplicate entry -> ignore
			continue;
		}

		ServerUser *receiver = qhUsers.value(receiverSessions.Get(i));

		if (receiver) {
			// We can simply redirect the message we have received to the clients
			sendMessage(receiver, msg);
		}
	}
}

#undef RATELIMIT
#undef MSG_SETUP
#undef MSG_SETUP_NO_UNIDLE
#undef VICTIM_SETUP
#undef PERM_DENIED
#undef PERM_DENIED_TYPE
#undef PERM_DENIED_FALLBACK
#undef PERM_DENIED_HASH
