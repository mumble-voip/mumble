// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MUComboBox.h"

#include <QtWidgets/QListView>

MUComboBox::MUComboBox(QWidget *parent) : QComboBox(parent) {
	// Set the QComboBox to be backed by a QListView.
	// By default on macOS, QComboBoxes are backed by
	// something that tries to emulate a native macOS
	// menu.
	//
	// However, that QAbstractItemView behaves
	// inconsistently when styled. For example, it does
	// not seem possible to set the size of individual
	// items, because they're restricted to the height
	// of a normal macOS menu item.
	// Also, at least for this QComboBox (which lives
	// inside a QToolbar), the height of the QAbstractItemView
	// was also wrong when styled. This caused the combo box
	// to always scroll, even though it seemingly was sized
	// correctly.
	//
	// To get consistent behavior, we use QListView instead.
	QListView *lv = new QListView();
	// Don't show ellipses. In this combo box, the
	// text does fit -- and is resized automatically to fit.
	// But ellipses are added anyway.
	// So, forcefully disable them.
	lv->setTextElideMode(Qt::ElideNone);

	this->setView(lv);
}
