# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestServerDatabase
	ServerDatabaseTest.cpp

	"${CMAKE_CURRENT_SOURCE_DIR}/../TestUtils.cpp"
)

target_compile_definitions(TestServerDatabase PRIVATE ${MUMBLE_DB_TEST_DEFINES})

set_target_properties(TestServerDatabase PROPERTIES AUTOMOC ON)

target_link_libraries(TestServerDatabase PRIVATE Qt5::Test mumble_server_object_lib)

# Add parent directory to include path to be able to share helper functions with general DB tests
target_include_directories(TestServerDatabase PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/..")

add_test(NAME TestServerDatabase COMMAND $<TARGET_FILE:TestServerDatabase>)
