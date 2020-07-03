# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

find_package(PkgConfig QUIET)

include(FindPackageMessage)

function(pkgconfig_search MODULE)
	if(NOT PkgConfig_FOUND)
		return()
	endif()

	# We don't want pkg_search_module() to write into the variables that will be passed to the pkgconfig_search()'s caller.
	set(PRIVATE "PRIVATE_${MODULE}")

	pkg_search_module(${PRIVATE} ${MODULE} QUIET)

	if(NOT ${PRIVATE}_FOUND)
		return()
	endif()

	if(NOT ${PRIVATE}_LINK_LIBRARIES)
		foreach(LIBRARY ${${PRIVATE}_LIBRARIES})
			# A cache entry named by <VAR> is created to store the result of find_library().
			# If the library is found the result is stored in the variable and the search will not be repeated unless the variable is cleared.
			# Using a name that depends on the loop-item is suggested in contrast to clearing the variable.
			# @ref https://cmake.org/pipermail/cmake/2011-November/047171.html
			find_library(${LIBRARY}_LINK ${PRIVATE_LIBRARY} PATHS ${${PRIVATE}_LIBRARY_DIRS})
			list(APPEND ${PRIVATE}_LINK_LIBRARIES ${${LIBRARY}_LINK})
		endforeach()
	endif()

	set(${MODULE}_INCLUDE_DIRS ${${PRIVATE}_INCLUDE_DIRS} PARENT_SCOPE)
	set(${MODULE}_LIBRARIES ${${PRIVATE}_LINK_LIBRARIES} PARENT_SCOPE)
	set(${MODULE}_VERSION ${${PRIVATE}_VERSION} PARENT_SCOPE)
	set(${MODULE}_FOUND ${${PRIVATE}_FOUND} PARENT_SCOPE)
endfunction()

