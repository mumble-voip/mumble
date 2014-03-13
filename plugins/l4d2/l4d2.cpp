/* Copyright (C) 2005-2012, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2012, dark_skeleton (d-rez) <dark.skeleton@gmail.com>

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

static BYTE *posptr;
static BYTE *rotptr;
static BYTE *stateptr;
static BYTE *contextptr;

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
	float h = rot[0];
	float v = rot[1];

	if ((v < -180.0f) || (v > 180.0f) || (h < -180.0f) || (h > 180.0f))
		return false;

	h *= static_cast<float>(M_PI / 180.0f);
	v *= static_cast<float>(M_PI / 180.0f);

	// Seems L4D is in inches. INCHES?!?
	opos[0] = pos[0] / 39.37f;
	opos[1] = pos[2] / 39.37f;
	opos[2] = pos[1] / 39.37f;

	front[0] = cos(v) * cos(h);
	front[1] = -sin(h);
	front[2] = sin(v) * cos(h);

	h -= static_cast<float>(M_PI / 2.0f);

	top[0] = cos(v) * cos(h);
	top[1] = -sin(h);
	top[2] = sin(v) * cos(h);
	/*
		printf("Poll\n");
		printf("%f %f %f : %f %f\n",pos[0],pos[1],pos[2], rot[0], rot[1]);
		printf("%f %f %f :: %.2f %.2f %.2f :: %.2f %.2f %.2f\n", opos[0], opos[1], opos[2], front[0], front[1], front[2], top[0], top[1], top[2]);
	*/
	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	posptr = rotptr = stateptr = contextptr = NULL;

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
	
	pos: float likely in a range of 0 to 10000 (changes if you move. constant if you view around)
	rot: float in range of -180 to 180. If you look up the value is -89.0 . If you look down it is 89.0 (changes if you viewaround. constant if you only move)
	state: single player: search for loopback. Go back to menu. There is likely only one instance which has the string when having a game running and empty when not running a game.
	context: same as state appearantly

	*/
	posptr = pModule + 0x818950;
	rotptr = pModule + 0x7D0D18;
	stateptr = pModule + 0x8169BC;
	contextptr = pModule + 0x8169BC;
	
	float pos[3];
	float rot[3];
	char state, _context[21];
	bool ok = peekProc(posptr, pos, 12) &&
	          peekProc(rotptr, rot, 12) &&
			  peekProc(stateptr, &state, 1) &&
			  peekProc(contextptr, _context);

	if (ok) {
		float opos[3], top[3], front[3];
		return calcout(pos, rot, opos, top, front);
	}

	generic_unlock();

	return false;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &/*identity*/) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], rot[3];
	bool ok;
	char state;
	char _context[21];

	// stateptr returns byte values: 0 when map is not loaded; first character in hostname when map is loaded
	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12) &&
		 peekProc(stateptr, &state, 1) &&
		 peekProc(contextptr, _context);

	if (state == 0) {
		context = std::string(""); // clear context
	 	return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	}

	if (ok) {
		int res = calcout(ipos, rot, avatar_pos, avatar_front, avatar_top);
		if (res) {
			for (int i=0;i<3;++i) {
				camera_pos[i] = avatar_pos[i];
				camera_front[i] = avatar_front[i];
				camera_top[i] = avatar_top[i];

				// Example only -- only set these when you have sane values, and make sure they're pretty constant (every change causes a sever message).
				//context = std::string("server/map/blah");
				//identity = std::wstring(L"STEAM_1:2:3456789");
			}

			_context[sizeof(_context) - 1] = '\0';
			std::string sHost(_context);
			// This string can be either "xxx.xxx.xxx.xxx:yyyyy" (or shorter), "loopback:0" or "" (empty) when in menus. Hence 21 size for char.
			if (!sHost.empty())
			{
				if (sHost.find("loopback") == std::string::npos)
				{
					std::ostringstream newcontext;
					newcontext << "{\"ipport\": \"" << sHost << "\"}";
					context = newcontext.str();
				}
						
			}
			return res;
		}
	}

	return false;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports L4D2 version 2.1.3.5 with context support. No identity support yet.");
}

static std::wstring description(L"Left 4 Dead 2 (version 2.1.3.5)");
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
