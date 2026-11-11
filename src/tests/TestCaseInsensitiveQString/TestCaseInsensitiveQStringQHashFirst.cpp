// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// Intentionally include <QHash> before "QtUtils.h" to verify that ADL finds
// Mumble::QtUtils::qHash regardless of header include order.
#include <QHash>

#include "QtUtils.h"

#include <QObject>
#include <QTest>

class TestCaseInsensitiveQStringQHashFirst : public QObject {
	Q_OBJECT
private slots:
	void hash_and_qhash_with_qhash_included_first() const {
		using Mumble::QtUtils::CaseInsensitiveQString;

		QVERIFY(qHash(CaseInsensitiveQString("test")) == qHash(CaseInsensitiveQString("TEST")));

		QHash< CaseInsensitiveQString, int > hash;
		hash.insert(QString("TeSt"), 42);

		QVERIFY(hash.contains(QString("test")));
		QCOMPARE(hash.value(QString("TEST")), 42);
		QVERIFY(hash.remove(QString("tEsT")));
		QVERIFY(hash.isEmpty());
	}
};

QTEST_MAIN(TestCaseInsensitiveQStringQHashFirst)
#include "TestCaseInsensitiveQStringQHashFirst.moc"
