// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "murmur_pch.h"

#include "Timer.h"

Timer::Timer(bool start) {
	uiStart = start ? now() : 0;
}

quint64 Timer::elapsed() const {
	Q_ASSERT(uiStart != 0);
	return now() - uiStart;
}

bool Timer::isElapsed(quint64 us) {
	Q_ASSERT(uiStart != 0);
	if (elapsed() > us) {
		uiStart += us;
		return true;
	}
	return false;
}

quint64 Timer::restart() {
	quint64 n = now();
	quint64 e = n - uiStart;
	uiStart = n;
	return e;
}

bool Timer::isStarted() const {
	return uiStart != 0;
}

bool Timer::operator<(const Timer &other) const {
	return uiStart > other.uiStart;
}

bool Timer::operator>(const Timer &other) const {
	return uiStart < other.uiStart;
}

#if defined(Q_OS_WIN)
#include <windows.h>

quint64 Timer::now() {
	static double scale = 0;

	if (scale == 0) {
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		scale = 1000000. / freq.QuadPart;
	}

	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	quint64 e = li.QuadPart;

	return static_cast<quint64>(e * scale);
}
#elif defined(Q_OS_UNIX)
#include <sys/time.h>
quint64 Timer::now() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	quint64 e= tv.tv_sec * 1000000LL;
	e += tv.tv_usec;
	return e;
}
#else
quint64 Timer::now() {
	static QTime ticker;
	quint64 elapsed = ticker.elapsed();
	return elapsed * 1000LL;
}
#endif
