// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_QTWIDGETUTILS_H_
#define MUMBLE_MUMBLE_QTWIDGETUTILS_H_

#include <cstdint>

#include <QPoint>

class QScreen;
class QTextDocument;

namespace Mumble {
namespace QtUtils {

	/**
	 * @returns The QScreen the given position is on or nullptr if there is no such screen
	 */
	QScreen *screenAt(QPoint position);

	/**
	 * @returns Whether the given position is on any screen (as opposed to off-screen)
	 */
	bool positionIsOnScreen(QPoint position);

	void elideText(QTextDocument &doc, uint32_t width);

} // namespace QtUtils
} // namespace Mumble

#endif // MUMBLE_MUMBLE_QTWIDGETUTILS_H_
