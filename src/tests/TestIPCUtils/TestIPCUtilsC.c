// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils_c.h"

#include <stdlib.h>
#include <string.h>

/* Compile this caller as C, so both the public header and the C ABI are exercised. */
int test_overlay_pipe_path(const char *expected) {
	char *actual = get_overlay_pipe_path();
	int matches  = expected == NULL ? actual == NULL : actual != NULL && strcmp(actual, expected) == 0;
	free(actual);
	return matches;
}
