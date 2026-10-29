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

int test_overlay_pipe_path_warning(const char *expected, const char *warning) {
	struct {
		char text[1024];
		char guard;
	} buffer;
	const size_t sizes[] = { 0, 1, 8, sizeof(buffer.text) };
	int matches          = 1;
	for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); ++i) {
		memset(&buffer, 'x', sizeof(buffer));
		char *actual = get_overlay_pipe_path_with_warning(buffer.text, sizes[i]);
		matches &= expected == NULL ? actual == NULL : actual != NULL && strcmp(actual, expected) == 0;
		free(actual);
		if (sizes[i] == 0) {
			matches &= buffer.text[0] == 'x';
		} else {
			size_t count = strlen(warning);
			if (count >= sizes[i]) {
				count = sizes[i] - 1;
			}
			matches &= memcmp(buffer.text, warning, count) == 0 && buffer.text[count] == '\0';
			if (sizes[i] < sizeof(buffer.text)) {
				matches &= buffer.text[sizes[i]] == 'x';
			}
		}
		matches &= buffer.guard == 'x';
	}
	char *actual = get_overlay_pipe_path_with_warning(NULL, 10);
	matches &= expected == NULL ? actual == NULL : actual != NULL && strcmp(actual, expected) == 0;
	free(actual);
	return matches;
}
