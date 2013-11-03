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

#ifndef MUMBLE_MUMBLE_USERLISTMODEL_H_
#define MUMBLE_MUMBLE_USERLISTMODEL_H_

#include <Qt>
#include <QAbstractTableModel>
#include <QDateTime>
#include <QHash>
#include <QItemSelection>
#include <QList>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QVariant>

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
		UserListModel(const MumbleProto::UserList& userList, QObject *parent = NULL);
	
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
	
		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
		bool setData(const QModelIndex &index, const QVariant &value, int role);
		Qt::ItemFlags flags(const QModelIndex &index) const;
	
		bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	
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
		QVariant lastSeenToTodayDayCount(const std::string& lastSeenDate) const;
		/// Returns a textual representation of the channel hierarchy of the given channel
		QString pathForChannelId(const int channelId) const;
		/// Converts a ISO formatted UTC time string to a QDateTime object.
		QDateTime isoUTCToDateTime(const std::string& isoTime) const;
	
		typedef QList<MumbleProto::UserList_User> ModelUserList;
		/// Model backend for user data
		ModelUserList m_userList;
	
		typedef QHash< ::google::protobuf::uint32, MumbleProto::UserList_User> ModelUserListChangeMap;
		/// Change map indexed by user id
		ModelUserListChangeMap m_changes;
	
		/// True if the message given on construction lacked column data (true for murmur <= 1.2.4)
		bool m_legacyMode;
	
		/// Cache for lastSeenToTodayDayCount
		mutable QHash<QString, QVariant> m_stringToLastSeenToTodayCount;
		/// Cache for pathForChannelId conversions
		mutable QHash<int, QString> m_channelIdToPathMap;
};

#endif // MUMBLE_MUMBLE_USERLISTMODEL_H_
