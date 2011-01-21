/* Copyright (C) 2009-2010, Snares <snares@users.sourceforge.net>
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

//

////
#include "../mumble_plugin_win32.h"

using namespace std;

BYTE *pos0ptr;
BYTE *pos1ptr;
BYTE *pos2ptr;
BYTE *faceptr;
BYTE *topptr;
//BYTE *stateptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &, std::wstring &) {
	char state;
	//char ccontext[128];
	bool ok;

	float face_corrector[3];
	float top_corrector[3];

	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	ok = peekProc((BYTE *) 0x01DEAFD9, &state, 1);
	if (! ok)
		return false;

	if (state == 1)
		return true;

	/*
	   Z-Value is increasing when heading north
				  decreasing when heading south
	   X-Value is increasing when heading east
				  decreasing when heading west
	   Y-Value is increasing when going up
				  decreasing when going down
	*/

	//Convert to left-handed coordinate system

	ok = peekProc(pos2ptr, avatar_pos, 4) &&	//X
	     peekProc(pos1ptr, avatar_pos+1, 4) &&	//Y
	     peekProc(pos0ptr, avatar_pos+2, 4) &&  //Z
	     peekProc(faceptr, &face_corrector, 12) &&
	     peekProc(topptr, &top_corrector, 12);

	//peekProc((BYTE *) 0x0122E0B8, ccontext, 128);

	if (! ok)
		return false;

	if (face_corrector[1] <= -0.98) {
		top_corrector[1] = -top_corrector[1];
	}
	if (face_corrector[1] >= 0.98) {
		top_corrector[1] = -top_corrector[1];
	}

	//Find north by playing on a Warfare game type - center view on the up arrow on the mini map
	avatar_front[0] = face_corrector[2];
	avatar_front[1] = face_corrector[1];
	avatar_front[2] = face_corrector[0];

	avatar_top[0] = top_corrector[2];
	avatar_top[1] = top_corrector[1];
	avatar_top[2] = top_corrector[0];

	//avatar_top[0] = top_corrector[2];
	//avatar_top[1] = top_corrector[1];

	//ccontext[127] = 0;
	//context = std::string(ccontext);

	//if (context.find(':')==string::npos)
	//	context.append(":UT3PORT");

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	pos0ptr = pos1ptr = pos2ptr = faceptr = NULL;

	if (! initialize(pids, L"UT3.exe", L"wrap_oal.dll"))
		return false;

	BYTE *ptraddress = pModule + 0x8A740;
	BYTE *baseptr = peekProc<BYTE *>(ptraddress);

	pos0ptr = baseptr;
	pos1ptr = baseptr + 0x4;
	pos2ptr = baseptr + 0x8;
	faceptr = baseptr + 0x18;
	topptr = baseptr + 0x24;

	//stateptr = pModule + 0xC4;

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
	return std::wstring(L"Supports Unreal Tournament 3 (v2.1). No context or identity support yet.");
}

static std::wstring description(L"Unreal Tournament 3 (v2.1)");
static std::wstring shortname(L"Unreal Tournament 3");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin ut3plug = {
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

static MumblePlugin2 ut3plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &ut3plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &ut3plug2;
}
