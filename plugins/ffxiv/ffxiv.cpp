// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"  // Include standard positional audio header.
#include "mumble_positional_audio_utils.h" // Include positional audio header for special functions, like "escape".

#include <cmath>

// Offset values can be obtained from:
//   http://xivapp.com/api/structures?patchVersion=latest&platform=x86
//   http://xivapp.com/api/structures?patchVersion=latest&platform=x64
// Y is North/South, South is increasing values
// X is East/West, West is increasing values
// Heading is in radians, 0 faces south

#ifdef FFXIV_USE_x64
// Memory offsets
const procptr_t camera_ptr    = 0x1673350;
const procptr_t avatar_ptr    = 0x1674950;
const procptr_t state_offset  = 0x1641990;
const procptr_t map_id_offset = 0x16409E8;
// Avatar struct offsets
const procptr_t identity_offset       = 48;  // Name
const procptr_t avatar_pos_offset     = 176; // X, Z, Y
const procptr_t avatar_azimuth_offset = 192; // Heading (-pi to pi)
// Camera struct offsets
const procptr_t camera_is_free_offset   = 272; // 0: First person mode; 1: 3rd person
const procptr_t camera_pos_offset       = 80;  // X, Z, Y
const procptr_t camera_azimuth_offset   = 304; // (-pi to pi)
const procptr_t camera_elevation_offset = 308; // (-pi to pi)
// Module names
const wchar_t *exe_name = L"ffxiv_dx11.exe";
// Plugin long description
static const std::wstring longdesc() {
	return std::wstring(
		L"Supports Final Fantasy XIV X64 version 2016.11.11.0000.0000 with context and identity support.");
}
// Plugin short description
static std::wstring description(L"Final Fantasy XIV X64 (2016.11.11.0000.0000)");
#else
// Memory offsets
const procptr_t camera_ptr    = 0x1045C40;
const procptr_t avatar_ptr    = 0x10468EC;
const procptr_t state_offset  = 0x1048C60;
const procptr_t map_id_offset = 0x10210B0;
// Avatar struct offsets
const procptr_t identity_offset       = 48;  // Name
const procptr_t avatar_pos_offset     = 160; // X, Z, Y
const procptr_t avatar_azimuth_offset = 176; // Heading (-pi to pi)
// Camera struct offsets
const procptr_t camera_is_free_offset   = 256; // 0: First person mode; 1: 3rd person
const procptr_t camera_pos_offset       = 64;  // X, Z, Y
const procptr_t camera_azimuth_offset   = 288; // (-pi to pi)
const procptr_t camera_elevation_offset = 292; // (-pi to pi)
// Module names
const wchar_t *exe_name = L"ffxiv.exe";
// Plugin long description
static const std::wstring longdesc() {
	return std::wstring(L"Supports Final Fantasy XIV version 2016.11.11.0000.0000 with context and identity support.");
}
// Plugin short description
static std::wstring description(L"Final Fantasy XIV (2016.11.11.0000.0000)");
#endif
// Plugin short name
static std::wstring shortname(L"Final Fantasy XIV");

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &identity) {
	for (int i = 0; i < 3; i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	// Boolean value to check if game addresses retrieval is successful
	bool ok;
	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float avatar_pos_corrector[3], camera_pos_corrector[3], avatar_azimuth, camera_azimuth, camera_elevation;
	// Values for extra features
	char player[64];
	unsigned int map_id;
	// State
	unsigned char state, camera_is_free;

	// Retrieve Avatar and Camera addresses
#ifdef FFXIV_USE_x64
	procptr_t avatar_address = peekProcPtr(pModule + avatar_ptr);
	procptr_t camera_address = peekProcPtr(pModule + camera_ptr);
#else
	procptr_t avatar_address = peekProcPtr(pModule + avatar_ptr);
	procptr_t camera_address = peekProcPtr(pModule + camera_ptr);
#endif
	if (!avatar_address || !camera_address)
		return false;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(pModule + state_offset, &state, 1)
		 && // Magical state value: 0 or 255 when in main menu and 1 when in-game.
		 peekProc(camera_address + camera_pos_offset, &camera_pos_corrector, 12)
		 &&                                                                      // Camera Position values (X, Z and Y).
		 peekProc(camera_address + camera_azimuth_offset, &camera_azimuth, 4) && // Camera azimuth float.
		 peekProc(camera_address + camera_elevation_offset, &camera_elevation, 4) && // Camera elevation float.
		 peekProc(camera_address + camera_is_free_offset, &camera_is_free, 1) &&     // Camera is in first person mode
		 peekProc(avatar_address + avatar_pos_offset, &avatar_pos_corrector, 12)
		 &&                                                                      // Avatar Position values (X, Z and Y).
		 peekProc(avatar_address + avatar_azimuth_offset, &avatar_azimuth, 4) && // Avatar azimuth float.
		 peekProc(pModule + map_id_offset, &map_id, 4) &&                        // Map id.
		 peekProc(avatar_address + identity_offset, player);                     // Player name.

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from
	// memory addresses.
	if (!ok)
		return false;

	// State
	if (state != 1) {     // If not in-game
		context.clear();  // Clear context
		identity.clear(); // Clear identity
		// Set vectors values to 0.
		for (int i = 0; i < 3; i++) {
			avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
		}

		return true; // This tells Mumble to ignore all vectors.
	}

	// Begin context
	std::ostringstream ocontext;
	ocontext << "{";

	// Map id
	ocontext << "Map: " << map_id << ""; // Set map id in identity.

	ocontext << "}";
	context = ocontext.str();
	// End context

	// Begin identity
	std::wostringstream oidentity;
	oidentity << "{";

	// Map id
	oidentity << "Map: " << map_id << ", "; // Set map id in identity.

	// Player name
	escape(player, sizeof(player));
	if (strcmp(player, "") != 0) {
		oidentity << "Player: \"" << player << "\""; // Set player name in identity.
	} else {
		oidentity << "Player: null";
	}

	oidentity << "}";
	identity = oidentity.str();
	// End identity

	/*
	Mumble | Game (in L hand coordinate system)
	X      | -X
	Y      | Y
	Z      | Z
	*/
	avatar_pos[0] = -avatar_pos_corrector[0];
	avatar_pos[1] = avatar_pos_corrector[1];
	avatar_pos[2] = avatar_pos_corrector[2];

	camera_pos[0] = -camera_pos_corrector[0];
	camera_pos[1] = camera_pos_corrector[1];
	camera_pos[2] = camera_pos_corrector[2];

	avatar_front[0] = static_cast< float >(std::sin(-avatar_azimuth));
	avatar_front[1] = 0.0f;
	avatar_front[2] = static_cast< float >(std::cos(-avatar_azimuth));

	camera_front[0] = static_cast< float >(std::cos(camera_elevation) * std::sin(-camera_azimuth));
	camera_front[1] = static_cast< float >(std::sin(camera_elevation));
	camera_front[2] = static_cast< float >(std::cos(camera_elevation) * std::cos(-camera_azimuth));

	if (camera_is_free) {
		camera_front[0] *= -1;
		camera_front[2] *= -1;
	}

	// Convert from yards (yalms) to meters
	for (int i = 0; i < 3; i++) {
		avatar_pos[i] *= 0.9144f;
		camera_pos[i] *= 0.9144f;
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, exe_name)) { // Retrieve "exe_name" module's memory address
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

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin ffxivplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								  generic_unlock,      longdesc,    fetch };

static MumblePlugin2 ffxivplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &ffxivplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &ffxivplug2;
}
