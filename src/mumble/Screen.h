// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCREEN_H
#define MUMBLE_MUMBLE_SCREEN_H

class QPoint;
class QScreen;
class QWidget;
class QWindow;

namespace Mumble {

namespace Screen {
	/// Inspired by https://phabricator.kde.org/D22379
	QWindow *windowFromWidget(const QWidget &widget);
	QScreen *screenFromWidget(const QWidget &widget);
	QScreen *screenAt(const QPoint &point);
} // namespace Screen

} // namespace Mumble

#endif
