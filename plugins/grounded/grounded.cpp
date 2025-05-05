// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ProcessWindows.h"
#include "MumblePlugin.h"

#include "mumble_positional_audio_utils.h"

#include <cassert>
#include <cstring>
#include <memory>

using GroundedHandle = std::tuple< ProcessWindows, procptr_t >;

static std::unique_ptr< GroundedHandle > handle;

/* the indexes of these float[3] from the game are
 *   0: south low, north high
 *   1: west low, east high
 *   2: altitude, high toward sky, goes up when you jump */
struct GroundedCam {
	float top[3];
	std::uint8_t _unused1[4];
	float front[3];
	std::uint8_t _unused2[4];
	float pos[3];
};

static_assert(sizeof(struct GroundedCam) == 16 + 16 + 12, "GroundedCam struct has unexpected size");

constexpr float unreal_to_mumble_units(float unreal) {
	return unreal / 100.0f;
}

float float3_magnitude(float f[3]) {
	return sqrtf(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
}

bool float3_is_unit(float f[3]) {
	const float err = 0.001f;
	const float mag = float3_magnitude(f);
	return mag > (1.0f - err) && mag < (1.0f + err);
}


mumble_error_t mumble_init(uint32_t) {
	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
}

MumbleStringWrapper mumble_getName() {
	static const char name[] = "Grounded";

	MumbleStringWrapper wrapper;
	wrapper.data           = name;
	wrapper.size           = strlen(name);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	static const char description[] = "Positional audio support for Grounded. Steam release version >= 1.4.3.4578.";

	MumbleStringWrapper wrapper;
	wrapper.data           = description;
	wrapper.size           = strlen(description);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getAuthor() {
	static const char author[] = "MumbleDevelopers";

	MumbleStringWrapper wrapper;
	wrapper.data           = author;
	wrapper.size           = strlen(author);
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
	return { 1, 0, 0 };
}

uint32_t mumble_getFeatures() {
	return MUMBLE_FEATURE_POSITIONAL;
}

uint8_t mumble_initPositionalData(const char *const *programNames, const uint64_t *programPIDs, size_t programCount) {
	const std::string exename = "Maine-Win64-Shipping.exe";

	for (size_t i = 0; i < programCount; ++i) {
		if (programNames[i] != exename) {
			continue;
		}

		ProcessWindows proc(programPIDs[i], programNames[i]);

		if (!proc.isOk()) {
			continue;
		}

		const Modules &modules = proc.modules();
		const auto iter        = modules.find(exename);

		if (iter == modules.cend()) {
			continue;
		}

		// An address pointing to the start our pointer chain is moved
		// into r10. findPattern and peekRIP read executable regions of
		// memory and give us the address that would be copied to r10.
		//
		// This address is in executable code and points to a data
		// page in the executable. It's not moving around between
		// program launches. Instead, we use findPattern to support
		// different versions of the game, hopefully future versions.
		// If a new executable is shipped with a game update, and the
		// start of our pointer chain in the data page moves, we can
		// find it again by finding this code pattern and seeing what
		// it points to.
		//
		// 4C 8B 15 ?? ?? ?? ??  mov r10,qword ptr ds:[?? ?? ?? ??]
		// 44 8D 4A FF           lea r9d,qword ptr ds:[rdx-1]
		// 49 63 C1              movsxd rax,r9d
		// 49 8B 34 C2           mov rsi,qword ptr ds:[r10+rax*8]
		const std::vector< uint8_t > pattern = {
			0x4C, 0x8B, 0x15, '?',  '?', '?', '?', /**/
			0x44, 0x8D, 0x4A, 0xFF,                /**/
			0x49, 0x63, 0xC1,                      /**/
			0x49, 0x8B, 0x34, 0xC2,                /**/
		};

		procptr_t addr, ok;

		if (!(addr = proc.findPattern(pattern, iter->second))) {
			continue;
		}

		if (!(addr = proc.peekRIP(addr + 0x3))) {
			continue;
		}

		/* Only test that we can read the memory in the address we got.
		 *
		 * Normally, the memory at that address is another pointer to
		 * the start of our pointer chain. But, if we're at the main
		 * menu and haven't loaded in to any world yet, the pointer at
		 * this address will be null. We should be able to read this
		 * memory early on, but expect to read out a null pointer if we
		 * haven't loaded into the world yet. */

		if (!(proc.peek(addr, ok))) {
			continue;
		}

		handle = std::make_unique< GroundedHandle >(std::move(proc), addr);

		return MUMBLE_PDEC_OK;
	}

	return MUMBLE_PDEC_ERROR_TEMP;
}

void mumble_shutdownPositionalData() {
	handle.reset();
}

enum FollowChain {
	CHAIN_OK = 0,
	CHAIN_LATER,
	CHAIN_BAD,
};

enum FollowChain followPointerChain(const ProcessWindows &proc, const procptr_t start, GroundedCam &cam) {
	procptr_t chain;

	/* If we can't read the starting address, the program probably quit;
	 * we should shut down our positional audio */
	if (!proc.peek< procptr_t >(start, chain)) {
		return CHAIN_BAD;
	}

	if (chain == 0) {
		/* Probably at the main menu. We expect this to point to a
		 * valid address later. */
		return CHAIN_LATER;
	}

	if (!(chain = proc.peekPtr(chain))) {
		return CHAIN_BAD;
	}

	if (!(chain = proc.peekPtr(chain + 0x8))) {
		return CHAIN_BAD;
	}

	if (!proc.peek(chain + 0x700, cam)) {
		return CHAIN_BAD;
	}

	return CHAIN_OK;
}

bool mumble_fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis, float *cameraPos,
								float *cameraDir, float *cameraAxis, const char **contextPtr,
								const char **identityPtr) {
	*contextPtr  = "";
	*identityPtr = "";

	const ProcessWindows &proc = std::get< 0 >(*handle);
	const procptr_t start      = std::get< 1 >(*handle);
	GroundedCam cam;
	auto result = followPointerChain(proc, start, cam);

	switch (result) {
		case CHAIN_OK:
			/* We expect top and front to be unit vectors in the game. */
			assert(float3_is_unit(cam.top));
			assert(float3_is_unit(cam.front));

			avatarAxis[0] = cameraAxis[0] = -cam.top[0];
			avatarAxis[1] = cameraAxis[1] = cam.top[2];
			avatarAxis[2] = cameraAxis[2] = -cam.top[1];

			avatarDir[0] = cameraDir[0] = -cam.front[0];
			avatarDir[1] = cameraDir[1] = cam.front[2];
			avatarDir[2] = cameraDir[2] = -cam.front[1];

			avatarPos[0] = cameraPos[0] = unreal_to_mumble_units(cam.pos[0]);
			avatarPos[1] = cameraPos[1] = unreal_to_mumble_units(cam.pos[2]);
			avatarPos[2] = cameraPos[2] = unreal_to_mumble_units(cam.pos[1]);
			return true;

		default:
			std::fill_n(avatarPos, 3, 0.f);
			std::fill_n(avatarDir, 3, 0.f);
			std::fill_n(avatarAxis, 3, 0.f);

			std::fill_n(cameraPos, 3, 0.f);
			std::fill_n(cameraDir, 3, 0.f);
			std::fill_n(cameraAxis, 3, 0.f);

			return result == CHAIN_LATER;
	}
}
