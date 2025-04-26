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
   after every update Remember to disable scanning writable memory only in CE! We're searching for functions here, not
   values! Current addresses as of version 1.0.0.145

	Camera position vector address: F3 0F 11 03 F3 0F 10 44 24 14 D9 5C 24 28			:00B738E8
	Camera front vector address: campos+0x14 (offset, not pointer!)

	D9 5F 40 D9 46 04 D9 5F 44 D9 46 08 D9 5F 48 59 C3 CC (non-static! NEEDS POINTER)	:00E22E90
	Avatar front vector address: 		+0x2acc [offset can change]

	D9 9E E0 01 00 00 D9 40 70															:02F5E5F8
	Avatar position vector address:		+0x1e0 (pointer offset) [offset can change]

	IP is kept as text @ hostipptr
	PORT is kept as a 4-byte decimal value @ hostportptr
	Look for a non-unicode string that will contain server's IP. 28 bytes further from IP, there should be server's port
																						:0AF69D18
	PORT:					+0x1C (offset, not pointer!)
*/

static procptr_t posptr, afrontptr, tmpptr;

static procptr_t posptr_     = 0x2F5E5F8;
static procptr_t camptr      = 0xB738E8;
static procptr_t camfrontptr = 0x14;
static procptr_t gameptr     = 0xE22E90;

static procptr_t hostipptr   = 0xAF69D18;
static procptr_t hostportptr = hostipptr + 0x1C;

static std::string prev_hostip; // These should never change while the game is running, but just in case...
static int prev_hostport;

static bool calcout(float *pos, float *cam, float *opos, float *ocam) {
	// Seems League of Legends is in centimeters? ;o Well it's not inches for sure :)
	for (int i = 0; i < 3; i++) {
		opos[i] = pos[i] / 100.00f;
		ocam[i] = cam[i] / 100.00f;
	}

	return true;
}

static bool refreshPointers(void) {
	posptr = afrontptr = tmpptr = 0;

	// Avatar front vector pointer
	tmpptr = peekProcPtr(gameptr);
	if (!tmpptr)
		return false; // Something went wrong, unlink

	afrontptr = tmpptr + 0x2acc;

	// Avatar position vector
	tmpptr = peekProcPtr(posptr_);
	if (!tmpptr)
		return false; // Something went wrong, unlink

	posptr = tmpptr + 0x1e0;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring & /*identity*/) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], cam[3];
	int hostport;
	char hostip[16];
	bool ok;

	// Player not in game (or something broke), unlink
	if (!peekProcPtr(gameptr))
		return false;

	ok = peekProc(camfrontptr, camera_front, 12) && peekProc(camptr, cam, 12) && peekProc(posptr, ipos, 12)
		 && peekProc(afrontptr, avatar_front, 12) && peekProc(hostipptr, hostip) && peekProc(hostportptr, &hostport, 4);

	if (!ok)
		return false;

	// Ensure strings are zero terminated
	hostip[sizeof(hostip) - 1] = '\0';

	calcout(ipos, cam, avatar_pos, camera_pos); // calculate proper position values

	if (hostip != prev_hostip || hostport != prev_hostport) {
		prev_hostip   = hostip;
		prev_hostport = hostport;

		if (strcmp(hostip, "") != 0) {
			std::ostringstream contextstream;
			contextstream << "{\"ipport\": \"" << hostip << ":" << hostport << "\"}";
			context = contextstream.str();
		} else {
			context.clear();
		}
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"League of Legends.exe"))
		return false;

	if (refreshPointers()) { // unlink plugin if this fails
		prev_hostip.clear();
		prev_hostport = 0;

		float avatar_pos[3], avatar_front[3], avatar_top[3];
		float camera_pos[3], camera_front[3], camera_top[3];
		std::string context;
		std::wstring identity;

		if (fetch(avatar_pos, avatar_front, avatar_top, camera_pos, camera_front, camera_top, context, identity)) {
			prev_hostip.clear(); // we need to do this again since fetch() above overwrites this (which results in empty
								 // context until next change)
			prev_hostport = 0;
			return true;
		}
	}

	generic_unlock();
	return false;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports League of Legends v1.0.0.145 with context. No identity support.");
}

static std::wstring description(L"League of Legends (v1.0.0.145)");
static std::wstring shortname(L"League of Legends");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin lolplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								generic_unlock,      longdesc,    fetch };

static MumblePlugin2 lolplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &lolplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &lolplug2;
}
