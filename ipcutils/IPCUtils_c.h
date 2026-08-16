// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_IPCUTILS_IPCUTILS_C_H_
#define MUMBLE_IPCUTILS_IPCUTILS_C_H_

#ifdef __cplusplus
extern "C" {
#endif

/// A \0-terminated heap-allocated string: the pipe path on *nix, or the pipe name on Windows.
/// The caller is responsible for free()ing it. Returns NULL if allocation fails or if the
/// runtime directory doesn't exist and can't be created.
char *get_overlay_pipe_path(void);

#ifdef __cplusplus
}
#endif

#endif
