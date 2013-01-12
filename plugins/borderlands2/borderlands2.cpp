/* Copyright (C) 2013, Steve Hill <github@cheesy.sackheads.org>
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

#include "../mumble_plugin_win32.h"

VOID *vects_ptr;
VOID *state_ptr;
VOID *character_name_ptr_loc;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &, std::wstring &identity)
{
	// Zero out the structures
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=0.0f;

	bool ok;

	char state;

	// State 1 == in-game, 0 == in-menu
	ok = peekProc(state_ptr, state);
	if (!ok) return false;

	if (state == 0)
             return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

	struct
	{
		float front[3];
		float top[3];
		float position[3];
	} game_vects;


	ok = peekProc(vects_ptr, game_vects);
	if (!ok) return false;

	// Copy game vectors into return values
	for (int i=0;i<3;i++)
	{
		camera_pos[i]   = avatar_pos[i]   = game_vects.position[i] / 100.0f; // Scale to meters
		camera_front[i] = avatar_front[i] = game_vects.front[i];
		camera_top[i]   = avatar_top[i]   = game_vects.top[i];
	}


	// Extract the character name
	BYTE *ptr1 = peekProc<BYTE*>(character_name_ptr_loc);
	BYTE *ptr2 = peekProc<BYTE*>(ptr1 + 0xC);
	BYTE *character_name_ptr = ptr2 + 0x80;

	char character_name[16]; // The game limits us to 15 char names
	ok = peekProc(character_name_ptr, character_name);
	if (ok)
	{
		// character_name is zero terminated, but using strnlen for double-plus safety
		identity.assign(character_name, character_name + strnlen(character_name, sizeof(character_name)));
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids)
{
	if (!initialize(pids, L"Borderlands2.exe"))
		return false;

	char detected_version[32];

	if (peekProc(pModule + 0x1E6D048, detected_version)
		&& strcmp(detected_version, "WILLOW2-PCSAGE-28-CL697606") != 0)
	{
		vects_ptr = pModule + 0x1E792B0;
		state_ptr = pModule + 0x1E79BC8;
		character_name_ptr_loc = pModule + 0x1E7302C;
	}
	else
	{
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

static std::wstring description(L"Borderlands 2 (v1.3.1)");
static std::wstring shortname(L"Borderlands 2");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin aaplug = {
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

static MumblePlugin2 aaplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &aaplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &aaplug2;
}
