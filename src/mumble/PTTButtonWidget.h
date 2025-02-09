// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_PTTBUTTONWIDGET_H_
#define MUMBLE_MUMBLE_PTTBUTTONWIDGET_H_

#include "ui_PTTButtonWidget.h"

class PTTButtonWidget : public QWidget, public Ui::qwPTTButtonWidget {
	Q_OBJECT
	Q_DISABLE_COPY(PTTButtonWidget)
protected:
	void closeEvent(QCloseEvent *e) Q_DECL_OVERRIDE;

public:
	PTTButtonWidget(QWidget *p = 0);
public slots:
	void on_qpbPushToTalk_pressed();
	void on_qpbPushToTalk_released();
signals:
	void triggered(bool checked, QVariant);
};

#endif // PTTBUTTONWIDGET_H_
