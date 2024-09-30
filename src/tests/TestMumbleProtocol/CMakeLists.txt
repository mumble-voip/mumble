# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestMumbleProtocol TestMumbleProtocol.cpp)

set_target_properties(TestMumbleProtocol PROPERTIES AUTOMOC ON)

target_link_libraries(TestMumbleProtocol PRIVATE shared Qt6::Test)

add_test(NAME TestMumbleProtocol COMMAND $<TARGET_FILE:TestMumbleProtocol>)
