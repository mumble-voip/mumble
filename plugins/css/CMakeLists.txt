# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(css SHARED "../null_plugin.cpp")

target_compile_definitions(css PRIVATE "NULL_DESC=L\"Counter Strike: Source (retracted, now using Link)\"")
