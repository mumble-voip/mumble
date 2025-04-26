// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SERVER_SERVERAPPLICATION_H_
#define MUMBLE_SERVER_SERVERAPPLICATION_H_

#include <QtGlobal>

#ifdef Q_OS_WIN
#	include <QApplication>
#else
#	include <QCoreApplication>
#endif

class QObject;
class QEvent;

#ifdef Q_OS_WIN
class ServerApplication : public QApplication {
#else
class ServerApplication : public QCoreApplication {
#endif
	Q_OBJECT

public:
#ifdef Q_OS_WIN
	using QApplication::QApplication;
#else
	using QCoreApplication::QCoreApplication;
#endif
	~ServerApplication();

	/**
	 * We override this function that is responsible for performing the actual
	 * signal-slot-calling in order to be able to catch exceptions in there.
	 */
	bool notify(QObject *receiver, QEvent *event) override;
};

#endif // MUMBLE_SERVER_SERVERAPPLICATION_H_
