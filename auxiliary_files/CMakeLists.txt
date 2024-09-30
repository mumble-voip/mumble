# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(delayed_configure_files)
include(pkg-utils)

if(NOT BUILD_RELEASE_DATE)
	# If BUILD_RELEASE_DATE has not been set, default to time of build
	string(TIMESTAMP BUILD_RELEASE_DATE "%Y-%m-%d" UTC)
endif()

if(overlay)
	if(UNIX)
		delayed_configure_files(
			TARGET overlay_gl
			FILES
				"${CMAKE_CURRENT_SOURCE_DIR}/run_scripts/mumble-overlay.in=${CMAKE_CURRENT_BINARY_DIR}/mumble-overlay"
			PASSTHROUGH_VARIABLES
				MUMBLE_BUILD_YEAR
				MUMBLE_INSTALL_ABS_LIBDIR
			VARIABLES
				"MUMBLE_OVERLAY_BINARY_BASENAME=$<TARGET_FILE_BASE_NAME:overlay_gl>"
			PYTHON_INTERPRETER "${PYTHON_INTERPRETER}"
			@ONLY
		)

		# install overlay script
		install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/mumble-overlay" DESTINATION "${MUMBLE_INSTALL_SCRIPTDIR}")

		# install overlay man-files
		install(FILES "man_files/mumble-overlay.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)
	endif()
endif()

if(client AND UNIX AND NOT APPLE)
	delayed_configure_files(
		TARGET mumble
		FILES
		"${CMAKE_CURRENT_SOURCE_DIR}/config_files/info.mumble.Mumble.appdata.xml.in=${CMAKE_CURRENT_BINARY_DIR}/info.mumble.Mumble.appdata.xml"
		"${CMAKE_CURRENT_SOURCE_DIR}/config_files/info.mumble.Mumble.desktop.in=${CMAKE_CURRENT_BINARY_DIR}/info.mumble.Mumble.desktop"
		PASSTHROUGH_VARIABLES
			MUMBLE_BUILD_YEAR
			CMAKE_PROJECT_VERSION
			BUILD_RELEASE_DATE
			CMAKE_PROJECT_HOMEPAGE_URL
			CMAKE_PROJECT_DESCRIPTION
		VARIABLES
			"MUMBLE_CLIENT_BINARY_NAME=$<TARGET_FILE_NAME:mumble>"
		PYTHON_INTERPRETER "${PYTHON_INTERPRETER}"
		@ONLY
	)

	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/info.mumble.Mumble.appdata.xml" DESTINATION "${MUMBLE_INSTALL_METADATADIR}")
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/info.mumble.Mumble.desktop" DESTINATION "${MUMBLE_INSTALL_APPLICATIONSDIR}")

	# Install Mumble client man files
	install(FILES "man_files/mumble.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)
endif()

if(server)
	file(COPY "mumble-server.ini" DESTINATION ${CMAKE_BINARY_DIR})

	# systemd related files
	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		delayed_configure_files(
			TARGET mumble-server
			FILES
				"${CMAKE_CURRENT_SOURCE_DIR}/config_files/mumble-server.service.in=${CMAKE_CURRENT_BINARY_DIR}/mumble-server.service"
				"${CMAKE_CURRENT_SOURCE_DIR}/config_files/mumble-server.tmpfiles.in=${CMAKE_CURRENT_BINARY_DIR}/mumble-server.tmpfiles"
				"${CMAKE_CURRENT_SOURCE_DIR}/run_scripts/mumble-server-user-wrapper.in=${CMAKE_CURRENT_BINARY_DIR}/mumble-server-user-wrapper"
			PASSTHROUGH_VARIABLES
				MUMBLE_BUILD_YEAR
				MUMBLE_INSTALL_ABS_EXECUTABLEDIR
				MUMBLE_INSTALL_ABS_SYSCONFDIR
			VARIABLES
				"MUMBLE_SERVER_BINARY_NAME=$<TARGET_FILE_NAME:mumble-server>"
			PYTHON_INTERPRETER "${PYTHON_INTERPRETER}"
			@ONLY
		)

		install(FILES "man_files/mumble-server-user-wrapper.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)

		install(FILES "config_files/mumble-server.sysusers" DESTINATION "${MUMBLE_INSTALL_SYSUSERSDIR}" COMPONENT mumble_server RENAME "mumble-server.conf")
		install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mumble-server.tmpfiles" DESTINATION "${MUMBLE_INSTALL_TMPFILESDIR}" COMPONENT mumble_server RENAME "mumble-server.conf")

		install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mumble-server.service" DESTINATION "${MUMBLE_INSTALL_SERVICEFILEDIR}" COMPONENT mumble_server)
		install(
			FILES "${CMAKE_CURRENT_BINARY_DIR}/mumble-server-user-wrapper" DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_server
			PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
	endif()

	if(UNIX AND NOT APPLE)
		install(FILES "man_files/mumble-server.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)

		install(
			FILES "mumble-server.ini" DESTINATION "${MUMBLE_INSTALL_SYSCONFDIR}" COMPONENT mumble_server
			PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ
		)

		if(ice)
			install(FILES "${CMAKE_SOURCE_DIR}/src/murmur/MumbleServer.ice" DESTINATION "${MUMBLE_INSTALL_SERVER_SHAREDIR}" COMPONENT mumble_server)
		endif()
	endif()
endif()

