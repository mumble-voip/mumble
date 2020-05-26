# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

find_pkg(Qt5 COMPONENTS Test REQUIRED)

file(GLOB ITEMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(ITEM ${ITEMS})
	if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
		add_subdirectory(${ITEM})
		set_target_properties(${ITEM} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
	endif()
endforeach()
