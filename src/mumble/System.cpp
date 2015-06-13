/* Copyright (C) 2011, Stefan Hacker <dd0t@users.sourceforge.net>

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

#include "System.h"

#if defined(Q_OS_LINUX)
#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>
#elif defined(Q_OS_WIN)
#include "windows.h"
#endif

#include <limits>

unsigned int System::getIdleSeconds() {
#if defined(Q_OS_WIN)
	LASTINPUTINFO info;
	info.cbSize = sizeof(LASTINPUTINFO);
	if (GetLastInputInfo(&info)) {
		return static_cast<unsigned int>((GetTickCount() - info.dwTime) / 1000);
	}
#elif defined(Q_OS_LINUX)
	Display *display;
	int event_base, error_base;
	XScreenSaverInfo info;
	float seconds;

	display = XOpenDisplay("");

	if (XScreenSaverQueryExtension(display, &event_base, &error_base)) {
		XScreenSaverQueryInfo(display, DefaultRootWindow(display), &info);
		return static_cast<unsigned int>(info.idle / 1000);
	}
#endif
	return std::numeric_limits<unsigned int>::min();
}
