// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_EVENTFILTERS_H_
#define MUMBLE_MUMBLE_WIDGETS_EVENTFILTERS_H_

#include <vector>

#include <QEvent>
#include <QObject>
#include <QPoint>

class KeyEventObserver : public QObject {
	Q_OBJECT

public:
	KeyEventObserver(QObject *parent, QEvent::Type eventType, bool consume, std::vector< Qt::Key > keys);

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;

signals:
	void keyEventObserved();

private:
	QEvent::Type m_eventType;
	bool m_consume;
	std::vector< Qt::Key > m_keys;
};

class MouseWheelEventObserver : public QObject {
	Q_OBJECT

public:
	MouseWheelEventObserver(QObject *parent, std::vector< Qt::ScrollPhase > phases, bool consume);

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;

signals:
	void wheelEventObserved(QPoint delta);

private:
	std::vector< Qt::ScrollPhase > m_phases;
	bool m_consume;
};

class MouseClickEventObserver : public QObject {
	Q_OBJECT

public:
	MouseClickEventObserver(QObject *parent, bool consume);

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;

signals:
	void clickEventObserved(Qt::MouseButtons buttons);

private:
	bool m_consume;
};

class UpDownKeyEventFilter : public QObject {
	Q_OBJECT

public:
	UpDownKeyEventFilter(QObject *parent);

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
};

class OverrideTabOrderFilter : public QObject {
	Q_OBJECT

public:
	OverrideTabOrderFilter(QObject *parent, QWidget *target);
	QWidget *focusTarget;

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
};

class SkipFocusEventFilter : public QObject {
	Q_OBJECT

public:
	SkipFocusEventFilter(QObject *parent);

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
};

class FocusEventObserver : public QObject {
	Q_OBJECT

public:
	FocusEventObserver(QObject *parent, bool consume);

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;

signals:
	void focusInObserved(Qt::FocusReason reason);
	void focusOutObserved(Qt::FocusReason reason);

private:
	bool m_consume;
};

#endif
