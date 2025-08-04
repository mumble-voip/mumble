// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Timer.h"

#include <cassert>
#include <chrono>
#include <compare>

Timer::Timer(bool start)
	: m_start(start ? std::chrono::steady_clock::now() : std::chrono::time_point< std::chrono::steady_clock >{}) {
}

bool Timer::isElapsed(std::chrono::microseconds duration) {
	assert(isStarted());

	if (elapsed() > duration) {
		m_start += duration;

		return true;
	}

	return false;
}

std::chrono::microseconds Timer::restart() {
	std::chrono::microseconds elapsed = isStarted() ? this->elapsed() : std::chrono::microseconds{ 0 };

	m_start = std::chrono::steady_clock::now();

	return elapsed;
}

bool Timer::isStarted() const {
	return m_start != std::chrono::time_point< std::chrono::steady_clock >{};
}

std::strong_ordering Timer::operator<=>(const Timer &other) const {
	// Note that the comparison of the timer, which is meant to be based on duration, has the inverse ordering than the
	// comparison of the starting time
	return other.m_start <=> m_start;
}
