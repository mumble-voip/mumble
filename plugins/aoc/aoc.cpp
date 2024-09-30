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
	float h = rot[0];
	float v = rot[1];

	if ((v < -360.0f) || (v > 360.0f) || (h < -360.0f) || (h > 360.0f))
		return false;

	h *= static_cast< float >(M_PI / 180.0f);
	v *= static_cast< float >(M_PI / 180.0f);

	// Seems AOC is in inches. INCHES?!?
	opos[0] = pos[0] / 39.37f;
	opos[1] = pos[2] / 39.37f;
	opos[2] = pos[1] / 39.37f;

	front[0] = cos(v) * cos(h);
	front[1] = -sin(h);
	front[2] = sin(v) * cos(h);

	h -= static_cast< float >(M_PI / 2.0f);

	top[0] = cos(v) * cos(h);
	top[1] = -sin(h);
	top[2] = sin(v) * cos(h);

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, string &context, wstring &) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], rot[3];
	bool ok;
	char state;
	char chHostStr[40];
	string sHost;
	wostringstream new_identity;
	ostringstream new_context;

	ok = peekProc(posptr, ipos, 12) && peekProc(rotptr, rot, 12) && peekProc(stateptr, &state, 1)
		 && peekProc(hostptr, chHostStr, 40);
	if (!ok)
		return false;

	chHostStr[39] = 0;

	sHost.assign(chHostStr);
	if (sHost.find(':') == string::npos)
		sHost.append(":27015");

	new_context << "<context>"
				<< "<game>aoc</game>"
				<< "<hostport>" << sHost << "</hostport>"
				<< "</context>";
	context = new_context.str();

	/* TODO
	new_identity << "<identity>"
				 << "<name>" << "SAS" << "</name>"
				 << "</identity>";
	identity = new_identity.str(); */

	// Check to see if you are spawned
	if (state == 0 || state == 1 || state == 2)
		return true; // Deactivate plugin

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

	// Check if we really have AOC running
	/*
		position tuple:     client.dll+0x748e14  (x,y,z, float)
		orientation tuple:  client.dll+0x73dc9c  (v,h float)
		ID string:          client.dll+0x7071e8 = "ageofchivalry" (13 characters, text)
		spawn state:        client.dll+0x6d4334  (0 when at main menu, 1 when at team selection, 2 when not spawned,
												  between 6 and 7 when spawned)
		context offsets:    engine.dll + 0x3bf69f = (ip:port)
							server.dll + 0x71a0a4 = (team info, 3 Mason, 1 Agathia)
	*/

	// Remember addresses for later
	posptr   = pModule + 0x748e14;
	rotptr   = pModule + 0x73dc9c;
	stateptr = pModule + 0x6d4334;
	hostptr  = mod_engine + 0x3C2A84;

	// Gamecheck
	char sMagic[13];
	if (!peekProc(pModule + 0x7071e8, sMagic, 13) || strncmp("ageofchivalry", sMagic, 13) != 0)
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
	return std::wstring(L"Supports AOC Build 4104. No identity support yet.");
}

static std::wstring description(L"Age of Chivalry (Build 4104)");
static std::wstring shortname(L"Age of Chivalry");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin aocplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								generic_unlock,      longdesc,    fetch };

static MumblePlugin2 aocplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &aocplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &aocplug2;
}
