# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
	message("Including TestPlugin in debug mode")
	add_library(testPlugin SHARED "testPlugin.cpp")
endif()
