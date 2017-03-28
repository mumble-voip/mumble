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

void TestTimer::accuracy() {
	QTime a;
	Timer t;

	a.restart();
	t.restart();
	do {
	} while (a.elapsed() < 1000);

	QVERIFY(abs((int)(t.elapsed() / 1000ULL - a.elapsed())) < 10);
}

void TestTimer::resolution() {
	QTime a;
	Timer t;

	a.restart();
	t.restart();

	quint64 achange = 0;
	quint64 tchange = 0;

	quint64 aelapsed = 0, telapsed = 0;

	do {
		quint64 ae = a.elapsed();
		quint64 te = t.elapsed();
		if (ae != aelapsed)
			achange++;
		if (te != telapsed)
			tchange++;

		aelapsed = ae;
		telapsed = te;

	} while (achange < 10);

	QVERIFY(tchange > (achange * 100));
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
	quint64 delta = 0;
	if (elapsed > ttime) {
		delta = elapsed - ttime;
	} else if (ttime > elapsed) {
		delta = ttime - elapsed;
	}

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
