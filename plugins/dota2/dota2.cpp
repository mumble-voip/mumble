/* Copyright (C) 2012, dark_skeleton <dark.skeleton@gmail.com> 
   Many pieces of code by Thorvald Natvig <thorvald@natvig.com>

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

   Oh god developing a plugin is freakin' hard work and lots of testing and debugging. Just when you think you got
   the right address/pointer, you realize how wrong you are :D
*/ 

#include "../mumble_plugin_win32.h"  

BYTE *posptr;
BYTE *rotptr;
BYTE *camptr;
BYTE *afrontptr;
BYTE *gameptr;

static bool calcout(float *pos, float *rot, float *cam, float *opos, float *front, float *top, float *ocam) {
	float h = rot[0];
	float v = rot[1];

	if ((v < -180.0f) || (v > 180.0f) || (h < -180.0f) || (h > 180.0f))
		return false;

	h *= static_cast<float>(M_PI / 180.0f);
	v *= static_cast<float>(M_PI / 180.0f);

	// Seems Dota 2 is in inches like all Source-based games...
	opos[0] = pos[0] / 39.37f;
	opos[1] = pos[2] / 39.37f;
	opos[2] = pos[1] / 39.37f;

	ocam[0] = cam[0] / 39.37f;
	ocam[1] = cam[2] / 39.37f;
	ocam[2] = cam[1] / 39.37f;

	front[0] = cos(v) * cos(h);
	front[1] = -sin(h);
	front[2] = sin(v) * cos(h);

	h -= static_cast<float>(M_PI / 2.0f);

	top[0] = cos(v) * cos(h);
	top[1] = -sin(h);
	top[2] = sin(v) * cos(h);

	return true;
}

inline bool refreshPointers(void)
{
	rotptr = posptr = camptr = afrontptr = gameptr = NULL;
	// camera position vector @ client.dll+1587888 -> +0x8a4
	// camera angle vector    @ client.dll+1587888 -> +0x8b0
	// avatar position vector @ client.dll+15b139c -> +0x3b8
	// avatar front vector    @ client.dll+15916c4 -> +0x430

	BYTE *tmpptr; // temporary pointer to avoid creating many pointers
	
	tmpptr = peekProc<BYTE *>(pModule + 0x1587888);
	
	// Camera position pointer
	camptr = tmpptr + 0x8a4;
	
	// Camera angle pointer
	rotptr = tmpptr + 0x8b0;

	// Avatar front vector pointer
	gameptr = pModule + 0x15916c4;			// NOTE: This pointer is availible ONLY when ingame. We are using this fact 
	tmpptr = peekProc<BYTE *>(gameptr);		// to unlink plugin when not ingame.
	if (!tmpptr) return false;				// Player not in game (most likely in menus), unlink plugin
	afrontptr = tmpptr + 0x430;
	
	// Avatar position vector pointer
	tmpptr = peekProc<BYTE *>(pModule + 0x15b139c);
	if (!tmpptr) return false;				//Player not in game (or something broke lol)
	posptr = tmpptr + 0x3b8;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], rot[3], cam[3], afront[3];
	bool ok;

	if (!peekProc<BYTE *>(gameptr)) return false; // Player not in game, unlink

	ok = peekProc(rotptr, rot, 12) &&
		 peekProc(camptr, cam, 12) &&
		 peekProc(posptr, ipos, 12) &&
		 peekProc(afrontptr, afront, 12);

	if (ok) {
		int res = calcout(ipos, rot, cam, avatar_pos, camera_front, camera_top, camera_pos);
		if (res) {
			avatar_top[0] = 0;
			avatar_top[1] = 1; // it's Dota, your character is always looking straight ahead ;)
			avatar_top[2] = 0;
			avatar_front[0] = afront[0];
			avatar_front[1] = afront[2];
			avatar_front[2] = -afront[1]; // For some reason when character moves up, Z-axis position increases, but 
										  // front vector shows -1 for Z-axis, therefore this small fix

				// Example only -- only set these when you have sane values, and make sure they're pretty constant (every change causes a sever message).
				context = std::string(" serverip:port:team");
				identity = std::wstring(L"STEAM ID");
			return res;
		}
	}

	return false;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	// Since we're using pointers, we don't have to worry about states too much

	if (! initialize(pids, L"dota.exe",L"client.dll"))
		return false;

	float pos[3], rot[3], opos[3], top[3], front[3], cam[3], ocam[3], afront[3];
	
	std::wstring sidentity;
	std::string scontext;

	if (!refreshPointers()) { generic_unlock(); return false; }// unlink plugin if this fails

	bool ok = peekProc(rotptr,rot,12) &&
			  peekProc(posptr,pos,12) &&
			  peekProc(camptr,cam,12) &&
			  peekProc(afrontptr,afront,12);

	if (ok) {
		return calcout(pos,rot,cam,opos,front,top,ocam); // make sure values are OK
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Dota 2 build 4926. No identity/context support yet (feel free to contribute). Might work with newer builds. Supports independent camera and avatar positions, front and top vectors, with state detection (if ingame or not).");
}

static std::wstring description(L"Dota 2 (build 4926)");
static std::wstring shortname(L"Dota 2");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin dota2plug = {
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

static MumblePlugin2 dota2plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &dota2plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &dota2plug2;
}