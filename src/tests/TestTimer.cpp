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

	QVERIFY(abs((long)(ttime - b.elapsed())) < 100);
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
