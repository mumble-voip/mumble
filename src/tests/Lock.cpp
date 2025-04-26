// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/**
 * Benchmark of different locking mechanisms; QMutex, PosixMutex, Silly
 * (int-flag).
 */

#include <QtCore>
#include <QtNetwork>

#include "Timer.h"

// It's important this is high enough the process doesn't complete in
// one timeslice.
#define ITER 100000000

typedef QPair< quint64, int > tr;

class SillyLock {
public:
	int counter;
	SillyLock();
	void lock();
	void unlock();
};

SillyLock::SillyLock() {
	counter = 0;
}

void SillyLock::lock() {
	if (counter == 0)
		counter = 1;
	else
		qFatal("Recursive lock");
}

void SillyLock::unlock() {
	if (counter == 1)
		counter = 0;
	else
		qFatal("Nonlock-unlock");
}

class PosixLock {
public:
	pthread_mutex_t m;
	PosixLock();
	void lock();
	void unlock();
};

PosixLock::PosixLock() {
	pthread_mutex_init(&m, nullptr);
}

void PosixLock::lock() {
	pthread_mutex_lock(&m);
}

void PosixLock::unlock() {
	pthread_mutex_unlock(&m);
}

template< class T > class SpeedTest {
public:
	T &lock;

	SpeedTest(T &l) : lock(l) {}

	quint64 test() {
		Timer t;
		t.restart();
		for (int i = 0; i < ITER; i++) {
			lock.lock();
			lock.unlock();
		}
		return t.elapsed();
	}
};

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);

	QMutex qm;
	QMutex qmr(QMutex::Recursive);
	SillyLock sl;
	PosixLock pl;

	SpeedTest< QMutex > stqm(qm);
	SpeedTest< QMutex > stqmr(qmr);
	SpeedTest< SillyLock > stsl(sl);
	SpeedTest< PosixLock > stpl(pl);

	quint64 elapsed;

	elapsed = stsl.test();
	qWarning("SillyLock: %8lld", elapsed);

	elapsed = stqm.test();
	qWarning("QMutex   : %8lld", elapsed);

	elapsed = stqmr.test();
	qWarning("QMutexR  : %8lld", elapsed);

	elapsed = stpl.test();
	qWarning("PosixLock: %8lld", elapsed);
}

// #include "Lock.moc"

#undef ITER
