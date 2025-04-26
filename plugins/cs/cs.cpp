// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2009-2010, Imre Botos <zokny@users.sourceforge.net>
   Copyright (C) 2009-2012, Snares <snares@users.sourceforge.net>
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

using namespace std;

procptr_t pEngine;

/*  DESCRIPTION             ADDRESS             TYPE    VALUE

	* Client based addresses
	Connected to server:    client.dll+0xFC228  byte    (0 - not; 1 - connected)
	Position tuple:         client.dll+0x11D470 float   (x, y, z in inches)
	Orientation tuple:      client.dll+0x11D47C float   (v, h in degrees)
	Check string:           client.dll+0xE46F5  char    ("CSSpectatorGUI@@" | 16 bytes)
	Player state:           client.dll+0xFBC2C  byte    (0 - died; 1 - stand; 2 - duck; 3 - walk; 4 - run; 5 -
   crouchmove; 6 - fall) Team state:             client.dll+0xFC94C  byte    (0 - Spectator; 1 - Terrorist; 2 -
   Counter-Terrorist) ALT+TAB:                client.dll+0xFD05C  byte    (0 - tabbed; 1 - ingame) Map string:
   client.dll+0x11DCE4 str     ("maps/de_nuke.bsp")

	* Engine based addresses
	IP:Port string          hw.dll+0x697E60     str     (ip:port)
	ALT+TAB:                hw.dll+0x7486B4     byte    (0 - tabbed; 1 - ingame)
*/

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, string &context, wstring &) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;
	bool bConnected;
	char cPlayerState;
	char cHostAddr[40];
	float fViewHor, fViewVer;

	/*  Dust2
		INC         DEC     | Ver               Hor
	X - long        B       | 0     center      0/360   long
	Y - CT spawn    T spawn | -89   lookup      90      CT spawn
	Z - up          down    | 89    lookdown    180     B
							|                   270     T spawn
	*/

	ok = peekProc(pModule + 0x11D470, avatar_pos, 12) && peekProc(pModule + 0x11D47C, &fViewHor, 4)
		 && peekProc(pModule + 0x11D480, &fViewVer, 4) && peekProc(pModule + 0xFC228, &bConnected, 1)
		 && peekProc(pModule + 0xFBC2C, &cPlayerState, 1) && peekProc(pEngine + 0x697E60, cHostAddr, 40);
	if (!ok)
		return false;

	if (!bConnected)
		return true;

	/*	// FOR TEST ONLY - constant position (d2 CT middle, looking T spawn through doorgap)
		avatar_pos[0] = -380.359375f;
		avatar_pos[1] = 2188.1328125f;
		avatar_pos[2] = -91.96875f;
		fViewHor = 269,703369140625;
		fViewVer = -4,45587635040283;
	*/
	cHostAddr[39] = '\0';
	context       = cHostAddr;

	if (!cPlayerState)
		return true;

	if ((fViewVer < -720.0f) || (fViewVer > 720.0f) || (fViewHor < -720.0f) || (fViewHor > 720.0f))
		return false;

	fViewHor *= static_cast< float >(M_PI / 180.0f);
	fViewVer *= static_cast< float >(M_PI / 180.0f);

	avatar_pos[0] = avatar_pos[0] / 39.37f;
	avatar_pos[1] = avatar_pos[2] / 39.37f;
	avatar_pos[2] = avatar_pos[1] / 39.37f;

	avatar_front[0] = cos(fViewVer) * cos(fViewHor);
	avatar_front[1] = -sin(fViewHor);
	avatar_front[2] = sin(fViewVer) * cos(fViewHor);

	fViewHor -= static_cast< float >(M_PI / 2.0f);

	avatar_top[0] = cos(fViewVer) * cos(fViewHor);
	avatar_top[1] = -sin(fViewHor);
	avatar_top[2] = sin(fViewVer) * cos(fViewHor);

	for (int i = 0; i < 3; ++i) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"hl.exe", L"client.dll"))
		return false;

	pEngine = getModuleAddr(L"hw.dll");
	if (!pEngine)
		return false;

	// Gamecheck
	char sMagic[16];
	if (!peekProc(pModule + 0xE46F5, sMagic, 16) || strncmp("CSSpectatorGUI@@", sMagic, 16) != 0)
		return false;

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, context, identity))
		return true;

	generic_unlock();
	return false;
}

static const std::wstring longdesc() {
	return std::wstring(
		L"Supports Counter-Strike 1.6 (Steam). Context support based on server address. No identity support.");
}

static std::wstring description(L"Counter-Strike 1.6");
static std::wstring shortname(L"Counter-Strike");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin csplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
							   generic_unlock,      longdesc,    fetch };

static MumblePlugin2 csplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &csplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &csplug2;
}
