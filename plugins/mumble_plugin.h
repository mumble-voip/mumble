/* Copyright (C) 2005-2012, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef MUMBLE_MUMBLE_PLUGIN_H_
#define MUMBLE_MUMBLE_PLUGIN_H_

#include <string>
#include <map>

#define MUMBLE_PLUGIN_MAGIC 0xd63ab7c0
#define MUMBLE_PLUGIN_MAGIC_2 0xd63ab7cf
#define MUMBLE_PLUGIN_VERSION 2

typedef struct _MumblePlugin {
	unsigned int magic;
	const std::wstring &description;
	const std::wstring &shortname;
	void (__cdecl *about)(HWND);
	void (__cdecl *config)(HWND);
	int (__cdecl *trylock)();
	void (__cdecl *unlock)();
	const std::wstring(__cdecl *longdesc)();
	int (__cdecl *fetch)(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front, float *camera_top, std::string &context, std::wstring &identity);
} MumblePlugin;

typedef struct _MumblePlugin2 {
	unsigned int magic;
	unsigned int version;
	int (__cdecl *trylock)(const std::multimap<std::wstring, unsigned long long int> &);
} MumblePlugin2;

typedef MumblePlugin *(__cdecl *mumblePluginFunc)();
typedef MumblePlugin2 *(__cdecl *mumblePlugin2Func)();

/*
 * All plugins must implement one function called mumbleGetPlugin(), which
 * follows the mumblePluginFunc type and returns a MumblePlugin struct.
 *
 * magic should be initialized to MUMBLE_PLUGIN_MAGIC. description is the
 * name of the plugin shown in the GUI, while shortname is used for TTS.
 *
 * The individual functions are:
 * about(HWND parent) - Player clicked the about button over plugin
 * config(HWND parent) - Player clicked the config button
 * trylock() - Search system for likely process and try to lock on.
 *      The parameter is a set of process names and associated PIDs.
 *		Return 1 if succeeded, else 0. Note that once a plugin is locked on,
 *		no other plugins will be queried.
 * unlock() - Unlock from process. Either from user intervention or
 *		because fetch failed.
 * fetch(...) - Fetch data from locked process. avatar_pos is position in
 *		world coordinates (1 meter per unit). avatar_front and avatar_top
 *      specify the heading of the player, as in where he is looking.
 *		You need at minimum to figure out pos and front, otherwise
 *		sounds cannot be placed. If you do not fetch top, make it the
 *		same as front but rotated 90 degrees "upwards".
 *
 *      camera_x is the same, but for the camera. Make this identical to the
 *      avatar position if you don't know (or if it's a 1st person
 *      perspective).
 *
 *		It is important that you set all fields to 0.0 if you can't
 *		fetch meaningfull values, like between rounds and such.
 *
 *      context and identity are transmitted to the server. Only players
 *      with identical context will hear positional audio from each other.
 *      Mumble will automatically prepend the shortname of the plugin to
 *      the context, so make this a representation of the game server and
 *      team the player is on.
 *
 *      identity is retained by the server and is pollable over Ice/DBus,
 *      to be used by external scripts. This should uniquiely identify the
 *      player inside the game.
 *
 *      ctx_len and id_len are initialized to the bufferspace available. Set these
 *      to -1 to keep the previous value (as parsing and optimizing can be CPU
 *      intensive)
 *
 *		The function should return 1 if it is still "locked on",
 *		otherwise it should return 0. Mumble will call unlock()
 *		if it return 0, and go back to polling with trylock()
 *		once in a while.
 */

#endif
