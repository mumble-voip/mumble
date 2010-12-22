/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <QtCore>
#include "Timer.h"

Timer::Timer() {
	uiStart = now();
}

quint64 Timer::elapsed() const {
	return now() - uiStart;
}

bool Timer::isElapsed(quint64 us) {
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
