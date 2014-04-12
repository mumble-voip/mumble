/* Copyright (C) 2013, Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
	 this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
	 this list of conditions and the following disclaimer in the documentation
	 and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
	 contributors may be used to endorse or promote products derived from this
	 software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QStringList>
#include <QTextDocument>

#include "UserListModel.h"
#include "Channel.h"
#include "Message.h"

#include <vector>
#include <algorithm>

UserListModel::UserListModel(const MumbleProto::UserList& userList, QObject *parent)
	: QAbstractTableModel(parent)
	, m_legacyMode(false) {

	m_userList.reserve(userList.users_size());
	for (int i = 0; i < userList.users_size(); ++i) {
		m_userList.append(userList.users(i));
	}

	if (!m_userList.empty()) {
		const MumbleProto::UserList_User& user = m_userList.back();
		m_legacyMode = !user.has_last_seen() || !user.has_last_channel();
	}
}

int UserListModel::rowCount(const QModelIndex &parent) const {
	if (parent.isValid())
		return 0;

	return m_userList.size();
}

int UserListModel::columnCount(const QModelIndex &parent) const {
	if (parent.isValid())
		return 0;

	if (m_legacyMode) {
		// Only COL_NICK
		return 1;
	}

	return COUNT_COL;
}

QVariant UserListModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation != Qt::Horizontal)
		return QVariant();
	
	if (section < 0 || section >= columnCount())
		return QVariant();

	if (role == Qt::DisplayRole) {
		switch (section) {
			case COL_NICK:         return tr("Nick");
			case COL_INACTIVEDAYS: return tr("Inactive days");
			case COL_LASTCHANNEL:  return tr("Last channel");
			default:               return QVariant();
		}
	}

	return QVariant();
}

QVariant UserListModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();
	
	if (index.row() < 0 && index.row() >= m_userList.size())
		return QVariant();
	
	if (index.column() >= columnCount())
		return QVariant();

	const MumbleProto::UserList_User& user = m_userList[index.row()];

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
			case COL_NICK:         return u8(user.name());
			case COL_INACTIVEDAYS: return lastSeenToTodayDayCount(user.last_seen());
			case COL_LASTCHANNEL:  return pathForChannelId(user.last_channel());
			default:               return QVariant();
		}
	} else if (role == Qt::ToolTipRole) {
		switch (index.column()) {
			case COL_INACTIVEDAYS: return tr("Last seen: %1").arg(user.last_seen().empty() ?
				                                                      tr("Never")
				                                                    : Qt::escape(u8(user.last_seen())));
			case COL_LASTCHANNEL:  return tr("Channel id: %1").arg(user.last_channel());
			default:               return QVariant();
		}
	} else if (role == Qt::UserRole) {
		switch (index.column()) {
			case COL_INACTIVEDAYS: return isoUTCToDateTime(user.last_seen());
			case COL_LASTCHANNEL:  return user.last_channel();
			default:               return QVariant();
		}
	} else if (role == Qt::EditRole) {
		if (index.column() == COL_NICK) {
			return u8(user.name());
		}
	}

	return QVariant();
}

bool UserListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
	if (!index.isValid())
		return false;
	
	if (index.column() != COL_NICK || role != Qt::EditRole)
		return false;
	
	if (index.row() < 0 || index.row() >= m_userList.size())
		return false;

	const std::string newNick = u8(value.toString());
	if (newNick.empty()) {
		// Empty nick is not valid
		return false;
	}

	MumbleProto::UserList_User& user = m_userList[index.row()];
	if (newNick != user.name()) {
		foreach (const MumbleProto::UserList_User& otherUser, m_userList) {
			if (otherUser.name() == newNick) {
				// Duplicate is not valid
				return false;
			}
		}

		user.set_name(newNick);
		m_changes[user.user_id()] = user;

		emit dataChanged(index, index);
	}

	return true;
}

Qt::ItemFlags UserListModel::flags(const QModelIndex &index) const {
	const Qt::ItemFlags original = QAbstractTableModel::flags(index);

	if (index.column() == COL_NICK) {
		return original | Qt::ItemIsEditable;
	}

	return original;
}

bool UserListModel::removeRows(int row, int count, const QModelIndex &parent) {
	if (row + count > m_userList.size())
		return false;

	beginRemoveRows(parent, row, row + count - 1);

	ModelUserList::Iterator startIt = m_userList.begin() + row;
	ModelUserList::Iterator endIt = startIt + count;

	for (ModelUserList::Iterator it = startIt;
		 it != endIt;
		 ++it) {
		it->clear_name();
		m_changes[it->user_id()] = *it;
	}

	m_userList.erase(startIt, endIt);

	endRemoveRows();
	return true;
}

void UserListModel::removeRowsInSelection(const QItemSelection &selection) {
	QModelIndexList indices = selection.indexes();

	std::vector<int> rows;
	rows.reserve(indices.size());

	foreach (const QModelIndex& idx, indices) {
		if (idx.column() != COL_NICK)
			continue;

		rows.push_back(idx.row());
	}

	// Make sure to presort the rows so we work from back (high) to front (low).
	// This prevents the row numbers from becoming invalid after removing a group.
	// The basic idea is to take a number of sorted rows (e.g. 10,9,5,3,2,1) and
	// delete them with the minimum number of removeRows calls. This means grouping
	// adjacent rows (e.g. (10,9),(5),(3,2,1)) and using a removeRows call for each group.
	std::sort(rows.begin(), rows.end(), std::greater<int>());

	int nextRow = -2;
	int groupRowCount = 0;

	for (size_t i = 0; i < rows.size(); ++i) {
		if (rows[i] == nextRow) {
			++groupRowCount;
			--nextRow;
		} else {
			if (groupRowCount > 0)  {
				// Remove previous group
				const int lastRowInGroup = nextRow + 1;
				removeRows(lastRowInGroup, groupRowCount);
			}

			// Start next group
			nextRow = rows[i] - 1;
			groupRowCount = 1;
		}
	}

	if (groupRowCount > 0) {
		// Remove leftover group
		const int lastRowInGroup = nextRow + 1;
		removeRows(lastRowInGroup, groupRowCount);
	}
}

MumbleProto::UserList UserListModel::getUserListUpdate() const {
	MumbleProto::UserList updateList;

	for (ModelUserListChangeMap::ConstIterator it = m_changes.constBegin();
	     it != m_changes.constEnd();
	     ++it) {
		MumbleProto::UserList_User* user = updateList.add_users();
		*user = it.value();
	}

	return updateList;
}

bool UserListModel::isUserListDirty() const {
	return !m_changes.empty();
}

bool UserListModel::isLegacy() const {
	return m_legacyMode;
}

QVariant UserListModel::lastSeenToTodayDayCount(const std::string &lastSeenDate) const {
	if (lastSeenDate.empty())
		return QVariant();

	QVariant count = m_stringToLastSeenToTodayCount.value(u8(lastSeenDate));
	if (count.isNull()) {
		QDateTime dt = isoUTCToDateTime(lastSeenDate);
		if (!dt.isValid()) {
			// Not convertable to int
			return QVariant();
		}
		count = dt.daysTo(QDateTime::currentDateTime().toUTC());
		m_stringToLastSeenToTodayCount.insert(u8(lastSeenDate), count);
	}
	return count;
}

QString UserListModel::pathForChannelId(const int channelId) const {
	QString path = m_channelIdToPathMap.value(channelId);
	if (path.isNull()) {
		path = QLatin1String("-");

		Channel *channel = Channel::get(channelId);
		if (channel != NULL) {
			QStringList pathParts;

			while (channel->cParent != NULL) {
				pathParts.prepend(channel->qsName);
				channel = channel->cParent;
			}

			pathParts.append(QString());

			path = QLatin1String("/ ") + pathParts.join(QLatin1String(" / "));
		}

		m_channelIdToPathMap.insert(channelId, path);
	}
	return path;
}

QDateTime UserListModel::isoUTCToDateTime(const std::string &isoTime) const {
	QDateTime dt = QDateTime::fromString(u8(isoTime), Qt::ISODate);
	dt.setTimeSpec(Qt::UTC);
	return dt;
}
