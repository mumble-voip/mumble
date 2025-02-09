# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(amongus SHARED
	"amongus.cpp"
	"Game.cpp"

	"../Module.cpp"
	"../ProcessBase.cpp"
	"../ProcessWindows.cpp"
)

if(WIN32)
	target_sources(amongus PRIVATE "../HostWindows.cpp")
else()
	target_sources(amongus PRIVATE "../HostLinux.cpp")
endif()

target_link_libraries(amongus PRIVATE utfcpp)
