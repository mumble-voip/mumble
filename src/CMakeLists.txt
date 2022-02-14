# Copyright 2020-2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Mumble.proto")

option(client "Build the client (Mumble)" ON)
option(server "Build the server (Murmur)" ON)

option(qssldiffiehellmanparameters "Build support for custom Diffie-Hellman parameters." ON)

option(zeroconf "Build support for zeroconf (mDNS/DNS-SD)." ON)

option(dbus "Build support for DBus." ON)

option(tracy "Enable the tracy profiler." OFF)

find_pkg(Qt5
	COMPONENTS
		Core
		Network
		Xml
	REQUIRED
)

find_pkg(OpenSSL
	COMPONENTS
		Crypto
		SSL
	REQUIRED
)

find_pkg(Protobuf REQUIRED)

add_library(shared STATIC)

set_property(TARGET shared PROPERTY INTERPROCEDURAL_OPTIMIZATION ${lto})

protobuf_generate(LANGUAGE cpp TARGET shared PROTOS ${PROTO_FILE} OUT_VAR BUILT_PROTO_FILES)

# Disable warnings for the generated source files
foreach(CURRENT_FILE IN LISTS BUILT_PROTO_FILES)
	set_source_files_properties("${CURRENT_FILE}" PROPERTIES COMPILE_FLAGS "-w")
endforeach()

set_target_properties(shared
	PROPERTIES
		AUTOMOC ON
		# We explicitly tell CMake not to call any autogen tools (e.g. MOC) for the generated files.
		# @ref https://cmake.org/cmake/help/latest/policy/CMP0071.html
		SKIP_AUTOGEN ON
)

set(SHARED_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SHARED_SOURCES
	"Ban.cpp"
	"EnvUtils.cpp"
	"FFDHE.cpp"
	"HostAddress.cpp"
	"HTMLFilter.cpp"
	"License.cpp"
	"LogEmitter.cpp"
	"OSInfo.cpp"
	"PasswordGenerator.cpp"
	"PlatformCheck.cpp"
	"QtUtils.cpp"
	"ProcessResolver.cpp"
	"SelfSignedCertificate.cpp"
	"ServerAddress.cpp"
	"ServerResolver.cpp"
	"ServerResolverRecord.cpp"
	"SSL.cpp"
	"SSLLocks.cpp"
	"Timer.cpp"
	"UnresolvedServerAddress.cpp"
	"Version.cpp"

	"crypto/CryptographicHash.cpp"
	"crypto/CryptographicRandom.cpp"
	"crypto/CryptStateOCB2.cpp"

	"${3RDPARTY_DIR}/arc4random/arc4random_uniform.cpp"
)

set(SHARED_HEADERS
	"Ban.h"
	"ByteSwap.h"
	"EnvUtils.h"
	"FFDHE.h"
	"FFDHETable.h"
	"HostAddress.h"
	"HTMLFilter.h"
	"License.h"
	"licenses.h"
	"LogEmitter.h"
	"Net.h"
	"OSInfo.h"
	"PasswordGenerator.h"
	"PlatformCheck.h"
	"ProcessResolver.h"
	"SelfSignedCertificate.h"
	"ServerAddress.h"
	"ServerResolver.h"
	"ServerResolverRecord.h"
	"SSL.h"
	"SSLLocks.h"
	"Timer.h"
	"UnresolvedServerAddress.h"
	"Version.h"

	"crypto/CryptographicHash.h"
	"crypto/CryptographicRandom.h"
	"crypto/CryptState.h"
	"crypto/CryptStateOCB2.h"

	"${3RDPARTY_DIR}/arc4random/arc4random_uniform.h"
)

target_sources(shared
	PRIVATE
		${SHARED_HEADERS}
		${SHARED_SOURCES}
)

target_compile_definitions(shared
	PUBLIC
		"MUMBLE_VERSION=${PROJECT_VERSION}"
)

target_compile_definitions(shared
	PRIVATE
		"QT_RESTRICTED_CAST_FROM_ASCII"
)

target_include_directories(shared
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_BINARY_DIR}
	PRIVATE
		"${3RDPARTY_DIR}/arc4random"
)

target_link_libraries(shared
	PUBLIC
		protobuf::libprotobuf
		OpenSSL::Crypto
		OpenSSL::SSL
		Qt5::Core
		Qt5::Network
		Qt5::Xml
)

if(WIN32)
	target_link_libraries(shared PUBLIC qwave.lib)
elseif(APPLE)
	find_library(LIB_CORESERVICES "CoreServices")
	target_link_libraries(shared PUBLIC ${LIB_CORESERVICES})
endif()

if(qssldiffiehellmanparameters)
	target_compile_definitions(shared PUBLIC "USE_QSSLDIFFIEHELLMANPARAMETERS")
endif()

# Note: We always include and link against Tracy but it is only enabled, if we set the TRACY_ENABLE cmake option
# to ON, before including the respective subdirectory
set(TRACY_ENABLE ${tracy} CACHE BOOL "" FORCE)
set(TRACY_ON_DEMAND ON CACHE BOOL "" FORCE)
add_subdirectory("${3RDPARTY_DIR}/tracy" "tracy")
disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/tracy")
message(STATUS "Tracy: ${TRACY_ENABLE}")

target_link_libraries(shared PUBLIC Tracy::TracyClient)

if(client)
	add_subdirectory(mumble)

	configure_file("${CMAKE_SOURCE_DIR}/scripts/info.mumble.Mumble.appdata.xml.in" "${CMAKE_BINARY_DIR}/info.mumble.Mumble.appdata.xml")

	if(UNIX AND NOT APPLE)
		install(FILES "${CMAKE_BINARY_DIR}/info.mumble.Mumble.appdata.xml" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/metainfo")
		install(FILES "${CMAKE_SOURCE_DIR}/scripts/info.mumble.Mumble.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
		install(FILES "${CMAKE_SOURCE_DIR}/icons/mumble.svg" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps")
		install(FILES "${CMAKE_SOURCE_DIR}/icons/mumble_256x256.png" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps" RENAME "mumble.png")
	endif()
endif()

if(server)
	add_subdirectory(murmur)
endif()

if(tests)
	add_subdirectory(tests)
endif()
