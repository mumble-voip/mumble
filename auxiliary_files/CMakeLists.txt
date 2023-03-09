# Copyright 2022-2023 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(delayed_configure_files)
include(pkg-utils)

if(NOT BUILD_RELEASE_DATE)
	# If BUILD_RELEASE_DATE has not been set, default to time of build
	string(TIMESTAMP BUILD_RELEASE_DATE "%Y-%m-%d")
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

	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/info.mumble.Mumble.appdata.xml" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/metainfo")
	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/info.mumble.Mumble.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")

	# Install Mumble client man files
	install(FILES "man_files/mumble.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)
endif()

if(server)
	file(COPY "mumble-server.ini" DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY "config_files/mumble-server.conf" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

	# systemd related files
	if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
		get_pkgconf_variable(
			OUTPUT_VARIABLE SYSUSERS_DIR
			MODULE systemd
			VARIABLE_NAME sysusersdir
			QUIET
		)
		if(NOT SYSUSERS_DIR)
			# Fallback value, in case it could not be fetched via pkg-config
			set(SYSUSERS_DIR "${CMAKE_INSTALL_SYSCONFDIR}/sysconfig.d")
		endif()
		get_pkgconf_variable(
			OUTPUT_VARIABLE SYSTEMD_SERVICE_DIR
			MODULE systemd
			VARIABLE_NAME systemd_system_unit_dir
			QUIET
		)
		if(NOT SYSTEMD_SERVICE_DIR)
			# Default to /etc/systemd/system - This is not where packages ought to install their service files (rather
			# it's where an admin is supposed to add their system files), but this seems to be the most consistent
			# path across different platforms, so we use it anyway.
			set(SYSTEMD_SERVICE_DIR "${CMAKE_INSTALL_SYSCONFDIR}/systemd/system")
		endif()

		delayed_configure_files(
			TARGET mumble-server
			FILES
				"${CMAKE_CURRENT_SOURCE_DIR}/config_files/mumble-server.service.in=${CMAKE_CURRENT_BINARY_DIR}/mumble-server.service"
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

		install(FILES "config_files/mumble-server.sysusers" DESTINATION "${SYSUSERS_DIR}" COMPONENT mumble_server RENAME "mumble-server.conf")

		install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mumble-server.service" DESTINATION "${SYSTEMD_SERVICE_DIR}" COMPONENT mumble_server)
		install(FILES "${CMAKE_CURRENT_BINARY_DIR}/mumble-server-user-wrapper" DESTINATION "${MUMBLE_INSTALL_EXECUTABLEDIR}" COMPONENT mumble_server)
		install(FILES "config_files/mumble-server.conf" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/dbus-1/system.d" COMPONENT mumble_server)
	endif()

	if(UNIX AND NOT APPLE)
		install(FILES "man_files/mumble-server.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)

		install(FILES "mumble-server.ini" DESTINATION "${MUMBLE_INSTALL_SYSCONFDIR}" COMPONENT mumble_server)

		if(ice)
			install(FILES "${CMAKE_SOURCE_DIR}/src/murmur/MumbleServer.ice" DESTINATION "${MUMBLE_INSTALL_SYSCONFDIR}" COMPONENT mumble_server)
		endif()
	endif()
endif()

