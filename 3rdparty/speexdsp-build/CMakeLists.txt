set(SPEEXDSP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../speexdsp")

set(SPEEXDSP_SRC_DIR "${SPEEXDSP_DIR}/libspeexdsp")

if(NOT EXISTS "${SPEEXDSP_DIR}/COPYING")
	message(FATAL_ERROR
		"${SPEEX_DIR} or ${SPEEXDSP_DIR} was not found. You need to do one of the following:\n"
		"Option 1: Checkout the submodule:\n"
		"git submodule update --init --recursive\n"
		"Option 2: Use system Speex and SpeexDSP (v1.2 or later):\n"
		"cmake .. -Dbundled-speexdsp=OFF"
	)
endif()

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

# Speex doesn't use include guards in its headers, so unity builds will fail
set_target_properties(speexdsp PROPERTIES UNITY_BUILD FALSE)

target_compile_definitions(speexdsp PRIVATE "HAVE_CONFIG_H")

target_include_directories(speexdsp PRIVATE SYSTEM "${SPEEX_DIR}/libspeex" "${SPEEXDSP_DIR}/libspeexdsp")
target_include_directories(speexdsp PUBLIC SYSTEM "${SPEEX_DIR}/include" "${SPEEXDSP_DIR}/include")

if(MSVC)
	# We include "win32" only for MSVC because MinGW uses the "config.h" for Unix-like systems.
	target_include_directories(speexdsp PRIVATE SYSTEM BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/win32")
else()
	target_include_directories(speexdsp PUBLIC SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(WIN32)
	target_compile_definitions(speexdsp PRIVATE "_USE_MATH_DEFINES" "USE_SMALLFT")
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

target_disable_warnings(speexdsp)
