// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

using namespace std;

procptr_t pos0ptr, pos1ptr, pos2ptr, faceptr, topptr;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	// char ccontext[128];
	float face_corrector[3];
	float top_corrector[3];

	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;

	/*
	   Z-Value is increasing when heading north
				  decreasing when heading south
	   X-Value is increasing when heading east
				  decreasing when heading west
	   Y-Value is increasing when going up
				  decreasing when going down
	*/
	ok = peekProc(pos2ptr, avatar_pos, 4) &&     // X
		 peekProc(pos1ptr, avatar_pos + 1, 4) && // Y
		 peekProc(pos0ptr, avatar_pos + 2, 4) && // Z
		 peekProc(faceptr, &face_corrector, 12) && peekProc(topptr, &top_corrector, 12);
	// peekProc((BYTE *) 0x0122E0B8, ccontext, 128);

	if (!ok)
		return false;

	// Convert to left-handed coordinate system
	avatar_front[0] = face_corrector[2];
	avatar_front[1] = face_corrector[1];
	avatar_front[2] = face_corrector[0];

	avatar_top[0] = top_corrector[2];
	avatar_top[1] = top_corrector[1];
	avatar_top[2] = top_corrector[0];

	// ccontext[127] = 0;
	// context = std::string(ccontext);

	// if (context.find(':')==string::npos)
	//	context.append(":UT3PORT");

	for (int i = 0; i < 3; i++) {
		camera_pos[i]   = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i]   = avatar_top[i];
	}

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	pos0ptr = pos1ptr = pos2ptr = faceptr = topptr = 0;

	if (!initialize(pids, L"UT2004.exe", L"Engine.dll"))
		return false;

	procptr_t ptraddress = pModule + 0x4A44FC;
	procptr_t ptr2       = peekProcPtr(ptraddress);
	procptr_t ptr3       = peekProcPtr(ptr2 + 0xCC);
	procptr_t baseptr    = ptr3 + 0x1C8;

	pos0ptr = baseptr;
	pos1ptr = baseptr + 0x4;
	pos2ptr = baseptr + 0x8;

	faceptr = baseptr + 0x18;

	topptr = baseptr + 0x24;

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
	return std::wstring(L"Supports Unreal Tournament 2004 (v3369). No context or identity support yet.");
}

static std::wstring description(L"Unreal Tournament 2004 (v3369)");
static std::wstring shortname(L"Unreal Tournament 2004");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin ut2004plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								   generic_unlock,      longdesc,    fetch };

static MumblePlugin2 ut2004plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &ut2004plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &ut2004plug2;
}
