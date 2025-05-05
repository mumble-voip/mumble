# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(AudioReceiverBuffer_benchmark "AudioReceiverBuffer_benchmark.cpp")

target_link_libraries(AudioReceiverBuffer_benchmark PRIVATE shared)

target_link_libraries(AudioReceiverBuffer_benchmark PRIVATE benchmark::benchmark)

target_include_directories(AudioReceiverBuffer_benchmark PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")


# In order to be able to mock the ServerUser class, we have to extract the server-specific source and header
# files into an isolated environment, such that they don't include/link with the remaining server files.
set(CUSTOM_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")
file(MAKE_DIRECTORY "${CUSTOM_INCLUDE_DIR}")
set(HEADER_TO_COPY "${CMAKE_SOURCE_DIR}/src/murmur/AudioReceiverBuffer.h")
set(SOURCE_TO_COPY "${CMAKE_SOURCE_DIR}/src/murmur/AudioReceiverBuffer.cpp")
get_filename_component(HEADER_NAME "${HEADER_TO_COPY}" NAME)
get_filename_component(SOURCE_NAME "${SOURCE_TO_COPY}" NAME)
set(COPIED_HEADER "${CUSTOM_INCLUDE_DIR}/${HEADER_NAME}")
set(COPIED_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_NAME}")

add_custom_command(OUTPUT "${COPIED_SOURCE}"
	COMMAND ${CMAKE_COMMAND} -E copy "${HEADER_TO_COPY}" "${COPIED_HEADER}"
	COMMAND ${CMAKE_COMMAND} -E copy "${SOURCE_TO_COPY}" "${COPIED_SOURCE}"
	DEPENDS "${HEADER_TO_COPY}" "${SOURCE_TO_COPY}"
)

target_sources(AudioReceiverBuffer_benchmark PRIVATE "${COPIED_SOURCE}")

target_include_directories(AudioReceiverBuffer_benchmark PRIVATE "${CUSTOM_INCLUDE_DIR}")
