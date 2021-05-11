// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_SEARCHDIALOGTREE_H_
#define MUMBLE_MUMBLE_WIDGETS_SEARCHDIALOGTREE_H_

#include <QTreeWidget>

class QResizeEvent;

class SearchDialogTree : public QTreeWidget {
public:
	using QTreeWidget::QTreeWidget;

protected:
	void resizeEvent(QResizeEvent *event) override;
};

#endif // MUMBLE_MUMBLE_WIDGETS_SEARCHDIALOGTREE_H_
