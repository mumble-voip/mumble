// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MultiColumnTreeWidget.h"

#include <algorithm>

MultiColumnTreeWidget::MultiColumnTreeWidget(QWidget *parent) : QTreeWidget(parent) {
}

QModelIndex MultiColumnTreeWidget::moveCursor(QAbstractItemView::CursorAction cursorAction,
											  Qt::KeyboardModifiers modifiers) {
	QModelIndex mi = QTreeWidget::moveCursor(cursorAction, modifiers);

	if (cursorAction == QAbstractItemView::MoveLeft) {
		mi = model()->index(mi.row(), std::max(0, mi.column() - 1));
	}

	if (cursorAction == QAbstractItemView::MoveRight) {
		mi = model()->index(mi.row(), std::min(model()->columnCount() - 1, mi.column() + 1));
	}

	if (cursorAction == QAbstractItemView::MoveUp || cursorAction == QAbstractItemView::MoveDown) {
		mi = model()->index(mi.row(), 0);
	}

	return mi;
}
