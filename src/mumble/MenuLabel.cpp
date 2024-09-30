// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MenuLabel.h"
#include "widgets/EventFilters.h"

#include <QGridLayout>
#include <QLabel>

MenuLabel::MenuLabel(const QString &text, QObject *parent) : QWidgetAction(parent), m_text(text) {
	setMenuRole(QAction::NoRole);
}

QWidget *MenuLabel::createWidget(QWidget *parent) {
	QWidget *widget = new QWidget(parent);

	// Not setting any focus is not an alternative here, as the
	// QWidgetAction WILL get and automatically forward focus to the child.
	// The widget needs tab focus policy, so it can process the event filters
	// and also forward the focus.
	widget->setFocusPolicy(Qt::TabFocus);
	widget->installEventFilter(new SkipFocusEventFilter(widget));

	// Using a widget and layout here instead of a plain label
	// because otherwise screen readers might partially read the
	// label text.
	QGridLayout *layout = new QGridLayout();
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	QLabel *label = new QLabel(m_text, parent);
	layout->addWidget(label, 0, 0);
	widget->setLayout(layout);

	return widget;
}
