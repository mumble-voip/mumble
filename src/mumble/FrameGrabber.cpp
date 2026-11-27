// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "FrameGrabber.h"

#ifdef HAS_XCB_FRAME_GRABBER
std::unique_ptr< FrameGrabber > createXcbFrameGrabber(const CaptureSource &source);
#elif defined(Q_OS_WIN)
std::unique_ptr< FrameGrabber > createWindowsFrameGrabber(const CaptureSource &source);
#endif

std::unique_ptr< FrameGrabber > FrameGrabber::create(const CaptureSource &source) {
#ifdef HAS_XCB_FRAME_GRABBER
	return createXcbFrameGrabber(source);
#elif defined(Q_OS_WIN)
	return createWindowsFrameGrabber(source);
#else
	Q_UNUSED(source);
	return nullptr;
#endif
}
