# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>. 

if(tests)
	if(WIN32 AND NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
		# We're building for Windows on a different operating system.
		find_program(WINE
			NAMES
				"wine"
				"wine-development"
			DOC
				"Wine (to run tests)"
		)
		if(WINE)
			set(CMAKE_CROSSCOMPILING_EMULATOR ${WINE})
			message(STATUS "The following Wine binary will be used to run tests: \"${WINE}\"")
		else()
			message(STATUS "You are cross-compiling for Windows but don't have Wine, you will not be able to run tests.")
		endif()
	endif()
endif()

# Qt-related performance tweaks.
add_definitions(
	"-DQT_USE_FAST_CONCATENATION"
	"-DQT_USE_FAST_OPERATOR_PLUS"
)

if(WIN32)
	add_definitions(
		"-DUNICODE"
		"-DWIN32_LEAN_AND_MEAN"
		# Prevent Windows headers from defining the macros "min" and "max" that mess up e.g. std::min usage
		"-DNOMINMAX"
	)
else()
	if(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
		include_directories("/usr/local/include")
		link_directories("/usr/local/lib")
	endif()

	find_pkg(OpenSSL QUIET)
	find_pkg(Qt5 QUIET)

	if(NOT OpenSSL_FOUND)
		if(APPLE)
			# Homebrew
			set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl")
		endif()
	endif()

	if(NOT Qt5_FOUND)
		if(APPLE)
			# Homebrew
			set(Qt5_DIR "/usr/local/opt/qt5/lib/cmake/Qt5")
		elseif(${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD")
			set(Qt5_DIR "/usr/local/lib/qt5/cmake/Qt5")
		endif()
	endif()
endif()
