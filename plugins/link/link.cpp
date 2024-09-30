// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>

#include "LinkedMem.h"
#include "MumblePlugin.h"
#include "SharedMemory.h"

#include <utf8/cpp11.h>

#define UNUSED(x) (void) x


constexpr const char *defaultName        = "Link";
constexpr const char *defaultDescription = "Reads positional data from a linked game/software";

std::string pluginName(defaultName);
std::string applicationName;
std::string pluginDescription(defaultDescription);
std::string pluginContext;
std::string pluginIdentity;

SharedMemory sharedMem;

std::uint32_t last_tick     = 0;
std::int64_t last_tick_time = 0;

/**
 * @returns Time in ms since Epoch
 */
static std::uint64_t getTimeSinceEpoch() {
	return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
}

static std::string wideToUtf8(const wchar_t *buf) {
	switch (sizeof(wchar_t)) {
		case 2: {
			try {
				const std::u16string str16(reinterpret_cast< const char16_t * >(buf));
				return utf8::utf16to8(str16);
			} catch (const utf8::invalid_utf16 &) {
				return {};
			}
		}
		case 4: {
			try {
				const std::u32string str32(reinterpret_cast< const char32_t * >(buf));
				return utf8::utf32to8(str32);
			} catch (const utf8::invalid_code_point &) {
				return {};
			}
		}
	}

	return {};
}

mumble_error_t mumble_init(mumble_plugin_id_t id) {
	UNUSED(id);

	if (!sharedMem.mapMemory(getLinkedMemoryName())) {
		std::cerr << "Link plugin: Failed to setup shared memory: " << sharedMem.lastError() << std::endl;

		return MUMBLE_EC_INTERNAL_ERROR;
	}

	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
	sharedMem.close();
}

MumbleStringWrapper mumble_getName() {
	MumbleStringWrapper wrapper;
	wrapper.data           = pluginName.c_str();
	wrapper.size           = pluginName.size();
	wrapper.needsReleasing = false;

	return wrapper;
}

mumble_version_t mumble_getAPIVersion() {
	return MUMBLE_PLUGIN_API_VERSION;
}

void mumble_registerAPIFunctions(void *apiStruct) {
	UNUSED(apiStruct);
}

void mumble_releaseResource(const void *pointer) {
	// This function should never be called
	UNUSED(pointer);

	std::terminate();
}

mumble_version_t mumble_getVersion() {
	return { 1, 3, 0 };
}

MumbleStringWrapper mumble_getAuthor() {
	static const char *author = "Mumble Developers";

	MumbleStringWrapper wrapper;
	wrapper.data           = author;
	wrapper.size           = std::strlen(author);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	MumbleStringWrapper wrapper;
	wrapper.data           = pluginDescription.c_str();
	wrapper.size           = pluginDescription.size();
	wrapper.needsReleasing = false;

	return wrapper;
}

uint32_t mumble_getFeatures() {
	return MUMBLE_FEATURE_POSITIONAL;
}

uint8_t mumble_initPositionalData(const char *const *programNames, const uint64_t *programPIDs, size_t programCount) {
	UNUSED(programNames);
	UNUSED(programPIDs);
	UNUSED(programCount);

	if (!sharedMem.isMemoryMapped()) {
		return MUMBLE_PDEC_ERROR_TEMP;
	}

	LinkedMem lm = sharedMem.read();

	if ((lm.uiVersion == 1) || (lm.uiVersion == 2)) {
		if (lm.uiTick != last_tick) {
			last_tick      = lm.uiTick;
			last_tick_time = getTimeSinceEpoch();

			wchar_t buff[2048];

			if (lm.name[0]) {
				wcsncpy(buff, lm.name, 256);
				buff[255]       = 0;
				applicationName = wideToUtf8(buff);

				// Call the plugin itself "Link (<whatever>)"
				pluginName += " (" + applicationName + ")";
			}

			if (lm.description[0]) {
				wcsncpy(buff, lm.description, 2048);
				buff[2047]        = 0;
				pluginDescription = wideToUtf8(buff);
			}

			return MUMBLE_PDEC_OK;
		}
	}

	return MUMBLE_PDEC_ERROR_TEMP;
}

#define SET_TO_ZERO(name) \
	name[0] = 0.0f;       \
	name[1] = 0.0f;       \
	name[2] = 0.0f
bool mumble_fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis, float *cameraPos,
								float *cameraDir, float *cameraAxis, const char **context, const char **identity) {
	SET_TO_ZERO(avatarPos);
	SET_TO_ZERO(avatarDir);
	SET_TO_ZERO(avatarAxis);
	SET_TO_ZERO(cameraPos);
	SET_TO_ZERO(cameraDir);
	SET_TO_ZERO(cameraAxis);

	LinkedMem lm = sharedMem.read();

	if (lm.uiTick != last_tick) {
		last_tick      = lm.uiTick;
		last_tick_time = getTimeSinceEpoch();
	} else if ((getTimeSinceEpoch() - last_tick_time) > 5000) {
		return false;
	}

	if ((lm.uiVersion != 1) && (lm.uiVersion != 2)) {
		return false;
	}

	for (int i = 0; i < 3; ++i) {
		avatarPos[i]  = lm.fAvatarPosition[i];
		avatarDir[i]  = lm.fAvatarFront[i];
		avatarAxis[i] = lm.fAvatarTop[i];
	}

	if (lm.uiVersion == 2) {
		for (int i = 0; i < 3; ++i) {
			cameraPos[i]  = lm.fCameraPosition[i];
			cameraDir[i]  = lm.fCameraFront[i];
			cameraAxis[i] = lm.fCameraTop[i];
		}

		if (lm.context_len > 255) {
			lm.context_len = 255;
		}
		lm.identity[255] = 0;

		pluginContext.assign(reinterpret_cast< const char * >(lm.context), lm.context_len);
		pluginIdentity = wideToUtf8(lm.identity);
	} else {
		for (int i = 0; i < 3; ++i) {
			cameraPos[i]  = lm.fAvatarPosition[i];
			cameraDir[i]  = lm.fAvatarFront[i];
			cameraAxis[i] = lm.fAvatarTop[i];
		}

		pluginContext.clear();
		pluginIdentity.clear();
	}

	*context  = pluginContext.c_str();
	*identity = pluginIdentity.c_str();

	return true;
}

#undef SET_TO_ZERO

void mumble_shutdownPositionalData() {
	if (!applicationName.empty()) {
		// We know that pluginName is in the format "Link (<whatever>)" where <whatever> is the applicationName
		pluginName.erase(pluginName.size() - applicationName.size() - 3, std::string::npos);
	} else if (applicationName.size() != std::strlen(defaultName)) {
		// This code part should actually never run, since we expect the pluginName to be modified in the described way
		// as soon as applicationName is defined.
		pluginName.clear();
		pluginName.append(defaultName);
	}

	applicationName.clear();
	pluginDescription = std::string(defaultDescription);
	pluginContext.clear();
	pluginIdentity.clear();

	sharedMem.reset();
}

MumbleStringWrapper mumble_getPositionalDataContextPrefix() {
	MumbleStringWrapper wrapper;
	wrapper.data           = applicationName.c_str();
	wrapper.size           = applicationName.size();
	wrapper.needsReleasing = false;

	return wrapper;
}
