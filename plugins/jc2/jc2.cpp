// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.


/* Copyright (C) 2012, Mike <mike@flomp.net>
   Copyright (C) 2005-2012, Thorvald Natvig <thorvald@natvig.com>

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

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

const unsigned int off_character_manager = 0xD8FB24;
const unsigned int off_local_player      = 0x3570;
const unsigned int off_character         = 0x48;
const unsigned int off_char_matrix       = 0x15C;

const unsigned int off_camera_manager = 0xD919F4;
const unsigned int off_cam_matrix     = 0x2FC;

static procptr_t char_matrix_ptr = 0;
static procptr_t cam_matrix_ptr  = 0;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} Vector4;

typedef struct {
	Vector4 left;
	Vector4 top;
	Vector4 front;
	Vector4 pos;
} Matrix4;

static int setuppointers() {
	procptr_t character_manager, local_player, character, camera_manager;

	if (!peekProc(pModule + off_character_manager, &character_manager, 4) || !character_manager)
		return false;

	if (!peekProc(character_manager + off_local_player, &local_player, 4) || !local_player)
		return false;

	if (!peekProc(local_player + off_character, &character, 4) || !character)
		return false;

	char_matrix_ptr = character + off_char_matrix;

	if (!peekProc(pModule + off_camera_manager, &camera_manager, 4) || !camera_manager)
		return false;

	cam_matrix_ptr = camera_manager + off_cam_matrix;

	return true;
}

static int fetch(float *avatar_pos, float *avatar_front, float *avatar_top, float *camera_pos, float *camera_front,
				 float *camera_top, std::string &, std::wstring &) {
	for (int i = 0; i < 3; i++)
		avatar_pos[i] = avatar_front[i] = avatar_top[i] = 0.0f;

	if ((!char_matrix_ptr || !cam_matrix_ptr) && !setuppointers()) {
		return false;
	}

	bool ok;

	Matrix4 char_matrix;
	Matrix4 cam_matrix;

	ok = peekProc(char_matrix_ptr, &char_matrix, sizeof(char_matrix))
		 && peekProc(cam_matrix_ptr, &cam_matrix, sizeof(cam_matrix));

	if (!ok)
		return false;

	avatar_pos[0] = char_matrix.pos.x;
	avatar_pos[1] = char_matrix.pos.y;
	avatar_pos[2] = -char_matrix.pos.z;

	avatar_front[0] = char_matrix.front.x;
	avatar_front[1] = char_matrix.front.y;
	avatar_front[2] = char_matrix.front.z;

	avatar_top[0] = char_matrix.top.x;
	avatar_top[1] = char_matrix.top.y;
	avatar_top[2] = char_matrix.top.z;

	camera_pos[0] = avatar_pos[0] + (cam_matrix.pos.x / 10000);
	camera_pos[1] = avatar_pos[1] + (cam_matrix.pos.y / 10000);
	camera_pos[2] = avatar_pos[2] - (cam_matrix.pos.z / 10000);

	camera_front[0] = cam_matrix.front.x;
	camera_front[1] = cam_matrix.front.y;
	camera_front[2] = cam_matrix.front.z;

	camera_top[0] = cam_matrix.top.x;
	camera_top[1] = cam_matrix.top.y;
	camera_top[2] = cam_matrix.top.z;

	return true;
}

static int trylock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"JustCause2.exe"))
		return false;

	float apos[3], afront[3], atop[3];
	float cpos[3], cfront[3], ctop[3];
	std::wstring sidentity;
	std::string scontext;

	if (setuppointers() && fetch(apos, afront, atop, cpos, cfront, ctop, scontext, sidentity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longdesc() {
	return std::wstring(L"Supports Just Cause 2 (v1.0.0.2). No identity support.");
}

static std::wstring description(L"Just Cause 2 (v1.0.0.2)");
static std::wstring shortname(L"Just Cause 2");

static int trylock1() {
	return trylock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin jc2plug = { MUMBLE_PLUGIN_MAGIC, description, shortname, nullptr, nullptr, trylock1,
								generic_unlock,      longdesc,    fetch };

static MumblePlugin2 jc2plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, trylock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &jc2plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &jc2plug2;
}
