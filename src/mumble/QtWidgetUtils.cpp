// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "QtWidgetUtils.h"

#include <QGuiApplication>
#include <QScreen>

namespace Mumble {
namespace QtUtils {
	QScreen *screenAt(QPoint point) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
		// screenAt was only introduced in Qt 5.10
		return QGuiApplication::screenAt(point);
#else
		for (QScreen *currentScreen : QGuiApplication::screens()) {
			if (currentScreen->availableGeometry().contains(point)) {
				return currentScreen;
			}
		}

		return nullptr;
#endif
	}

	bool positionIsOnScreen(QPoint point) { return screenAt(point) != nullptr; }

}; // namespace QtUtils
}; // namespace Mumble
