// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

/* Copyright (C) 2010, Mark-Willem Jansen <rawnar@users.sourceforge.net>
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

procptr_t posptr, frontptr, topptr, contextptraddress, stateaddress, loginaddress;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &) {
	static bool loggedin = false;
	static procptr_t contextptr;
	bool ok;

	// Zeroing the floats
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	// When you log in the context pointer needs to be set.
	char login;
	ok = peekProc(loginaddress, &login, sizeof(login));
	if (!ok)
		return false;

	if (login == 0) {
		loggedin = false;
	} else if (!loggedin) {
		procptr_t ptr1 = peekProcPtr(contextptraddress);
		procptr_t ptr2 = peekProcPtr(ptr1 + 0x28c);
		procptr_t ptr3 = peekProcPtr(ptr2 + 0x210);
		if (ptr3 != 0)
			loggedin = true; // pointer set
		contextptr = ptr3 + 0x2c;
	}

	// Magic State value
	char state;
	ok = peekProc(stateaddress, &state, sizeof(state));
	if (!ok)
		return false;

	// State is 0 when you are in the main menu and 1 when your are not.
	if (state == 0)
		return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

	float pos_corrector[3];
	float front_corrector[3];
	float top_corrector[3];
	ok = peekProc(posptr, &pos_corrector, 12) && peekProc(frontptr, &front_corrector, 12)
		 && peekProc(topptr, &top_corrector, 12);
	if (!ok)
		return false;

	// coordinate systems:
	// "ENU"	"Mumble"	"In game"
	// East		x+ [0]		z+ [2]
	// North	z+ [2]		x+ [0]
	// Up		y+ [1]		y+ [1]
	avatar_pos[0] = pos_corrector[2];
	avatar_pos[1] = pos_corrector[1];
	avatar_pos[2] = pos_corrector[0];

	for (int i = 0; i < 3; i++)
		avatar_pos[i] /= 100.0f; // Unreal Unit is set to centimeters

	avatar_front[0] = front_corrector[2];
	avatar_front[1] = front_corrector[1];
	avatar_front[2] = front_corrector[0];

	avatar_top[0] = top_corrector[2];
	avatar_top[1] = top_corrector[1];
	avatar_top[2] = top_corrector[0];

	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	if (loggedin) {
		char ccontext[64];
		ok = peekProc(contextptr, &ccontext, sizeof(ccontext));
		if (!ok)
			return false;

		std::string new_context;
		new_context.assign(ccontext, sizeof(ccontext));
		if (new_context.find("bderlandspc") != std::string::npos)
			new_context.erase(0, new_context.find("bderlandspc"));
		if (new_context.find(":7777") != std::string::npos)
			new_context.erase(new_context.find(":7777") + 5);
		context = new_context;
	}

	return ok;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	posptr = frontptr = topptr = contextptraddress = stateaddress = loginaddress = 0;

	if (!initialize(pids, L"Borderlands.exe"))
		return false;

	// Trying to assess which version of Borderlands is running.
	char version[6];
	if (!peekProc(0x01f16ce8, &version, sizeof(version))) {
		generic_unlock();
		return false;
	}

	procptr_t ptraddress;
	if (strncmp("the cl", version, sizeof(version)) == 0) { // retail version
		ptraddress        = 0x01f73744;
		stateaddress      = 0x01f9bb18;
		contextptraddress = 0x01fd7398;
		loginaddress      = 0x01fd83a8;
	} else if (strncmp("Tir-ku", version, sizeof(version)) == 0) { // steam version
		ptraddress        = 0x01f705c4;
		stateaddress      = 0x01f98998;
		contextptraddress = 0x01fd4218;
		loginaddress      = 0x01fd5220;
	} else if (strncmp("german", version, sizeof(version)) == 0) { // german version
		ptraddress        = 0x01f72744;
		stateaddress      = 0x01f9ab18;
		contextptraddress = 0x01fd6398;
		loginaddress      = 0x01fd73a8;
	} else { // unknown version
		generic_unlock();
		return false;
	}

	procptr_t ptr1 = peekProcPtr(ptraddress);
	if (ptr1 == 0) {
		generic_unlock();
		return false;
	}

	posptr   = ptr1 + 0x9200;
	frontptr = ptr1 + 0x9248;
	topptr   = ptr1 + 0x9230;

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
	return std::wstring(
		L"Supports Borderlands v1.40, including german and steam version. Context string is used with online games.");
}

static std::wstring description(L"Borderlands v1.40");
static std::wstring shortname(L"Borderlands");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin borderlandsplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
										generic_unlock,      longdesc,    fetch };

static MumblePlugin2 borderlandsplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &borderlandsplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &borderlandsplug2;
}
