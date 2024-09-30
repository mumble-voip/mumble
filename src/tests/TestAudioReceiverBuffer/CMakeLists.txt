# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestAudioReceiverBuffer TestAudioReceiverBuffer.cpp)

set_target_properties(TestAudioReceiverBuffer PROPERTIES AUTOMOC ON)

target_link_libraries(TestAudioReceiverBuffer PRIVATE shared Qt6::Test)

target_include_directories(TestAudioReceiverBuffer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

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

add_custom_command(
	OUTPUT "${COPIED_SOURCE}"
	COMMAND ${CMAKE_COMMAND} -E copy "${HEADER_TO_COPY}" "${COPIED_HEADER}"
	COMMAND ${CMAKE_COMMAND} -E copy "${SOURCE_TO_COPY}" "${COPIED_SOURCE}"
	DEPENDS "${HEADER_TO_COPY}" "${SOURCE_TO_COPY}"
	COMMENT "Copying necessary source files"
)

target_sources(TestAudioReceiverBuffer PRIVATE "${COPIED_SOURCE}")

target_include_directories(TestAudioReceiverBuffer PRIVATE "${CUSTOM_INCLUDE_DIR}")

add_test(NAME TestAudioReceiverBuffer COMMAND $<TARGET_FILE:TestAudioReceiverBuffer>)
