// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TIMER_H_
#define MUMBLE_TIMER_H_

#include <cassert>
#include <chrono>
#include <compare>

class Timer {
public:
	Timer(bool start = true);

	/**
	 * Checks whether the given time has passed already and if this is the case, moves the internal start time by the
	 * respective duration. This means that the next call to isElapsed will then ask if an additional time period has
	 * been passed.
	 */
	bool isElapsed(std::chrono::microseconds duration);

	/**
	 * @returns The time elapsed since the timer was started
	 */
	template< typename DurationType = std::chrono::microseconds > DurationType elapsed() const {
		assert(isStarted());
		return std::chrono::duration_cast< DurationType >(std::chrono::steady_clock::now() - m_start);
	}

	/**
	 * Restarts the timer
	 * @returns The time elapsed since the timer was started before this restart
	 */
	std::chrono::microseconds restart();

	/**
	 * @returns Whether the timer has been started
	 */
	bool isStarted() const;

	std::strong_ordering operator<=>(const Timer &other) const;

protected:
	std::chrono::time_point< std::chrono::steady_clock > m_start;
};

#endif
