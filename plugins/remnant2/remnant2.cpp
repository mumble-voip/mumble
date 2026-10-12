// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ProcessWindows.h"
#include "MumblePlugin.h"

#include "mumble_positional_audio_utils.h"

#include <cstring>
#include <memory>

struct Vec3 {
	float x, y, z;
};

struct RemnantIICamera {
	Vec3 front;
	Vec3 top;
	Vec3 position;
};

struct RemnantIIPlayer {
	Vec3 front;
	Vec3 top;
	Vec3 position;
};

struct RemnantIIAttachment {
	ProcessWindows proc;
	procptr_t camera;
	procptr_t player;
	procptr_t world;
};

static std::unique_ptr< RemnantIIAttachment > handle;

static void to_mumble_position(Vec3 unreal, float mumble[3]) {
	mumble[0] = unreal.x / 200.0f;
	mumble[1] = unreal.y / 200.0f;
	mumble[2] = unreal.z / 200.0f;
}

static void to_mumble_unit(Vec3 unreal, float mumble[3]) {
	// UE4 left-hand z-up coordinate system; do not scale unit vectors
	mumble[0] = unreal.x;
	mumble[1] = unreal.y;
	mumble[2] = unreal.z;
}

mumble_error_t mumble_init(uint32_t) {
	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
}

MumbleStringWrapper mumble_getName() {
	static const char name[] = "Remnant II";

	MumbleStringWrapper wrapper;
	wrapper.data           = name;
	wrapper.size           = strlen(name);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	static const char description[] = "Positional audio support for Remnant II.";

	MumbleStringWrapper wrapper;
	wrapper.data           = description;
	wrapper.size           = strlen(description);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getAuthor() {
	static const char author[] = "sqwishy <somebody@froghat.ca>";

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
	const std::string exename = "Remnant2-Win64-Shipping.exe";

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

		// clang-format off
		procptr_t camera, player, world;
		/* 48:8B3D ???? | mov rdi,qword ptr ds:[????]
		 * 48:85FF      | test rdi,rdi
		 * 0F84 ????    | je remnant2-win64-shipping.????
		 * 48:896C24 40 | mov qword ptr ss:[rsp+40],rbp
		 * 48:897424 48 | mov qword ptr ss:[rsp+48],rsi */
		const std::vector< uint8_t > camera_pattern = {
			0x48, 0x8b, 0x3d, '?', '?', '?', '?',
			0x48, 0x85, 0xff,
			0x0f, 0x84, '?', '?', '?', '?',
			0x48, 0x89, 0x6c, 0x24, 0x40,
			0x48, 0x89, 0x74, 0x24, 0x48,
		};
		if (   !(camera = proc.findPattern(camera_pattern, iter->second))
		    || !(camera = proc.peekRIP(camera + 0x3))
		    || !(proc.peek< procptr_t >(camera))) {
			continue;
		}
		/* 48:8B1D ???? | mov rbx,qword ptr ds:[????]
		 * 33C0         | xor eax,eax
		 * 32C9         | xor cl,cl
		 * 48:894424 50 | mov qword ptr ss:[rsp+50],rax
		 * 0FB7D0       | movzx edx,ax */
		const std::vector< uint8_t > player_pattern = {
			0x48, 0x8b, 0x1d, '?', '?', '?', '?',
			0x33, 0xc0,
			0x32, 0xc9,
			0x48, 0x89, 0x44, 0x24, 0x50,
			0x0f, 0xb7, 0xd0,
		};
		if (   !(player = proc.findPattern(player_pattern, iter->second))
		    || !(player = proc.peekRIP(player + 0x3))
		    || !(proc.peek< procptr_t >(player))) {
			continue;
		}
		/* 48:8B05 ????     | mov rax,qword ptr ds:[7FF65D531CA0]
		 * 48:8B80 28020000 | mov rax,qword ptr ds:[rax+228]
		 * 48:83C4 20       | add rsp,20
		 * 5B               | pop rbx
		 * C3               | ret */
		const std::vector< uint8_t > world_pattern = {
			0x48, 0x8b, 0x05, '?', '?', '?', '?',
			0x48, 0x8b, 0x80, 0x28, 0x02, 0x00, 0x00,
			0x48, 0x83, 0xc4, 0x20,
			0x5b,
			0xc3,
		};
		if (   !(world = proc.findPattern(world_pattern, iter->second))
		    || !(world = proc.peekRIP(world + 0x3))
		    || !(proc.peek< procptr_t >(world))) {
			continue;
		}
		// clang-format on

		RemnantIIAttachment a = { std::move(proc), camera, player, world };
		handle                = std::make_unique< RemnantIIAttachment >(std::move(a));

		return MUMBLE_PDEC_OK;
	}

	return MUMBLE_PDEC_ERROR_TEMP;
}

