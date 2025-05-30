# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(CompilerFlags)
include(CheckCXXCompilerFlag)

if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
	set(64_BIT TRUE)
elseif(${CMAKE_SIZEOF_VOID_P} EQUAL 4)
	set(32_BIT TRUE)
else()
	message(FATAL_ERROR "Unknown architecture - only 32bit and 64bit are supported")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(WIN32)
	message(STATUS "Using Win7 as the oldest supported Windows version")
	add_compile_definitions(_WIN32_WINNT=0x0601)
endif()

set(WANTED_FEATURES "ENABLE_MOST_WARNINGS" "ENSURE_DEFAULT_CHAR_IS_SIGNED")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	list(APPEND WANTED_FEATURES "OPTIMIZE_FOR_SPEED")
endif()

if(warnings-as-errors)
	list(APPEND WANTED_FEATURES "ENABLE_WARNINGS_AS_ERRORS")
endif()

get_compiler_flags(
	${WANTED_FEATURES}
	OUTPUT_VARIABLE MUMBLE_COMPILER_FLAGS
)

message(STATUS "Using (among others) the following compiler flags: ${MUMBLE_COMPILER_FLAGS}")

if(MSVC)
	if(32_BIT)
		# SSE2 code is generated by default, unless an explicit arch is set.
		# Our 32 bit binaries should not contain any SSE2 code, so override the default.
		add_compile_options("-arch:SSE")
	endif()

	if(symbols)
		# Configure build to be able to properly debug release builds (https://docs.microsoft.com/cpp/build/how-to-debug-a-release-build).
		# This includes explicitly disabling /Oy to help debugging (https://docs.microsoft.com/cpp/build/reference/oy-frame-pointer-omission).
		# Also set /Zo to enhance optimized debugging (https://docs.microsoft.com/cpp/build/reference/zo-enhance-optimized-debugging).
		add_compile_options(
			"/GR"
			"/Zi"
			"/Zo"
			"/Oy-"
		)
		add_link_options(
			"/DEBUG"
			"/OPT:REF"
			"/OPT:ICF"
			"/INCREMENTAL:NO"
			# Ignore warnings about PDBs not being found (happens e.g. for vc140 as vcpkg's portfile doesn't install the PDB files)
			"/ignore:4099"
		)
	endif()
elseif(UNIX OR MINGW)
	add_compile_options(
		"-fvisibility=hidden"
	)

	if(optimize)
		add_compile_options(
			"-march=native"
		)
	endif()

	if(APPLE)
		add_link_options("-Wl,-dead_strip")

		if(symbols)
			add_compile_options(
				"-gfull"
				"-gdwarf-2"
			)
		endif()
	else()
		if(NOT MINGW)
			add_link_options(
				"-Wl,-z,relro"
				"-Wl,-z,now"
			)
		endif()

		# Ensure _FORTIFY_SOURCE is not used in debug builds.
		#
		# First, ensure _FORTIFY_SOURCE is undefined.
		# Then -- and, only for release builds -- set _FORTIFY_SOURCE=2.
		#
		# We can't use _FORTIFY_SOURCE in debug builds (which are built with -O0) because _FORTIFY_SOURCE=1 requires -O1 and _FORTIFY_SOURCE=2 requires -O2.
		# Modern GLIBCs warn about this since https://sourceware.org/bugzilla/show_bug.cgi?id=13979.
		# In Mumble builds with warnings-as-errors, this will cause build failures.
		add_compile_options("-U_FORTIFY_SOURCE")

		if(NOT MINGW)
			add_compile_options($<$<CONFIG:Debug>:-fstack-protector>)
		endif()

		add_compile_options(
			$<$<NOT:$<CONFIG:Debug>>:-D_FORTIFY_SOURCE=2>
		)

		add_link_options(
			$<$<CONFIG:Debug>:-Wl,--no-add-needed>
		)

		if(symbols)
			add_compile_options("-g")
		endif()
	endif()
endif()

function(target_disable_warnings TARGET)
	get_compiler_flags(
		DISABLE_ALL_WARNINGS
		DISABLE_DEFAULT_FLAGS
		OUTPUT_VARIABLE NO_WARNING_FLAGS
	)

	target_compile_options(${TARGET} PRIVATE ${NO_WARNING_FLAGS})
endfunction()
