# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_subdirectory(overlay_exe)

set(DX11_PIXEL_SHADERS "overlay11.ps")
set(DX11_VERTEX_SHADERS "overlay11.vs")
set(EFFECTS "overlay.fx")

set(COMPILED_DX11_PIXEL_SHADERS "overlay11.ps.h")
set(COMPILED_DX11_VERTEX_SHADERS "overlay11.vs.h")
set(COMPILED_EFFECTS "overlay.hex")

set(OVERLAY_RC "mumble_ol.rc")

# We save the output name in a variable because it's used by configure_file()
if(64_BIT)
	set(OUTPUT_NAME "mumble_ol_x64")
else()
	set(OUTPUT_NAME "mumble_ol")
endif()

remove_definitions("-DUNICODE")

add_library(overlay SHARED)

configure_file(${OVERLAY_RC} ${CMAKE_CURRENT_BINARY_DIR})

find_program(FXC "fxc" DOC "Effect-Compiler tool")
if(NOT FXC)
	message(FATAL_ERROR "Effect-Compiler tool not found!")
endif()

add_custom_command(
	OUTPUT ${COMPILED_DX11_PIXEL_SHADERS}
	COMMAND ${FXC} "/Tps_5_0" "/O3" "/Vng_pixel_shader" "/Fh" "${CMAKE_CURRENT_BINARY_DIR}/${COMPILED_DX11_PIXEL_SHADERS}" "${CMAKE_CURRENT_SOURCE_DIR}/${DX11_PIXEL_SHADERS}"
	MAIN_DEPENDENCY ${DX11_PIXEL_SHADERS}
	COMMENT "Compiling ${DX11_PIXEL_SHADERS}"
	VERBATIM
)

add_custom_command(
	OUTPUT ${COMPILED_DX11_VERTEX_SHADERS}
	COMMAND ${FXC} "/Tvs_5_0" "/O3" "/Vng_vertex_shader" "/Fh" "${CMAKE_CURRENT_BINARY_DIR}/${COMPILED_DX11_VERTEX_SHADERS}" "${CMAKE_CURRENT_SOURCE_DIR}/${DX11_VERTEX_SHADERS}"
	MAIN_DEPENDENCY ${DX11_VERTEX_SHADERS}
	COMMENT "Compiling ${DX11_VERTEX_SHADERS}"
	VERBATIM
)

add_custom_command(
	OUTPUT ${COMPILED_EFFECTS}
	COMMAND ${FXC} "/Tfx_4_0" "/O3" "/Fh" "${CMAKE_CURRENT_BINARY_DIR}/${COMPILED_EFFECTS}" "${CMAKE_CURRENT_SOURCE_DIR}/${EFFECTS}"
	MAIN_DEPENDENCY ${EFFECTS}
	COMMENT "Compiling ${EFFECTS}"
	VERBATIM
)

add_custom_target(build-overlay-files ALL DEPENDS
	"${CMAKE_CURRENT_BINARY_DIR}/${COMPILED_EFFECTS}"
	"${CMAKE_CURRENT_BINARY_DIR}/${COMPILED_DX11_VERTEX_SHADERS}"
	"${CMAKE_CURRENT_BINARY_DIR}/${COMPILED_DX11_PIXEL_SHADERS}"
)

target_include_directories(overlay PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(overlay build-overlay-files)

target_sources(overlay
	PRIVATE
		"D11StateBlock.cpp"
		"D11StateBlock.h"
		"HardHook.cpp"
		"HardHook.h"
		"ancestor.cpp"
		"ancestor.h"
		"d3d9.cpp"
		"d3d10.cpp"
		"d3d11.cpp"
		"dxgi.cpp"
		"excludecheck.cpp"
		"excludecheck.h"
		"lib.cpp"
		"lib.h"
		"ods.cpp"
		"ods.h"
		"olsettings.cpp"
		"olsettings.h"
		"opengl.cpp"
		"overlay_blacklist.h"
		"overlay_launchers.h"
		"overlay_whitelist.h"

		"${CMAKE_CURRENT_BINARY_DIR}/${OVERLAY_RC}"
		"${3RDPARTY_DIR}/GL/glext.h"
)

set_property(TARGET overlay PROPERTY OUTPUT_NAME ${OUTPUT_NAME})
set_property(TARGET overlay PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_pkg(Boost REQUIRED)
target_include_directories(overlay PRIVATE ${Boost_INCLUDE_DIRS})

if(64_BIT)
	add_subdirectory("${3RDPARTY_DIR}/minhook-src" "${CMAKE_CURRENT_BINARY_DIR}/minhook" EXCLUDE_FROM_ALL)
	target_compile_definitions(overlay PRIVATE "USE_MINHOOK")
	target_link_libraries(overlay PRIVATE minhook)
endif()

if(MSVC)
	# Set "/MT" for release builds and "/MTd" for debug ones.
	set_property(TARGET overlay PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
