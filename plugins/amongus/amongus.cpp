// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Game.h"

#include "MumblePlugin.h"

#include <cstring>
#include <memory>
#include <sstream>

static std::unique_ptr< Game > game;

mumble_error_t mumble_init(uint32_t) {
	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
}

MumbleStringWrapper mumble_getName() {
	static const char name[] = "Among Us";

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

MumbleStringWrapper mumble_getAuthor() {
	static const char author[] = "MumbleDevelopers";

	MumbleStringWrapper wrapper;
	wrapper.data           = author;
	wrapper.size           = strlen(author);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	static const char description[] = "Provides positional audio functionality for Among Us. "
									  "Context and identity are provided.";

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
		if (strcmp(programNames[i], "Among Us.exe") != 0) {
			continue;
		}

		game = std::make_unique< Game >(programPIDs[i], programNames[i]);

		ret = game->init();
		if (ret == MUMBLE_PDEC_OK) {
			const auto fields = game->clientFields();
			if (!game->isMultiplayer(fields)) {
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
	for (uint8_t i = 0; i < 3; ++i) {
		avatarPos[i] = cameraPos[i] = 0.f;
		avatarDir[i] = cameraDir[i] = 0.f;
		avatarAxis[i] = cameraAxis[i] = 0.f;
	}

	const auto fields = game->clientFields();

	if (!game->isMultiplayer(fields)) {
		return false;
	}

	if (fields.gameState == GameState::Ended) {
		return true;
	}

	const auto playerControlFields = game->playerControlFields();

	// Mumble | Game
	// X      | X
	// Y      | -
	// Z      | Y
	const auto position = game->playerPosition(playerControlFields);
	avatarPos[0] = cameraPos[0] = position.x;
	avatarPos[2] = cameraPos[2] = position.y;

	*contextPtr  = game->context(fields).c_str();
	*identityPtr = game->identity(fields, playerControlFields).c_str();

	return true;
}
