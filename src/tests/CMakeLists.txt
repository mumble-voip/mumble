# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(project-utils)

find_pkg(Qt6 COMPONENTS Test REQUIRED)

option(online-tests "Whether or not tests that need a working internet connection should be included" OFF)
option(test-lto "Whether to use LTO when building test cases" ${lto})

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${test-lto})

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")

if (static)
	find_pkg(Qt6 VERSION 6 COMPONENTS Widgets REQUIRED)
	if (NOT TARGET Qt6::QOffscreenIntegrationPlugin)
		message(FATAL_ERROR "Unable to locate offscreen plugin (needed for tests)")
	endif()
	add_library(mumble_test_qt_plugins STATIC)
	include_qt_plugin(mumble_test_qt_plugins PRIVATE "QOffscreenIntegrationPlugin")
	target_link_libraries(mumble_test_qt_plugins PUBLIC Qt6::Widgets Qt6::QOffscreenIntegrationPlugin)

	link_libraries(mumble_test_qt_plugins)
endif()

if(client)
	add_subdirectory("TestXMLTools")
	if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD")
		# For some reason Qt segfaults when executing this test on FreeBSD without a display (even when using the offscreen plugin)
		add_subdirectory("TestSettingsJSONSerialization")
	endif()
endif()

if(server)
	add_subdirectory("TestCrypt")
	add_subdirectory("TestAudioReceiverBuffer")
endif()

# Shared tests
add_subdirectory("TestCaseInsensitiveQString")
add_subdirectory("TestCryptographicHash")
add_subdirectory("TestCryptographicRandom")
add_subdirectory("TestDatabase")
add_subdirectory("TestFFDHE")
add_subdirectory("TestPacketDataStream")
add_subdirectory("TestPasswordGenerator")
add_subdirectory("TestMumbleProtocol")
add_subdirectory("TestSelfSignedCertificate")
add_subdirectory("TestServerAddress")
add_subdirectory("TestSSLLocks")
add_subdirectory("TestStdAbs")
add_subdirectory("TestTimer")
add_subdirectory("TestUnresolvedServerAddress")
add_subdirectory("TestVersion")

if(online-tests)
	message(STATUS "Including online tests - These will fail if you don't have a working internet connection when running them")
	add_subdirectory("TestServerResolver")
else()
	message(STATUS "Omitting online tests - Testing can be performed without an active internet connection")
endif()
