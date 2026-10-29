// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils.h"

#ifndef _WIN32
#	include <cstdlib>
#	include <string>

#	include <unistd.h>
#endif

namespace Mumble {

IPCPath getRuntimeDirectory() {
#ifdef _WIN32
	return {};
#else
	static const IPCPath s_dir = [] {
		std::filesystem::path base;
		bool useXdg               = false;
#	ifndef __APPLE__
		const char *xdgRuntimeDir = std::getenv("XDG_RUNTIME_DIR");
		base                      = xdgRuntimeDir ? xdgRuntimeDir : "";
		// The XDG specification requires relative values to be ignored. Select a location from
		// the environment before checking the filesystem, so a temporary failure cannot send
		// the client and overlay to different directories. They must share the same environment.
		useXdg = !base.empty() && base.is_absolute();
#	endif
		if (!useXdg) {
			base = std::filesystem::temp_directory_path();
		}

		const std::filesystem::path dir =
			base / (useXdg ? "info.mumble.Mumble" : "info.mumble.Mumble-" + std::to_string(getuid()));
		std::filesystem::create_directory(dir);
		std::string warning;
#	ifndef __APPLE__
		if (!useXdg) {
			warning = "$XDG_RUNTIME_DIR is not available, falling back to \"" + dir.string() + "\" for IPC endpoints";
		}
#	endif
		return IPCPath{ dir, warning };
	}();

	return s_dir;
#endif
}

IPCPath getOverlayPipePath() {
#ifdef _WIN32
	return { "MumbleOverlayPipe", {} };
#else
	IPCPath result = getRuntimeDirectory();
	result.path /= "MumbleOverlayPipe";
	return result;
#endif
}

#ifdef _WIN32
std::wstring getOverlayPipeDevicePath() {
	return LR"(\\.\pipe\)" + getOverlayPipePath().path.wstring();
}
#endif

IPCPath getSocketPath(std::string_view basename) {
#ifdef _WIN32
	return { basename, {} };
#else
	IPCPath result = getRuntimeDirectory();
	result.path /= std::string(basename) + "Socket";
	return result;
#endif
}

} // namespace Mumble
