// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"  // Include standard positional audio header.
#include "mumble_positional_audio_utils.h" // Include positional audio header for special functions, like "escape".

// Variable to contain module's addresses
procptr_t RendDX9 = 0;


static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &identity) {
	bool ok;
	char server_name[100], soldier_name[100];
	uint8_t team_id, squad_id, is_commander, is_squad_leader, target_squad_id, on_voip, on_voip_com;

	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;


	// Server name pointers
	// Doubles as an in-server state variable (NULL if not in a game)
	procptr_t server_offset_0 = peekProcPtr(pModule + 0x61B5D8);
	if (!server_offset_0) {
		context.clear();  // Clear context
		identity.clear(); // Clear identity

		return true; // This tells Mumble to ignore all vectors.
	}

	procptr_t server_offset_1 = peekProcPtr(server_offset_0 + 0x108);
	if (!server_offset_1)
		return false;

	// Team ID pointers
	procptr_t team_offset_0 = peekProcPtr(pModule + 0x624430);
	if (!team_offset_0)
		return false;
	procptr_t team_offset_1 = peekProcPtr(team_offset_0 + 0x1B4);
	if (!team_offset_1)
		return false;

	// Commander, SquadLeader, SquadID base pointers
	procptr_t role_offset_0 = peekProcPtr(pModule + 0x685228);
	if (!role_offset_0)
		return false;
	procptr_t role_offset_1 = peekProcPtr(role_offset_0 + 0x6C);
	if (!role_offset_1)
		return false;

	// Tactical pointers
	procptr_t tactical_offset_0 = peekProcPtr(pModule + 0x72D148);
	if (!tactical_offset_0)
		return false;
	procptr_t tactical_offset_1 = peekProcPtr(tactical_offset_0 + 0xE4);
	if (!tactical_offset_1)
		return false;

	// VoiP pointers
	procptr_t voip_offset_0 = peekProcPtr(pModule + 0x620D30);
	if (!voip_offset_0)
		return false;

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(pModule + 0x6697C8, avatar_pos, 12) &&       // Avatar position values (X, Y and Z).
		 peekProc(RendDX9 + 0x20E9F4, camera_pos, 12) &&       // Camera position values (X, Y and Z).
		 peekProc(RendDX9 + 0x2139D4, camera_front, 12) &&     // Avatar front vector values (X, Y and Z).
		 peekProc(RendDX9 + 0x2139C4, camera_top, 12) &&       // Avatar top vector values (X, Y and Z).
		 peekProc(server_offset_1, server_name) &&             // Server name.
		 peekProc(pModule + 0x6228B9, soldier_name) &&         // Soldier name.
		 peekProc(team_offset_1 + 0xA0, team_id) &&            // Team ID (0 for PAC, 1 for EU).
		 peekProc(role_offset_1 + 0x108, squad_id);            // Squad ID (0 for none, 1-9 for squads).
	peekProc(role_offset_1 + 0x10C, is_commander) &&           // Whether Commander.
		peekProc(role_offset_1 + 0x10D, is_squad_leader) &&    // Whether Squad Leader.
		peekProc(tactical_offset_1 + 0xCC, target_squad_id) && // Commander's selected squad.
		peekProc(voip_offset_0 + 0xA1, on_voip) &&             // Whether VoiP is active.
		peekProc(voip_offset_0 + 0xA0, on_voip_com);           // Whether VoiP to/from commander is active.

	// This prevents the plugin from linking to the game in case something goes wrong during values retrieval from
	// memory addresses.
	if (!ok)
		return false;


	// Begin context
	std::ostringstream ocontext;

	// Server name
	escape(server_name, sizeof(server_name));

	if (strcmp(server_name, "") != 0)
		ocontext << " {\"ipport\": \"" << server_name << "\"}";

	context = ocontext.str();
	// End context

	// Begin identity
	std::wostringstream oidentity;

	// Soldier name
	escape(soldier_name, sizeof(soldier_name));

	oidentity << "{"
			  << "\"ipport\": \"" << server_name << "\", "
			  << "\"team_id\": " << team_id << ", "
			  << "\"squad_id\": " << squad_id << ", "
			  << "\"is_commander\": " << (is_commander ? "true" : "false") << ", "
			  << "\"is_squad_leader\": " << (is_squad_leader ? "true" : "false") << ", "
			  << "\"target_squad_id\": " << target_squad_id << ", "
			  << "\"on_voip\": " << (on_voip ? "true" : "false") << ", "
			  << "\"on_voip_com\": " << (on_voip_com ? "true" : "false") << ", "
			  << "\"name\": \"" << soldier_name << "\""
			  << "}";

	identity = oidentity.str();
	// End identity

	// Copy camera direction vectors to avatar
	for (int i = 0; i < 3; i++) {
		avatar_front[i] = camera_front[i];
		avatar_top[i]   = camera_top[i];
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"BF2142.exe")) // Retrieve game executable's memory address
		return false;

	RendDX9 = getModuleAddr(L"RendDX9.dll"); // Retrieve "RendDX9.dll" module's memory address
	// This prevents the plugin from linking to the game in case something goes wrong during module's memory address
	// retrieval.
	if (!RendDX9)
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
	return std::wstring(L"Supports Battlefield 2142 version 1.51."); // Plugin long description
}

static std::wstring description(L"Battlefield 2142 1.51"); // Plugin short description
static std::wstring shortname(L"Battlefield 2142");        // Plugin short name

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin bf2142plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								   generic_unlock,      longdesc,    fetch };

static MumblePlugin2 bf2142plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bf2142plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &bf2142plug2;
}
