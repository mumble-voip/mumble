# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(gtav SHARED
	"gtav.cpp"
	"Game.cpp"

	"../Module.cpp"
	"../ProcessBase.cpp"
	"../ProcessWindows.cpp"
)

if(WIN32)
	target_sources(gtav PRIVATE "../HostWindows.cpp")
else()
	target_sources(gtav PRIVATE "../HostLinux.cpp")
endif()
