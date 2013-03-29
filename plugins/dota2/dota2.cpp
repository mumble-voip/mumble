/*
   Copyright (C) 2012, dark_skeleton (d-rez) <dark.skeleton@gmail.com>
   Copyright (C) 2013, GDur <gdur.mugen@gmail.com> 
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

static BYTE *aposptr;
static BYTE *afrontptr;
static BYTE *cposptr;
static BYTE *contextptr;

enum state_values {
	STATE_IN_GAME            = 0,
	STATE_UNKNOWN            = 1,
	STATE_LOADING_OR_IN_MENU = 2
};

static bool resolvePointers(void)
{
	aposptr = afrontptr = cposptr = contextptr = NULL;

	// Camera position pointer
	cposptr		= pModule + 0x1F2BDD0;

	// Context contains <ip>:<port> as a string, is emty when disconneted
	contextptr	= pModule + 0x1EB1050;
	
	// Avatar dynamic pointer
	BYTE *ptr1	= peekProc<BYTE*>(pModule + 0x01E69544);
	if (! ptr1)
		return false;

	BYTE *ptr2	= peekProc<BYTE*>(ptr1 + 0x18);
	if (! ptr2)
		return false;
	
	BYTE *ptr3	= peekProc<BYTE*>(ptr2 + 0x2c);
	if (! ptr3)
		return false;

	BYTE *ptr4	= peekProc<BYTE*>(ptr3 + 0x4);
	if (! ptr4)
		return false;
	
	// Avatar position pointer
	aposptr		= ptr4 + 0x310;

	// Avatar angle pointer
	afrontptr	= ptr4 + 0x380;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float apos[3], afront[3];
	float cpos[3], cfront[3];

	bool ok;

	// if state available: connect to game
	int state;
	ok = peekProc(pModule + 0x1EB0D18, state);
	if (! ok)
		return false;
	context.clear();
	
	if (! resolvePointers())
		return true;
	
	char ccontext[20];

	ok = peekProc(aposptr,    apos,     12) &&
		 peekProc(afrontptr,  afront,   12) &&
		 peekProc(cposptr,    cpos,     12) &&
		 peekProc(contextptr, ccontext, 20);

	if (! ok)
		return true;
	
	ccontext[sizeof(ccontext) - 1] = 0;
	if (ccontext[0] != '0') {
		std::ostringstream ocontext;
		ocontext << " { \"ipport\": \"" << ccontext << "\" }";
		context = ocontext.str();
	}

	// ADJUSTING CALCULATIONS
	float h         = afront[0];
	float v         = afront[1];

	if ((v < -180.0f) || (v > 180.0f) || (h < -180.0f) || (h > 180.0f))
		return false;

	h               *= static_cast<float>(M_PI / 180.0f);
	v               *= static_cast<float>(M_PI / 180.0f);
	
	// AVATAR
	avatar_pos[0]   = apos[0] / 39.37f;
	avatar_pos[1]   = apos[2] / 39.37f;
	avatar_pos[2]   = apos[1] / 39.37f;

	// SWAP [1] and [2]
	avatar_front[0] =  afront[0];
	avatar_front[1] =  afront[2];
	avatar_front[2] = -afront[1]; // NEGATE

	avatar_top[0]   = 0.0f;
	avatar_top[1]   = 1.0f;
	avatar_top[2]   = 0.0f;


	// CAMERA
	camera_pos[0]   = cpos[0] / 39.37f;		
	camera_pos[1]   = avatar_pos[1];    // SAME HEIGHT AS PLAYER! -> avatar_pos[1]
	camera_pos[2]   = cpos[1] / 39.37f;
	
	// LOOK NORTH
	camera_front[0] = 0.0f;
	camera_front[1] = 0.0f;
	camera_front[2] = 1.0f;

	// ORTOGONAL TO PLANE AS PLAYER
	camera_top[0]   = 0.0f;
	camera_top[1]   = 1.0f;
	camera_top[2]   = 0.0f;

	return ok;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"dota.exe",L"client.dll"))
		return false;
	
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	std::wstring sidentity;
	std::string scontext;

	if (! fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
		generic_unlock();
		return false;
	}

	return true;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports independent camera and avatar positions, front and top vectors. State detection will enable PA if not in menu. Context support will make mumble able to differentiate between matches. No identity support yet (feel free to contribute).");
}

static std::wstring description(L"DotA 2 (build 5255:570)");
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