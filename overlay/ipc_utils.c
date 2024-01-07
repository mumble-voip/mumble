// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ipc_utils.h"
// #include <cwalk.h>
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

// can't trust POSIX's nor Window's PATH_MAX constants
// see: https://eklitzke.org/path-max-is-tricky
//      https://stackoverflow.com/a/56385296
#define MUMBLE_MAX_PATH 1024

char *getRuntimePath__() {
	char buffer[MUMBLE_MAX_PATH];
	char *path = NULL;

#ifdef _WIN32
	path = strdup("\0");
#elif __linux__
	char *xdgRuntimeDir = getenv("XDG_RUNTIME_DIR");
	if (xdgRuntimeDir == NULL || !xdgRuntimeDir) {
		// char uid[10];
		// sprintf(uid, "%d", getuid());
		xdgRuntimeDir = "/run/user/";
		// cwk_path_join(xdgRuntimeDir, uid, buffer, sizeof(buffer));
		snprintf(buffer, sizeof(buffer), "%s/%d", xdgRuntimeDir, getuid());
	}
	// size_t path_len = cwk_path_join(xdgRuntimeDir, "mumble", buffer, sizeof(buffer));
	int path_len        = snprintf(buffer, sizeof(buffer), "%s/mumble/", xdgRuntimeDir);
#else
	char *home = getenv("HOME");
	if (home == NULL) {
		struct passwd *pwent = getpwuid(getuid());
		if (pwent && pwent->pw_dir && pwent->pw_dir[0])
			home = pwent->pw_dir;
		else
			return NULL;
	}
	if ((path = malloc(strlen(home) + 1)) == NULL)
		return NULL;
	int path_len = snprintf(buffer, sizeof(buffer), "%s/", home);
#endif
#if !defined _WIN32
	// if (path_len != strlen(buffer))
	// 		buffer is too small. Result is truncated
	if ((path = malloc(path_len + 1)) == NULL)
		return NULL;
	strcpy(path, buffer);
#endif
	return path;
}

char *getAndCreateOverlayPipePath__() {
	char buffer[MUMBLE_MAX_PATH];
	char *overlapyPipeFile = "MumbleOverlayPipe";
	char *path             = NULL;
	char *runtimePath      = getRuntimePath__();
	if (runtimePath == NULL)
		return NULL;
#if !defined __linux__ && !defined _WIN32
	char *path_template = "%s.%s";
#else
	char *path_template = "%s%s";
#endif
#if _WIN32
/*
 * on Windows we don't create the directory as getRuntimePath__() returns an empty string.
 * _mkdir(runtimePath);
 */
#else
	mkdir(runtimePath, 0755);
#endif
	// size_t path_len = cwk_path_join(runtimePath, overlapyPipeFile, buffer, sizeof(buffer));
	int path_len = snprintf(buffer, sizeof(buffer), path_template, runtimePath, overlapyPipeFile);
	// if (path_len != strlen(path))
	// 		path is too small. Result is truncated
	if ((path = malloc(path_len + 1)) == NULL)
		return NULL;
	strcpy(path, buffer);
	free(runtimePath);
	return path;
}
