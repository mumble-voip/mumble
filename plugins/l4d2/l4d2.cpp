/* Copyright (C) 2012-2016, Joel Kees <joelkees@gmail.com>
   Copyright (C) 2005-2012, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2012, dark_skeleton (d-rez) <dark.skeleton@gmail.com>

   Copyright 2016 The Mumble Developers. All rights reserved.
   Use of this source code is governed by a BSD-style license
   that can be found in the LICENSE file at the root of the
   Mumble source tree or at <https://www.mumble.info/LICENSE>.
   
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

/*  DESCRIPTION        ADDRESS              TYPE    VALUE

    position tuple:    client.dll+0x774B40  float   (x, y, z location in inches)
    front tuple:       client.dll+0x774BA0  float   (x, y, z as unit vector in direction you are facing)
    top tuple:         client.dll+0x774BD0  float   (x, y, z as unit vector pointing out top of head)
	state:             client.dll+0x772ACC  bye     (0 in menu; non-zero in game)
    context:           client.dll+0x772ACC  str     (see context support below for its possible values)
*/

static BYTE *posptr;
static BYTE *frontptr;
static BYTE *topptr;
static BYTE *stateptr;
static BYTE *contextptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &/*identity*/);

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	posptr = frontptr = topptr = stateptr = contextptr = NULL;

	if (! initialize(pids, L"left4dead2.exe", L"client.dll"))
		return false;
	
	/*
	Some hints to make things easier next time valve updates this game:
	use Cheat Engine (non-network single player or non VAC-secured servers to be safe)
	type is Float
	use unknown initial value
	use value unchanged/ value changed
	speed things up by limiting the scan range to 40000000 to 90000000 (hex number)
	you need addresses relative to client.dll. You can get address by double clicking on the address.
	
	pos: float likely in a range of 0 to 10000 (changes if you move. constant if you view around.)
	front: float in range of -1 to 1. (changes if you move your camera. constant if you do not.)
	top: float in range of -1 to 1. (changes if you move your camera. constant if you do not.)
	state: single player -> search for "loopback:0"
	       public game -> search for server ip "xxx.xxx.xxx.xxx:yyyyy"
		   menu -> empty string ""
	context: same as state (they are used differently)

	*/
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;
	
	if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity))
		return true;

	generic_unlock();
	return false;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &/*identity*/) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float pos[3], front[3], top[3];
	bool ok;
	char state;
	char _context[21];
	
	// when valve updates the game and breaks the plugin, these following 5 addresses will need changed
	// nothing else should need to be modified
	posptr = pModule + 0x774B40;
	frontptr = pModule + 0x774BA0;
	topptr = pModule + 0x774BD0;
	stateptr = pModule + 0x772ACC;
	contextptr = pModule + 0x772ACC;

	ok = peekProc(posptr, pos, 12) &&
	     peekProc(frontptr, front, 12) &&
		 peekProc(topptr, top, 12) &&
		 peekProc(stateptr, &state, 1) &&
		 peekProc(contextptr, _context);

	// in menu -> disable plugin
	if (state == 0) {
		context = std::string(""); // clear context
	 	return true; // This results in all vectors being zero which tells Mumble to ignore them.
	}
	
	if(!ok)
		return false;
	
	// convert all vectors from right handed coordinate system to left handed coordinate system
	// L4D2 is in inches so convert to meters
	avatar_pos[0] = pos[0] / 39.37f;
	avatar_pos[1] = pos[2] / 39.37f;
	avatar_pos[2] = pos[1] / 39.37f;
	
	avatar_front[0] = front[0];
	avatar_front[1] = front[2];
	avatar_front[2] = front[1];
	
	avatar_top[0] = top[0];
	avatar_top[1] = top[2];
	avatar_top[2] = top[1];
	
	for (int i=0;i<3;++i) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];

		// Example only -- only set these when you have sane values, and make sure they're pretty constant (every change causes a sever message).
		//context = std::string("server/map/blah");
		//identity = std::wstring(L"STEAM_1:2:3456789");
	}
	
	// context support
	// used to disable positional audio when people are on different servers
	_context[sizeof(_context) - 1] = '\0';
	std::string sHost(_context);
	// this string will change depending on your context:
	// if in public game then server ip  -> "xxx.xxx.xxx.xxx:yyyyy"
	// if in single player then loopback -> "loopback:0"
	// if in menu then empty             -> ""
	if (!sHost.empty())
	{
		if (sHost.find("loopback") == std::string::npos)
		{
			std::ostringstream newcontext;
			newcontext << "{\"ipport\": \"" << sHost << "\"}";
			context = newcontext.str();
		}
				
	}

	return true;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports L4D2 version 2.1.4.5 with context support. No identity support yet.");
}

static std::wstring description(L"Left 4 Dead 2 (version 2.1.4.5)");
static std::wstring shortname(L"Left 4 Dead 2");

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

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &l4d2plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &l4d2plug2;
}
