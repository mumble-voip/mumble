# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if(debug-dependency-search)
	# Don't be quiet if the user wants to debug the dependency-search
	set(QUIET_STR "")
else()
	set(QUIET_STR "QUIET")
endif()

find_package(PkgConfig ${QUIET_STR})

include(FindPackageMessage)

function(pkgconfig_search MODULE)
	if(NOT PkgConfig_FOUND)
		return()
	endif()

	# We don't want pkg_search_module() to write into the variables that will be passed to the pkgconfig_search()'s caller.
	set(PRIVATE "PRIVATE_${MODULE}")

	pkg_search_module(${PRIVATE} ${MODULE} ${QUIET_STR})

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
	cmake_parse_arguments(FIND_PACKAGE "ARG_ALIASES;CONFIG;MODULE;NO_DEFAULT_PATH;NO_MODULE;REQUIRED;QUIET" "VERSION" "COMPONENTS;OPTIONAL_COMPONENTS;PATHS" ${ARGN})

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
				find_package(${NAME} ${FIND_PACKAGE_VERSION} COMPONENTS ${COMPONENT} ${FIND_PACKAGE_ARGUMENTS} ${QUIET_STR} CONFIG ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
				if(NOT ${NAME}_FOUND)
					find_package(${NAME} ${FIND_PACKAGE_VERSION} COMPONENTS ${COMPONENT} ${FIND_PACKAGE_ARGUMENTS} ${QUIET_STR} MODULE ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
				endif()

				if(NOT ${NAME}_FOUND)
					if(NOT ${NAME} STREQUAL ${LAST_ALIAS})
						break()
					else()
						if(FIND_PACKAGE_REQUIRED)
							message(FATAL_ERROR "${NAME} component not found: ${COMPONENT} ${FIND_PACKAGE_VERSION}")
						endif()

						if(NOT FIND_PACKAGE_QUIET)
							message(STATUS "${NAME} component not found: ${COMPONENT} ${FIND_PACKAGE_VERSION}")
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
			find_package(${NAME} ${FIND_PACKAGE_ARGUMENTS} ${QUIET_STR} CONFIG ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
			if(NOT ${NAME}_FOUND)
				find_package(${NAME} ${FIND_PACKAGE_ARGUMENTS} ${QUIET_STR} MODULE ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
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
				find_package(${NAME} COMPONENTS ${COMPONENT} ${FIND_PACKAGE_ARGUMENTS} ${QUIET_STR} CONFIG ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
				if(NOT ${NAME}_FOUND)
					find_package(${NAME} COMPONENTS ${COMPONENT} ${FIND_PACKAGE_ARGUMENTS} ${QUIET_STR} MODULE ${FIND_PACKAGE_UNPARSED_ARGUMENTS})
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

		# If multiple capitalizations are searched for this will ensure that the
		# lowercase name of the library's variables are set as well.
		# Simplifying usage of find_pkg on case-insensitive file systems.
		string(TOLOWER ${NAME} NAME_LOWER)
		if(NOT ${NAME} STREQUAL NAME_LOWER AND ${NAME_LOWER} IN_LIST ALIASES)

			if(NOT ${NAME_LOWER}_VERSION)
				set(${NAME_LOWER}_VERSION ${${NAME}_VERSION})
			endif()

			if(NOT ${NAME_LOWER}_LIBRARIES)
				set(${NAME_LOWER}_LIBRARIES ${${NAME}_LIBRARIES})
			endif()

			if(NOT ${NAME_LOWER}_FOUND)
				set(${NAME_LOWER}_FOUND ${${NAME}_FOUND})
			endif()

			if(NOT ${NAME_LOWER}_INCLUDE_DIRS)
				set(${NAME_LOWER}_INCLUDE_DIRS ${${NAME}_INCLUDE_DIRS})
			endif()
		endif()
		unset(NAME_LOWER)
	elseif(NOT FIND_PACKAGE_QUIET)
		if(NOT PkgConfig_FOUND)
			message(WARNING "pkg-config was not found, consider installing it for better chances of finding a package")
		endif()
	endif()

	unset(ALIASES)
	unset(NAME)
endmacro()


# Uses pkg-config to obtain the value of a module variable
# In contrast to standard cmake's pkg_get_variable, this function abstracts the
# difference between pkg-config and pkgconf where one of them uses underscores in
# the variable names, whereas the other does not. Thus, when provided with a
# variable name that contains underscores and for which no value could be fetched,
# the function will automatically check whether removing the underscores from the
# variable name will yield a non-empty value and if this is the case, this value
# will be returned.
#
# OUTPUT_VARIABLE <varName> Name of the output variable (Required)
# MODULE <module>           Name of the module to search for (Required)
# VARIABLE_NAME <varName>   Name of the variable to search for within the module (Required)
#                           Note: This name should be the one containing underscores.
# QUIET                     Don't print any status messages
# REQUIRED                  Error, if no non-empty value could be retrieved
# DEBUG                     Print status messages intended to debug this function
function(get_pkgconf_variable)
	set(options QUIET REQUIRED DEBUG)
	set(oneValueArgs OUTPUT_VARIABLE MODULE VARIABLE_NAME)
	set(multiValueArgs)

	cmake_parse_arguments(GET_PKGCONF_VAR "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	# Handle required args
	set(REQUIRED_ARGS ${oneValueArgs})
	foreach(CURRENT IN LISTS REQUIRED_ARGS)
		if (NOT GET_PKGCONF_VAR_${CURRENT})
			message(FATAL_ERROR "get_pkgconf_variable: Missing mandatory parameter \"${CURRENT}\"")
		endif()
	endforeach()

	# Check that we have found pkg-config to begin with
	if(NOT PkgConfig_FOUND)
		message(FATAL_ERROR "get_pkgconf_variable: pkg-config was not found - can't use it to fetch variables")
	endif()

	pkg_search_module("PKGCONF_MODULE" "${GET_PKGCONF_VAR_MODULE}" QUIET)
	if(NOT PKGCONF_MODULE_FOUND)
		if(GET_PKGCONF_VAR_DEBUG)
			message(STATUS "Unable to find module \"${GET_PKGCONF_VAR_MODULE}\" via pkg-conf")
			set(VAR_VALUE "NOTFOUND")
		endif()
	else()
		if(GET_PKGCONF_VAR_DEBUG)
			message(STATUS "get_pkgconf_variable: Searching for \"${GET_PKGCONF_VAR_VARIABLE_NAME}\" in module \"${GET_PKGCONF_VAR_MODULE}\"")
		endif()

		pkg_get_variable(VAR_VALUE "${GET_PKGCONF_VAR_MODULE}" "${GET_PKGCONF_VAR_VARIABLE_NAME}")

		if(NOT VAR_VALUE AND GET_PKGCONF_VAR_VARIABLE_NAME MATCHES ".*_.*")
			# There seems to be a difference between pkgconf and pkg-config where one uses underscores
			# in variable names, whereas the other does not. We assume that VAR_NAME is the version with
			# the underscores, so if that failed to fetch a value, we try again without the underscores.
			string(REPLACE "_" "" VAR_NAME_WITHOUT_UNDERSCORES "${GET_PKGCONF_VAR_VARIABLE_NAME}")

			if(GET_PKGCONF_VAR_DEBUG)
				message(STATUS "get_pkgconf_variable: Removed underscores from variable name")
				message(STATUS "get_pkgconf_variable: Now searching for variable \"${VAR_NAME_WITHOUT_UNDERSCORES}\"")
			endif()

			pkg_get_variable(VAR_VALUE "${GET_PKGCONF_VAR_MODULE}" "${VAR_NAME_WITHOUT_UNDERSCORES}")
		endif()
	endif()

	if(NOT VAR_VALUE)
		if(GET_PKGCONF_VAR_REQUIRED)
			message(FATAL_ERROR "get_pkgconf_variable: Failed to fetch \"${GET_PKGCONF_VAR_VARIABLE_NAME}\" of module \"${GET_PKGCONF_VAR_MODULE}\"")
		endif()
		if(NOT GET_PKGCONF_VAR_QUIET)
			message(WARNING "get_pkgconf_variable: Failed to fetch \"${GET_PKGCONF_VAR_VARIABLE_NAME}\" of module \"${GET_PKGCONF_VAR_MODULE}\"")
		endif()
	endif()

	if(GET_PKGCONF_VAR_DEBUG)
		message(STATUS "get_pkgconf_variable: Determined ${GET_PKGCONF_VAR_VARIABLE_NAME}=\"${VAR_VALUE}\"")
	endif()

	set(${GET_PKGCONF_VAR_OUTPUT_VARIABLE} "${VAR_VALUE}" PARENT_SCOPE)
endfunction()
