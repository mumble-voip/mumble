# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(OVERLAY_EXE_RC "${CMAKE_CURRENT_BINARY_DIR}/overlay_exe.rc")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/overlay_exe.rc.in" "${OVERLAY_EXE_RC}")

add_definitions("-DUNICODE" "-DWIN32_LEAN_AND_MEAN")

# We save the output name in a variable because it's used by configure_file()
if(64_BIT AND NOT BUILD_OVERLAY_XCOMPILE)
	set(OUTPUT_NAME "mumble_ol_helper_x64")
else()
	set(OUTPUT_NAME "mumble_ol_helper")
endif()

if(NOT BUILD_OVERLAY_XCOMPILE)
	set(MUMBLE_SOURCE_ROOT "${CMAKE_SOURCE_DIR}")
endif()

add_executable(overlay_exe WIN32
	"overlay_exe.cpp"
	"overlay_exe.h"

	"${MUMBLE_SOURCE_ROOT}/auxiliary_files/mumble.appcompat.manifest"
	"${OVERLAY_EXE_RC}"
)

set_target_properties(overlay_exe PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})

if(BUILD_OVERLAY_XCOMPILE)
	set_target_properties(overlay_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MUMBLE_BINARY_DIR})

	target_include_directories(overlay_exe PRIVATE ${MUMBLE_SOURCE_ROOT}/overlay)
else()
	set_target_properties(overlay_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

	target_include_directories(overlay_exe PRIVATE ${CMAKE_SOURCE_DIR}/overlay)

	install(TARGETS overlay_exe RUNTIME DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_client)
endif()

if(MSVC)
	set_target_properties(overlay_exe PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_dependencies(overlay_exe overlay)

target_link_libraries(overlay_exe PRIVATE Shlwapi.lib)
