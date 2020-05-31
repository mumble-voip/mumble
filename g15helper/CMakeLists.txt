# Copyright 2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

option(g15-emulator "Build the g15helper executable in emulator mode. This will cause an emulated G15 window to appear on screen. Allows the use of Mumble's G15 support without owning the physical hardware." OFF)

set(G15HELPER_RC "g15helper.rc")
set(G15HELPER_ICON "${CMAKE_SOURCE_DIR}/icons/g15helper.ico")

get_target_property(MUMBLE_SOURCE_DIR mumble SOURCE_DIR)

configure_file(${G15HELPER_RC} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(g15-helper WIN32
	"${MUMBLE_SOURCE_DIR}/mumble.appcompat.manifest"
	"${CMAKE_CURRENT_BINARY_DIR}/${G15HELPER_RC}"
)

set_target_properties(g15-helper
	PROPERTIES
		OUTPUT_NAME "mumble-g15-helper"
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

if($<CONFIG:Debug>)
	target_compile_definitions(g15-helper PRIVATE "USE_LOGFILE")
endif()

if(g15-emulator)
	find_pkg(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
	set_target_properties(g15-helper PROPERTIES AUTOMOC ON)
	target_sources(g15-helper PRIVATE "g15helper_emu.cpp" "g15helper_emu.h")
	target_link_libraries(g15-helper PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

	if(static AND WIN32 AND TARGET Qt5::QWindowsIntegrationPlugin)
		include_qt_plugin(g15-helper PRIVATE QWindowsIntegrationPlugin)
		target_link_libraries(g15-helper PRIVATE Qt5::QWindowsIntegrationPlugin)
	endif()
else()
	if(MSVC)
		set_target_properties(g15-helper PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
	endif()

	target_sources(g15-helper PRIVATE "g15helper.c" "g15helper.h")

	if(NOT G15_DIR)
		set(G15_DIR "${3RDPARTY_DIR}/g15")
	endif()

	if(64_BIT)
		find_library(LIB_LGLCD "lglcd" HINTS "${G15_DIR}/Lib/x64")
	else()
		find_library(LIB_LGLCD "lglcd" HINTS "${G15_DIR}/Lib/x86")
	endif()

	target_include_directories(g15-helper PRIVATE "${G15_DIR}/Src")

	target_link_libraries(g15-helper PRIVATE ${LIB_LGLCD})
endif()
