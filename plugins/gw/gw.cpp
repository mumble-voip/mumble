/* Copyright (C) 2012, dark_skeleton (d-rez) <dark.skeleton@gmail.com> 

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

static BYTE *posptr;
static BYTE *camptr;
static BYTE *frontptr;
static BYTE *camfrontptr;

static BYTE *magicptr;
static BYTE *areaptr;
static BYTE *characternameptr;

static bool calcout(float *pos, float *front, float *cam, float *camfront, float *opos, float *ofront, float *ocam, float *ocamfront) {

	// Seems Guild Wars is in... inches, yeah :) ---> same as in GW2, proof here: http://www.guildwars2guru.com/topic/21519-reddit-ama-all-questions-answers (question #31)
	// coordinate Y is swapped with Z
	// Y is negative (looks like somewhere underground is 0.00 and land is for example -120. When we climb up a hill, it goes down (e.g. -130), and when we walk downhill, it goes up (e.g. -100)

	opos[0] = pos[0] / 39.37f;	
	opos[1] = -pos[2] / 39.37f;
	opos[2] = pos[1] / 39.37f;
		
	ocam[0] = cam[0] / 39.37f;
	ocam[1] = -cam[2] / 39.37f;
	ocam[2] = cam[1] / 39.37f;

	ofront[0] = front[0];
	ofront[1] = -front[2];
	ofront[2] = front[1];

	ocamfront[0] = camfront[0];
	ocamfront[1] = -camfront[2];
	ocamfront[2] = camfront[1];

	return true;
}

static bool refreshPointers(void)
{
	// We can find all vectors (camera position, camera front, avatar position, avatar front) in just one place, yay!

	camfrontptr = camptr = posptr = frontptr = magicptr = characternameptr = areaptr = NULL;
	
	camptr = (BYTE *) 0xd551b8;
	posptr = (BYTE *) 0xd551c4; // camptr + 0xC
	camfrontptr = (BYTE *) 0xd551d0; // posptr + 0xC
	frontptr = (BYTE *) 0xd551dc; // camfrontptr + 0xC

	/* Magic Pointer: 0xa3fad8
		25/26/27 when ingame (differs for cities and explorable areas/missions). I don't know how it works exactly, because sometimes it's 26 for cities and 25 for explorables, while another time it's 27 for cities and 26 for explorables. Key fact: it is different for cities and explorable areas/missions :)
		0 when logged in / in character select / in loading screen
		1 when not logged in (in login screen)
	*/
	magicptr = (BYTE *) 0xa3fad8;

	/* Area Pointer: 0xd54e54
		This is a 4-byte decimal stating which area we are in. Note however, that some missions have the same area assigned as cities, therefore we need our Magic Pointer to distinguish where exactly we are to specify context properly.
	*/
	areaptr = (BYTE *) 0xd54e54;

	/* Character Name Pointer: 0xa2a440
		This is a 19-characters long unicode string which contains a player's character name. This is unique for every character, but can change while ingame (players can switch characters)
	*/
	characternameptr = (BYTE *) 0xa2a440;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

	bool ok;
	float cam[3],pos[3],front[3],camfront[3];
	char state;
	int areaid;
	wchar_t charactername [20];

	ok = peekProc(camptr,cam) &&
		 peekProc(posptr,pos) &&
		 peekProc(camfrontptr,camfront) &&
		 peekProc(frontptr,front) &&
		 peekProc(magicptr,&state,1) &&
		 peekProc(areaptr,&areaid,4) &&
		 peekProc(characternameptr,charactername);

	if (ok) {
		if (state == 0 || state == 1 || areaid == 0) // Player not ingame
		{
			context.clear();
			identity.clear();
			return true;
		}
		int res = calcout(pos, front, cam, camfront, avatar_pos, avatar_front, camera_pos, camera_front);
		if (res) {
			{
				std::ostringstream _context;
				_context << "{\"areaid\": " << areaid << ","
						 << "\"magic\": " << int(state) << "}";
				context = _context.str();
				identity = std::wstring(charactername);
			}
			return res;
		}
	}

	return false;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"Gw.exe"))
		return false;

	if (!refreshPointers()) { generic_unlock(); return false; }// unlink plugin if this fails

	float cam[3],pos[3],front[3],camfront[3];
	float ocam[3],opos[3],ofront[3],ocamfront[3];
	char state;
	int areaid;
	wchar_t charactername [20];

	bool ok = peekProc(camptr,cam) &&
		 peekProc(posptr,pos) &&
		 peekProc(camfrontptr,camfront) &&
		 peekProc(frontptr,front) &&
		 peekProc(magicptr,&state,1) &&
		 peekProc(areaptr,&areaid,4) &&
		 peekProc(characternameptr,charactername);

	if (ok) {
		return calcout(pos, front, cam, camfront, opos, ofront, ocam, ocamfront);
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Guild Wars build 35,787 with context and identity support.");
}

static std::wstring description(L"Guild Wars b35787");
static std::wstring shortname(L"Guild Wars");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin gwplug = {
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

static MumblePlugin2 gwplug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &gwplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &gwplug2;
}
