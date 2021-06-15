# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(mumble_exe WIN32)

target_include_directories(mumble_exe PRIVATE "${SHARED_SOURCE_DIR}")

configure_file("${SHARED_SOURCE_DIR}/mumble/mumble.rc.in" "${CMAKE_CURRENT_BINARY_DIR}/mumble.rc")

target_sources(mumble_exe PRIVATE
	"mumble_exe.cpp"
	"Overlay.cpp"

	"${SHARED_SOURCE_DIR}/mumble/mumble.appcompat.manifest"
	"${MUMBLE_RC}"
)

set_target_properties(mumble_exe
	PROPERTIES
		OUTPUT_NAME "mumble"
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if(MINGW)
	target_link_options(mumble_exe PRIVATE "-municode")
endif()

if(MSVC)
	# Set "/MT" for release builds and "/MTd" for debug ones.
	set_target_properties(mumble_exe PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(elevation)
	set_property(TARGET mumble_exe APPEND_STRING PROPERTY LINK_FLAGS " /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\"")
endif()

add_dependencies(mumble_exe mumble)

if(overlay)
	add_dependencies(mumble_exe overlay_exe)
endif()

target_link_libraries(mumble_exe PRIVATE shlwapi.lib)

install(TARGETS mumble_exe RUNTIME DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_client)

if(packaging)
	set(overlay ON)
	set(plugins ON)

	if(translations)
		list(APPEND installer_vars "--all-languages")
	endif()


	list(APPEND installer_vars
		"--version" ${PROJECT_VERSION}
		"--arch" "${MUMBLE_TARGET_ARCH}"
	)

	if(overlay)
		list(APPEND installer_vars
			"--overlay"
		)
	endif()

	if(g15)
		list(APPEND installer_vars
			"--g15"
		)
	endif()

	file(COPY 
		${CMAKE_SOURCE_DIR}/installer/MumbleInstall.cs
		${CMAKE_SOURCE_DIR}/installer/ClientInstaller.cs
		DESTINATION
			${CMAKE_BINARY_DIR}/installer/client
	)

	add_custom_command(TARGET mumble_exe
		POST_BUILD
		COMMAND cscs.exe -cd MumbleInstall.cs 
		COMMAND cscs.exe ClientInstaller.cs ${installer_vars}
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/installer/client
	)
endif()
