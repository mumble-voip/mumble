/* Copyright (C) 2005, Thorvald Natvig <thorvald@natvig.com>

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

#ifndef _MUMBLE_PLUGIN_H
#define _MUMBLE_PLUGIN_H

#define MUMBLE_PLUGIN_MAGIC 0xd63ab7fd

typedef struct _MumblePlugin {
	unsigned int magic;
	wchar_t *description;
	wchar_t *shortname;
	void (__cdecl *about)(HWND);
	void (__cdecl *config)(HWND);
	int (__cdecl *trylock)();
	void (__cdecl *unlock)();
	int (__cdecl *fetch)(float *pos, float *vel, float *front, float *top);
} MumblePlugin;

typedef MumblePlugin *(__cdecl *mumblePluginFunc)();

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
 *		Return 1 if succeeded, else 0. Note that once a plugin is locked on,
 *		no other plugins will be queried.
 * unlock() - Unlock from process. Either from user intervention or
 *		because fetch failed.
 * fetch(...) - Fetch data from locked process. pos is position in
 *		world coordinates (1 meter per unit). front and top specify
 *		the heading of the player, as in where he is looking. vel
 *		is the current velocity vector.
 *		You need at minimum to figure out pos and front, otherwise
 *		sounds cannot be placed. If you do not fetch top, make it the
 *		same as front but rotated 90 degrees "upwards". Fetching
 *		velocity is optional.
 *		It is important that you set all fields to 0.0 if you can't
 *		fetch meaningfull values, like between rounds and such.
 *		The function should return 1 if it is still "locked on",
 *		otherwise it should return 0. Mumble will call unlock()
 *		if it return 0, and go back to polling with trylock()
 *		once in a while.
 */

#endif
