// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_USEREDIT_H_
#define MUMBLE_MUMBLE_USEREDIT_H_

#include "User.h"
#include "ui_UserEdit.h"

#include <QSortFilterProxyModel>

class UserListModel;
class UserListFilterProxyModel;

namespace MumbleProto {
class UserList;
class UserList_User;
} // namespace MumbleProto

///
/// Dialog used for server-side registered user list editing.
///
class UserEdit : public QDialog, public Ui::UserEdit {
	Q_OBJECT
	Q_DISABLE_COPY(UserEdit)
public:
	/// Constructs a dialog for editing the given userList.
	UserEdit(const MumbleProto::UserList &userList, QWidget *p = nullptr);

public slots:
	void accept() Q_DECL_OVERRIDE;

	void on_qlSearch_textChanged(QString);
	void on_qpbRemove_clicked();
	void on_qpbRename_clicked();
	void on_qtvUserList_customContextMenuRequested(const QPoint &);
	void onSelectionChanged(const QItemSelection & /*selected*/, const QItemSelection & /*deselected*/);
	void onCurrentRowChanged(const QModelIndex &current, const QModelIndex & /*previous*/);
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
	explicit UserListFilterProxyModel(QObject *parent = nullptr);

	bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

public slots:
	/// Sets the amount of inactive days below which rows will get filtered by the proxy
	void setFilterMinimumInactiveDays(int minimumInactiveDays);
	/// Helper function for removing all rows involved in a given selection (must include COL_NICK).
	void removeRowsInSelection(const QItemSelection &selection);

private:
	/// Every row with less inactive days will be filtered.
	int m_minimumInactiveDays;
};


#endif // MUMBLE_MUMBLE_USEREDIT_H_
