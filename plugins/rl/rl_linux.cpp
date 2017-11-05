// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_linux.h"

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, MumbleString *, MumbleWideString *) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=camera_pos[i]=camera_front[i]=camera_top[i]=0.0f;

	// Boolean value to check if game addresses retrieval is successful
	bool ok;

	// Avatar pointers
	procptr_t avatar_base = peekProcPtr(pModule + 0x302A4F0);
	if (!avatar_base) return false;
	procptr_t avatar_offset_0 = peekProcPtr(avatar_base + 0x6c8);
	if (!avatar_offset_0) return false;
	procptr_t avatar_offset_1 = peekProcPtr(avatar_offset_0 + 0x2b0);
	if (!avatar_offset_1) return false;
	procptr_t avatar_offset = peekProcPtr(avatar_offset_1 + 0x38);
	if (!avatar_offset) return false;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(avatar_offset + 0x60, avatar_pos, 12) && // Avatar Position values (X, Y and Z).
			peekProc(pModule + 0x302AAB8, camera_pos, 12) && // Camera Position values (X, Y and Z).
			peekProc(avatar_offset + 0x6C, avatar_front, 12) && // Avatar Front values (X, Y and Z).
			peekProc(pModule + 0x302AAA0, camera_front, 12) && // Camera Front Vector values (X, Y and Z).
			peekProc(pModule + 0x302AAAC, camera_top, 12); // Camera Top Vector values (X, Y and Z).

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from memory addresses.
	if (! ok)
		return false;

	avatar_top[2] = -1; // This tells Mumble to automatically calculate top vector using front vector.

	// Scale from centimeters to meters
	for (int i=0;i<3;i++) {
		avatar_pos[i]/=100.0f;
		camera_pos[i]/=100.0f;
	}

	return true;
}

static int trylock(const MumblePIDLookup lookupFunc, const MumblePIDLookupContext lookupContext) {

	if (! initialize(lookupFunc, lookupContext, L"RocketLeague")) // Link the game executable
		return false;

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	MumbleWideString sidentity;
	MumbleString scontext;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, &scontext, &sidentity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static MumblePlugin rlplug = {
	MUMBLE_PLUGIN_MAGIC,
	1, // Plugin version
	false, // Is the plugin retracted?
	MumbleInitConstWideString(L"Rocket League"), // Plugin name
	MumbleInitConstWideString(L"1.29"), // Game version
	MumbleInitConstWideString(L"Supports Rocket League without context or identity support yet."), // Plugin description
	fetch,
	trylock,
	generic_unlock
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &rlplug;
}
