// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_ALLOW_DEPRECATED_LEGACY_PLUGIN_API
#include "mumble_legacy_plugin.h"

#include "mumble_positional_audio_utils.h"

#ifdef OS_LINUX
#	include "ProcessLinux.h"
#endif
#include "ProcessWindows.h"

#include <cstring>
#include <memory>
#include <sstream>

std::unique_ptr< ProcessBase > proc;

#ifdef OS_WINDOWS
static constexpr bool isWin32 = true;
#else
static bool isWin32 = false;
#endif

#include "client.h"
#include "common.h"
#include "engine.h"

static inline void anglesToVectors(const float (&angles)[3], float *front, float *top) {
	// Calculate pitch
	float pitchSin, pitchCos;
	sinCos(degreesToRadians(angles[0]), pitchSin, pitchCos);

	// Calculate yaw
	float yawSin, yawCos;
	sinCos(degreesToRadians(angles[1]), yawSin, yawCos);

	// Calculate roll
	float rollSin, rollCos;
	sinCos(degreesToRadians(angles[2]), rollSin, rollCos);

	// Calculate front vector
	front[0] = pitchCos * yawCos;
	front[1] = -pitchSin;
	front[2] = pitchCos * yawSin;

	// Calculate top vector
	top[0] = (rollCos * pitchSin * yawCos + -rollSin * -yawSin);
	top[1] = rollCos * pitchCos;
	top[2] = (rollCos * pitchSin * yawSin + -rollSin * yawCos);
}

procptr_t localClient, localPlayer;
int32_t signOnStateOffset, deadFlagOffset, rotationOffset, originPositionOffset, eyesPositionOffsetOffset,
	netInfoOffset, levelNameOffset;

static int fetch(float *avatarPos, float *avatarFront, float *avatarTop, float *cameraPos, float *cameraFront,
				 float *cameraTop, std::string &context, std::wstring &identity) {
	const auto signOnState = proc->peek< uint32_t >(localClient + signOnStateOffset);

	// 0: SIGNONSTATE_NONE
	// 1: SIGNONSTATE_CHALLENGE
	// 2: SIGNONSTATE_CONNECTED
	// 3: SIGNONSTATE_NEW
	// 4: SIGNONSTATE_PRESPAWN
	// 5: SIGNONSTATE_SPAWN
	// 6: SIGNONSTATE_FULL
	// 7: SIGNONSTATE_CHANGELEVEL
	if (signOnState != 6) {
		return false;
	}

	const auto isPlayerDead = proc->peek< bool >(localPlayer + deadFlagOffset);
	if (isPlayerDead) {
		context.clear();
		identity.clear();

		for (uint8_t i = 0; i < 3; ++i) {
			avatarPos[i] = avatarFront[i] = avatarTop[i] = cameraPos[i] = cameraFront[i] = cameraTop[i] = 0.0f;
		}

		return true;
	}

	float rotation[3];
	if (!proc->peek(localPlayer + rotationOffset, rotation, sizeof(rotation))) {
		return false;
	}

	float originPosition[3];
	if (!proc->peek(localPlayer + originPositionOffset, originPosition, sizeof(originPosition))) {
		return false;
	}

	float eyesPositionOffset[3];
	if (!proc->peek(localPlayer + eyesPositionOffsetOffset, eyesPositionOffset, sizeof(eyesPositionOffset))) {
		return false;
	}

	NetInfo ni;
	if (!proc->peek(localClient + netInfoOffset, ni)) {
		return false;
	}

	std::string host;

	// 0: NA_NULL
	// 1: NA_LOOPBACK
	// 2: NA_BROADCAST
	// 3: NA_IP
	switch (ni.type) {
		case 3: {
			std::ostringstream ss;
			for (uint8_t i = 0; i < 4; ++i) {
				ss << std::to_string(ni.ip[i]);
				if (i < 3) {
					ss << ".";
				}
			}

			ss << ":" << networkToHost(ni.port);
			host = ss.str();
			break;
		}
		default:
			return true;
	}

	// 40 is the size of the char array in the engine's code
	char map[40];
	if (!proc->peek(localClient + levelNameOffset, map, sizeof(map))) {
		map[0] = '\0';
	}

	// Begin context
	std::ostringstream ocontext;
	ocontext << " {\"Host\": \"" << host << "\"}"; // Set context with server's IP address and port.
	context = ocontext.str();
	// End context

	// Begin identity
	std::wostringstream oidentity;
	oidentity << "{";

	// Map
	escape(map, sizeof(map));
	if (strcmp(map, "") != 0) {
		oidentity << std::endl << "\"Map\": \"" << map << "\""; // Set map name in identity.
	} else {
		oidentity << std::endl << "\"Map\": null";
	}

	oidentity << std::endl << "}";
	identity = oidentity.str();
	// End identity

	// Mumble | Game
	// X      | X
	// Y      | Z
	// Z      | Y
	//
	// originPosition corresponds to the origin of the player model,
	// usually the point (i.e. feet) where it touches the ground.
	// eyesPositionOffset is the difference between the eyes and the origin position.
	// We use the eyes' position because they are closer to the mouth.
	avatarPos[0] = originPosition[0] + eyesPositionOffset[0];
	avatarPos[1] = originPosition[2] + eyesPositionOffset[2];
	avatarPos[2] = originPosition[1] + eyesPositionOffset[1];

	anglesToVectors(rotation, avatarFront, avatarTop);

	// Sync camera vectors with avatar ones.
	//
	// This is not ideal, but it's not an issue unless 3rd person mode is enabled.
	// The command to set it is "thirdperson" and requires "sv_cheats" to be enabled.
	// For reference, the functions that return what we need are MainViewOrigin() and MainViewAngles().
	for (uint8_t i = 0; i < 3; ++i) {
		cameraPos[i]   = avatarPos[i] /= 39.37f; // Convert from inches to meters.
		cameraFront[i] = avatarFront[i];
		cameraTop[i]   = avatarTop[i];
	}

	return true;
}

