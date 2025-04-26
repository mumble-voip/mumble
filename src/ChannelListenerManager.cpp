// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ChannelListenerManager.h"
#include "Channel.h"
#include "User.h"

#include <QReadLocker>
#include <QWriteLocker>

std::size_t qHash(const ChannelListener &listener) {
	return std::hash< ChannelListener >()(listener);
}

bool operator==(const ChannelListener &lhs, const ChannelListener &rhs) {
	return lhs.channelID == rhs.channelID && lhs.userSession == rhs.userSession;
}

ChannelListenerManager::ChannelListenerManager()
	: QObject(nullptr), m_listenerLock(), m_listeningUsers(), m_listenedChannels(), m_volumeLock(),
	  m_listenerVolumeAdjustments() {
}

void ChannelListenerManager::addListener(unsigned int userSession, unsigned int channelID) {
	QWriteLocker lock(&m_listenerLock);

	m_listeningUsers[userSession] << channelID;
	m_listenedChannels[channelID] << userSession;
}

void ChannelListenerManager::removeListener(unsigned int userSession, unsigned int channelID) {
	QWriteLocker lock(&m_listenerLock);

	m_listeningUsers[userSession].remove(channelID);
	m_listenedChannels[channelID].remove(userSession);
}

bool ChannelListenerManager::isListening(unsigned int userSession, unsigned int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID].contains(userSession);
}

bool ChannelListenerManager::isListeningToAny(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return !m_listeningUsers[userSession].isEmpty();
}

bool ChannelListenerManager::isListenedByAny(unsigned int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return !m_listenedChannels[channelID].isEmpty();
}

const QSet< unsigned int > ChannelListenerManager::getListenersForChannel(unsigned int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID];
}

const QSet< unsigned int > ChannelListenerManager::getListenedChannelsForUser(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return m_listeningUsers[userSession];
}

int ChannelListenerManager::getListenerCountForChannel(unsigned int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return static_cast< int >(m_listenedChannels[channelID].size());
}

int ChannelListenerManager::getListenedChannelCountForUser(unsigned int userSession) const {
	QReadLocker lock(&m_listenerLock);

	return static_cast< int >(m_listeningUsers[userSession].size());
}

void ChannelListenerManager::setListenerVolumeAdjustment(unsigned int userSession, unsigned int channelID,
														 const VolumeAdjustment &volumeAdjustment) {
	float oldValue = 1.0f;
	{
		QWriteLocker lock(&m_volumeLock);

		ChannelListener key = {};
		key.channelID       = channelID;
		key.userSession     = userSession;

		auto it = m_listenerVolumeAdjustments.find(key);
		if (it != m_listenerVolumeAdjustments.end()) {
			oldValue = it->second.factor;
		}

		m_listenerVolumeAdjustments[key] = volumeAdjustment;
	}

	if (oldValue != volumeAdjustment.factor) {
		emit localVolumeAdjustmentsChanged(channelID, volumeAdjustment.factor, oldValue);
	}
}

const VolumeAdjustment &ChannelListenerManager::getListenerVolumeAdjustment(unsigned int userSession,
																			unsigned int channelID) const {
	static VolumeAdjustment fallbackObj = VolumeAdjustment::fromFactor(1.0f);

	QReadLocker lock(&m_volumeLock);

	ChannelListener key = {};
	key.channelID       = channelID;
	key.userSession     = userSession;

	auto it = m_listenerVolumeAdjustments.find(key);

	if (it == m_listenerVolumeAdjustments.end()) {
		return fallbackObj;
	} else {
		return it->second;
	}
}

std::unordered_map< unsigned int, VolumeAdjustment >
	ChannelListenerManager::getAllListenerVolumeAdjustments(unsigned int userSession) const {
	QReadLocker lock1(&m_volumeLock);
	QReadLocker lock2(&m_listenerLock);

	std::unordered_map< unsigned int, VolumeAdjustment > adjustments;

	for (unsigned int channelID : m_listeningUsers.value(userSession)) {
		ChannelListener listener = {};
		listener.channelID       = channelID;
		listener.userSession     = userSession;

		auto it = m_listenerVolumeAdjustments.find(listener);

		if (it != m_listenerVolumeAdjustments.end() && it->second.factor != 1.0f) {
			adjustments[channelID] = it->second;
		}
	}

	return adjustments;
}

void ChannelListenerManager::clear() {
	{
		QWriteLocker lock(&m_listenerLock);
		m_listeningUsers.clear();
		m_listenedChannels.clear();
	}
	{
		QWriteLocker lock(&m_volumeLock);
		m_listenerVolumeAdjustments.clear();
	}
}
