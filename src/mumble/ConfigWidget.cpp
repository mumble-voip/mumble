/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

