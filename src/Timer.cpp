// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Timer.h"
#include <chrono>

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

quint64 Timer::now() {
    return static_cast<quint64>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
}
