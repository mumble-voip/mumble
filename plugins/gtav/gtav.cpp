// Copyright 2005-2019 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_main.h" // Include standard plugin header.
#include "../mumble_plugin_utils.h" // Include plugin header for special functions, like "escape".

#include <algorithm> // Include algorithm header for the game version detector

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	// Memory addresses
	char game_name[20];
	procptr_t state_address, in_game_address, avatar_pos_address, camera_pos_address, avatar_base_address, avatar_base, avatar_front_address, avatar_top_address, camera_front_address, camera_top_address, player_address, vehicle_address, location_address, street_address;

#define VERSION_EQ(buf, strlit) \
	memcmp(buf, strlit, std::min(sizeof(buf), sizeof(strlit)-1)) == 0

	// Steam version
	if (peekProc(pModule + 0x18AFBA8, game_name) && VERSION_EQ(game_name, "Grand Theft Auto V")) {
		state_address = pModule + 0x28ACB20;
		in_game_address = pModule + 0x25C7740;
		avatar_pos_address = pModule + 0x1F3E7A0;
		camera_pos_address = pModule + 0x1F3B110;
		avatar_front_address = pModule + 0x1FDDB50;
		avatar_top_address = pModule + 0x1FDDB40;
		camera_front_address = pModule + 0x1F3D990;
		camera_top_address = pModule + 0x1F3D980;
		player_address = pModule + 0x28B9E5C;
		vehicle_address = pModule + 0x2499C10;
		location_address = pModule + 0x249956B;
		street_address = pModule + 0x24963B0;
	// Retail version
	} else if (peekProc(pModule + 0x180D4D8, game_name) && VERSION_EQ(game_name, "Grand Theft Auto V")) {
		state_address = pModule + 0x2733490;
		in_game_address = pModule + 0x1BC6687;
		avatar_pos_address = pModule + 0x1F7EAA0;
		camera_pos_address = pModule + 0x1C58630;
		avatar_base_address = pModule + 0x1B956C0;
		// Avatar pointer
		avatar_base = peekProcPtr(avatar_base_address);
		if (!avatar_base) return false;
		avatar_front_address = avatar_base + 0x70;
		avatar_top_address = avatar_base + 0x80;
		camera_front_address = pModule + 0x1C5A0F0;
		camera_top_address = pModule + 0x1F7D9F0;
		player_address = pModule + 0x273DBAC;
		vehicle_address = pModule + 0x2331890;
		location_address = pModule + 0x233125B;
		street_address = pModule + 0x232DFA0;
	// Unknown version
	} else {
		generic_unlock();
		return false;
	}


	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], avatar_front_corrector[3], avatar_top_corrector[3], camera_front_corrector[3], camera_top_corrector[3];
	// Char values for extra features
	char state, in_game, player[50], vehicle[50], location[50], street[50];

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(state_address, &state, 1) && // Magical state value: 0 when in single player, 2 when online and 3 when in a lobby.
			peekProc(in_game_address, &in_game, 1) && // 0 when loading or not in-game, 1 when in-game.
			peekProc(avatar_pos_address, avatar_pos_corrector, 12) && // Avatar Position values (X, Z and Y).
			peekProc(camera_pos_address, camera_pos_corrector, 12) && // Camera Position values (X, Z and Y).
			peekProc(avatar_front_address, avatar_front_corrector, 12) && // Avatar Front Vector values (X, Z and Y).
			peekProc(avatar_top_address, avatar_top_corrector, 12) && // Avatar Top Vector values (X, Z and Y).
			peekProc(camera_front_address, camera_front_corrector, 12) && // Camera Front Vector values (X, Z and Y).
			peekProc(camera_top_address, camera_top_corrector, 12) && // Camera Top Vector values (X, Z and Y).
			peekProc(player_address, player) && // Player nickname.
			peekProc(vehicle_address, vehicle) && // Vehicle name if in a vehicle, empty if not.
			peekProc(location_address, location) && // Location name.
			peekProc(street_address, street); // Street name if on a street, empty if not.

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
	escape(player, sizeof(player));
	if (strcmp(player, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Player\": \"" << player << "\","; // Set player nickname in identity.
	} else {
		oidentity << std::endl << "\"Player\": null,";
	}

	// Vehicle
	escape(vehicle, sizeof(vehicle));
	if (strcmp(vehicle, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Vehicle\": \"" << vehicle << "\","; // Set vehicle name in identity.
	} else {
		oidentity << std::endl << "\"Vehicle\": null,";
	}

	// Location
	escape(location, sizeof(location));
	if (strcmp(location, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Location\": \"" << location << "\","; // Set location name in identity.
	} else {
		oidentity << std::endl << "\"Location\": null,";
	}

	// Street
	escape(street, sizeof(street));
	if (strcmp(street, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Street\": \"" << street << "\""; // Set street name in identity.
	} else {
		oidentity << std::endl << "\"Street\": null";
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

	camera_front[0] = camera_front_corrector[0];
	camera_front[1] = camera_front_corrector[2];
	camera_front[2] = camera_front_corrector[1];

	camera_top[0] = camera_top_corrector[0];
	camera_top[1] = camera_top_corrector[2];
	camera_top[2] = camera_top_corrector[1];

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"GTA5.exe")) { // Retrieve game executable's memory address
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
	return std::wstring(L"Supports Grand Theft Auto V version 1.48 (Steam) and 1.38 (Retail) with identity support."); // Plugin long description
}

static std::wstring description(L"Grand Theft Auto V (v1.48 [Steam] & v1.38 [Retail])"); // Plugin short description
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

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &gtavplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &gtavplug2;
}
