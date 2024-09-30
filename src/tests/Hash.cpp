// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <QtCore>

#include "Timer.h"

#define ITER 100000

typedef QPair< quint64, int > tr;

template< class T > class SpeedTest {
public:
	QList< T > list;

	SpeedTest() {}

	void add(T v) { list << v; }

	tr testres(T v) {
		int val = 0;
		Timer t;
		t.restart();
		for (int i = 0; i < ITER; i++) {
			if (list.contains(v))
				val++;
		}
		return tr(t.elapsed(), val);
	}
};

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);

	QSet< int > set;
	SpeedTest< QVariant > stv;
	SpeedTest< int > sti;
	SpeedTest< QList< QVariant > > stl;
	SpeedTest< QVariant > stvl;

	for (int i = 0; i < 300; i++) {
		set.insert(i);

		QVariant v = i;
		QList< QVariant > ql;
		ql << v;
		QVariant vl = QVariant(ql);

		stv.add(v);
		sti.add(i);
		stl.add(ql);
		stvl.add(vl);
	}

	int i      = 397;
	QVariant v = i;
	QList< QVariant > ql;
	ql << v;
	QVariant vl = QVariant(ql);

	Timer tt;
	tr t;
	int val = 0;
	tt.restart();
	for (int i = 0; i < ITER; i++)
		if (set.contains(i))
			val++;
	t.first  = tt.elapsed();
	t.second = val;
	qWarning("set: %llu", t.first);

	t = sti.testres(i);
	qWarning("int: %llu", t.first / ITER);

	t = stv.testres(v);
	qWarning("var: %llu", t.first / ITER);

	t = stl.testres(ql);
	qWarning("lst: %llu", t.first / ITER);

	t = stvl.testres(vl);
	qWarning("vls: %llu", t.first / ITER);
}

#include "Hash.moc"

#undef ITER
