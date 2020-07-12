# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(sto SHARED "../null_plugin.cpp")

target_compile_definitions(sto PRIVATE "NULL_DESC=L\"Star Trek Online (retracted due to lack of maintenance)\"")
