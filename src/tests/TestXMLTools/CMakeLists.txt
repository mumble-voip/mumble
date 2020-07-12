# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

get_target_property(MUMBLE_SOURCE_DIR mumble SOURCE_DIR)

set(TESTXMLTOOLS_SOURCES
	TestXMLTools.cpp

	"${MUMBLE_SOURCE_DIR}/XMLTools.cpp"
	"${MUMBLE_SOURCE_DIR}/XMLTools.h"
)

add_executable(TestXMLTools ${TESTXMLTOOLS_SOURCES})

set_target_properties(TestXMLTools PROPERTIES AUTOMOC ON)

target_include_directories(TestXMLTools PRIVATE ${MUMBLE_SOURCE_DIR})

target_link_libraries(TestXMLTools PRIVATE shared Qt5::Test)

add_test(NAME TestXMLTools COMMAND $<TARGET_FILE:TestXMLTools>)
