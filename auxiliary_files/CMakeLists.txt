# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if(client)
	# Install Mumble man files
	install(FILES "man_files/mumble.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)

	if(UNIX AND NOT APPLE)
		configure_file("config_files/info.mumble.Mumble.appdata.xml.in" "${CMAKE_BINARY_DIR}/info.mumble.Mumble.appdata.xml")
		configure_file("config_files/info.mumble.Mumble.desktop.in" "${CMAKE_BINARY_DIR}/info.mumble.Mumble.desktop")
		install(FILES "${CMAKE_BINARY_DIR}/info.mumble.Mumble.appdata.xml" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/metainfo")
		install(FILES "${CMAKE_BINARY_DIR}/info.mumble.Mumble.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
	endif()
endif()

if(server)
	file(COPY "mumble_server.ini" DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY "config_files/mumble-server.conf" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

	if(UNIX)
		# Install Murmur man files
		install(FILES "man_files/mumble-server.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)
		install(FILES "man_files/mumble-server-user-wrapper.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)
	endif()
endif()
if(overlay)
	if(UNIX)
		# install overlay script
		install(PROGRAMS "run_scripts/mumble-overlay" DESTINATION "${MUMBLE_INSTALL_SCRIPTDIR}")

		# install overlay man-files
		install(FILES "man_files/mumble-overlay.1" DESTINATION "${MUMBLE_INSTALL_MANDIR}" COMPONENT doc)
	endif()
endif()
