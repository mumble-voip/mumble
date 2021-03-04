# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestAES256GCMCrypt TestAES256GCMCrypt.cpp)

set_target_properties(TestAES256GCMCrypt PROPERTIES AUTOMOC ON)

target_include_directories(TestAES256GCMCrypt PRIVATE "${CMAKE_SOURCE_DIR}/src/murmur")

target_link_libraries(TestAES256GCMCrypt PRIVATE shared Qt5::Test)

add_test(NAME TestAES256GCMCrypt COMMAND $<TARGET_FILE:TestAES256GCMCrypt>)
