#include "mumble_pch.hpp"

#include "PTTButtonWidget.h"

#include "Global.h"

PTTButtonWidget::PTTButtonWidget(QWidget *parent) :
		QWidget(parent) {
	setupUi(this);

	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);

	if (!g.s.qbaPTTButtonWindowGeometry.isEmpty()) {
		restoreGeometry(g.s.qbaPTTButtonWindowGeometry);
	}
}

void PTTButtonWidget::closeEvent(QCloseEvent *event) {
	g.s.qbaPTTButtonWindowGeometry = saveGeometry();
	QWidget::closeEvent(event);
}

void PTTButtonWidget::on_qpbPushToTalk_pressed() {
	emit triggered(true, QVariant());
}

void PTTButtonWidget::on_qpbPushToTalk_released() {
	emit triggered(false, QVariant());
}
