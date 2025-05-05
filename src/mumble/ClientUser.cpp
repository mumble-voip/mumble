// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ClientUser.h"

#include "AudioOutput.h"
#include "Channel.h"
#include "PluginManager.h"
#include "Global.h"

QHash< unsigned int, ClientUser * > ClientUser::c_qmUsers;
QReadWriteLock ClientUser::c_qrwlUsers;

QList< ClientUser * > ClientUser::c_qlTalking;
QReadWriteLock ClientUser::c_qrwlTalking;

ClientUser::ClientUser(QObject *p)
	: QObject(p), tsState(Settings::Passive), tLastTalkStateChange(false), bLocalIgnore(false), bLocalIgnoreTTS(false),
	  bLocalMute(false), fPowerMin(0.0f), fPowerMax(0.0f), fAverageAvailable(0.0f), iFrames(0), iSequence(0) {
}

float ClientUser::getLocalVolumeAdjustments() const {
	return m_localVolume;
}

QString ClientUser::getLocalNickname() const {
	return m_localNickname;
}

ClientUser *ClientUser::get(unsigned int uiSession) {
	QReadLocker lock(&c_qrwlUsers);
	ClientUser *p = c_qmUsers.value(uiSession);
	return p;
}

QList< ClientUser * > ClientUser::getTalking() {
	QReadLocker lock(&c_qrwlTalking);
	return c_qlTalking;
}

QList< ClientUser * > ClientUser::getActive() {
	QReadLocker lock(&c_qrwlUsers);
	QList< ClientUser * > activeUsers;
	foreach (ClientUser *cu, c_qmUsers) {
		if (cu->isActive())
			activeUsers << cu;
	}
	return activeUsers;
}

bool ClientUser::isValid(unsigned int uiSession) {
	QReadLocker lock(&c_qrwlUsers);

	return c_qmUsers.contains(uiSession);
}

ClientUser *ClientUser::add(unsigned int uiSession, QObject *po) {
	QWriteLocker lock(&c_qrwlUsers);

	ClientUser *p        = new ClientUser(po);
	p->uiSession         = uiSession;
	c_qmUsers[uiSession] = p;

	QObject::connect(p, &ClientUser::talkingStateChanged, Global::get().pluginManager,
					 &PluginManager::on_userTalkingStateChanged);

	return p;
}

ClientUser *ClientUser::match(const ClientUser *other, bool matchname) {
	QReadLocker lock(&c_qrwlUsers);

	ClientUser *p;
	foreach (p, c_qmUsers) {
		if (p == other)
			continue;
		if ((p->iId >= 0) && (p->iId == other->iId))
			return p;
		if (matchname && (p->qsName == other->qsName))
			return p;
	}
	return nullptr;
}

void ClientUser::remove(unsigned int uiSession) {
	ClientUser *p;
	{
		QWriteLocker lock(&c_qrwlUsers);
		p = c_qmUsers.take(uiSession);

		if (p) {
			if (p->cChannel)
				p->cChannel->removeUser(p);

			if (p->tsState != Settings::Passive) {
				QWriteLocker writeLock(&c_qrwlTalking);
				c_qlTalking.removeAll(p);
			}
		}
	}

	if (p) {
		AudioOutputPtr ao = Global::get().ao;
		if (ao) {
			// It is safe to call this function and to give the ClientUser pointer
			// to it even though we don't hold the lock anymore as it will only take
			// the pointer to use as the key in a HashMap lookup. At no point in the
			// code triggered by this function call will the ClientUser pointer be
			// dereferenced.
			// Furthermore ClientUser objects are deleted in UserModel::removeUser which
			// calls this very function before doing so. Thus the object shouldn't be
			// deleted before this function returns anyways.
			ao->removeUser(p);
		}
	}
}

void ClientUser::remove(ClientUser *p) {
	remove(p->uiSession);
}

QString ClientUser::getFlagsString() const {
	QStringList flags;

	if (!qsFriendName.isEmpty())
		flags << ClientUser::tr("Friend");
	if (iId >= 0)
		flags << ClientUser::tr("Authenticated");
	if (bPrioritySpeaker)
		flags << ClientUser::tr("Priority speaker");
	if (bRecording)
		flags << ClientUser::tr("Recording");
	if (bMute)
		flags << ClientUser::tr("Muted (server)");
	if (bDeaf)
		flags << ClientUser::tr("Deafened (server)");
	if (bLocalIgnore)
		flags << ClientUser::tr("Local Ignore (Text messages)");
	if (bLocalIgnoreTTS)
		flags << ClientUser::tr("Local Ignore (Text-To-Speech)");
	if (bLocalMute)
		flags << ClientUser::tr("Local Mute");
	if (bSelfMute)
		flags << ClientUser::tr("Muted (self)");
	if (bSelfDeaf)
		flags << ClientUser::tr("Deafened (self)");

	return flags.join(QLatin1String(", "));
}

