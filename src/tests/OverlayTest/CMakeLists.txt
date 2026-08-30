# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(OverlayTest OverlayTest.cpp)

set_target_properties(OverlayTest PROPERTIES AUTOMOC ON)

find_package(Qt6 COMPONENTS Gui Widgets REQUIRED)

target_link_libraries(OverlayTest PRIVATE mumble_client_lib)
target_link_libraries(OverlayTest PRIVATE shared ipcutils Qt6::Test Qt6::Widgets)

add_test(NAME OverlayTest COMMAND $<TARGET_FILE:OverlayTest>)

# OverlayTest is disabled because it can only be tested by humans checking that the application is correctly displaying the overlay
set_tests_properties(OverlayTest PROPERTIES DISABLED TRUE)
