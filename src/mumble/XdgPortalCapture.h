// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_XDGPORTALCAPTURE_H_
#define MUMBLE_MUMBLE_XDGPORTALCAPTURE_H_

// Use __linux__ (compiler-native) rather than Q_OS_LINUX so this guard works even when
// included before Qt headers have been processed.
#if defined(__linux__) && defined(USE_SCREEN_SHARING) && defined(HAS_WAYLAND_PORTAL)

#	include <QtCore/QString>
#	include <QtGui/QImage>
#	include <functional>

/// Returns true if Wayland portal screen capture is available.
/// Requires: running under a Wayland compositor (QPA platform == "wayland"),
/// the xdg-desktop-portal D-Bus service to be present, and PipeWire to be running.
bool xdg_portal_isNativePickerAvailable();

/// Shows the xdg-desktop-portal ScreenCast picker and, upon selection,
/// begins delivering captured frames via a PipeWire stream.
///
/// Mirrors the macOS sckit_startWithNativePicker() API.
/// All callbacks are invoked on the Qt main thread.
///
/// @param onStarted    Called once when the PipeWire stream starts delivering frames.
/// @param onCancelled  Called if the user dismisses the portal picker without a selection.
/// @param onError      Called with a description string if the session or stream fails.
/// @param onFrame      Called for each captured frame (QImage::Format_RGBA8888, ~15 fps).
void xdg_portal_startCapture(std::function< void() > onStarted, std::function< void() > onCancelled,
							 std::function< void(QString) > onError, std::function< void(QImage) > onFrame);

/// Stops the active PipeWire stream and closes the portal session.
void xdg_portal_stop();

#endif // __linux__ && USE_SCREEN_SHARING && HAS_WAYLAND_PORTAL
#endif // MUMBLE_MUMBLE_XDGPORTALCAPTURE_H_
