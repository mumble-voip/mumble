// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils.h"

#ifndef _WIN32
#	include <cerrno>
#	include <cstdio>
#	include <cstdlib>
#	include <string>
#	include <system_error>

#	include <sys/stat.h>
#	include <unistd.h>
#endif

namespace Mumble {

#ifndef _WIN32
namespace {

	void checkPrivateDirectory(const std::filesystem::path &dir) {
		const std::filesystem::file_status status = std::filesystem::symlink_status(dir);
		if (std::filesystem::is_symlink(status) || !std::filesystem::is_directory(status)) {
			throw std::filesystem::filesystem_error("Runtime directory is not a real directory", dir,
													std::make_error_code(std::errc::not_a_directory));
		}
		if ((status.permissions() & std::filesystem::perms::mask) != std::filesystem::perms::owner_all) {
			throw std::filesystem::filesystem_error("Runtime directory must have permissions 0700", dir,
													std::make_error_code(std::errc::permission_denied));
		}

		// std::filesystem::file_status does not expose the owning user ID.
		struct stat st;
		if (::lstat(dir.c_str(), &st) != 0) {
			const std::error_code error(errno, std::generic_category());
			throw std::filesystem::filesystem_error("Unable to check runtime directory owner", dir, error);
		}
		if (st.st_uid != getuid()) {
			throw std::filesystem::filesystem_error("Runtime directory is owned by another user", dir,
													std::make_error_code(std::errc::permission_denied));
		}
	}

	void checkTemporaryDirectory(const std::filesystem::path &dir) {
		const std::filesystem::file_status status = std::filesystem::status(dir);
		if (!std::filesystem::is_directory(status)) {
			throw std::filesystem::filesystem_error("Temporary path is not a directory", dir,
													std::make_error_code(std::errc::not_a_directory));
		}

		// A shared parent must prevent other users from replacing our directory after validation.
		// Follow the base's symlink here: /tmp is a symlink on macOS.
		struct stat st;
		if (::stat(dir.c_str(), &st) != 0) {
			const std::error_code error(errno, std::generic_category());
			throw std::filesystem::filesystem_error("Unable to check temporary directory owner", dir, error);
		}
		const std::filesystem::perms writableByOthers =
			std::filesystem::perms::group_write | std::filesystem::perms::others_write;
		if ((st.st_uid != getuid() && st.st_uid != 0)
			|| ((status.permissions() & writableByOthers) != std::filesystem::perms::none
				&& (status.permissions() & std::filesystem::perms::sticky_bit) == std::filesystem::perms::none)) {
			throw std::filesystem::filesystem_error("Temporary directory does not protect runtime directory ownership",
													dir, std::make_error_code(std::errc::permission_denied));
		}
	}

	void ensurePrivateDirectory(const std::filesystem::path &dir) {
		// Unlike std::filesystem::create_directory, mkdir can restrict access from the instant of
		// creation. Only create the leaf, never system-managed parents such as /run/user/<uid>.
		if (::mkdir(dir.c_str(), S_IRWXU) == 0) {
			// Restore owner permissions if the process's umask removed any. Never chmod an existing
			// directory, and do not follow a symlink if the leaf was replaced in the meantime.
			std::filesystem::permissions(dir, std::filesystem::perms::owner_all,
										 std::filesystem::perm_options::replace
											 | std::filesystem::perm_options::nofollow);
		} else if (errno != EEXIST) {
			const std::error_code error(errno, std::generic_category());
			throw std::filesystem::filesystem_error("Unable to create runtime directory", dir, error);
		}

		checkPrivateDirectory(dir);
	}

	// Prints the warning message the XDG Base Directory Specification mandates for falling back
	// away from $XDG_RUNTIME_DIR. ipcutils doesn't depend on Qt (it's a static library linked into
	// overlay_gl, which gets injected into other processes), so qWarning() isn't available here.
	void warnRuntimeDirFallback(const std::filesystem::path &dir) {
		std::fprintf(stderr, "Mumble: $XDG_RUNTIME_DIR is not available, falling back to \"%s\" for IPC endpoints\n",
					 dir.c_str());
	}

} // namespace
#endif

std::filesystem::path getRuntimeDirectory() {
#ifdef _WIN32
	return {};
#else
	static const std::filesystem::path dir = [] {
		const char *xdgRuntimeDir  = std::getenv("XDG_RUNTIME_DIR");
		std::filesystem::path base = xdgRuntimeDir ? xdgRuntimeDir : "";
		// The XDG specification requires relative values to be ignored. Select a location from
		// the environment before checking the filesystem, so a temporary failure cannot send
		// the client and overlay to different directories. They must share the same environment.
		const bool useXdg = !base.empty() && base.is_absolute();
		if (useXdg) {
			// Ignore trailing separators when checking whether the base itself is a symlink.
			while (base.has_relative_path() && base.filename().empty()) {
				base = base.parent_path();
			}
			checkPrivateDirectory(base);
		} else {
			base = std::filesystem::temp_directory_path();
			if (!base.is_absolute()) {
				throw std::filesystem::filesystem_error("Temporary directory must be absolute", base,
														std::make_error_code(std::errc::invalid_argument));
			}
			checkTemporaryDirectory(base);
		}

		const std::filesystem::path dir =
			base / (useXdg ? "info.mumble.Mumble" : "info.mumble.Mumble-" + std::to_string(getuid()));
		ensurePrivateDirectory(dir);
		if (!useXdg) {
			warnRuntimeDirFallback(dir);
		}
		return dir;
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