# This macro's main purpose is to call find_package() with CONFIG and then with MODULE if it fails.
# It also handles multiple package names and searches them with pkg-config if they are not found.
macro(find_pkg ARG_ALIASES)
	# We specify "CONFIG", "MODULE" and "NO_MODULE" so that they are not considered unparsed arguments (passed to find_package()).
	cmake_parse_arguments(FIND_PACKAGE "ARG_ALIASES;CONFIG;MODULE;NO_DEFAULT_PATH;NO_MODULE;REQUIRED;QUIET" "" "COMPONENTS;OPTIONAL_COMPONENTS;PATHS" ${ARGN})

	if(FIND_PACKAGE_PATHS)
		list(APPEND FIND_PACKAGE_ARGUMENTS "PATHS" ${FIND_PACKAGE_PATHS})
	endif()

	if(FIND_PACKAGE_NO_DEFAULT_PATH)
		list(APPEND FIND_PACKAGE_ARGUMENTS "NO_DEFAULT_PATH")
	endif()

	# We add the aliases to a clean list because of an issue with list(GET).
	# If we pass the argument list directly to it, LAST_ALIAS is always set to "NOTFOUND".
	foreach(ALIAS ${ARG_ALIASES})
		list(APPEND ALIASES ${ALIAS})
	endforeach()

	list(REMOVE_DUPLICATES ALIASES)

	list(GET ALIASES -1 LAST_ALIAS)

	foreach(ALIAS ${ALIASES})
		set(NAME ${ALIAS})

		if(FIND_PACKAGE_COMPONENTS)
			foreach(COMPONENT ${FIND_PACKAGE_COMPONENTS})
				find_package(${NAME} COMPONENTS ${COMPONENT} ${FIND_PACKAGE_ARGUMENTS} QUIET CONFIG ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
				if(NOT ${NAME}_FOUND)
					find_package(${NAME} COMPONENTS ${COMPONENT} ${FIND_PACKAGE_ARGUMENTS} QUIET MODULE ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
				endif()

				if(NOT ${NAME}_FOUND)
					if(NOT ${NAME} STREQUAL ${LAST_ALIAS})
						break()
					else()
						if(FIND_PACKAGE_REQUIRED)
							message(FATAL_ERROR "${NAME} component not found: ${COMPONENT}")
						endif()

						if(NOT FIND_PACKAGE_QUIET)
							message(STATUS "${NAME} component not found: ${COMPONENT}")
						endif()

						break()
					endif()
				endif()

				if(NOT FIND_PACKAGE_QUIET)
					if(${NAME}_VERSION)
						find_package_message(${NAME} "${NAME} component found: ${COMPONENT} | Version: ${${NAME}_VERSION}" "[${NAME}][${COMPONENT}][${${NAME}_VERSION}]")
					else()
						find_package_message(${NAME} "${NAME} component found: ${COMPONENT}" "[${NAME}][${COMPONENT}]")
					endif()
				endif()
			endforeach()

			if(NOT ${NAME}_FOUND)
				continue()
			endif()
		else()
			find_package(${NAME} ${FIND_PACKAGE_ARGUMENTS} QUIET CONFIG ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
			if(NOT ${NAME}_FOUND)
				find_package(${NAME} ${FIND_PACKAGE_ARGUMENTS} QUIET MODULE ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
				if(NOT ${NAME}_FOUND)
					pkgconfig_search(${NAME})
				endif()
			endif()

			if(NOT ${NAME}_FOUND)
				if(NOT ${NAME} STREQUAL ${LAST_ALIAS})
					continue()
				else()
					if(FIND_PACKAGE_REQUIRED)
						message(FATAL_ERROR "${NAME} not found")
					endif()

					if(NOT FIND_PACKAGE_QUIET)
						message(STATUS "${NAME} not found")
					endif()

					break()
				endif()
			endif()

			if(NOT FIND_PACKAGE_QUIET)
				if(${NAME}_VERSION)
					find_package_message(${NAME} "${NAME} found | Version: ${${NAME}_VERSION}" "[${NAME}][${${NAME}_VERSION}]")
				else()
					find_package_message(${NAME} "${NAME} found" "[${NAME}]")
				endif()
			endif()
		endif()

		if(FIND_PACKAGE_OPTIONAL_COMPONENTS)
			foreach(COMPONENT ${FIND_PACKAGE_OPTIONAL_COMPONENTS})
				find_package(${NAME} COMPONENTS ${COMPONENT} ${FIND_PACKAGE_ARGUMENTS} QUIET CONFIG ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
				if(NOT ${NAME}_FOUND)
					find_package(${NAME} COMPONENTS ${COMPONENT} ${FIND_PACKAGE_ARGUMENTS} QUIET MODULE ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
				endif()

				if(${NAME}_FOUND AND NOT FIND_PACKAGE_QUIET)
					if(${NAME}_VERSION)
						find_package_message(${NAME} "${NAME} optional component found: ${COMPONENT} | Version: ${${NAME}_VERSION}" "[${NAME}][${COMPONENT}][${${NAME}_VERSION}]")
					else()
						find_package_message(${NAME} "${NAME} optional component found: ${COMPONENT}" "[${NAME}][${COMPONENT}]")
					endif()
				endif()
			endforeach()
		endif()

		if(${NAME}_FOUND)
			break()
		endif()
	endforeach()

	unset(ALIASES)
	unset(LAST_ALIAS)
	unset(FIND_PACKAGE_ARGUMENTS)

	if(${NAME}_FOUND)
		if(NOT ${NAME}_VERSION OR NOT ${NAME}_LIBRARIES)
			# The FindOpenSSL module defines the variables with an uppercase prefix.
			string(TOUPPER ${NAME} NAME_UPPER)

			if(NOT ${NAME}_VERSION)
				set(${NAME}_VERSION ${${NAME_UPPER}_VERSION})
			endif()

			if(NOT ${NAME}_LIBRARIES)
				set(${NAME}_LIBRARIES ${${NAME_UPPER}_LIBRARIES})
			endif()

			unset(NAME_UPPER)
		endif()
	elseif(NOT FIND_PACKAGE_QUIET)
		if(NOT PkgConfig_FOUND)
			message(WARNING "pkg-config was not found, consider installing it for better chances of finding a package")
		endif()
	endif()

	unset(NAME)
endmacro()
