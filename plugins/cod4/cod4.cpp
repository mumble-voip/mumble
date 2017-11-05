// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h"

using namespace std;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, MumbleString *context, MumbleWideString *) {
	float viewHor, viewVer;
	char state;
	char ccontext[128];

	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;

	/*
		This plugin uses the following Variables:

			Address			Type	Description
			===================================
			0x0072AFD0		float	Z-Coordinate
			0x0072AFE0		float	X-Coordinate
			0x0072AFF0		float	Y-Coordinate
			0x0072AF3C		float	Horizontal view (degrees)
			0x0072AF38		float	Vertical view (degrees)

			0x0074E380		byte	Magical state value
	*/
	ok = peekProc(0x0074E380, &state, 1); // Magical state value
	if (! ok)
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

	ok = peekProc(0x0072AFD0, avatar_pos+2, 4) &&	//Z
	     peekProc(0x0072AFE0, avatar_pos, 4) &&	//X
	     peekProc(0x0072AFF0, avatar_pos+1, 4) && //Y
	     peekProc(0x0072AF3C, &viewHor, 4) && //Hor
	     peekProc(0x0072AF38, &viewVer, 4) && //Ver
	     peekProc(0x00956D88, ccontext, 128);

	if (! ok)
		return false;

	/*
	    Get context string; in this plugin this will be an
	    ip:port (char 256 bytes) string
	*/

	ccontext[127] = 0;
	std::string context_str(ccontext);

	if (context_str.find(':')==string::npos)
		context_str.append(":28960");

	MumbleStringAssign(context, context_str);

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
	viewVer *= static_cast<float>(M_PI / 180.0f);
	viewHor *= static_cast<float>(M_PI / 180.0f);

	avatar_front[0] = -sin(viewHor) * cos(viewVer);
	avatar_front[1] = -sin(viewVer);
	avatar_front[2] = cos(viewHor) * cos(viewVer);

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const MumblePIDLookup lookupFunc, const MumblePIDLookupContext lookupContext) {
	if (! initialize(lookupFunc, lookupContext, L"iw3mp.exe"))
		return false;

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

static MumblePlugin cod4plug = {
	MUMBLE_PLUGIN_MAGIC,
	1,
	false,
	MumbleInitConstWideString(L"Call of Duty 4"),
	MumbleInitConstWideString(L"1.7.568"),
	MumbleInitConstWideString(L"Supports Call of Duty 4. No identity support yet."),
	fetch,
	trylock,
	generic_unlock,
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &cod4plug;
}
