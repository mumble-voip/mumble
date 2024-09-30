// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2013, Steve Hill <github@cheesy.sackheads.org>
   Copyright (C) 2013, Gabriel Risterucci <cleyfaye@gmail.com>
   Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
	 this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
	 this list of conditions and the following disclaimer in the documentation
	 and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
	 contributors may be used to endorse or promote products derived from this
	 software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

#include <algorithm>

procptr_t vects_ptr;
procptr_t state_ptr;
procptr_t character_name_ptr_loc;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &identity) {
	// Zero out the structures
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;

	char state;

	// State 1 == in-game, 0 == in-menu
	ok = peekProc(state_ptr, state);
	if (!ok)
		return false;

	if (state == 0)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

	struct {
		float front[3];
		float top[3];
		float position[3];
	} game_vects;


	ok = peekProc(vects_ptr, game_vects);
	if (!ok)
		return false;

	// Copy game vectors into return values
	for (int i = 0; i < 3; i++) {
		camera_pos[i] = avatar_pos[i] = game_vects.position[i] / 100.0f; // Scale to meters
		camera_front[i] = avatar_front[i] = game_vects.front[i];
		camera_top[i] = avatar_top[i] = game_vects.top[i];
	}


	// Extract the character name
	procptr_t ptr1               = peekProcPtr(character_name_ptr_loc);
	procptr_t ptr2               = peekProcPtr(ptr1 + 0xC);
	procptr_t character_name_ptr = ptr2 + 0x80;

	char character_name[16]; // The game limits us to 15 char names
	ok = peekProc(character_name_ptr, character_name);
	if (ok) {
		// character_name is zero terminated, but using strnlen for double-plus safety
		identity.assign(character_name, character_name + strnlen(character_name, sizeof(character_name)));
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"Borderlands2.exe")) {
		return false;
	}

	unsigned char detected_version[32];

	// Note for further versions:
	// The "version" string above change. However, it looks like it will always start
	// with "WILLOW2-". Everything after this can change between versions.
	// Position vectors are read as triplet (X,Y,Z). The tree triplet are in this order:
	// front, top, position.
	// When entering the game, in Sanctuary (after liftoff), with the character Zero,
	// the reading are the following (rounded):
	// front.X = 0
	// front.Y = 0
	// front.Z = 1
	// top.X = 0
	// top.Y = 1
	// top.Z = 0
	// pos.X = -8109
	// pos.Y = 3794
	// pos.Z = 2930
	// The "state" ptr is just a value that reliably alternate between 0 (in main menu)
	// and 1 (not in main menu). There is a lot of value that keep reliably changing even
	// across restart, change of characters...
	// Note that I couldn't find an address that would do this reliably with the game "pause"
	// menu, only the main menu (when you initially start the game, or completely exit your
	// current game)

	// VERSION_EQ safely checks whether the content of `buf'
	// (a buffer in the form of a C-style array of unsigned char)
	// contains the byte-level content of the string literal `strlit'.
	//
	// The NUL terminator of the string literal is not considered in
	// this equality check.
#define VERSION_EQ(buf, strlit) memcmp(buf, strlit, std::min(sizeof(buf), sizeof(strlit) - 1)) == 0

	// 1.3.1
	if (peekProc(pModule + 0x1E6D048, detected_version) && VERSION_EQ(detected_version, "WILLOW2-PCSAGE-28-CL697606")) {
		vects_ptr              = pModule + 0x1E792B0;
		state_ptr              = pModule + 0x1E79BC8;
		character_name_ptr_loc = pModule + 0x1E7302C;
	}
	// 1.4.0
	else if (peekProc(pModule + 0x1E8D1D8, detected_version)
			 && VERSION_EQ(detected_version, "WILLOW2-PCSAGE-77-CL711033")) {
		vects_ptr              = pModule + 0x1E993F0;
		state_ptr              = pModule + 0x1E99D08;
		character_name_ptr_loc = pModule + 0x1E93194;
	}
	// 1.5.0
	else if (peekProc(pModule + 0x01E9F338, detected_version)
			 && VERSION_EQ(detected_version, "WILLOW2-PCLILAC-60-CL721220")) {
		vects_ptr              = pModule + 0x1EAB650;
		state_ptr              = pModule + 0x1EABF68;
		character_name_ptr_loc = pModule + 0x01EA5384;
	}
	// 1.7.0
	else if (peekProc(pModule + 0x01ED53A8, detected_version)
			 && VERSION_EQ(detected_version, "WILLOW2-PCALLIUM-55-CL770068")) {
		vects_ptr              = pModule + 0x1EE18E0;
		state_ptr              = pModule + 0x1EE21F8;
		character_name_ptr_loc = pModule + 0x01EDB5B4;
	}
	// 1.8.3
	else if (peekProc(pModule + 0x1EE63C8, detected_version)
			 && VERSION_EQ(detected_version, "WILLOW2-PCCHINA-29-CL827556")) {
		vects_ptr              = pModule + 0x1EF2930;
		state_ptr              = pModule + 0x1EF3248;
		character_name_ptr_loc = pModule + 0x01EEC5D4;
	} else {
		generic_unlock();
		return false;
	}

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
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
	return std::wstring(L"Supports Borderlands 2. No context support yet.");
}

static std::wstring description(L"Borderlands 2 (v1.8.3)");
static std::wstring shortname(L"Borderlands 2");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin bl2plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								generic_unlock,      longdesc,    fetch };

static MumblePlugin2 bl2plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &bl2plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &bl2plug2;
}
