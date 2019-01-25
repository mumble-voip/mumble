// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h"

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &, std::wstring &) {
	float viewHor, viewVer;
	char state;

	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;

	/*
		This plugin uses the following Variables:

			Address			Type	Description
			===================================
			0x01516608		float	Z-Coordinate (=0 out-of-game)
			0x0151660C		float	X-Coordinate (=0 out-of-game)
			0x01516610		float	Y-Coordinate (=0 out-of-game)
			0x0151A114		float	Horizontal view (degrees) (=0 out-of-game)
			0x0151A110		float	Vertical view (degrees) (=0 out-of-game)
			0x0096B688		byte	Magic value (0=ingame/out-of-game, 4=spectator)
	*/
	ok = peekProc(0x0096B688, &state, 1);
	if (! ok)
		return false;

	if (state == 4)
		return true; // If this magic value is 4 we are spectating, so switch of PA

	ok = peekProc(0x01516608, avatar_pos+2, 4) &&	//Z
	     peekProc(0x0151660C, avatar_pos, 4) &&	//X
	     peekProc(0x01516610, avatar_pos+1, 4) && //Y
	     peekProc(0x0151A114, &viewHor, 4) && //Hor
	     peekProc(0x0151A110, &viewVer, 4); //Ver

	if (! ok)
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
	for (int i=0;i<3;i++)
		avatar_pos[i]/=40.0f; // Scale to meters
	avatar_pos[0]*=(-1.0f); // Convert right to left handed

	avatar_top[2] = -1; // Head movement is in front vector

	// Calculate view unit vector
	/*
	   Vertical view 0 when centered
					85	when looking down
				   275 when looking up
	   Decreasing when looking up.

	   Horizontal is 0 when facing North
					90 when facing West
				   180 when facing South
				   270 when facing East
	   Increasing when turning left.
	*/
	viewVer *= static_cast<float>(M_PI / 180.0f);
	viewHor *= static_cast<float>(M_PI / 180.0f);

	avatar_front[0] = -sin(viewHor) * cos(viewVer);
	avatar_front[1] = -sin(viewVer);
	avatar_front[2] = cos(viewHor) * cos(viewVer);

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	if (! initialize(pids, L"CoD2MP_s.exe"))
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
	return std::wstring(L"Supports Call of Duty 2 v1.3. No context or identity support yet.");
}

static std::wstring description(L"Call of Duty 2 v1.3");
static std::wstring shortname(L"Call of Duty 2");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin cod2plug = {
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

static MumblePlugin2 cod2plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &cod2plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &cod2plug2;
}
