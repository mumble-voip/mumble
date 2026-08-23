// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include <algorithm>

#include "HostAddress.h"
#include "ServerAddress.h"

class TestServerAddress : public QObject {
	Q_OBJECT
private slots:
	void defaultCtor();
	void isValid();
	void ctor();
	void equals();
	void lessThan();
	void qhash();
	void match();
	void ipv6ToString();
};

void TestServerAddress::defaultCtor() {
	ServerAddress sa;
	QVERIFY(sa.host == HostAddress());
	QVERIFY(sa.port == 0);
	QVERIFY(!sa.isValid());
}

void TestServerAddress::isValid() {
	ServerAddress invalid1;
	QVERIFY(!invalid1.isValid());

	ServerAddress invalid2(HostAddress(), 0);
	QVERIFY(!invalid2.isValid());

	ServerAddress invalid3(HostAddress(), 64738);
	QVERIFY(!invalid3.isValid());

	ServerAddress invalid4(HostAddress(QHostAddress("127.0.0.1")), 0);
	QVERIFY(!invalid4.isValid());

	ServerAddress valid(HostAddress(QHostAddress("127.0.0.1")), 443);
	QVERIFY(valid.isValid());
}

void TestServerAddress::ctor() {
	ServerAddress sa(HostAddress(QHostAddress("127.0.0.1")), 443);
	QCOMPARE(sa.host, HostAddress(QHostAddress("127.0.0.1")));
	QCOMPARE(sa.port, static_cast< unsigned short >(443));
}

void TestServerAddress::equals() {
	ServerAddress a1(HostAddress(QHostAddress("127.0.0.1")), 443);
	ServerAddress a2(HostAddress(QHostAddress("127.0.0.1")), 443);
	ServerAddress b(HostAddress(QHostAddress("127.0.0.1")), 64738);
	ServerAddress c(HostAddress(QHostAddress("10.0.0.1")), 80);

	QVERIFY(a1 == a2);
	QVERIFY(a1 != b);
	QVERIFY(a1 != c);
	QVERIFY(b != c);
}

void TestServerAddress::lessThan() {
	QList< ServerAddress > testdata;

	testdata << ServerAddress();
	testdata << ServerAddress(HostAddress(), 1);
	testdata << ServerAddress(HostAddress(), 999);
	testdata << ServerAddress(HostAddress(QHostAddress("0.0.0.1")), 0);
	testdata << ServerAddress(HostAddress(QHostAddress("0.0.0.2")), 0);
	testdata << ServerAddress(HostAddress(QHostAddress("0.0.0.2")), 1);
	testdata << ServerAddress(HostAddress(QHostAddress("80.0.0.1")), 0);
	testdata << ServerAddress(HostAddress(QHostAddress("80.0.0.1")), 100);
	testdata << ServerAddress(HostAddress(QHostAddress("80.0.0.1")), 64738);
	testdata << ServerAddress(HostAddress(QHostAddress("80.0.0.2")), 64738);
	testdata << ServerAddress(HostAddress(QHostAddress("255.255.255.255")), 0);
	testdata << ServerAddress(HostAddress(QHostAddress("255.255.255.255")), 65535);

	QList< ServerAddress > sorted(testdata);
	std::sort(sorted.begin(), sorted.end());
	QVERIFY(testdata == sorted);
}

void TestServerAddress::qhash() {
	ServerAddress a1(HostAddress(QHostAddress("127.0.0.1")), 443);
	ServerAddress a2(HostAddress(QHostAddress("127.0.0.1")), 443);
	ServerAddress b(HostAddress(QHostAddress("127.0.0.1")), 64738);
	ServerAddress c(HostAddress(QHostAddress("10.0.0.1")), 80);

	QVERIFY(qHash(a1) == qHash(a2));
	QVERIFY(qHash(a1) != qHash(b));
	QVERIFY(qHash(a1) != qHash(c));
	QVERIFY(qHash(b) != qHash(c));
}

void TestServerAddress::match() {
	// A /20 IPv4 prefix is a 116-bit prefix (96 + 20) whose last compared byte is only partially masked.
	HostAddress net(QHostAddress("10.16.0.0"));

	QVERIFY(net.match(HostAddress(QHostAddress("10.16.15.255")), 116));
	QVERIFY(!net.match(HostAddress(QHostAddress("10.16.16.0")), 116));

	// Byte-aligned masks were already handled correctly; keep them covered.
	QVERIFY(net.match(HostAddress(QHostAddress("10.16.99.99")), 112));
	QVERIFY(!net.match(HostAddress(QHostAddress("10.17.0.0")), 112));
}

void TestServerAddress::ipv6ToString() {
	// An address with more than one separate run of zero groups must still render
	// as a valid IPv6 string (only the longest run may be collapsed to "::").
	// Previously the output contained two "::" and could not be parsed back.
	const char *addresses[] = { "2001:db8:0:0:1:0:0:2", "fe80:0:0:1:0:0:0:1", "2001:0:1:0:0:0:0:1" };
	for (const char *address : addresses) {
		const QHostAddress qHostAddress = QHostAddress(QLatin1String(address));
		const HostAddress original(qHostAddress);
		const QString rendered = original.toString(false);
		const QHostAddress parsed(rendered);

		QCOMPARE(parsed.protocol(), QAbstractSocket::IPv6Protocol);
		QCOMPARE(HostAddress(parsed), original);
	}
}

QTEST_MAIN(TestServerAddress)
#include "TestServerAddress.moc"
