// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32_32bit.h"

procptr32_t posptr, faceptr, topptr, stateptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &, std::wstring &) {
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

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	posptr = faceptr = topptr = 0;

	if (! initialize(pids, L"BFHeroes.exe", L"BFAudio.dll"))
		return false;

	procptr32_t cacheaddr = pModule + 0x4745c;
	procptr32_t cache = peekProc<procptr32_t>(cacheaddr);

	posptr = peekProc<procptr32_t>(cache + 0xc0);
	faceptr = peekProc<procptr32_t>(cache + 0xc4);
	topptr = peekProc<procptr32_t>(cache + 0xc8);
	stateptr = peekProc<procptr32_t>(cache + 0xc0);

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Battlefield Heroes. No identity support yet.");
}

static std::wstring description(L"Battlefield Heroes");
static std::wstring shortname(L"Battlefield Heroes");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin bfheroesplug = {
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

static MumblePlugin2 bfheroesplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bfheroesplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &bfheroesplug2;
}
