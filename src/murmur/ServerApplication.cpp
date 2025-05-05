// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ServerApplication.h"

#include <exception>
#include <iostream>

#include <QEvent>
#include <QObject>

ServerApplication::~ServerApplication() {
}

bool ServerApplication::notify(QObject *receiver, QEvent *event) {
	bool handled = true;

	try {
		handled = QCoreApplication::notify(receiver, event);
	} catch (const std::exception &e) {
		std::cerr << "Terminating due to exception with message \"" << e.what() << "\"" << std::endl;
		exit(1);
	} catch (...) {
		std::cerr << "Terminating due to a caught non std::exception" << std::endl;
		exit(1);
	}

	return handled;
}
