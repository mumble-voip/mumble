// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_MUCOMBOBOX_H_
#define MUMBLE_MUMBLE_WIDGETS_MUCOMBOBOX_H_

#include <QComboBox>

class MUComboBox : public QComboBox {
	Q_OBJECT
public:
	MUComboBox(QWidget *parent = nullptr);
};

#endif
