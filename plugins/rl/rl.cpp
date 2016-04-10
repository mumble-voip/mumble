// Copyright 2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h"

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

        // char state;
	bool ok;
        // Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float pos_corrector[3];
	float front_corrector[3];
	float top_corrector[3];

	/*
		value is <     >
	*/
	// ok = peekProc((BYTE *) 0x<offset>, &state, 1); // Magical state value
	// if (! ok)
	//	return false;

	// if (state == 0)
	//  return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

        // Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc((BYTE *) pModule + 0x015E5EB0, &pos_corrector, 12) &&
	     peekProc((BYTE *) pModule + 0x015E5EB0, &front_corrector, 12) &&
	     peekProc((BYTE *) pModule + 0x015E5EB0, &top_corrector, 12);

	if (! ok)
		return false;

        // X = pModule + 0x015E5EB8
        // Y = pModule + 0x015E5EB4
        // Z = pModule + 0x015E5EB0
	avatar_pos[0] = pos_corrector[2];
	avatar_pos[1] = pos_corrector[1];
	avatar_pos[2] = pos_corrector[0];

	for (int i=0;i<3;i++)
		avatar_pos[i]/=32.0f; // Scale to meters

	avatar_front[0] = front_corrector[2];
	avatar_front[1] = front_corrector[1];
	avatar_front[2] = front_corrector[0];

	avatar_top[0] = top_corrector[2];
	avatar_top[1] = top_corrector[1];
	avatar_top[2] = top_corrector[0];

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
	return std::wstring(L"Supports Rocket League version 1.0.10897.0 without identity nor context support yet.");
}

static std::wstring description(L"Rocket League (version 1.0.10897.0)");
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
