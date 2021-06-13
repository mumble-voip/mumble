// Copyright 2021 The Mumble Developers. All rights reserved.
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
