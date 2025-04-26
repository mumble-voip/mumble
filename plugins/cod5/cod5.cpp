// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	float viewHor, viewVer;
	char state;

	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;

	/*
		This plugin uses the following Variables:

			Address			Type	Description
			===================================
			0x008DE23C		float	Z-Coordinate
			0x008DE234		float	X-Coordinate
			0x008DE238		float	Y-Coordinate
			0x008DE244		float	Horizontal view (degrees)
			0x008DE240		float	Vertical view (degrees)

			0x0098FD2C		byte	Magical state value
	*/
	ok = peekProc(0x0098FD2C, &state, 1); // Magical state value
	if (!ok)
		return false;
	/*
		state value is:
		0		while not in game
		4		while playing
		8		while spectating

		This value is used for disabling pa for spectators
		or people not on a server.
	*/
	if (state != 4)
		return true; // This results in all vectors beeing zero which tells mumble to ignore them.

	ok = peekProc(0x008DE23C, avatar_pos + 2, 4) && // Z
		 peekProc(0x008DE234, avatar_pos, 4) &&     // X
		 peekProc(0x008DE238, avatar_pos + 1, 4) && // Y
		 peekProc(0x008DE244, &viewHor, 4) &&       // Hor
		 peekProc(0x008DE240, &viewVer, 4);         // Ver

	if (!ok)
		return false;

	// Scale Coordinates
	/*
	   Z-Value is increasing when heading north
				  decreasing when heading south
	   X-Value is increasing when heading west
				  decreasing when heading east
	   Y-Value is increasing when going up
				  decreasing when going down
	   40 units = 1 meter (not confirmed)
	*/
	for (int i = 0; i < 3; i++)
		avatar_pos[i] /= 40.0f; // Scale to meters
	avatar_pos[0] *= (-1.0f);   // Convert right to left handed

	avatar_top[2] = -1; // Head movement is in front vector

	// Calculate view unit vector
	/*
	   Vertical view 0° when centered
					85°	when looking down
				   275° when looking up
	   Decreasing when looking up.

	   Horizontal is 0° when facing North
					90° when facing West
				   180° when facing South
				   270° when facing East
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
	if (!initialize(pids, L"CoDWaWmp.exe"))
		return false;

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
	return std::wstring(L"Supports Call of Duty 5 MP v1.7.1263 only. No context or identity support yet.");
}

static std::wstring description(L"Call of Duty 5 MP v1.7.1263");
static std::wstring shortname(L"Call of Duty 5 MP");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin cod5plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								 generic_unlock,      longdesc,    fetch };

static MumblePlugin2 cod5plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &cod5plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &cod5plug2;
}
