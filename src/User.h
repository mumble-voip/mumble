// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_USER_H_
#define MUMBLE_USER_H_

#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QString>

#include <optional>

class Channel;

class User {
private:
	Q_DISABLE_COPY(User)

public:
	unsigned int uiSession;
	int iId;
	QString qsName;
	QByteArray qbaCommentHash;
	QString qsHash;
	bool bMute, bDeaf, bSuppress;
	bool bSelfMute, bSelfDeaf;
	bool bPrioritySpeaker;
	bool bRecording;
	Channel *cChannel;
	QByteArray qbaTexture;
	QByteArray qbaTextureHash;

	virtual void setComment(const QString &qsComment);
	virtual void clearComment();

	[[nodiscard]] const QString &qsComment() const;

	User();
	virtual ~User(){};

	static bool lessThan(const User *, const User *);

protected:
	QString m_qsComment;
};

// for last seen
struct UserInfo {
	int user_id;
	QString name;
	std::optional< int > last_channel;
	QDateTime last_active;

	UserInfo() : user_id(-1) {}
	UserInfo(int id, QString uname) : user_id(id), name(uname) {}
};

#endif
