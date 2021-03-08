// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "QtWidgetUtils.h"

#include <QFontMetrics>
#include <QGuiApplication>
#include <QScreen>
#include <QTextCursor>
#include <QTextDocument>

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

	void elideText(QTextDocument &doc, uint32_t width) {
		if (doc.size().width() > width) {
			// Elide text
			QTextCursor cursor(&doc);
			cursor.movePosition(QTextCursor::End);

			const QString elidedPostfix = "...";
			QFontMetrics metric(doc.defaultFont());
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
			uint32_t postfixWidth = metric.horizontalAdvance(elidedPostfix);
#else
			uint32_t postfixWidth = metric.width(elidedPostfix);
#endif

			while (doc.size().width() > std::max(width - postfixWidth, static_cast< uint32_t >(0))) {
				cursor.deletePreviousChar();
				doc.adjustSize();
			}

			cursor.insertText(elidedPostfix);
		}
	}

}; // namespace QtUtils
}; // namespace Mumble
