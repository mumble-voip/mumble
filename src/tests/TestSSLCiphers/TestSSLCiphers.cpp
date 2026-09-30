// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "SSL.h"

class TestSSLCiphers : public QObject {
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();
	void acceptsTLS13CipherSuites();
	void rejectsEmptyCipherStrings();
	void omitsUnrequestedTLS13CipherSuites();
};

void TestSSLCiphers::initTestCase() {
	MumbleSSL::initialize();
}

void TestSSLCiphers::cleanupTestCase() {
	MumbleSSL::destroy();
}

void TestSSLCiphers::acceptsTLS13CipherSuites() {
	const QString tls13Cipher = QLatin1String("TLS_AES_256_GCM_SHA384");
	if (QSslCipher(tls13Cipher).isNull()) {
		QSKIP("TLS 1.3 cipher suites are not supported by this Qt SSL backend");
	}

	const QList< QSslCipher > ciphers = MumbleSSL::ciphersFromOpenSSLCipherString(tls13Cipher);

	QCOMPARE(ciphers.size(), 1);
	QCOMPARE(ciphers.constFirst().name(), tls13Cipher);
}

void TestSSLCiphers::rejectsEmptyCipherStrings() {
	QVERIFY(MumbleSSL::ciphersFromOpenSSLCipherString(QString()).isEmpty());
	QVERIFY(MumbleSSL::ciphersFromOpenSSLCipherString(QLatin1String("::")).isEmpty());
}

void TestSSLCiphers::omitsUnrequestedTLS13CipherSuites() {
	const QList< QSslCipher > ciphers =
		MumbleSSL::ciphersFromOpenSSLCipherString(QLatin1String("DHE-RSA-AES256-SHA:DHE-RSA-AES128-SHA"));

	QVERIFY(!ciphers.isEmpty());
	for (const QSslCipher &cipher : ciphers) {
		QVERIFY(!MumbleSSL::isTLS13CipherSuiteName(cipher.name()));
	}
}

QTEST_MAIN(TestSSLCiphers)
#include "TestSSLCiphers.moc"
