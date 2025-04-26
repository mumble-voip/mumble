// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <boost/version.hpp>

// Not all Boost versions can use a header-only
// boost_system without build failures.
//
// See mumble-voip/mumble#2366.
//
// This was fixed in Boost 1.56.0.
//
// See:
//  https://github.com/boostorg/system/blob/boost-1.56.0/include/boost/system/error_code.hpp#L514-L516
// vs.
//  https://github.com/boostorg/system/blob/boost-1.55.0/include/boost/system/error_code.hpp#L515-L517
#if BOOST_VERSION >= 105600 && !defined(__MINGW32__)
#	define USE_BOOST_CHRONO
#endif

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
	uiStart   = n;
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

#ifdef USE_BOOST_CHRONO
// Ensure boost_system is header only.
#	define BOOST_ERROR_CODE_HEADER_ONLY
// Ensure boost_chrono is header only.
#	define BOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
#	define BOOST_CHRONO_HEADER_ONLY

#	include <boost/chrono.hpp>

quint64 Timer::now() {
	using namespace boost::chrono;
	time_point< steady_clock > now                     = steady_clock::now();
	time_point< steady_clock >::duration epochDuration = now.time_since_epoch();
	microseconds epochDurationUsec                     = duration_cast< microseconds >(epochDuration);
	return static_cast< quint64 >(epochDurationUsec.count());
}
#elif defined(Q_OS_WIN)
#	include "win.h"

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

	return static_cast< quint64 >(e * scale);
}
#elif defined(Q_OS_UNIX)
#	include <errno.h>
#	include <string.h>
#	include <time.h>
#	include <unistd.h>
#	if defined(_POSIX_TIMERS) && defined(_POSIX_MONOTONIC_CLOCK)
quint64 Timer::now() {
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
		qFatal("Timer: clock_gettime() failed: (%i) %s", errno, strerror(errno));
	}
	quint64 e = ts.tv_sec * 1000000LL;
	e += ts.tv_nsec / 1000LL;
	return e;
}
#	else
quint64 Timer::now() {
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	quint64 e = tv.tv_sec * 1000000LL;
	e += tv.tv_usec;
	return e;
}
#	endif
#else
quint64 Timer::now() {
	static QTime ticker;
	quint64 elapsed = ticker.elapsed();
	return elapsed * 1000LL;
}
#endif
