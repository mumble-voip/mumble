// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/**
 * Benchmark of storage mechanisms; array, vector, Qt collections.
 */

#include <QtCore>
#include <vector>

#include "Timer.h"

using namespace std;

#define SL 10000
#define NE 300

template< class T > class QListSet {
public:
	QList< T > l;
	void insert(const T &i) {
		if (!l.contains(i))
			l.append(i);
	}
	void remove(const T &i) { l.removeAll(i); }
};

int main(int argc, char **argv) {
	Timer t;
	quint64 usins, usiter;
	int sum;

	{
		t.restart();
		QSet< int > s;
		s.reserve(NE);
		for (int i = 0; i < NE; i++)
			s.insert(i);
		usins = t.restart();
		sum   = 0;
		for (int j = 0; j < SL; j++)
			foreach (int v, s)
				sum += v;
		usiter = t.restart();
	}
	qWarning("QSet: %lldus insert, %lldus iterate: %d", usins, usiter, sum);

	{
		t.restart();
		QList< int > s;
		for (int i = 0; i < NE; i++)
			s.append(i);
		usins = t.restart();
		sum   = 0;
		for (int j = 0; j < SL; j++)
			foreach (int v, s)
				sum += v;
		usiter = t.restart();
	}
	qWarning("QList: %lldus insert, %lldus iterate: %d", usins, usiter, sum);

	{
		t.restart();
		QVector< int > s;
		s.reserve(NE);
		for (int i = 0; i < NE; i++)
			s.append(i);
		usins = t.restart();
		sum   = 0;
		for (int j = 0; j < SL; j++)
			foreach (int v, s)
				sum += v;
		usiter = t.restart();
	}
	qWarning("QVector: %lldus insert, %lldus iterate: %d", usins, usiter, sum);

	{
		t.restart();
		vector< int > s;
		for (int i = 0; i < NE; i++)
			s.push_back(i);
		usins  = t.restart();
		sum    = 0;
		int sz = s.size();
		for (int j = 0; j < SL; j++)
			for (int i = 0; i < sz; i++)
				sum += s[i];
		usiter = t.restart();
	}
	qWarning("std::vector: %lldus insert, %lldus iterate: %d", usins, usiter, sum);

	{
		t.restart();
		int s[NE];
		for (int i = 0; i < NE; i++)
			s[i] = i;
		usins = t.restart();
		sum   = 0;
		for (int j = 0; j < SL; j++)
			for (int i = 0; i < NE; i++)
				sum += s[i];
		usiter = t.restart();
	}
	qWarning("c99 alloc: %lldus insert, %lldus iterate: %d", usins, usiter, sum);


	{
		t.restart();
		QListSet< int > s;
		for (int i = 0; i < NE; i++)
			s.insert(i);
		usins = t.restart();
		sum   = 0;
		for (int j = 0; j < SL; j++)
			foreach (int v, s.l)
				sum += v;
		usiter = t.restart();
	}
	qWarning("QListSet: %lldus insert, %lldus iterate: %d", usins, usiter, sum);
}

#undef SL
#undef NE
