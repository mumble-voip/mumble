// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2011, Mike <mike@flomp.net>
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

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

static unsigned int playerid;
static procptr_t base_address;
static procptr_t cvecptr;
static procptr_t displayptr;

static int setuppointers() {
	procptr_t playerptr, charptr;

	// Player stuff
	if (!peekProc(base_address + 0xF1CC68, playerid))
		return false;

	if (!peekProc(base_address + 0x11A7008 + (playerid * 4), &playerptr, 4) || !playerptr)
		return false;

	if (!peekProc(playerptr + 0x58C, &charptr, 4) || !charptr)
		return false;

	if (!peekProc(charptr + 0x20, &cvecptr, 4) || !cvecptr)
		return false;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	unsigned int playerid_check;
	if (!peekProc(base_address + 0xF1CC68, playerid_check))
		return false;
	if (playerid_check != playerid) {
		if (!setuppointers())
			return false;
	}

	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = 0.0f;

	bool ok;

	float pos_corrector[3];
	float front_corrector[3];
	float top_corrector[3];

	float campos_corrector[3];
	float camfront_corrector[3];
	float camtop_corrector[3];

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(cvecptr + 0x30, &pos_corrector, 12) && peekProc(cvecptr + 0x20, &top_corrector, 12)
		 && peekProc(cvecptr + 0x10, &front_corrector, 12) && peekProc(displayptr + 0x30, &campos_corrector, 12)
		 && peekProc(displayptr + 0x10, &camtop_corrector, 12) && peekProc(displayptr + 0x20, &camfront_corrector, 12);

	if (!ok)
		return false;

	// Convert to left-handed coordinate system
	avatar_pos[0] = pos_corrector[0];
	avatar_pos[1] = pos_corrector[2];
	avatar_pos[2] = pos_corrector[1];

	avatar_front[0] = front_corrector[0];
	avatar_front[1] = front_corrector[2];
	avatar_front[2] = front_corrector[1];

	avatar_top[0] = top_corrector[0];
	avatar_top[1] = top_corrector[2];
	avatar_top[2] = top_corrector[1];

	camera_pos[0] = campos_corrector[0];
	camera_pos[1] = campos_corrector[2];
	camera_pos[2] = campos_corrector[1];

	camera_front[0] = -camfront_corrector[0];
	camera_front[1] = -camfront_corrector[2];
	camera_front[2] = -camfront_corrector[1];

	camera_top[0] = camtop_corrector[0];
	camera_top[1] = camtop_corrector[2];
	camera_top[2] = camtop_corrector[1];

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"GTAIV.exe"))
		return false;

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	std::wstring sidentity;
	std::string scontext;
	procptr_t viewportptr;

	base_address = pModule - 0x400000;

	if (!peekProc(base_address + 0x10F47F0, &viewportptr, 4) || !viewportptr)
		return false;

	displayptr = viewportptr + 0x50;

	if (setuppointers() && fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Grand Theft Auto IV (v1.0.7.0). No identity support.");
}

static std::wstring description(L"Grand Theft Auto IV v1.0.7.0");
static std::wstring shortname(L"GTA IV");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin gtaivplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								  generic_unlock,      longdesc,    fetch };

static MumblePlugin2 gtaivplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &gtaivplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &gtaivplug2;
}
