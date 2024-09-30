# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

find_pkg(Qt6 COMPONENTS Test REQUIRED)

option(online-tests "Whether or not tests that need a working internet connection should be included" OFF)

set(TESTS "")

macro(use_test TEST_NAME)
	add_subdirectory("${TEST_NAME}")
	list(APPEND TESTS "${TEST_NAME}")
endmacro()

if(client)
	use_test("TestXMLTools")
	if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "FreeBSD")
		# For some reason Qt segfaults when executing this test on FreeBSD without a display (even when using the offscreen plugin)
		use_test("TestSettingsJSONSerialization")
	endif()
endif()

if(server)
	use_test("TestCrypt")
	use_test("TestAudioReceiverBuffer")
endif()

# Shared tests
use_test("TestCryptographicHash")
use_test("TestCryptographicRandom")
use_test("TestFFDHE")
use_test("TestPacketDataStream")
use_test("TestPasswordGenerator")
use_test("TestMumbleProtocol")
use_test("TestSelfSignedCertificate")
use_test("TestServerAddress")
use_test("TestSSLLocks")
use_test("TestStdAbs")
use_test("TestTimer")
use_test("TestUnresolvedServerAddress")
use_test("TestVersion")

if(online-tests)
	message(STATUS "Including online tests - These will fail if you don't have a working internet connection when running them")
	use_test("TestServerResolver")
else()
	message(STATUS "Omitting online tests - Testing can be performed without an active internet connection")
endif()


# Set output directory
foreach(CURRENT_TEST IN LISTS TESTS)
	set_target_properties("${CURRENT_TEST}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
endforeach()
