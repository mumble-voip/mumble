// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils_c.h"
#include "IPCUtils.h"

#include <cstdlib>
#include <cstring>
#include <filesystem>

char *get_overlay_pipe_path(void) {
	// getOverlayPipePath() may throw if the runtime directory can't be created. That exception must
	// not unwind into the extern "C" caller, so it's turned into a NULL return instead.
	std::string path;
	try {
		path = Mumble::getOverlayPipePath().string();
	} catch (const std::filesystem::filesystem_error &) {
		return nullptr;
	}

	if (path.empty()) {
		return nullptr;
	}

	char *result = static_cast< char * >(std::malloc((path.size() + 1) * sizeof(char)));
	if (result == nullptr) {
		return nullptr;
	}

	std::memcpy(result, path.c_str(), path.size() + 1);
	return result;
}
