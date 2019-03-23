// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PTTButtonWidget.h"

#include "Global.h"

PTTButtonWidget::PTTButtonWidget(QWidget *p) :
		QWidget(p) {
	setupUi(this);

	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);

	if (!g.s.qbaPTTButtonWindowGeometry.isEmpty()) {
		restoreGeometry(g.s.qbaPTTButtonWindowGeometry);
	}
}

void PTTButtonWidget::closeEvent(QCloseEvent *e) {
	g.s.qbaPTTButtonWindowGeometry = saveGeometry();
	QWidget::closeEvent(e);
}

void PTTButtonWidget::on_qpbPushToTalk_pressed() {
	emit triggered(true, QVariant());
}

void PTTButtonWidget::on_qpbPushToTalk_released() {
	emit triggered(false, QVariant());
}
