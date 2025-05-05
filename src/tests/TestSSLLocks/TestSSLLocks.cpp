// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "SSL.h"

#include <openssl/rand.h>

/// SSLRacer is a thread that runs operations on OpenSSL's
/// RAND infrastructure, in an attempt to crash/segfault
/// the test process.
///
/// The thread can either query OpenSSL for random bytes, or
/// seed the RAND system. This is controlled by the |seed|
/// parameter to the constructor.
class SSLRacer : public QThread {
public:
	bool m_seed;
	QAtomicInt *m_running;

	SSLRacer(QAtomicInt *running, bool seed) : m_seed(seed), m_running(running) {}

	void run() {
		unsigned char buf[64];
		while (m_running->loadRelaxed() == 1) {
			for (int i = 0; i < 1024; i++) {
				if (m_seed) {
					RAND_seed(buf, sizeof(buf));
				} else {
					RAND_bytes(buf, sizeof(buf));
				}
			}
		}
	}
};

class TestSSLLocks : public QObject {
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();
	void stress();
};

void TestSSLLocks::initTestCase() {
	// For OpenSSL < 1.1, if you comment out this line,
	// you'll be running OpenSSL without locking callbacks
	// enabled. That'll make this test fail by crashing/segfaulting.
	MumbleSSL::initialize();
}

void TestSSLLocks::cleanupTestCase() {
	MumbleSSL::destroy();
}

/// Stress test that our locking callbacks for OpenSSL are set up and
/// working correctly.
///
/// We do this by spawning a bunch of SSLStresser threads. Some of the
/// threads will try to read random bytes in a tight loop. Other threads
/// will, at the same time, try to add/seed random bytes to the OpenSSL
/// RAND system.
///
/// The idea is that without proper locking, the data races we're causing
/// should quite quickly cause the process to crash.
void TestSSLLocks::stress() {
	std::vector< SSLRacer * > racers;
	QAtomicInt running(1);

	// Spawn 24 threads in total. 12 readers, and 12 writers.
	// Don't be too careful about cleaning up the threads. We'll either
	// pass or crash, so the threads will be cleaned up either way.
	int nthreads = 24;
	for (int i = 0; i < nthreads; i++) {
		bool seeder     = i % 2;
		SSLRacer *racer = new SSLRacer(&running, seeder);
		racers.push_back(racer);
		racer->start();
	}

	// Wait 2 seconds for a crash/segfault.
	// If we don't crash within 2 seconds, we expect
	// that our locking implementation works.
	QTest::qSleep(2000);

	// Signal to the racers that they should stop.
	running.fetchAndStoreOrdered(0);

	// Wait for all racers to complete.
	for (size_t i = 0; i < racers.size(); i++) {
		SSLRacer *racer = racers.at(i);
		racer->wait();
		delete racer;
	}
}

QTEST_MAIN(TestSSLLocks)
#include "TestSSLLocks.moc"