static bool tryInit(const std::multimap< std::wstring, unsigned long long int > &pids) {
	const std::vector< std::string > names{
#ifdef OS_LINUX
		"hl2_linux",
#endif
		"left4dead2.exe", "left4dead.exe"
	};

	for (const auto &name : names) {
		const auto iter = pids.find(std::wstring(name.cbegin(), name.cend()));
		if (iter == pids.cend()) {
			continue;
		}

		const auto id = iter->second;
#ifdef OS_WINDOWS
		proc.reset(new ProcessWindows(id, name));
#else
		isWin32 = HostLinux::isWine(id);
		if (isWin32) {
			proc.reset(new ProcessWindows(id, name));
		} else {
			proc.reset(new ProcessLinux(id, name));
		}
#endif
		if (proc->isOk()) {
			return true;
		}
	}

	return false;
}

static int tryLock(const std::multimap< std::wstring, unsigned long long int > &pids) {
	if (!tryInit(pids)) {
		return false;
	}

	const auto modules = proc->modules();

	auto iter = modules.find(isWin32 ? "engine.dll" : "engine.so");
	if (iter == modules.cend()) {
		return false;
	}

	const auto engine = iter->second.baseAddress();
	if (!engine) {
		return false;
	}

	iter = modules.find(isWin32 ? "client.dll" : "client.so");
	if (iter == modules.cend()) {
		return false;
	}

	const auto client = iter->second.baseAddress();
	if (!client) {
		return false;
	}

	const auto engineInterfaces = getInterfaces(engine);

	const auto engineClient = getInterfaceAddress("VEngineClient013", engineInterfaces);
	if (!engineClient) {
		return false;
	}

	localClient = getLocalClient(engineClient);
	if (!localClient) {
		return false;
	}

	signOnStateOffset = getSignOnStateOffset(engineClient);
	if (!signOnStateOffset) {
		return false;
	}

	const auto signOnState = proc->peek< uint32_t >(localClient + signOnStateOffset);
	if (signOnState != 6) {
		return false;
	}

	const auto clientInterfaces = getInterfaces(client);

	const auto clientEntityList = getInterfaceAddress("VClientEntityList003", clientInterfaces);
	if (!clientEntityList) {
		return false;
	}

	localPlayer = getLocalPlayer(localClient, clientEntityList, engineClient);
	if (!localPlayer) {
		return false;
	}

	// "pl" is the offset to the internal player class.
	const auto pl = getDataVarFromEntity("pl", localPlayer);
	if (!pl) {
		return false;
	}

	// "deadflag" is the offset relative to "pl".
	//
	// For some reason it's missing from Left 4 Dead 2 on Linux, along with many other datavars we don't need.
	// The offset (0x4) has never changed across the various engine versions released during the years (10+), thus we
	// force it.
	const auto deadflag = getDataVarFromEntity("deadflag", localPlayer);
	deadFlagOffset      = pl + (deadflag ? deadflag : 0x4);

	originPositionOffset = getDataVarFromEntity("m_vecAbsOrigin", localPlayer);
	if (!originPositionOffset) {
		return false;
	}

	eyesPositionOffsetOffset = getDataVarFromEntity("m_vecViewOffset", localPlayer);
	if (!eyesPositionOffsetOffset) {
		return false;
	}

	rotationOffset = getDataVarFromEntity("m_angAbsRotation", localPlayer);
	if (!rotationOffset) {
		return false;
	}

	netInfoOffset = getNetInfoOffset(localClient, engineClient);
	if (!netInfoOffset) {
		return false;
	}

	levelNameOffset = getLevelNameOffset(engineClient);
	if (!levelNameOffset) {
		return false;
	}

	return true;
}

static const std::wstring longDesc() {
	return std::wstring(L"Supports Source Engine games with context and identity support.");
}

static std::wstring description(L"Source Engine");
static std::wstring shortName(L"Source Engine");

static int tryLock1() {
	return tryLock(std::multimap< std::wstring, unsigned long long int >());
}

static void nullUnlock() {
}

static MumblePlugin sePlug = { MUMBLE_PLUGIN_MAGIC, description, shortName, nullptr, nullptr, tryLock1,
							   nullUnlock,          longDesc,    fetch };

static MumblePlugin2 sePlug2 = { MUMBLE_PLUGIN_MAGIC_2, MUMBLE_PLUGIN_VERSION, tryLock };

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin *getMumblePlugin() {
	return &sePlug;
}

extern "C" MUMBLE_PLUGIN_EXPORT MumblePlugin2 *getMumblePlugin2() {
	return &sePlug2;
}
