// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_OVERLAY_WHITELIST_H_
#define MUMBLE_OVERLAY_WHITELIST_H_

static const char *overlayWhitelist[] = {
	// We can't whitelist the GW2 launcher
	"gw2.exe", "gw2-64.exe",

	nullptr
};

#endif
