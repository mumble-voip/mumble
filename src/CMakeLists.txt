# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Mumble.proto")

option(client "Build the client (Mumble)" ON)
option(server "Build the server (Murmur)" ON)

option(qssldiffiehellmanparameters "Build support for custom Diffie-Hellman parameters." ON)

option(zeroconf "Build support for zeroconf (mDNS/DNS-SD)." ON)

option(dbus "Build support for DBus." ON)

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
	"SSLCipherInfo.cpp"
	"SSLLocks.cpp"
	"Timer.cpp"
	"UnresolvedServerAddress.cpp"
	"User.cpp"
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
	"SSLCipherInfo.h"
	"SSLCipherInfoTable.h"
	"SSLLocks.h"
	"Timer.h"
	"UnresolvedServerAddress.h"
	"User.h"
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
		# Only define this macro if RELEASE_ID is actually set
		$<$<NOT:$<STREQUAL:"${RELEASE_ID}","">>:MUMBLE_RELEASE_ID=${RELEASE_ID}>
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

if(client)
	add_subdirectory(mumble)

	if(UNIX AND NOT APPLE)
		install(FILES "${CMAKE_SOURCE_DIR}/scripts/org.mumble_voip.mumble.appdata.xml" DESTINATION "share/metainfo")
		install(FILES "${CMAKE_SOURCE_DIR}/scripts/org.mumble_voip.mumble.desktop" DESTINATION "share/applications")
		install(FILES "${CMAKE_SOURCE_DIR}/icons/mumble.svg" DESTINATION "share/icons/hicolor/scalable/apps")
		install(FILES "${CMAKE_SOURCE_DIR}/icons/mumble_256x256.png" DESTINATION "share/icons/hicolor/256x256/apps" RENAME "mumble.png")
	endif()
endif()

if(server)
	add_subdirectory(murmur)
endif()

if(tests)
	add_subdirectory(tests)
endif()
