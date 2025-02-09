// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "SSL.h"

#include "crypto/CryptographicRandom.h"

#include <limits>
#include <stdint.h>
#include <stdlib.h>

class TestCryptographicRandom : public QObject {
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();
	void fillBuffer();
	void uint32();
	void uniform();
};

void TestCryptographicRandom::initTestCase() {
	MumbleSSL::initialize();
}

void TestCryptographicRandom::cleanupTestCase() {
	MumbleSSL::destroy();
}

// Verify the entropy of the data returned by the random source
// by zlib compressing it and ensuring the compressed size is at
// least 99% of the size of the input data.
//
// The inspiration for this check is from the rand_test.go file
// for Go standard library's crypt/rand package:
// https://golang.org/src/crypto/rand/rand_test.go
bool verifyEntropy(unsigned char *buf, int len) {
	QByteArray flated = qCompress(buf, len);
	return (flated.size() >= ((len * 99) / 100));
}

void TestCryptographicRandom::fillBuffer() {
	const int buflen = 1000000;

	for (int i = 0; i < 10; i++) {
		unsigned char *buf = reinterpret_cast< unsigned char * >(calloc(buflen, 1));
		CryptographicRandom::fillBuffer(buf, buflen);
		QVERIFY(verifyEntropy(buf, buflen));
		free(buf);
	}
}

void TestCryptographicRandom::uint32() {
	const int buflen = 1000000;

	for (int i = 0; i < 10; i++) {
		unsigned char *buf = reinterpret_cast< unsigned char * >(calloc(buflen, 1));
		unsigned int niter = buflen / sizeof(uint32_t);
		for (unsigned int j = 0; j < niter; j++) {
			unsigned int off = static_cast< unsigned int >(j * sizeof(uint32_t));
			uint32_t val     = CryptographicRandom::uint32();
			buf[off]         = static_cast< unsigned char >(val & 0xff);
			buf[off + 1]     = static_cast< unsigned char >((val >> 8) & 0xff);
			buf[off + 2]     = static_cast< unsigned char >((val >> 16) & 0xff);
			buf[off + 3]     = static_cast< unsigned char >((val >> 24) & 0xff);
		}
		QVERIFY(verifyEntropy(buf, buflen));
		free(buf);
	}
}

void TestCryptographicRandom::uniform() {
	for (uint32_t upperBound = 0; upperBound < 10000; upperBound++) {
		uint32_t val = CryptographicRandom::uniform(upperBound);
		QVERIFY(val <= upperBound);
	}
}


QTEST_MAIN(TestCryptographicRandom)
#include "TestCryptographicRandom.moc"
