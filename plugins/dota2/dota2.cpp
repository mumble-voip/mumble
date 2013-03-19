/* Copyright (C) 2012, dark_skeleton (d-rez) <dark.skeleton@gmail.com>
   Copyright (C) 2013, gdur <gdur.mugen@gmail.com> 
   Copyright (C) 2013, maun <ma.adameit@gmail.com> 
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
static BYTE *rotptr;
static BYTE *camptr;
static BYTE *afrontptr;

enum state_values {
    STATE_IN_GAME = 0,
    STATE_UNKN    = 1,
    STATE_LOADING_OR_IN_MENU  = 2
};

static bool calcout(float *pos, float *rot, float *cam, float *opos, float *front, float *top, float *ocam) {
	float h  = rot[0];
	float v  = rot[1];

	if ((v < -180.0f) || (v > 180.0f) || (h < -180.0f) || (h > 180.0f))
		return false;

	h       *= static_cast<float>(M_PI / 180.0f);
	v       *= static_cast<float>(M_PI / 180.0f);

	// Seems Dota 2 is in inches like all Source-based games...
	opos[0]  = pos[0] / 39.37f;
	opos[1]  = pos[2] / 39.37f;
	opos[2]  = pos[1] / 39.37f;
 
	ocam[0]  = cam[0] / 39.37f;
	// SAME HEIGHT AS PLAYER! -> pos[2]
	ocam[1]  = pos[2] / 39.37f;
	ocam[2]  = cam[1] / 39.37f;
	
	// LOOK NORTH
	front[0] = 0.0f;
	front[1] = 0.0f;
	front[2] = 1.0f;

	// ORTOGONAL TO PLANE AS PLAYER
	top[0] = 0.0f;
	top[1] = 1.0f;
	top[2] = 0.0f;
	
	return true;
}

static bool refreshPointers(void)
{
	rotptr     = posptr = camptr = afrontptr = NULL;

  // Camera position pointer (static ptr: 5F7D6DD8)
	camptr     = pModule + 0x1E96DD8;

  // Camera angle pointer
	rotptr     = pModule + 0x1E96DE4;
	

  // Avatar front vector pointer
	BYTE *ptr1 = peekProc<BYTE*>(pModule + 0x01E335E4);
	if (!ptr1)
		return false;

	BYTE *ptr2 = peekProc<BYTE*>(ptr1 + 0x5ec);
	if (!ptr2)
		return false;
	
	BYTE *ptr3 = peekProc<BYTE*>(ptr2 + 0x0);
	if (!ptr3)
		return false;

	BYTE *ptr4 = peekProc<BYTE*>(ptr3 + 0x3dc);
	if (!ptr4)
		return false;
	
	posptr     = ptr4 + 0x310;
	
	afrontptr  = ptr4 + 0x380;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], rot[3], cam[3], afront[3];
	bool ok;

  // gamestate
	int state;
	ok = peekProc(pModule + 0x1EA4B20, state);
	if (!ok) return false;

	if (state == STATE_LOADING_OR_IN_MENU)
             return true;

	ok = peekProc(posptr, ipos, 12) &&
		 peekProc(rotptr, rot, 12) &&
		 peekProc(camptr, cam, 12) &&
		 peekProc(afrontptr, afront, 12);

	if (ok) {
		int res = calcout(ipos, rot, cam, avatar_pos, camera_front, camera_top, camera_pos);
		if (res) {
			avatar_top[0]   = 0;
			avatar_top[1]   = 1; // it's Dota, your character is always looking straight ahead ;)
			avatar_top[2]   = 0;
			avatar_front[0] = afront[0];
			avatar_front[1] = afront[2];
			avatar_front[2] = -afront[1]; 
			// For some reason when character moves up,
			// Z-axis position increases, but front vector shows
			// -1 for Z-axis, therefore this small fix

			// Example only -- only set these when you have sane values,
			// and make sure they're pretty constant (every change causes a sever message).
			//context = std::string(" serverip:port:team");
			//identity = std::wstring(L"STEAM ID");
			return res;
		}
	}

	return false;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"dota.exe",L"client.dll"))
		return false;

	float pos[3], rot[3], opos[3], top[3], front[3], cam[3], ocam[3], afront[3];

	if (!refreshPointers()) { generic_unlock(); return false; } // unlink plugin if this fails

	bool ok = peekProc(posptr,pos,12) &&
			  peekProc(rotptr,rot,12) &&
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
	return std::wstring(L"Supports DotA 2 build 5241:40. No identity/context support yet (feel free to contribute). Might work with newer builds. Supports independent camera and avatar positions, front and top vectors, with state detection (if ingame or not).");
}

static std::wstring description(L"DotA 2 (build 5241:40)");
static std::wstring shortname(L"DotA 2");

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