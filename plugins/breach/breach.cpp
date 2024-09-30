// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2011, Mark-Willem Jansen <rawnar@users.sourceforge.net>
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

procptr_t posptr, frontptr, topptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	bool ok;

	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// State value is working most of the time
	unsigned char state;
	ok = peekProc(0x11146bb, &state, sizeof(state));
	if (!ok)
		return false;
	// State is 255 when you are in a menu
	if (state == 255)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them

	// coordinate systems is already left handed so no change needed
	ok = peekProc(posptr, avatar_pos, 12) && peekProc(frontptr, avatar_front, 12) && peekProc(topptr, avatar_top, 12);
	if (!ok)
		return false;

	// The ingame units are inches
	for (int i = 0; i < 3; i++)
		avatar_pos[i] /= 39.37f;

	// As it is a First Person Shooter the avatar and camera are the same
	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	posptr = frontptr = topptr = 0;

	if (!initialize(pids, L"Breach.exe", L"fmodex.dll"))
		return false;

	// Checking the version of Breach
	char version[12];
	if ((!peekProc(0x1118f98, &version, sizeof(version))) || (strncmp("breach 1.1.0", version, sizeof(version)) != 0)) {
		generic_unlock();
		return false;
	}

	// Setting the pointers for the avatar information
	procptr_t ptr1 = peekProcPtr(pModule + 0x177980);
	if (ptr1 == 0) {
		generic_unlock();
		return false;
	}
	posptr   = ptr1 + 0x9200;
	frontptr = ptr1 + 0x9248;
	topptr   = ptr1 + 0x9230;

	// Final check by calling fetch.
	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;
	if (!fetch(apos, afront, atop, cpos, cfront, ctop, context, identity)) {
		generic_unlock();
		return false;
	}

	return true;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Breach v1.1.0, steam version. Context and Identity on supported.");
}

static std::wstring description(L"Breach v1.1.0");
static std::wstring shortname(L"Breach");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin breachplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								   generic_unlock,      longdesc,    fetch };

static MumblePlugin2 breachplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &breachplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &breachplug2;
}
