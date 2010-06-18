/* Copyright (C) 2010, Mark-Willem Jansen <rawnar@users.sourceforge.net>
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
BYTE *contextptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	char logincheck;
	char state;
	bool ok;
	// containers that will hold raw unedited data.
	char ccontext[64];
	float pos_corrector[3];
	float front_corrector[3];
	float top_corrector[3];

	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// When your are not logged in and the context pointer will not work.
	ok = peekProc((BYTE *) 0x01fce170, &logincheck, 1);
	if (! ok)
		return false;

	if (logincheck == 0)
		return false;

	//	State value is working most of the time.
	ok = peekProc((BYTE *) 0x01fb1b99, &state, 1); // Magical state value
	if (! ok)
		return false;

	// When state is 0 your are loaded into a map, when it is 1 your are not in a map.
	if (state == 1)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

	ok = peekProc(posptr, &pos_corrector, 12) &&
	     peekProc(frontptr, &front_corrector, 12) &&
	     peekProc(topptr, &top_corrector, 12) &&
	     peekProc(contextptr, ccontext, 64);

	if (! ok)
		return false;

	//	In-game coordinate system:
	//	x points to the North (z in mumble)
	//	y points to the East (x in mumble)
	//	z points upwards (y in mumble)
	avatar_pos[0] = pos_corrector[1];
	avatar_pos[1] = pos_corrector[2];
	avatar_pos[2] = pos_corrector[0];

	for (int i=0;i<3;i++)
		avatar_pos[i]/=100.0f; // Unreal Unit is set to centimeters

	avatar_front[0] = front_corrector[1];
	avatar_front[1] = front_corrector[2];
	avatar_front[2] = front_corrector[0];

	avatar_top[0] = top_corrector[1];
	avatar_top[1] = top_corrector[2];
	avatar_top[2] = top_corrector[0];

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	ccontext[63] = 0;
	context = std::string(ccontext);
	if (context.find("bderlandspc") != std::string::npos)
		context.erase(0, context.find("bderlandspc"));
	if (context.find(":7777") != std::string::npos)
		context.erase(context.find(":7777") + 5);

	return ok;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	posptr = frontptr = topptr = contextptr = NULL;

	if (!initialize(pids, L"Borderlands.exe", L"Borderlands.exe"))
		return false;

	BYTE *ptr1 = peekProc<BYTE *>(pModule +  0x01bba2c8);
	BYTE *ptr2 = peekProc<BYTE *>(ptr1 + 0x34c);
	BYTE *ptr3 = peekProc<BYTE *>(ptr2 + 0x64);
	BYTE *ptr4 = peekProc<BYTE *>(ptr3 + 0xbc);

	posptr = ptr4 + 0x4;
	frontptr = ptr4 + 0x28;
	topptr = ptr4 + 0x10;

	ptr1 = peekProc<BYTE *>(pModule + 0x01bcd184);
	ptr2 = peekProc<BYTE *>(ptr1 + 0x28c);
	ptr3 = peekProc<BYTE *>(ptr2 + 0x210);

	contextptr = ptr3 + 0x2c;

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
	return std::wstring(L"Supports Borderlands v1.30. No identity support, but is also not needed.");
}

static std::wstring description(L"Borderlands v1.30");
static std::wstring shortname(L"Borderlands");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin borderlandsplug = {
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

static MumblePlugin2 borderlandsplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &borderlandsplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &borderlandsplug2;
}
