// Copyright 2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ChannelListener.h"
#include "Channel.h"
#include "User.h"
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>

// init static instance
ChannelListener ChannelListener::s_instance;

ChannelListener::ChannelListener()
	: QObject(nullptr),
	  m_listenerLock(),
	  m_listeningUsers(),
	  m_listenedChannels()
#ifdef MUMBLE
	, m_volumeLock()
	, m_listenerVolumeAdjustments()
#endif
{}

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

const QSet<unsigned int> ChannelListener::getListenersForChannelImpl(int channelID) const {
	QReadLocker lock(&m_listenerLock);

	return m_listenedChannels[channelID];
}

const QSet<int> ChannelListener::getListenedChannelsForUserImpl(unsigned int userSession) const {
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
	QWriteLocker lock(&m_volumeLock);

	m_listenerVolumeAdjustments.insert(channelID, volumeAdjustment);
}

float ChannelListener::getListenerLocalVolumeAdjustmentImpl(int channelID) const {
	QReadLocker lock(&m_volumeLock);

	return m_listenerVolumeAdjustments.value(channelID, 1.0f);
}
#endif


ChannelListener& ChannelListener::get() {
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

const QSet<unsigned int> ChannelListener::getListenersForChannel(int channelID) {
	return get().getListenersForChannelImpl(channelID);
}

const QSet<unsigned int> ChannelListener::getListenersForChannel(const Channel *channel) {
	return get().getListenersForChannelImpl(channel->iId);
}

const QSet<int> ChannelListener::getListenedChannelsForUser(unsigned int userSession) {
	return get().getListenedChannelsForUserImpl(userSession);
}

const QSet<int> ChannelListener::getListenedChannelsForUser(const User *user) {
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
#endif
