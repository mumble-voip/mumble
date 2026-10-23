// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef USE_SCREEN_SHARING

#	include "CaptureSourceLister.h"

#	include <QtGui/QGuiApplication>
#	include <QtGui/QPixmap>
#	include <QtGui/QScreen>

static constexpr int THUMBNAIL_WIDTH  = 160;
static constexpr int THUMBNAIL_HEIGHT = 90;

static QPixmap scaledThumbnail(const QPixmap &px) {
	if (px.isNull())
		return {};
	return QPixmap::fromImage(
		px.toImage().scaled(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

/// QScreen::grabWindow() takes an X window ID on X11 and a HWND on Windows.
static QPixmap grabNativeWindow(quintptr nativeWindowId) {
	QScreen *screen = QGuiApplication::primaryScreen();
	if (!screen)
		return {};
	return screen->grabWindow(static_cast< WId >(nativeWindowId));
}

QList< CaptureSource > listCaptureSources() {
	QList< CaptureSource > sources;

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

	for (CaptureSource &window : listCaptureWindows()) {
		window.thumbnail = scaledThumbnail(grabNativeWindow(window.nativeWindowId));
		sources.append(window);
	}

	return sources;
}

QImage grabCaptureSource(const CaptureSource &source) {
	QPixmap px;
	if (source.type == CaptureSource::Type::EntireScreen) {
		const QList< QScreen * > screens = QGuiApplication::screens();
		if (source.screenIndex < 0 || source.screenIndex >= screens.size())
			return {};
		px = screens.at(source.screenIndex)->grabWindow(0);
	} else {
		px = grabNativeWindow(source.nativeWindowId);
	}

	if (px.isNull())
		return {};
	return px.toImage().convertToFormat(QImage::Format_RGBA8888);
}

#endif // USE_SCREEN_SHARING
