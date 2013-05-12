/* Copyright (C) 2013, Gabriel Risterucci <cleyfaye@gmail.com>
   Copyright (C) 2013, Steve Hill <github@cheesy.sackheads.org>
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

VOID *pos_ptr;
VOID *dir_ptr;
VOID *state_ptr;

typedef struct {
    float coord[3];
} VecT;

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &, std::wstring &identity)
{
    // Zero out the structures
    for (int i=0;i<3;i++)
        avatar_pos[i]=avatar_front[i]=avatar_top[i]=camera_pos[i]=camera_front[i]=camera_top[i]=0.0f;

    bool ok;

    char state;

    // State 1 == in-game, 0 == in-menu
    ok = peekProc(state_ptr, state);
    if (!ok) return false;

    if (state == 0)
             return true; // This results in all vectors beeing zero which tells Mumble to ignore them.

    VecT pos;
    VecT dir;
    ok = peekProc(pos_ptr, pos) && peekProc(dir_ptr, dir);
    if (!ok) return false;

    // Copy game vectors into return values
    camera_pos[0] = avatar_pos[0] = -pos.coord[0] / 100.f;
    camera_pos[1] = avatar_pos[1] = pos.coord[2] / 100.f;
    camera_pos[2] = avatar_pos[2] = pos.coord[1] / 100.f;
    camera_front[0] = avatar_front[0] = dir.coord[0];
    camera_front[1] = avatar_front[1] = dir.coord[1];
    camera_front[2] = avatar_front[2] = dir.coord[2];
	camera_top[0] = avatar_top[0] = 0;
	camera_top[1] = avatar_top[1] = 1;
	camera_top[2] = avatar_top[2] = 0;

    return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids)
{
    if (!initialize(pids, L"Borderlands2.exe")) {
        return false;
	}
	
    char detected_version[32];

    if (peekProc(pModule + 0x1E9F338, detected_version)
        && strcmp(detected_version, "WILLOW2-PCLILAC-60-CL721220") == 0)
    {
		// Note for further versions:
		// The "version" string above change. However, it looks like it will always start
		// with "WILLOW2-". Everything after this can change between versions.
		// The position triplet is read as X,Z,Y
		// The front vector is read as X,Y,Z
		// I didn't bother looking for a UP vector, as we can't roll much in this game :)
		// When entering the game, in Sanctuary, with the character Zero,
		// the reading are the following (rounded):
		// pos.X = 8109
		// pos.Z = 2930
		// pos.Y = 3800
		// front.X = 0
		// front.Y = 0
		// front.Z = 1
		// The "state" ptr is just a value that reliably alternate between 0 (in main menu)
		// and 1 (not in main menu). It never took another value in the length of all my probing,
		// and keep reliably changing even across restart, change of characters...
		// Note that I couldn't find a location that would do this reliably with the game "pause" 
		// menu, only the main menu (when you initially start the game, or completely exit your
		// current game)
        pos_ptr = pModule + 0x1E7C390;
        dir_ptr = pModule + 0x1EAB650;
        state_ptr = pModule + 0x1EABF68;
    }
    else
    {
        generic_unlock();
        return false;
    }

    // Check if we can get meaningful data from it
    float apos[3], afront[3], atop[3];
    float cpos[3], cfront[3], ctop[3];
    std::wstring sidentity;
    std::string scontext;

    if (fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
        return true;
    } else {
        generic_unlock();
        return false;
    }
}

static const std::wstring longdesc() {
    return std::wstring(L"Supports Borderlands 2. No context support yet.");
}

static std::wstring description(L"Borderlands 2 (v1.5.0)");
static std::wstring shortname(L"Borderlands 2");

static int trylock1() {
    return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin aaplug = {
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

static MumblePlugin2 aaplug2 = {
    MUMBLE_PLUGIN_MAGIC_2,
    MUMBLE_PLUGIN_VERSION,
    trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
    return &aaplug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
    return &aaplug2;
}
