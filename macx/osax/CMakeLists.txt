# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Mumble Overlay scripting addition
# (for injection into running processes)

add_library(MumbleOverlay MODULE
	"osax.m"
	"MumbleOverlay.sdef"
)

# The overlay doesn't work well with unity builds
set_target_properties(MumbleOverlay PROPERTIES UNITY_BUILD FALSE)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/osax.plist.in" "${CMAKE_CURRENT_BINARY_DIR}/osax.plist")

set_target_properties(MumbleOverlay
	PROPERTIES
		BUNDLE TRUE
		BUNDLE_EXTENSION "osax"
		RESOURCE "MumbleOverlay.sdef"
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_BINARY_DIR}/osax.plist"
)

find_library(LIB_COCOA "Cocoa")
find_library(LIB_FOUNDATION "Foundation")

target_link_libraries(MumbleOverlay
	PRIVATE
		${LIB_COCOA}
		${LIB_FOUNDATION}
)
