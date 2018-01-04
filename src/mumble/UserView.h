// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_USERVIEW_H_
#define MUMBLE_MUMBLE_USERVIEW_H_

#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
# include <QtWidgets/QStyledItemDelegate>
# include <QtWidgets/QTreeView>
#else 
# include <QtGui/QStyledItemDelegate>
# include <QtGui/QTreeView>
#endif

#include "Timer.h"

class UserDelegate : public QStyledItemDelegate {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(UserDelegate)
	public:
		UserDelegate(QObject *parent = NULL);
		void paint(QPainter * painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

		//! Width/height in px of user/channel flag icons
		const static int FLAG_ICON_DIMENSION;
		//! Padding in px around user/channel flag icons
		const static int FLAG_ICON_PADDING;
		//! Width/height in px of user/channel flags including padding
		const static int FLAG_DIMENSION;

	public slots:
		bool helpEvent(QHelpEvent *event, QAbstractItemView *view, const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;
};

class UserView : public QTreeView {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(UserView)
	protected:
		void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;
		void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;
		bool event(QEvent *) Q_DECL_OVERRIDE;

		QTimer *qtSearch;
		QPersistentModelIndex qpmiSearch;
		Timer tSearch;
		QString qsSearch;
	public:
		UserView(QWidget *);
		void keyboardSearch(const QString &search) Q_DECL_OVERRIDE;
#if QT_VERSION >= 0x050000
		void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int> ()) Q_DECL_OVERRIDE;
#else
		void dataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight) Q_DECL_OVERRIDE;
#endif
		
	public slots:
		void nodeActivated(const QModelIndex &idx);
		void selectSearchResult();
		void updateChannel(const QModelIndex &index);
};

#endif
