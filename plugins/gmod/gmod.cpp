// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

using namespace std;

procptr_t posptr, rotptr, stateptr, hostptr;

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
	float v = rot[0];
	float h = rot[1];

	if ((v < -360.0f) || (v > 360.0f) || (h < -360.0f) || (h > 360.0f))
		return false;

	v *= static_cast< float >(M_PI / 180.0f);
	h *= static_cast< float >(M_PI / 180.0f);

	// Seems Gmod is in inches. INCHES?!?
	opos[0] = pos[0] / 39.37f;
	opos[1] = pos[2] / 39.37f;
	opos[2] = pos[1] / 39.37f;

	front[0] = cos(h) * cos(v);
	front[1] = -sin(v);
	front[2] = sin(h) * cos(v);

	v -= static_cast< float >(M_PI / 2.0f);

	top[0] = cos(h) * cos(v);
	top[1] = -sin(v);
	top[2] = sin(h) * cos(v);

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &context, std::wstring &) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], rot[3];
	bool ok;
	// char state;
	char chHostStr[40];
	string sHost;
	ostringstream new_context;

	ok = peekProc(posptr, ipos, 12)
		 && peekProc(rotptr, rot, 12)
		 //&& peekProc(stateptr, &state, 1)
		 && peekProc(hostptr, chHostStr, 40);
	if (!ok)
		return false;

	chHostStr[39] = 0;

	sHost.assign(chHostStr);
	if (sHost.find(':') == string::npos)
		sHost.append(":27015");

	new_context << "<context>"
				<< "<game>gmod</game>"
				<< "<hostport>" << sHost << "</hostport>"
				<< "</context>";
	context = new_context.str();
	/*
		// Check to see if you are spawned
		if (state != 18)
			return true; // Deactivate plugin
	*/
	ok = calcout(ipos, rot, avatar_pos, avatar_front, avatar_top);
	if (ok) {
		for (int i = 0; i < 3; ++i) {
			camera_pos[i]   = avatar_pos[i];
			camera_front[i] = avatar_front[i];
			camera_top[i]   = avatar_top[i];
		}
		return true;
	}

	return false;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	posptr = rotptr = 0;

	if (!initialize(pids, L"hl2.exe", L"client.dll"))
		return false;

	procptr_t mod_engine = getModuleAddr(L"engine.dll");
	if (!mod_engine)
		return false;

	// Remember addresses for later
	// position tuple: x,y,z, float
	// client.dll+0x6856B8
	posptr = pModule + 0x6856B8;
	// orientation tuple: v,h,? float
	// v: up = -90°, down = 90°; h (rotation): -180 - 180°
	// client.dll+0x5B5914
	rotptr = pModule + 0x5B5914;
	// spawn state: client.dll+0x?????? - 0 when at main menu, 2 when not spawned, 15 to 14 when spawned, byte
	// This could not be verified/found by Kissaki
	stateptr = mod_engine + 0x375565;
	// ID string; Game name "garrysmod"
	// engine.dll+0x6622DC
	procptr_t idptr = mod_engine + 0x6622DC;
	// host string: String in form "ip:port".
	// engine.dll+0x49176C
	hostptr = mod_engine + 0x49176C;

	// Gamecheck
	const char ID[] = "garrysmod";
	char sMagic[18];
	if (!peekProc(idptr, sMagic, sizeof(ID)) || strncmp(ID, sMagic, sizeof(ID)) != 0)
		return false;

	// Check if we can get meaningful data from it
	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	wstring sidentity;
	string scontext;

	if (fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	// Exe build: 20:55:48 Jun 17 2014 (5692) (4000)
	return std::wstring(L"Supports Gmod 11 build 4426. No identity support yet.");
}

static std::wstring description(L"Garry's Mod 11 (Build 5692)");
static std::wstring shortname(L"Garry's Mod 11");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin gmodplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								 generic_unlock,      longdesc,    fetch };

static MumblePlugin2 gmodplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &gmodplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &gmodplug2;
}
