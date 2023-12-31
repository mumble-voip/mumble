# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
message(STATUS "Components: ${COMPONENTS}")

set(CPACK_COMPONENTS_GROUPING "IGNORE")
set(CPACK_COMPONENTS_ALL "mumble_client" "mumble_server")

string(TOLOWER "${CMAKE_PROJECT_NAME}" PACKAGE_NAME)
string(APPEND PACKAGE_NAME "_upstream")

set(CPACK_PACKAGE_NAME "${PACKAGE_NAME}")
set(CPACK_PACKAGE_VENDOR "The Mumble Developers")
set(CPACK_PACKAGE_DESCRIPTION "${CMAKE_PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://www.mumble.info/")
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/icons/mumble_256x256.png")
set(CPACK_PACKAGE_CHECKSUM "SHA256")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_CONTACT "Mumble Developers <contact@mumble.info>")
set(CPACK_DEB_COMPONENT_INSTALL ON)


##########################################################
######################### DEB ############################
##########################################################
set(CPACK_DEBIAN_PACKAGE_SECTION "comm")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")

# client
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_DEPENDS "libasound2 (>= 1.0.16), libavahi-compat-libdnssd1 (>= 0.6.16), libc6 (>= 2.29), libgcc-s1 (>= 3.0),
	libjack-jackd2-0 (>= 1.9.10+20150825), 	libprotobuf23 (>= 3.12.4), libpulse0 (>= 0.99.1), libqt5core5a (>= 5.15.1), libqt5dbus5 (>= 5.14.1),
	libqt5gui5 (>= 5.14.1), libqt5network5 (>= 5.14.1), libqt5sql5 (>= 5.3.0), libqt5sql5-sqlite, libqt5svg5 (>= 5.6.0~beta), libqt5widgets5 (>= 5.14.1),
	libqt5xml5 (>= 5.0.2), libsndfile1 (>= 1.0.20), libspeechd2 (>= 0.7.1), libspeexdsp1 (>= 1.2~beta3.2-1), libssl3 (>= 3.0.0), libstdc++6 (>= 9),
	libx11-6 (>= 2:1.2.99.901), libxi6 (>= 2:1.2.99.4), lsb-release, libopus0")
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_SUGGESTS "speech-dispatcher" "mumble-server")
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_BREAKS "mumble")
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_BREAKS "mumble")
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_CONFLICTS "mumble")
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_REPLACES "mumble")

# server
set(CPACK_DEBIAN_MUMBLE_SERVER_PACKAGE_DEPENDS "adduser, debconf (>= 0.5), libavahi-compat-libdnssd1 (>= 0.6.16), libc6 (>= 2.17), libcap2 (>=
	1:2.10), libgcc-s1 (>= 3.0), libprotobuf23 (>= 3.12.4), libqt5core5a (>= 5.15.1), libqt5dbus5 (>= 5.14.1), libqt5network5 (>= 5.14.1),
	libqt5sql5 (>= 5.0.2), libqt5sql5-sqlite, libqt5sql5-mysql, libqt5sql5-psql, libqt5xml5 (>= 5.0.2), libssl3 (>= 3.0.0), libstdc++6 (>= 5.2),
	libzeroc-ice3.7, lsb-base (>= 3.0-6)")
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_BREAKS "mumble-server" "murmur")
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_CONFLICTS "mumble-server" "murmur")
set(CPACK_DEBIAN_MUMBLE_CLIENT_PACKAGE_REPLACES "mumble-server" "murmur")

include(CPack)

if(server)
	cpack_add_component(
		mumble_server
		DISPLAY_NAME "Mumble server"
		DESCRIPTION "The Mumble server (formerly known as Murmur)"
		GROUP mumble_server_group
	)
endif()

if(client)
	cpack_add_component(
		mumble_client
		DISPLAY_NAME "Mumble client"
		DESCRIPTION "The Mumble client"
		GROUP mumble_client_group
	)
endif()
