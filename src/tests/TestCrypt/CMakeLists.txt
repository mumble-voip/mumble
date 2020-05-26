# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestCrypt TestCrypt.cpp)

set_target_properties(TestCrypt PROPERTIES AUTOMOC ON)

get_target_property(MURMUR_SOURCE_DIR murmur SOURCE_DIR)

target_include_directories(TestCrypt PRIVATE ${MURMUR_SOURCE_DIR})

target_link_libraries(TestCrypt PRIVATE shared Qt5::Test)

add_test(NAME TestCrypt COMMAND $<TARGET_FILE:TestCrypt>)
