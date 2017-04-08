// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>
#include <QtTest>

#include "Timer.h"

class TestTimer : public QObject {
		Q_OBJECT
	private slots:
		void resolution();
		void accuracy();
		void atomicity();
		void order();
};

static quint64 delta64(quint64 a, quint64 b) {
	if (a > b) {
		return a - b;
	}
	return b - a;
}

void TestTimer::accuracy() {
	QTime a;
	Timer t;

	a.restart();
	t.restart();
	do {
	} while (a.elapsed() < 1000);

	quint64 tElapsedMs = t.elapsed() / 1000ULL;
	quint64 aElapsedMs = a.elapsed();

	quint64 delta = delta64(tElapsedMs, aElapsedMs);

	qWarning("Timer elapsed time: %llu milliseconds", static_cast<unsigned long long>(tElapsedMs));
	qWarning("QTime elapsed time: %llu milliseconds", static_cast<unsigned long long>(aElapsedMs));
	qWarning("Delta: %llu milliseconds", static_cast<unsigned long long >(delta));

	QVERIFY(delta < 10);
}

// This tests that the timer implemented by the Timer
// class is a high resolution timer. In this case, we
// check that the output of the elapsed() method changes
// at least every 5 microseconds.
void TestTimer::resolution() {
	Timer t;

	t.restart();

	quint64 nchanges = 0;
	quint64 lastElapsed = 0;
	quint64 curElapsed  = 0;

	do {
		curElapsed = t.elapsed();
		if (curElapsed != lastElapsed) {
			nchanges++;
		}
		lastElapsed = curElapsed;
	} while (curElapsed < 150000); // 150 ms

	quint64 totalElapsed = t.elapsed();
	double usecsPerChange = static_cast<double>(totalElapsed) / static_cast<double>(nchanges);

	qWarning("Total elapsed time: %llu microseconds", static_cast<unsigned long long>(totalElapsed));
	qWarning("Number of elapsed changes: %llu", static_cast<unsigned long long>(nchanges));
	qWarning("Resolution: %.2f microseconds", usecsPerChange);

	if (usecsPerChange >= 5.0f) {
		QFAIL("Insufficient timer resolution. Got >= 5 usec, expected < 5 usec resolution...");
	}
}

void TestTimer::atomicity() {
	QTime t;
	Timer a, b;

	a = b;

	quint64 ttime = 0;

	t.restart();
	do {
		ttime += a.restart();
	} while (t.elapsed() < 10);

	quint64 elapsed = b.elapsed();

	// Find the delta between the calculated elapsed time (ttime)
	// and the elapsed time according to the Timer class (elapsed).
	quint64 delta = delta64(elapsed, ttime);

	qWarning("Timer class's elapsed time: %llu microseconds", static_cast<unsigned long long>(elapsed));
	qWarning("Calculated elapsed time: %llu microseconds", static_cast<unsigned long long>(elapsed));
	qWarning("Delta: %llu microseconds", static_cast<unsigned long long>(delta));

	QVERIFY(delta < 100);
}

void TestTimer::order() {
	Timer a;

	while (a.elapsed() == 0) {};

	Timer b;

	QVERIFY(a > b);
	QVERIFY(!(b > a));
	QVERIFY(!(a < b));
	QVERIFY(b < a);
}

QTEST_MAIN(TestTimer)
#include "TestTimer.moc"
