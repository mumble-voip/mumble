// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ChannelListener.h"
#include "Channel.h"
#include "User.h"

#ifdef MUMBLE
#	include "ServerHandler.h"
#	include "Database.h"
#endif

#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>


#ifdef MUMBLE
// We define a global macro called 'g'. This can lead to issues when included code uses 'g' as a type or parameter name
// (like protobuf 3.7 does). As such, for now, we have to make this our last include.
#	include "Global.h"
#endif

// init static instance
ChannelListener ChannelListener::s_instance;

ChannelListener::ChannelListener()
	: QObject(nullptr), m_listenerLock(), m_listeningUsers(), m_listenedChannels()
#ifdef MUMBLE
	  ,
	  m_volumeLock(), m_listenerVolumeAdjustments(), m_initialSyncDone(false)
#endif
{
}

void ChannelListener::addListenerImpl(unsigned int userSession, int channelID) {
	QWriteLocker lock(&m_listenerLock);

	m_listeningUsers[userSession] << channelID;
	m_listenedChannels[channelID] << userSession;
}

void ChannelListener::removeListenerImpl(unsigned int userSession, int channelID) {
	QWriteLocker lock(&m_listenerLock);

	m_listeningUsers[userSession].remove(channelID);
	m_listenedChannels[channelID].remove(userSession);
}

bool ChannelListener::isListeningImpl(unsigned int userSession, int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID].contains(userSession);
}

bool ChannelListener::isListeningToAnyImpl(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return !m_listeningUsers[userSession].isEmpty();
}

bool ChannelListener::isListenedByAnyImpl(int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return !m_listenedChannels[channelID].isEmpty();
}

const QSet< unsigned int > ChannelListener::getListenersForChannelImpl(int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID];
}

const QSet< int > ChannelListener::getListenedChannelsForUserImpl(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return m_listeningUsers[userSession];
}

int ChannelListener::getListenerCountForChannelImpl(int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID].size();
}

int ChannelListener::getListenedChannelCountForUserImpl(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return m_listeningUsers[userSession].size();
}

#ifdef MUMBLE
void ChannelListener::setListenerLocalVolumeAdjustmentImpl(int channelID, float volumeAdjustment) {
	float oldValue;
	{
		QWriteLocker lock(&m_volumeLock);

		oldValue = m_listenerVolumeAdjustments.value(channelID, 1.0f);
		m_listenerVolumeAdjustments.insert(channelID, volumeAdjustment);
	}

	if (oldValue != volumeAdjustment) {
		emit localVolumeAdjustmentsChanged(channelID, volumeAdjustment, oldValue);
	}
}

float ChannelListener::getListenerLocalVolumeAdjustmentImpl(int channelID) const {
	QReadLocker lock(&m_volumeLock);

	return m_listenerVolumeAdjustments.value(channelID, 1.0f);
}

QHash< int, float > ChannelListener::getAllListenerLocalVolumeAdjustmentsImpl(bool filter) const {
	QReadLocker lock(&m_volumeLock);

	if (!filter) {
		return m_listenerVolumeAdjustments;
	} else {
		QHash< int, float > volumeMap;

		QHashIterator< int, float > it(m_listenerVolumeAdjustments);

		while (it.hasNext()) {
			it.next();

			if (it.value() != 1.0f) {
				volumeMap.insert(it.key(), it.value());
			}
		}

		return volumeMap;
	}
}

void ChannelListener::setInitialServerSyncDoneImpl(bool done) {
	m_initialSyncDone.store(done);
}
#endif

void ChannelListener::clearImpl(){ { QWriteLocker lock(&m_listenerLock);
m_listeningUsers.clear();
m_listenedChannels.clear();
}
#ifdef MUMBLE
{
	QWriteLocker lock(&m_volumeLock);
	m_listenerVolumeAdjustments.clear();
}
#endif
}


ChannelListener &ChannelListener::get() {
	return s_instance;
}


void ChannelListener::addListener(unsigned int userSession, int channelID) {
	get().addListenerImpl(userSession, channelID);
}

void ChannelListener::addListener(const User *user, const Channel *channel) {
	get().addListenerImpl(user->uiSession, channel->iId);
}

void ChannelListener::removeListener(unsigned int userSession, int channelID) {
	get().removeListenerImpl(userSession, channelID);
}

