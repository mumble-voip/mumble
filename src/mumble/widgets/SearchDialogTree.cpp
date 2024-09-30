// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SearchDialogTree.h"

#include <QResizeEvent>

void SearchDialogTree::resizeEvent(QResizeEvent *event) {
	// We have to update the layout on every resize since we have wrapping text that is displayed in the
	// different rows of this tree. That means that by changing its size we potentially change the way
	// the text is being wrapped in these rows, causing their vertical space requirements to change. This
	// makes it necessary to re-adjust to layout to these (potential) new requirements.

	QTreeWidget::resizeEvent(event);

	scheduleDelayedItemsLayout();
}

QModelIndex SearchDialogTree::moveCursor(QAbstractItemView::CursorAction cursorAction,
										 Qt::KeyboardModifiers modifiers) {
	// Hack to make screen readers read search results...

	QModelIndex mi = QTreeWidget::moveCursor(cursorAction, modifiers);

	if (cursorAction == QAbstractItemView::MoveUp || cursorAction == QAbstractItemView::MoveDown) {
		mi = model()->index(mi.row(), 1);
	}

	return mi;
}
