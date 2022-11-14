# Copyright 2020-2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(OverlayTest OverlayTest.cpp)

set_target_properties(OverlayTest PROPERTIES AUTOMOC ON)

find_pkg(Qt5 COMPONENTS Gui Widgets REQUIRED)

target_link_libraries(OverlayTest PRIVATE mumble_client_object_lib)
target_link_libraries(OverlayTest PRIVATE shared Qt5::Test Qt5::Widgets)

add_test(NAME OverlayTest COMMAND $<TARGET_FILE:OverlayTest>)

# OverlayTest is disabled because it can only be tested by humans checking that the application is correctly displaying the overlay
set_tests_properties(OverlayTest PROPERTIES DISABLED TRUE)
