# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Overlay payload for UNIX-like systems.

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
	option(overlay-xcompile "Build 32 bit overlay library, necessary for the overlay to work with 32 bit processes." ON)
endif()

add_library(overlay_gl SHARED "overlay.c")

set_target_properties(overlay_gl
	PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
		OUTPUT_NAME "mumbleoverlay"
		VERSION ${CMAKE_PROJECT_VERSION}
)

if(NOT APPLE)
	target_link_options(overlay_gl
		PRIVATE
			"-Wl,-z,lazy"
	)

	set_target_properties(overlay_gl
		PROPERTIES
			COMPILE_DEFINITIONS
				"TARGET_UNIX"
	)

	if(overlay-xcompile)
		message(STATUS "\nIf the 32 bit overlay library fails to compile, make sure the requirements are installed (\"g++-multilib\" package on Debian-based distributions).\n")

		set_target_properties(overlay_gl
			PROPERTIES
				OUTPUT_NAME "mumbleoverlay.x86_64"
		)

		add_library(overlay_gl_x86 SHARED "overlay.c")

		target_compile_definitions(overlay_gl_x86
			PRIVATE
				"TARGET_UNIX"
		)

		target_compile_options(overlay_gl_x86
			PRIVATE
				"-m32"
		)

		# Linking the overlay library with '-z now' requires all target processes to have libGL symbols in them at load time.
		# If it doesn't, the program will not start at all.
		#
		# Instead, explicitly use '-z lazy' to defer libGL symbol resolution until first use, which is never for non-libGL users.
		target_link_options(overlay_gl_x86
			PRIVATE
				"-m32"
				"-Wl,-z,lazy"
		)

		set_target_properties(overlay_gl_x86
			PROPERTIES
				LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
				OUTPUT_NAME "mumbleoverlay.x86"
				VERSION ${CMAKE_PROJECT_VERSION}
		)
	endif()

	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		target_link_libraries(overlay_gl
			PRIVATE
				"-ldl"
				"-lrt"
		)

		if(TARGET overlay_gl_x86)
			target_link_libraries(overlay_gl_x86
				PRIVATE
					"-ldl"
					"-lrt"
			)
		endif()
	endif()
else()
	add_subdirectory("${3RDPARTY_DIR}/mach-override-build" "${CMAKE_CURRENT_BINARY_DIR}/mach-override")

	find_library(LIB_COREFOUNDATION "CoreFoundation")

	target_compile_definitions(overlay_gl
		PRIVATE
			"TARGET_MAC"
	)
	target_compile_options(overlay_gl
		PRIVATE
			"-ObjC"
	)
	target_sources(overlay_gl
		PRIVATE
			"avail_mac.h"
			"overlay_gl.plist"
	)
	target_link_libraries(overlay_gl
		PRIVATE
			"-undefined dynamic_lookup" # Defer libGL symbol resolution until first use, which is never for non-libGL users.
			mach-override
			${LIB_COREFOUNDATION}
	)
endif()
