// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_SCREEN_SHARING

#	include "CaptureSourceLister.h"

#	include <QtGui/QGuiApplication>
#	include <QtGui/QPixmap>
#	include <QtGui/QScreen>

#	include <windows.h>

static constexpr int THUMBNAIL_WIDTH  = 160;
static constexpr int THUMBNAIL_HEIGHT = 90;

struct EnumWindowsContext {
	QList< CaptureSource > *sources;
};

static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam) {
	auto *ctx = reinterpret_cast< EnumWindowsContext * >(lParam);

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
	const QString displayName = QString::fromWCharArray(title);

	// Thumbnail: grab via Qt (works for windows on Windows via QScreen::grabWindow(HWND)).
	QPixmap px;
	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen) {
		px = screen->grabWindow(reinterpret_cast< WId >(hwnd));
		if (!px.isNull()) {
			px = QPixmap::fromImage(
				px.toImage().scaled(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	}

	CaptureSource s;
	s.type           = CaptureSource::Type::Window;
	s.nativeWindowId = reinterpret_cast< quintptr >(hwnd);
	s.displayName    = displayName;
	s.thumbnail      = px;
	ctx->sources->append(s);
	return TRUE;
}

QList< CaptureSource > listCaptureSources() {
	QList< CaptureSource > sources;

	// Screens.
	const QList< QScreen * > screens = QGuiApplication::screens();
	for (int i = 0; i < screens.size(); ++i) {
		QScreen *screen = screens.at(i);
		CaptureSource s;
		s.type        = CaptureSource::Type::EntireScreen;
		s.screenIndex = i;
		s.displayName =
			QObject::tr("Display %1 (%2×%3)").arg(i + 1).arg(screen->size().width()).arg(screen->size().height());
		QPixmap px = screen->grabWindow(0);
		if (!px.isNull()) {
			s.thumbnail = QPixmap::fromImage(
				px.toImage().scaled(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
		sources.append(s);
	}

	// Windows.
	EnumWindowsContext ctx{ &sources };
	EnumWindows(enumWindowsProc, reinterpret_cast< LPARAM >(&ctx));

	return sources;
}

QImage grabCaptureSource(const CaptureSource &source) {
	if (source.type == CaptureSource::Type::EntireScreen) {
		const QList< QScreen * > screens = QGuiApplication::screens();
		if (source.screenIndex < 0 || source.screenIndex >= screens.size())
			return {};
		QPixmap px = screens.at(source.screenIndex)->grabWindow(0);
		if (px.isNull())
			return {};
		return px.toImage().convertToFormat(QImage::Format_RGBA8888);
	}

	// Qt's grabWindow with HWND works on Windows.
	QScreen *screen = QGuiApplication::primaryScreen();
	if (!screen)
		return {};
	QPixmap px = screen->grabWindow(static_cast< WId >(source.nativeWindowId));
	if (px.isNull())
		return {};
	return px.toImage().convertToFormat(QImage::Format_RGBA8888);
}

#endif // USE_SCREEN_SHARING
