// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_USER_H_
#define MUMBLE_USER_H_

#ifndef Q_MOC_RUN
# include <boost/optional.hpp>
#endif

#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QString>
#include <QtCore/QList>

class Channel;

class User {
	private:
		Q_DISABLE_COPY(User)
	protected:
		/// A list of channel IDs this user is listening to without being in them.
		QList<int> m_listeningChannelIDs;

	public:
		unsigned int uiSession;
		int iId;
		QString qsName;
		QString qsComment;
		QByteArray qbaCommentHash;
		QString qsHash;
		bool bMute, bDeaf, bSuppress;
		bool bSelfMute, bSelfDeaf;
		bool bPrioritySpeaker;
		bool bRecording;
		Channel *cChannel;
		QByteArray qbaTexture;
		QByteArray qbaTextureHash;

		User();
		virtual ~User() {};

		void addListeningChannel(const Channel *chan);
		void addListeningChannel(int channelID);
		void removeListeningChannel(const Channel *chan);
		void removeListeningChannel(int channelID);
		bool isListeningTo(const Channel *c) const;
		bool isListeningTo(int channelID) const;
		const QList<int>& listeningChannelIDs() const;

		static bool lessThan(const User *, const User *);
};

// for last seen
struct UserInfo {
	int user_id;
	QString name;
	boost::optional<int> last_channel;
	QDateTime last_active;

	UserInfo() : user_id(-1) {}
	UserInfo(int id, QString uname) : user_id(id), name(uname) {}
};

#endif
