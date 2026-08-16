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
/// Since on Windows named pipes aren't part of the fs, it returns an empty path.
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
