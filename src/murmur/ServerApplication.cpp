// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerApplication.h"
#include "Logger.h"

#include <exception>

#include <QEvent>
#include <QObject>

ServerApplication::~ServerApplication() {
}

bool ServerApplication::notify(QObject *receiver, QEvent *event) {
	bool handled = true;

	try {
#ifdef Q_OS_WIN
		handled = QApplication::notify(receiver, event);
#else
		handled = QCoreApplication::notify(receiver, event);
#endif
	} catch (const std::exception &e) {
		mumble::log::fatal("Terminating due to exception with message \"{}\"", e.what());
	} catch (...) {
		mumble::log::fatal("Terminating due to a caught non std::exception");
	}

	return handled;
}
