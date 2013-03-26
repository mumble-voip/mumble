/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2012-2013, Moritz Schneeweiss <quirb@hotmail.com>

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

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &/*identity*/) {
	for (int i=0;i<3;i++)
		avatar_pos[i]=avatar_front[i]=avatar_top[i]=camera_pos[i]=camera_front[i]=camera_top[i]=0.0f;

	unsigned int state;
	bool ok;

	BYTE *stateptr0;
	BYTE *stateptr1;
	BYTE *stateptr2;
	ok = peekProc((BYTE *) pModule+0x2B57830, stateptr0) &&
	     peekProc((BYTE *) stateptr0 + 0x134, stateptr1) &&
	     peekProc((BYTE *) stateptr1 + 0x24, stateptr2) &&
	     peekProc((BYTE *) stateptr2 + 0x5fc, state);

	if (! ok)
		return false;

	if (state != 0xFFFFFFFF) {
		context.clear();
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	}

	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float cam_corrector[3];
	float pos_corrector[3];
	float front_corrector[3];
	float top_corrector[3];

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	BYTE *baseptr0;
	BYTE *baseptr1;

	ok = peekProc((BYTE *) pModule + 0x00001984, baseptr0) &&
	     peekProc((BYTE *) baseptr0 + 0x4AC, baseptr1) &&
	     peekProc((BYTE *) baseptr1 + 0x1e10, cam_corrector) &&
	     peekProc((BYTE *) baseptr1 + 0x1e30, front_corrector) &&
	     peekProc((BYTE *) baseptr1 + 0x1e20, top_corrector) &&
	     peekProc((BYTE *) baseptr1 + 0xB0, pos_corrector);

	if (! ok)
		return false;

	// Convert to left-handed coordinate system
	camera_pos[0] = cam_corrector[2];
	camera_pos[1] = cam_corrector[1];
	camera_pos[2] = cam_corrector[0];

	avatar_pos[0] = pos_corrector[2];
	avatar_pos[1] = pos_corrector[1];
	avatar_pos[2] = pos_corrector[0];

	avatar_pos[1] += 1.5f;

	avatar_front[0] = front_corrector[2];
	avatar_front[1] = front_corrector[1];
	avatar_front[2] = front_corrector[0];
	
	avatar_top[0] = -top_corrector[2];
	avatar_top[1] = -top_corrector[1];
	avatar_top[2] = -top_corrector[0];
	
	for (int i=0;i<3;i++) {
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}

	// Read continent name
	BYTE *cbase = peekProc<BYTE *> ((BYTE *) pModule + 0x02B0F548);
	BYTE *cptr1 = cbase + 0xEC;

	char continentname[6];
	peekProc(cptr1, continentname);
	continentname[sizeof(continentname)/sizeof(continentname[0]) - 1] = '\0';

	std::ostringstream contextss;
	contextss << "{"
		<< "\"continent\":\"" << continentname << "\""
		<< "}";

	context = contextss.str();

	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"PlanetSide2.exe"))
		return false;

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	std::wstring sidentity;
	std::string scontext;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Planetside 2 (v0.590.2.201856). No identity support yet. Context support limited to continentname.");
}

static std::wstring description(L"Planetside 2 (v0.590.2.201856)");
static std::wstring shortname(L"Planetside 2");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin ps2plug = {
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

static MumblePlugin2 ps2plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &ps2plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &ps2plug2;
}
