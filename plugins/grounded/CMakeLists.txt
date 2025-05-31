# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(grounded SHARED
	"grounded.cpp"

	"../Module.cpp"
	"../ProcessBase.cpp"
	"../ProcessWindows.cpp")

if(WIN32)
	target_sources(grounded PRIVATE "../HostWindows.cpp")
else()
	target_sources(grounded PRIVATE "../HostLinux.cpp")
endif()
