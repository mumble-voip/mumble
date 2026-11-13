# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestXboxInput TestXboxInput.cpp)
set_target_properties(TestXboxInput PROPERTIES AUTOMOC ON)
target_link_libraries(TestXboxInput PRIVATE mumble_client_lib Qt6::Test)

add_test(
	NAME TestXboxInput
	COMMAND $<TARGET_FILE:TestXboxInput>
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)
