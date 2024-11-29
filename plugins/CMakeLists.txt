# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

option(retracted-plugins "Build redacted (outdated) plugins as well" OFF)

if(retracted-plugins)
	message(STATUS "Including retracted plugins")
endif()

add_custom_target(plugins ALL)

set(AVAILABLE_PLUGINS "")

# Plugins available on all platforms
list(APPEND AVAILABLE_PLUGINS
	"link"
)

if(${CMAKE_BUILD_TYPE} MATCHES Debug)
	message("Including TestPlugin in debug mode")
	list(APPEND AVAILABLE_PLUGINS
		"testPlugin"
		"deadLockPlugin"
	)
endif()

if(WIN32 OR (UNIX AND CMAKE_SYSTEM_NAME STREQUAL "Linux"))
	# Plugins available on Windows and Linux
	list(APPEND AVAILABLE_PLUGINS
		"amongus"
		"aoc"
		"arma2"
		"bf1"
		"bf1942"
		"bf2"
		"bf2142"
		"bf3"
		"bf4"
		"bf4_x86"
		"bfbc2"
		"bfheroes"
		"blacklight"
		"borderlands"
		"borderlands2"
		"breach"
		"cod2"
		"cod4"
		"cod5"
		"codmw2"
		"codmw2so"
		"cs"
		"css"
		"dods"
		"dys"
		"etqw"
		"ffxiv"
		"ffxiv_x64"
		"gmod"
		"gtaiv"
		"gtasa"
		"gtav"
		"gw"
		"hl2dm"
		"insurgency"
		"jc2"
		"l4d"
		"l4d2"
		"lol"
		"lotro"
		"ql"
		"rl"
		"se"
		"sr"
		"sto"
		"tf2"
		"ut2004"
		"ut3"
		"ut99"
		"wolfet"
		"wow"
		"wow_x64"
	)
endif()

list(REMOVE_DUPLICATES AVAILABLE_PLUGINS)


# Note: We are assuming that all plugins follow the convention of naming their sub-directory the same as the
# plugin cmake target. Therefore we can use the CURRENT_PLUGIN variable to reference the dir as well as the
# target.
foreach(CURRENT_PLUGIN IN LISTS AVAILABLE_PLUGINS)
	set(PLUGIN_RETRACTED OFF)

	# If the plugin is retracted the corresponding CMakeLists.txt is supposed to set the
	# PLUGIN_RETRACTED variable in the parent scope so that we can access it here
	add_subdirectory(${CURRENT_PLUGIN})

	if(PLUGIN_RETRACTED AND NOT retracted-plugins)
		# The included subdir didn't actually add a target since the associated plugin is retracted
		# and therefore it should not be built.
		continue()
	endif()

	target_include_directories(${CURRENT_PLUGIN} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

	if(WIN32)
		target_compile_definitions(${CURRENT_PLUGIN} PRIVATE "OS_WINDOWS")
		target_link_libraries(${CURRENT_PLUGIN} PRIVATE user32.lib)

		# Shared library on Windows (e.g. ".dll")
		set_target_properties(${CURRENT_PLUGIN} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
		install(TARGETS ${CURRENT_PLUGIN} RUNTIME DESTINATION "${MUMBLE_INSTALL_PLUGINDIR}" COMPONENT mumble_client)
	elseif(UNIX)
		if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
			target_compile_definitions(${CURRENT_PLUGIN} PRIVATE "OS_LINUX")
		elseif(APPLE)
			target_compile_definitions(${CURRENT_PLUGIN} PRIVATE "OS_MACOS")
		endif()

		# Shared library on UNIX (e.g. ".so")
		set_target_properties(${CURRENT_PLUGIN} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
		install(TARGETS ${CURRENT_PLUGIN} LIBRARY DESTINATION "${MUMBLE_INSTALL_PLUGINDIR}" COMPONENT mumble_client)
	endif()

	add_dependencies(plugins ${CURRENT_PLUGIN})
endforeach()
