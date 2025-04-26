// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

procptr_t faceptr, topptr;
// BYTE *stateptr;
// BYTE *contextptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// char ccontext[128];
	// char state;
	// char spawn;
	bool ok;

	// ok = peekProc(stateptr, &state, 1); // Magical state value
	// if (! ok)
	//	return false;

	// if (state == 0)
	//	return false;

	// peekProc(contextptr, ccontext, 128);

	ok =
		peekProc(0x00976274, avatar_pos, 12) && peekProc(faceptr, avatar_front, 12) && peekProc(topptr, avatar_top, 12);

	if (!ok)
		return false;

	/*
		Get context string; in this plugin this will be an
		ip:port (char 256 bytes) string
	*/
	// ccontext[127] = 0;
	// context = std::string(ccontext);

	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	faceptr = topptr = 0;

	if (!initialize(pids, L"BF1942.exe"))
		return false;

	procptr_t ptr1 = peekProcPtr(0x009A9468);
	procptr_t ptr2 = peekProcPtr(ptr1 + 0x98);

	faceptr = ptr2 + 0x5C;
	topptr  = ptr2 + 0x4C;

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
	return std::wstring(L"Supports Battlefield 1942 v1.61b. No identity or context support yet.");
}

static std::wstring description(L"Battlefield 1942 v1.61b");
static std::wstring shortname(L"Battlefield 1942");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin bf1942plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								   generic_unlock,      longdesc,    fetch };

static MumblePlugin2 bf1942plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bf1942plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &bf1942plug2;
}
