# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestStdAbs TestStdAbs.cpp)

set_target_properties(TestStdAbs PROPERTIES AUTOMOC ON)

target_link_libraries(TestStdAbs PRIVATE shared Qt6::Test)

add_test(NAME TestStdAbs COMMAND $<TARGET_FILE:TestStdAbs>)
