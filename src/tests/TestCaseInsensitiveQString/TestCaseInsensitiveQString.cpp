// Copyright 2024 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "QtUtils.h"

#include <QHash>
#include <QObject>
#include <QTest>

using namespace Mumble::QtUtils;

class TestCaseInsensitiveQString : public QObject {
	Q_OBJECT
private slots:

	void equality() const {
		CaseInsensitiveQString istr("test");

		QVERIFY(QString("Test") == istr);
		QVERIFY(istr == QString("TEST"));
		QVERIFY(istr == istr);
	}

	void inequality() const {
		CaseInsensitiveQString istr("test");

		QVERIFY(QString("Tezt") != istr);
		QVERIFY(istr != QString("TETS"));
		QVERIFY(istr != CaseInsensitiveQString("test2"));
	}

	void less_than() const {
		CaseInsensitiveQString istr("test");

		QVERIFY(QString("abc") < istr);
		QVERIFY(istr < QString("xyz"));
		QVERIFY(CaseInsensitiveQString("another") < istr);
	}

	void less_equal() const {
		CaseInsensitiveQString istr("test");

		QVERIFY(QString("Abc") <= istr);
		QVERIFY(istr <= QString("xyz"));
		QVERIFY(CaseInsensitiveQString("Taxi") <= istr);

		QVERIFY(QString("teSt") <= istr);
		QVERIFY(istr <= QString("Test"));
		QVERIFY(CaseInsensitiveQString("TEST") <= istr);
	}

	void greater_than() const {
		CaseInsensitiveQString istr("test");

		QVERIFY(QString("xyz") > istr);
		QVERIFY(istr > QString("abc"));
		QVERIFY(CaseInsensitiveQString("Xenia") > istr);
	}

	void greater_equal() const {
		CaseInsensitiveQString istr("test");

		QVERIFY(QString("xyz") >= istr);
		QVERIFY(istr >= QString("abc"));
		QVERIFY(CaseInsensitiveQString("Xenia") >= istr);

		QVERIFY(QString("Test") >= istr);
		QVERIFY(istr >= QString("teST"));
		QVERIFY(CaseInsensitiveQString("TeSt") >= istr);
	}

	void hash() const {
		QVERIFY(qHash(CaseInsensitiveQString("test")) == qHash(CaseInsensitiveQString("TEST")));
		QVERIFY(qHash(CaseInsensitiveQString("test")) != qHash(CaseInsensitiveQString("TESTER")));
	}

	void use_in_qhash() const {
		QHash< CaseInsensitiveQString, int > hash;
		hash.insert(QString("TeSt"), 42);

		QVERIFY(hash.contains(QString("test")));
		QCOMPARE(hash[QString("test")], 42);
		QCOMPARE(static_cast< const QString & >(hash.begin().key()), QString("TeSt"));
		QVERIFY(static_cast< const QString & >(hash.begin().key()) != QString("test"));
	}
};

QTEST_MAIN(TestCaseInsensitiveQString)
#include "TestCaseInsensitiveQString.moc"
