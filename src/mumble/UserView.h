// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_USERVIEW_H_
#define MUMBLE_MUMBLE_USERVIEW_H_

#include <QtCore/QtGlobal>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QMenu>

#include "QtUtils.h"
#include "Timer.h"
#include "UserHistory.h"

class UserDelegate : public QStyledItemDelegate {
private:
	Q_OBJECT
	Q_DISABLE_COPY(UserDelegate)

	int m_iconTotalDimension;
	int m_iconIconPadding;
	int m_iconIconDimension;

public:
	UserDelegate(QObject *parent);
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
	void adjustIcons(int iconTotalDimension, int iconIconPadding, int iconIconDimension);

public slots:
	bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option,
				   const QModelIndex &index) Q_DECL_OVERRIDE;
};

class UserView : public QTreeView {
private:
	Q_OBJECT
	Q_DISABLE_COPY(UserView)

	int m_iconTotalDimension;
	qt_unique_ptr< UserDelegate > m_userDelegate;
	void adjustIcons();
	void showContextMenu(const QPoint& pos);

protected:
	void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
	bool event(QEvent *) Q_DECL_OVERRIDE;
	void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

public:
	UserView(QWidget *);
	void keyboardSearch(const QString &search) Q_DECL_OVERRIDE;
	void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
					 const QVector< int > &roles = QVector< int >()) Q_DECL_OVERRIDE;

public slots:
	void nodeActivated(const QModelIndex &idx);
	void updateChannel(const QModelIndex &index);
	void banUser(const QString& hash);
};

#endif
