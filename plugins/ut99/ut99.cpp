// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2012, Moritz Schneeweiss

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

#include <utf8/cpp11.h>

procptr_t posptr;
procptr_t frtptr;
procptr_t topptr;

static bool cross(float *a, float *b, float *c) {
	if (a == 0 || b == 0 || c == 0)
		return false;

	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];

	return true;
}

static void norm(float *a) {
	float length = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	if (length != 0.0f) {
		a[0] /= length;
		a[1] /= length;
		a[2] /= length;
	}
}

static bool correctFront(float *front, float *top) {
	float n[3] = { 0 };

	if (top[1] < 0) {
		front[0] *= -1;
		front[1] *= -1;
		front[2] *= -1;
	}

	cross(front, top, n);
	norm(n);
	if (n[0] == 0 && n[1] == 0 && n[2] == 0) {
		front[0] = 0;
		front[1] = -1;
		front[2] = 0;
	} else {
		cross(top, n, front);
		norm(front);
	}

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring & /*identity*/) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = 0.0f;

	char state;
	bool ok;

	ok = peekProc(pModule + 0x290557, state); // Magical state value
	if (!ok)
		return false;

	if (state != 8) {
		context.clear();
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.
	}

	// Create containers to stuff our raw data into, so we can convert it to Mumble's coordinate system
	float pos_corrector[3];
	float front_corrector[3];
	float top_corrector[3];

	// Peekproc and assign game addresses to our containers, so we can retrieve positional data
	ok = peekProc(posptr, pos_corrector) && peekProc(frtptr, front_corrector) && peekProc(topptr, top_corrector);

	if (!ok)
		return false;

	// Convert to left-handed coordinate system

	avatar_pos[0] = -pos_corrector[0];
	avatar_pos[1] = pos_corrector[2];
	avatar_pos[2] = pos_corrector[1];

	for (int i = 0; i < 3; i++)
		avatar_pos[i] /= 64.0f; // Scale to meters

	avatar_front[0] = -front_corrector[1];
	avatar_front[1] = front_corrector[2];
	avatar_front[2] = -front_corrector[0];

	avatar_top[0] = top_corrector[0];
	avatar_top[1] = -top_corrector[2];
	avatar_top[2] = -top_corrector[1];

	correctFront(avatar_front, avatar_top);

	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	// Read server name
	procptr_t cbase = peekProcPtr(pModule + 0x00290550);
	procptr_t cptr0 = peekProcPtr(cbase + 0x30);
	procptr_t cptr1 = peekProcPtr(cptr0 + 0x73C);
	procptr_t cptr2 = peekProcPtr(cptr1 + 0x244);

	std::u16string servername;
	servername.resize(60);
	if (!peekProc(cptr2, &servername[0], servername.size())) {
		return false;
	}

	std::ostringstream contextss;
	contextss << "{";
	try {
		contextss << "\"servername\":\"" << utf8::utf16to8(servername) << "\"";
	} catch (const utf8::invalid_utf16 &) {
		contextss << "\"servername\":\" null";
	}
	contextss << "}";

	context = contextss.str();

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"UnrealTournament.exe", L"Engine.dll"))
		return false;

	procptr_t base = pModule + 0x290584;
	posptr         = base;
	frtptr         = base + 0x0C;
	topptr         = base + 0x18;

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
	return std::wstring(L"Supports Unreal Tournament (v436). No identity support yet.");
}

static std::wstring description(L"Unreal Tournament (v436)");
static std::wstring shortname(L"Unreal Tournament");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin ut99plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								 generic_unlock,      longdesc,    fetch };

static MumblePlugin2 ut99plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &ut99plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &ut99plug2;
}
