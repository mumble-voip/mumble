# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

option(retracted-plugins "Build redacted (outdated) plugins as well" OFF)

if(retracted-plugins)
	message(STATUS "Including retracted plugins")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT WIN32 AND NOT (${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
	add_subdirectory(link)

	# Shared library on UNIX (e.g. ".so")
	set_target_properties(link PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")

	return()
endif()

file(GLOB ITEMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/*")

foreach(ITEM ${ITEMS})
	if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${ITEM}")
		set(PLUGIN_RETRACTED OFF)

		# If the plugin is retracted the corresponding CMakeLists.txt is supposed to set the
		# PLUGIN_RETRACTED variable in the parent scope so that we can access it here
		add_subdirectory(${ITEM})

		if(${ITEM} STREQUAL "testPlugin" AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug)
			# The testPlugin is only included in Debug builds
			continue()
		endif()

		if(PLUGIN_RETRACTED AND NOT retracted-plugins)
			# The included subdir didn't actually add a target since the associated plugin is retracted
			# and therefore it should not be built.
			continue()
		endif()

		target_include_directories(${ITEM} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

		if(WIN32)
			target_compile_definitions(${ITEM} PRIVATE "OS_WINDOWS")
			target_link_libraries(${ITEM} user32.lib)

			# Shared library on Windows (e.g. ".dll")
			set_target_properties(${ITEM} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
			install(TARGETS ${ITEM} RUNTIME DESTINATION "${MUMBLE_INSTALL_PLUGINDIR}" COMPONENT mumble_client)
		else()
			if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
				target_compile_definitions(${ITEM} PRIVATE "OS_LINUX")
			endif()

			# Shared library on UNIX (e.g. ".so")
			set_target_properties(${ITEM} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
			install(TARGETS ${ITEM} LIBRARY DESTINATION "${MUMBLE_INSTALL_PLUGINDIR}" COMPONENT mumble_client)
		endif()

		if(packaging)
			add_dependencies(mumble ${ITEM})
		endif()
	endif()
endforeach()
