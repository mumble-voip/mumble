# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestOCB2Crypt TestOCB2Crypt.cpp)

set_target_properties(TestOCB2Crypt PROPERTIES AUTOMOC ON)

target_include_directories(TestOCB2Crypt PRIVATE "${CMAKE_SOURCE_DIR}/src/murmur")

target_link_libraries(TestOCB2Crypt PRIVATE shared Qt5::Test)

add_test(NAME TestOCB2Crypt COMMAND $<TARGET_FILE:TestOCB2Crypt>)
