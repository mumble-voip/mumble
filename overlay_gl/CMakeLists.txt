# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Overlay payload for UNIX-like systems.

include(CheckIncludeFile)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "amd64")
	if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "OpenBSD")
		option(overlay-xcompile "Build 32 bit overlay library, necessary for the overlay to work with 32 bit processes." ON)
	endif()
endif()

add_library(overlay_gl SHARED "overlay.c")

set_target_properties(overlay_gl
	PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
		OUTPUT_NAME "mumbleoverlay"
		VERSION ${CMAKE_PROJECT_VERSION}
)

if(NOT APPLE)
	find_pkg(gl REQUIRED)
	target_include_directories(overlay_gl PRIVATE ${gl_INCLUDE_DIRS})

	target_link_options(overlay_gl BEFORE
		PRIVATE
			"-Wl,-z,lazy"
	)

	set_target_properties(overlay_gl
		PROPERTIES
			COMPILE_DEFINITIONS
				"TARGET_UNIX"
	)

	if(overlay-xcompile)
		# Just check for this header file while using a 32bit target as a really small and incomplete check whether g++-multilib seems to be
		# installed. If we don't find it, we can assume it's not there but if we do find it, we still don't know. Thus we still print the
		# message about the 32bit target potentially failing due to missing g++-multilib.
		CHECK_INCLUDE_FILE("sys/cdefs.h" FOUND_CDEFS "-m32")
		if(NOT FOUND_CDEFS)
			message(FATAL_ERROR "Can't find the 32bit version of sys/cdefs.h - did you install g++-multilib?")
		else()
			message(STATUS "\nIf the 32 bit overlay library fails to compile, make sure the requirements are installed (\"g++-multilib\" package on Debian-based distributions).\n")
		endif()

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
		target_link_options(overlay_gl_x86 BEFORE
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

	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux" OR ${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
		if(TARGET overlay_gl_x86)
			# install 32bit overlay library
			install(TARGETS overlay_gl_x86 LIBRARY DESTINATION "${MUMBLE_INSTALL_LIBDIR}")
		endif()
	endif()
else()
	add_subdirectory("${3RDPARTY_DIR}/mach-override-build" "${CMAKE_CURRENT_BINARY_DIR}/mach-override" EXCLUDE_FROM_ALL)

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

# install native overlay library
install(TARGETS overlay_gl LIBRARY DESTINATION "${MUMBLE_INSTALL_LIBDIR}")

