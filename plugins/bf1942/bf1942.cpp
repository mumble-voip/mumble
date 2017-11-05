// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h"

procptr_t faceptr, topptr;
//BYTE *stateptr;
//BYTE *contextptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, MumbleString *, MumbleWideString *) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	//char ccontext[128];
	//char state;
	//char spawn;
	bool ok;

	//ok = peekProc(stateptr, &state, 1); // Magical state value
	//if (! ok)
	//	return false;

	//if (state == 0)
	//	return false;

	//peekProc(contextptr, ccontext, 128);

	ok = peekProc(0x00976274, avatar_pos, 12) &&
	     peekProc(faceptr, avatar_front, 12) &&
	     peekProc(topptr, avatar_top, 12);

	if (! ok)
		return false;

	/*
	    Get context string; in this plugin this will be an
	    ip:port (char 256 bytes) string
	*/
	//ccontext[127] = 0;
	//context = std::string(ccontext);

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const MumblePIDLookup lookupFunc, const MumblePIDLookupContext lookupContext) {
	faceptr = topptr = 0;

	if (! initialize(lookupFunc, lookupContext, L"BF1942.exe"))
		return false;

	procptr_t ptr1 = peekProcPtr(0x009A9468);
	procptr_t ptr2 = peekProcPtr(ptr1 + 0x98);

	faceptr = ptr2 + 0x5C;
	topptr = ptr2 + 0x4C;

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

static MumblePlugin bf1942plug = {
	MUMBLE_PLUGIN_MAGIC,
	1,
	false,
	L"Battlefield 1942",
	L"1.61b",
	L"Supports Battlefield 1942. No identity or context support yet.",
	fetch,
	trylock,
	generic_unlock,
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bf1942plug;
}
