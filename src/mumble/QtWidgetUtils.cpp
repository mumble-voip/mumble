// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "QtWidgetUtils.h"

#include <QFontMetrics>
#include <QGuiApplication>
#include <QScreen>
#include <QTextCursor>
#include <QTextDocument>

#include <cstdint>

namespace Mumble {
namespace QtUtils {
	QScreen *screenAt(QPoint point) { return QGuiApplication::screenAt(point); }

	bool positionIsOnScreen(QPoint point) { return screenAt(point) != nullptr; }

	void elideText(QTextDocument &doc, uint32_t width) {
		if (doc.size().width() > width) {
			// Elide text
			QTextCursor cursor(&doc);
			cursor.movePosition(QTextCursor::End);

			const QString elidedPostfix = "…";
			QFontMetrics metric(doc.defaultFont());
			uint32_t postfixWidth = static_cast< std::uint32_t >(metric.horizontalAdvance(elidedPostfix));

			while (doc.size().width() > std::max(width - postfixWidth, static_cast< uint32_t >(0))) {
				cursor.deletePreviousChar();
				doc.adjustSize();
			}

			cursor.insertText(elidedPostfix);
		}
	}

} // namespace QtUtils
} // namespace Mumble
