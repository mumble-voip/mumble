// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
# include "win.h"
#endif

#include "Channel.h"
#include "Group.h"
#include "Meta.h"
#include "Server.h"
#include "ServerDB.h"
#include "ServerUser.h"
#include "Version.h"

#ifdef Q_OS_WIN
# include <winsock2.h>
#endif

void Server::setUserState(User *pUser, Channel *cChannel, bool mute, bool deaf, bool suppressed, bool prioritySpeaker, const QString& name, const QString &comment) {
	bool changed = false;

	if (deaf)
		mute = true;
	if (! mute)
		deaf = false;

	MumbleProto::UserState mpus;
	mpus.set_session(pUser->uiSession);
	if (mute != pUser->bMute) {
		changed = true;
		mpus.set_mute(mute);
	}
	if (deaf != pUser->bDeaf) {
		changed = true;
		mpus.set_deaf(deaf);
	}
	if (suppressed != pUser->bSuppress) {
		changed = true;
		mpus.set_suppress(suppressed);
	}
	if (prioritySpeaker != pUser->bPrioritySpeaker) {
		changed = true;
		mpus.set_priority_speaker(prioritySpeaker);
	}
	if (comment != pUser->qsComment) {
		changed = true;
		mpus.set_comment(u8(comment));
		if (pUser->iId >= 0) {
			QMap<int, QString> info;
			info.insert(ServerDB::User_Comment, comment);
			setInfo(pUser->iId, info);
		}
	}
	if (name != pUser->qsName) {
		changed = true;
		mpus.set_name(u8(name));
	}

	{
		QWriteLocker wl(&qrwlVoiceThread);
		pUser->bDeaf = deaf;
		pUser->bMute = mute;
		pUser->bSuppress = suppressed;
	}

	pUser->bPrioritySpeaker = prioritySpeaker;
	pUser->qsName = name;
	hashAssign(pUser->qsComment, pUser->qbaCommentHash, comment);

	if (cChannel != pUser->cChannel) {
		changed = true;
		mpus.set_channel_id(cChannel->iId);
		userEnterChannel(pUser, cChannel, mpus);
	}

	if (changed) {
		sendAll(mpus, ~ 0x010202);
		if (mpus.has_comment() && ! pUser->qbaCommentHash.isEmpty()) {
			mpus.clear_comment();
			mpus.set_comment_hash(blob(pUser->qbaCommentHash));
		}
		sendAll(mpus, 0x010202);

		emit userStateChanged(pUser);
	}
}

// Sets err to error message on failure.
bool Server::setChannelStateGRPC(const MumbleProto::ChannelState &cs, QString &err) {
	::MumbleProto::ChannelState mpcs;
	bool changed = false;
	bool updated = false;

	if (!cs.has_channel_id()) {
		err = QLatin1String("missing channel ID");
		return false;
	}
	Channel *channel = qhChannels.value(cs.channel_id());
	if (!channel) {
		err = QLatin1String("invalid channel");
		return false;
	}
	mpcs.set_channel_id(cs.channel_id());

	// Links and parent channel are processed first, because they can return
	// errors. Without doing this, the server state can be changed without
	// notifying users.
	QSet< ::Channel *> newLinksSet;
	for (int i = 0; i < cs.links_size(); i++) {
		Channel *link = qhChannels.value(cs.links(i));
		if (!link) {
			err = QLatin1String("invalid channel link");
			return false;
		}
		newLinksSet.insert(link);
	}

	if (cs.has_parent()) {
		Channel *parent = qhChannels.value(cs.parent());
		if (!parent) {
			err = QLatin1String("invalid parent channel");
			return false;
		}
		if (parent != channel->cParent) {
			Channel *p = parent;
			while (p) {
				if (p == channel) {
					err = QLatin1String("parent channel cannot be a descendant of channel");
					return false;
				}
				p = p->cParent;
			}
			if (!canNest(parent, channel)) {
				err = QLatin1String("channel cannot be nested in the given parent");
				return false;
			}

			{
				QWriteLocker wl(&qrwlVoiceThread);
				channel->cParent->removeChannel(channel);
				parent->addChannel(channel);
			}

			mpcs.set_parent(parent->iId);

			changed = true;
			updated = true;
		}
	}

	if (cs.has_name()) {
		QString qsName = u8(cs.name());
		if (channel->qsName != qsName) {
			channel->qsName = qsName;
			mpcs.set_name(cs.name());

			changed = true;
			updated = true;
		}
	}

	const QSet<Channel *> &oldLinksSet = channel->qsPermLinks;

	if (newLinksSet != oldLinksSet) {
		// Remove
		foreach(Channel *l, oldLinksSet) {
			if (!newLinksSet.contains(l)) {
				removeLink(channel, l);
				mpcs.add_links_remove(l->iId);
			}
		}
		// Add
		foreach(Channel *l, newLinksSet) {
			if (! oldLinksSet.contains(l)) {
				addLink(channel, l);
				mpcs.add_links_add(l->iId);
			}
		}

		changed = true;
	}

	if (cs.has_position() && cs.position() != channel->iPosition) {
		channel->iPosition = cs.position();
		mpcs.set_position(cs.position());

		changed = true;
		updated = true;
	}

	if (cs.has_description()) {
		QString qsDescription = u8(cs.description());
		if (qsDescription != channel->qsDesc) {
			hashAssign(channel->qsDesc, channel->qbaDescHash, qsDescription);
			mpcs.set_description(cs.description());

			changed = true;
			updated = true;
		}
	}

	if (updated) {
		updateChannel(channel);
	}
	if (changed) {
		sendAll(mpcs, ~ 0x010202);
		if (mpcs.has_description() && !channel->qbaDescHash.isEmpty()) {
			mpcs.clear_description();
			mpcs.set_description_hash(blob(channel->qbaDescHash));
		}
		sendAll(mpcs, 0x010202);
		emit channelStateChanged(channel);
	}

	return true;
}

