# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(GNUInstallDirs)

# Turns a path into an absolute path if it isn't absolute already
function(make_absolute out_path in_path)
	get_filename_component(abs_path "${in_path}" ABSOLUTE BASE_DIR "${CMAKE_INSTALL_PREFIX}")
	set(${out_path} "${abs_path}" PARENT_SCOPE)
endfunction()

function(assert_is_relative path)
	make_absolute(abs_path "${path}")

	if("${path}" STREQUAL "${abs_path}")
		message(FATAL_ERROR "Encountered absolute install path but expected a relative one (\"${path}\")")
	endif()
endfunction()


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

if(packaging)
	# Using absolute install paths doesn't allow CPack to create the installer for us.
	# Therefore we have to make sure that the paths are indeed relative.
	assert_is_relative("${MUMBLE_INSTALL_EXECUTABLEDIR}")
	assert_is_relative("${MUMBLE_INSTALL_LIBDIR}")
	assert_is_relative("${MUMBLE_INSTALL_PLUGINDIR}")
	assert_is_relative("${MUMBLE_INSTALL_SCRIPTDIR}")
	assert_is_relative("${MUMBLE_INSTALL_MANDIR}")
	assert_is_relative("${MUMBLE_INSTALL_DOCDIR}")
	assert_is_relative("${MUMBLE_INSTALL_LICENSEDIR}")

	if(WIN32)
		if(NOT "${MUMBLE_INSTALL_LIBDIR}" STREQUAL "${LIBDIR_DEFAULT}")
			# The path has been altered which will not allow for a working installer to be created
			message(FATAL_ERROR "Found non default MUMBLE_INSTALL_LIBDIR path, that will not result in a working installer! (\"${MUMBLE_INSTALL_LIBDIR}\")")
		endif()
	endif()
else()
	# Turn all install paths into absolute ones as this is required for e.g. shared libraries to be loaded
	make_absolute(MUMBLE_INSTALL_EXECUTABLEDIR "${MUMBLE_INSTALL_EXECUTABLEDIR}")
	make_absolute(MUMBLE_INSTALL_LIBDIR "${MUMBLE_INSTALL_LIBDIR}")
	make_absolute(MUMBLE_INSTALL_PLUGINDIR "${MUMBLE_INSTALL_PLUGINDIR}")
	make_absolute(MUMBLE_INSTALL_SCRIPTDIR "${MUMBLE_INSTALL_SCRIPTDIR}")
	make_absolute(MUMBLE_INSTALL_MANDIR "${MUMBLE_INSTALL_MANDIR}")
	make_absolute(MUMBLE_INSTALL_DOCDIR "${MUMBLE_INSTALL_DOCDIR}")
	make_absolute(MUMBLE_INSTALL_LICENSEDIR "${MUMBLE_INSTALL_LICENSEDIR}")
endif()

option(display-install-paths OFF)

if(display-install-paths)
	message(STATUS "")
	message(STATUS "These are the paths the different components will be installed to:")
	message(STATUS "Executables:    \"${MUMBLE_INSTALL_EXECUTABLEDIR}\"")
	message(STATUS "Libraries:      \"${MUMBLE_INSTALL_LIBDIR}\"")
	message(STATUS "Plugins:        \"${MUMBLE_INSTALL_PLUGINDIR}\"")
	message(STATUS "Scripts:        \"${MUMBLE_INSTALL_SCRIPTDIR}\"")
	message(STATUS "Man-files:      \"${MUMBLE_INSTALL_MANDIR}\"")
	message(STATUS "Documentation:  \"${MUMBLE_INSTALL_DOCDIR}\"")
	message(STATUS "Licenses:       \"${MUMBLE_INSTALL_LICENSEDIR}\"")
	message(STATUS "")
endif()

