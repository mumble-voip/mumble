# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

set(PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Mumble.proto")
set(UDP_PROTO_FILE "${CMAKE_CURRENT_SOURCE_DIR}/MumbleUDP.proto")

option(benchmarks "Build benchmarks" OFF)

option(qssldiffiehellmanparameters "Build support for custom Diffie-Hellman parameters." ON)

option(zeroconf "Build support for zeroconf (mDNS/DNS-SD)." ON)

option(tracy "Enable the tracy profiler." OFF)

option(bundled-gsl "Use the bundled GSL version instead of looking for one on the system" ON)
option(bundled-spdlog "Use the bundled spdlog version instead of looking for one on the system" ON)

find_pkg(Qt6
	VERSION 6.2
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

add_compile_options(${MUMBLE_COMPILER_FLAGS})

add_library(shared STATIC)

protobuf_generate(LANGUAGE cpp TARGET shared PROTOS ${PROTO_FILE} OUT_VAR BUILT_PROTO_FILES)
protobuf_generate(LANGUAGE cpp TARGET shared PROTOS ${UDP_PROTO_FILE} OUT_VAR BUILT_UDP_PROTO_FILES)

# Disable warnings for the generated source files
foreach(CURRENT_FILE IN LISTS BUILT_PROTO_FILES BUILT_UDP_PROTO_FILES)
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
	"MumbleProtocol.cpp"
	"OSInfo.cpp"
	"PasswordGenerator.cpp"
	"PlatformCheck.cpp"
	"QtUtils.cpp"
	"ProcessResolver.cpp"
	"ProtoUtils.cpp"
	"SelfSignedCertificate.cpp"
	"ServerAddress.cpp"
	"ServerResolver.cpp"
	"ServerResolverRecord.cpp"
	"SSL.cpp"
	"SSLLocks.cpp"
	"Timer.cpp"
	"UnresolvedServerAddress.cpp"
	"Version.cpp"
	"VolumeAdjustment.cpp"

	"crypto/CryptographicHash.cpp"
	"crypto/CryptographicRandom.cpp"
	"crypto/CryptState.cpp"
	"crypto/CryptStateOCB2.cpp"

	"${3RDPARTY_DIR}/arc4random/arc4random_uniform.cpp"
)

set(SHARED_HEADERS
	"Ban.h"
	"ByteSwap.h"
	"EnvUtils.h"
	"FFDHE.h"
	"HostAddress.h"
	"HTMLFilter.h"
	"License.h"
	"LogEmitter.h"
	"MumbleProtocol.h"
	"Net.h"
	"OSInfo.h"
	"PasswordGenerator.h"
	"PlatformCheck.h"
	"ProcessResolver.h"
	"ProtoUtils.h"
	"SelfSignedCertificate.h"
	"ServerAddress.h"
	"ServerResolver.h"
	"ServerResolverRecord.h"
	"SSL.h"
	"SSLLocks.h"
	"Timer.h"
	"UnresolvedServerAddress.h"
	"Version.h"
	"VolumeAdjustment.h"

	"crypto/CryptographicHash.h"
	"crypto/CryptographicRandom.h"
	"crypto/CryptState.h"
	"crypto/CryptStateOCB2.h"

	"${3RDPARTY_DIR}/arc4random/arc4random_uniform.h"
)

add_custom_command(
	OUTPUT "FFDHETable.h"
	COMMAND ${PYTHON_INTERPRETER}
	ARGS ${CMAKE_SOURCE_DIR}/scripts/generate-ffdhe.py --output FFDHETable.h
	COMMENT "Generating FFDHETable.h"
)
list(APPEND SHARED_HEADERS "FFDHETable.h")

add_custom_command(
	OUTPUT "licenses.h"
	COMMAND ${PYTHON_INTERPRETER}
	ARGS "${CMAKE_SOURCE_DIR}/scripts/generate_license_header.py" --output "licenses.h"
	COMMENT "Generating licenses.h"
)
add_custom_target(generate_license_header
	DEPENDS "licenses.h"
)
add_dependencies(shared generate_license_header)
target_include_directories(shared PRIVATE "${CMAKE_BINARY_DIR}")

target_sources(shared
	PRIVATE
		${SHARED_HEADERS}
		${SHARED_SOURCES}
)

target_compile_definitions(shared
	PUBLIC
		"MUMBLE_VERSION=${CMAKE_PROJECT_VERSION}"
		"MUMBLE_VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR}"
		"MUMBLE_VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR}"
		"MUMBLE_VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH}"
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
		Qt6::Core
		Qt6::Network
		Qt6::Xml
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
# We force to build Tracy as a static library in order to not create a dependency on the respective .so file
# (can cause issues for packagers as that dependency is retained even though Tracy is disabled)
set(PREV_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory("${3RDPARTY_DIR}/tracy" "tracy" EXCLUDE_FROM_ALL)
disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/tracy")
message(STATUS "Tracy: ${TRACY_ENABLE}")

# Restore whatever BUILD_SHARED_LIBS was set to before
set(BUILD_SHARED_LIBS ${PREV_BUILD_SHARED_LIBS} CACHE BOOL "" FORCE)

target_link_libraries(shared PUBLIC Tracy::TracyClient)

# Add the GSL
if(bundled-gsl)
	add_subdirectory("${3RDPARTY_DIR}/gsl" "${CMAKE_CURRENT_BINARY_DIR}/gsl" EXCLUDE_FROM_ALL)

	target_link_libraries(shared PUBLIC GSL)
else()
	# First try to locate via GSL's native cmake support (via a *Config.cmake file that should have been installed alongside
	# GSL since version 3)
	find_pkg("Microsoft.GSL")

	if (TARGET Microsoft.GSL::GSL)
		target_link_libraries(shared PUBLIC Microsoft.GSL::GSL)
	else()
		# If the above failed, it could mean that there is an installation of GSL < v3.0 on this system, which does not yet
		# provide cmake support for finding it. Thus, we have to use our custom Find-script.

		# We have to avoid a name clash with the GNU scientific library, which is commonly referred to as "GSL". Thus, we refer to the
		# guideline support library as MSGSL (Microsoft guideline support library).
		find_pkg("MSGSL" REQUIRED)

		# As we usually compile with -Werror, let the user know that warnings are expected in this case
		message(WARNING "It seems you are using a version of GSL < 3.0, which is known to cause compile warnings (about narrowing conversions)")

		target_link_libraries(shared PUBLIC MSGSL::MSGSL)
	endif()
endif()

if(bundled-spdlog)
	target_include_directories(shared SYSTEM PUBLIC "${3RDPARTY_DIR}/spdlog/include")
else()
	find_pkg("spdlog")
	target_link_libraries(shared PUBLIC spdlog::spdlog)
endif()

if(client)
	add_subdirectory(mumble)

	if(UNIX AND NOT APPLE)
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

if(benchmarks)
	add_subdirectory(benchmarks)
endif()
