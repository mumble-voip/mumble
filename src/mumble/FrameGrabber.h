// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_FRAMEGRABBER_H_
#define MUMBLE_MUMBLE_FRAMEGRABBER_H_

#ifdef USE_SCREEN_SHARING

#	include "CaptureSource.h"

#	include <QtGui/QImage>

#	include <memory>

/// Grabs pictures of a capture source whose content has to be polled at the frame rate (as opposed to the native
/// capture streams of macOS and the xdg-desktop-portal, which deliver frames on their own).
///
/// A grabber is created on the GUI thread, where the source can be looked up. Afterwards it is only used on the
/// capture thread, where it is also destroyed. Grabbing doesn't involve the GUI thread, so a busy GUI can't hold up
/// capturing and grabbing doesn't block the GUI.
///
/// Grabbed images may use any format that stores a pixel in 32 bits (e.g. QImage::Format_RGB32), so that the
/// pictures can be passed on without converting them first.
class FrameGrabber {
public:
	enum class Result {
		/// A new picture was grabbed
		Frame,
		/// The content didn't change since the last picture, or can't be grabbed right now (e.g. a minimised
		/// window). Nothing has to be encoded.
		Unchanged,
		/// The source is gone or can't be grabbed anymore
		Failed
	};

	virtual ~FrameGrabber() = default;

	/// Grabs the current picture of the source into image, if the result is Result::Frame.
	virtual Result grab(QImage &image) = 0;

	/// Creates a grabber for the given source. Returns nullptr if there is no grabber for this source on this
	/// platform. The source then has to be grabbed with grabCaptureSource() on the GUI thread instead.
	static std::unique_ptr< FrameGrabber > create(const CaptureSource &source);
};

#endif // USE_SCREEN_SHARING
#endif // MUMBLE_MUMBLE_FRAMEGRABBER_H_
