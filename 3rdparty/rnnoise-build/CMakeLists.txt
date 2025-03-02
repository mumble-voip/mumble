# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(FetchContent)

set(RNNOISE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../rnnoise-src")
set(RNNOISE_MODEL_URL "https://media.xiph.org/rnnoise/models/rnnoise_data-")

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
		"${RNNOISE_SRC_DIR}/src"
)

file(READ "${RNNOISE_SRC_DIR}/model_version" rnnoise_model_hash)
string(STRIP ${rnnoise_model_hash} rnnoise_model_hash)
string(CONCAT rnnoise_model_url "${RNNOISE_MODEL_URL}" "${rnnoise_model_hash}" ".tar.gz")

FetchContent_Declare(
  rnnoise_model
  URL      "${rnnoise_model_url}"
  URL_HASH SHA256=${rnnoise_model_hash}
  DOWNLOAD_EXTRACT_TIMESTAMP 1
)

FetchContent_MakeAvailable(rnnoise_model)

add_custom_command(
    OUTPUT
        "${RNNOISE_SRC_DIR}/src/rnnoise_data.c"
        "${RNNOISE_SRC_DIR}/src/rnnoise_data.h"
        "${RNNOISE_SRC_DIR}/src/rnnoise_data_little.c"
        "${RNNOISE_SRC_DIR}/src/rnnoise_data_little.h"

    WORKING_DIRECTORY "${RNNOISE_SRC_DIR}"
    COMMAND "${CMAKE_COMMAND}" -E copy
        "${rnnoise_model_SOURCE_DIR}/src/rnnoise_data.c"
        "${rnnoise_model_SOURCE_DIR}/src/rnnoise_data.h"
        "${rnnoise_model_SOURCE_DIR}/src/rnnoise_data_little.c"
        "${rnnoise_model_SOURCE_DIR}/src/rnnoise_data_little.h"
        "${RNNOISE_SRC_DIR}/src"
    COMMENT "Downloading RNNoise model files"
)

target_sources(rnnoise PRIVATE
	"${RNNOISE_SRC_DIR}/src/rnnoise_data.c"
	"${RNNOISE_SRC_DIR}/src/rnnoise_tables.c"
	"${RNNOISE_SRC_DIR}/src/rnn.c"
	"${RNNOISE_SRC_DIR}/src/pitch.c"
	"${RNNOISE_SRC_DIR}/src/nnet.c"
	"${RNNOISE_SRC_DIR}/src/nnet_default.c"
	"${RNNOISE_SRC_DIR}/src/parse_lpcnet_weights.c"
	"${RNNOISE_SRC_DIR}/src/kiss_fft.c"
	"${RNNOISE_SRC_DIR}/src/denoise.c"
	"${RNNOISE_SRC_DIR}/src/celt_lpc.c"
)

target_disable_warnings(rnnoise)
