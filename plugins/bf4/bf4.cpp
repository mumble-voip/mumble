// Copyright 2016 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "../mumble_plugin_win32.h"
#include <algorithm>

std::string jsonStringEscape(const std::string &input) {
    std::string output(input);
    std::replace(output.begin(), output.end(), '"', ' '); // Replace '"' character with ' ' (space)
    return output;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity) {
    for (int i=0;i<3;i++) {
        avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] = camera_front[i] = camera_top[i] = 0.0f;
    }

    bool ok, state;
    char serverid[37], host[22], team[3];
    BYTE squad, squad_leader;

    // Server ID pointers
    BYTE *serverid_base = peekProc<BYTE *>(pModule + 0x02210658);
    if (!serverid_base) return false;
    BYTE *serverid_offset_0 = peekProc<BYTE *>(serverid_base + 0x18);
    if (!serverid_offset_0) return false;
    BYTE *serverid_offset_1 = peekProc<BYTE *>(serverid_offset_0 + 0x28);
    if (!serverid_offset_1) return false;
    BYTE *serverid_offset = peekProc<BYTE *>(serverid_offset_1 + 0x350);
    if (!serverid_offset) return false;

    // Squad pointers
    BYTE *squad_base = peekProc<BYTE *>(pModule + 0x02210D68);
    if (!squad_base) return false;
    BYTE *squad_offset_0 = peekProc<BYTE *>(squad_base + 0x570);
    if (!squad_offset_0) return false;
    BYTE *squad_offset_1 = peekProc<BYTE *>(squad_offset_0 + 0x100);
    if (!squad_offset_1) return false;
    BYTE *squad_offset_2 = peekProc<BYTE *>(squad_offset_1 + 0x58);
    if (!squad_offset_2) return false;

    // Peekproc and assign game addresses to our containers, so we can retrieve positional data
    ok = peekProc((BYTE *) pModule + 0x21CAFF0, &state, 1) && // Magical state value: 0 when in-game and 1 when in menu/dead.
            peekProc((BYTE *) pModule + 0x21C6D40, avatar_pos, 12) && // Avatar Position values (X, Y and Z).
            peekProc((BYTE *) pModule + 0x21CAF80, camera_pos, 12) && // Camera Position values (X, Y and Z).
            peekProc((BYTE *) pModule + 0x21CAF60, avatar_top, 12) && // Avatar Top Vector values (X, Y and Z).
            peekProc((BYTE *) pModule + 0x21CAF70, avatar_front, 12) && // Avatar Front Vector values (X, Y and Z).
            peekProc((BYTE *) serverid_offset, serverid) && // Server ID (36 characters).
            peekProc((BYTE *) pModule + 0x21B80C0, host) && // Host value: "IP:Port" when in a server, "bot" when loading and empty when it's hidden.
            peekProc((BYTE *) pModule + 0x24AFAE5, team) && // Team value: US (United States); RU (Russia); CH (China).
            peekProc((BYTE *) squad_offset_2 + 0x230, squad) && // Squad value: 0 (not in a squad); 1 (Alpha); 2 (Bravo); 3 (Charlie)... 26 (Zulu).
            peekProc((BYTE *) squad_offset_2 + 0x234, squad_leader); // Squad leader value: 1 (True); 2 (False).

    // This prevents the plugin from linking to the game in case something goes wrong during values retrieval from memory addresses.
    if (! ok) {
        return false;
    }

    if (state) { // If not in-game
        context.clear(); // Clear context
        identity.clear(); // Clear identity
        // Set vectors values to 0.
        for (int i=0;i<3;i++) {
            avatar_pos[i] = avatar_front[i] = avatar_top[i] = camera_pos[i] =  camera_front[i] = camera_top[i] = 0.0f;
        }

        return true; // This tells Mumble to ignore all vectors.
    }

    serverid[sizeof(serverid)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
    std::string ServerID(serverid);
    ServerID = jsonStringEscape(ServerID); // Replace '"' character with ' ' (space) to prevent JSON break.
    std::ostringstream ocontext;
    ocontext << " {\"Server ID\": \"" << ServerID << "\"}"; // Set context with server ID
    context = ocontext.str();

    std::wostringstream oidentity;
    oidentity << "{";
    host[sizeof(host)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
    std::string Host(host);
    Host = jsonStringEscape(Host);
    if (Host.find("bot") == std::string::npos) // Set host string as empty if "bot" is found in it.
        if (!Host.empty()) {
            oidentity << std::endl << "\"Host\": \"" << host << "\","; // If it's not empty, set Host (IP:Port) in identity.
        }

    team[sizeof(team)-1] = 0; // NUL terminate queried C strings. We do this to ensure the strings from the game are NUL terminated. They should be already, but we can't take any chances.
    std::string Team(team);
    Team = jsonStringEscape(Team);
    if (!Team.empty()) {
        oidentity << std::endl;
        if (Team == "US")
            oidentity << "\"Team\": \"United States\","; // If team value is US, set "United States" as team in identity.
        else if (Team == "CH")
            oidentity << "\"Team\": \"China\","; // If team value is CH, set "China" as team in identity.
        else if (Team == "RU")
            oidentity << "\"Team\": \"Russia\","; // If team value is RU, set "Russia" as team in identity.
    }

    // If squad value is in a value between 1 and 26, set squad name in identity using NATO Phonetic alphabet.
    if (squad > 0 && squad < 27) {
        if (squad == 1)
            oidentity << std::endl << "\"Squad\": \"Alpha\",";
        else if (squad == 2)
            oidentity << std::endl << "\"Squad\": \"Bravo\",";
        else if (squad == 3)
            oidentity << std::endl << "\"Squad\": \"Charlie\",";
        else if (squad == 4)
            oidentity << std::endl << "\"Squad\": \"Delta\",";
        else if (squad == 5)
            oidentity << std::endl << "\"Squad\": \"Echo\",";
        else if (squad == 6)
            oidentity << std::endl << "\"Squad\": \"Foxtrot\",";
        else if (squad == 7)
            oidentity << std::endl << "\"Squad\": \"Golf\",";
        else if (squad == 8)
            oidentity << std::endl << "\"Squad\": \"Hotel\",";
        else if (squad == 9)
            oidentity << std::endl << "\"Squad\": \"India\",";
        else if (squad == 10)
            oidentity << std::endl << "\"Squad\": \"Juliet\",";
        else if (squad == 11)
            oidentity << std::endl << "\"Squad\": \"Kilo\",";
        else if (squad == 12)
            oidentity << std::endl << "\"Squad\": \"Lima\",";
        else if (squad == 13)
            oidentity << std::endl << "\"Squad\": \"Mike\",";
        else if (squad == 14)
            oidentity << std::endl << "\"Squad\": \"November\",";
        else if (squad == 15)
            oidentity << std::endl << "\"Squad\": \"Oscar\",";
        else if (squad == 16)
            oidentity << std::endl << "\"Squad\": \"Papa\",";
        else if (squad == 17)
            oidentity << std::endl << "\"Squad\": \"Quebec\",";
        else if (squad == 18)
            oidentity << std::endl << "\"Squad\": \"Romeo\",";
        else if (squad == 19)
            oidentity << std::endl << "\"Squad\": \"Sierra\",";
        else if (squad == 20)
            oidentity << std::endl << "\"Squad\": \"Tango\",";
        else if (squad == 21)
            oidentity << std::endl << "\"Squad\": \"Uniform\",";
        else if (squad == 22)
            oidentity << std::endl << "\"Squad\": \"Victor\",";
        else if (squad == 23)
            oidentity << std::endl << "\"Squad\": \"Whiskey\",";
        else if (squad == 24)
            oidentity << std::endl << "\"Squad\": \"X-ray\",";
        else if (squad == 25)
            oidentity << std::endl << "\"Squad\": \"Yankee\",";
        else if (squad == 26)
            oidentity << std::endl << "\"Squad\": \"Zulu\",";
        if (squad_leader == 1)
            oidentity << std::endl << "\"Squad leader\": true"; // If squad leader value is true, set squad leader state to "True" in identity.
        else
            oidentity << std::endl << "\"Squad leader\": false"; // If squad leader value is false, set squad leader state to "False" in identity.
    } else {
        oidentity << std::endl << "\"Squad\": null,"; // If squad value isn't between 1 and 26, set squad state to "null" in identity.
        oidentity << std::endl << "\"Squad leader\": null"; // If not in a squad, set squad leader state to "null" in identity.
    }

    oidentity << std::endl << "}";
    identity = oidentity.str();

    // Flip the front vector
    for (int i=0;i<3;i++) {
        avatar_front[i] = -avatar_front[i];
    }

    // Convert from right to left handed
    avatar_pos[0] = -avatar_pos[0];
    camera_pos[0] = -camera_pos[0];
    avatar_front[0] = -avatar_front[0];
    avatar_top[0] = -avatar_top[0];

    // Sync camera front and top vectors with avatar ones
    for (int i=0;i<3;i++) {
        camera_front[i] = avatar_front[i];
        camera_top[i] = avatar_top[i];
    }

    return true;
}

static int trylock(const std::multimap<std::wstring, unsigned long long int> &pids) {

    if (! initialize(pids, L"bf4.exe")) { // Link the game executable
        return false;
    }

    // Check if we can get meaningful data from it
    float apos[3], afront[3], atop[3], cpos[3], cfront[3], ctop[3];
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
    return std::wstring(L"Supports Battlefield 4 with context and identity support."); // Plugin long description
}

static std::wstring description(L"Battlefield 4 (x64) version 1.7.2.45672"); // Plugin short description
static std::wstring shortname(L"Battlefield 4"); // Plugin short name

static int trylock1() {
    return trylock(std::multimap<std::wstring, unsigned long long int>());
}

static MumblePlugin bf4plug = {
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

static MumblePlugin2 bf4plug2 = {
    MUMBLE_PLUGIN_MAGIC_2,
    MUMBLE_PLUGIN_VERSION,
    trylock
};

extern "C" __declspec(dllexport) MumblePlugin *getMumblePlugin() {
    return &bf4plug;
}

extern "C" __declspec(dllexport) MumblePlugin2 *getMumblePlugin2() {
    return &bf4plug2;
}
