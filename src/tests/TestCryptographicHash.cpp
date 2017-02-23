// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>
#include <QLatin1String>

#include "CryptographicHash.h"

class TestCryptographicHash : public QObject {
		Q_OBJECT
	private slots:
		void sha1_data();
		void sha1();

		void sha256_data();
		void sha256();
};

void TestCryptographicHash::sha1_data() {
	QTest::addColumn<QString>("input");
	QTest::addColumn<QString>("expectedOutput");

	// First 4 SHA1 test vectors from http://www.di-mgt.com.au/sha_testvectors.html
	// ...The rest are a bit too excessive to put in a table-based test.
	QTest::newRow("1") << QString(QLatin1String("abc")) << QString(QLatin1String("a9993e36 4706816a ba3e2571 7850c26c 9cd0d89d"));
	QTest::newRow("2") << QString(QLatin1String("")) << QString(QLatin1String("da39a3ee 5e6b4b0d 3255bfef 95601890 afd80709"));
	QTest::newRow("3") << QString(QLatin1String("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")) << QString(QLatin1String("84983e44 1c3bd26e baae4aa1 f95129e5 e54670f1"));
	QTest::newRow("4") << QString(QLatin1String("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu")) << QString(QLatin1String("a49b2446 a02c645b f419f995 b6709125 3a04a259"));
}

void TestCryptographicHash::sha1() {
	QFETCH(QString, input);
	QFETCH(QString, expectedOutput);

	QByteArray outBuf = CryptographicHash::hash(input.toLatin1(), CryptographicHash::Sha1);
	QByteArray hexBuf = outBuf.toHex();
	QString output = QString::fromLatin1(hexBuf);

	// Strip whitespace from the expected form in the table.
	expectedOutput.remove(QChar(' '));

	QCOMPARE(output, expectedOutput);
}

void TestCryptographicHash::sha256_data() {
	QTest::addColumn<QString>("input");
	QTest::addColumn<QString>("expectedOutput");

	// First 4 SHA256 test vectors from http://www.di-mgt.com.au/sha_testvectors.html
	// ...The rest are a bit too excessive to put in a table-based test.
	QTest::newRow("1") << QString(QLatin1String("abc")) << QString(QLatin1String("ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad"));
	QTest::newRow("2") << QString(QLatin1String("")) << QString(QLatin1String("e3b0c442 98fc1c14 9afbf4c8 996fb924 27ae41e4 649b934c a495991b 7852b855"));
	QTest::newRow("3") << QString(QLatin1String("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq")) << QString(QLatin1String("248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1"));
	QTest::newRow("4") << QString(QLatin1String("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu")) << QString(QLatin1String("cf5b16a7 78af8380 036ce59e 7b049237 0b249b11 e8f07a51 afac4503 7afee9d1"));
}

void TestCryptographicHash::sha256() {
	QFETCH(QString, input);
	QFETCH(QString, expectedOutput);

	QByteArray outBuf = CryptographicHash::hash(input.toLatin1(), CryptographicHash::Sha256);
	QByteArray hexBuf = outBuf.toHex();
	QString output = QString::fromLatin1(hexBuf);

	// Strip whitespace from the expected form in the table.
	expectedOutput.remove(QChar(' '));

	QCOMPARE(output, expectedOutput);
}

QTEST_MAIN(TestCryptographicHash)
#include "TestCryptographicHash.moc"
