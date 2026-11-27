// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "QtUtils.h"

#include <QObject>
#include <QTest>
#include <QTime>

class TestQtUtils : public QObject {
	Q_OBJECT
private slots:
	void formatTime() const {
		QCOMPARE(Mumble::QtUtils::formatTime(QTime(0, 5, 9), true), QStringLiteral("00:05:09"));
		QCOMPARE(Mumble::QtUtils::formatTime(QTime(0, 5, 9), false), QStringLiteral("12:05:09 AM"));

		QCOMPARE(Mumble::QtUtils::formatTime(QTime(9, 5, 3), true), QStringLiteral("09:05:03"));
		QCOMPARE(Mumble::QtUtils::formatTime(QTime(9, 5, 3), false), QStringLiteral("09:05:03 AM"));

		QCOMPARE(Mumble::QtUtils::formatTime(QTime(12, 0, 0), true), QStringLiteral("12:00:00"));
		QCOMPARE(Mumble::QtUtils::formatTime(QTime(12, 0, 0), false), QStringLiteral("12:00:00 PM"));

		QCOMPARE(Mumble::QtUtils::formatTime(QTime(15, 42, 7), true), QStringLiteral("15:42:07"));
		QCOMPARE(Mumble::QtUtils::formatTime(QTime(15, 42, 7), false), QStringLiteral("03:42:07 PM"));

		QCOMPARE(Mumble::QtUtils::formatTime(QTime(23, 59, 59), true), QStringLiteral("23:59:59"));
		QCOMPARE(Mumble::QtUtils::formatTime(QTime(23, 59, 59), false), QStringLiteral("11:59:59 PM"));

		QCOMPARE(Mumble::QtUtils::formatTime(QTime(), true), QString());
		QCOMPARE(Mumble::QtUtils::formatTime(QTime(), false), QString());
	}
};

QTEST_MAIN(TestQtUtils)
#include "TestQtUtils.moc"
