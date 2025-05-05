// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Timer.h"
#include <QtCore>
#include <boost/bind.hpp>
#include <boost/signals.hpp>

class Slotter : public QObject {
	Q_OBJECT

public:
	Slotter(QObject *p = nullptr) : QObject(p){};
public Q_SLOTS:
	virtual void slottest(int &res) { res += 1; };
};

class Emitter : public QObject {
	Q_OBJECT

public:
	Slotter *s;
	Emitter(Slotter *s) : QObject(s) { this->s = s; };

	boost::signal< void(int &) > bsig;
Q_SIGNALS:
	void sig(int &);
public Q_SLOTS:
	void emittest(int &val) { Q_EMIT sig(val); };
	void directtest(int &val) { s->slottest(val); };
	void boosttest(int &val) { bsig(val); }
};

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);
	Timer t;

	Slotter *s  = new Slotter();
	Slotter *s2 = new Slotter();
	Emitter *e  = new Emitter(s);

	int v;

	v = 0;
	t.restart();
	for (int i = 0; i < 1000000; ++i)
		s->slottest(v);
	printf("Raw   : %8lld %d\n", t.elapsed(), v);

	v = 0;
	t.restart();
	for (int i = 0; i < 1000000; ++i)
		e->directtest(v);
	printf("Direct: %8lld %d\n", t.elapsed(), v);

	v = 0;
	t.restart();
	for (int i = 0; i < 1000000; ++i)
		e->emittest(v);
	printf("NoSig : %8lld %d\n", t.elapsed(), v);

	QObject::connect(e, SIGNAL(sig(int &)), s, SLOT(slottest(int &)));
	v = 0;
	t.restart();
	for (int i = 0; i < 1000000; ++i)
		e->emittest(v);
	printf("Signal: %8lld %d\n", t.elapsed(), v);

	QObject::connect(e, SIGNAL(sig(int &)), s2, SLOT(slottest(int &)));
	v = 0;
	t.restart();
	for (int i = 0; i < 1000000; ++i)
		e->emittest(v);
	printf("2Slots: %8lld %d\n", t.elapsed(), v);

	v = 0;
	t.restart();
	for (int i = 0; i < 1000000; ++i)
		e->boosttest(v);
	printf("BNoSig: %8lld %d\n", t.elapsed(), v);

	e->bsig.connect(boost::bind(&Slotter::slottest, s, _1));
	v = 0;
	t.restart();
	for (int i = 0; i < 1000000; ++i)
		e->boosttest(v);
	printf("Boost:  %8lld %d\n", t.elapsed(), v);

	e->bsig.connect(boost::bind(&Slotter::slottest, s2, _1));
	v = 0;
	t.restart();
	for (int i = 0; i < 1000000; ++i)
		e->boosttest(v);
	printf("2Boost: %8lld %d\n", t.elapsed(), v);
}

#include "Emit.moc"
