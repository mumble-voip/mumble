// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32_x86.h"

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=camera_pos[i]=camera_front[i]=camera_top[i]=0.0f;

	// Boolean value to check if game addresses retrieval is successful
	bool ok;

	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], avatar_front_corrector[3], camera_front_corrector[3], camera_top_corrector[3];

	// Avatar pointers
	procptr32_t avatar_base = peekProc<procptr32_t>(pModule + 0x016FEC04);
	if (!avatar_base) return false;
	procptr32_t avatar_offset_0 = peekProc<procptr32_t>(avatar_base + 0x448);
	if (!avatar_offset_0) return false;
	procptr32_t avatar_offset_1 = peekProc<procptr32_t>(avatar_offset_0 + 0x440);
	if (!avatar_offset_1) return false;
	procptr32_t avatar_offset_2 = peekProc<procptr32_t>(avatar_offset_1 + 0x0);
	if (!avatar_offset_2) return false;
	procptr32_t avatar_offset = peekProc<procptr32_t>(avatar_offset_2 + 0x1C);
	if (!avatar_offset) return false;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(avatar_offset + 0x0, &avatar_pos_corrector, 12) && // Avatar Position values (Z, Y and X).
			peekProc(pModule + 0x016FEE40, &camera_pos_corrector, 12) && // Camera Position values (Z, Y and X).
			peekProc(avatar_offset + 0xC, &avatar_front_corrector, 12) && // Avatar Front values (Z, Y and X).
			peekProc(pModule + 0x016FEE28, &camera_front_corrector, 12) && // Camera Front Vector values (Z, Y and X).
			peekProc(pModule + 0x016FEE34, &camera_top_corrector, 12); // Camera Top Vector values (Z, Y and X).

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from memory addresses.
	if (! ok)
		return false;

	/*
	Mumble | Game
	X      | Z
	Y      | Y
	Z      | -X
	*/
	avatar_pos[0] = avatar_pos_corrector[2];
	avatar_pos[1] = avatar_pos_corrector[1];
	avatar_pos[2] = -avatar_pos_corrector[0]; // Convert from right handed to left handed coordinate system

	camera_pos[0] = camera_pos_corrector[2];
	camera_pos[1] = camera_pos_corrector[1];
	camera_pos[2] = -camera_pos_corrector[0]; // Convert from right handed to left handed coordinate system

	avatar_front[0] = avatar_front_corrector[2];
	avatar_front[1] = avatar_front_corrector[1];
	avatar_front[2] = avatar_front_corrector[0];

	avatar_top[2] = -1; // This tells Mumble to automatically calculate top vector using front vector.

	camera_front[0] = camera_front_corrector[2];
	camera_front[1] = camera_front_corrector[1];
	camera_front[2] = camera_front_corrector[0];

	camera_top[0] = camera_top_corrector[2];
	camera_top[1] = camera_top_corrector[1];
	camera_top[2] = camera_top_corrector[0];

	// Scale from centimeters to meters
	for (int i=0;i<3;i++) {
		avatar_pos[i]/=100.0f;
		camera_pos[i]/=100.0f;
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"RocketLeague.exe")) // Link the game executable
		return false;

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
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
	return std::wstring(L"Supports Rocket League version 1.18 without context or identity support yet."); // Plugin long description
}

static std::wstring description(L"Rocket League (v1.18)"); // Plugin short description
static std::wstring shortname(L"Rocket League"); // Plugin short name

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
