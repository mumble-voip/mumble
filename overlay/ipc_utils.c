// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ipc_utils.h"
#include <cwalk.h>
#include <string.h>

#ifdef _WIN32
#	include <direct.h>
#	include <stdlib.h>
#	include <malloc.h>
#else
#	include <pwd.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#	include <unistd.h>
#endif

char *getRuntimePath__() {
	char *path = NULL;

#ifdef __linux__
	char buffer[MUMBLE_MAX_PATH];
	char *xdgRuntimeDir = getenv("XDG_RUNTIME_DIR");
	if (xdgRuntimeDir == NULL || !xdgRuntimeDir) {
		char uid[10];
		sprintf(uid, "%d", getuid());
		xdgRuntimeDir = "/run/user/";
		cwk_path_join(xdgRuntimeDir, uid, buffer, sizeof(buffer));
	}
	size_t path_len = cwk_path_join(xdgRuntimeDir, "mumble", buffer, sizeof(buffer));
	// if (path_len != strlen(buffer))
	// 		buffer is too small. Result is truncated
	if ((path = malloc(path_len)) == NULL)
		return NULL;
	strcpy(path, buffer);
#elif defined(_WIN32)
	path = strdup("");
#else
	char *home = getenv("HOME");
	if (home == NULL) {
		struct passwd *pwent = getpwuid(getuid());
		if (pwent && pwent->pw_dir && pwent->pw_dir[0])
			home = pwent->pw_dir;
		else
			return NULL;
	}
	if ((path = malloc(strlen(home))) == NULL)
		return NULL;
	strcpy(path, home);
#endif
	return path;
}

char *getAndCreateOverlayPipePath__() {
	char buffer[MUMBLE_MAX_PATH];
	char *runtimePath      = getRuntimePath__();
	char *overlapyPipeFile = NULL;
	char *path             = NULL;
	if (runtimePath == NULL)
		return NULL;
#if _WIN32
/*
 * on Windows we don't create the directory as getRuntimePath__() returns an empty string.
 * _mkdir(runtimePath);
 */
#else
	mkdir(runtimePath, 0755);
#endif
#if !defined __linux__ && !defined _WIN32
	overlapyPipeFile = ".MumbleOverlayPipe";
#else
	overlapyPipeFile = "MumbleOverlayPipe";
#endif
	size_t path_len = cwk_path_join(runtimePath, overlapyPipeFile, buffer, sizeof(buffer));
	// if (path_len != strlen(path))
	// 		path is too small. Result is truncated
	if ((path = malloc(path_len)) == NULL)
		return NULL;
	strcpy(path, buffer);
	free(runtimePath);
	return path;
}