bool Server::setChannelState(Channel *cChannel, Channel *cParent, const QString &qsName, const QSet<Channel *> &links, const QString &desc, const int position) {
	bool changed = false;
	bool updated = false;

	MumbleProto::ChannelState mpcs;
	mpcs.set_channel_id(cChannel->iId);

	if (cChannel->qsName != qsName) {
		cChannel->qsName = qsName;
		mpcs.set_name(u8(qsName));
		updated = true;
		changed = true;
	}

	if ((cParent != cChannel) && (cParent != cChannel->cParent)) {
		Channel *p = cParent;
		while (p) {
			if (p == cChannel)
				return false;
			p = p->cParent;
		}

		if (!canNest(cParent, cChannel)) {
			return false;
		}

		{
			QWriteLocker wl(&qrwlVoiceThread);
			cChannel->cParent->removeChannel(cChannel);
			cParent->addChannel(cChannel);
		}

		mpcs.set_parent(cParent->iId);

		updated = true;
		changed = true;
	}

	const QSet<Channel *> &oldset = cChannel->qsPermLinks;

	if (links != oldset) {
		// Remove
		foreach(Channel *l, oldset) {
			if (! links.contains(l)) {
				removeLink(cChannel, l);
				mpcs.add_links_remove(l->iId);
			}
		}

		// Add
		foreach(Channel *l, links) {
			if (! oldset.contains(l)) {
				addLink(cChannel, l);
				mpcs.add_links_add(l->iId);
			}
		}

		changed = true;
	}

	if (position != cChannel->iPosition) {
		changed = true;
		updated = true;
		cChannel->iPosition = position;
		mpcs.set_position(position);
	}

	if (! desc.isNull() && desc != cChannel->qsDesc) {
		updated = true;
		changed = true;
		hashAssign(cChannel->qsDesc, cChannel->qbaDescHash, desc);
		mpcs.set_description(u8(desc));
	}

	if (updated)
		updateChannel(cChannel);
	if (changed) {
		sendAll(mpcs, ~ 0x010202);
		if (mpcs.has_description() && ! cChannel->qbaDescHash.isEmpty()) {
			mpcs.clear_description();
			mpcs.set_description_hash(blob(cChannel->qbaDescHash));
		}
		sendAll(mpcs, 0x010202);
		emit channelStateChanged(cChannel);
	}

	return true;
}

void Server::sendTextMessageGRPC(const ::MumbleProto::TextMessage &tm) {
	MumbleProto::TextMessage mptm;
	mptm.set_message(tm.message());

	if (tm.has_actor()) {
		mptm.set_actor(tm.actor());
	}

	// Broadcast
	if (!tm.session_size() && !tm.channel_id_size() && !tm.tree_id_size()) {
		sendAll(mptm);
		return;
	}

	// Single targets
	for (int i = 0; i < tm.session_size(); i++) {
		ServerUser *user = qhUsers.value(tm.session(i));
		if (!user) {
			continue;
		}
		mptm.add_session(user->uiSession);
		sendMessage(user, mptm);
		mptm.clear_session();
	}

	// Channel targets
	QSet<Channel *> chans;

	for (int i = 0; i < tm.channel_id_size(); i++) {
		Channel *channel = qhChannels.value(tm.channel_id(i));
		if (!channel) {
			continue;
		}
		chans.insert(channel);
		mptm.add_channel_id(channel->iId);
	}

	QQueue<Channel *> chansQ;
	for (int i = 0; i < tm.tree_id_size(); i++) {
		Channel *channel = qhChannels.value(tm.tree_id(i));
		if (!channel) {
			continue;
		}
		chansQ.enqueue(channel);
		mptm.add_tree_id(channel->iId);
	}
	while (!chansQ.isEmpty()) {
		Channel *c = chansQ.dequeue();
		chans.insert(c);
		foreach(c, c->qlChannels) {
			chansQ.enqueue(c);
		}
	}

	foreach(Channel *c, chans) {
		foreach(::User *p, c->qlUsers) {
			sendMessage(static_cast< ::ServerUser *>(p), mptm);
		}
	}
}

