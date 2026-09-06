// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_IPCUTILS_IPCUTILS_H_
#define MUMBLE_IPCUTILS_IPCUTILS_H_

#include <filesystem>
#include <string>
#include <string_view>

namespace Mumble {

/// The directory Mumble places its IPC endpoints in. It is created if it doesn't exist yet.
/// The following candidates are tried in order, and the first usable one is used:
///  - $XDG_RUNTIME_DIR, if it names an existing directory this process can write to and enter.
///  - /run/user/<uid>, if it already exists and is usable; its creation is the system's
///    responsibility (e.g. systemd-logind), so this function never attempts to create it.
///  - A directory named "info.mumble.Mumble", qualified with the current uid, inside the
///    system's shared temp directory. Since that directory is typically writable by every local
///    user, the candidate is only accepted if it turns out to be a real, non-symlinked directory
///    owned by the current user with permissions restricted to exactly 0700; otherwise another
///    local user may have created it first, and it is discarded.
///  - The current directory, as an unconditional last resort.
/// Since on Windows named pipes aren't part of the fs, it returns an empty path.
/// Its result is computed once and cached, so repeated calls always return the same path.
/// @throws std::filesystem::filesystem_error if the directory doesn't exist and can't be created.
std::filesystem::path getRuntimeDirectory();

/// The path (on *nix) or the name (on win) on which Mumble's overlay listens for connections.
/// On Windows this is a bare pipe name, meant for Qt's QLocalServer/QLocalSocket.
std::filesystem::path getOverlayPipePath();

#ifdef _WIN32
/// The full Win32 named pipe path on which Mumble's overlay listens for connections,
/// ready to be used with the raw Win32 API (CreateFile, CreateNamedPipe, ...).
std::wstring getOverlayPipeDevicePath();
#endif

/// The path (on *nix) or the name (on win) on which the specified SocketRPC endpoint listens
/// for connections. It is identified by the given basename.
std::filesystem::path getSocketPath(std::string_view basename);

} // namespace Mumble

#endif
