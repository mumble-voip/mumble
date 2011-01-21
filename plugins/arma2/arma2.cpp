/* Copyright (C) 2005-2010, Snares <snares@users.sourceforge.net>
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

BYTE *posptr;
BYTE *frontptr;
BYTE *topptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &, std::wstring &) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// char state;
	bool ok;

	/*
	float front_corrector1;
	float front_corrector2;
	float front_corrector3;

	float top_corrector1;
	float top_corrector2;
	float top_corrector3;
	*/

	/*
		value is 0 when one is not in a game, 4 when one is

	   ok = peekProc((BYTE *) 0x, &state, 1); // Magical state value
	   if (! ok)
	 	return false;

	   if (state == 0)
	          return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	*/

	ok = peekProc(posptr, avatar_pos, 12) &&
	     peekProc(frontptr, avatar_front, 12) &&
	     peekProc(topptr, avatar_top, 12);

	if (avatar_pos[1] > 999000000.0)
		return false;

	/*
	peekProc(frontptr, &front_corrector1, 4) &&
	peekProc(frontptr + 0xC, &front_corrector2, 4) &&
	peekProc(frontptr + 0x18, &front_corrector3, 4) &&
	peekProc(topptr, &top_corrector1, 4) &&
	peekProc(topptr + 0xC, &top_corrector2, 4) &&
	peekProc(topptr + 0x18, &top_corrector3, 4);
	*/

	if (! ok)
		return false;

	/*
	avatar_front[0] = front_corrector1;
	avatar_front[1] = front_corrector2;
	avatar_front[2] = front_corrector3;

	avatar_top[0] = top_corrector1;
	avatar_top[1] = top_corrector2;
	avatar_top[2] = top_corrector3;
	*/

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	posptr = NULL;

	if (! initialize(pids, L"arma2.exe"))
		return false;

	/*
	BYTE bState;
	peekProc((BYTE *) 0x00BF64D0, &bState, 1);
	if (bState == 0)
		return false;
	*/

	/*
	   Comment out code we don't need
	   BYTE *pModule=getModuleAddr(L"<module name, if you need it>.dll");
	   if (!pModule)
	*/

	BYTE *ptr1 = peekProc<BYTE *>((BYTE *) 0x00C500FC);

	BYTE *ptr2 = peekProc<BYTE *>(ptr1 + 0x88);

	BYTE *base = ptr2 + 0x10;

	posptr = base + 0x18;
	frontptr = base;
	topptr = base + 0xC;

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
	return std::wstring(L"Supports Armed Assault 2 v1.08. No identity or context support yet.");
}

static std::wstring description(L"ArmA 2 v1.08");
static std::wstring shortname(L"ArmA 2");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin arma2plug = {
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

static MumblePlugin2 arma2plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &arma2plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &arma2plug2;
}
