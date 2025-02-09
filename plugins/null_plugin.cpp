// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#ifndef NULL_DESC
#	define NULL_DESC L"Retracted plugin"
#endif // NULL_DESC

static int fetch(float *, float *, float *, float *, float *, float *, std::string &, std::wstring &) {
	// Empty
	return false;
}

static int trylock() {
	// Empty
	return false;
}

static const std::wstring longdesc() {
	return std::wstring(L"NULL plugin for retracted plugin. This should not be visible in Mumble >= 1.2.4");
}

static std::wstring description(NULL_DESC);
static std::wstring shortname(L"Retracted");

static MumblePlugin nullplug = {
	MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock, nullptr, longdesc, fetch
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &nullplug;
}
