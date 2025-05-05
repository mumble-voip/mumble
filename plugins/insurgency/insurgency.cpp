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

	// Seems Insurgency is in inches. INCHES?!?
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
				<< "<game>insurgency</game>"
				<< "<hostport>" << sHost << "</hostport>"
				<< "</context>";
	context = new_context.str();

	/* TODO
	new_identity << "<identity>"
				 << "<name>" << "SAS" << "</name>"
				 << "</identity>";
	identity = new_identity.str(); */

	// Check to see if you are spawned
	if (state == 0 || state == 1)
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

	// Check if we really have Insurgency running
	/*
		position tuple:		client.dll+0x47171c  (x,y,z, float)
		orientation tuple:	client.dll+0x4f1458  (v,h float)
		ID string:			client.dll+0x31a80a = "CombatWeapon@@" (14 characters, text)
		spawn state:        client.dll+0x4aee58  (0 when at main menu, 1 when not spawned, 4 to 5 when spawned)
		host string: 		engine.dll+0x3909c4 (ip:port zero-terminated string)
	*/

	// Remember addresses for later
	posptr   = pModule + 0x4F1EE0;
	rotptr   = pModule + 0x4F247C;
	stateptr = pModule + 0x4AFEB8;
	hostptr  = mod_engine + 0x3909A4;

	// Gamecheck
	char sMagic[14];
	if (!peekProc(pModule + 0x46B512, sMagic, 14) || strncmp("CombatWeapon@@", sMagic, 14) != 0)
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
	return std::wstring(L"Supports Insurgency: Modern Infantry Combat build 4044. No identity support yet.");
}

static std::wstring description(L"Insurgency: Modern Infantry Combat (Build 4044)");
static std::wstring shortname(L"Insurgency: Modern Infantry Combat");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin insurgencyplug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
									   generic_unlock,      longdesc,    fetch };

static MumblePlugin2 insurgencyplug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &insurgencyplug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &insurgencyplug2;
}
