// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "ipc_utils.h"
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
	size_t n   = 0;
	char *path = NULL;

#ifdef __linux__
	char *xdgRuntimeDir = getenv("XDG_RUNTIME_DIR");

	if (xdgRuntimeDir != NULL) {
		if (xdgRuntimeDir)
			n += strlen(xdgRuntimeDir);
		if (xdgRuntimeDir[(strlen(xdgRuntimeDir) - 1)] != '/')
			n++;
		n += strlen("mumble/");

		if ((path = malloc(n + 1)) == NULL)
			return path;
		*path = '\0';

		strcpy(path, xdgRuntimeDir);
		if (xdgRuntimeDir[(strlen(xdgRuntimeDir) - 1)] != '/')
			strcat(path, "/");
		strcat(path, "mumble/");
	} else {
		char uid[10];
		n += strlen("/run/user//mumble/");
		sprintf(uid, "%d", getuid());
		n += strlen(uid);

		if ((path = malloc(n + 1)) == NULL)
			return path;
		*path = '\0';

		strcpy(path, "/run/user/");
		strcat(path, uid);
		strcat(path, "/mumble/");
	}
#elif defined(_WIN32)
	path = strdup("");
#else
	char *home = getenv("HOME");
	if (home == NULL) {
		struct passwd *pwent = getpwuid(getuid());
		if (pwent && pwent->pw_dir && pwent->pw_dir[0])
			home = pwent->pw_dir;
	}
	if (home == NULL)
		return NULL;
	n += strlen(home);
	if (home[(strlen(home) - 1)] != '/')
		n++;
	if ((path = malloc(n + 1)) == NULL)
		return path;
	strcpy(path, home);
	if (home[(strlen(home) - 1)] != '/')
		strcat(path, "/");
#endif
	return path;
}

char *getAndCreateOverlayPipePath__() {
	char *runtimePath = getRuntimePath__();
	char *path        = NULL;
	if (runtimePath == NULL)
		return runtimePath;
#if _WIN32
/*
 * on Windows we don't create the directory as getRuntimePath__() returns an empty string.
	_mkdir(runtimePath);
*/
#else
	mkdir(runtimePath, 0755);
#endif
	size_t n = 1;
	n += strlen(runtimePath);
	n += strlen("MumbleOverlayPipe");
	path = (char *) malloc(n + 1);
	if (path == NULL)
		return path;
#if !defined __linux__ && !defined _WIN32
	strcat(path, ".");
#endif
	strcpy(path, runtimePath);
	strcat(path, "MumbleOverlayPipe");
	free(runtimePath);
	return path;
}