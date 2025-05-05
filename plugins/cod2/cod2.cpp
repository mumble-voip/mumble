// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ProcessWindows.h"

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_utils.h"

#include <memory>

std::unique_ptr< ProcessWindows > process;

static inline bool inGame() {
	uint8_t state;
	if (!process->peek(0x96B688, &state, sizeof(state))) {
		return false;
	}

	// 0 = in-game/out-of-game
	// 4 = spectator
	return state != 4;
}

static int fetch(float *avatarPos, float *avatarFront, float *avatarTop, float *cameraPos, float *cameraFront,
				 float *cameraTop, std::string &, std::wstring &) {
	for (uint8_t i = 0; i < 3; ++i) {
		avatarPos[i] = avatarFront[i] = avatarTop[i] = cameraPos[i] = cameraFront[i] = cameraTop[i] = 0.0f;
	}

	if (!inGame()) {
		return true;
	}

	float position[3];
	if (!process->peek(0x1516608, position, sizeof(position))) {
		return false;
	}

	float rotation[2];
	if (!process->peek(0x151A110, rotation, sizeof(rotation))) {
		return false;
	}

	// Mumble | Game
	// X      | -Y
	// Y      | Z
	// Z      | X
	//
	// 40 units = 1 meter (not confirmed)
	avatarPos[0] = -position[1] / 40.0f;
	avatarPos[1] = position[2] / 40.0f;
	avatarPos[2] = position[0] / 40.0f;


	rotation[0] = degreesToRadians(rotation[0]);
	rotation[1] = degreesToRadians(rotation[1]);

	avatarFront[0] = -sin(rotation[1]) * cos(rotation[0]);
	avatarFront[1] = -sin(rotation[0]);
	avatarFront[2] = cos(rotation[0]) * cos(rotation[1]);

	// Sync camera vectors with avatar ones.
	for (uint8_t i = 0; i < 3; ++i) {
		cameraPos[i]   = avatarPos[i];
		cameraFront[i] = avatarFront[i];
	}

	return true;
}

static int tryLock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	const std::string name = "CoD2MP_s.exe";

	const auto iter = pids.find(std::wstring(name.cbegin(), name.cend()));
	if (iter == pids.cend()) {
		return false;
	}

	process = std::make_unique< ProcessWindows >(iter->second, name);
	if (!process->isOk()) {
		process.reset();
		return false;
	}

	return true;
}

static const std::wstring longDesc() {
	return std::wstring(L"Supports Call of Duty 2 v1.3. No context or identity support yet.");
}

static std::wstring description(L"Call of Duty 2 v1.3");
static std::wstring shortName(L"Call of Duty 2");

static int tryLock1() {
	return tryLock(std::multimap< std::wstring, unsigned long long int >());
}

static void nullUnlock() {
}

static MumblePlugin cod2Plug = { MUMBLE_PLUGIN_MAGIC, description, shortName, nullptr, nullptr, tryLock1,
								 nullUnlock,          longDesc,    fetch };

static MumblePlugin2 cod2Plug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, tryLock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &cod2Plug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &cod2Plug2;
}
