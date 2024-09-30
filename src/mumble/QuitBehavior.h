// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_QUITBEHAVIOR_H
#define MUMBLE_QUITBEHAVIOR_H

/// This enum lists a series of quit options
enum class QuitBehavior {
	ALWAYS_ASK              = 0,
	ASK_WHEN_CONNECTED      = 1,
	ALWAYS_MINIMIZE         = 2,
	MINIMIZE_WHEN_CONNECTED = 3,
	ALWAYS_QUIT             = 4
};

#endif // MUMBLE_QUITBEHAVIOR_H
