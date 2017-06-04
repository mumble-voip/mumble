// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>
#include <QSignalSpy>

#include "ServerResolver.h"

class TestServerResolver : public QObject {
		Q_OBJECT
	private slots:
		void simpleSrv();
		void simpleA();
		void simpleAAAA();
};

void TestServerResolver::simpleSrv() {
#ifdef USE_NO_SRV
	return;
#endif

	ServerResolver r;
	QSignalSpy spy(&r, SIGNAL(resolved()));

	QString hostname = QString::fromLatin1("simple.serverresolver.mumble.info");
	quint16 port = 64738;

	r.resolve(hostname, port);

	// Equivalent to QSignalSpy::wait() in Qt 5.
	{
		QTestEventLoop loop;
		loop.enterLoop(5);
	}

	QCOMPARE(spy.count(), 1);

	QList<ServerResolverRecord> records = r.records();
	QCOMPARE(records.size(), 1);

	ServerResolverRecord record = records.at(0);
	QCOMPARE(record.hostname(), hostname);
	QCOMPARE(record.port(), port);
	// Allow 1 or 2 results. The answer depends on whether
	// the system supports IPv4, IPv6, or both.
	QVERIFY(record.addresses().size() == 1 || record.addresses().size() == 2);
	QCOMPARE(record.priority(), 65545); // priority=1, weight=10 -> 1 * 65535 + 10

	bool hasipv4 = false;
	bool hasipv6 = false;

	HostAddress v4(QHostAddress(QLatin1String("127.0.0.1")));
	HostAddress v6(QHostAddress(QLatin1String("::1")));

	foreach (HostAddress ha, record.addresses()) {
		if (ha == v4) {
			hasipv4 = true;
		}
		if (ha == v6) {
			hasipv6 = true;
		}
	}

	// Require either an IPv4 match, or an IPv6 match.
	QVERIFY(hasipv4 || hasipv6);
}

void TestServerResolver::simpleA() {
	ServerResolver r;
	QSignalSpy spy(&r, SIGNAL(resolved()));

	QString hostname = QString::fromLatin1("simplea.serverresolver.mumble.info");
	quint16 port = 64738;

	r.resolve(hostname, port);

	// Equivalent to QSignalSpy::wait() in Qt 5.
	{
		QTestEventLoop loop;
		loop.enterLoop(5);
	}

	QCOMPARE(spy.count(), 1);

	QList<ServerResolverRecord> records = r.records();
	// Since not all systems have IPv4 support, we have to
	// skip this test if we get no matches. Not ideal, but
	// at least we get some test coverage on IPv4 systems.
	if (records.size() == 0) {
		qWarning("Skipping test: No results returned. Assuming non-IPv4 system.");
		return;
	}

	QCOMPARE(records.size(), 1);

	ServerResolverRecord record = records.at(0);
	QCOMPARE(record.hostname(), hostname);
	QCOMPARE(record.port(), port);
	QCOMPARE(record.addresses().size(), 1);
	QCOMPARE(record.priority(), static_cast<qint64>(0));

	HostAddress want(QHostAddress(QLatin1String("127.0.0.1")));
	HostAddress got = record.addresses().at(0);
	QCOMPARE(want, got);
}

void TestServerResolver::simpleAAAA() {
	ServerResolver r;
	QSignalSpy spy(&r, SIGNAL(resolved()));

	QString hostname = QString::fromLatin1("simpleaaaa.serverresolver.mumble.info");
	quint16 port = 64738;

	r.resolve(hostname, port);

	// Equivalent to QSignalSpy::wait() in Qt 5.
	{
		QTestEventLoop loop;
		loop.enterLoop(5);
	}

	QCOMPARE(spy.count(), 1);

	QList<ServerResolverRecord> records = r.records();
	// Since not all systems have IPv6 support, we have to
	// skip this test if we get no matches. Not ideal, but
	// at least we get some test coverage on IPv6 systems.
	if (records.size() == 0) {
		qWarning("Skipping test: No results returned. Assuming non-IPv6 system.");
		return;
	}

	QCOMPARE(records.size(), 1);

	ServerResolverRecord record = records.at(0);
	QCOMPARE(record.hostname(), hostname);
	QCOMPARE(record.port(), port);
	QCOMPARE(record.addresses().size(), 1);
	QCOMPARE(record.priority(), static_cast<qint64>(0));

	HostAddress want(QHostAddress(QLatin1String("::1")));
	HostAddress got = record.addresses().at(0);
	QCOMPARE(want, got);
}

QTEST_MAIN(TestServerResolver)
#include "TestServerResolver.moc"
