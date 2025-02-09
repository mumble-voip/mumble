// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "EventFilters.h"

#include <algorithm>

#include <QApplication>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QWidget>

KeyEventObserver::KeyEventObserver(QObject *parent, QEvent::Type eventType, bool consume, std::vector< Qt::Key > keys)
	: QObject(parent), m_eventType(eventType), m_consume(consume), m_keys(std::move(keys)) {
}

bool KeyEventObserver::eventFilter(QObject *obj, QEvent *event) {
	QWidget *widget = static_cast< QWidget * >(obj);

	if (!widget || !widget->hasFocus()) {
		return false;
	}

	QKeyEvent *keyEvent = static_cast< QKeyEvent * >(event);

	if (!keyEvent || keyEvent->type() != m_eventType) {
		return false;
	}

	Qt::Key key = static_cast< Qt::Key >(keyEvent->key());

	if (std::find(m_keys.begin(), m_keys.end(), key) == m_keys.end()) {
		return false;
	}

	emit keyEventObserved();

	return m_consume;
}

MouseWheelEventObserver::MouseWheelEventObserver(QObject *parent, std::vector< Qt::ScrollPhase > phases, bool consume)
	: QObject(parent), m_phases(std::move(phases)), m_consume(consume) {
}

bool MouseWheelEventObserver::eventFilter(QObject *obj, QEvent *event) {
	QWidget *widget = static_cast< QWidget * >(obj);

	if (!widget || !widget->isVisible()) {
		return false;
	}

	QWheelEvent *wheelEvent = static_cast< QWheelEvent * >(event);

	if (!wheelEvent) {
		return false;
	}

	Qt::ScrollPhase phase = wheelEvent->phase();

	if (std::find(m_phases.begin(), m_phases.end(), phase) == m_phases.end()) {
		return false;
	}

	emit wheelEventObserved(wheelEvent->pixelDelta());

	return m_consume;
}

MouseClickEventObserver::MouseClickEventObserver(QObject *parent, bool consume) : QObject(parent), m_consume(consume) {
}

bool MouseClickEventObserver::eventFilter(QObject *obj, QEvent *event) {
	if (event->type() == QEvent::MouseButtonRelease) {
		QMouseEvent *mouseEvent = static_cast< QMouseEvent * >(event);

		emit clickEventObserved(mouseEvent->buttons());

		return m_consume;
	}

	return QObject::eventFilter(obj, event);
}

UpDownKeyEventFilter::UpDownKeyEventFilter(QObject *parent) : QObject(parent) {
}

bool UpDownKeyEventFilter::eventFilter(QObject *obj, QEvent *event) {
	// Converts up/down to tab/backtab
	// Useful when overriding interactive QWidgetActions such as sliders

	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast< QKeyEvent * >(event);

		if (keyEvent->key() == Qt::Key_Up) {
			QKeyEvent *keyPress = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backtab, Qt::NoModifier);
			QApplication::sendEvent(QApplication::focusWidget(), keyPress);
			return true;
		}

		if (keyEvent->key() == Qt::Key_Down) {
			QKeyEvent *keyPress = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
			QApplication::sendEvent(QApplication::focusWidget(), keyPress);
			return true;
		}
	}

	return QObject::eventFilter(obj, event);
}

OverrideTabOrderFilter::OverrideTabOrderFilter(QObject *parent, QWidget *target)
	: QObject(parent), focusTarget(target) {
}

bool OverrideTabOrderFilter::eventFilter(QObject *obj, QEvent *event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast< QKeyEvent * >(event);

		if (keyEvent->key() == Qt::Key_Tab && QApplication::focusWidget() == obj) {
			focusTarget->setFocus(Qt::TabFocusReason);
			return true;
		}
	}

	return QObject::eventFilter(obj, event);
}

SkipFocusEventFilter::SkipFocusEventFilter(QObject *parent) : QObject(parent) {
}

bool SkipFocusEventFilter::eventFilter(QObject *obj, QEvent *event) {
	// Detecting FocusIn events is glitchy, therefore we detect key release events
	// and forward the focus to the next/previous element.

	if (event->type() == QEvent::KeyRelease && QApplication::focusWidget() == obj) {
		QKeyEvent *keyEvent = static_cast< QKeyEvent * >(event);

		if (keyEvent->key() == Qt::Key_Up || keyEvent->key() == Qt::Key_Backtab) {
			QKeyEvent *keyPress = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backtab, Qt::NoModifier);
			QApplication::sendEvent(QApplication::focusWidget(), keyPress);
			return true;
		}

		if (keyEvent->key() == Qt::Key_Down || keyEvent->key() == Qt::Key_Tab) {
			QKeyEvent *keyPress = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab, Qt::NoModifier);
			QApplication::sendEvent(QApplication::focusWidget(), keyPress);
			return true;
		}
	}

	return QObject::eventFilter(obj, event);
}

FocusEventObserver::FocusEventObserver(QObject *parent, bool consume) : QObject(parent), m_consume(consume) {
}

bool FocusEventObserver::eventFilter(QObject *obj, QEvent *event) {
	QWidget *widget = static_cast< QWidget * >(obj);

	if (widget && event->type() == QEvent::FocusAboutToChange) {
		QFocusEvent *focusEvent = static_cast< QFocusEvent * >(event);

		if (!widget->hasFocus() && focusEvent->gotFocus()) {
			emit focusInObserved(focusEvent->reason());
			return m_consume;
		} else if (widget->hasFocus() && !focusEvent->gotFocus()) {
			emit focusOutObserved(focusEvent->reason());
			return m_consume;
		}
	}

	return QObject::eventFilter(obj, event);
}
