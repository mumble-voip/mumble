// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

using namespace std;


bool ptr_chain_valid = false;

// Modules
procptr_t pmodule_bf2, pmodule_renddx9;

// Magic ptrs
procptr_t const login_ptr = 0x30058642;
procptr_t const state_ptr = 0x00A1D0A8;

// Vector ptrs
procptr_t pos_ptr, face_ptr, top_ptr;

// Context ptrs
procptr_t const ipport_ptr = 0x009A80B8;

// Identity ptrs
procptr_t commander_ptr, squad_leader_ptr, squad_state_ptr, team_state_ptr, voip_ptr, voip_com_ptr, target_squad_ptr;

inline bool resolve_ptrs() {
	pos_ptr = face_ptr = top_ptr = commander_ptr = squad_leader_ptr = squad_state_ptr = team_state_ptr = voip_ptr =
		voip_com_ptr = target_squad_ptr = 0;
	//
	// Resolve all pointer chains to the values we want to fetch
	//

	procptr_t base_bf2audio   = pModule + 0x4645c;
	procptr_t base_bf2audio_2 = peekProcPtr(base_bf2audio);
	if (!base_bf2audio_2)
		return false;

	pos_ptr  = peekProcPtr(base_bf2audio_2 + 0xb4);
	face_ptr = peekProcPtr(base_bf2audio_2 + 0xb8);
	top_ptr  = peekProcPtr(base_bf2audio_2 + 0xbc);
	if (!pos_ptr || !face_ptr || !top_ptr)
		return false;

	/*
	Magic:
		Logincheck:         0x30058642                                   BYTE      0 means not logged in
		state:              0x00A1D0A8                                   BYTE      0 while not in game
																				   usually 1, never 0 if you create your
	own server ingame; this value will switch to 1 the instant you click "Join Game" usually 3, never 0 if you load into
	a server; this value will switch to 3 the instant you click "Join Game"

	Context:
		IP:Port of server:  0x009A80B8                                   char[128] ip:port of the server

	Identity:
		Commander:          RendDX9.dll+00244AE0 -> 60 -> 110            BYTE      0 means not commander
		Squad leader state: RendDX9.dll+00244AE0 -> 60 -> 111            BYTE      0 is not squad leader
		Squad state:        RendDX9.dll+00244AE0 -> 60 -> 10C            BYTE      0 is not in squad; 1 is in Alpha
	squad, 2 Bravo, ... , 9 India Team state:         BF2.exe+0058734C -> 239                      BYTE      0 is blufor
	(US team, for example), 1 is opfor (Insurgents) VoiP state:         BF2.exe+005A4DA0 -> 61 BYTE      1 is VoiP
	active (held down) Com. VoiP state:    BF2.exe+005A4DA0 -> 4E                       BYTE      1 is VoiP on commander
	channel active (held down) Target squad state: RendDX9.dll+00266D84 -> C0 -> C0 -> 40 -> AC BYTE      1 is Alpha
	squad, 2 Bravo... selected on commander screen
	*/
	procptr_t base_renddx9 = peekProcPtr(pmodule_renddx9 + 0x00244AE0);
	if (!base_renddx9)
		return false;

	procptr_t base_renddx9_2 = peekProcPtr(base_renddx9 + 0x60);
	if (!base_renddx9_2)
		return false;

	commander_ptr    = base_renddx9_2 + 0x110;
	squad_leader_ptr = base_renddx9_2 + 0x111;
	squad_state_ptr  = base_renddx9_2 + 0x10C;

	procptr_t base_bf2 = peekProcPtr(pmodule_bf2 + 0x0058734C);
	if (!base_bf2)
		return false;

	team_state_ptr = base_bf2 + 0x239;

	procptr_t base_voip = peekProcPtr(pmodule_bf2 + 0x005A4DA0);
	if (!base_voip)
		return false;

	voip_ptr     = base_voip + 0x61;
	voip_com_ptr = base_voip + 0x4E;

	procptr_t base_target_squad = peekProcPtr(pmodule_renddx9 + 0x00266D84);
	if (!base_target_squad)
		return false;
	procptr_t base_target_squad_2 = peekProcPtr(base_target_squad + 0xC0);
	if (!base_target_squad_2)
		return false;
	procptr_t base_target_squad_3 = peekProcPtr(base_target_squad_2 + 0xC0);
	if (!base_target_squad_3)
		return false;
	procptr_t base_target_squad_4 = peekProcPtr(base_target_squad_3 + 0x40);
	if (!base_target_squad_4)
		return false;

	target_squad_ptr = base_target_squad_4 + 0xAC;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &identity) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;
	uint8_t logincheck;
	ok = peekProc(login_ptr, &logincheck, 1);
	if (!ok)
		return false;

	if (logincheck == 0)
		return false;

	uint8_t state;
	ok = peekProc(state_ptr, &state, 1); // Magical state value
	if (!ok)
		return false;

	if (state == 0) {
		ptr_chain_valid = false;
		context.clear();
		identity.clear();
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	} else if (!ptr_chain_valid) {
		if (!resolve_ptrs())
			return false;
		ptr_chain_valid = true;
	}


	char ccontext[128];
	uint8_t is_commander;
	uint8_t is_squad_leader;
	uint8_t is_in_squad;
	uint8_t is_opfor;
	uint8_t on_voip;
	uint8_t on_voip_com;
	uint8_t target_squad_id;

	ok = peekProc(pos_ptr, avatar_pos, 12) && peekProc(face_ptr, avatar_front, 12) && peekProc(top_ptr, avatar_top, 12)
		 && peekProc(ipport_ptr, ccontext, 128) && peekProc(commander_ptr, is_commander)
		 && peekProc(squad_leader_ptr, is_squad_leader) && peekProc(squad_state_ptr, is_in_squad)
		 && peekProc(team_state_ptr, is_opfor) && peekProc(voip_ptr, on_voip) && peekProc(voip_com_ptr, on_voip_com)
		 && peekProc(target_squad_ptr, target_squad_id);

	if (!ok)
		return false;

	/*
		Get context string; in this plugin this will be an
		ip:port (char 128 bytes) string
	*/
	ccontext[127] = 0;
	if (ccontext[0] != '0') {
		// With the current plugin ipport can switch to "0" sometimes.
		// As this is only transitory and switches back quickly just
		// keep on reporting the previous state as long as this happens.
		ostringstream ocontext;
		ocontext << "{ \"ipport\": \"" << ccontext << "\"}";

		context = ocontext.str();

		/*
			Get identity string.
		*/
		wostringstream oidentity;
		oidentity << "{"
				  << "\"ipport\": \"" << ccontext << "\", "
				  << "\"commander\":" << (is_commander ? "true" : "false") << ", "
				  << "\"squad_leader\":" << (is_squad_leader ? "true" : "false") << ", "
				  << "\"squad\":" << static_cast< unsigned int >(is_in_squad) << ", "
				  << "\"team\":\"" << (is_opfor ? "opfor" : "blufor") << "\", "
				  << "\"on_voip\":" << (on_voip ? "true" : "false") << ", "
				  << "\"on_voip_com\":" << (on_voip_com ? "true" : "false") << ", "
				  << "\"target_squad_id\":" << static_cast< unsigned int >(target_squad_id) << "}";

		identity = oidentity.str();
	}

	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	return ok;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"BF2.exe", L"BF2Audio.dll"))
		return false;

	pmodule_bf2 = getModuleAddr(L"BF2.exe");
	if (!pmodule_bf2)
		return false;

	pmodule_renddx9 = getModuleAddr(L"RendDX9.dll");
	if (!pmodule_renddx9)
		return false;

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Battlefield 2 v1.50");
}

static std::wstring description(L"Battlefield 2 v1.50");
static std::wstring shortname(L"Battlefield 2");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin bf2plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								generic_unlock,      longdesc,    fetch };

static MumblePlugin2 bf2plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bf2plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &bf2plug2;
}
