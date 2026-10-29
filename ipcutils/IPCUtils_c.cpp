// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils_c.h"
#include "IPCUtils.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>

char *get_overlay_pipe_path(void) {
	return get_overlay_pipe_path_with_warning(nullptr, 0);
}

char *get_overlay_pipe_path_with_warning(char *warning, size_t warningSize) {
	if (warning != nullptr && warningSize > 0) {
		warning[0] = '\0';
	}
	try {
		const Mumble::IPCPath ipcPath = Mumble::getOverlayPipePath();
		const std::string path        = ipcPath.path.string();
		if (path.empty()) {
			return nullptr;
		}

		char *result = static_cast< char * >(std::malloc(path.size() + 1));
		if (result == nullptr) {
			return nullptr;
		}
		std::memcpy(result, path.c_str(), path.size() + 1);
		if (warning != nullptr && warningSize > 0) {
			const size_t count = std::min(warningSize - 1, ipcPath.warning.size());
			std::memcpy(warning, ipcPath.warning.c_str(), count);
			warning[count] = '\0';
		}
		return result;
	} catch (...) {
		return nullptr;
	}
}
