// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h"

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
        
        // Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float pos_corrector[3];

        // Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc((BYTE *) pModule + 0x015F4EF0, &pos_corrector, 12);

	if (! ok)
		return false;

        // X = pModule + 0x015F4EF8
        // Y = pModule + 0x015F4EF4
        // Z = pModule + 0x015F4EF0
	avatar_pos[0] = pos_corrector[2];
	avatar_pos[1] = pos_corrector[1];
	avatar_pos[2] = pos_corrector[0];

	for (int i=0;i<3;i++)
		avatar_pos[i]/=100.0f; // Scale to meters

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"RocketLeague.exe"))
		return false;

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	std::wstring sidentity;
	std::string scontext;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Rocket League version 1.17 without context or identity support yet.");
}

static std::wstring description(L"Rocket League (v1.17)");
static std::wstring shortname(L"Rocket League");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin rlplug = {
	MUMBLE_PLUGIN_MAGIC,
	description,
	shortname,
	NULL,
	NULL,
	trylock1,
	generic_unlock,
	longdesc,
	fetch
};

static MumblePlugin2 rlplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &rlplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &rlplug2;
}
