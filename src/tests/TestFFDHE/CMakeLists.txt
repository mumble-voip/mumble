# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestFFDHE TestFFDHE.cpp)

set_target_properties(TestFFDHE PROPERTIES AUTOMOC ON)

target_link_libraries(TestFFDHE PRIVATE shared Qt5::Test)

add_test(NAME TestFFDHE COMMAND $<TARGET_FILE:TestFFDHE>)