static bool readCamera(const ProcessWindows &proc, procptr_t p, RemnantIICamera &out) {
	// clang-format off
	return (p = proc.peekPtr(p))
	    && (p = proc.peekPtr(p + 0x20))
	    && (proc.peek(p, out));
	// clang-format on
}

static bool readPlayer(const ProcessWindows &proc, procptr_t p, RemnantIIPlayer &out) {
	// clang-format off
	return (p = proc.peekPtr(p))
	    && (p = proc.peekPtr(p))
	    && (p = proc.peekPtr(p + 0x28))
	    && (p = proc.peekPtr(p + 0x28))
	    && (p = proc.peekPtr(p + 0x20))
	    && (proc.peek(p, out));
	// clang-format on
}

static bool readUNetDriver(const ProcessWindows &proc, procptr_t p, procptr_t &out) {
	// clang-format off
	return (p = proc.peekPtr(p))
	    && (proc.peek(p + 0x38, out));
	// clang-format on
}

bool mumble_fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis, float *cameraPos,
								float *cameraDir, float *cameraAxis, const char **contextPtr,
								const char **identityPtr) {
	RemnantIICamera camera;
	RemnantIIPlayer player;
	procptr_t online;

	*contextPtr  = "";
	*identityPtr = "";

	std::fill_n(avatarPos, 3, 0.f);
	std::fill_n(avatarDir, 3, 0.f);
	std::fill_n(avatarAxis, 3, 0.f);

	std::fill_n(cameraPos, 3, 0.f);
	std::fill_n(cameraDir, 3, 0.f);
	std::fill_n(cameraAxis, 3, 0.f);

	/* If we fail to read the very start of the camera or player
	 * pointer chain, it's likely the game is shutting down or at a state
	 * where subsequent attempts will not succeed. */
	if (!handle || !(handle->proc.peekPtr(handle->camera)) || !(handle->proc.peekPtr(handle->player))) {
		return false;
	}

	/* This fails when at the main menu or the lobby is not open.
	 * Leave context unset to disable positional audio but do not unlink. */
	if (!readUNetDriver(handle->proc, handle->world, online) || !online) {
		return true;
	}

	/* If we can't read the rest of the pointer chain, the game might just
	 * be starting up. Keep the handle, but zero the values and try later. */
	if (!readCamera(handle->proc, handle->camera, camera)) {
		return true;
	}

	if (!readPlayer(handle->proc, handle->player, player)) {
		return true;
	}

	/* Set a context here to keep voice non-positional when hearing voice
	 * that encountered a branch above. */
	*contextPtr = "In World";

	to_mumble_unit(camera.top, cameraAxis);
	to_mumble_unit(camera.front, cameraDir);
	to_mumble_position(camera.position, cameraPos);

	to_mumble_unit(player.top, avatarAxis);
	to_mumble_unit(player.front, avatarDir);
	to_mumble_position(player.position, avatarPos);

	return true;
}

void mumble_shutdownPositionalData() {
	handle.reset();
}
