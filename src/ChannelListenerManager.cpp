// Copyright 2021-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ChannelListenerManager.h"
#include "Channel.h"
#include "User.h"

#include <QReadLocker>
#include <QWriteLocker>

ChannelListenerManager::ChannelListenerManager()
	: QObject(nullptr), m_listenerLock(), m_listeningUsers(), m_listenedChannels()
#ifdef MUMBLE
	  ,
	  m_volumeLock(), m_listenerVolumeAdjustments()
#endif
{
}

void ChannelListenerManager::addListener(unsigned int userSession, int channelID) {
	QWriteLocker lock(&m_listenerLock);

	m_listeningUsers[userSession] << channelID;
	m_listenedChannels[channelID] << userSession;
}

void ChannelListenerManager::removeListener(unsigned int userSession, int channelID) {
	QWriteLocker lock(&m_listenerLock);

	m_listeningUsers[userSession].remove(channelID);
	m_listenedChannels[channelID].remove(userSession);
}

bool ChannelListenerManager::isListening(unsigned int userSession, int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID].contains(userSession);
}

bool ChannelListenerManager::isListeningToAny(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return !m_listeningUsers[userSession].isEmpty();
}

bool ChannelListenerManager::isListenedByAny(int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return !m_listenedChannels[channelID].isEmpty();
}

const QSet< unsigned int > ChannelListenerManager::getListenersForChannel(int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID];
}

const QSet< int > ChannelListenerManager::getListenedChannelsForUser(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return m_listeningUsers[userSession];
}

int ChannelListenerManager::getListenerCountForChannel(int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID].size();
}

int ChannelListenerManager::getListenedChannelCountForUser(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return m_listeningUsers[userSession].size();
}

#ifdef MUMBLE
void ChannelListenerManager::setListenerLocalVolumeAdjustment(int channelID, float volumeAdjustment) {
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

float ChannelListenerManager::getListenerLocalVolumeAdjustment(int channelID) const {
	QReadLocker lock(&m_volumeLock);

	return m_listenerVolumeAdjustments.value(channelID, 1.0f);
}

QHash< int, float > ChannelListenerManager::getAllListenerLocalVolumeAdjustments(bool filter) const {
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
#endif

void ChannelListenerManager::clear() {
	{
		QWriteLocker lock(&m_listenerLock);
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
