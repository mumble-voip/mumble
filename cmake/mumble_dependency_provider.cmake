# Note: find_package (and FetchContent_MakeAvailable for that matter) have the package/dependency
# name as their first argument, which is why we can rely on that here as well.
macro(mumble_provide_dependency METHOD DEP_NAME)
	if (NOT "${METHOD}" STREQUAL "FIND_PACKAGE")
		message(FATAL_ERROR "mumble_provide_dependency called for unexpected method: ${METHOD}")
	endif()

	string(TOUPPER "${DEP_NAME}" DEP_NAME_UPPER)


	# Handle defaults while still respecting package-specific overwrites
	if (DEFINED MUMBLE_DEP_SKIP_FIND_PACKAGE AND NOT DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_FIND_PACKAGE)
		set(MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_FIND_PACKAGE ${MUMBLE_DEP_SKIP_FIND_PACKAGE})
	endif()
	if (DEFINED MUMBLE_DEP_SKIP_PKGCONF AND NOT DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_PKGCONFIG)
		set(MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_PKGCONFIG ${MUMBLE_DEP_SKIP_PKGCONF})
	endif()
	if (DEFINED MUMBLE_DEP_SKIP_FETCHCONTENT AND NOT DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_FETCHCONTENT)
		set(MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_FETCHCONTENT ${MUMBLE_DEP_SKIP_FETCHCONTENT})
	endif()


	# Remove REQUIRED keyword so we have a chance at trying different methods to satisfy the
	# dependency without CMake erroring in between.
	set(FIND_ARGS "${ARGN}")
	list(FIND FIND_ARGS "REQUIRED" REQUIRED_IDX)
	if ("${REQUIRED_IDX}" GREATER_EQUAL 0)
		set(DEP_REQUIRED ON)
		list(REMOVE_AT FIND_ARGS "${REQUIRED_IDX}")
	else()
		set(DEP_REQUIRED OFF)
	endif()
	# We also want a custom handling of the QUIET option
	list(FIND FIND_ARGS "QUIET" QUIET_IDX)
	if ("${QUIET_IDX}" GREATER_EQUAL 0)
		set(DEP_QUIET ON)
		list(REMOVE_AT FIND_ARGS "${QUIET_IDX}")
	else()
		set(DEP_QUIET OFF)
	endif()

	if (NOT MUMBLE_DEP_DEBUG_MODE)
		set(QUIET_KEYWORD "QUIET")
	endif()

	if (NOT MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_FIND_PACKAGE)
		if (DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_FIND_PACKAGE_ARGS)
			set(FIND_PACKAGE_ARGS "${MUMBLE_DEP_${DEP_NAME_UPPER}_FIND_PACKAGE_ARGS}")
		else()
			set(FIND_PACKAGE_ARGS "${FIND_ARGS}")
		endif()

		if (DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_FIND_PACKAGE_EXTRA_ARGS)
			set(FIND_PACKAGE_ARGS "${FIND_PACKAGE_ARGS};${MUMBLE_DEP_${DEP_NAME_UPPER}_FIND_PACKAGE_EXTRA_ARGS}")
		endif()

		find_package(${DEP_NAME} ${FIND_PACKAGE_ARGS} ${QUIET_KEYWORD} BYPASS_PROVIDER)
	endif()

	
	find_package(PkgConfig ${QUIET_KEYWORD} BYPASS_PROVIDER)
	if (NOT ${DEP_NAME}_FOUND AND NOT MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_PKGCONFIG AND PkgConfig_FOUND)
		set(POTENTIAL_NAMES "${DEP_NAME}")
		cmake_parse_arguments(DUMMY "REQUIRED" "" "NAMES" ${ARGN})
		if (DUMMY_NAMES)
			list(APPEND POTENTIAL_NAMES ${DUMMY_NAMES})
		endif()

		foreach(CURRENT_NAME IN LISTS POTENTIAL_NAMES)
			# We use a different name to avoid clashes with variables that pkg_search_module might set
			set(PKG_BASENAME "MUMBLE_DEP_PROVIDER_PKGCONF_${CURRENT_NAME}")

			pkg_search_module("${PKG_BASENAME}" "${CURRENT_NAME}" ${QUIET_KEYWORD} IMPORTED_TARGET)

			if (TARGET PkgConfig::${PKG_BASENAME})
				if (DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_EXPECTED_MAIN_TARGET)
					add_library("${MUMBLE_DEP_${DEP_NAME_UPPER}_EXPECTED_MAIN_TARGET}" ALIAS PkgConfig::${PKG_BASENAME})
				endif()

				set(${DEP_NAME}_LIBRARIES "PkgConfig::${PKG_BASENAME}")
				set(${DEP_NAME}_FOUND TRUE)
				set(${DEP_NAME}_VERSION "${${PKG_BASENAME}_VERSION}")

				break()
			endif()
		endforeach()
	endif()


	if (NOT ${DEP_NAME}_FOUND AND NOT MUMBLE_DEP_${DEP_NAME_UPPER}_SKIP_FETCHCONTENT AND DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_FETCHCONTENT_ID)
		include(FetchContent)
		FetchContent_MakeAvailable(${MUMBLE_DEP_${DEP_NAME_UPPER}_FETCHCONTENT_ID})
		set(${DEP_NAME}_FOUND TRUE)
	endif()


	if (DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_EXPECTED_MAIN_TARGET)
		if (NOT TARGET ${MUMBLE_DEP_${DEP_NAME_UPPER}_EXPECTED_MAIN_TARGET})
			set(${DEP_NAME}_FOUND FALSE)
		endif()
	endif()

	if (${DEP_NAME}_FOUND AND NOT DEFINED MUMBLE_DEP_${DEP_NAME_UPPER}_LICENSE AND NOT DEP_NAME_UPPER STREQUAL "THREADS" AND NOT DEP_NAME_UPPER STREQUAL "PKGCONFIG")
		message(WARNING "Using dependency '${DEP_NAME}' with unknown license")
	endif()

	# TODO: find_package_handle_standard_args for printing status of find_package call
	
	# TODO: Handle creation of alias targets (cmp. what we do for utfcpp)

	# TODO: clean local variables to prevent spillage to outside
endmacro()

cmake_language(SET_DEPENDENCY_PROVIDER mumble_provide_dependency SUPPORTED_METHODS FIND_PACKAGE)
