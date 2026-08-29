// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils.h"

#ifndef _WIN32
#	include <cstdlib>
#	include <string>
#	include <system_error>

#	include <unistd.h>
#endif

namespace Mumble {

std::filesystem::path getRuntimeDirectory() {
#ifdef _WIN32
	return {};
#else
	// Tries to create (or reuse) the given directory using only non-throwing std::filesystem
	// calls, so that this function never throws even on a read-only or sandboxed filesystem.
	auto tryCandidate = [](const std::filesystem::path &dir) {
		std::error_code ec;
		std::filesystem::create_directories(dir, ec);
		if (!ec) {
			return true;
		}

		std::error_code isDirEc;
		return std::filesystem::is_directory(dir, isDirEc);
	};

	const char *xdgRuntimeDir = std::getenv("XDG_RUNTIME_DIR");
	if (xdgRuntimeDir != nullptr && xdgRuntimeDir[0] != '\0') {
		std::filesystem::path candidate = std::filesystem::path(xdgRuntimeDir) / "info.mumble.Mumble";
		if (tryCandidate(candidate)) {
			return candidate;
		}
	}

	// /run/user/<uid> is normally created by the system (e.g. by systemd-logind), so this
	// process must not attempt to create it or its /run parent itself. Only use it if it is
	// already there.
	{
		std::filesystem::path runUserDir = std::filesystem::path("/run/user") / std::to_string(getuid());
		std::error_code existsEc;
		if (std::filesystem::exists(runUserDir, existsEc)) {
			std::filesystem::path candidate = runUserDir / "info.mumble.Mumble";
			if (tryCandidate(candidate)) {
				return candidate;
			}
		}
	}

	// Last resort: fall back to a hidden directory in the user's home, since it is the last
	// candidate guaranteed to be owned by the current user. The shared system temp directory is
	// deliberately not used here, as it may be writable by other local users. This is returned
	// unconditionally, even if it could not be created, since there is nothing else left to try.
	const char *home               = std::getenv("HOME");
	std::filesystem::path fallback = (home != nullptr && home[0] != '\0')
										 ? std::filesystem::path(home) / ".info.mumble.Mumble"
										 : std::filesystem::path(".info.mumble.Mumble");
	tryCandidate(fallback);

	return fallback;
#endif
}

std::filesystem::path getOverlayPipePath() {
#ifdef _WIN32
	return "MumbleOverlayPipe";
#else
	return getRuntimeDirectory() / "MumbleOverlayPipe";
#endif
}

#ifdef _WIN32
std::wstring getOverlayPipeDevicePath() {
	return LR"(\\.\pipe\)" + getOverlayPipePath().wstring();
}
#endif

std::filesystem::path getSocketPath(std::string_view basename) {
#ifdef _WIN32
	return basename;
#else
	return getRuntimeDirectory() / (std::string(basename) + "Socket");
#endif
}

} // namespace Mumble
