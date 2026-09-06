// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils.h"

#ifndef _WIN32
#	include <cstdlib>
#	include <string>
#	include <system_error>

#	include <sys/stat.h>
#	include <unistd.h>
#endif

namespace Mumble {

#ifndef _WIN32
namespace {

	// Creates the given directory if it doesn't exist yet, using only non-throwing
	// std::filesystem calls, so that this never throws even on a read-only or sandboxed
	// filesystem. If the directory was created by this call, its permissions are restricted to
	// the owner (0700), since it is then a directory Mumble controls; an already-existing
	// directory's permissions are left untouched, since it may belong to the user or another
	// application.
	void ensureDirectoryCreated(const std::filesystem::path &dir) {
		std::error_code ec;
		bool created = std::filesystem::create_directories(dir, ec);
		if (created) {
			std::error_code permEc;
			std::filesystem::permissions(dir, std::filesystem::perms::owner_all, std::filesystem::perm_options::replace,
										 permEc);
		}
	}

	// Returns whether the given path is an existing directory the current user can both write to
	// and enter.
	bool isUsableDirectory(const std::filesystem::path &dir) {
		std::error_code ec;
		if (!std::filesystem::is_directory(dir, ec) || ec) {
			return false;
		}

		return ::access(dir.c_str(), W_OK | X_OK) == 0;
	}

	// Returns whether the given path is a real directory (not a symlink) owned by the current
	// user with permissions restricted to exactly 0700. lstat() is used instead of
	// std::filesystem::is_directory() so that a symlink someone else planted at this path is
	// rejected instead of followed.
	bool isPrivateOwnedDirectory(const std::filesystem::path &dir) {
		struct stat st;
		if (::lstat(dir.c_str(), &st) != 0) {
			return false;
		}

		return S_ISDIR(st.st_mode) && st.st_uid == getuid() && (st.st_mode & 07777) == S_IRWXU;
	}

} // namespace
#endif

std::filesystem::path getRuntimeDirectory() {
#ifdef _WIN32
	return {};
#else
	static const std::filesystem::path dir = [] {
		const char *xdgRuntimeDir = std::getenv("XDG_RUNTIME_DIR");
		if (xdgRuntimeDir != nullptr && xdgRuntimeDir[0] != '\0') {
			std::filesystem::path base(xdgRuntimeDir);
			if (isUsableDirectory(base)) {
				std::filesystem::path candidate = base / "info.mumble.Mumble";
				ensureDirectoryCreated(candidate);
				return candidate;
			}
		}

		// /run/user/<uid> is normally created and maintained by the system (e.g. by
		// systemd-logind), so this process must not attempt to create it or its /run parent
		// itself. Only use it if it is already there and usable.
		std::filesystem::path runUserDir = std::filesystem::path("/run/user") / std::to_string(getuid());
		if (isUsableDirectory(runUserDir)) {
			std::filesystem::path candidate = runUserDir / "info.mumble.Mumble";
			ensureDirectoryCreated(candidate);
			return candidate;
		}

		// Fall back to the system's shared temp directory. Since it is typically writable by
		// every local user, the leaf name is qualified with the current uid and only accepted if
		// it turns out to be a private directory this process itself owns; if another user got
		// there first, or the path is a symlink, the candidate is discarded instead of being used.
		std::error_code ec;
		std::filesystem::path tmpDir = std::filesystem::temp_directory_path(ec);
		if (!ec && !tmpDir.empty()) {
			std::filesystem::path candidate = tmpDir / ("info.mumble.Mumble-" + std::to_string(getuid()));
			ensureDirectoryCreated(candidate);
			if (isPrivateOwnedDirectory(candidate)) {
				return candidate;
			}
		}

		// Last resort: the current directory. This is returned unconditionally, even if it could
		// not be created, since there is nothing else left to try.
		std::filesystem::path candidate = std::filesystem::path(".") / "info.mumble.Mumble";
		ensureDirectoryCreated(candidate);
		return candidate;
	}();

	return dir;
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
