// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CAPTURESOURCELISTER_H_
#define MUMBLE_MUMBLE_CAPTURESOURCELISTER_H_

#ifdef USE_SCREEN_SHARING

#	include "CaptureSource.h"

#	include <QtCore/QList>
#	include <QtGui/QImage>

/// Returns all available capture sources — screens first, then visible application windows.
/// Thumbnails are populated. Must be called on the GUI thread.
QList< CaptureSource > listCaptureSources();

/// Grabs one full-resolution frame from the given source and returns it as QImage (Format_RGBA8888).
/// On macOS uses CGWindowListCreateImage for Window sources; uses QScreen::grabWindow elsewhere.
/// Must be called on the GUI thread (screen capture APIs require it).
QImage grabCaptureSource(const CaptureSource &source);

#endif // USE_SCREEN_SHARING
#endif // MUMBLE_MUMBLE_CAPTURESOURCELISTER_H_
