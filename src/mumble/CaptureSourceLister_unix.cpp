// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_SCREEN_SHARING

#	include "CaptureSourceLister.h"

#	include <QtGui/QGuiApplication>
#	include <QtGui/QPixmap>
#	include <QtGui/QScreen>

// X11 window enumeration — only available when running on X11 (not Wayland).
#	ifdef HAS_X11_WINDOW_LIST
#		include <X11/Xatom.h>
#		include <X11/Xlib.h>
#	endif

static constexpr int THUMBNAIL_WIDTH  = 160;
static constexpr int THUMBNAIL_HEIGHT = 90;

static QPixmap scaledThumbnail(const QPixmap &px) {
	if (px.isNull())
		return {};
	return QPixmap::fromImage(
		px.toImage().scaled(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

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

QList< CaptureSource > listCaptureSources() {
	QList< CaptureSource > sources;

	// Screens — always available.
	const QList< QScreen * > screens = QGuiApplication::screens();
	for (int i = 0; i < screens.size(); ++i) {
		QScreen *screen = screens.at(i);
		CaptureSource s;
		s.type        = CaptureSource::Type::EntireScreen;
		s.screenIndex = i;
		s.displayName =
			QObject::tr("Display %1 (%2×%3)").arg(i + 1).arg(screen->size().width()).arg(screen->size().height());
		s.thumbnail = scaledThumbnail(screen->grabWindow(0));
		sources.append(s);
	}

#	ifdef HAS_X11_WINDOW_LIST
	// Windows — X11 only.  Wayland's XOpenDisplay returns nullptr, so this block
	// is naturally skipped when running under a Wayland compositor.
	Display *display = XOpenDisplay(nullptr);
	if (display) {
		const QList< Window > windowIds = getX11WindowList(display);
		for (Window xid : windowIds) {
			const QString title = getX11WindowTitle(display, xid);
			if (title.isEmpty())
				continue;

			// Thumbnail via Qt (QScreen::grabWindow(XID) works on X11).
			QPixmap px;
			if (QScreen *screen = QGuiApplication::primaryScreen())
				px = scaledThumbnail(screen->grabWindow(static_cast< WId >(xid)));

			CaptureSource s;
			s.type           = CaptureSource::Type::Window;
			s.nativeWindowId = static_cast< quintptr >(xid);
			s.displayName    = title;
			s.thumbnail      = px;
			sources.append(s);
		}
		XCloseDisplay(display);
	}
#	endif // HAS_X11_WINDOW_LIST

	return sources;
}

QImage grabCaptureSource(const CaptureSource &source) {
	if (source.type == CaptureSource::Type::EntireScreen) {
		const QList< QScreen * > screens = QGuiApplication::screens();
		if (source.screenIndex < 0 || source.screenIndex >= screens.size())
			return {};
		QPixmap px = screens.at(source.screenIndex)->grabWindow(0);
		if (px.isNull()) {
			return {};
		}
		return px.toImage().convertToFormat(QImage::Format_RGBA8888);
	}

	// Window capture on X11: QScreen::grabWindow(XID) works.
	QScreen *screen = QGuiApplication::primaryScreen();
	if (!screen)
		return {};
	QPixmap px = screen->grabWindow(static_cast< WId >(source.nativeWindowId));
	if (px.isNull())
		return {};
	return px.toImage().convertToFormat(QImage::Format_RGBA8888);
}

#endif // USE_SCREEN_SHARING
