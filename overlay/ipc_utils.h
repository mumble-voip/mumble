// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_OVERLAY_IPC_UTILS_H__
#define MUMBLE_OVERLAY_IPC_UTILS_H__

// can't trust POSIX's nor Window's PATH_MAX constants
// see: https://eklitzke.org/path-max-is-tricky
//      https://stackoverflow.com/a/56385296
const int MUMBLE_MAX_PATH = 1024;

char *getRuntimePath__();

char *getAndCreateOverlayPipePath__();

#endif // MUMBLE_OVERLAY_IPC_UTILS_H__