// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Game.h"

#include "MumblePlugin.h"

#include <algorithm>
#include <cstring>
#include <memory>

static std::unique_ptr< Game > game;

mumble_error_t mumble_init(uint32_t) {
	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
}

MumbleStringWrapper mumble_getName() {
	static const char name[] = "Grand Theft Auto V";

	MumbleStringWrapper wrapper;
	wrapper.data           = name;
	wrapper.size           = strlen(name);
	wrapper.needsReleasing = false;

	return wrapper;
}

mumble_version_t mumble_getAPIVersion() {
	return MUMBLE_PLUGIN_API_VERSION;
}

void mumble_registerAPIFunctions(void *) {
}

void mumble_releaseResource(const void *) {
}

mumble_version_t mumble_getVersion() {
	return { 2, 0, 0 };
}

MumbleStringWrapper mumble_getAuthor() {
	static const char author[] = "MumbleDevelopers";

	MumbleStringWrapper wrapper;
	wrapper.data           = author;
	wrapper.size           = strlen(author);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	static const char description[] = "Provides positional audio functionality for GTA V. "
									  "Identity is provided.";

	MumbleStringWrapper wrapper;
	wrapper.data           = description;
	wrapper.size           = strlen(description);
	wrapper.needsReleasing = false;

	return wrapper;
}

uint32_t mumble_getFeatures() {
	return MUMBLE_FEATURE_POSITIONAL;
}

uint8_t mumble_initPositionalData(const char *const *programNames, const uint64_t *programPIDs, size_t programCount) {
	auto ret = MUMBLE_PDEC_ERROR_TEMP;

	for (size_t i = 0; i < programCount; ++i) {
		if (strcmp(programNames[i], "GTA5.exe") != 0) {
			continue;
		}

		game = std::make_unique< Game >(programPIDs[i], programNames[i]);

		ret = game->init();
		if (ret == MUMBLE_PDEC_OK) {
			const CNetworkPlayerMgr mgr = game->playerMgr();
			if (!game->isMultiplayer(mgr)) {
				ret = MUMBLE_PDEC_ERROR_TEMP;
			}
		}

		if (ret != MUMBLE_PDEC_OK) {
			game.reset();
		}

		break;
	}

	return ret;
}

void mumble_shutdownPositionalData() {
	game.reset();
}

bool mumble_fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis, float *cameraPos,
								float *cameraDir, float *cameraAxis, const char **contextPtr,
								const char **identityPtr) {
	std::fill_n(avatarPos, 3, 0.f);
	std::fill_n(avatarDir, 3, 0.f);
	std::fill_n(avatarAxis, 3, 0.f);

	std::fill_n(cameraPos, 3, 0.f);
	std::fill_n(cameraDir, 3, 0.f);
	std::fill_n(cameraAxis, 3, 0.f);

	const CNetworkPlayerMgr mgr = game->playerMgr();

	if (!game->isMultiplayer(mgr)) {
		return false;
	}

	const CNetGamePlayer player = game->player(mgr);

	const CPlayerInfo info = game->playerInfo(player);
	if (info.gameState != GameState::Playing) {
		return true;
	}

	const CPed ent = game->playerEntity(info);

	std::copy(ent.position.cbegin(), ent.position.cend(), avatarPos);
	std::copy(ent.forward.cbegin(), ent.forward.cend(), avatarDir);
	std::copy(ent.up.cbegin(), ent.up.cend(), avatarAxis);

	const CPlayerAngles cam = game->playerAngles();

	std::copy(cam.position.cbegin(), cam.position.cend(), cameraPos);
	std::copy(cam.forward.cbegin(), cam.forward.cend(), cameraDir);
	std::copy(cam.up.cbegin(), cam.up.cend(), cameraAxis);

	// Mumble | Game
	// X      | X
	// Y      | Z
	// Z      | Y
	for (auto &vec : { avatarPos, avatarDir, avatarAxis, cameraPos, cameraDir, cameraAxis }) {
		std::swap(vec[1], vec[2]);
	}

	*contextPtr  = "";
	*identityPtr = game->identity(player, info, ent).c_str();

	return true;
}
