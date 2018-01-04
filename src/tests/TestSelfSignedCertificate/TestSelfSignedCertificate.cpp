// Copyright 2005-2018 The Mumble Developers. All rights reserved.
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
	
	bool ok = SelfSignedCertificate::generateMumbleCertificate(QLatin1String("Test"), QLatin1String("test@test.test"), cert, key);
	QCOMPARE(ok, true);
	QCOMPARE(cert.isNull(), false);
	QCOMPARE(cert.isNull(), false);

	ok = SelfSignedCertificate::generateMumbleCertificate(QString(), QString(), cert, key);
	QCOMPARE(ok, false);
	QCOMPARE(cert.isNull(), true);
	QCOMPARE(cert.isNull(), true);
}

void TestSelfSignedCertificate::exerciseServerCert() {
	QSslCertificate cert;
	QSslKey key;
	
	bool ok = SelfSignedCertificate::generateMurmurV2Certificate(cert, key);
	QCOMPARE(ok, true);
	QCOMPARE(cert.isNull(), false);
	QCOMPARE(cert.isNull(), false);
}

QTEST_MAIN(TestSelfSignedCertificate)
#include "TestSelfSignedCertificate.moc"
