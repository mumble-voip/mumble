// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#define MUMBLE_PLUGIN_API_MAJOR_MACRO 1
#define MUMBLE_PLUGIN_API_MINOR_MACRO 0
#define MUMBLE_PLUGIN_API_PATCH_MACRO 0

#include "ProcessWindows.h"
#include "MumblePlugin.h"

#include "mumble_positional_audio_utils.h"

#include <cstring>
#include <memory>

struct Vec3 {
	float x;
	float y;
	float z;
};

struct Quat {
	float x;
	float y;
	float z;
	float w;
};

/* teardown's coordinate system is as such:
 *
 * x: west low, east high
 * y: altitude, goes up when you jump, down when you fall
 * z: north low, south high
 *
 * west and north are based on the bird's eye view from hitting tab, where the
 * top of the screen is north.
 *
 * I think this makes it a right-handed coordinate system?
 * whereas mumble is left-handed */
struct TeardownPositional {
	/* This information isn't perfect, the quaternion flops around when the
	 * player is seated in a vehicle. But the position is pretty good! When
	 * in a lift vehicle, for example, your body is in the upper part that
	 * moves vertically, and playerPos will report that position
	 * consistently in both first and third person perspectives. That is
	 * what we want because other players will always see you on the
	 * upper/vertical part of the lift, so your voice should always come
	 * from there. */
	Vec3 playerPos; /* 0x58 */
	Quat playerRot;
	/* There is a third-person camera position and quaternion immediately
	 * after the player above, but it doesn't work right when you get into
	 * a vehicle. At 0x1cc there is a more accurate camera structure that
	 * works in first-person, third-person, in a vehicle, and in the bird's
	 * eye view when hitting tab. */
	uint8_t _[0x158];
	Vec3 cameraPos; /* 0x1cc */
	Quat cameraRot;
};

struct TeardownAttachment {
	ProcessWindows proc;
	procptr_t start;
};

const uint8_t TEARDOWN_NO_MP      = 0;
const uint8_t TEARDOWN_MP_LOBBY   = 2;
const uint8_t TEARDOWN_MP_LOADING = 3;
const uint8_t TEARDOWN_MP_INGAME  = 4;

static std::unique_ptr< TeardownAttachment > handle;

static Vec3 teardown_up(Quat q) {
	/* apparently quaternion vector multiplication is:
	 *
	 * > t  = 2 * cross(q.xyz, v)
	 * > v' = v + q.w * t + cross(q.xyz, t)
	 *
	 * https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
	 *
	 * l cross r is just:
	 * > x' = l.y * r.z - r.y * l.z
	 * > y' = l.z * r.x - r.z * l.x
	 * > z' = l.x * r.y - r.x * l.y
	 *
	 * The top vector in teardown is Y up.
	 *
	 * So any multiplication in the cross product involving X and Z
	 * components are zero. */

	/*                2   * cross(q.xyz, v) */
	const float t_x = 2.f * -q.z;
	const float t_z = 2.f * q.x;

	/*      v   + q.w * t    + cross(q.xyz, t) */
	// clang-format off
	return Vec3 {
		0.f + q.w * t_x  + q.y * t_z,
		1.f              + q.z * t_x - t_z * q.x,
		0.f + q.w * t_z  +           - t_x * q.y,
	};
	// clang-format on
}

static Vec3 teardown_forward(Quat q) {
	/* The forward vector in teardown is negative Z */

	/*                2   * cross(q.xyz, v) */
	const float t_x = 2.f * -q.y;
	const float t_y = 2.f * q.x;

	/*      v   + q.w * t    + cross(q.xyz, t) */
	// clang-format off
	return Vec3 {
		 0.f + q.w * t_x +           - t_y * q.z,
		 0.f + q.w * t_y + q.z * t_x,
		-1.f             + q.x * t_y - t_x * q.y,
	};
	// clang-format on
}

/* teardown's coordinate system (right-hand y-up) to mumble (left-hand y-up) */
static void to_mumble_coordinates(Vec3 td, float *mumble) {
	mumble[0] = td.x;
	mumble[1] = td.y;
	mumble[2] = -td.z;
}

mumble_error_t mumble_init(uint32_t) {
	return MUMBLE_STATUS_OK;
}

void mumble_shutdown() {
}

