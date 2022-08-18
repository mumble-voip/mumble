// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MenuLabel.h"

#include <QLabel>

MenuLabel::MenuLabel(const QString &text, QObject *parent) : QWidgetAction(parent), m_text(text) {
}

QWidget *MenuLabel::createWidget(QWidget *parent) {
	QLabel *label = new QLabel(m_text, parent);
	return label;
}
