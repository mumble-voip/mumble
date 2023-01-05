# Copyright 2020-2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.


cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0069 NEW)
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.20.0")
	cmake_policy(SET CMP0118 NEW)
endif()

set(BUILD_NUMBER CACHE STRING "The build number of the current build. Will be used in Mumble's version to make sure newer builds upgrade older installations properly.")
set(BUILD_RELEASE_DATE CACHE STRING "The release date to be used in the generated appstream metadata.")

if ("${BUILD_NUMBER}" STREQUAL "")
	if(packaging)
		message(FATAL_ERROR "Tried to create a Mumble package, without specifying BUILD_NUMBER!")
	else()
		set(BUILD_NUMBER "0")
	endif()
endif()

# Get compilation year
string(TIMESTAMP MUMBLE_BUILD_YEAR "%Y")

project(Mumble
	VERSION "1.6.${BUILD_NUMBER}"
	DESCRIPTION "Open source, low-latency, high quality voice chat."
	HOMEPAGE_URL "https://www.mumble.info"
	LANGUAGES "C" "CXX"
)

set(3RDPARTY_DIR "${CMAKE_SOURCE_DIR}/3rdparty")
set(PLUGINS_DIR "${CMAKE_SOURCE_DIR}/plugins")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)

list(APPEND CMAKE_MODULE_PATH
	"${CMAKE_SOURCE_DIR}/cmake"
	"${CMAKE_SOURCE_DIR}/cmake/FindModules"
	"${3RDPARTY_DIR}/FindPythonInterpreter"
)


include(pkg-utils)
include(project-utils)
include(TargetArch)
include(CheckIPOSupported)
include(FindPythonInterpreter)

# Locate a usable Python3 interpreter
set(PYTHON_HINTS
	"C:/Python39-x64" # Path on the AppVeyor CI server
)

find_python_interpreter(
	VERSION 3
	INTERPRETER_OUT_VAR PYTHON_INTERPRETER
	HINTS ${PYTHON_HINTS}
	REQUIRED
)



check_ipo_supported(RESULT LTO_DEFAULT)



option(optimize "Build a heavily optimized version, specific to the machine it's being compiled on." OFF)
option(static "Build static binaries." OFF)
option(symbols "Build binaries in a way that allows easier debugging." OFF)
option(warnings-as-errors "All warnings are treated as errors." ON)

option(client "Build the client (Mumble)" ON)
option(server "Build the server (Murmur)" ON)
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD")
	option(overlay "Build overlay." ${client})
endif()
option(packaging "Build package." OFF)
option(tests "Build tests." ${packaging})
option(plugins "Build plugins." ON)

option(debug-dependency-search "Prints extended information during the search for the needed dependencies" OFF)

if(NOT CMAKE_BUILD_TYPE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(
        "-DDEBUG"
        "-DSNAPSHOT_BUILD"
    )

	# Disable LTO in Debug builds in order to reduce time required for linking
	set(LTO_DEFAULT OFF)
endif()

option(lto "Enables link-time optimizations" ${LTO_DEFAULT})

include(compiler)
include(os)

target_architecture(MUMBLE_TARGET_ARCH)
string(TOLOWER "${MUMBLE_TARGET_ARCH}" MUMBLE_TARGET_ARCH)

message(STATUS "##################################################")
message(STATUS "Mumble version:          ${PROJECT_VERSION}")
message(STATUS "Architecture:            ${MUMBLE_TARGET_ARCH}")
if(NOT IS_MULTI_CONFIG)
    message(STATUS "Build type:              ${CMAKE_BUILD_TYPE}")
else()
    message(STATUS "Using multi-config generator that will determine build type on-the-fly")
endif()
message(STATUS "Using LTO:               ${lto}")
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

if (WIN32)
	set(MUMBLE_TARGET_OS "windows")
elseif (APPLE)
	set(MUMBLE_TARGET_OS "macos")
elseif (UNIX)
	set(MUMBLE_TARGET_OS "linux")
else()
	message(FATAL_ERROR "Unable to determine target OS")
endif()


# Make the build year accessible as a macro
add_compile_definitions(MUMBLE_BUILD_YEAR=${MUMBLE_BUILD_YEAR})

# Make sure that math constants are always defined
add_compile_definitions(_USE_MATH_DEFINES)


# Provide the information about the target architecture to all Mumble source files in form of a macro
add_compile_definitions(MUMBLE_TARGET_ARCH="${MUMBLE_TARGET_ARCH}")

# Also provide information about the target OS
add_compile_definitions(MUMBLE_TARGET_OS="${MUMBLE_TARGET_OS}")


set(CMAKE_UNITY_BUILD_BATCH_SIZE 40)

if(client OR server)
	add_subdirectory(src)
endif()

if(g15 AND WIN32)
	add_subdirectory("helpers/g15helper")
endif()

if(APPLE AND MUMBLE_TARGET_ARCH MATCHES "arm.*")
	# mach_override doesn't support ARM
	# https://github.com/rentzsch/mach_override/issues/6
	set(overlay OFF CACHE BOOL "" FORCE)
	message(STATUS "Disabling the overlay on ARM macOS")
endif()

if(overlay)
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

add_subdirectory(auxiliary_files)

if(packaging)
	file(COPY 
		${CMAKE_SOURCE_DIR}/installer/gpl.txt
		${CMAKE_SOURCE_DIR}/installer/lgpl.txt
		${CMAKE_SOURCE_DIR}/installer/Mumble.rtf
		${CMAKE_SOURCE_DIR}/installer/portaudio.txt
		${CMAKE_SOURCE_DIR}/installer/qt.txt
		${CMAKE_SOURCE_DIR}/installer/speex.txt
		DESTINATION 
			${CMAKE_BINARY_DIR}/licenses
	)
	if(WIN32)
		file(COPY
			${CMAKE_SOURCE_DIR}/installer/bannrbmp.bmp
			${CMAKE_SOURCE_DIR}/installer/dlgbmp.bmp
			DESTINATION
				${CMAKE_BINARY_DIR}/installer
		)
		
		file(COPY 
			${CMAKE_SOURCE_DIR}/icons/mumble.ico
			${CMAKE_SOURCE_DIR}/icons/murmur.ico
			DESTINATION 
				${CMAKE_BINARY_DIR}/installer/icons
		)
	endif()
endif()
