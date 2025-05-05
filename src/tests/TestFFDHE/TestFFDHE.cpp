// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
#	include <QtNetwork/QSslDiffieHellmanParameters>
#endif

#include "FFDHE.h"
#include "SSL.h"

class TestFFDHE : public QObject {
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
	void exercise_data();
	void exercise();
	void namedGroupsMethod();
#endif
};

void TestFFDHE::initTestCase() {
	MumbleSSL::initialize();
}

void TestFFDHE::cleanupTestCase() {
	MumbleSSL::destroy();
}

#if defined(USE_QSSLDIFFIEHELLMANPARAMETERS)
void TestFFDHE::exercise_data() {
	QTest::addColumn< QString >("name");
	QTest::addColumn< bool >("expectedToWork");

	QTest::newRow("ffdhe2048") << QString(QLatin1String("ffdhe2048")) << true;
	QTest::newRow("ffdhe3072") << QString(QLatin1String("ffdhe3072")) << true;
	QTest::newRow("ffdhe4096") << QString(QLatin1String("ffdhe4096")) << true;
	QTest::newRow("ffdhe6144") << QString(QLatin1String("ffdhe6144")) << true;
	QTest::newRow("ffdhe8192") << QString(QLatin1String("ffdhe8192")) << true;

	QTest::newRow("ffdhe2048_upper") << QString(QLatin1String("FFDHE2048")) << true;
	QTest::newRow("ffdhe2048_random") << QString(QLatin1String("fFdHe2048")) << true;
	QTest::newRow("noname") << QString(QLatin1String("")) << false;
	QTest::newRow("typo") << QString(QLatin1String("ffdhe2047")) << false;
	QTest::newRow("trailingspace") << QString(QLatin1String("ffdhe2048 ")) << false;
}

static bool tryFFDHELookupByName(QString name) {
	bool ok = true;

	QByteArray pem = FFDHE::PEMForNamedGroup(name);
	if (pem.isEmpty() || pem.isNull()) {
		ok = false;
	}
	if (ok) {
		QSslDiffieHellmanParameters dh = QSslDiffieHellmanParameters::fromEncoded(pem, QSsl::Pem);
		ok                             = dh.isValid();
		if (!ok) {
			qWarning("QSslDiffieHellman error: %s", qPrintable(dh.errorString()));
		}
	}

	return ok;
}

void TestFFDHE::exercise() {
	QFETCH(QString, name);
	QFETCH(bool, expectedToWork);
	QCOMPARE(tryFFDHELookupByName(name), expectedToWork);
}

void TestFFDHE::namedGroupsMethod() {
	foreach (QString name, FFDHE::NamedGroups()) { QCOMPARE(tryFFDHELookupByName(name), true); }
}
#endif

QTEST_MAIN(TestFFDHE)
#include "TestFFDHE.moc"
