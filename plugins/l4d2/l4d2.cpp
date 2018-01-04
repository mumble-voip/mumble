// Copyright 2005-2018 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifdef WIN32
#include "../mumble_plugin_win32.h" // Include standard plugin header.
#else
#include "../mumble_plugin_linux.h" // Include standard plugin header.
#endif

#include "../mumble_plugin_utils.h" // Include plugin header for special functions, like "escape".

// Variables to contain modules addresses
procptr_t steamclient = 0;
procptr_t server = 0;
procptr_t engine = 0;

#ifdef WIN32
// Memory offsets
const procptr_t state_offset				= 0x6ACBD5;
const procptr_t avatar_pos_offset			= 0x6B9E1C;
const procptr_t camera_pos_offset			= 0x774B98;
const procptr_t avatar_front_offset			= 0x774BF8;
const procptr_t avatar_top_offset			= 0x774C28;
const procptr_t host_offset					= 0x772B24;
const procptr_t servername_offset			= 0x772D2C;
const procptr_t map_offset					= 0x772C28;
const procptr_t serverid_steamclient_offset	= 0x95E56D;
const procptr_t player_server_offset		= 0x7F87BC;
const procptr_t playerid_engine_offset		= 0x4EBF88;
// Module names
const wchar_t *exe_name						= L"left4dead2.exe";
const wchar_t *client_name					= L"client.dll";
const wchar_t *steamclient_name				= L"steamclient.dll";
const wchar_t *server_name					= L"server.dll";
const wchar_t *engine_name					= L"engine.dll";
#else
// Memory offsets
const procptr_t state_offset				= 0xE0A24C;
const procptr_t avatar_pos_offset			= 0xE773FC;
const procptr_t camera_pos_offset			= 0xED8700;
const procptr_t avatar_front_offset			= 0xE3C138;
const procptr_t avatar_top_offset			= 0xE3C150;
const procptr_t host_offset					= 0xE356D0;
const procptr_t servername_offset			= 0xE358D8;
const procptr_t map_offset					= 0xE09E9D;
const procptr_t serverid_steamclient_offset	= 0x1216CA5;
const procptr_t player_server_offset		= 0xF340E4;
const procptr_t playerid_engine_offset		= 0xA62C60;
// Module names
const wchar_t *exe_name						= L"hl2_linux";
const wchar_t *client_name					= L"client.so";
const wchar_t *steamclient_name				= L"steamclient.so";
const wchar_t *server_name					= L"server.so";
const wchar_t *engine_name					= L"engine.so";
#endif

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], avatar_front_corrector[3], avatar_top_corrector[3];
	// Char values for extra features
	char serverid[22], host[22], servername[50], map[30], player[33], playerid[22];
	// State
	unsigned char state;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(pModule + state_offset, &state, 1) && // Magical state value: 0 or 255 when in main menu and 1 when in-game.
			peekProc(pModule + avatar_pos_offset, avatar_pos_corrector, 12)		&& // Avatar Position values (X, Z and Y).
			peekProc(pModule + camera_pos_offset, camera_pos_corrector, 12)		&& // Camera Position values (X, Z and Y).
			peekProc(pModule + avatar_front_offset, avatar_front_corrector, 12)	&& // Front vector values (X, Z and Y).
			peekProc(pModule + avatar_top_offset, avatar_top_corrector, 12)		&& // Top vector values (Z, X and Y).
			peekProc(steamclient + serverid_steamclient_offset, serverid)		&& // Unique server Steam ID.
			peekProc(pModule + host_offset, host)								&& // Server value: "IP:Port" (xxx.xxx.xxx.xxx:yyyyy) when in a remote server, "loopback:0" when on a local server and empty when not playing.
			peekProc(pModule + servername_offset, servername)					&& // Server name.
			peekProc(pModule + map_offset, map)									&& // Map name.
			peekProc(server + player_server_offset, player)						&& // Player nickname.
			peekProc(engine + playerid_engine_offset, playerid);				   // Unique player Steam ID.

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
	escape(serverid, sizeof(serverid));
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
	escape(host, sizeof(host));
	if (strcmp(host, "") != 0 && strstr(host, "loopback") == NULL) { // Only include host (IP:Port) if it is not empty and does not include the string "loopback" (which means it's a local server).
		oidentity << std::endl << "\"Host\": \"" << host << "\","; // Set host address in identity.
	} else {
		oidentity << std::endl << "\"Host\": null,";
	}

	// Server name
	escape(servername, sizeof(servername));
	if (strcmp(servername, "") != 0) {
		oidentity << std::endl << "\"Server name\": \"" << servername << "\","; // Set server name in identity.
	} else {
		oidentity << std::endl << "\"Server name\": null,";
	}

	// Map
	escape(map, sizeof(map));
	if (strcmp(map, "") != 0) {
		oidentity << std::endl << "\"Map\": \"" << map << "\","; // Set map name in identity.
	} else {
		oidentity << std::endl << "\"Map\": null,";
	}

	// Player nickname
	escape(player, sizeof(player));
	if (strcmp(player, "") != 0) {
		oidentity << std::endl << "\"Player\": \"" << player << "\","; // Set player nickname in identity.
	} else {
		oidentity << std::endl << "\"Player\": null,";
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

	if (! initialize(pids, exe_name, client_name)) { // Retrieve "client_name" module's memory address
		return false;
	}

	// Server ID
	steamclient = getModuleAddr(steamclient_name); // Retrieve "steamclient_name" module's memory address
	// This prevents the plugin from linking to the game in case something goes wrong during module's memory address retrieval.
	if (!steamclient)
		return false;

	// Player name
	server = getModuleAddr(server_name); // Retrieve "server_name" module's memory address
	// This prevents the plugin from linking to the game in case something goes wrong during module's memory address retrieval.
	if (!server)
		return false;

	// Player ID
	engine = getModuleAddr(engine_name); // Retrieve "engine_name" module's memory address
	// This prevents the plugin from linking to the game in case something goes wrong during module's memory address retrieval.
	if (!engine)
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

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &l4d2plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &l4d2plug2;
}
