/* Copyright (C) 2010-2011, Snares <snares@users.sourceforge.net>
   Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2011, Ryan Austin <ryan@gameforcecenters.com>
   Copyright (C) 2012, Bojan Hartmann <bogie@bawki.de>

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
static bool ptr_chain_valid = false;
static BYTE *pmodule_bf3;

// Magic ptrs
static BYTE* const state_ptr = (BYTE *) 0x2366F4C;

// Vector ptrs
static BYTE* const avatar_pos_ptr = (BYTE *) 0x02366EE0;
static BYTE* const avatar_front_ptr = (BYTE *) 0x2366EF0;
static BYTE* const avatar_top_ptr = (BYTE *) 0x2366F00;

// Context ptrs
static BYTE* const ipport_ptr = (BYTE *) 0x233DA30;

// Identity ptrs
static BYTE *team_state_ptr;
static BYTE *squad_state_ptr;
static BYTE *squad_lead_state_ptr;

// Offsets
static const int base_offset = 0x01F5CD6C;
static const int identity_offset1 = 0x1C;
static const int identity_offset2 = 0xBC;
static const int identity_offset3 = 0x36C;
static const int identity_offset4 = 0x8;
static const int squad_state_offset = 0x104;
static const int squad_lead_state_offset = 0x108;
static const int team_state_offset = 0x31C;

enum state_values {
    STATE_UNKN = 0,
    STATE_LOADING = 1,
    STATE_IN_GAME = 2,
    STATE_IN_MENU = 3
};

inline bool resolve_ptrs() {
    team_state_ptr = squad_state_ptr = squad_lead_state_ptr = NULL;

    /*
      Analysis for future patches:

      - state_ptr is always 0x6C after avatar_pos_ptr
      - avatar_front_ptr is always 0x30 after avatar_pos_ptr
      - avatar_top_ptr is always 0x20 after avatar_pos_ptr
      - you can find avatar_pos_ptr easily by jumping in a jet and search for altitude
      */
    /*
    Magic:
        state : 0x02347E3C                                                       BYTE        1 when playing
                                                                                             2 while in menu/dead

    Context:
        IP:Port of server: 0x023340D0                                            char[128]   ip:port of the server

    Identity:
        Squad state: BF3.exe+0x01F42AE4 + 0x8 + 0xBC + 0x36C + 0x8 + 0x104      BYTE        0 is not in squad; 1 is in Alpha squad, 2 Bravo, ... , 9 India
        SLead state: BF3.exe+0x01F42AE4 + 0x8 + 0xBC + 0x36C + 0x8 + 0x108      BYTE        0 is not lead; 1 is lead
        Team state:  BF3.exe+0x01F42AE4 + 0x8 + 0xBC + 0x31C                    BYTE        1 is blufor (US team, for example), 2 is opfor (RU), 0 is probably upcoming spec mode
    */

    BYTE *base_bf3 = peekProc<BYTE *>(pmodule_bf3 + base_offset);
    if (!base_bf3)
        return false;

    BYTE *offset_ptr1 = peekProc<BYTE *>(base_bf3 + identity_offset1);
    if (!offset_ptr1) return false;
    BYTE *offset_ptr2 = peekProc<BYTE *>(offset_ptr1 + identity_offset2);
    if (!offset_ptr2) return false;
    BYTE *offset_ptr3 = peekProc<BYTE *>(offset_ptr2 + identity_offset3);
    if (!offset_ptr3) return false;
    BYTE *offset_ptr4 = peekProc<BYTE *>(offset_ptr3 + identity_offset4);
    if (!offset_ptr4) return false;

    squad_state_ptr = offset_ptr4 + squad_state_offset;
    squad_lead_state_ptr = offset_ptr4 + squad_lead_state_offset;
    team_state_ptr = offset_ptr2 + team_state_offset;

    return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
    for (int i=0;i<3;i++)
        avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;

    char ccontext[128];
    char state;
    BYTE squad_state;
    BYTE is_squadleader;
    BYTE team_state;
    bool ok;

    ok = peekProc(state_ptr, &state, 1); // State value
    if (! ok)
        return false;

    if (state != STATE_IN_GAME && state != STATE_IN_MENU) {
        ptr_chain_valid = false;
        context.clear();
        identity.clear();
        return true;
    } else if (!ptr_chain_valid) {
        if (!resolve_ptrs())
            return false;
        ptr_chain_valid = true;
    }

    ok = peekProc(avatar_pos_ptr, avatar_pos, 12) &&
        peekProc(avatar_front_ptr, avatar_front, 12) &&
        peekProc(avatar_top_ptr, avatar_top, 12) &&
        peekProc(squad_state_ptr,&squad_state,1) &&
        peekProc(squad_lead_state_ptr, &is_squadleader,1) &&
        peekProc(team_state_ptr,&team_state,1) &&
        peekProc(ipport_ptr,ccontext,128);

    if (! ok)
        return false;

    ccontext[sizeof(ccontext) - 1] = 0;
    if (ccontext[0] != '0') {
        std::ostringstream ocontext;
        ocontext << "{ \"ipport\": \"" << ccontext << "\" }";

        context = ocontext.str();

        /*
            Get identity string.
        */
        std::wostringstream oidentity;
        oidentity << "{"
        << "\"squad\":" << static_cast<unsigned int>(squad_state) << ","
        << "\"squad_leader\":" << (is_squadleader ? "true" : "false") << ",";
        if (team_state == 0)
            oidentity << "\"team\": \"SPEC\"";
        else if (team_state == 1)
            oidentity << "\"team\": \"US\"";
        else if (team_state == 2)
            oidentity << "\"team\": \"RU\"";
        oidentity << "}";
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
    if (!pmodule_bf3)
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
    return std::wstring(L"Supports Battlefield 3 with context and identity support.");
}

static std::wstring description(L"Battlefield 3 v1089904 - Aftermath");
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
