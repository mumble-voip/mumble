// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h" // Include standard plugin header.
#include "../mumble_plugin_utils.h" // Include plugin header for special functions, like "escape".

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	// Boolean values to check if game addresses retrieval is successful and if player is in-game
	bool ok, state, spec;
	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], viewHor, viewVer;
	// Char values for extra features
	char host[22], map[30];
	// Team
	BYTE team;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc((BYTE *) pModule + 0x0188248, &state, 1) && // Magical state value: 1 when in-game and 0 when in main menu.
			peekProc((BYTE *) pModule + 0x1041C68, &spec, 1) && // Spectator state value: 1 when spectating and 0 when playing.
			peekProc((BYTE *) pModule + 0x0EB8950, avatar_pos_corrector, 12) && // Avatar Position values (Z, X and Y, respectively).
			peekProc((BYTE *) pModule + 0x0E6093C, camera_pos_corrector, 12) && // Camera Position values (Z, X and Y, respectively).
			peekProc((BYTE *) pModule + 0x106CE04, &viewHor, 4) && // Changes in a range from 180 to -180 when moving the view to left/right.
			peekProc((BYTE *) pModule + 0x106CE00, &viewVer, 4) && // Changes in a range from 87.890625 (looking down) to -87.890625 (looking up).
			peekProc((BYTE *) pModule + 0x0E4A638, host) && // Server value: "IP:Port" when in a remote server, "loopback" when on a local server.
			peekProc((BYTE *) pModule + 0x12DE8D8, map) && // Map name.
			peekProc((BYTE *) pModule + 0x106CE6C, team); // Team value: 0 when in a FFA game (no team); 1 when in Red team; 2 when in Blue team; 3 when in Spectators.

	if (! ok) {
		return false;
	}

	if (! state) { // If not in-game
		context.clear(); // Clear context
		identity.clear(); // Clear identity

		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	}

	if (state && spec) { // If in-game as spectator
		// Set to 0 avatar and camera values.
		for (int i=0;i<3;i++) {
			avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] =  camera_front[i] = camera_top[i] = 0.0f;
		}
		// Set team to SPEC.
		std::wostringstream oidentity;
		oidentity << "{\"team\": \"SPEC\"}";
		identity = oidentity.str();

		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	}

	// Begin context
	host[sizeof(host)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(host);
	std::ostringstream ocontext;
	ocontext << " {";
	if (strcmp(host, "") != 0 && strstr(host, "loopback") == NULL) { // Only include host (IP:Port) if it is not empty and does not include the string "loopback" (which means it's a local server).
		ocontext << "\"Host\": \"" << host << "\""; // Set host address in identity.
	} else {
		ocontext << "\"Host\": null";
	}

	ocontext << "}";
	context = ocontext.str();
	// End context

	// Begin identity
	std::wostringstream oidentity;
	oidentity << "{";

	// Map
	map[sizeof(map)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(map);
	if (strcmp(map, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Map\": \"" << map << "\","; // Set map name in identity.
	} else {
		oidentity << std::endl << "\"Map\": null,";
	}

	// Team
	if (team >= 0 && team <= 3) {
		if (team == 0)
			oidentity << std::endl << "\"Team\": \"FFA\""; // If team value is 0, set "FFA" as team in identity.
		else if (team == 1)
			oidentity << std::endl << "\"Team\": \"Red\""; // If team value is 1, set "Red" as team in identity.
		else if (team == 2)
			oidentity << std::endl << "\"Team\": \"Blue\""; // If team value is 2, set "Blue" as team in identity.
		else if (team == 3)
			oidentity << std::endl << "\"Team\": \"Spectators\""; // If team value is 3, set "Spectators" as team in identity.
	} else {
		oidentity << std::endl << "\"Team\": null";
	}

	oidentity << std::endl << "}";
	identity = oidentity.str();
	// End identity

	/*
	Mumble | Game
	X      | Y
	Y      | Z
	Z      | X
	*/
	avatar_pos[0] = avatar_pos_corrector[1];
	avatar_pos[1] = avatar_pos_corrector[2];
	avatar_pos[2] = avatar_pos_corrector[0];

	camera_pos[0] = camera_pos_corrector[1];
	camera_pos[1] = camera_pos_corrector[2];
	camera_pos[2] = camera_pos_corrector[0];

	// Calculate view unit vector
	viewVer *= static_cast<float>(M_PI / 180.0f);
	viewHor *= static_cast<float>(M_PI / 180.0f);

	avatar_front[0] = camera_front[0] = cos(viewVer) * cos(viewHor);
	avatar_front[1] = camera_front[1] = -sin(viewVer);
	avatar_front[2] = camera_front[2] = cos(viewVer) * sin(viewHor);

	avatar_top[2] = camera_top[2] = -1; // This tells Mumble to automatically calculate top vector using front vector.

	// Scale to meters
	for (int i=0;i<3;i++) {
		avatar_pos[i]/=70.0f;
		camera_pos[i]/=70.0f;
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"quakelive_steam.exe")) { // Retrieve game executable's memory address
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
	return std::wstring(L"Supports Quake Live version 1069 with context and identity support."); // Plugin long description
}

static std::wstring description(L"Quake Live (v1069)"); // Plugin short description
static std::wstring shortname(L"Quake Live"); // Plugin short name

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin qlplug = {
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

static MumblePlugin2 qlplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &qlplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &qlplug2;
}