void ClientUser::setTalking(Settings::TalkState ts) {
	if (tsState == ts)
		return;

	bool nstate = false;
	if (ts == Settings::Passive)
		nstate = true;
	else if (tsState == Settings::Passive)
		nstate = true;

	tsState = ts;
	tLastTalkStateChange.restart();
	emit talkingStateChanged();

	if (nstate && cChannel) {
		QWriteLocker lock(&c_qrwlTalking);
		if (ts == Settings::Passive)
			c_qlTalking.removeAll(this);
		else
			c_qlTalking << this;
	}
}

void ClientUser::setMute(bool mute) {
	if (bMute == mute)
		return;
	bMute = mute;
	if (!bMute)
		bDeaf = false;
	emit muteDeafStateChanged();
}

void ClientUser::setSuppress(bool suppress) {
	if (bSuppress == suppress)
		return;
	bSuppress = suppress;
	emit muteDeafStateChanged();
}

void ClientUser::setLocalIgnore(bool ignore) {
	if (bLocalIgnore == ignore)
		return;
	bLocalIgnore = ignore;
	emit muteDeafStateChanged();
}

void ClientUser::setLocalIgnoreTTS(bool ignoreTTS) {
	bLocalIgnoreTTS = ignoreTTS;
}

void ClientUser::setLocalMute(bool mute) {
	if (bLocalMute == mute)
		return;
	bLocalMute = mute;
	emit muteDeafStateChanged();
}

void ClientUser::setDeaf(bool deaf) {
	bDeaf = deaf;
	if (bDeaf)
		bMute = true;
	emit muteDeafStateChanged();
}

void ClientUser::setSelfMute(bool mute) {
	bSelfMute = mute;
	if (!mute)
		bSelfDeaf = false;
	emit muteDeafStateChanged();
}

void ClientUser::setSelfDeaf(bool deaf) {
	bSelfDeaf = deaf;
	if (deaf)
		bSelfMute = true;
	emit muteDeafStateChanged();
}

void ClientUser::setPrioritySpeaker(bool priority) {
	if (bPrioritySpeaker == priority)
		return;
	bPrioritySpeaker = priority;
	emit prioritySpeakerStateChanged();
}

void ClientUser::setRecording(bool recording) {
	if (bRecording == recording)
		return;
	bRecording = recording;
	emit recordingStateChanged();
}

void ClientUser::setLocalVolumeAdjustment(float adjustment) {
	float oldAdjustment = m_localVolume;
	m_localVolume       = adjustment;

	emit localVolumeAdjustmentsChanged(m_localVolume, oldAdjustment);
}

void ClientUser::setLocalNickname(const QString &nickname) {
	if (m_localNickname != nickname) {
		m_localNickname = nickname;

		emit localNicknameChanged();
	}
}

bool ClientUser::lessThanOverlay(const ClientUser *first, const ClientUser *second) {
	if (Global::get().s.os.osSort == OverlaySettings::LastStateChange) {
		// Talkers above non-talkers
		if (first->tsState != Settings::Passive && second->tsState == Settings::Passive)
			return true;
		if (first->tsState == Settings::Passive && second->tsState != Settings::Passive)
			return false;

		// Valid time above invalid time (possible when there wasn't a state-change yet)
		if (first->tLastTalkStateChange.isStarted() && !second->tLastTalkStateChange.isStarted())
			return true;
		if (!first->tLastTalkStateChange.isStarted() && second->tLastTalkStateChange.isStarted())
			return false;

		// If both have a valid time
		if (first->tLastTalkStateChange.isStarted() && second->tLastTalkStateChange.isStarted()) {
			// Among talkers, long > short
			// (if two clients are talking, the client that started first is above the other)
			if (first->tsState != Settings::Passive && second->tsState != Settings::Passive)
				return first->tLastTalkStateChange > second->tLastTalkStateChange;

			// Among non-talkers, short -> long
			// (if two clients are passive, the client that most recently stopped talking is above)
			if (first->tsState == Settings::Passive && second->tsState == Settings::Passive)
				return first->tLastTalkStateChange < second->tLastTalkStateChange;
		}

		// If both times are invalid, fall back to alphabetically (continuing below)
	}

	if (first->cChannel == second->cChannel || !first->cChannel || !second->cChannel)
		return lessThan(first, second);

	// When sorting for the overlay always place the local users
	// channel above the others
	ClientUser *self = c_qmUsers.value(Global::get().uiSession);
	if (self) {
		if (self->cChannel == first->cChannel)
			return true;
		else if (self->cChannel == second->cChannel)
			return false;
	}

	return Channel::lessThan(first->cChannel, second->cChannel);
}

void ClientUser::sortUsersOverlay(QList< ClientUser * > &list) {
	QReadLocker lock(&c_qrwlUsers);

	std::sort(list.begin(), list.end(), ClientUser::lessThanOverlay);
}

bool ClientUser::isActive() {
	if (tsState != Settings::Passive)
		return true;

	if (!tLastTalkStateChange.isStarted())
		return false;

	return tLastTalkStateChange.elapsed() < static_cast< unsigned int >(Global::get().s.os.uiActiveTime) * 1000000U;
}

/* From Channel.h
 */
void Channel::addClientUser(ClientUser *p) {
	addUser(p);
	p->setParent(this);
}
