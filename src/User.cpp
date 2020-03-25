// Copyright 2005-2020 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "User.h"
#include "Channel.h"

User::User() {
	uiSession = 0;
	iId = -1;
	bMute = bDeaf = false;
	bSelfMute = bSelfDeaf = false;
	bPrioritySpeaker = false;
	bRecording = false;
	bSuppress = false;
	cChannel = 0;
}

void User::addListeningChannel(const Channel *chan) {
	addListeningChannel(chan->iId);
}

void User::addListeningChannel(int channelID) {
	m_listeningChannelIDs << channelID;
}

void User::removeListeningChannel(const Channel *chan) {
	removeListeningChannel(chan->iId);
}

void User::removeListeningChannel(int channelID) {
	m_listeningChannelIDs.removeAll(channelID);
}

bool User::isListeningTo(const Channel *chan) const {
	return isListeningTo(chan->iId);
}

bool User::isListeningTo(int channelID) const {
	return m_listeningChannelIDs.contains(channelID);
}

bool User::lessThan(const User *first, const User *second) {
	return (QString::localeAwareCompare(first->qsName, second->qsName) < 0);
}

const QList<int>& User::listeningChannelIDs() const {
	return m_listeningChannelIDs;
}
