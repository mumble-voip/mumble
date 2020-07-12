# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(mumble_exe WIN32)

target_include_directories(mumble_exe PRIVATE "${SHARED_SOURCE_DIR}")

configure_file("${SHARED_SOURCE_DIR}/mumble/${MUMBLE_RC}" ${CMAKE_CURRENT_BINARY_DIR})

target_sources(mumble_exe PRIVATE
	"mumble_exe.cpp"
	"Overlay.cpp"

	"${SHARED_SOURCE_DIR}/mumble/mumble.appcompat.manifest"
	"${CMAKE_CURRENT_BINARY_DIR}/${MUMBLE_RC}"
)

set_target_properties(mumble_exe
	PROPERTIES
		OUTPUT_NAME "mumble"
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if(MINGW)
	target_link_options(mumble_exe PRIVATE "-municode")
endif()

if(MSVC)
	# Set "/MT" for release builds and "/MTd" for debug ones.
	set_target_properties(mumble_exe PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(elevation)
	set_property(TARGET mumble_exe APPEND_STRING PROPERTY LINK_FLAGS " /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\"")
endif()

add_dependencies(mumble_exe mumble)

if(overlay)
	add_dependencies(mumble_exe overlay_exe)
endif()

target_link_libraries(mumble_exe PRIVATE shlwapi.lib)

install(TARGETS mumble_exe RUNTIME DESTINATION . COMPONENT mumble_client)
