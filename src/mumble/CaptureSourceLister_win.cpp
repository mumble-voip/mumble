// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_SCREEN_SHARING

#	include "CaptureSourceLister.h"

#	include "win.h"

static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
	auto *windows = reinterpret_cast< QList< CaptureSource > * >(lParam);

	if (!IsWindowVisible(hwnd))
		return TRUE;
	if (GetWindowTextLengthW(hwnd) == 0)
		return TRUE;
	// Skip tool windows (e.g. system tray popups).
	LONG exStyle = GetWindowLongW(hwnd, GWL_EXSTYLE);
	if (exStyle & WS_EX_TOOLWINDOW)
		return TRUE;

	wchar_t title[512] = {};
	GetWindowTextW(hwnd, title, 512);

	CaptureSource s;
	s.type           = CaptureSource::Type::Window;
	s.nativeWindowId = reinterpret_cast< quintptr >(hwnd);
	s.displayName    = QString::fromWCharArray(title);
	windows->append(s);
	return TRUE;
}

QList< CaptureSource > listCaptureWindows() {
	QList< CaptureSource > windows;
	EnumWindows(enumWindowsProc, reinterpret_cast< LPARAM >(&windows));
	return windows;
}

#endif // USE_SCREEN_SHARING
