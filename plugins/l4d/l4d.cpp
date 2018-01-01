// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h" // Include standard plugin header.
#include "../mumble_plugin_utils.h" // Include plugin header for special functions, like "escape".

static procptr_t steamclient, engine; // Variables to contain modules addresses

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], camera_front_corrector[3], camera_top_corrector[3];
	// Char values for extra features
	char serverid[22], host[22], map[30], playerid[22];
	// State
	bool state;

	// State pointers
	procptr_t state_base = peekProcPtr(pModule + 0x5A7354);
	if (state_base == 0) return false;
	procptr_t state_offset = peekProcPtr(state_base + 0x5C);
	if (state_offset == 0) return false;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(state_offset + 0x8, state) && // Magical state value: 0 when not playing and 1 when in-game.
			peekProc(pModule + 0x512264, avatar_pos_corrector) && // Avatar position values (X, Z and Y).
			peekProc(pModule + 0x5943B0, camera_pos_corrector) && // Camera position values (X, Z and Y).
			peekProc(pModule + 0x594410, camera_front_corrector) && // Front vector values (X, Z and Y).
			peekProc(pModule + 0x594440, camera_top_corrector) && // Top vector values (X, Z and Y).
			peekProc(steamclient + 0x95E56D, serverid) && // Unique server Steam ID.
			peekProc(engine + 0x61F5D8, host) && // Server value: "IP:Port" (xxx.xxx.xxx.xxx:yyyyy) when in a remote server and "loopback" when on a local server.
			peekProc(pModule + 0x5A7A29, map) && // Map name.
			peekProc(engine + 0x40CF00, playerid); // Unique player Steam ID.

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from memory addresses.
	if (! ok)
		return false;

	// State
	if (!state) { // If not in-game
		context.clear(); // Clear context
		identity.clear(); // Clear identity

		// Set vectors values to 0.
		for (int i=0;i<3;i++) {
			avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] =  camera_front[i] = camera_top[i] = 0.0f;
		}

		return true; // This tells Mumble to ignore all vectors.
	}

	// Begin context
	escape(serverid, sizeof(serverid));
	escape(host, sizeof(host));
	std::ostringstream ocontext;
	if (strcmp(serverid, "") != 0) {
		ocontext << " {\"Server ID\": \"" << serverid << "\"}"; // Set context with server ID
	} else {
		ocontext << " {\"Host\": \"" << host << "\"}"; // Set context with IP address and port
	}

	context = ocontext.str();
	// End context

	// Begin identity
	std::wostringstream oidentity;
	oidentity << "{";

	// Host
	if (strcmp(host, "") != 0 && strstr(host, "loopback") == NULL) { // Only include host (IP:Port) if it is not empty and does not include the string "loopback" (which means it's a local server).
		oidentity << std::endl << "\"Host\": \"" << host << "\","; // Set host address in identity.
	} else {
		oidentity << std::endl << "\"Host\": null,";
	}

	// Map
	escape(map, sizeof(map));
	if (strcmp(map, "") != 0) {
		oidentity << std::endl << "\"Map\": \"" << map << "\","; // Set map name in identity.
	} else {
		oidentity << std::endl << "\"Map\": null,";
	}

	// Player ID
	escape(playerid, sizeof(playerid));
	if (strcmp(playerid, "") != 0) {
		oidentity << std::endl << "\"Player ID\": \"" << playerid << "\""; // Set player ID in identity.
	} else {
		oidentity << std::endl << "\"Player ID\": null";
	}

	oidentity << std::endl << "}";
	identity = oidentity.str();
	// End identity

	/*
	Mumble | Game
	X      | X
	Y      | Z
	Z      | Y
	*/
	avatar_pos[0] = avatar_pos_corrector[0];
	avatar_pos[1] = avatar_pos_corrector[2];
	avatar_pos[2] = avatar_pos_corrector[1];

	camera_pos[0] = camera_pos_corrector[0];
	camera_pos[1] = camera_pos_corrector[2];
	camera_pos[2] = camera_pos_corrector[1];

	camera_front[0] = camera_front_corrector[0];
	camera_front[1] = camera_front_corrector[2];
	camera_front[2] = camera_front_corrector[1];

	camera_top[0] = camera_top_corrector[0];
	camera_top[1] = camera_top_corrector[2];
	camera_top[2] = camera_top_corrector[1];

	// Convert from inches to meters and sync avatar vectors with camera ones
	for (int i=0;i<3;i++) {
		avatar_pos[i]/=39.37f;
		camera_pos[i]/=39.37f;
		avatar_front[i] = camera_front[i];
		avatar_top[i] = camera_top[i];
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"left4dead.exe", L"client.dll")) { // Retrieve "client.dll" module's memory address
		return false;
	}

	// Server ID
	steamclient = getModuleAddr(L"steamclient.dll"); // Retrieve "steamclient.dll" module's memory address
	// This prevents the plugin from linking to the game in case something goes wrong during module's memory address retrieval.
	if (steamclient == 0)
		return false;

	// Host & Player ID
	engine = getModuleAddr(L"engine.dll"); // Retrieve "engine.dll" module's memory address
	// This prevents the plugin from linking to the game in case something goes wrong during module's memory address retrieval.
	if (engine == 0)
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
	return std::wstring(L"Supports Left 4 Dead version 1.0.3.1 with context and identity support."); // Plugin long description
}

static std::wstring description(L"Left 4 Dead (v1.0.3.1)"); // Plugin short description
static std::wstring shortname(L"Left 4 Dead"); // Plugin short name

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin l4dplug = {
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

static MumblePlugin2 l4dplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &l4dplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &l4dplug2;
}
