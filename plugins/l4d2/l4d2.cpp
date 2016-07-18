// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32_x86.h" // Include standard plugin header.
#include "../mumble_plugin_utils.h" // Include plugin header for special functions, like "escape".

procptr32_t serverid_steamclient, player_engine; // BYTE values to contain modules addresses

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], avatar_front_corrector[3], avatar_top_corrector[3];
	// Char values for extra features
	char serverid[22], host[22], servername[50], map[30], player[33];
	// State
	BYTE state;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(pModule + 0x6ACBD5, &state, 1) && // Magical state value: 0 or 255 when in main menu and 1 when in-game.
			peekProc(pModule + 0x6B9E1C, avatar_pos_corrector, 12) && // Avatar Position values (X, Z and Y).
			peekProc(pModule + 0x774B98, camera_pos_corrector, 12) && // Camera Position values (X, Z and Y).
			peekProc(pModule + 0x774BF8, avatar_front_corrector, 12) && // Front vector values (X, Z and Y).
			peekProc(pModule + 0x774C28, avatar_top_corrector, 12) && // Top vector values (Z, X and Y).
			peekProc(serverid_steamclient, serverid) && // Unique server Steam ID.
			peekProc(pModule + 0x772B24, host) && // Server value: "IP:Port" (xxx.xxx.xxx.xxx:yyyyy) when in a remote server, "loopback:0" when on a local server and empty when not playing.
			peekProc(pModule + 0x772D2C, servername) && // Server name.
			peekProc(pModule + 0x772C28, map) && // Map name.
			peekProc(player_engine, player); // Player nickname.

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from memory addresses.
	if (! ok)
		return false;

	// State
	if (state != 1) { // If not in-game
		context.clear(); // Clear context
		identity.clear(); // Clear identity
		// Set vectors values to 0.
		for (int i=0;i<3;i++) {
			avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] =  camera_front[i] = camera_top[i] = 0.0f;
		}

		return true; // This tells Mumble to ignore all vectors.
	}

	// Begin context
	serverid[sizeof(serverid)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(serverid);
	std::ostringstream ocontext;
	if (strcmp(serverid, "") != 0) {
		ocontext << " {\"Server ID\": \"" << serverid << "\"}"; // Set context with IP address and port
	}

	context = ocontext.str();
	// End context

	// Begin identity
	std::wostringstream oidentity;
	oidentity << "{";

	// Host
	host[sizeof(host)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(host);
	if (strcmp(host, "") != 0 && strstr(host, "loopback") == NULL) { // Only include host (IP:Port) if it is not empty and does not include the string "loopback" (which means it's a local server).
		oidentity << std::endl << "\"Host\": \"" << host << "\","; // Set host address in identity.
	} else {
		oidentity << std::endl << "\"Host\": null,";
	}

	// Server name
	servername[sizeof(servername)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(servername);
	if (strcmp(servername, "") != 0) {
		oidentity << std::endl << "\"Server name\": \"" << servername << "\","; // Set server name in identity.
	} else {
		oidentity << std::endl << "\"Server name\": null,";
	}

	// Map
	map[sizeof(map)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(map);
	if (strcmp(map, "") != 0) {
		oidentity << std::endl << "\"Map\": \"" << map << "\","; // Set map name in identity.
	} else {
		oidentity << std::endl << "\"Map\": null,";
	}

	// Player
	player[sizeof(player)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
	escape(player);
	if (strcmp(player, "") != 0) {
		oidentity << std::endl << "\"Player\": \"" << player << "\""; // Set player nickname in identity.
	} else {
		oidentity << std::endl << "\"Player\": null";
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

	avatar_front[0] = avatar_front_corrector[0];
	avatar_front[1] = avatar_front_corrector[2];
	avatar_front[2] = avatar_front_corrector[1];

	avatar_top[0] = avatar_top_corrector[0];
	avatar_top[1] = avatar_top_corrector[2];
	avatar_top[2] = avatar_top_corrector[1];

	// Convert from inches to meters and sync camera vectors with avatar ones
	for (int i=0;i<3;i++) {
		avatar_pos[i]/=39.37f;
		camera_pos[i]/=39.37f;
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"left4dead2.exe", L"client.dll")) { // Link the game executable
		return false;
	}

	procptr32_t steamclient=getModuleAddr(L"steamclient.dll"); // Link "steamclient.dll" module
	// This prevents the plugin from linking to the game in case something goes wrong during module linking.
	if (!steamclient)
		return false;

	serverid_steamclient = steamclient + 0x94D9ED; // Module + Server ID offset

	procptr32_t engine=getModuleAddr(L"engine.dll"); // // Link "engine.dll" module
	// This prevents the plugin from linking to the game in case something goes wrong during module linking.
	if (!engine)
		return false;

	player_engine = engine + 0x6795D1; // Module + Player offset

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
	return std::wstring(L"Supports Left 4 Dead 2 version 2.1.4.6 with context and identity support."); // Plugin long description
}

static std::wstring description(L"Left 4 Dead 2 (v2.1.4.6)"); // Plugin short description
static std::wstring shortname(L"Left 4 Dead 2"); // Plugin short name

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin l4d2plug = {
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

static MumblePlugin2 l4d2plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &l4d2plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &l4d2plug2;
}
