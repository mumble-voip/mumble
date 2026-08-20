# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(ipcutils STATIC
	"IPCUtils.cpp"
	"IPCUtils.h"
	"IPCUtils_c.cpp"
	"IPCUtils_c.h"
)

target_include_directories(ipcutils PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# can be linked into overlay_gl
set_target_properties(ipcutils PROPERTIES POSITION_INDEPENDENT_CODE ON)
