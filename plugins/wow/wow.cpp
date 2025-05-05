// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"  // Include standard positional audio header.
#include "mumble_positional_audio_utils.h" // Include positional audio header for special functions, like "escape".

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &identity) {
	for (int i = 0; i < 3; i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	// To update visit
	// http://www.ownedcore.com/forums/world-of-warcraft/world-of-warcraft-bots-programs/wow-memory-editing and look for
	// a thread called "[WoW] [Version] Release Info Dump Thread".
	// http://www.ownedcore.com/forums/world-of-warcraft/world-of-warcraft-bots-programs/wow-memory-editing/585582-wow-7-0-3-22624-release-info-dump-thread.html#post3615091

	// Avatar pointer
	procptr_t avatar_offset = peekProcPtr(pModule + 0xDE565C); // "LocalPlayer" in the thread.
	if (!avatar_offset)
		return false;

	// Camera pointer
	procptr_t camera_base = peekProcPtr(pModule + 0xEBA11C); // "CameraStruct" in the thread.
	if (!camera_base)
		return false;
	procptr_t camera_offset = peekProcPtr(camera_base + 0x3264); // "CameraOffset" in the thread.
	if (!camera_offset)
		return false;

	// Realm pointer
	procptr_t realm_offset = peekProcPtr(pModule + 0xF3AF7C); // Not available in the thread.
	if (!realm_offset)
		return false;

	// Memory addresses
	procptr_t state_address          = 0xE5BD19; // "GameState" in the thread.
	procptr_t avatar_pos_address     = 0xAF8;    // "UnitOrigin" in the thread.
	procptr_t camera_pos_address     = 0x08;     // "CameraOrigin" in the thread.
	procptr_t camera_front_address   = 0x14;     // "CameraMatrix" in the thread.
	procptr_t camera_top_address     = 0x2C;     // "CameraMatrix" + 0x18.
	procptr_t avatar_heading_address = 0xB08;    // "UnitAngle" in the thread.
	procptr_t realm_address          = 0x384;    // Not available in the thread.
	procptr_t player_address         = 0xF3B288; // Not available in the thread.

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], avatar_heading, camera_front_corrector[3],
		camera_top_corrector[3];
	// Char values for extra features
	char state, player[50], realm[50];

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(pModule + state_address, &state, 1) && // Magical state value: 1 when in-game, 0 when not.
		 peekProc(avatar_offset + avatar_pos_address, avatar_pos_corrector, 12)
		 && // Avatar Position values (Z, -X and Y).
		 peekProc(camera_offset + camera_pos_address, camera_pos_corrector, 12)
		 &&                                                                  // Camera Position values (Z, -X and Y).
		 peekProc(avatar_offset + avatar_heading_address, avatar_heading) && // Avatar heading.
		 peekProc(camera_offset + camera_front_address, camera_front_corrector, 12)
		 && // Camera Front Vector values (Z, -X and Y).
		 peekProc(camera_offset + camera_top_address, camera_top_corrector, 12)
		 &&                                               // Camera Top Vector values (Z, -X and Y).
		 peekProc(realm_offset + realm_address, realm) && // Realm name.
		 peekProc(pModule + player_address, player);      // Player nickname.

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from
	// memory addresses.
	if (!ok)
		return false;

	// State
	if (state != 1) {     // If not in-game
		context.clear();  // Clear context
		identity.clear(); // Clear identity
		// Set vectors values to 0.
		for (int i = 0; i < 3; i++)
			avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

		return true; // This tells Mumble to ignore all vectors.
	}

	// Begin context
	std::ostringstream ocontext;

	// Realm
	escape(realm, sizeof(realm));
	if (strcmp(realm, "") != 0) {                        // Set Realm string only if not empty
		ocontext << " {\"Realm\": \"" << realm << "\"}"; // Set context with Realm's name
	}

	context = ocontext.str();
	// End context

	// Begin identity
	std::wostringstream oidentity;
	oidentity << "{";

	// Player
	escape(player, sizeof(player));
	if (strcmp(player, "") != 0) {
		oidentity << std::endl;
		oidentity << "\"Player\": \"" << player << "\""; // Set player nickname in identity.
	} else {
		oidentity << std::endl << "\"Player\": null";
	}

	oidentity << std::endl << "}";
	identity = oidentity.str();
	// End identity

	/*
	Mumble | Game
	X      | Z
	Y      | -X
	Z      | Y
	*/
	avatar_pos[0] = -avatar_pos_corrector[1];
	avatar_pos[1] = avatar_pos_corrector[2];
	avatar_pos[2] = avatar_pos_corrector[0];

	camera_pos[0] = -camera_pos_corrector[1];
	camera_pos[1] = camera_pos_corrector[2];
	camera_pos[2] = camera_pos_corrector[0];

	avatar_front[0] = -sin(avatar_heading);
	avatar_front[1] = 0.0f;
	avatar_front[2] = cos(avatar_heading);

	avatar_top[2] = -1; // Dummy top vector, you can't tilt your head sideways in WoW.

	camera_front[0] = -camera_front_corrector[1];
	camera_front[1] = camera_front_corrector[2];
	camera_front[2] = camera_front_corrector[0];

	camera_top[0] = -camera_top_corrector[1];
	camera_top[1] = camera_top_corrector[2];
	camera_top[2] = camera_top_corrector[0];

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"Wow.exe")) { // Retrieve game executable's memory address
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
	return std::wstring(
		L"Supports World of Warcraft (x86) with context and identity support."); // Plugin long description
}

static std::wstring description(L"World of Warcraft (x86) version 7.0.3.22810"); // Plugin short description
static std::wstring shortname(L"World of Warcraft");                             // Plugin short name

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin wowplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								generic_unlock,      longdesc,    fetch };

static MumblePlugin2 wowplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &wowplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &wowplug2;
}
