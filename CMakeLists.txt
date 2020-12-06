# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0091 NEW)

set(version "1.4.0" CACHE STRING "Project version")

string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]" version_short ${version})

# Get compilation year
string(TIMESTAMP MUMBLE_BUILD_YEAR "%Y")

project(Mumble
	VERSION ${version_short}
	DESCRIPTION "Open source, low-latency, high quality voice chat."
	HOMEPAGE_URL "https://www.mumble.info"
	LANGUAGES "C" "CXX"
)

set(3RDPARTY_DIR "${CMAKE_SOURCE_DIR}/3rdparty")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.9)

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
	"${CMAKE_SOURCE_DIR}/cmake/FindModules"
)


include(pkg-utils)
include(project-utils)


option(tests "Build tests" OFF)

option(optimize "Build a heavily optimized version, specific to the machine it's being compiled on." OFF)
option(static "Build static binaries." OFF)
option(symbols "Build binaries in a way that allows easier debugging." OFF)
option(warnings-as-errors "All warnings are treated as errors." ON)

option(overlay "Build overlay." ON)
option(packaging "Build package." OFF)
option(plugins "Build plugins." ON)

option(debug-dependency-search "Prints extended information during the search for the needed dependencies" OFF)

if(NOT CMAKE_BUILD_TYPE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(
        "-DDEBUG"
        "-DSNAPSHOT_BUILD"
    )
endif()

include(compiler)
include(os)

if (64_BIT)
	set(ARCH "64bit")
else()
	set(ARCH "32bit")
endif()

message(STATUS "##################################################")
message(STATUS "Mumble version:  ${version}")
message(STATUS "Architecture:    ${ARCH}")
if(NOT IS_MULTI_CONFIG)
    message(STATUS "Build type:      ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Using multi-config generator that will determine build type on-the-fly")
endif()
message(STATUS "##################################################")

include(install-paths)

# We have to check for BUILD_TESTING before including CTest as CTest defines this variable
if(DEFINED BUILD_TESTING AND NOT BUILD_TESTING_CHECKED)
	message(WARNING "Use of option \"BUILD_TESTING\" is deprecated. Use \"tests\" instead.")

	if(NOT tests)
		# Allow deprecated option to enable tests if they had been disabled otherwise
		set(tests "${BUILD_TESTING}")
	endif()
endif()

set(BUILD_TESTING_CHECKED ON INTERNAL CACHE BOOL "Persistent helper variable" FORCE)

if(tests)
	include(CTest)
endif()


add_subdirectory(src)

if(g15 AND WIN32)
	add_subdirectory(g15helper)
endif()

if(overlay AND client)
	if(WIN32)
		add_subdirectory(overlay)
	else()
		add_subdirectory(overlay_gl)

		if(APPLE)
			add_subdirectory(macx/osax)
		endif()
	endif()
endif()

if(plugins AND client)
	add_subdirectory(plugins)
endif()

if(packaging)
	if(WIN32)
		file(COPY "${CMAKE_SOURCE_DIR}/scripts/Create-Win32InstallerMUI.ps1" DESTINATION ${CMAKE_BINARY_DIR})
	endif()

	include(cmake/packaging.cmake)
endif()
