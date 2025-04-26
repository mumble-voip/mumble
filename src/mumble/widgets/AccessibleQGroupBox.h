// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_ACCESSIBLEQGROUPBOX_H_
#define MUMBLE_MUMBLE_ACCESSIBLEQGROUPBOX_H_

#include <QGridLayout>
#include <QGroupBox>

class AccessibleQGroupBox : public QGroupBox {
	Q_OBJECT

public:
	AccessibleQGroupBox(QWidget *parent);

	QString textAtPosition(QGridLayout *gridLayout, int y, int x);

	void updateAccessibleText();
};

#endif
