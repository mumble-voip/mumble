# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(OVERLAY_EXE_RC "overlay_exe.rc")

add_definitions("-DUNICODE" "-DWIN32_LEAN_AND_MEAN")

# We save the output name in a variable because it's used by configure_file()
if(64_BIT)
	set(OUTPUT_NAME "mumble_ol_helper_x64")
else()
	set(OUTPUT_NAME "mumble_ol_helper")
endif()

get_target_property(MUMBLE_SOURCE_DIR mumble SOURCE_DIR)

configure_file(${OVERLAY_EXE_RC} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(overlay_exe WIN32
	"overlay_exe.cpp"
	"overlay_exe.h"

	"${MUMBLE_SOURCE_DIR}/mumble.appcompat.manifest"
	"${CMAKE_CURRENT_BINARY_DIR}/${OVERLAY_EXE_RC}"
)

set_property(TARGET overlay_exe PROPERTY OUTPUT_NAME ${OUTPUT_NAME})
set_property(TARGET overlay_exe PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(MSVC)
	set_property(TARGET overlay_exe PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

find_library(LIB_SHLWAPI "shlwapi")
target_link_libraries(overlay_exe PRIVATE ${LIB_SHLWAPI})

