/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_USEREDIT_H_
#define MUMBLE_MUMBLE_USEREDIT_H_

#include "Message.h"
#include "User.h"
#include "ui_UserEdit.h"

#include <QSortFilterProxyModel>

class UserListModel;
class UserListFilterProxyModel;

namespace MumbleProto {
	class UserList;
	class UserList_User;
}

///
/// Dialog used for server-side registered user list editing.
///
class UserEdit : public QDialog, public Ui::UserEdit {
		Q_OBJECT
		Q_DISABLE_COPY(UserEdit)
	public:
		/// Constructs a dialog for editing the given userList.
		UserEdit(const MumbleProto::UserList &userList, QWidget *parent = NULL);
	
	public slots:
		void accept() Q_DECL_OVERRIDE;
	
		void on_qlSearch_textChanged(QString);
		void on_qpbRemove_clicked();
		void on_qpbRename_clicked();
		void on_qtvUserList_customContextMenuRequested(const QPoint&);
		void onSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/);
		void onCurrentRowChanged(const QModelIndex & current, const QModelIndex &/*previous*/);
		void on_qsbInactive_valueChanged(int);
		void on_qcbInactive_currentIndexChanged(int);
	
	private:
		enum TimespanUnits { TU_DAYS, TU_WEEKS, TU_MONTHS, TU_YEARS, COUNT_TU };
	
		/// Polls the inactive-filter controls for their current value and updates the model filter.
		void updateInactiveDaysFilter();
	
		UserListModel *m_model;
		UserListFilterProxyModel *m_filter;
};

///
/// Provides filtering and sorting capabilities for UserListModel instances to UserEdit.
/// @see UserEdit
/// @see UserListModel
///
class UserListFilterProxyModel : public QSortFilterProxyModel {
	Q_OBJECT
public:
	explicit UserListFilterProxyModel(QObject *parent = NULL);

	bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

public slots:
	/// Sets the amount of inactive days below which rows will get filterd by the proxy
	void setFilterMinimumInactiveDays(int minimumInactiveDays);
	/// Helper function for removing all rows involved in a given selection (must include COL_NICK).
	void removeRowsInSelection(const QItemSelection& selection);

private:
	/// Every row with less inactive days will be filtered.
	int m_minimumInactiveDays;
};


#endif // MUMBLE_MUMBLE_USEREDIT_H_
