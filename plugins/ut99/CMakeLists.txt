# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(ut99 SHARED "ut99.cpp")

find_package(utf8cpp REQUIRED)

target_link_libraries(ut99 PRIVATE $<IF:$<TARGET_EXISTS:utf8cpp::utf8cpp>,utf8cpp::utf8cpp,utf8cpp>)