void Server::sendTextMessage(Channel *cChannel, ServerUser *pUser, bool tree, const QString &text) {
	MumbleProto::TextMessage mptm;
	mptm.set_message(u8(text));

	if (pUser) {
		mptm.add_session(pUser->uiSession);
		sendMessage(pUser, mptm);
	} else {
		if (tree)
			mptm.add_tree_id(cChannel->iId);
		else
			mptm.add_channel_id(cChannel->iId);

		QSet<Channel *> chans;
		QQueue<Channel *> q;
		q << cChannel;
		chans.insert(cChannel);
		Channel *c;

		if (tree) {
			while (! q.isEmpty()) {
				c = q.dequeue();
				chans.insert(c);
				foreach(c, c->qlChannels)
					q.enqueue(c);
			}
		}
		foreach(c, chans) {
			foreach(User *p, c->qlUsers)
				sendMessage(static_cast<ServerUser *>(p), mptm);
		}
	}
}

/**
 * Sets the list of temporary groups the given userid is a member of. If no channel is given root will
 * be targeted.
 *
 * If userid is negative the absolute value is a session id. If it is positive it is a registration id.
 */
void Server::setTempGroups(int userid, int sessionId, Channel *cChannel, const QStringList &groups) {
	if (! cChannel)
		cChannel = qhChannels.value(0);

	{
		QWriteLocker wl(&qrwlVoiceThread);

		Group *g;
		foreach(g, cChannel->qhGroups) {
			g->qsTemporary.remove(userid);
			if (sessionId != 0)
				g->qsTemporary.remove(- sessionId);
		}

		QString gname;
		foreach(gname, groups) {
			g = cChannel->qhGroups.value(gname);
			if (! g) {
				g = new Group(cChannel, gname);
			}
			g->qsTemporary.insert(userid);
			if (sessionId != 0)
				g->qsTemporary.insert(- sessionId);
		}
	}

	User *p = qhUsers.value(userid);
	if (p)
		clearACLCache(p);
}

/**
 * Clears temporary group memberships for the given User. If no channel is given root will be targeted.
 * If recursion is activated all temporary memberships in related channels will also be cleared.
 */
void Server::clearTempGroups(User *user, Channel *cChannel, bool recurse) {
	QList<Channel*> qlChans;
	if (! cChannel)
		cChannel = qhChannels.value(0);

	qlChans.append(cChannel);

	{
		QWriteLocker wl(&qrwlVoiceThread);

		while (!qlChans.isEmpty()) {
			Channel *chan = qlChans.takeLast();
			Group *g;
			foreach(g, chan->qhGroups) {
				g->qsTemporary.remove(user->iId);
				g->qsTemporary.remove(-static_cast<int>(user->uiSession));
			}

			if (recurse)
				qlChans << chan->qlChannels;
		}
	}

	clearACLCache(user);
}

void Server::connectAuthenticator(QObject *obj) {
	connect(this, SIGNAL(registerUserSig(int &, const QMap<int, QString> &)), obj, SLOT(registerUserSlot(int &, const QMap<int, QString> &)));
	connect(this, SIGNAL(unregisterUserSig(int &, int)), obj, SLOT(unregisterUserSlot(int &, int)));
	connect(this, SIGNAL(getRegisteredUsersSig(const QString &, QMap<int, QString> &)), obj, SLOT(getRegisteredUsersSlot(const QString &, QMap<int, QString> &)));
	connect(this, SIGNAL(getRegistrationSig(int &, int, QMap<int, QString> &)), obj, SLOT(getRegistrationSlot(int &, int, QMap<int, QString> &)));
	connect(this, SIGNAL(authenticateSig(int &, QString &, int, const QList<QSslCertificate> &, const QString &, bool, const QString &)), obj, SLOT(authenticateSlot(int &, QString &, int, const QList<QSslCertificate> &, const QString &, bool, const QString &)));
	connect(this, SIGNAL(setInfoSig(int &, int, const QMap<int, QString> &)), obj, SLOT(setInfoSlot(int &, int, const QMap<int, QString> &)));
	connect(this, SIGNAL(setTextureSig(int &, int, const QByteArray &)), obj, SLOT(setTextureSlot(int &, int, const QByteArray &)));
	connect(this, SIGNAL(idToNameSig(QString &, int)), obj, SLOT(idToNameSlot(QString &, int)));
	connect(this, SIGNAL(nameToIdSig(int &, const QString &)), obj, SLOT(nameToIdSlot(int &, const QString &)));
	connect(this, SIGNAL(idToTextureSig(QByteArray &, int)), obj, SLOT(idToTextureSlot(QByteArray &, int)));
}

