// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CLIENTUSER_H_
#define MUMBLE_MUMBLE_CLIENTUSER_H_

#include <QtCore/QHash>
#include <QtCore/QReadWriteLock>

#include "Settings.h"
#include "Timer.h"
#include "User.h"

class ClientUser : public QObject, public User {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ClientUser)

protected:
	float m_localVolume = 1.0f;
	QString m_localNickname;

public:
	Settings::TalkState tsState;
	Timer tLastTalkStateChange;
	bool bLocalIgnore;
	bool bLocalIgnoreTTS;
	bool bLocalMute;

	float fPowerMin, fPowerMax;
	float fAverageAvailable;

	int iFrames;
	int iSequence;

	QByteArray qbaTextureFormat;
	QString qsFriendName;

	QString getFlagsString() const;
	ClientUser(QObject *p = nullptr);

	float getLocalVolumeAdjustments() const;

	QString getLocalNickname() const;

	/**
	 * Determines whether a user is active or not
	 * A user is active when it is currently speaking or when the user has
	 * spoken within Settings::uiActiveTime amount of seconds.
	 */
	bool isActive();

	static QHash< unsigned int, ClientUser * > c_qmUsers;
	static QReadWriteLock c_qrwlUsers;

	static QList< ClientUser * > c_qlTalking;
	static QReadWriteLock c_qrwlTalking;
	static QList< ClientUser * > getTalking();
	static QList< ClientUser * > getActive();

	static void sortUsersOverlay(QList< ClientUser * > &list);

	static ClientUser *get(unsigned int);
	static bool isValid(unsigned int);
	static ClientUser *add(unsigned int, QObject *p = nullptr);
	static ClientUser *match(const ClientUser *p, bool matchname = false);
	static void remove(unsigned int);
	static void remove(ClientUser *);

protected:
	static bool lessThanOverlay(const ClientUser *, const ClientUser *);
public slots:
	void setTalking(Settings::TalkState ts);
	void setMute(bool mute);
	void setDeaf(bool deaf);
	void setSuppress(bool suppress);
	void setLocalIgnore(bool ignore);
	void setLocalIgnoreTTS(bool ignoreTTS);
	void setLocalMute(bool mute);
	void setSelfMute(bool mute);
	void setSelfDeaf(bool deaf);
	void setPrioritySpeaker(bool priority);
	void setRecording(bool recording);
	void setLocalVolumeAdjustment(float adjustment);
	void setLocalNickname(const QString &nickname);
signals:
	void talkingStateChanged();
	void muteDeafStateChanged();
	void prioritySpeakerStateChanged();
	void recordingStateChanged();
	void localVolumeAdjustmentsChanged(float newAdjustment, float oldAdjustment);
	void localNicknameChanged();
};

#endif
