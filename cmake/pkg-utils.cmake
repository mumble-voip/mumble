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
