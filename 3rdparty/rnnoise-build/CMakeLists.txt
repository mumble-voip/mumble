# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(RNNOISE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../rnnoise-src")

if(NOT EXISTS "${RNNOISE_SRC_DIR}/COPYING")
	message(FATAL_ERROR
		"${RNNOISE_SRC_DIR} was not found.\n"
		"Please checkout the submodule:\n"
		"git submodule update --init --recursive"
	)
endif()

if(WIN32)
	add_library(rnnoise SHARED)
	set_target_properties(rnnoise PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	if(MINGW)
		# Remove "lib" prefix.
		set_target_properties(rnnoise PROPERTIES PREFIX "")
	endif()
	target_compile_definitions(rnnoise
		PRIVATE
			"WIN32"
			"DLL_EXPORT"
	)
else()
	add_library(rnnoise STATIC)
endif()

target_compile_definitions(rnnoise PRIVATE "HAVE_CONFIG_H")

target_include_directories(rnnoise
	PRIVATE SYSTEM
		${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC SYSTEM
		"${RNNOISE_SRC_DIR}/include"
)

if(MSVC)
	# Use malloc() and free() instead of variable length arrays (unsupported)
	target_compile_definitions(rnnoise PRIVATE "USE_MALLOC")
	# Define M_PI
	target_compile_definitions(rnnoise PRIVATE "_USE_MATH_DEFINES")
endif()

target_sources(rnnoise PRIVATE
	"${RNNOISE_SRC_DIR}/src/rnn_data.c"
	"${RNNOISE_SRC_DIR}/src/rnn.c"
	"${RNNOISE_SRC_DIR}/src/pitch.c"
	"${RNNOISE_SRC_DIR}/src/kiss_fft.c"
	"${RNNOISE_SRC_DIR}/src/denoise.c"
	"${RNNOISE_SRC_DIR}/src/celt_lpc.c"
)

target_disable_warnings(rnnoise)
