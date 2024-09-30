// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

using namespace std;

procptr_t pos1ptr, pos2ptr, pos3ptr, rot1ptr, rot2ptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &) {
	char menustate;
	char ccontext[128];

	float viewHor, viewVer;
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;

	/*
		This plugin uses the following Variables:

			Address			Type	Description
			===================================
			0x013F79CC		float	Y-Coordinate
			0x013E8CF4		float	X-Coordinate
			0x013E8CF8		float	Z-Coordinate
			0x013F9E20		float	Horizontal view
			0x013F9E1C		float	Vertical view
			0x013E8D18		byte	Magic value (32 ingame / 0 spectating)
	*/
	ok = peekProc(0x00801BA4, &menustate, 1);
	if (!ok)
		return false;
	if (menustate == 0)
		return true;

	ok = peekProc(pos1ptr, avatar_pos + 1, 4) && // Y
		 peekProc(pos2ptr, avatar_pos, 4) &&     // X
		 peekProc(pos3ptr, avatar_pos + 2, 4) && // Z
		 peekProc(rot1ptr, &viewHor, 4) &&       // Hor
		 peekProc(rot2ptr, &viewVer, 4) &&       // Ver
		 peekProc(0x0122E0B8, ccontext, 128);

	if (!ok)
		return false;

	avatar_top[2] = -1; // Head movement is in front vector

	ccontext[127] = 0;
	context       = std::string(ccontext);

	if (context.find(':') == string::npos)
		context.append(":27733");

	/*
	   Z-Value is increasing when heading north
				  decreasing when heading south
	   X-Value is increasing when heading east
				  decreasing when heading west
	   Y-Value is increasing when going up
				  decreasing when going down
	   1 unit = 1 meter (not confirmed)
	*/

	// Calculate view unit vector
	/*
	   Vertical view 0 when centered
				   ~+271 when looking up
				   ~+88 when looking down
	   Increasing when looking down.

	   Horizontal is +90 when facing north
					   0 when facing east
					+270 when facing south
					+180 when facing west
	   Increasing when turning left.
	*/
	viewVer *= static_cast< float >(M_PI / 180.0f);
	viewHor *= static_cast< float >(M_PI / 180.0f);

	avatar_front[0] = -sin(viewHor) * cos(viewVer);
	avatar_front[1] = -sin(viewVer);
	avatar_front[2] = cos(viewHor) * cos(viewVer);

	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	pos1ptr = pos2ptr = pos3ptr = rot1ptr = rot2ptr = 0;

	if (!initialize(pids, L"etqw.exe", L"gamex86.dll"))
		return false;

	pos1ptr = pModule + 0x74EABC;
	pos2ptr = pModule + 0x74EAB4;
	pos3ptr = pModule + 0x74EAB8;
	rot1ptr = pModule + 0x75D2B4;
	rot2ptr = pModule + 0x75D30C;

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
	return std::wstring(L"Supports Enemy Territory: Quake Wars v1.50. No identity support yet.");
}

static std::wstring description(L"Enemy Territory: Quake Wars v1.50");
static std::wstring shortname(L"Enemy Territory: Quake Wars");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin etqwplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								 generic_unlock,      longdesc,    fetch };

static MumblePlugin2 etqwplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &etqwplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &etqwplug2;
}
