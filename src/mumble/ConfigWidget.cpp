// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_pch.hpp"

#include "ConfigWidget.h"

QMap<int, ConfigWidgetNew> *ConfigRegistrar::c_qmNew;

ConfigRegistrar::ConfigRegistrar(int priority, ConfigWidgetNew n) {
	if (! c_qmNew)
		c_qmNew = new QMap<int, ConfigWidgetNew>();
	iPriority = priority;
	c_qmNew->insert(priority,n);
}

ConfigRegistrar::~ConfigRegistrar() {
	c_qmNew->remove(iPriority);
	if (c_qmNew->isEmpty()) {
		delete c_qmNew;
		c_qmNew = NULL;
	}
}

ConfigWidget::ConfigWidget(Settings &st) : s(st) {
}

QIcon ConfigWidget::icon() const {
	return qApp->windowIcon();
}

void ConfigWidget::accept() const {
}

void ConfigWidget::loadSlider(QSlider *slider, int v) {
	if (v != slider->value()) {
		slider->setValue(v);
	} else {
		connect(this, SIGNAL(intSignal(int)), slider, SIGNAL(valueChanged(int)));
		emit intSignal(v);
		disconnect(SIGNAL(intSignal(int)));
	}
}

void ConfigWidget::loadCheckBox(QAbstractButton *c, bool v) {
	if (v != c->isChecked()) {
		c->setChecked(v);
	} else {
		connect(this, SIGNAL(intSignal(int)), c, SIGNAL(stateChanged(int)));
		emit intSignal(v ? 1 : 0);
		disconnect(SIGNAL(intSignal(int)));
	}
}

void ConfigWidget::loadComboBox(QComboBox *c, int v) {
	if (c->currentIndex() != v) {
		c->setCurrentIndex(v);
	} else {
		connect(this, SIGNAL(intSignal(int)), c, SIGNAL(currentIndexChanged(int)));
		emit intSignal(v);
		disconnect(SIGNAL(intSignal(int)));
	}
}

