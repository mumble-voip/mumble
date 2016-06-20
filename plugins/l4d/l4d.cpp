// Copyright 2005-2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32_x86.h"

procptr32_t posptr, rotptr;

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
	float h = rot[0];
	float v = rot[1];

	if ((v < -180.0f) || (v > 180.0f) || (h < -180.0f) || (h > 180.0f))
		return false;

	h *= static_cast<float>(M_PI / 180.0f);
	v *= static_cast<float>(M_PI / 180.0f);

	// Seems L4D is in inches. INCHES?!?
	opos[0] = pos[0] / 39.37f;
	opos[1] = pos[2] / 39.37f;
	opos[2] = pos[1] / 39.37f;

	front[0] = cos(v) * cos(h);
	front[1] = -sin(h);
	front[2] = sin(v) * cos(h);

	h -= static_cast<float>(M_PI / 2.0f);

	top[0] = cos(v) * cos(h);
	top[1] = -sin(h);
	top[2] = sin(v) * cos(h);
	/*
		printf("Poll\n");
		printf("%f %f %f : %f %f\n",pos[0],pos[1],pos[2], rot[0], rot[1]);
		printf("%f %f %f :: %.2f %.2f %.2f :: %.2f %.2f %.2f\n", opos[0], opos[1], opos[2], front[0], front[1], front[2], top[0], top[1], top[2]);
	*/
	return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	posptr = rotptr = NULL;

	if (! initialize(pids, L"left4dead.exe", L"client.dll"))
		return false;

	posptr = pModule + 0x596E30;
	rotptr = pModule + 0x596E3C;

	float pos[3];
	float rot[3];

	bool ok = peekProc(posptr, pos, 12) &&
	          peekProc(rotptr, rot, 12);

	if (ok) {
		float opos[3], top[3], front[3];
		return calcout(pos, rot, opos, top, front);
	}

	generic_unlock();

	return false;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], rot[3];
	bool ok;

	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12);

	if (ok) {
		int res = calcout(ipos, rot, avatar_pos, avatar_front, avatar_top);
		if (res) {
			for (int i=0;i<3;++i) {
				camera_pos[i] = avatar_pos[i];
				camera_front[i] = avatar_front[i];
				camera_top[i] = avatar_top[i];

				// Example only -- only set these when you have sane values, and make sure they're pretty constant (every change causes a sever message).
				context = std::string("server/map/blah");
				identity = std::wstring(L"STEAM_1:2:3456789");
			}
			return res;
		}
	}

	return false;
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports L4D version 4440 only. Supports no fancy stuff.");
}

static std::wstring description(L"Left 4 Dead (Build 4440)");
static std::wstring shortname(L"Left 4 Dead");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin l4dplug = {
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

static MumblePlugin2 l4dplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &l4dplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &l4dplug2;
}
