# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(MURMUR_RC "${CMAKE_CURRENT_BINARY_DIR}/murmur.rc")
set(MURMUR_ICON "${CMAKE_SOURCE_DIR}/icons/murmur.ico")
set(MURMUR_PLIST "${CMAKE_CURRENT_BINARY_DIR}/murmur.plist")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/murmur.plist.in" "${MURMUR_PLIST}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/murmur.rc.in" "${MURMUR_RC}")

set(ICE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/MumbleServer.ice")

include(qt-utils)

option(ice "Build support for Ice RPC." ON)

find_pkg(Qt6 COMPONENTS Sql REQUIRED)

set(MURMUR_SOURCES
	"main.cpp"
	"AudioReceiverBuffer.cpp"
	"AudioReceiverBuffer.h"
	"Cert.cpp"
	"Messages.cpp"
	"Meta.cpp"
	"Meta.h"
	"PBKDF2.cpp"
	"PBKDF2.h"
	"Register.cpp"
	"RPC.cpp"
	"Server.cpp"
	"Server.h"
	"ServerDB.cpp"
	"ServerDB.h"
	"ServerUser.cpp"
	"ServerUser.h"

	"${SHARED_SOURCE_DIR}/ACL.cpp"
	"${SHARED_SOURCE_DIR}/ACL.h"
	"${SHARED_SOURCE_DIR}/Channel.cpp"
	"${SHARED_SOURCE_DIR}/Channel.h"
	"${SHARED_SOURCE_DIR}/ChannelListenerManager.cpp"
	"${SHARED_SOURCE_DIR}/ChannelListenerManager.h"
	"${SHARED_SOURCE_DIR}/Connection.cpp"
	"${SHARED_SOURCE_DIR}/Connection.h"
	"${SHARED_SOURCE_DIR}/Group.cpp"
	"${SHARED_SOURCE_DIR}/Group.h"
	"${SHARED_SOURCE_DIR}/User.cpp"
	"${SHARED_SOURCE_DIR}/User.h"
)

if(WIN32)
	add_executable(mumble-server WIN32 ${MURMUR_SOURCES})
else()
	add_executable(mumble-server ${MURMUR_SOURCES})
endif()

set_target_properties(mumble-server
	PROPERTIES
		AUTOMOC ON
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set(AUTOGEN_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/murmur_autogen")
message(STATUS "File: ${AUTOGEN_BUILD_DIR}")
set_source_files_properties("${AUTOGEN_BUILD_DIR}/mocs_compilation.cpp" PROPERTIES COMPILE_FLAGS "-w")

target_compile_definitions(mumble-server
	PRIVATE
		"MURMUR"
		"QT_RESTRICTED_CAST_FROM_ASCII"
)

target_include_directories(mumble-server PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR} # This is required for includes in current folder to be found by files from the shared directory.
	${SHARED_SOURCE_DIR}
)

target_link_libraries(mumble-server PRIVATE shared Qt6::Sql)

if(static)
	# MariaDB and MySQL
	if(TARGET Qt6::QMYSQLDriverPlugin)
		include_qt_plugin(mumble-server PRIVATE "QMYSQLDriverPlugin")
		target_link_libraries(mumble-server PRIVATE Qt6::QMYSQLDriverPlugin)
	endif()
	# Open DataBase Connectivity
	if(TARGET Qt6::QODBCDriverPlugin)
		include_qt_plugin(mumble-server PRIVATE "QODBCDriverPlugin")
		target_link_libraries(mumble-server PRIVATE Qt6::QODBCDriverPlugin)
		if(WIN32)
			find_library(LIB_ODBC32 "odbc32")
			target_link_libraries(mumble-server PRIVATE ${LIB_ODBC32})
		endif()
	endif()
	# PostgreSQL
	if(TARGET Qt6::QPSQLDriverPlugin)
		include_qt_plugin(mumble-server PRIVATE "QPSQLDriverPlugin")
		target_link_libraries(mumble-server PRIVATE Qt6::QPSQLDriverPlugin)
	endif()
	# SQLite
	if(TARGET Qt6::QSQLiteDriverPlugin)
		include_qt_plugin(mumble-server PRIVATE "QSQLiteDriverPlugin")
		target_link_libraries(mumble-server PRIVATE Qt6::QSQLiteDriverPlugin)
	endif()
endif()

if(WIN32)
	target_sources(mumble-server
		PRIVATE
			"About.cpp"
			"About.h"
			"Tray.cpp"
			"Tray.h"
			"murmur.qrc"

			"${CMAKE_SOURCE_DIR}/auxiliary_files/mumble.appcompat.manifest"
			"${MURMUR_RC}"
	)

	set_target_properties(mumble-server
		PROPERTIES
			AUTORCC ON
	)

	find_pkg(Qt6 COMPONENTS Widgets REQUIRED)

	target_link_libraries(mumble-server 
		PRIVATE 
			Qt6::Widgets
			dbghelp.lib
			shlwapi.lib
			rpcrt4.lib
	)

	if(static AND TARGET Qt6::QWindowsIntegrationPlugin)
		include_qt_plugin(mumble-server PRIVATE QWindowsIntegrationPlugin)
		target_link_libraries(mumble-server PRIVATE Qt6::QWindowsIntegrationPlugin)
	endif()
else()
	target_sources(mumble-server
		PRIVATE
			"UnixMurmur.cpp"
			"UnixMurmur.h"
	)

	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		find_library(CAP_LIBRARY NAMES cap)
		target_link_libraries(mumble-server PRIVATE ${CAP_LIBRARY})
	endif()
endif()

if(zeroconf)
	if(NOT APPLE)
		find_pkg(avahi-compat-libdns_sd QUIET)
		if(avahi-compat-libdns_sd_FOUND)
			target_include_directories(mumble-server PRIVATE ${avahi-compat-libdns_sd_INCLUDE_DIRS})
			target_link_libraries(mumble-server PRIVATE ${avahi-compat-libdns_sd_LIBRARIES})
		else()
			find_library(LIB_DNSSD "dnssd")
			if(${LIB_DNSSD} STREQUAL "LIB_DNSSD-NOTFOUND")
				message(FATAL_ERROR "DNS-SD library not found!")
			endif()
			target_link_libraries(mumble-server PRIVATE ${LIB_DNSSD})
		endif()
	endif()

	target_compile_definitions(mumble-server PRIVATE "USE_ZEROCONF")

	target_include_directories(mumble-server PRIVATE "${3RDPARTY_DIR}/qqbonjour")

	target_sources(mumble-server
		PRIVATE
			"Zeroconf.cpp"
			"Zeroconf.h"
			# Unlike what the name implies, this 3rdparty helper is not actually related to Bonjour.
			# It just uses the API provided by mDNSResponder, making it compatible with Avahi too.
			"${3RDPARTY_DIR}/qqbonjour/BonjourRecord.h"
			"${3RDPARTY_DIR}/qqbonjour/BonjourServiceRegister.cpp"
			"${3RDPARTY_DIR}/qqbonjour/BonjourServiceRegister.h"
	)
endif()

if(ice)
	find_pkg(Ice
		COMPONENTS
			Ice
			IceSSL
		OPTIONAL_COMPONENTS
			IceDiscovery
			IceLocatorDiscovery
			IceUtil
		REQUIRED
	)

	if(Ice_VERSION_MAJOR LESS 3)
		message(FATAL_ERROR "Unsupported Ice version, at least 3.0 is required.")
	endif()

	get_filename_component(ICE_FILE_NAME ${ICE_FILE} NAME_WE)

	list(APPEND ICE_GENERATED_FILES
		"${ICE_FILE_NAME}.cpp"
		"${ICE_FILE_NAME}.h"
	)

	foreach(SLICE_DIR ${Ice_SLICE_DIRS})
		list(APPEND SLICE_INCLUDE_ARGS "-I${SLICE_DIR}")
	endforeach()

	add_custom_command(
		OUTPUT ${ICE_GENERATED_FILES}
		COMMAND ${Ice_SLICE2CPP_EXECUTABLE}
		ARGS "--checksum" ${SLICE_INCLUDE_ARGS} ${ICE_FILE}
		MAIN_DEPENDENCY ${ICE_FILE}
		COMMENT "Generating Ice files"
		VERBATIM
	)

	add_custom_target(generate_murmur_ice_wrapper
		DEPENDS "MumbleServerIceWrapper.cpp" 
	)
	add_dependencies(mumble-server generate_murmur_ice_wrapper)
	add_custom_command(
		OUTPUT "MumbleServerIceWrapper.cpp"
		COMMAND ${PYTHON_INTERPRETER}
		ARGS "${CMAKE_SOURCE_DIR}/scripts/generateIceWrapper.py" -i "${ICE_FILE}"
			-g "${ICE_FILE_NAME}.h" -o "MumbleServerIceWrapper.cpp" -q
		DEPENDS ${ICE_GENERATED_FILES}
		COMMENT "Generating MumbleServerIceWrapper"
	)
	# Even though the file is a .cpp, it is still being included in MumbleServerIce.cpp
	target_include_directories(mumble-server PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

	# We explicitly tell CMake not to call any autogen tools (e.g. MOC) for the generated files.
	# @ref https://cmake.org/cmake/help/latest/policy/CMP0071.html
	set_property(SOURCE ${ICE_GENERATED_FILES} PROPERTY SKIP_AUTOGEN ON)

	target_sources(mumble-server
		PRIVATE
			"MumbleServerIce.cpp"
			"MumbleServerIce.h"
			${ICE_GENERATED_FILES}
	)
	target_compile_definitions(mumble-server
		PRIVATE
			"USE_ICE"
			"ICE_STATIC_LIBS"
	)

	target_include_directories(mumble-server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

	if(static)
		# BZip2 is used by Ice, specifically in ConnectionI.cpp.
		# We link to the library explicitly because CMake is not aware of the dependency.
		# TODO: Fix or rewrite the official FindIce.cmake.
		find_pkg(BZip2 REQUIRED)
	endif()

	target_link_libraries(mumble-server
		PRIVATE
			$<TARGET_NAME_IF_EXISTS:BZip2::BZip2>

			Ice::Ice
			Ice::IceSSL
			$<TARGET_NAME_IF_EXISTS:Ice::IceDiscovery>
			$<TARGET_NAME_IF_EXISTS:Ice::IceLocatorDiscovery>
			$<TARGET_NAME_IF_EXISTS:Ice::IceUtil>
	)
	
	file(COPY "${ICE_FILE}" DESTINATION ${CMAKE_BINARY_DIR})
endif()

install(TARGETS mumble-server RUNTIME DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_server)

if(packaging)
	if(WIN32)
		if(translations)
			list(APPEND installer_vars "--all-languages")
		endif()

		list(APPEND installer_vars
			"--version" ${PROJECT_VERSION}
			"--arch" ${MUMBLE_TARGET_ARCH}
		)

		file(COPY
			${CMAKE_SOURCE_DIR}/installer/MumbleInstall.cs
			${CMAKE_SOURCE_DIR}/installer/ServerInstaller.cs
			DESTINATION
				${CMAKE_BINARY_DIR}/installer/server
		)

		add_custom_command(TARGET mumble-server
			POST_BUILD
			COMMAND cscs.exe -cd MumbleInstall.cs 
			COMMAND cscs.exe ServerInstaller.cs ${installer_vars}
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/installer/server
		)
	endif()
endif()
