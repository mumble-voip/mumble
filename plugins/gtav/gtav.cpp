// Copyright 2016-2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Game.h"

#include "MumblePlugin_v_1_0_x.h"
#include "mumble_positional_audio_utils.h"

#include <cstring>

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
	// we reuse the GTA version as plugin version
	return { 1, 54, 2245 };
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
									  "Supports GTA V version 1.54 Build 2245 (Steam) and 1.38 (Retail). "
									  "Also provides identity based on username.";

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

		game = std::make_unique< Game >(static_cast< procid_t >(programPIDs[i]), programNames[i]);
		ret  = game->init();
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
	if (game->fetchPositionalData(avatarPos, avatarDir, avatarAxis, cameraPos, cameraDir, cameraAxis, contextPtr,
								  identityPtr)
		!= MUMBLE_PDEC_OK) {
		return false;
	}

	/*
	Mumble | Game
	X      | X
	Y      | Z
	Z      | Y
	*/
	for (auto vec : { avatarPos, avatarDir, avatarAxis, cameraPos, cameraDir, cameraAxis }) {
		// Swap Y with Z component
		std::swap(vec[1], vec[2]);
	}

	return true;
}
