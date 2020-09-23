# Copyright 2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(GNUInstallDirs)

if(UNIX)
	if(APPLE)
		set(EXECUTABLEDIR_DEFAULT ".")
		set(LIBDIR_DEFAULT "${CMAKE_INSTALL_LIBDIR}/mumble")
		set(PLUGINDIR_DEFAULT "${CMAKE_INSTALL_LIBDIR}/mumble/plugins")
		set(SCRIPTDIR_DEFAULT "${CMAKE_INSTALL_BINDIR}")
		set(MANDIR_DEFAULT "${CMAKE_INSTALL_MANDIR}/man1")
		set(DOCDIR_DEFAULT "${CMAKE_INSTALL_DOCDIR}")
		set(LICENSEDIR_DEFAULT "${EXECUTABLEDIR_DEFAULT}/licenses")
	else()
		set(EXECUTABLEDIR_DEFAULT "bin")
		set(LIBDIR_DEFAULT "${CMAKE_INSTALL_LIBDIR}/mumble")
		set(PLUGINDIR_DEFAULT "${CMAKE_INSTALL_LIBDIR}/mumble/plugins")
		set(SCRIPTDIR_DEFAULT "${CMAKE_INSTALL_BINDIR}")
		set(MANDIR_DEFAULT "${CMAKE_INSTALL_MANDIR}/man1")
		set(DOCDIR_DEFAULT "${CMAKE_INSTALL_DOCDIR}")
		set(LICENSEDIR_DEFAULT "${EXECUTABLEDIR_DEFAULT}/licenses")
	endif()
else()
	set(EXECUTABLEDIR_DEFAULT ".")
	# In order for the installer to produce a working version of Mumble, this
	# path must not be changed.
	set(LIBDIR_DEFAULT ".")
	set(PLUGINDIR_DEFAULT "./plugins")
	set(SCRIPTDIR_DEFAULT "./scripts")
	set(MANDIR_DEFAULT "./man1")
	set(DOCDIR_DEFAULT "./docs")
		set(LICENSEDIR_DEFAULT "${EXECUTABLEDIR_DEFAULT}/licenses")
endif()

set(MUMBLE_INSTALL_EXECUTABLEDIR "${EXECUTABLEDIR_DEFAULT}" CACHE PATH "The directory to install the main executable(s) into")
set(MUMBLE_INSTALL_LIBDIR "${LIBDIR_DEFAULT}" CACHE PATH "The directory to install the built libraries into")
set(MUMBLE_INSTALL_PLUGINDIR "${PLUGINDIR_DEFAULT}" CACHE PATH "The directory to install the built plugins into")
set(MUMBLE_INSTALL_SCRIPTDIR "${SCRIPTDIR_DEFAULT}" CACHE PATH "The directory to install any additional scripts into")
set(MUMBLE_INSTALL_MANDIR "${MANDIR_DEFAULT}" CACHE PATH "The directory to install man files into")
set(MUMBLE_INSTALL_DOCDIR "${DOCDIR_DEFAULT}" CACHE PATH "The directory to install documentation files into")
set(MUMBLE_INSTALL_LICENSEDIR "${LICENSEDIR_DEFAULT}" CACHE PATH "The directory to install license files into")

option(display-install-paths OFF)

if(display-install-paths)
	# Turn into absolute paths by prepending the install-prefix (unless they are absolute paths already)
	# We only do that for printing the paths as using absolute install paths prevents CPack from working and
	# therefore prevents us from being able to build the installer.
	get_filename_component(EXECUTABLEDIR_ABS "${MUMBLE_INSTALL_EXECUTABLEDIR}" ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")
	get_filename_component(LIBDIR_ABS "${MUMBLE_INSTALL_LIBDIR}" ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")
	get_filename_component(PLUGINDIR_ABS "${MUMBLE_INSTALL_PLUGINDIR}" ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")
	get_filename_component(SCRIPTDIR_ABS "${MUMBLE_INSTALL_SCRIPTDIR}" ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")
	get_filename_component(MANDIR_ABS "${MUMBLE_INSTALL_MANDIR}" ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")
	get_filename_component(DOCDIR_ABS "${MUMBLE_INSTALL_DOCDIR}" ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")
	get_filename_component(LICENSEDIR_ABS "${MUMBLE_INSTALL_LICENSEDIR}" ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")

	message(STATUS "These are the paths the different components will be installed to:")
	message(STATUS "Executables:    \"${EXECUTABLEDIR_ABS}\"")
	message(STATUS "Libraries:      \"${LIBDIR_ABS}\"")
	message(STATUS "Plugins:        \"${PLUGINDIR_ABS}\"")
	message(STATUS "Scripts:        \"${SCRIPTDIR_ABS}\"")
	message(STATUS "Man-files:      \"${MANDIR_ABS}\"")
	message(STATUS "Documentation:  \"${DOCDIR_ABS}\"")
	message(STATUS "Licenses:       \"${LICENSEDIR_ABS}\"")
endif()

