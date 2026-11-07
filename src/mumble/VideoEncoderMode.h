// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VIDEOENCODERMODE_H_
#define MUMBLE_MUMBLE_VIDEOENCODERMODE_H_

/// How the video encoder for screen sharing is chosen.
enum class VideoEncoderMode {
	/// The best encoder that works on this system, regardless of what the viewers can decode
	Best,
	/// The best encoder whose codec the viewers can decode
	Optimised,
	/// The encoder the user picked, as long as it works
	Manual,
};

#endif // MUMBLE_MUMBLE_VIDEOENCODERMODE_H_
