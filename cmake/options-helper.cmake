# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

cmake_minimum_required(VERSION 3.15)

# declare_option(
#     NAME <option_name>
#     DESCRIPTION <option_description>
#     [DEFAULT <option_default>]
#     [RESTRICT_TO ...]
#     [EXCLUDE_ON ...]
#     [<system>_DEFAULT <system-specific default value>]
# )
#
# Declares an option with the given name and description. The default value for the declared option is either the value passed to
# the DEFAULT argument, or to the system-specific default key (e.g. LINUX_DEFAULT or WINDOWS_DEFAULT). If given, the latter takes
# precedence.
# The RESTRICT_TO keyword can be used to restrict a given option to the list of specified systems. Defining the option on any other
# system will result in an error.
# The inverse logic is applied to the arguments given to the EXCLUDE_ON keyword. If the option is specified on one of the listed
# systems, an error will be generated.
function(declare_option)
	set(MUMBLE_OPTIONS_SUPPORTED_OS "WINDOWS" "LINUX" "DARWIN" "FREEBSD" "OPENBSD")

	string(TOUPPER "${CMAKE_SYSTEM_NAME}" UPPER_SYSTEM_NAME)

	set(CURRENT_OS_DEFAULT_VAR MUMBLE_OPTIONS_${UPPER_SYSTEM_NAME}_DEFAULT)


	# Declare known arguments
	set(FLAGS "")
	set(ONE_VALUE_KEYWORDS NAME DESCRIPTION DEFAULT)
	set(MULTI_VALUE_KEYWORDS RESTRICT_TO EXCLUDE_ON)

	# Build and append OS-specific function arguments
	foreach(CURRENT_OS IN LISTS MUMBLE_OPTIONS_SUPPORTED_OS)
		list(APPEND ONE_VALUE_KEYWORDS "${CURRENT_OS}_DEFAULT")
	endforeach()

	# Parse arguments
	cmake_parse_arguments(MUMBLE_OPTIONS "${FLAGS}" "${ONE_VALUE_KEYWORDS}" "${MULTI_VALUE_KEYWORDS}" ${ARGN})

	# Error handling
	if (DEFINED MUMBLE_OPTIONS_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unrecognized arguments to declare_option: \"${MUMBLE_OPTIONS_UNPARSED_ARGUMENTS}\"")
	endif()
	if (NOT DEFINED MUMBLE_OPTIONS_NAME)
		message(FATAL_ERROR "Called declare_option without specifying a NAME")
	endif()
	if (NOT DEFINED MUMBLE_OPTIONS_DESCRIPTION)
		message(FATAL_ERROR "declare_option: No DESCRIPTION provided")
	endif()
	if (NOT DEFINED MUMBLE_OPTIONS_DEFAULT AND NOT DEFINED ${CURRENT_OS_DEFAULT_VAR})
		message(FATAL_ERROR "declare_option: No DEFAULT provided for option \"${MUMBLE_OPTIONS_NAME}\" and ${CURRENT_OS_DEFAULT_VAR} is not given either")
	endif()
	if (DEFINED MUMBLE_OPTIONS_RESTRICT_TO)
		# Make sure the OS is given in uppercase
		list(TRANSFORM MUMBLE_OPTIONS_RESTRICT_TO TOUPPER)

		foreach (CURRENT_OS IN LISTS MUMBLE_OPTIONS_RESTRICT_TO)
			if (NOT ${CURRENT_OS} IN_LIST MUMBLE_OPTIONS_SUPPORTED_OS)
				message(FATAL_ERROR "declare_option: Trying to restrict option ${MUMBLE_OPTIONS_NAME} to unknown/unsupported OS ${CURRENT_OS}")
			endif()
		endforeach()
	endif()
	if (DEFINED MUMBLE_OPTIONS_EXCLUDE_ON)
		# Make sure the OS is given in uppercase
		list(TRANSFORM MUMBLE_OPTIONS_EXCLUDE_ON TOUPPER)

		foreach (CURRENT_OS IN LISTS MUMBLE_OPTIONS_EXCLUDE_ON)
			if (NOT ${CURRENT_OS} IN_LIST MUMBLE_OPTIONS_SUPPORTED_OS)
				message(FATAL_ERROR "declare_option: Trying to exclude option ${MUMBLE_OPTIONS_NAME} on unknown/unsupported OS ${CURRENT_OS}")
			endif()
		endforeach()
	endif()


	# First check if the given option is supposed to be defined on the current OS
	if ((DEFINED MUMBLE_OPTIONS_RESTRICT_TO AND NOT ${UPPER_SYSTEM_NAME} IN_LIST MUMBLE_OPTIONS_RESTRICT_TO)
		OR (DEFINED MUMBLE_OPTIONS_EXCLUDE_ON AND ${UPPER_SYSTEM_NAME} IN_LIST MUMBLE_OPTIONS_EXCLUDE_ON))
		# The specified option is not supposed to exist on the given system. Make sure it is not set by accident
		if (DEFINED ${MUMBLE_OPTIONS_NAME})
			# The option was set regardless -> error (avoid confused people wondering why setting option xy has no effect)
			message(FATAL_ERROR "declare_option: Option \"${MUMBLE_OPTIONS_NAME}\" does not apply to system ${CMAKE_SYSTEM_NAME} but was set to"
			" ${${MUMBLE_OPTIONS_NAME}}.")
		endif()

		# Nothing more to do in this case - just return from this function
		return()
	endif()

	# Prepare to define the option
	if (DEFINED ${CURRENT_OS_DEFAULT_VAR})
		# Prefer the system-specific default value over the generic one
		set(DEFAULT_VALUE ${${CURRENT_OS_DEFAULT_VAR}})
	else()
		set(DEFAULT_VALUE ${MUMBLE_OPTIONS_DEFAULT})
	endif()

	message(DEBUG "Declaring option ${MUMBLE_OPTIONS_NAME} with default value ${DEFAULT_VALUE} and description \"${MUMBLE_OPTIONS_DESCRIPTION}\"")

	# Actually define the option with the provided default and description
	option(${MUMBLE_OPTIONS_NAME} "${MUMBLE_OPTIONS_DESCRIPTION}" ${DEFAULT_VALUE})
endfunction()
