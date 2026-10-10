// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_SCREEN_SHARING

#	include "CaptureSourceLister.h"

#	include <QtGui/QGuiApplication>

// X11 window enumeration — only available when running on X11 (not Wayland).
#	ifdef HAS_X11_WINDOW_LIST
#		include <X11/Xatom.h>
#		include <X11/Xlib.h>
#	endif

#	ifdef HAS_X11_WINDOW_LIST
/// Read the _NET_CLIENT_LIST_STACKING property from the root window to get the ordered
/// list of managed windows (top of stack last, which is the natural display order).
static QList< Window > getX11WindowList(Display *display) {
	QList< Window > result;
	Atom netClientList = XInternAtom(display, "_NET_CLIENT_LIST_STACKING", True);
	if (netClientList == None)
		netClientList = XInternAtom(display, "_NET_CLIENT_LIST", True);
	if (netClientList == None)
		return result;

	Atom actualType;
	int actualFormat;
	unsigned long itemCount, bytesAfter;
	unsigned char *data = nullptr;
	if (XGetWindowProperty(display, DefaultRootWindow(display), netClientList, 0, ~0L, False, XA_WINDOW, &actualType,
						   &actualFormat, &itemCount, &bytesAfter, &data)
		== Success) {
		if (data) {
			auto *windows = reinterpret_cast< Window * >(data);
			// Reverse: top of stack (most recently active) first.
			for (long i = static_cast< long >(itemCount) - 1; i >= 0; --i)
				result.append(windows[i]);
			XFree(data);
		}
	}
	return result;
}

static QString getX11WindowTitle(Display *display, Window window) {
	// Try _NET_WM_NAME (UTF-8) first.
	Atom netWmName  = XInternAtom(display, "_NET_WM_NAME", False);
	Atom utf8String = XInternAtom(display, "UTF8_STRING", False);
	Atom actualType;
	int actualFormat;
	unsigned long itemCount, bytesAfter;
	unsigned char *data = nullptr;

	if (XGetWindowProperty(display, window, netWmName, 0, 1024, False, utf8String, &actualType, &actualFormat,
						   &itemCount, &bytesAfter, &data)
			== Success
		&& data) {
		QString title = QString::fromUtf8(reinterpret_cast< const char * >(data));
		XFree(data);
		if (!title.isEmpty())
			return title;
	}

	// Fallback to XFetchName (Latin-1).
	char *name = nullptr;
	if (XFetchName(display, window, &name) && name) {
		QString title = QString::fromLatin1(name);
		XFree(name);
		return title;
	}
	return {};
}
#	endif // HAS_X11_WINDOW_LIST

QList< CaptureSource > listCaptureWindows() {
	QList< CaptureSource > windows;

#	ifdef HAS_X11_WINDOW_LIST
	// Windows — X11 only. Under Wayland, XOpenDisplay() may still succeed because of XWayland, but Qt can't grab
	// those windows there.
	Display *display = QGuiApplication::platformName() == QLatin1String("xcb") ? XOpenDisplay(nullptr) : nullptr;
	if (display) {
		const QList< Window > windowIds = getX11WindowList(display);
		for (Window xid : windowIds) {
			const QString title = getX11WindowTitle(display, xid);
			if (title.isEmpty())
				continue;

			CaptureSource s;
			s.type           = CaptureSource::Type::Window;
			s.nativeWindowId = static_cast< quintptr >(xid);
			s.displayName    = title;
			windows.append(s);
		}
		XCloseDisplay(display);
	}
#	endif // HAS_X11_WINDOW_LIST

	return windows;
}

#endif // USE_SCREEN_SHARING
