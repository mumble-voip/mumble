/* Copyright (C) 2010-2011, Snares <snares@users.sourceforge.net>
   Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2011, Ryan Austin <ryan@gameforcecenters.com>

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

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	//char ccontext[128];
	char state;
	bool ok;

	ok = peekProc((BYTE *) 0x02348B6C, &state, 1); // State value
	if (! ok)
		return false;
	
	/*
	    Get context string; in this plugin this will be an
	    ip:port (char 256 bytes) string

	ccontext[127] = 0;
	context = std::string(ccontext);
	if ( state == 3 ) 
		context = std::string("menu");
	if ( state == 2 ) 
		context = std::string("ingame");
	if ( state == 1 ) 
		context = std::string("loading");
	*/

	// Current assesment of state values are:
	// 3 = in main menu
	// 2 = in game
	// 1 = loading
	if ( state != 2 )
		return true;	// This results in all vectors being zero which tells Mumble to ignore them.
	
	ok = peekProc((BYTE *) 0x02348B00, avatar_pos, 12) &&
		peekProc((BYTE *) 0x02348B30, avatar_front, 12) &&
		peekProc((BYTE *) 0x02348B20, avatar_top, 12);

	if (! ok)
		return false;

	// Flip our front vector
	for (int i=0;i<3;i++) {
		avatar_front[i] = -avatar_front[i];
	}

	// Convert from right to left handed
	avatar_pos[0] = -avatar_pos[0];
	avatar_front[0] = -avatar_front[0];
	avatar_top[0] = -avatar_top[0];

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return ok;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"bf3.exe"))
		return false;

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (!fetch(apos, afront, atop, cpos, cfront, ctop, context, identity)) {
		generic_unlock();
		return false;
	}

	return true;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Battlefield 3. No identity or context support.");
}

static std::wstring description(L"Battlefield 3 v876650, Plugin v0.1");
static std::wstring shortname(L"Battlefield 3");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin bf3plug = {
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

static MumblePlugin2 bf3plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &bf3plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &bf3plug2;
}
