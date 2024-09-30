// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CHANNEL_H_
#define MUMBLE_CHANNEL_H_

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>
#include <QtCore/QSet>
#include <QtCore/QString>

#ifdef MUMBLE
#	include <atomic>
#	include "ChannelFilterMode.h"
#endif

class User;
class Group;
class ChanACL;

class ClientUser;

class Channel : public QObject {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Channel)
private:
	QSet< Channel * > qsUnseen;

public:
	static constexpr int ROOT_ID = 0;

	unsigned int iId;
	int iPosition;
	bool bTemporary;
	Channel *cParent;
	QString qsName;
	QString qsDesc;
	QByteArray qbaDescHash;
	QList< Channel * > qlChannels;
	QList< User * > qlUsers;
	QHash< QString, Group * > qhGroups;
	QList< ChanACL * > qlACL;

#ifdef MUMBLE
	/// A flag indicating whether this channel has enter restrictions (ACL denying ENTER) in place
	std::atomic< bool > hasEnterRestrictions;
	/// A flag indicating whether the local user is currently able to enter this channel. In theory this should
	/// represent the correct access state (apart from the time it takes to synchronize ACL changes from the server
	/// to the client), but in the end, it's the server who decides whether the user can enter. This flag is only
	/// meant for UI purposes and should not be used influence actual behaviour.
	std::atomic< bool > localUserCanEnter;
#endif

	QSet< Channel * > qsPermLinks;
	QHash< Channel *, int > qhLinks;

	bool bInheritACL;

	/// Maximum number of users allowed in the channel. If this
	/// value is zero, the maximum number of users allowed in the
	/// channel is given by the server's "usersperchannel"
	/// setting.
	unsigned int uiMaxUsers;

	Channel(unsigned int id, const QString &name, QObject *p = nullptr);
	~Channel();

#ifdef MUMBLE
	unsigned int uiPermissions;

	ChannelFilterMode m_filterMode;

	void setFilterMode(ChannelFilterMode filterMode);
	void clearFilterMode();
	bool isFiltered() const;

	static QHash< unsigned int, Channel * > c_qhChannels;
	static QReadWriteLock c_qrwlChannels;

	static Channel *get(unsigned int);
	static Channel *add(unsigned int, const QString &);
	static void remove(Channel *);

	void addClientUser(ClientUser *p);
#endif
	static bool lessThan(const Channel *, const Channel *);

	size_t getLevel() const;
	size_t getDepth() const;
	QString getPath() const;

	void addChannel(Channel *c);
	void removeChannel(Channel *c);
	void addUser(User *p);
	void removeUser(User *p);

	bool isLinked(Channel *c) const;
	void link(Channel *c);
	void unlink(Channel *c = nullptr);

	QSet< Channel * > allLinks();
	QSet< Channel * > allChildren();

	operator QString() const;

signals:
	/// Signal emitted whenever a user enters a channel.
	///
	/// @param newChannel A pointer to the Channel the user has just entered
	/// @param prevChannel A pointer to the Channel the user is coming from or nullptr if
	/// 	there is no such channel.
	/// @param user A pointer to the User that has triggered this signal
	void channelEntered(const Channel *newChannel, const Channel *prevChannel, const User *user);
	/// Signal emitted whenever a user leaves a channel.
	///
	/// @param channel A pointer to the Channel the user has left
	/// @param user A pointer to the User that has triggered this signal
	void channelExited(const Channel *channel, const User *user);
};

#endif
