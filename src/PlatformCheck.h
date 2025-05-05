// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_PLATFORMCHECK_H_
#define MUMBLE_PLATFORMCHECK_H_

/// PlatformCheck performs platform checks.
class PlatformCheck {
public:
	/// IsWine returns true when running under Wine.
	static bool IsWine();
};

#endif
