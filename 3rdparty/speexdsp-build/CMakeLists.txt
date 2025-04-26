# Copyright 2020-2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(SPEEXDSP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../speexdsp")

set(SPEEXDSP_SRC_DIR "${SPEEXDSP_DIR}/libspeexdsp")

if(NOT EXISTS "${SPEEXDSP_DIR}/COPYING")
	message(FATAL_ERROR
		"${SPEEX_DIR} or ${SPEEXDSP_DIR} was not found. You need to do one of the following:\n"
		"Option 1: Checkout the submodule:\n"
		"git submodule update --init --recursive\n"
		"Option 2: Use system SpeexDSP (v1.2 or later):\n"
		"cmake .. -Dbundled-speex=OFF"
	)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(try_compile_snippet)
include(CheckIncludeFile)

if(WIN32)
	add_library(speexdsp SHARED)
	set_target_properties(speexdsp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	if(MINGW)
		# Remove "lib" prefix.
		set_target_properties(speexdsp PROPERTIES PREFIX "")
	endif()
else()
	add_library(speexdsp STATIC)
endif()

CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H)

if(NOT HAVE_STDINT_H)
	message(FATAL_ERROR "Assumed header stdint.h to be available")
endif()

target_include_directories(speexdsp PRIVATE SYSTEM "${SPEEXDSP_DIR}/libspeexdsp")
target_include_directories(speexdsp PUBLIC SYSTEM "${SPEEXDSP_DIR}/include")

if(WIN32)
	target_compile_definitions(speexdsp PRIVATE "_USE_MATH_DEFINES")
	target_sources(speexdsp PRIVATE "mumble_speex_init.c")
endif()

set(SPEEXDSP_SOURCES
	"${SPEEXDSP_SRC_DIR}/fftwrap.c"
	"${SPEEXDSP_SRC_DIR}/filterbank.c"
	"${SPEEXDSP_SRC_DIR}/jitter.c"
	"${SPEEXDSP_SRC_DIR}/mdf.c"
	"${SPEEXDSP_SRC_DIR}/preprocess.c"
	"${SPEEXDSP_SRC_DIR}/resample.c"
	"${SPEEXDSP_SRC_DIR}/scal.c"
	"${SPEEXDSP_SRC_DIR}/smallft.c"
)

target_sources(speexdsp PRIVATE ${SPEEXDSP_SOURCES} "speexdsp.def")

##########################################################
####### Check whether given features are available #######
##########################################################

try_compile_snippet(
	RESULT_VAR VARIABLE_SIZE_ARRAYS_SUPPORTED
	LANG C
	STATEMENTS
		"int size = 4;"
		"int array[size];"
)

message(STATUS "speexdsp: Variable sized arrays supported: ${VARIABLE_SIZE_ARRAYS_SUPPORTED}")
if(VARIABLE_SIZE_ARRAYS_SUPPORTED)
	target_compile_definitions(speexdsp PRIVATE "VAR_ARRAYS")
endif()

try_compile_snippet(
	RESULT_VAR ALLOCA_AVAILABLE
	LANG C
	INCLUDES
		"#ifdef HAVE_ALLOCA_H\n#include <alloca.h>\n#endif"
	INCLUDE_HEADERS
		"stdlib.h"
	STATEMENTS
		"int size = 10;"
		"int *array = alloca(size);"
)

message(STATUS "speexdsp: alloca available: ${ALLOCA_AVAILABLE}")
if(ALLOCA_AVAILABLE AND NOT VARIABLE_SIZE_ARRAYS_SUPPORTED)
	# If available, variable-sized arrays should be preferred
	target_compile_definitions(speexdsp PRIVATE "USE_ALLOCA")
endif()

try_compile_snippet(
	RESULT_VAR SSE_AVAILABLE
	LANG C
	INCLUDE_HEADERS
		"xmmintrin.h"
	STATEMENTS
		"float a = 0.8;"
		"float b = 0.5;"
		"_mm_add_ps(_mm_loadu_ps(&a), _mm_loadu_ps(&b));"
)

message(STATUS "speexdsp: SSE available: ${SSE_AVAILABLE}")
if(SSE_AVAILABLE)
	target_compile_definitions(speexdsp PRIVATE "USE_SSE")
endif()

try_compile_snippet(
	RESULT_VAR SSE2_AVAILABLE
	LANG C
	INCLUDE_HEADERS
		"emmintrin.h"
	STATEMENTS
		"double a = 0.8;"
		"double b = 0.5;"
		"_mm_add_pd(_mm_loadu_pd(&a), _mm_loadu_pd(&b));"
)

message(STATUS "speexdsp: SSE2 available: ${SSE2_AVAILABLE}")
if(SSE2_AVAILABLE)
	target_compile_definitions(speexdsp PRIVATE "USE_SSE2")
endif()

# We simply assume that any machine this runs on has a floating point unit (FPU) available and
# thus we'll always use the floating point implementation, for which smallft is the default
# FFT implementation.
target_compile_definitions(speexdsp PRIVATE "FLOATING_POINT")
target_compile_definitions(speexdsp PRIVATE "USE_SMALLFT")

# Define empty EXPORT macro, as we don't care about messing with symbol visibility
target_compile_definitions(speexdsp PRIVATE EXPORT=)


# Generate a header with the necessary type definitions for fixed-width integers. We have
# checked before, that stdint.h is available, so we'll just default to using that.
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/include/speexdsp_config_types.h"
"#ifndef SPEEXDSP_CONFIGTYPES_H__\n\
#define SPEEXDSP_CONFIGTYPES_H__
#include <stdint.h>\n\
\n\
typedef int16_t spx_int16_t;\n\
typedef uint16_t spx_uint16_t;\n\
typedef int32_t spx_int32_t;\n\
typedef uint32_t spx_uint32_t;\n\
#endif\n")

target_include_directories(speexdsp PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/include")
