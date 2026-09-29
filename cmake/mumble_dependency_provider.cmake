# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

message(STATUS "Using Mumble's dependency provider")

include(FetchContent)
include(FindPackageHandleStandardArgs)

# Determines how to provide the given dependency. Results are exported as _MUMBLE_DEP_<DEP_NAME>_*
# and registered in _MUMBLE_DEP_<DEP_NAME>_VARIABLES_TO_BE_CLEARED.
function(_mumble_dep_prepare DEP_NAME)
	string(TOUPPER "${DEP_NAME}" DEP_NAME_UPPER)
	set(TO_BE_CLEARED "${_MUMBLE_DEP_${DEP_NAME}_VARIABLES_TO_BE_CLEARED}")

	# Package-specific settings take precedence over the global defaults
	foreach(METHOD IN ITEMS "FIND_PACKAGE" "FETCHCONTENT")
		if (DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_${METHOD})
			set(SKIP "${MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_${METHOD}}")
		else()
			set(SKIP "${MUMBLE_DEP_SKIP_${METHOD}}")
		endif()
		set(_MUMBLE_DEP_${DEP_NAME}_SKIP_${METHOD} "${SKIP}" PARENT_SCOPE)
		list(APPEND TO_BE_CLEARED "_MUMBLE_DEP_${DEP_NAME}_SKIP_${METHOD}")
	endforeach()

	if (DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_FIND_PACKAGE_ARGS)
		set(FIND_ARGS "${MUMBLE_DEP_${DEP_NAME_UPPER}_FIND_PACKAGE_ARGS}")
	else()
		set(FIND_ARGS "${ARGN}")
		# Without REQUIRED, a failed find_package doesn't prevent us from trying other methods.
		# QUIET is handled separately below.
		list(REMOVE_ITEM FIND_ARGS "REQUIRED" "QUIET")
	endif()
	list(APPEND FIND_ARGS ${MUMBLE_DEP_${DEP_NAME_UPPER}_FIND_PACKAGE_EXTRA_ARGS})
	if (NOT MUMBLE_DEP_DEBUG_MODE)
		list(APPEND FIND_ARGS "QUIET")
	endif()
	set(_MUMBLE_DEP_${DEP_NAME}_FIND_PACKAGE_ARGS "${FIND_ARGS}" PARENT_SCOPE)
	list(APPEND TO_BE_CLEARED "_MUMBLE_DEP_${DEP_NAME}_FIND_PACKAGE_ARGS")

	if (DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_FETCHCONTENT_ID)
		set(_MUMBLE_DEP_${DEP_NAME}_FETCHCONTENT_ID "${MUMBLE_DEP_${DEP_NAME_UPPER}_FETCHCONTENT_ID}" PARENT_SCOPE)
		list(APPEND TO_BE_CLEARED "_MUMBLE_DEP_${DEP_NAME}_FETCHCONTENT_ID")
	endif()

	set(_MUMBLE_DEP_${DEP_NAME}_VARIABLES_TO_BE_CLEARED "${TO_BE_CLEARED}" PARENT_SCOPE)
endfunction()

# Validates the obtained dependency and updates <DEP_NAME>_FOUND accordingly.
function(_mumble_dep_verify DEP_NAME)
	string(TOUPPER "${DEP_NAME}" DEP_NAME_UPPER)

	if (DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_EXPECTED_MAIN_TARGET)
		if (NOT TARGET ${MUMBLE_DEP_${DEP_NAME_UPPER}_EXPECTED_MAIN_TARGET})
			set(${DEP_NAME}_FOUND FALSE)
			set(${DEP_NAME}_FOUND FALSE PARENT_SCOPE)
		endif()
	endif()

	if (${DEP_NAME}_FOUND AND NOT DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_LICENSE AND NOT DEP_NAME_UPPER STREQUAL "THREADS" AND NOT DEP_NAME_UPPER STREQUAL "PKGCONFIG")
		message(WARNING "Using dependency '${DEP_NAME}' with unknown license (${DEP_NAME_UPPER})")
	endif()
endfunction()

# This has to be a macro so that variables set by find_package and FetchContent_MakeAvailable reach the caller.
# Both may call the provider recursively, which overwrites any plain local variable. Macro parameters are
# immune to this, so all state is stored in variables that contain DEP_NAME in their name.
macro(mumble_provide_dependency METHOD DEP_NAME)
	if (NOT "${METHOD}" STREQUAL "FIND_PACKAGE")
		message(FATAL_ERROR "mumble_provide_dependency called for unexpected method: ${METHOD}")
	endif()

	if (DEFINED _MUMBLE_DEP_${DEP_NAME}_ACTIVE)
		message(FATAL_ERROR "Recursive request for dependency '${DEP_NAME}' while it is being provided")
	endif()
	set(_MUMBLE_DEP_${DEP_NAME}_ACTIVE ON)
	set(_MUMBLE_DEP_${DEP_NAME}_VARIABLES_TO_BE_CLEARED "_MUMBLE_DEP_${DEP_NAME}_VARIABLES_TO_BE_CLEARED;_MUMBLE_DEP_${DEP_NAME}_ACTIVE")

	_mumble_dep_prepare("${DEP_NAME}" ${ARGN})

	if (NOT _MUMBLE_DEP_${DEP_NAME}_SKIP_FIND_PACKAGE)
		find_package(${DEP_NAME} ${_MUMBLE_DEP_${DEP_NAME}_FIND_PACKAGE_ARGS} BYPASS_PROVIDER)
	endif()

	if (NOT ${DEP_NAME}_FOUND AND NOT _MUMBLE_DEP_${DEP_NAME}_SKIP_FETCHCONTENT AND DEFINED _MUMBLE_DEP_${DEP_NAME}_FETCHCONTENT_ID)
		FetchContent_MakeAvailable(${_MUMBLE_DEP_${DEP_NAME}_FETCHCONTENT_ID})
		set(${DEP_NAME}_FOUND TRUE)
		set(${DEP_NAME}_FETCHED TRUE)
	endif()

	_mumble_dep_verify("${DEP_NAME}")

	if (${DEP_NAME}_VERSION)
		set(_MUMBLE_DEP_${DEP_NAME}_VERSION_ARGS "HANDLE_VERSION_RANGE;VERSION_VAR;${DEP_NAME}_VERSION")
		list(APPEND _MUMBLE_DEP_${DEP_NAME}_VARIABLES_TO_BE_CLEARED "_MUMBLE_DEP_${DEP_NAME}_VERSION_ARGS")
	endif()

	find_package_handle_standard_args("${DEP_NAME}"
		REQUIRED_VARS "${DEP_NAME}_FOUND"
		NAME_MISMATCHED
		${_MUMBLE_DEP_${DEP_NAME}_VERSION_ARGS}
	)

	# Clear up created variables
	list(REVERSE _MUMBLE_DEP_${DEP_NAME}_VARIABLES_TO_BE_CLEARED)
	foreach(_MUMBLE_DEP_${DEP_NAME}_CURRENT IN LISTS _MUMBLE_DEP_${DEP_NAME}_VARIABLES_TO_BE_CLEARED)
		unset("${_MUMBLE_DEP_${DEP_NAME}_CURRENT}")
	endforeach()
	unset(_MUMBLE_DEP_${DEP_NAME}_CURRENT)
endmacro()

cmake_language(SET_DEPENDENCY_PROVIDER mumble_provide_dependency SUPPORTED_METHODS FIND_PACKAGE)
