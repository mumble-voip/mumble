set(SPEEX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../speex-src")
set(SPEEXDSP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../speexdsp")

set(SPEEX_SRC_DIR "${SPEEX_DIR}/libspeex")
set(SPEEXDSP_SRC_DIR "${SPEEXDSP_DIR}/libspeexdsp")

if(NOT EXISTS "${SPEEX_DIR}/COPYING" OR NOT EXISTS "${SPEEXDSP_DIR}/COPYING")
	message(FATAL_ERROR
		"${SPEEX_DIR} or ${SPEEXDSP_DIR} was not found. You need to do one of the following:\n"
		"Option 1: Checkout the submodule:\n"
		"git submodule update --init --recursive\n"
		"Option 2: Use system Speex and SpeexDSP (v1.2 or later):\n"
		"cmake .. -Dbundled-speex=OFF"
	)
endif()

if(WIN32)
	add_library(speex SHARED)
	set_target_properties(speex PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	if(MINGW)
		# Remove "lib" prefix.
		set_target_properties(speex PROPERTIES PREFIX "")
	endif()
else()
	add_library(speex STATIC)
endif()

# Speex doesn't use include guards in its headers, so unity builds will fail
set_target_properties(speex PROPERTIES UNITY_BUILD FALSE)

target_compile_definitions(speex PRIVATE "HAVE_CONFIG_H")

target_include_directories(speex PRIVATE SYSTEM "${SPEEX_DIR}/libspeex" "${SPEEXDSP_DIR}/libspeexdsp")
target_include_directories(speex PUBLIC SYSTEM "${SPEEX_DIR}/include" "${SPEEXDSP_DIR}/include")

if(MSVC)
	# We include "win32" only for MSVC because MinGW uses the "config.h" for Unix-like systems.
	target_include_directories(speex PRIVATE SYSTEM BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/win32")
else()
	target_include_directories(speex PUBLIC SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(WIN32)
	target_compile_definitions(speex PRIVATE "_USE_MATH_DEFINES" "USE_SMALLFT")
	target_sources(speex PRIVATE "mumble_speex_init.c")
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

set(SPEEX_SOURCES
	"${SPEEX_SRC_DIR}/bits.c"
	"${SPEEX_SRC_DIR}/cb_search.c"
	"${SPEEX_SRC_DIR}/exc_5_64_table.c"
	"${SPEEX_SRC_DIR}/exc_5_256_table.c"
	"${SPEEX_SRC_DIR}/exc_8_128_table.c"
	"${SPEEX_SRC_DIR}/exc_10_16_table.c"
	"${SPEEX_SRC_DIR}/exc_10_32_table.c"
	"${SPEEX_SRC_DIR}/exc_20_32_table.c"
	"${SPEEX_SRC_DIR}/filters.c"
	"${SPEEX_SRC_DIR}/gain_table.c"
	"${SPEEX_SRC_DIR}/gain_table_lbr.c"
	"${SPEEX_SRC_DIR}/hexc_10_32_table.c"
	"${SPEEX_SRC_DIR}/hexc_table.c"
	"${SPEEX_SRC_DIR}/high_lsp_tables.c"
	"${SPEEX_SRC_DIR}/kiss_fft.c"
	"${SPEEX_SRC_DIR}/kiss_fftr.c"
	"${SPEEX_SRC_DIR}/lpc.c"
	"${SPEEX_SRC_DIR}/lsp.c"
	"${SPEEX_SRC_DIR}/lsp_tables_nb.c"
	"${SPEEX_SRC_DIR}/ltp.c"
	"${SPEEX_SRC_DIR}/modes.c"
	"${SPEEX_SRC_DIR}/modes_wb.c"
	"${SPEEX_SRC_DIR}/nb_celp.c"
	"${SPEEX_SRC_DIR}/quant_lsp.c"
	"${SPEEX_SRC_DIR}/sb_celp.c"
	"${SPEEX_SRC_DIR}/speex.c"
	"${SPEEX_SRC_DIR}/speex_callbacks.c"
	"${SPEEX_SRC_DIR}/speex_header.c"
	"${SPEEX_SRC_DIR}/stereo.c"
	"${SPEEX_SRC_DIR}/vbr.c"
	"${SPEEX_SRC_DIR}/vorbis_psy.c"
	"${SPEEX_SRC_DIR}/vq.c"
	"${SPEEX_SRC_DIR}/window.c"
)

target_sources(speex PRIVATE ${SPEEXDSP_SOURCES} ${SPEEX_SOURCES} "speex.def")

target_disable_warnings(speex)
