set(CELT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../celt-0.7.0-src")
set(LIBCELT_SRC_DIR "${CELT_SRC_DIR}/libcelt")

if(NOT EXISTS "${CELT_SRC_DIR}/COPYING")
	message(FATAL_ERROR
		"${CELT_SRC_DIR} was not found.\n"
		"Please checkout the submodule:\n"
		"git submodule update --init --recursive"
	)
endif()

add_library(celt SHARED)

set_target_properties(celt PROPERTIES VERSION "0.7.0")

target_compile_definitions(celt PRIVATE "HAVE_CONFIG_H")

target_include_directories(celt PUBLIC SYSTEM "${CELT_SRC_DIR}/libcelt")

if(MSVC)
	# We include "win32" only for MSVC because MinGW uses the "config.h" for Unix-like systems.
	target_include_directories(celt PRIVATE SYSTEM BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/win32")
else()
	target_include_directories(celt PRIVATE SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(WIN32)
	set_target_properties(celt
		PROPERTIES
			OUTPUT_NAME "celt0.0.7.0"
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
	if(MINGW)
		# Remove "lib" prefix.
		set_target_properties(celt PROPERTIES PREFIX "")
	endif()
else()
	set_target_properties(celt
		PROPERTIES
			OUTPUT_NAME "celt0"
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
endif()

target_sources(celt PRIVATE
	"${LIBCELT_SRC_DIR}/bands.c"
	"${LIBCELT_SRC_DIR}/celt.c"
	"${LIBCELT_SRC_DIR}/cwrs.c"
	"${LIBCELT_SRC_DIR}/entcode.c"
	"${LIBCELT_SRC_DIR}/entdec.c"
	"${LIBCELT_SRC_DIR}/entenc.c"
	"${LIBCELT_SRC_DIR}/header.c"
	"${LIBCELT_SRC_DIR}/kiss_fft.c"
	"${LIBCELT_SRC_DIR}/kiss_fftr.c"
	"${LIBCELT_SRC_DIR}/laplace.c"
	"${LIBCELT_SRC_DIR}/mdct.c"
	"${LIBCELT_SRC_DIR}/modes.c"
	"${LIBCELT_SRC_DIR}/pitch.c"
	"${LIBCELT_SRC_DIR}/psy.c"
	"${LIBCELT_SRC_DIR}/quant_bands.c"
	"${LIBCELT_SRC_DIR}/rangedec.c"
	"${LIBCELT_SRC_DIR}/rangeenc.c"
	"${LIBCELT_SRC_DIR}/rate.c"
	"${LIBCELT_SRC_DIR}/vq.c"
)

target_disable_warnings(celt)
