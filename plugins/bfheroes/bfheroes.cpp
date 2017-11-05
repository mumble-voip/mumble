// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h"

procptr_t posptr, faceptr, topptr, stateptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, MumbleString *, MumbleWideString *) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	char state;
	char logincheck;
	bool ok;

	ok = peekProc(0x00A24D6C, &logincheck, 1);
	if (! ok)
		return false;

	if (logincheck == 0)
		return false;

	ok = peekProc(posptr, avatar_pos, 12) &&
	     peekProc(faceptr, avatar_front, 12) &&
	     peekProc(topptr, avatar_top, 12) &&
	     peekProc(stateptr, &state, 1);

	if (! ok)
		return false;

	/*
	   This state value just uses the first memory position. If the memory position is "0," then it means that you are not ingame.
	*/
	if (state == 0)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const MumblePIDLookup lookupFunc, const MumblePIDLookupContext lookupContext) {
	posptr = faceptr = topptr = 0;

	if (! initialize(lookupFunc, lookupContext, L"BFHeroes.exe", L"BFAudio.dll"))
		return false;

	procptr_t cacheaddr = pModule + 0x4745c;
	procptr_t cache = peekProcPtr(cacheaddr);

	posptr = peekProcPtr(cache + 0xc0);
	faceptr = peekProcPtr(cache + 0xc4);
	topptr = peekProcPtr(cache + 0xc8);
	stateptr = peekProcPtr(cache + 0xc0);

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	MumbleString context;
	MumbleWideString identity;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, &context, &identity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static MumblePlugin bfheroesplug = {
	MUMBLE_PLUGIN_MAGIC,
	1,
	true,
	MumbleInitConstWideString(L"Battlefield Heroes"),
	MumbleInitConstWideString(L"Unknown"),
	MumbleInitConstWideString(L"Supports Battlefield Heroes. No identity support yet."),
	fetch,
	trylock,
	generic_unlock,
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bfheroesplug;
}
