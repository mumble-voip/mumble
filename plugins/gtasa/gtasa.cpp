// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_main.h"

struct Matrix {
	float right[4];
	float front[4];
	float top[4];
	float position[3];
};

static int fetch(float *avatarPosition, float *avatarFront, float *avatarTop, float *cameraPosition, float *cameraFront,
				 float *cameraTop, std::string &, std::wstring &) {
	for (uint8_t i = 0; i < 3; ++i) {
		avatarPosition[i] = avatarFront[i] = avatarTop[i] = 0.0f;
	}

	const auto playerPed = peekProcPtr(0xB7CD98);
	if (!playerPed) {
		return false;
	}

	const bool isInCar = (peekProc< uint8_t >(playerPed + 0x46C) == 1);

	const auto playerCar = peekProcPtr(playerPed + 0x58C);

	const auto matrixAddress = peekProcPtr((isInCar && playerCar ? playerCar : playerPed) + 0x14);
	if (!matrixAddress) {
		return false;
	}

	const auto matrix = peekProc< Matrix >(matrixAddress);

	/*
	Mumble | Game
	X      | X
	Y      | Z
	Z      | Y
	*/
	avatarPosition[0] = matrix.position[0];
	avatarPosition[1] = matrix.position[2];
	avatarPosition[2] = matrix.position[1];

	avatarFront[0] = matrix.front[0];
	avatarFront[1] = matrix.front[2];
	avatarFront[2] = matrix.front[1];

	avatarTop[0] = matrix.top[0];
	avatarTop[1] = matrix.top[2];
	avatarTop[2] = matrix.top[1];

	for (uint8_t i = 0; i < 3; ++i) {
		cameraPosition[i] = avatarPosition[i];
		cameraFront[i]    = avatarFront[i];
		cameraTop[i]      = avatarTop[i];
	}

	return true;
}

static int tryLock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!initialize(pids, L"gta_sa.exe")) {
		return false;
	}

	float avatarPosition[3], avatarFront[3], avatarTop[3];
	float cameraPosition[3], cameraFront[3], cameraTop[3];
	std::string context;
	std::wstring identity;

	if (fetch(avatarPosition, avatarFront, avatarTop, cameraPosition, cameraFront, cameraTop, context, identity)) {
		return true;
	} else {
		generic_unlock();
		return false;
	}
}

static const std::wstring longDesc() {
	return std::wstring(L"Supports Grand Theft Auto: San Andreas version 1.0.");
}

static std::wstring description(L"Grand Theft Auto: San Andreas (v1.0)");
static std::wstring shortName(L"Grand Theft Auto: San Andreas");

static int tryLock1() {
	return tryLock(std::multimap< std::wstring, unsigned long long int >());
}

static MumblePlugin gtasaPlug = { MUMBLE_PLUGIN_MAGIC, description, shortName, nullptr, nullptr, tryLock1,
								  generic_unlock,      longDesc,    fetch };

static MumblePlugin2 gtasaPlug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, tryLock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &gtasaPlug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &gtasaPlug2;
}
