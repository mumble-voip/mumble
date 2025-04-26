// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2012, dark_skeleton (d-rez) <dark.skeleton@gmail.com>
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

/*
	Arrays of bytes to find addresses accessed by respective functions so we don't have to blindly search for addresses
   after every update Remember to disable scanning only the writable memory in CE! We're searching for functions here,
   not values! This is not the final version and it doesn't distinguish camera and avatar positions. No identity support
   yet.

	Camera front vector function:		89 16 8B 54 24 20 89 46 04 8B 44 24 24
	Host IP:PORT changes to 'bot' when not ingame

	Valid addresses from v0.9.8.0

	Camera front vector address:				0x141bc20
	Camera top vector address:						+0xC (offset, not pointer)
	Camera position vector address:					+0x18 (offset, not pointer)
	Host IP:PORT address:						pbcl.dll + 0xB8C57

	TODO: Find Avatar position, front, top vectors, protect against version change (find a random pointer to check),
   distinguish spectator and normal mode
*/

static procptr_t camfrontptr = 0x141bc20;
static procptr_t camtopptr   = camfrontptr + 0xC;
static procptr_t camptr      = camfrontptr + 0x18;
static procptr_t hostipportptr;

static std::string prev_hostipport;

static char state = 0; // 1 if connected to a server, 0 if not

static bool calcout(float *cam, float *camfront, float *camtop, float *ocam, float *ocamfront, float *ocamtop) {
	// Seems Blacklight is in centimeters? ;o Well it's not inches for sure :)
	for (int i = 0; i < 3; i++) {
		ocam[i]      = cam[i] / 100.00f;
		ocamfront[i] = camfront[i];
		ocamtop[i]   = camtop[i];
	}

	return true;
}

static bool refreshPointers(void) {
	hostipportptr = 0;

	hostipportptr = pModule + 0xB8C57;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring & /*identity*/) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float cam[3], camtop[3], camfront[3];
	bool ok;
	char hostipport[22];

	ok = peekProc(camfrontptr, camfront, 12) && peekProc(camtopptr, camtop, 12) && peekProc(hostipportptr, hostipport)
		 && peekProc(camptr, cam);

	if (!ok)
		return false;

	hostipport[sizeof(hostipport) - 1] = '\0';
	if (hostipport != prev_hostipport) {
		context.clear();
		state = 0;

		prev_hostipport = hostipport;

		if (strcmp(hostipport, "") != 0 && strcmp(hostipport, "bot") != 0) {
			std::ostringstream contextstream;
			contextstream << "{\"ipport\": \"" << hostipport << "\"}";
			context = contextstream.str();

			state = 1;
		}
	}

	if (!state) // report positional data only when on a server!
		return true;

	calcout(cam, camfront, camtop, camera_pos, camera_front, camera_top); // calculate proper position values
	for (int i = 0; i < 3; i++) {
		avatar_front[i] = camera_front[i];
		avatar_pos[i]   = camera_pos[i];
		avatar_top[i]   = camera_top[i]; // copy values since we don't know avatar's vectors
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"BLR.exe", L"pbcl.dll"))
		return false;

	if (refreshPointers()) { // unlink plugin if this fails
		prev_hostipport.clear();
		float avatar_pos[3], avatar_front[3], avatar_top[3];
		float camera_pos[3], camera_front[3], camera_top[3];
		std::string context;
		std::wstring identity;

		if (fetch(avatar_pos, avatar_front, avatar_top, camera_pos, camera_front, camera_top, context, identity)) {
			prev_hostipport.clear(); // we need to do this again since fetch() above overwrites this (which results in
									 // empty context until next change)
			return true;
		}
	}

	generic_unlock();
	return false;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Blacklight: Retribution v0.9.8.0. No identity or avatar vectors support yet.");
}

static std::wstring description(L"Blacklight: Retribution (v0.9.8.0)");
static std::wstring shortname(L"Blacklight: Retribution");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin blacklightplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
									   generic_unlock,      longdesc,    fetch };

static MumblePlugin2 blacklightplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &blacklightplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &blacklightplug2;
}
