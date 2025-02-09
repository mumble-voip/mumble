// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "SSL.h"

#include "SelfSignedCertificate.h"

class TestSelfSignedCertificate : public QObject {
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();
	void exerciseClientCert();
	void exerciseServerCert();
};

void TestSelfSignedCertificate::initTestCase() {
	MumbleSSL::initialize();
}

void TestSelfSignedCertificate::cleanupTestCase() {
	MumbleSSL::destroy();
}

void TestSelfSignedCertificate::exerciseClientCert() {
	QSslCertificate cert;
	QSslKey key;

	bool ok = SelfSignedCertificate::generateMumbleCertificate(QLatin1String("Test"), QLatin1String("test@test.test"),
															   cert, key);
	QCOMPARE(ok, true);
	QCOMPARE(cert.isNull(), false);
	QCOMPARE(key.isNull(), false);

	// Test that certificates are auto-generated correctly
	ok = SelfSignedCertificate::generateMumbleCertificate(QString(), QString(), cert, key);
	QCOMPARE(ok, true);
	QCOMPARE(cert.isNull(), false);
	QCOMPARE(key.isNull(), false);

	// Test that users can create certificates without an email
	// address set.
	ok = SelfSignedCertificate::generateMumbleCertificate(QLatin1String("John Doe"), QString(), cert, key);
	QCOMPARE(ok, true);
	QCOMPARE(cert.isNull(), false);
	QCOMPARE(key.isNull(), false);

	// Test that it's possible to create a client certificate with
	// both a name and an email.
	ok = SelfSignedCertificate::generateMumbleCertificate(QLatin1String("John Doe"), QLatin1String("john@doe.family"),
														  cert, key);
	QCOMPARE(ok, true);
	QCOMPARE(cert.isNull(), false);
	QCOMPARE(key.isNull(), false);
}

void TestSelfSignedCertificate::exerciseServerCert() {
	QSslCertificate cert;
	QSslKey key;

	bool ok = SelfSignedCertificate::generateMurmurV2Certificate(cert, key);
	QCOMPARE(ok, true);
	QCOMPARE(cert.isNull(), false);
	QCOMPARE(key.isNull(), false);
}

QTEST_MAIN(TestSelfSignedCertificate)
#include "TestSelfSignedCertificate.moc"
