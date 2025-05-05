// Copyright The Mumble Developers. All rights reserved.
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
	void order();
};

// This tests that the timer implemented by the Timer
// class is a high resolution timer. In this case, we
// check that the output of the elapsed() method changes
// at least every 100 microseconds.
void TestTimer::resolution() {
	Timer t;

	t.restart();

	quint64 nchanges    = 0;
	quint64 lastElapsed = 0;
	quint64 curElapsed  = 0;

	do {
		curElapsed = t.elapsed();
		if (curElapsed != lastElapsed) {
			nchanges++;
		}
		lastElapsed = curElapsed;
	} while (curElapsed < 150000); // 150 ms

	quint64 totalElapsed  = t.elapsed();
	double usecsPerChange = static_cast< double >(totalElapsed) / static_cast< double >(nchanges);

	qWarning("Total elapsed time: %llu microseconds", static_cast< unsigned long long >(totalElapsed));
	qWarning("Number of elapsed changes: %llu", static_cast< unsigned long long >(nchanges));
	qWarning("Resolution: %.2f microseconds", usecsPerChange);

	if (usecsPerChange >= 100.0f) {
		QFAIL("Insufficient timer resolution. Got >= 100 usec, expected < 100 usec resolution...");
	}
}

void TestTimer::order() {
	Timer a;

	while (a.elapsed() == 0) {
	};

	Timer b;

	QVERIFY(a > b);
	QVERIFY(!(b > a));
	QVERIFY(!(a < b));
	QVERIFY(b < a);
}

QTEST_MAIN(TestTimer)
#include "TestTimer.moc"
