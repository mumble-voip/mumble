// Copyright 2019-2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Screen.h"

#include "MumbleApplication.h"

#include <QScreen>
#include <QWidget>
#include <QWindow>

namespace Mumble {
namespace Screen {

	QWindow *windowFromWidget(const QWidget &widget) {
		QWindow *window = widget.windowHandle();
		if (window) {
			return window;
		}

		const QWidget *parent = widget.nativeParentWidget();
		if (parent) {
			return parent->windowHandle();
		}

		return nullptr;
	}

	QScreen *screenFromWidget(const QWidget &widget) {
		const QWindow *window = windowFromWidget(widget);
		if (window && window->screen()) {
			return window->screen();
		}

		return qApp->primaryScreen();
	}

	QScreen *screenAt(const QPoint &point) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
		return qApp->screenAt(point);
#else
		// Adapted from qguiapplication.cpp (Qt)
		QVarLengthArray< const QScreen *, 8 > visitedScreens;

		for (const QScreen *screen : qApp->screens()) {
			if (visitedScreens.contains(screen)) {
				continue;
			}

			// The virtual siblings include the screen itself, so iterate directly
			for (QScreen *sibling : screen->virtualSiblings()) {
				if (sibling->geometry().contains(point)) {
					return sibling;
				}

				visitedScreens.append(sibling);
			}
		}

		return nullptr;
#endif
	}

}; // namespace Screen
}; // namespace Mumble
