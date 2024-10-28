// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "UserListModel.h"

#include "Channel.h"
#include "QtUtils.h"
#include "Utils.h"

#include <algorithm>

#ifdef _MSC_VER
#	include <functional>
#endif

#include <vector>

#include <QtCore/QTimeZone>

UserListModel::UserListModel(const MumbleProto::UserList &userList, QObject *parent_)
	: QAbstractTableModel(parent_), m_legacyMode(false) {
	m_userList.reserve(userList.users_size());
	for (int i = 0; i < userList.users_size(); ++i) {
		m_userList.append(userList.users(i));
	}

	if (!m_userList.empty()) {
		const MumbleProto::UserList_User &user = m_userList.back();
		m_legacyMode                           = !user.has_last_seen() || !user.has_last_channel();
	}
}

int UserListModel::rowCount(const QModelIndex &parentIndex) const {
	if (parentIndex.isValid())
		return 0;

	return static_cast< int >(m_userList.size());
}

int UserListModel::columnCount(const QModelIndex &parentIndex) const {
	if (parentIndex.isValid())
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
			case COL_NICK:
				return tr("Nick");
			case COL_INACTIVEDAYS:
				return tr("Inactive days");
			case COL_LASTCHANNEL:
				return tr("Last channel");
			default:
				return QVariant();
		}
	}

	return QVariant();
}

QVariant UserListModel::data(const QModelIndex &dataIndex, int role) const {
	if (!dataIndex.isValid())
		return QVariant();

	if (dataIndex.row() < 0 || dataIndex.row() >= m_userList.size())
		return QVariant();

	if (dataIndex.column() >= columnCount())
		return QVariant();

	const MumbleProto::UserList_User &user = m_userList[dataIndex.row()];

	if (role == Qt::DisplayRole) {
		switch (dataIndex.column()) {
			case COL_NICK:
				return u8(user.name());
			case COL_INACTIVEDAYS:
				return lastSeenToTodayDayCount(user.last_seen());
			case COL_LASTCHANNEL:
				return pathForChannelId(user.last_channel());
			default:
				return QVariant();
		}
	} else if (role == Qt::ToolTipRole) {
		switch (dataIndex.column()) {
			case COL_INACTIVEDAYS:
				return tr("Last seen: %1")
					.arg(user.last_seen().empty() ? tr("Never") : u8(user.last_seen()).toHtmlEscaped());
			case COL_LASTCHANNEL:
				return tr("Channel ID: %1").arg(user.last_channel());
			default:
				return QVariant();
		}
	} else if (role == Qt::UserRole) {
		switch (dataIndex.column()) {
			case COL_INACTIVEDAYS:
				return isoUTCToDateTime(user.last_seen());
			case COL_LASTCHANNEL:
				return user.last_channel();
			default:
				return QVariant();
		}
	} else if (role == Qt::EditRole) {
		if (dataIndex.column() == COL_NICK) {
			return u8(user.name());
		}
	}

	return QVariant();
}

bool UserListModel::setData(const QModelIndex &dataIndex, const QVariant &value, int role) {
	if (!dataIndex.isValid())
		return false;

	if (dataIndex.column() != COL_NICK || role != Qt::EditRole)
		return false;

	if (dataIndex.row() < 0 || dataIndex.row() >= m_userList.size())
		return false;

	const std::string newNick = u8(value.toString());
	if (newNick.empty()) {
		// Empty nick is not valid
		return false;
	}

	MumbleProto::UserList_User &user = m_userList[dataIndex.row()];
	if (newNick != user.name()) {
		foreach (const MumbleProto::UserList_User &otherUser, m_userList) {
			if (otherUser.name() == newNick) {
				// Duplicate is not valid
				return false;
			}
		}

		user.set_name(newNick);
		m_changes[user.user_id()] = user;

		emit dataChanged(dataIndex, dataIndex);
	}

	return true;
}

Qt::ItemFlags UserListModel::flags(const QModelIndex &flagIndex) const {
	const Qt::ItemFlags original = QAbstractTableModel::flags(flagIndex);

	if (flagIndex.column() == COL_NICK) {
		return original | Qt::ItemIsEditable;
	}

	return original;
}

bool UserListModel::removeRows(int row, int count, const QModelIndex &parentIndex) {
	if (row + count > m_userList.size())
		return false;

	beginRemoveRows(parentIndex, row, row + count - 1);

	ModelUserList::Iterator startIt = m_userList.begin() + row;
	ModelUserList::Iterator endIt   = startIt + count;

	for (ModelUserList::Iterator it = startIt; it != endIt; ++it) {
		it->clear_name();
		m_changes[it->user_id()] = *it;
	}

	m_userList.erase(startIt, endIt);

	endRemoveRows();
	return true;
}

void UserListModel::removeRowsInSelection(const QItemSelection &selection) {
	QModelIndexList indices = selection.indexes();

	std::vector< int > rows;
	rows.reserve(static_cast< std::size_t >(indices.size()));

	foreach (const QModelIndex &idx, indices) {
		if (idx.column() != COL_NICK)
			continue;

		rows.push_back(idx.row());
	}

	// Make sure to presort the rows so we work from back (high) to front (low).
	// This prevents the row numbers from becoming invalid after removing a group.
	// The basic idea is to take a number of sorted rows (e.g. 10,9,5,3,2,1) and
	// delete them with the minimum number of removeRows calls. This means grouping
	// adjacent rows (e.g. (10,9),(5),(3,2,1)) and using a removeRows call for each group.
	std::sort(rows.begin(), rows.end(), std::greater< int >());

	int nextRow       = -2;
	int groupRowCount = 0;

	for (size_t i = 0; i < rows.size(); ++i) {
		if (rows[i] == nextRow) {
			++groupRowCount;
			--nextRow;
		} else {
			if (groupRowCount > 0) {
				// Remove previous group
				const int lastRowInGroup = nextRow + 1;
				removeRows(lastRowInGroup, groupRowCount);
			}

			// Start next group
			nextRow       = rows[i] - 1;
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

	for (ModelUserListChangeMap::ConstIterator it = m_changes.constBegin(); it != m_changes.constEnd(); ++it) {
		MumbleProto::UserList_User *user = updateList.add_users();
		*user                            = it.value();
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
			// Not convertible to int
			return QVariant();
		}
		count = dt.daysTo(QDateTime::currentDateTime().toUTC());
		m_stringToLastSeenToTodayCount.insert(u8(lastSeenDate), count);
	}
	return count;
}

QString UserListModel::pathForChannelId(const unsigned int channelId) const {
	QString path = m_channelIdToPathMap.value(channelId);
	if (path.isNull()) {
		path = QLatin1String("-");

		Channel *channel = Channel::get(channelId);
		if (channel) {
			QStringList pathParts;

			while (channel->cParent) {
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	dt.setTimeZone(QTimeZone::UTC);
#else
	dt.setTimeSpec(Qt::UTC);
#endif
	return dt;
}
