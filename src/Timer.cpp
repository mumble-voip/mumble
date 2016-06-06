// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "murmur_pch.h"

// Ensure boost_system is header only.
#define BOOST_ERROR_CODE_HEADER_ONLY
// Ensure boost_chrono is header only.
#define BOOST_CHRONO_DONT_PROVIDE_HYBRID_ERROR_HANDLING
#define BOOST_CHRONO_HEADER_ONLY

#include <boost/chrono.hpp>

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

quint64 Timer::now() {
	using namespace boost::chrono;
	time_point<steady_clock> now = steady_clock::now();
	time_point<steady_clock>::duration epochDuration = now.time_since_epoch();
	microseconds epochDurationUsec = duration_cast<microseconds>(epochDuration);
	return static_cast<quint64>(epochDurationUsec.count());
}
