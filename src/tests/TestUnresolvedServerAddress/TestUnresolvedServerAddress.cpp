// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "UnresolvedServerAddress.h"

class TestUnresolvedServerAddress : public QObject {
	Q_OBJECT
private slots:
	void defaultCtor();
	void isValid();
	void ctor();
	void caseInsensitive();
	void equals();
	void lessThan();
	void qhash();
};

void TestUnresolvedServerAddress::defaultCtor() {
	UnresolvedServerAddress unresolved;
	QVERIFY(unresolved.hostname.isEmpty());
	QVERIFY(unresolved.port == 0);
	QVERIFY(!unresolved.isValid());
}

void TestUnresolvedServerAddress::isValid() {
	UnresolvedServerAddress invalid1;
	QVERIFY(!invalid1.isValid());

	UnresolvedServerAddress invalid2(QString(), 0);
	QVERIFY(!invalid2.isValid());

	UnresolvedServerAddress invalid3(QString(), 64738);
	QVERIFY(!invalid3.isValid());

	UnresolvedServerAddress invalid4(QLatin1String("mumble.info"), 0);
	QVERIFY(!invalid4.isValid());

	UnresolvedServerAddress valid(QLatin1String("mumble.info"), 443);
	QVERIFY(valid.isValid());
}

void TestUnresolvedServerAddress::ctor() {
	UnresolvedServerAddress usa(QLatin1String("mumble.info"), 443);
	QCOMPARE(usa.hostname, QString::fromLatin1("mumble.info"));
	QCOMPARE(usa.port, static_cast< unsigned short >(443));
}

void TestUnresolvedServerAddress::caseInsensitive() {
	UnresolvedServerAddress a(QLatin1String("mumble.info"), 443);
	UnresolvedServerAddress b(QLatin1String("MUMBLE.INFO"), 443);
	UnresolvedServerAddress c(QLatin1String("MuMbLe.iNFo"), 443);

	QVERIFY(a == b);
	QVERIFY(a == c);
}

void TestUnresolvedServerAddress::equals() {
	UnresolvedServerAddress a1(QLatin1String("mumble.info"), 443);
	UnresolvedServerAddress a2(QLatin1String("mumble.info"), 443);
	UnresolvedServerAddress b(QLatin1String("mumble.info"), 64738);
	UnresolvedServerAddress c(QLatin1String("mumble.hive.no"), 80);

	QVERIFY(a1 == a2);
	QVERIFY(a1 != b);
	QVERIFY(a1 != c);
	QVERIFY(b != c);
}

void TestUnresolvedServerAddress::lessThan() {
	QList< UnresolvedServerAddress > testdata;

	testdata << UnresolvedServerAddress();
	testdata << UnresolvedServerAddress(QString(), 1);
	testdata << UnresolvedServerAddress(QString(), 999);
	testdata << UnresolvedServerAddress(QLatin1String("aaaaa"), 0);
	testdata << UnresolvedServerAddress(QLatin1String("baaaa"), 0);
	testdata << UnresolvedServerAddress(QLatin1String("baaaa"), 1);
	testdata << UnresolvedServerAddress(QLatin1String("maaaa"), 0);
	testdata << UnresolvedServerAddress(QLatin1String("maaaa"), 100);
	testdata << UnresolvedServerAddress(QLatin1String("maaaa"), 64738);
	testdata << UnresolvedServerAddress(QLatin1String("naaaa"), 64738);
	testdata << UnresolvedServerAddress(QLatin1String("zaaaa"), 0);
	testdata << UnresolvedServerAddress(QLatin1String("zaaaa"), 65535);

	QList< UnresolvedServerAddress > sorted(testdata);
	std::sort(sorted.begin(), sorted.end());

	int i = 0;
	foreach (const UnresolvedServerAddress &unresolved, sorted) {
		qWarning("%i -> %s:%i", i, qPrintable(unresolved.hostname), unresolved.port);
		i += 1;
	}

	QVERIFY(testdata == sorted);
}

void TestUnresolvedServerAddress::qhash() {
	UnresolvedServerAddress a1(QLatin1String("mumble.info"), 443);
	UnresolvedServerAddress a2(QLatin1String("mumble.info"), 443);
	UnresolvedServerAddress b(QLatin1String("mumble.info"), 64738);
	UnresolvedServerAddress c(QLatin1String("mumble.hive.no"), 80);

	QVERIFY(qHash(a1) == qHash(a2));
	QVERIFY(qHash(a1) != qHash(b));
	QVERIFY(qHash(a1) != qHash(c));
	QVERIFY(qHash(b) != qHash(c));
}

QTEST_MAIN(TestUnresolvedServerAddress)
#include "TestUnresolvedServerAddress.moc"
