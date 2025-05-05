// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_MULTICOLUMNTREEWIDGET_H_
#define MUMBLE_MUMBLE_WIDGETS_MULTICOLUMNTREEWIDGET_H_

#include <QAbstractItemView>
#include <QModelIndex>
#include <QTreeWidget>

class MultiColumnTreeWidget : public QTreeWidget {
	Q_OBJECT

public:
	MultiColumnTreeWidget(QWidget *parent = nullptr);

	QModelIndex moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
};

#endif