MumbleStringWrapper mumble_getName() {
	static const char name[] = "Teardown";

	MumbleStringWrapper wrapper;
	wrapper.data           = name;
	wrapper.size           = strlen(name);
	wrapper.needsReleasing = false;

	return wrapper;
}

MumbleStringWrapper mumble_getDescription() {
	static const char description[] = "Positional audio support for Teardown.";

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
	const std::string exename = "teardown.exe";

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

		/*  mov rax,qword ptr ds:[?? ?? ?? ??]
		 *  mov r8d,dword ptr ds:[rcx+8]
		 *  mov rdx,qword ptr ds:[rax+38] */
		// clang-format off
		const std::vector< uint8_t > pattern = {
			0x48, 0x8B, 0x05, '?',  '?', '?', '?',
			0x44, 0x8B, 0x41, 0x08,
			0x48, 0x8B, 0x50, 0x38,
		};

		procptr_t addr, ok;

		if (   !(addr = proc.findPattern(pattern, iter->second))
		    || !(addr = proc.peekRIP(addr + 0x3))
		    || !(proc.peek(addr, ok))) {
			continue;
		}
		// clang-format on

		TeardownAttachment a = { std::move(proc), addr };
		handle               = std::make_unique< TeardownAttachment >(std::move(a));

		return MUMBLE_PDEC_OK;
	}

	return MUMBLE_PDEC_ERROR_TEMP;
}

static bool readState(const ProcessWindows &proc, procptr_t p, uint8_t &out) {
	// clang-format off
	return (p = proc.peekPtr(p))
	    && (p = proc.peekPtr(p + 0x160))
	    && (proc.peek(p + 0x4, out));
	// clang-format on
}

static bool readPositional(const ProcessWindows &proc, procptr_t p, TeardownPositional &out) {
	// clang-format off
	return (p = proc.peekPtr(p))
	    && (p = proc.peekPtr(p + 0x60))
	    && (proc.peek(p + 0x58, out));
	// clang-format on
}

bool mumble_fetchPositionalData(float *avatarPos, float *avatarDir, float *avatarAxis, float *cameraPos,
								float *cameraDir, float *cameraAxis, const char **contextPtr,
								const char **identityPtr) {
	TeardownPositional td;
	uint8_t state;
	bool ok = false;

	*contextPtr  = "";
	*identityPtr = "";

	if (!handle) {
		goto zero;
	}

	/* Reading the game state is used to disable PA outside of a
	 * multiplayer session. It is best effort. Maybe, in the future, the
	 * game updates or something and this stops working -- but the PA does
	 * work -- then instead of disabling PA entirely, it will just activate
	 * earlier than would be preferable. */
	if (readState(handle->proc, handle->start, state)) {
		/* Also, only zero out PA for states we know about. */
		if (state == TEARDOWN_NO_MP || state == TEARDOWN_MP_LOBBY || state == TEARDOWN_MP_LOADING) {
			ok = true;
			goto zero;
		}
	}

	if (!readPositional(handle->proc, handle->start, td)) {
		ok = false;
		goto zero;
	}

	/* Players with a different context string do not share positional
	 * audio. By setting a context string here, we ensure that players in
	 * game don't hear players in a menu as being at the zero world
	 * position. Similarly, players in a menu/lobby don't hear players in
	 * the game at their game positions. */
	*contextPtr = "tearing it down with your buddies";

	const Vec3 playerFw = teardown_forward(td.playerRot);
	const Vec3 playerUp = teardown_up(td.playerRot);

	to_mumble_coordinates(td.playerPos, avatarPos);
	to_mumble_coordinates(playerFw, avatarDir);
	to_mumble_coordinates(playerUp, avatarAxis);

	const Vec3 cameraFw = teardown_forward(td.cameraRot);
	const Vec3 cameraUp = teardown_up(td.cameraRot);

	to_mumble_coordinates(td.cameraPos, cameraPos);
	to_mumble_coordinates(cameraFw, cameraDir);
	to_mumble_coordinates(cameraUp, cameraAxis);

	return true;

zero:
	std::fill_n(avatarPos, 3, 0.f);
	std::fill_n(avatarDir, 3, 0.f);
	std::fill_n(avatarAxis, 3, 0.f);

	std::fill_n(cameraPos, 3, 0.f);
	std::fill_n(cameraDir, 3, 0.f);
	std::fill_n(cameraAxis, 3, 0.f);

	return ok;
}

void mumble_shutdownPositionalData() {
	handle.reset();
}
