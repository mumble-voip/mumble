// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "PacketDataStream.h"

#include <QObject>
#include <QtCore>
#include <QtNetwork>
#include <QtTest>

class TestPacketDataStream : public QObject {
	Q_OBJECT
private slots:
	void integer();
	void integer_data();
	void string();
	void string_data();
	void space();
	void floating();
	void floating_data();
	void undersize();
};

void TestPacketDataStream::floating_data() {
	QTest::addColumn< double >("value");
	for (int i = 1; i < 256; i++) {
		double v = 1.0 / (1.0 * i);
		QTest::newRow("Positive") << v;
		QTest::newRow("Negative") << -v;
	}
}

void TestPacketDataStream::floating() {
	QFETCH(double, value);

	char buff[256];
	double d;

	PacketDataStream out(buff, 256);
	out << value;
	PacketDataStream in(buff, out.size());
	in >> d;
	QCOMPARE(value, d);
	QVERIFY(in.isValid());
	QVERIFY(in.left() == 0);
}

void TestPacketDataStream::undersize() {
	QByteArray qba(32, 'Z');
	char buff[256];

	for (unsigned int i = 0; i < 32; i++) {
		PacketDataStream out(buff, i);
		out << qba;
		QCOMPARE(33 - i, out.undersize());
		QVERIFY(!out.isValid());
		QVERIFY(out.left() == 0);
	}
	PacketDataStream out(buff, 33);
	out << qba;
	QCOMPARE(out.undersize(), 0U);
	QVERIFY(out.isValid());
	QVERIFY(out.left() == 0);
}

void TestPacketDataStream::integer_data() {
	QTest::addColumn< quint64 >("value");
	for (int i = 0; i < 64; i++) {
		quint64 v = 1ULL << i;
		QTest::newRow("Integer") << v;
		QTest::newRow("~Integer") << ~v;
	}
}

void TestPacketDataStream::integer() {
	QFETCH(quint64, value);

	char buff[256];
	quint64 v;
	PacketDataStream out(buff, 256);
	out << value;
	PacketDataStream in(buff, out.size());
	in >> v;

	QCOMPARE(value, v);
	QVERIFY(in.isValid());
	QVERIFY(in.left() == 0);
}

void TestPacketDataStream::string_data() {
	QTest::addColumn< QString >("string");
	QTest::newRow("Empty") << QString("");
	QTest::newRow("Null") << QString();
	QTest::newRow("Bærtur") << QString("Bærtur");
}

void TestPacketDataStream::string() {
	QFETCH(QString, string);

	char buff[256];
	QString v;

	PacketDataStream out(buff, 256);
	out << string;
	PacketDataStream in(buff, out.size());
	in >> v;
	QCOMPARE(string, v);
	QVERIFY(in.isValid());
	QVERIFY(in.left() == 0);
}

void TestPacketDataStream::space() {
	char buff[256];
	for (int i = 0; i < 256; i++)
		buff[i] = 0x55;

	PacketDataStream out(buff, 1);

	char val = -2;

	out << val;
	QVERIFY(out.isValid());
	QVERIFY(out.size() == 1);
	QVERIFY(out.left() == 0);
	QVERIFY(buff[1] == 0x55);

	out << val;

	QVERIFY(!out.isValid());
	QVERIFY(out.size() == 1);
	QVERIFY(out.left() == 0);
	QVERIFY(buff[1] == 0x55);

	PacketDataStream in(buff, 1);

	char v;

	in >> v;
	QCOMPARE(v, val);
	QVERIFY(in.isValid());
	QVERIFY(in.size() == 1);
	QVERIFY(in.left() == 0);

	in >> v;
	QVERIFY(!in.isValid());
	QVERIFY(in.size() == 1);
	QVERIFY(in.left() == 0);
}

QTEST_MAIN(TestPacketDataStream)
#include "TestPacketDataStream.moc"
