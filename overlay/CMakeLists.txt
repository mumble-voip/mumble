# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
option(BUILD_OVERLAY_XCOMPILE "Build an x86 overlay" OFF)

if(BUILD_OVERLAY_XCOMPILE)
	cmake_minimum_required(VERSION 3.15)

	set(version "1.4.0" CACHE STRING "Project version")

	project(overlay_xcompile
		VERSION ${version}
		LANGUAGES CXX
	)

	list(APPEND CMAKE_MODULE_PATH "${3RDPARTY_DIR}/cmake-compiler-flags")
	include("${MUMBLE_SOURCE_ROOT}/cmake/compiler.cmake")
endif()

set(DX11_PIXEL_SHADERS "overlay11.ps")
set(DX11_VERTEX_SHADERS "overlay11.vs")
set(EFFECTS "overlay.fx")

set(COMPILED_DX11_PIXEL_SHADERS "overlay11.ps.h")
set(COMPILED_DX11_VERTEX_SHADERS "overlay11.vs.h")
set(COMPILED_EFFECTS "overlay.hex")

set(OVERLAY_RC "${CMAKE_CURRENT_BINARY_DIR}/mumble_ol.rc")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/mumble_ol.rc.in" "${OVERLAY_RC}")

# We save the output name in a variable because it's used by configure_file()
if(64_BIT AND NOT BUILD_OVERLAY_XCOMPILE)
	set(OUTPUT_NAME "mumble_ol_x64")
else()
	set(OUTPUT_NAME "mumble_ol")
endif()

remove_definitions("-DUNICODE")

add_library(overlay SHARED)

# The overlay doesn't work well with unity builds
set_target_properties(overlay PROPERTIES UNITY_BUILD FALSE)

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

		"${OVERLAY_RC}"
)

set_target_properties(overlay PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})

find_package(Boost REQUIRED)

if(BUILD_OVERLAY_XCOMPILE)
	set_target_properties(overlay PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MUMBLE_BINARY_DIR})

	target_include_directories(overlay
		PRIVATE
			${Boost_INCLUDE_DIRS}
			${MUMBLE_SOURCE_ROOT}
			${CMAKE_CURRENT_BINARY_DIR}
	)
else()
	set_target_properties(overlay PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

	target_include_directories(overlay
		PRIVATE
			${Boost_INCLUDE_DIRS}
			${CMAKE_SOURCE_DIR}
			${CMAKE_CURRENT_BINARY_DIR}
	)

	install(TARGETS overlay RUNTIME DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_client)
endif()

if(64_BIT AND NOT BUILD_OVERLAY_XCOMPILE)
	add_subdirectory("${3RDPARTY_DIR}/minhook" "${CMAKE_CURRENT_BINARY_DIR}/minhook" EXCLUDE_FROM_ALL)
	target_compile_definitions(overlay PRIVATE "USE_MINHOOK")
	target_link_libraries(overlay PRIVATE minhook)

	set_target_properties(minhook PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(MSVC)
	# Set "/MT" for release builds and "/MTd" for debug ones.
	set_target_properties(overlay PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(64_BIT AND MSVC)
	include(ExternalProject)
	
	string(REPLACE "x64" "x86" XCOMPILE_TRIPLET "${VCPKG_TARGET_TRIPLET}")
	message(STATUS "Using cross-compilation triplet '${XCOMPILE_TRIPLET}'")

	if(CMAKE_GENERATOR MATCHES "Visual Studio")
		ExternalProject_Add(overlay_xcompile
			DOWNLOAD_COMMAND ""
			CMAKE_GENERATOR ${CMAKE_GENERATOR}
			CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
				"-DVCPKG_TARGET_TRIPLET=${XCOMPILE_TRIPLET}"
				"-DMUMBLE_SOURCE_ROOT=${CMAKE_SOURCE_DIR}"
				"-DMUMBLE_BINARY_DIR=${CMAKE_BINARY_DIR}"
				"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
				"-D3RDPARTY_DIR=${3RDPARTY_DIR}"
				# Force MSVC, because CMake prioritizes MinGW over it.
				"-DCMAKE_C_COMPILER=cl.exe"
				"-DCMAKE_CXX_COMPILER=cl.exe"
				"-DBUILD_OVERLAY_XCOMPILE=ON"
				"-Dsymbols=${symbols}"
				"-Dversion=${PROJECT_VERSION}"
				${CMAKE_SOURCE_DIR}/overlay
			CMAKE_GENERATOR_PLATFORM "Win32"
			INSTALL_COMMAND ""
		)
	else()
		ExternalProject_Add(overlay_xcompile
			DOWNLOAD_COMMAND ""
			CONFIGURE_COMMAND ""
			BUILD_COMMAND ${CMAKE_SOURCE_DIR}/overlay/scripts/build_overlay_xcompile.cmd
				-G ${CMAKE_GENERATOR}
				-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
				"-DVCPKG_TARGET_TRIPLET=${XCOMPILE_TRIPLET}"
				"-DMUMBLE_SOURCE_ROOT=${CMAKE_SOURCE_DIR}"
				"-DMUMBLE_BINARY_DIR=${CMAKE_BINARY_DIR}"
				"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
				"-D3RDPARTY_DIR=${3RDPARTY_DIR}"
				# Force MSVC, because CMake prioritizes MinGW over it.
				"-DCMAKE_C_COMPILER=cl.exe"
				"-DCMAKE_CXX_COMPILER=cl.exe"
				"-DBUILD_OVERLAY_XCOMPILE=ON"
				"-Dsymbols=${symbols}"
				"-Dversion=${PROJECT_VERSION}"
				${CMAKE_SOURCE_DIR}/overlay
			INSTALL_COMMAND ""
		)
	endif()
	install(FILES ${CMAKE_BINARY_DIR}/mumble_ol.dll ${CMAKE_BINARY_DIR}/mumble_ol_helper.exe DESTINATION . COMPONENT mumble_client)
endif()

add_subdirectory(overlay_exe)
