// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "mumble_plugin.h"

#ifndef NULL_DESC
#define NULL_DESC L"NULL plugin for retracted plugin. This should not be visible in Mumble >= 1.2.4"
#endif // NULL_DESC

static int fetch(float *, float *, float *, float *, float *, float *, MumbleString *, MumbleWideString *) {
	// Empty
	return false;
}

static int trylock(const MumblePIDLookup, const MumblePIDLookupContext) {
	// Empty
	return false;
}

static MumblePlugin nullplug = {
	MUMBLE_PLUGIN_MAGIC,
	1,
	true,
	L"Retracted",
	L"",
	NULL_DESC,
	fetch,
	trylock,
	NULL,
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &nullplug;
}

