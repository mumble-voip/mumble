// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SIGNAL_CURRY_H_
#define MUMBLE_SIGNAL_CURRY_H_

#include <QObject>
#include <QVariant>

/**
 * \brief Small class for binding a QVariant parameter to a parameterless signal.
 * Static usage:
 *    SignalCurry::curry(sender, SIGNAL(emitted()), receiver, SLOT(called(QVariant)), curryParameter);
 */
class SignalCurry : public QObject {
	Q_OBJECT
	Q_DISABLE_COPY(SignalCurry)
private:
	bool bDeleteAfterFirstUse;
	QVariant qvData;

public:
	SignalCurry(QVariant data, bool deleteAfterFirstUse = false, QObject *p = 0)
		: QObject(p), bDeleteAfterFirstUse(deleteAfterFirstUse), qvData(data) {}

	static void curry(QObject *sender, const char *signal, QObject *receiver, const char *slot, QVariant data) {
		SignalCurry *c = new SignalCurry(data);
		connect(receiver, slot, c, SIGNAL(called(QVariant)));
		connect(sender, signal, c, SLOT(call));
	}

signals:
	void called(QVariant data);
public slots:
	void call() {
		emit called(qvData);
		if (bDeleteAfterFirstUse)
			deleteLater();
	}
};


#endif
