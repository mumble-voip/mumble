# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(MUMBLE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/mumble")
set(3RDPARTY_DIR "${CMAKE_SOURCE_DIR}/3rdparty")

set(TESTHRTF_SOURCES
	TestHrtfSpatializer.cpp
	"${MUMBLE_SOURCE_DIR}/HrtfSpatializer.cpp"
	"${MUMBLE_SOURCE_DIR}/HrtfSpatializer.h"
	"${3RDPARTY_DIR}/fftconvolver/FFTConvolver.cpp"
	"${3RDPARTY_DIR}/fftconvolver/AudioFFT.cpp"
	"${3RDPARTY_DIR}/fftconvolver/Utilities.cpp"
)

add_executable(TestHrtfSpatializer ${TESTHRTF_SOURCES})

set_target_properties(TestHrtfSpatializer PROPERTIES AUTOMOC ON)

target_include_directories(TestHrtfSpatializer PRIVATE
	${MUMBLE_SOURCE_DIR}
	"${3RDPARTY_DIR}/fftconvolver"
)

# Find libmysofa — try pkg-config first, fall back to find_library (mirrors main build)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
	pkg_check_modules(MYSOFA QUIET libmysofa)
endif()
if(MYSOFA_FOUND)
	target_include_directories(TestHrtfSpatializer PRIVATE ${MYSOFA_INCLUDE_DIRS})
	target_link_libraries(TestHrtfSpatializer PRIVATE ${MYSOFA_LIBRARIES})
else()
	find_library(MYSOFA_LIB mysofa REQUIRED)
	find_path(MYSOFA_INCLUDE_DIR mysofa.h REQUIRED)
	target_include_directories(TestHrtfSpatializer PRIVATE ${MYSOFA_INCLUDE_DIR})
	target_link_libraries(TestHrtfSpatializer PRIVATE ${MYSOFA_LIB})
endif()

target_compile_definitions(TestHrtfSpatializer PRIVATE
	"USE_HRTF"
	"MUMBLE_HRTF_DEFAULT_SOFA=\"${CMAKE_SOURCE_DIR}/data/hrtf/default.sofa\""
)

target_link_libraries(TestHrtfSpatializer PRIVATE Qt6::Test)

add_test(NAME TestHrtfSpatializer COMMAND $<TARGET_FILE:TestHrtfSpatializer>)
