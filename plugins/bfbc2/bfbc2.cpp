// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

bool is_steam = false;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// char ccontext[128];
	// char state;
	bool ok;

	/*
	ok = peekProc((BYTE *) 0x0, &state, 1); // Magical state value
	if (! ok)
		return false;
	*/

	// Find out whether this is the steam version
	char sMagic[6];
	if (!peekProc(0x015715b4, sMagic, 6)) {
		generic_unlock();
		return false;
	}

	is_steam = (strncmp("Score:", sMagic, 6) == 0);

	if (is_steam) {
		ok = peekProc(0x01571E90, avatar_pos, 12) && peekProc(0x01571E80, avatar_front, 12)
			 && peekProc(0x01571E70, avatar_top, 12);
	} else {
		ok = peekProc(0x01579600, avatar_pos, 12) && peekProc(0x015795F0, avatar_front, 12)
			 && peekProc(0x015795E0, avatar_top, 12);
	}

	if (!ok)
		return false;

	// Disable when not in game
	if (avatar_pos[1] == 9999)
		return true;

	/*
		Get context string; in this plugin this will be an
		ip:port (char 256 bytes) string

	ccontext[127] = 0;
	context = std::string(ccontext);
	*/
	/*
	if (state == 0)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	*/

	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	return ok;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"BFBC2Game.exe"))
		return false;

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (!fetch(apos, afront, atop, cpos, cfront, ctop, context, identity)) {
		generic_unlock();
		return false;
	}

	return true;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Battlefield Bad Company 2 build 795745. No identity or context support.");
}

static std::wstring description(L"Battlefield Bad Company 2 build 795745");
static std::wstring shortname(L"Battlefield Bad Company 2");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin bfbc2plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								  generic_unlock,      longdesc,    fetch };

static MumblePlugin2 bfbc2plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bfbc2plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &bfbc2plug2;
}
