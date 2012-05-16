/* Copyright (C) 2012, dark_skeleton (d-rez) <dark.skeleton@gmail.com> 
   Copyright (C) 2005-2012, Thorvald Natvig <thorvald@natvig.com>

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
static BYTE *camptr;
static BYTE *camfrontptr;
static BYTE *camtopptr;
static BYTE *afrontptr;

static BYTE *hostipptr;
static BYTE *hostportptr;

static BYTE *gameptr;

static char prev_hostip[16];
static int prev_hostport;

static bool calcout(float *pos, float *cam, float *opos, float *ocam) {
	// Seems League of Legends is in centimeters? ;o Well it's not inches for sure :)
	for (int i = 0; i < 3; i++) {
		opos[i] = pos[i] / 100.00f;
		ocam[i] = cam[i] / 100.00f;
	}

	return true;
}

static bool refreshPointers(void) {
	// camera position vector @ 0xb56308
	// camera front vector    @ 0xb562ec
	// camera top vector	  @ 0xb5631c
	// avatar position vector @ 0x2eafae8
	// avatar front vector    @ 0xe00f88 -> +0x2a54
	// host ip (text)		  @ 0xaf4f028
	// host port (4 bytes)	  @ 0xaf4f044

	posptr = camptr = camfrontptr = camtopptr = afrontptr = NULL;
	
	// Camera position
	camptr = (BYTE *)0xb56308;

	// Camera front
	camfrontptr = (BYTE *)0xb562ec;

	// Camera top
	camtopptr = (BYTE *)0xb5631c;

	// Avatar front vector pointer
	BYTE *tmpptr = NULL;
	gameptr = (BYTE *)0xe00f88;			// NOTE: This pointer is availible ONLY when ingame. We are using this fact to unlink plugin when not ingame.
	tmpptr = peekProc<BYTE *>(gameptr);	

	if (!tmpptr)
		return false;				// Player not in game (game is still loading), unlink plugin

	afrontptr = tmpptr + 0x2a54;
	
	// Avatar position vector
	posptr = (BYTE *)0x2eafae8;			// NOTE: This consists of all zeros right after game is loaded until your avatar moves, but we don't have to worry about it since (0,0,0) is close to our spawning position

	// Host IP:PORT. It is kept in 3 places in memory, but 1 of them looks the coolest, so let's use it, ha!
	// IP is kept as text @ hostipptr
	// PORT is kept as a 4-byte decimal value @ hostportptr

	hostipptr = (BYTE *)0xaf4f028;
	hostportptr = (BYTE *)0xaf4f044;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &/*identity*/) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], cam[3];
	int hostport;
	char hostip[16];
	bool ok;

	// Player not in game (or something broke), unlink
	if (!peekProc<BYTE *>(gameptr))
		return false;

	ok = peekProc(camfrontptr, camera_front, 12) &&
		 peekProc(camtopptr, camera_top, 12) &&
		 peekProc(camptr, cam, 12) &&
		 peekProc(posptr, ipos, 12) &&
		 peekProc(afrontptr, avatar_front, 12) &&
		 peekProc(hostipptr, hostip) &&
		 peekProc(hostportptr, &hostport, 4);

	if (ok) {
		int res = calcout(ipos, cam, avatar_pos, camera_pos);
		if (res) {
			avatar_top[0] = 0.0f;
			avatar_top[1] = 1.0f; // Your character is always looking straight ahead ;)
			avatar_top[2] = 0.0f;

			if (strcmp(hostip, prev_hostip) != 0 || hostport != prev_hostport) {
				context.clear();
				memcpy(prev_hostip, hostip, 16);
				prev_hostport = hostport;

				if (strcmp(hostip, "") != 0) {
					char buffer[50];
					sprintf_s(buffer, 50, "{\"ipport\": \"%s:%d\"}", hostip, hostport);
					context.assign(buffer);
				}
			}
		}
		return res;
	}

	return false;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	if (! initialize(pids, L"League of Legends.exe"))
		return false;

	float pos[3], opos[3];
	float cam[3], ocam[3];
	float afront[3];
	float camfront[3], camtop[3];
	char hostip[16];
	int hostport;

	// unlink plugin if this fails
	if (!refreshPointers()) {
		generic_unlock();
		return false;
	}

	bool ok = peekProc(camfrontptr, camfront, 12) &&
			  peekProc(camtopptr, camtop, 12) &&
			  peekProc(posptr, pos, 12) &&
			  peekProc(camptr, cam, 12) &&
			  peekProc(afrontptr, afront, 12) &&
			  peekProc(hostipptr, hostip) &&
			  peekProc(hostportptr, &hostport, 4);

	ok = ok && calcout(pos,cam,opos,ocam); // make sure values are OK

	if (ok) {
		*prev_hostip = '\0';
		prev_hostport = 0;
		return true;
	}

	generic_unlock();
	return false;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports League of Legends v1.0.0.139 with context. No identity support yet.");
}

static std::wstring description(L"League of Legends (v1.0.0.139)");
static std::wstring shortname(L"League of Legends");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin lolplug = {
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

static MumblePlugin2 lolplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &lolplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &lolplug2;
}
