// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

procptr_t posptr, frontptr, topptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// char state;
	bool ok;

	/*
	float front_corrector1;
	float front_corrector2;
	float front_corrector3;

	float top_corrector1;
	float top_corrector2;
	float top_corrector3;
	*/

	/*
		value is 0 when one is not in a game, 4 when one is

	ok = peekProc((BYTE *) 0x, &state, 1); // Magical state value
	if (! ok)
		return false;

	if (state == 0)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	*/

	ok = peekProc(posptr, avatar_pos, 12) && peekProc(frontptr, avatar_front, 12) && peekProc(topptr, avatar_top, 12);

	if (avatar_pos[1] > 999000000.0)
		return false;

	/*
	peekProc(frontptr, &front_corrector1, 4) &&
	peekProc(frontptr + 0xC, &front_corrector2, 4) &&
	peekProc(frontptr + 0x18, &front_corrector3, 4) &&
	peekProc(topptr, &top_corrector1, 4) &&
	peekProc(topptr + 0xC, &top_corrector2, 4) &&
	peekProc(topptr + 0x18, &top_corrector3, 4);
	*/

	if (!ok)
		return false;

	/*
	avatar_front[0] = front_corrector1;
	avatar_front[1] = front_corrector2;
	avatar_front[2] = front_corrector3;

	avatar_top[0] = top_corrector1;
	avatar_top[1] = top_corrector2;
	avatar_top[2] = top_corrector3;
	*/

	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	posptr = 0;

	if (!initialize(pids, L"arma2.exe"))
		return false;

	/*
	BYTE bState;
	peekProc((BYTE *) 0x00BF64D0, &bState, 1);
	if (bState == 0)
		return false;
	*/

	/*
	   Comment out code we don't need
	   BYTE *pModule=getModuleAddr(L"<module name, if you need it>.dll");
	   if (!pModule)
	*/

	procptr_t ptr1 = peekProcPtr(0x00C500FC);

	procptr_t ptr2 = peekProcPtr(ptr1 + 0x88);

	procptr_t base = ptr2 + 0x10;

	posptr   = base + 0x18;
	frontptr = base;
	topptr   = base + 0xC;

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
	return std::wstring(L"Supports Armed Assault 2 v1.08. No identity or context support yet.");
}

static std::wstring description(L"ArmA 2 v1.08");
static std::wstring shortname(L"ArmA 2");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin arma2plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								  generic_unlock,      longdesc,    fetch };

static MumblePlugin2 arma2plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &arma2plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &arma2plug2;
}
