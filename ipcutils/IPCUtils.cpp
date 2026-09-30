// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils.h"

#ifndef _WIN32
#	include <cstdlib>

#	include <unistd.h>
#endif

namespace Mumble {

std::filesystem::path getRuntimeDirectory() {
#ifdef _WIN32
	return {};
#else
	std::filesystem::path runtimeDir;

	const char *xdgRuntimeDir = std::getenv("XDG_RUNTIME_DIR");
	if (xdgRuntimeDir != nullptr && xdgRuntimeDir[0] != '\0') {
		runtimeDir = std::filesystem::path(xdgRuntimeDir) / "info.mumble.Mumble";
	} else {
		runtimeDir = std::filesystem::path("/run/user") / std::to_string(getuid()) / "info.mumble.Mumble";
	}

	std::filesystem::create_directories(runtimeDir);

	return runtimeDir;
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
