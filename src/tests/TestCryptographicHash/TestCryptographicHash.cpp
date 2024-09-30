// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QLatin1String>
#include <QtCore>
#include <QtTest>

#include "SSL.h"

#include "crypto/CryptographicHash.h"

class TestCryptographicHash : public QObject {
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();

	void sha1_data();
	void sha1();

	void sha256_data();
	void sha256();

	void staticHashVsObject();
	void multipleResultCalls();
	void addDataAfterResult();
};

void TestCryptographicHash::initTestCase() {
	MumbleSSL::initialize();
}

void TestCryptographicHash::cleanupTestCase() {
	MumbleSSL::destroy();
}

/// normalizeHash removes all whitespace from the hex-encoded hash string.
static QString normalizeHash(QString str) {
	str.replace(QLatin1String(" "), QLatin1String(""));
	str.replace(QLatin1String("\t"), QLatin1String(""));
	str.replace(QLatin1String("\r"), QLatin1String(""));
	str.replace(QLatin1String("\n"), QLatin1String(""));
	return str;
}

/// normalizeHash removes all whitespace from the hex-encoded hash byte array.
static QByteArray normalizeHash(QByteArray buf) {
	buf.replace(" ", "");
	buf.replace("\t", "");
	buf.replace("\r", "");
	buf.replace("\n", "");
	return buf;
}

void TestCryptographicHash::sha1_data() {
	QTest::addColumn< QString >("input");
	QTest::addColumn< QString >("expectedOutput");

	// First 4 SHA1 test vectors from http://www.di-mgt.com.au/sha_testvectors.html
	// ...The rest are a bit too excessive to put in a table-based test.
	QTest::newRow("1") << QString(QLatin1String("abc"))
					   << normalizeHash(QLatin1String("a9993e36 4706816a ba3e2571 7850c26c 9cd0d89d"));
	QTest::newRow("2") << QString(QLatin1String(""))
					   << normalizeHash(QLatin1String("da39a3ee 5e6b4b0d 3255bfef 95601890 afd80709"));
	QTest::newRow("3") << QString(QLatin1String("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"))
					   << normalizeHash(QLatin1String("84983e44 1c3bd26e baae4aa1 f95129e5 e54670f1"));
	QTest::newRow("4") << QString(QLatin1String("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmn"
												"opjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"))
					   << normalizeHash(QLatin1String("a49b2446 a02c645b f419f995 b6709125 3a04a259"));
}

void TestCryptographicHash::sha1() {
	QFETCH(QString, input);
	QFETCH(QString, expectedOutput);

	QByteArray outBuf = CryptographicHash::hash(input.toLatin1(), CryptographicHash::Sha1);
	QByteArray hexBuf = outBuf.toHex();
	QString output    = QString::fromLatin1(hexBuf);

	QCOMPARE(output, expectedOutput);
}

void TestCryptographicHash::sha256_data() {
	QTest::addColumn< QString >("input");
	QTest::addColumn< QString >("expectedOutput");

	// First 4 SHA256 test vectors from http://www.di-mgt.com.au/sha_testvectors.html
	// ...The rest are a bit too excessive to put in a table-based test.
	QTest::newRow("1") << QString(QLatin1String("abc"))
					   << normalizeHash(
							  QLatin1String("ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad"));
	QTest::newRow("2") << QString(QLatin1String(
		"")) << normalizeHash(QLatin1String("e3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855"));
	QTest::newRow("3") << QString(QLatin1String("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"))
					   << normalizeHash(
							  QLatin1String("248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1"));
	QTest::newRow("4") << QString(QLatin1String("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmn"
												"opjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"))
					   << normalizeHash(
							  QLatin1String("cf5b16a7 78af8380 036ce59e 7b049237 0b249b11 e8f07a51 afac4503 7afee9d1"));
}

void TestCryptographicHash::sha256() {
	QFETCH(QString, input);
	QFETCH(QString, expectedOutput);

	QByteArray outBuf = CryptographicHash::hash(input.toLatin1(), CryptographicHash::Sha256);

	QVERIFY(!outBuf.isEmpty());

	QByteArray hexBuf = outBuf.toHex();
	QString output    = QString::fromLatin1(hexBuf);

	QCOMPARE(output, expectedOutput);
}

void TestCryptographicHash::staticHashVsObject() {
	CryptographicHash h(CryptographicHash::Sha256);
	h.addData(QByteArray("abc"));
	QByteArray objectResult = h.result();

	QByteArray staticResult = CryptographicHash::hash(QByteArray("abc"), CryptographicHash::Sha256);

	QVERIFY(!objectResult.isEmpty());
	QVERIFY(!staticResult.isEmpty());

	QCOMPARE(objectResult, staticResult);
}

void TestCryptographicHash::multipleResultCalls() {
	CryptographicHash h(CryptographicHash::Sha256);
	h.addData(QByteArray("abc"));

	QByteArray result1 = h.result();
	QByteArray result2 = h.result();
	QByteArray result3 = h.result();

	QVERIFY(!result1.isEmpty());
	QVERIFY(!result2.isEmpty());
	QVERIFY(!result3.isEmpty());

	QCOMPARE(result1.toHex(),
			 normalizeHash(QByteArray("ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad")));
	QCOMPARE(result1, result2);
	QCOMPARE(result1, result3);
}

void TestCryptographicHash::addDataAfterResult() {
	CryptographicHash h(CryptographicHash::Sha256);
	h.addData(QByteArray("abc"));

	QByteArray result1 = h.result();

	QCOMPARE(result1.toHex(),
			 normalizeHash(QByteArray("ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad")));

	// Adding data after getting the result of a
	// CryptographicHash is an error.
	h.addData(QByteArray("abc"));
	QByteArray result2 = h.result();
	QVERIFY(result2.isEmpty());
}

QTEST_MAIN(TestCryptographicHash)
#include "TestCryptographicHash.moc"
