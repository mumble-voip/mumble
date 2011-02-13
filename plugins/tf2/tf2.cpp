/* Copyright (C) 2009-2010, Snares <snares@users.sourceforge.net>
   Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>

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

#include "../mumble_plugin_win32.h"

using namespace std;

BYTE *posptr;
BYTE *rotptr;
BYTE *stateptr;
BYTE *hostptr;

static bool calcout(float *pos, float *rot, float *opos, float *front, float *top) {
	float h = rot[0];
	float v = rot[1];

	if ((v < -360.0f) || (v > 360.0f) || (h < -360.0f) || (h > 360.0f))
		return false;

	h *= static_cast<float>(M_PI / 180.0f);
	v *= static_cast<float>(M_PI / 180.0f);

	// Seems TF2 is in inches. INCHES?!?
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

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, string &context, wstring &) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	float ipos[3], rot[3];
	bool ok;
	char state;
	char chHostStr[40];
	string sHost;
	wostringstream new_identity;
	ostringstream new_context;

	ok = peekProc(posptr, ipos, 12) &&
	     peekProc(rotptr, rot, 12) &&
	     peekProc(stateptr, &state, 1) &&
	     peekProc(hostptr, chHostStr, 40);

	if (!ok)
		return false;

	chHostStr[39] = 0;

	sHost.assign(chHostStr);
	if (sHost.find(':')==string::npos)
		sHost.append(":27015");

	new_context << "<context>"
	<< "<game>tf2</game>"
	<< "<hostport>" << sHost << "</hostport>"
	<< "</context>";
	context = new_context.str();

	/* TODO
	new_identity << "<identity>"
			<< "<name>" << "SAS" << "</name>"
		     << "</identity>";
	identity = new_identity.str(); */

	// Check to see if you are in a server and spawned
	if (state == 0 || state == 1 || state == 3)
		return true; // Deactivate plugin

	ok = calcout(ipos, rot, avatar_pos, avatar_front, avatar_top);
	if (ok) {
		for (int i=0;i<3;++i) {
			camera_pos[i] = avatar_pos[i];
			camera_front[i] = avatar_front[i];
			camera_top[i] = avatar_top[i];
		}
		return true;
	}

	return false;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {
	posptr = rotptr = NULL;

	if (! initialize(pids, L"hl2.exe", L"client.dll"))
		return false;

	BYTE *mod_engine=getModuleAddr(L"engine.dll");
	if (!mod_engine)
		return false;

	// Check if we really have TF2 running
	/*
		note that these are just examples of memory values, and may not be updated or correct
		position tuple:		client.dll+0x5753d8  (x,y,z, float)
		orientation tuple:	client.dll+0x4b691c  (v,h float)
		ID string:			client.dll+0x4eb30b = "teamJet@@" (9 characters, text)
		spawn state:        client.dll+0x4EABCC; (0 when at main menu, 1 when spectator, 3 when at team selection menu, and 6 or 9 when on a team (depending on the team side and gamemode), byte)
		host string: 		engine.dll+0x3c8124  (ip:port zero-terminated string; localhost:27015 if create a server ingame)
			note that memory addresses in this comment are for example only; the real ones are defined below
	*/

	// Remember addresses for later
	posptr = pModule + 0x6EFCC0;
	rotptr = pModule + 0x6F6544;
	stateptr = pModule + 0x606C64;
	hostptr = mod_engine + 0x3D3E94;

	/*
	// Gamecheck
	char sMagic[9];
	if (!peekProc(pModule + 0x617DE3, sMagic, 9) || strncmp("teamJet@@", sMagic, 9)!=0)
		return false;
	*/

	// Gamecheck
	char sMagic[13];
	if (!peekProc(mod_engine + 0x54A670, sMagic, 13) || strncmp("DemomanTaunts", sMagic, 13)!=0)
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
	return std::wstring(L"Supports TF2 build 4421. No identity support yet.");
}

static std::wstring description(L"Team Fortress 2 (Build 4421)");
static std::wstring shortname(L"Team Fortress 2");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin tf2plug = {
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

static MumblePlugin2 tf2plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &tf2plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &tf2plug2;
}