void Server::disconnectAuthenticator(QObject *obj) {
	disconnect(this, SIGNAL(registerUserSig(int &, const QMap<int, QString> &)), obj, SLOT(registerUserSlot(int &, const QMap<int, QString> &)));
	disconnect(this, SIGNAL(unregisterUserSig(int &, int)), obj, SLOT(unregisterUserSlot(int &, int)));
	disconnect(this, SIGNAL(getRegisteredUsersSig(const QString &, QMap<int, QString> &)), obj, SLOT(getRegisteredUsersSlot(const QString &, QMap<int, QString> &)));
	disconnect(this, SIGNAL(getRegistrationSig(int &, int, QMap<int, QString> &)), obj, SLOT(getRegistrationSlot(int &, int, QMap<int, QString> &)));
	disconnect(this, SIGNAL(authenticateSig(int &, QString &, int, const QList<QSslCertificate> &, const QString &, bool, const QString &)), obj, SLOT(authenticateSlot(int &, QString &, int, const QList<QSslCertificate> &, const QString &, bool, const QString &)));
	disconnect(this, SIGNAL(setInfoSig(int &, int, const QMap<int, QString> &)), obj, SLOT(setInfoSlot(int &, int, const QMap<int, QString> &)));
	disconnect(this, SIGNAL(setTextureSig(int &, int, const QByteArray &)), obj, SLOT(setTextureSlot(int &, int, const QByteArray &)));
	disconnect(this, SIGNAL(idToNameSig(QString &, int)), obj, SLOT(idToNameSlot(QString &, int)));
	disconnect(this, SIGNAL(nameToIdSig(int &, const QString &)), obj, SLOT(nameToIdSlot(int &, const QString &)));
	disconnect(this, SIGNAL(idToTextureSig(QByteArray &, int)), obj, SLOT(idToTextureSlot(QByteArray &, int)));
}

void Server::connectListener(QObject *obj) {
	connect(this, SIGNAL(userStateChanged(const User *)), obj, SLOT(userStateChanged(const User *)));
	connect(this, SIGNAL(userTextMessage(const User *, const TextMessage &)), obj, SLOT(userTextMessage(const User *, const TextMessage &)));
	connect(this, SIGNAL(userConnected(const User *)), obj, SLOT(userConnected(const User *)));
	connect(this, SIGNAL(userDisconnected(const User *)), obj, SLOT(userDisconnected(const User *)));
	connect(this, SIGNAL(channelStateChanged(const Channel *)), obj, SLOT(channelStateChanged(const Channel *)));
	connect(this, SIGNAL(channelCreated(const Channel *)), obj, SLOT(channelCreated(const Channel *)));
	connect(this, SIGNAL(channelRemoved(const Channel *)), obj, SLOT(channelRemoved(const Channel *)));
}

void Server::disconnectListener(QObject *obj) {
	disconnect(this, SIGNAL(userStateChanged(const User *)), obj, SLOT(userStateChanged(const User *)));
	disconnect(this, SIGNAL(userTextMessage(const User *, const TextMessage &)), obj, SLOT(userTextMessage(const User *, const TextMessage &)));
	disconnect(this, SIGNAL(userConnected(const User *)), obj, SLOT(userConnected(const User *)));
	disconnect(this, SIGNAL(userDisconnected(const User *)), obj, SLOT(userDisconnected(const User *)));
	disconnect(this, SIGNAL(channelStateChanged(const Channel *)), obj, SLOT(channelStateChanged(const Channel *)));
	disconnect(this, SIGNAL(channelCreated(const Channel *)), obj, SLOT(channelCreated(const Channel *)));
	disconnect(this, SIGNAL(channelRemoved(const Channel *)), obj, SLOT(channelRemoved(const Channel *)));
}

void Meta::connectListener(QObject *obj) {
	connect(this, SIGNAL(started(Server *)), obj, SLOT(started(Server *)));
	connect(this, SIGNAL(stopped(Server *)), obj, SLOT(stopped(Server *)));
}

void Meta::getVersion(int &major, int &minor, int &patch, QString &string) {
	string = QLatin1String(MUMBLE_RELEASE);
	major = minor = patch = 0;
	MumbleVersion::get(&major, &minor, &patch);
}
