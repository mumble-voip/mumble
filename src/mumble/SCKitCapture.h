// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_SCKITCAPTURE_H_
#define MUMBLE_MUMBLE_SCKITCAPTURE_H_

#if defined(Q_OS_MAC) && defined(USE_SCREEN_SHARING)

#	include <QtCore/QString>
#	include <QtGui/QImage>
#	include <functional>

/// Shows the native macOS screen/window picker (SCContentSharingPicker) and, upon selection,
/// begins delivering captured frames via SCStream.
///
/// All callbacks except onFrame are guaranteed to be invoked on the Qt/Cocoa main thread.
///
/// @param frameRate  Maximum number of frames per second to deliver
/// @param onStarted  Called once when the SCStream is successfully running.
/// @param onCancelled  Called if the user dismisses the picker without choosing a source, or ends the screen
///                     share through the system later on (e.g. with the screen sharing item in the menu bar).
/// @param onError  Called with a description string if stream startup fails or the running stream fails.
/// @param onFrame  Called on the stream's queue for each captured frame (QImage::Format_RGB32), so that frames
///                 don't have to wait for a busy GUI.
void sckit_startWithNativePicker(int frameRate, std::function< void() > onStarted, std::function< void() > onCancelled,
								 std::function< void(QString) > onError, std::function< void(QImage) > onFrame);

/// Stops the active SCStream and deactivates the picker.
void sckit_stop();

#endif // Q_OS_MAC && USE_SCREEN_SHARING
#endif // MUMBLE_MUMBLE_SCKITCAPTURE_H_
