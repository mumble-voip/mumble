# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(link SHARED
	"link.cpp"
	"SharedMemory.cpp"
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	find_library(LIB_RT "rt")
	target_link_libraries(link PRIVATE ${LIB_RT})
endif()

target_include_directories(link PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(link PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")
