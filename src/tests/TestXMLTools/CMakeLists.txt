# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(MUMBLE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/mumble")

set(TESTXMLTOOLS_SOURCES
	TestXMLTools.cpp

	"${MUMBLE_SOURCE_DIR}/XMLTools.cpp"
	"${MUMBLE_SOURCE_DIR}/XMLTools.h"
)

add_executable(TestXMLTools ${TESTXMLTOOLS_SOURCES})

set_target_properties(TestXMLTools PROPERTIES AUTOMOC ON)

target_include_directories(TestXMLTools PRIVATE ${MUMBLE_SOURCE_DIR})

target_link_libraries(TestXMLTools PRIVATE shared Qt6::Test)

add_test(NAME TestXMLTools COMMAND $<TARGET_FILE:TestXMLTools>)
