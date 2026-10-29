// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils.h"

#ifndef _WIN32
#	include <cstdio>
#	include <cstdlib>
#	include <string>

#	include <unistd.h>
#endif

namespace Mumble {

std::filesystem::path getRuntimeDirectory() {
#ifdef _WIN32
	return {};
#else
	static const std::filesystem::path s_dir = [] {
		const char *xdgRuntimeDir  = std::getenv("XDG_RUNTIME_DIR");
		std::filesystem::path base = xdgRuntimeDir ? xdgRuntimeDir : "";
		// The XDG specification requires relative values to be ignored. Select a location from
		// the environment before checking the filesystem, so a temporary failure cannot send
		// the client and overlay to different directories. They must share the same environment.
		const bool useXdg = !base.empty() && base.is_absolute();
		if (!useXdg) {
			base = std::filesystem::temp_directory_path();
		}

		const std::filesystem::path dir =
			base / (useXdg ? "info.mumble.Mumble" : "info.mumble.Mumble-" + std::to_string(getuid()));
		std::filesystem::create_directory(dir);
		if (!useXdg) {
			std::fprintf(stderr,
						 "Mumble: $XDG_RUNTIME_DIR is not available, falling back to \"%s\" for IPC endpoints\n",
						 dir.c_str());
		}
		return dir;
	}();

	return s_dir;
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
