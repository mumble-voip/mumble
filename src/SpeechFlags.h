// Copyright 2020-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_SPEECHFLAGS_H_
#define MUMBLE_SPEECHFLAGS_H_

namespace SpeechFlags {
enum SpeechFlags {
	Invalid = 0xFF, // This is the unsigned equivalent of -1
	Normal  = 0,
	Shout   = 1,
	Whisper = 2,
	Listen  = 3
};
}; // namespace SpeechFlags

#endif // MUMBLE_SPEECHFLAGS_H_
