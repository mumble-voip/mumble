/* Copyright (C) 2010-2011, Snares <snares@users.sourceforge.net>
   Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2011, Ryan Austin <ryan@gameforcecenters.com>

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
BYTE *pmodule_bf3;

// Magic ptrs
BYTE* const state_ptr = (BYTE *) 0x023C7A9C;

// Vector ptrs

// Context ptrs
BYTE* const ipport_ptr = (BYTE*)0x02396F40;

// Identity ptrs
BYTE *team_state_ptr;
BYTE *squad_state_ptr;

inline bool resolve_ptrs()
{
    team_state_ptr = squad_state_ptr = NULL;

    /*
    Magic:
        state : 0x023C7A9C                                                      BYTE		2 when playing
                                                                                            3 while in menu/dead

    Context:
        IP:Port of server: 0x02396F40                                           char[128]	ip:port of the server

    Identity:
        Squad state: BF3.exe+0x01F24F58 + 0x1C + 0xBC + 0x36C + 0x8 + 0x104     BYTE		0 is not in squad; 1 is in Alpha squad, 2 Bravo, ... , 9 India
        Team state: todo                                                        BYTE		0 is blufor (US team, for example), 1 is opfor (Insurgents)
    */

    BYTE *base_bf3 = peekProc<BYTE *>(pmodule_bf3 + 0x01F24F58);
    if(!base_bf3) return false;

    BYTE *ptr1 = peekProc<BYTE *>(base_bf3 + 0x1C);
    BYTE *ptr2 = peekProc<BYTE *>(ptr1 + 0xBC);
    BYTE *ptr3 = peekProc<BYTE *>(ptr2 + 0x36C);
    BYTE *ptr4 = peekProc<BYTE *>(ptr3 + 0x8);

    squad_state_ptr = ptr4 + 0x104;

    return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
	for (int i=0;i<3;i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

    char ccontext[128];
	char state;
    BYTE squadnum;
	bool ok;

    ok = peekProc(state_ptr, &state, 1); // State value
	if (! ok)
		return false;
	
    if(!resolve_ptrs())
        return false;

	if ( state != 2 )
		return true;	// This results in all vectors being zero which tells Mumble to ignore them.

	ok = peekProc((BYTE *) 0x023C7A30, avatar_pos, 12) &&
		peekProc((BYTE *)  0x023C7A60, avatar_front, 12) &&
        peekProc((BYTE *)  0x023C7A50, avatar_top, 12) &&
        peekProc(squad_state_ptr,&squadnum,1) &&
        peekProc(ipport_ptr,ccontext,128);

	if (! ok)
		return false;

    ccontext[127] = 0;
    if (ccontext[0] != '0') {
        ostringstream ocontext;
        ocontext << "{ \"ipport\": \"" << ccontext << "\"}";

        context = ocontext.str();

        /*
            Get identity string.
        */
        wostringstream oidentity;
        oidentity << "{"
        << "\"squad\":" << static_cast<unsigned int>(squadnum) << ","
        << "}";

        identity = oidentity.str();
    }

	// Flip our front vector
	for (int i=0;i<3;i++) {
		avatar_front[i] = -avatar_front[i];
	}

	// Convert from right to left handed
	avatar_pos[0] = -avatar_pos[0];
	avatar_front[0] = -avatar_front[0];
	avatar_top[0] = -avatar_top[0];

	for (int i=0;i<3;i++) {
		camera_pos[i] = avatar_pos[i];
		camera_front[i] = avatar_front[i];
		camera_top[i] = avatar_top[i];
	}


	return ok;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

	if (! initialize(pids, L"bf3.exe"))
		return false;

    pmodule_bf3 = getModuleAddr(L"bf3.exe");
    if(!pmodule_bf3)
        return false;

	float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
	std::string context;
	std::wstring identity;

	if (!fetch(apos, afront, atop, cpos, cfront, ctop, context, identity)) {
		generic_unlock();
		return false;
	}

	return true;
}

static const std::wstring longdesc() {
    return std::wstring(L"Supports Battlefield 3. New Context and rudimentary identity support.");
}

static std::wstring description(L"Battlefield 3 v944019ci");
static std::wstring shortname(L"Battlefield 3");

static int trylock1() {
	return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin bf3plug = {
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

static MumblePlugin2 bf3plug2 = {
	MUMBLE_PLUGIN_MAGIC_2,
	MUMBLE_PLUGIN_VERSION,
	trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
	return &bf3plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
	return &bf3plug2;
}
