// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

#ifdef WIN32
// Memory offsets
const procptr_t camera_pos_offset   = 0x1A2A338;
const procptr_t camera_front_offset = 0x1A2A320;
const procptr_t camera_top_offset   = 0x1A2A32C;
const procptr_t avatar_offset_base  = 0x18E1448;
const procptr_t avatar_offset_0     = 0x40;
const procptr_t avatar_offset_1     = 0x6B4;
const procptr_t avatar_offset_2     = 0x18;
const procptr_t avatar_offset_3     = 0x1C;
const procptr_t avatar_pos_offset   = 0x18;
const procptr_t avatar_front_offset = 0x0;
const procptr_t avatar_top_offset   = 0xC;
// Executable name
const wchar_t *exe_name = L"RocketLeague.exe";
#else
// Memory offsets
const procptr_t camera_pos_offset   = 0x30DE018;
const procptr_t camera_front_offset = 0x30DE000;
const procptr_t camera_top_offset   = 0x30DE00C;
const procptr_t avatar_offset_base  = 0x30DD6A0;
const procptr_t avatar_offset_0     = 0x408;
const procptr_t avatar_offset_1     = 0x648;
const procptr_t avatar_offset_2     = 0x40;
const procptr_t avatar_offset_3     = 0x80;
const procptr_t avatar_pos_offset   = 0x24;
const procptr_t avatar_front_offset = 0xC;
const procptr_t avatar_top_offset   = 0x18;
// Executable name
const wchar_t *exe_name = L"RocketLeague";
#endif

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// Boolean value to check if game addresses retrieval is successful
	bool ok;

	// Avatar pointers
	procptr_t avatar_ptr_base = peekProcPtr(pModule + avatar_offset_base);
	if (!avatar_ptr_base)
		return false;
	procptr_t avatar_ptr_0 = peekProcPtr(avatar_ptr_base + avatar_offset_0);
	if (!avatar_ptr_0)
		return false;
	procptr_t avatar_ptr_1 = peekProcPtr(avatar_ptr_0 + avatar_offset_1);
	if (!avatar_ptr_1)
		return false;
	procptr_t avatar_ptr_2 = peekProcPtr(avatar_ptr_1 + avatar_offset_2);
	if (!avatar_ptr_2)
		return false;
	procptr_t avatar_ptr_3 = peekProcPtr(avatar_ptr_2 + avatar_offset_3);
	if (!avatar_ptr_3)
		return false;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(avatar_ptr_3 + avatar_pos_offset, avatar_pos, 12) &&     // Avatar Position values (X, Y and Z).
		 peekProc(pModule + camera_pos_offset, camera_pos, 12) &&          // Camera Position values (X, Y and Z).
		 peekProc(avatar_ptr_3 + avatar_front_offset, avatar_front, 12) && // Avatar Front values (X, Y and Z).
		 peekProc(avatar_ptr_3 + avatar_top_offset, avatar_top, 12) &&     // Avatar Top values (X, Y and Z).
		 peekProc(pModule + camera_front_offset, camera_front, 12) &&      // Camera Front Vector values (X, Y and Z).
		 peekProc(pModule + camera_top_offset, camera_top, 12);            // Camera Top Vector values (X, Y and Z).

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from
	// memory addresses.
	if (!ok)
		return false;

	// Scale from centimeters to meters
	for (int i = 0; i < 3; i++) {
		avatar_pos[i] /= 100.0f;
		camera_pos[i] /= 100.0f;
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, exe_name)) // Link the game executable
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
	return std::wstring(
		L"Supports Rocket League version 1.42 without context or identity support yet."); // Plugin long description
}

static std::wstring description(L"Rocket League (v1.42)"); // Plugin short description
static std::wstring shortname(L"Rocket League");           // Plugin short name

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin rlplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
							   generic_unlock,      longdesc,    fetch };

static MumblePlugin2 rlplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &rlplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &rlplug2;
}
