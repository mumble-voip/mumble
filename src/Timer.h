// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_TIMER_H_
#define MUMBLE_TIMER_H_

#include <QtCore/QtGlobal>

// All timer resolutions are in microseconds.

class Timer {
protected:
	quint64 uiStart;
	static quint64 now();

public:
	Timer(bool start = true);
	bool isElapsed(quint64 us);
	quint64 elapsed() const;
	quint64 restart();
	bool isStarted() const;

	/**
	 * Compares the elapsed time, not the start time
	 */
	bool operator<(const Timer &other) const;

	/**
	 * Compares the elapsed time, not the start time
	 */
	bool operator>(const Timer &other) const;
};

#endif
