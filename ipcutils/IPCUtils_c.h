// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_IPCUTILS_IPCUTILS_C_H_
#define MUMBLE_IPCUTILS_IPCUTILS_C_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/// A \0-terminated heap-allocated string: the pipe path on *nix, or the pipe name on Windows.
/// The caller is responsible for free()ing it. Returns NULL if allocation fails or if the
/// runtime directory cannot be created.
char *get_overlay_pipe_path(void);

/// As get_overlay_pipe_path(), also copying any warning to the caller's buffer.
/// The buffer is always NUL-terminated when non-NULL and warningSize is nonzero.
/// Warnings are truncated to fit. A NULL buffer or zero size ignores the warning.
/// On failure the buffer is empty. The returned path must still be freed by the caller.
char *get_overlay_pipe_path_with_warning(char *warning, size_t warningSize);

#ifdef __cplusplus
}
#endif

#endif