void ChannelListener::removeListener(const User *user, const Channel *channel) {
	get().removeListenerImpl(user->uiSession, channel->iId);
}

bool ChannelListener::isListening(unsigned int userSession, int channelID) {
	return get().isListeningImpl(userSession, channelID);
}

bool ChannelListener::isListening(const User *user, const Channel *channel) {
	return get().isListeningImpl(user->uiSession, channel->iId);
}

bool ChannelListener::isListeningToAny(unsigned int userSession) {
	return get().isListeningToAnyImpl(userSession);
}

bool ChannelListener::isListeningToAny(const User *user) {
	return get().isListeningToAnyImpl(user->uiSession);
}

bool ChannelListener::isListenedByAny(int channelID) {
	return get().isListenedByAnyImpl(channelID);
}

bool ChannelListener::isListenedByAny(const Channel *channel) {
	return get().isListenedByAnyImpl(channel->iId);
}

const QSet< unsigned int > ChannelListener::getListenersForChannel(int channelID) {
	return get().getListenersForChannelImpl(channelID);
}

const QSet< unsigned int > ChannelListener::getListenersForChannel(const Channel *channel) {
	return get().getListenersForChannelImpl(channel->iId);
}

const QSet< int > ChannelListener::getListenedChannelsForUser(unsigned int userSession) {
	return get().getListenedChannelsForUserImpl(userSession);
}

const QSet< int > ChannelListener::getListenedChannelsForUser(const User *user) {
	return get().getListenedChannelsForUserImpl(user->uiSession);
}

int ChannelListener::getListenerCountForChannel(int channelID) {
	return get().getListenerCountForChannelImpl(channelID);
}

int ChannelListener::getListenerCountForChannel(const Channel *channel) {
	return get().getListenerCountForChannelImpl(channel->iId);
}

int ChannelListener::getListenedChannelCountForUser(unsigned int userSession) {
	return get().getListenedChannelCountForUserImpl(userSession);
}

int ChannelListener::getListenedChannelCountForUser(const User *user) {
	return get().getListenedChannelCountForUserImpl(user->uiSession);
}

#ifdef MUMBLE
void ChannelListener::setListenerLocalVolumeAdjustment(int channelID, float volumeAdjustment) {
	get().setListenerLocalVolumeAdjustmentImpl(channelID, volumeAdjustment);
}

void ChannelListener::setListenerLocalVolumeAdjustment(const Channel *channel, float volumeAdjustment) {
	get().setListenerLocalVolumeAdjustmentImpl(channel->iId, volumeAdjustment);
}


float ChannelListener::getListenerLocalVolumeAdjustment(int channelID) {
	return get().getListenerLocalVolumeAdjustmentImpl(channelID);
}

float ChannelListener::getListenerLocalVolumeAdjustment(const Channel *channel) {
	return get().getListenerLocalVolumeAdjustmentImpl(channel->iId);
}

QHash< int, float > ChannelListener::getAllListenerLocalVolumeAdjustments(bool filter) {
	return get().getAllListenerLocalVolumeAdjustmentsImpl(filter);
}

void ChannelListener::setInitialServerSyncDone(bool done) {
	get().setInitialServerSyncDoneImpl(done);
}

void ChannelListener::saveToDB() {
	if (!g.sh || g.sh->qbaDigest.isEmpty() || g.uiSession == 0) {
		// Can't save as we don't have enough context
		return;
	}

	if (!get().m_initialSyncDone.load()) {
		// If we were to save the listeners before the sync is done, we'd overwrite the list of listeners in the
		// DB with an empty set, effectively erasing all set-up listeners.
		qWarning("ChannelListener: Aborting save of user's listeners as initial ServerSync is not done yet");
		return;
	}

	// Save the currently listened channels
	g.db->setChannelListeners(g.sh->qbaDigest, ChannelListener::getListenedChannelsForUser(g.uiSession));
	// And also the currently set volume adjustments (if they're not set to 1.0)
	g.db->setChannelListenerLocalVolumeAdjustments(g.sh->qbaDigest,
												   ChannelListener::getAllListenerLocalVolumeAdjustments(true));
}
#endif

void ChannelListener::clear() {
	get().clearImpl();
}
