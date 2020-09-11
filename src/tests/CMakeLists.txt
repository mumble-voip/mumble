# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

find_pkg(Qt5 COMPONENTS Test REQUIRED)

set(TESTS "")

macro(use_test TEST_NAME)
	add_subdirectory("${TEST_NAME}")
	list(APPEND TESTS "${TEST_NAME}")
endmacro()

if(client)
	use_test("TestXMLTools")
endif()

if(server)
	use_test("TestCrypt")
endif()

# Shared tests
use_test("TestCryptographicHash")
use_test("TestCryptographicRandom")
use_test("TestFFDHE")
use_test("TestPacketDataStream")
use_test("TestPasswordGenerator")
use_test("TestSelfSignedCertificate")
use_test("TestServerAddress")
use_test("TestServerResolver")
use_test("TestSSLLocks")
use_test("TestStdAbs")
use_test("TestTimer")
use_test("TestUnresolvedServerAddress")


# Set output directory
foreach(CURRENT_TEST IN LISTS TESTS)
	set_target_properties("${CURRENT_TEST}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
endforeach()
