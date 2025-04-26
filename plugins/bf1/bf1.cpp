// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"  // Include standard positional audio header
#include "mumble_positional_audio_utils.h" // Include positional audio header for special functions, like "escape".

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &identity) {
	for (int i = 0; i < 3; i++) {
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
	}

	bool ok, state;
	char server_name[100], team[4];
	uint8_t squad, squad_leader;

	// State pointers
	procptr_t state_base = peekProcPtr(pModule + 0x33DBD08);
	if (!state_base)
		return false;
	procptr_t state_offset_0 = peekProcPtr(state_base + 0x5F0);
	if (!state_offset_0)
		return false;
	procptr_t state_offset_1 = peekProcPtr(state_offset_0 + 0x488);
	if (!state_offset_1)
		return false;
	procptr_t state_offset_2 = peekProcPtr(state_offset_1 + 0x8);
	if (!state_offset_2)
		return false;
	procptr_t state_offset_3 = peekProcPtr(state_offset_2 + 0x8);
	if (!state_offset_3)
		return false;

	// Camera pointer
	procptr_t camera_base = peekProcPtr(pModule + 0x368E828);
	if (!camera_base)
		return false;

	// Server name pointers
	procptr_t server_name_base = peekProcPtr(pModule + 0x3103A38);
	if (!server_name_base)
		return false;
	procptr_t server_name_offset_0 = peekProcPtr(server_name_base + 0x738);
	if (!server_name_offset_0)
		return false;
	procptr_t server_name_offset_1 = peekProcPtr(server_name_offset_0 + 0x370);
	if (!server_name_offset_1)
		return false;
	procptr_t server_name_offset_2 = peekProcPtr(server_name_offset_1 + 0x10);
	if (!server_name_offset_2)
		return false;

	// Team pointers
	procptr_t team_base = peekProcPtr(pModule + 0x36CC128);
	if (!team_base)
		return false;
	procptr_t team_offset_0 = peekProcPtr(team_base + 0x20);
	if (!team_offset_0)
		return false;
	procptr_t team_offset_1 = peekProcPtr(team_offset_0 + 0x648);
	if (!team_offset_1)
		return false;
	procptr_t team_offset_2 = peekProcPtr(team_offset_1 + 0xF8);
	if (!team_offset_2)
		return false;

	// Squad pointers
	procptr_t squad_base = peekProcPtr(pModule + 0x31C3E40);
	if (!squad_base)
		return false;
	procptr_t squad_offset_0 = peekProcPtr(squad_base + 0x30);
	if (!squad_offset_0)
		return false;
	procptr_t squad_offset_1 = peekProcPtr(squad_offset_0 + 0x578);
	if (!squad_offset_1)
		return false;
	procptr_t squad_offset_2 = peekProcPtr(squad_offset_1 + 0xC0);
	if (!squad_offset_2)
		return false;
	procptr_t squad_offset_3 = peekProcPtr(squad_offset_2 + 0x58);
	if (!squad_offset_3)
		return false;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data

	// Magical state value: 1 when in-game and 0 when not spawned or playing.
	ok = peekProc(state_offset_3 + 0x50, &state, 1)
		 // Avatar position values (X, Y and Z).
		 && peekProc(pModule + 0x36B1500, avatar_pos, 12)
		 // Camera position values (X, Y and Z).
		 && peekProc(camera_base + 0x2B0, camera_pos, 12)
		 // Avatar front vector values (X, Y and Z).
		 && peekProc(camera_base + 0x260, camera_front, 12)
		 // Avatar top vector values (X, Y and Z).
		 && peekProc(camera_base + 0x250, camera_top, 12)
		 // Server name.
		 && peekProc(server_name_offset_2, server_name)
		 // Team name.
		 && peekProc(team_offset_2 + 0x13, team)
		 // Squad value: 0 (not in a squad), 1 (Apples), 2 (Butter), 3 (Charlie)... 26 (Zebra).
		 && peekProc(squad_offset_3 + 0x240, squad)
		 // Squad leader value: 0 (False), 1 (True).
		 && peekProc(squad_offset_3 + 0x244, squad_leader);

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from
	// memory addresses.
	if (!ok) {
		return false;
	}

	if (!state) {         // If not in-game
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

	// Server name
	escape(server_name, sizeof(server_name));
	if (strcmp(server_name, "") != 0) {
		ocontext << " {\"Server name\": \"" << server_name << "\"}"; // Set context with server name.
	}

	context = ocontext.str();
	// End context

	// Begin identity
	std::wostringstream oidentity;
	oidentity << "{";

	// Team
	escape(team, sizeof(team));
	if (strcmp(team, "") != 0) {
		oidentity << std::endl << "\"Team\": \"" << team << "\","; // Set team name in identity.
	}

	// Squad
	if (squad > 0 && squad < 27) { // If squad value is between 1 and 26, set squad name in identity using RAF (1917)
								   // Phonetic alphabet.
		// Squad name
		if (squad == 1)
			oidentity << std::endl << "\"Squad\": \"Apples\",";
		else if (squad == 2)
			oidentity << std::endl << "\"Squad\": \"Butter\",";
		else if (squad == 3)
			oidentity << std::endl << "\"Squad\": \"Charlie\",";
		else if (squad == 4)
			oidentity << std::endl << "\"Squad\": \"Duff\",";
		else if (squad == 5)
			oidentity << std::endl << "\"Squad\": \"Edward\",";
		else if (squad == 6)
			oidentity << std::endl << "\"Squad\": \"Freddy\",";
		else if (squad == 7)
			oidentity << std::endl << "\"Squad\": \"George\",";
		else if (squad == 8)
			oidentity << std::endl << "\"Squad\": \"Harry\",";
		else if (squad == 9)
			oidentity << std::endl << "\"Squad\": \"Ink\",";
		else if (squad == 10)
			oidentity << std::endl << "\"Squad\": \"Johnnie\",";
		else if (squad == 11)
			oidentity << std::endl << "\"Squad\": \"King\",";
		else if (squad == 12)
			oidentity << std::endl << "\"Squad\": \"London\",";
		else if (squad == 13)
			oidentity << std::endl << "\"Squad\": \"Monkey\",";
		else if (squad == 14)
			oidentity << std::endl << "\"Squad\": \"Nuts\",";
		else if (squad == 15)
			oidentity << std::endl << "\"Squad\": \"Orange\",";
		else if (squad == 16)
			oidentity << std::endl << "\"Squad\": \"Pudding\",";
		else if (squad == 17)
			oidentity << std::endl << "\"Squad\": \"Queenie\",";
		else if (squad == 18)
			oidentity << std::endl << "\"Squad\": \"Robert\",";
		else if (squad == 19)
			oidentity << std::endl << "\"Squad\": \"Sugar\",";
		else if (squad == 20)
			oidentity << std::endl << "\"Squad\": \"Tommy\",";
		else if (squad == 21)
			oidentity << std::endl << "\"Squad\": \"Uncle\",";
		else if (squad == 22)
			oidentity << std::endl << "\"Squad\": \"Vinegar\",";
		else if (squad == 23)
			oidentity << std::endl << "\"Squad\": \"Willie\",";
		else if (squad == 24)
			oidentity << std::endl << "\"Squad\": \"Xerxes\",";
		else if (squad == 25)
			oidentity << std::endl << "\"Squad\": \"Yellow\",";
		else if (squad == 26)
			oidentity << std::endl << "\"Squad\": \"Zebra\",";
		// Squad leader
		if (squad_leader == 1)
			oidentity << std::endl
					  << "\"Squad leader\": true"; // If squad leader value is true, set squad leader state to "True" in
												   // identity.
		else
			oidentity << std::endl
					  << "\"Squad leader\": false"; // If squad leader value is false, set squad leader state to "False"
													// in identity.
													// When not in a squad
	} else {
		oidentity << std::endl
				  << "\"Squad\": null,"; // If squad value isn't between 1 and 26, set squad to "null" in identity.
		oidentity << std::endl
				  << "\"Squad leader\": null"; // If not in a squad, set squad leader state to "null" in identity.
	}

	oidentity << std::endl << "}";
	identity = oidentity.str();
	// End identity

	// Flip the front vector
	for (int i = 0; i < 3; i++) {
		camera_front[i] = -camera_front[i];
	}

	// Convert from right to left handed
	avatar_pos[0]   = -avatar_pos[0];
	camera_pos[0]   = -camera_pos[0];
	camera_front[0] = -camera_front[0];
	camera_top[0]   = -camera_top[0];

	// Sync avatar front and top vectors with camera ones
	for (int i = 0; i < 3; i++) {
		avatar_front[i] = camera_front[i];
		avatar_top[i]   = camera_top[i];
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"bf1.exe")) { // Retrieve game executable's memory address
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
	return std::wstring(L"Supports Battlefield 1 with context and identity support."); // Plugin long description
}

static std::wstring description(L"Battlefield 1 version 1.0.49.52296"); // Plugin short description
static std::wstring shortname(L"Battlefield 1");                        // Plugin short name

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin bf1plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								generic_unlock,      longdesc,    fetch };

static MumblePlugin2 bf1plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bf1plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &bf1plug2;
}
