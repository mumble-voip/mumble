// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_USERLISTMODEL_H_
#define MUMBLE_MUMBLE_USERLISTMODEL_H_

#include <QAbstractTableModel>
#include <QDateTime>
#include <QHash>
#include <QItemSelection>
#include <QList>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QVariant>
#include <Qt>

#include "Mumble.pb.h"

///
/// The UserListModel class provides a table model backed by a UserList protobuf structure.
/// It supports removing rows and editing user nicks.
///
class UserListModel : public QAbstractTableModel {
	Q_OBJECT
public:
	/// Enumerates the columns in the table model
	enum Columns { COL_NICK, COL_INACTIVEDAYS, COL_LASTCHANNEL, COUNT_COL };

	/// UserListModel constructs a table model representing the userList.
	/// @param userList User list protobuf structure (will be copied)
	/// @param parent Parent in QObject hierarchy
	UserListModel(const MumbleProto::UserList &userList, QObject *parent = nullptr);

	int rowCount(const QModelIndex &parentIndex = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parentIndex = QModelIndex()) const Q_DECL_OVERRIDE;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
	QVariant data(const QModelIndex &dataIndex, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	bool setData(const QModelIndex &dataIndex, const QVariant &value, int role) Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &flagIndex) const Q_DECL_OVERRIDE;

	bool removeRows(int row, int count, const QModelIndex &parentIndex = QModelIndex()) Q_DECL_OVERRIDE;

	/// Function for removing all rows in a selection
	void removeRowsInSelection(const QItemSelection &selection);

	/// Returns a message for updating the original server state to the current model state.
	MumbleProto::UserList getUserListUpdate() const;

	/// Returns true if the UserList given during construction was changed.
	bool isUserListDirty() const;

	/// Returns true if the model only contains COL_NICK (true for Mumble <= 1.2.4)
	bool isLegacy() const;

private:
	/// Given an ISO formatted UTC time string returns the number of days since then.
	/// @return QVariant() is returned for invalid strings.
	QVariant lastSeenToTodayDayCount(const std::string &lastSeenDate) const;
	/// Returns a textual representation of the channel hierarchy of the given channel
	QString pathForChannelId(unsigned int channelId) const;
	/// Converts a ISO formatted UTC time string to a QDateTime object.
	QDateTime isoUTCToDateTime(const std::string &isoTime) const;

	typedef QList< MumbleProto::UserList_User > ModelUserList;
	/// Model backend for user data
	ModelUserList m_userList;

	typedef QHash<::google::protobuf::uint32, MumbleProto::UserList_User > ModelUserListChangeMap;
	/// Change map indexed by user id
	ModelUserListChangeMap m_changes;

	/// True if the message given on construction lacked column data (true for murmur <= 1.2.4)
	bool m_legacyMode;

	/// Cache for lastSeenToTodayDayCount
	mutable QHash< QString, QVariant > m_stringToLastSeenToTodayCount;
	/// Cache for pathForChannelId conversions
	mutable QHash< unsigned int, QString > m_channelIdToPathMap;
};

#endif // MUMBLE_MUMBLE_USERLISTMODEL_H_
