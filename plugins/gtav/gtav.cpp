// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h"

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], avatar_front_corrector[3], avatar_top_corrector[3], camera_front_corrector[3], camera_top_corrector[3];
	// Char values for extra features
	char state, in_game, player[50], vehicle[50], location[50], street[50];

	// Avatar pointer
	BYTE *avatar_base = peekProc<BYTE *>(pModule + 0x01B2C3D0);
	if (!avatar_base) return false;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc((BYTE *) pModule + 0x264DE00, &state, 1) && // Magical state value: 0 when in single player, 2 when online and 3 when in a lobby.
			peekProc((BYTE *) pModule + 0x1B5C5EF, &in_game, 1) && // 0 when loading or not in-game, 1 when in-game.
			peekProc((BYTE *) pModule + 0x1B9E620, avatar_pos_corrector, 12) && // Avatar Position values (Z, X and Y).
			peekProc((BYTE *) pModule + 0x1BEDBE0, camera_pos_corrector, 12) && // Camera Position values (Z, X and Y).
			peekProc((BYTE *) avatar_base + 0x70, avatar_front_corrector, 12) && // Avatar Front Vector values (Z, X and Y).
			peekProc((BYTE *) avatar_base + 0x80, avatar_top_corrector, 12) && // Avatar Top Vector values (Z, X and Y).
			peekProc((BYTE *) pModule + 0x1BEF6A0, camera_front_corrector, 12) && // Camera Front Vector values (Z, X and Y).
			peekProc((BYTE *) pModule + 0x1B6F710, camera_top_corrector, 12) && // Camera Top Vector values (Z, X and Y).
			peekProc((BYTE *) pModule + 0x265857C, player) && // Player nickname.
			peekProc((BYTE *) pModule + 0x22506D0, vehicle) && // Vehicle name if in a vehicle, empty if not.
			peekProc((BYTE *) pModule + 0x22500AB, location) && // Location name.
			peekProc((BYTE *) pModule + 0x224CE70, street); // Street name if on a street, empty if not.

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from memory addresses.
	if (! ok)
		return false;

	// State
	if (state != 2 || in_game == 0) { // If not in-game
		context.clear(); // Clear context
		identity.clear(); // Clear identity
		// Set vectors values to 0.
		for (int i=0;i<3;i++)
			avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] =  camera_front[i] = camera_top[i] = 0.0f;

		return true; // This tells Mumble to ignore all vectors.
	}
	/*
	// Begin context
	std::ostringstream ocontext;

	// Host
	host[sizeof(host)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(host);
	if (strcmp(host, "") != 0 && strstr(host, "127.0.0.1") == NULL) { // Set host string as empty if "127.0.0.1" is found in it.
		ocontext << " {\"Host\": \"" << host << "\"}"; // Set context with IP address and port
	}

	context = ocontext.str();
	// End context
	*/
	// Begin identity
	std::wostringstream oidentity;
	oidentity << "{";

	// Player
	player[sizeof(player)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(player);
	if (strcmp(player, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Player\": \"" << player << "\","; // Set player nickname in identity.
	} else {
		oidentity << std::endl << "\"Player\": null,";
	}

	// Vehicle
	vehicle[sizeof(vehicle)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(vehicle);
	if (strcmp(vehicle, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Vehicle\": \"" << vehicle << "\","; // Set vehicle name in identity.
	} else {
		oidentity << std::endl << "\"Vehicle\": null,";
	}

	// Location
	location[sizeof(location)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(location);
	if (strcmp(location, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Location\": \"" << location << "\","; // Set location name in identity.
	} else {
		oidentity << std::endl << "\"Location\": null,";
	}

	// Street
	street[sizeof(street)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(street);
	if (strcmp(street, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Street\": \"" << street << "\","; // Set street name in identity.
	} else {
		oidentity << std::endl << "\"Street\": null";
	}

	oidentity << std::endl << "}";
	identity = oidentity.str();
	// End identity

	/*
	Mumble | Game
	X      | Z
	Y      | X
	Z      | Y
	*/
	avatar_pos[0] = avatar_pos_corrector[1];
	avatar_pos[1] = avatar_pos_corrector[2];
	avatar_pos[2] = avatar_pos_corrector[0];

	camera_pos[0] = camera_pos_corrector[1];
	camera_pos[1] = camera_pos_corrector[2];
	camera_pos[2] = camera_pos_corrector[0];

	avatar_front[0] = avatar_front_corrector[1];
	avatar_front[1] = avatar_front_corrector[2];
	avatar_front[2] = avatar_front_corrector[0];

	avatar_top[0] = avatar_top_corrector[1];
	avatar_top[1] = avatar_top_corrector[2];
	avatar_top[2] = avatar_top_corrector[0];

	camera_front[0] = camera_front_corrector[1];
	camera_front[1] = camera_front_corrector[2];
	camera_front[2] = camera_front_corrector[0];

	camera_top[0] = camera_top_corrector[1];
	camera_top[1] = camera_top_corrector[2];
	camera_top[2] = camera_top_corrector[0];

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"GTA5.exe")) { // Link the game executable
		return false;
	}

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
	return std::wstring(L"Supports Grand Theft Auto V with identity support."); // Plugin long description
}

static std::wstring description(L"Grand Theft Auto V version 1.33"); // Plugin short description
static std::wstring shortname(L"Grand Theft Auto V"); // Plugin short name

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin gtavplug = {
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

static MumblePlugin2 gtavplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &gtavplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &gtavplug2;
}
