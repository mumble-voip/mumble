# Declare our dependencies

include(FetchContent)

function(mumble_register_dependency NAME)
	if (NOT NAME)
		message(FATAL_ERROR "Called mumble_register_dependency without NAME")
	endif()

	string(TOUPPER "${NAME}" NAME_UPPER)

	set(options "")
	set(one_val_keywords "MAIN_TARGET;FETCHCONTENT_ID")
	set(multi_val_keywords "")
	cmake_parse_arguments(PARSE_ARGV 1 "DEP" "${options}" "${one_val_keywords}" "${multi_val_keywords}")

	if (DEP_UNPARSED_ARGUMENTS OR DEP_KEYWORDS_MISSING_VALUES)
		message(FATAL_ERROR "Invalid use of mumble_register_dependency: ${DEP_UNPARSED_ARGUMENTS} ${DEP_KEYWORDS_MISSING_VALUES}")
	endif()

	if (DEP_MAIN_TARGET)
		set(MUMBLE_DEP_${NAME_UPPER}_EXPECTED_MAIN_TARGET "${DEP_MAIN_TARGET}" PARENT_SCOPE)
	endif()
	if (DEP_FETCHCONTENT_ID)
		set(MUMBLE_DEP_${NAME_UPPER}_FETCHCONTENT_ID "${DEP_FETCHCONTENT_ID}" PARENT_SCOPE)
	endif()
endfunction()


# Read JSON file containing the dependency specification
file(READ "${PROJECT_SOURCE_DIR}/dependencies.json" MUMBLE_DEPENDENCIES)

string(JSON MUMBLE_DEPENDENCIES GET "${MUMBLE_DEPENDENCIES}" "dependencies")
string(JSON NUM_DEPS LENGTH "${MUMBLE_DEPENDENCIES}")
math(EXPR LAST_IDX "${NUM_DEPS} - 1")

foreach(CURRENT_DEP_IDX RANGE ${LAST_IDX})
	string(JSON DEP_NAME GET "${MUMBLE_DEPENDENCIES}" ${CURRENT_DEP_IDX} "name")
	string(JSON DEP_LICENSE GET "${MUMBLE_DEPENDENCIES}" ${CURRENT_DEP_IDX} "license")
	string(JSON DEP_SOURCE ERROR_VARIABLE DUMMY GET "${MUMBLE_DEPENDENCIES}" ${CURRENT_DEP_IDX} "source_code")
	string(JSON DEP_VERSION ERROR_VARIABLE DUMMY GET "${MUMBLE_DEPENDENCIES}" ${CURRENT_DEP_IDX} "tracked_version")
	string(JSON DEP_FILE ERROR_VARIABLE DUMMY GET "${MUMBLE_DEPENDENCIES}" ${CURRENT_DEP_IDX} "file_name")
	string(JSON DEP_HASH ERROR_VARIABLE DUMMY GET "${MUMBLE_DEPENDENCIES}" ${CURRENT_DEP_IDX} "file_hash")
	string(JSON DEP_TARGET ERROR_VARIABLE DUMMY GET "${MUMBLE_DEPENDENCIES}" ${CURRENT_DEP_IDX} "cmake_target")

	set(FETCHCONTENT_ARG "")
	if (DEP_SOURCE AND DEP_VERSION)
		if ("${DEP_SOURCE}" MATCHES ".*\\.git")
			FetchContent_Declare("${DEP_NAME}"
				GIT_TAG        "${DEP_VERSION}"
				GIT_REPOSITORY "${DEP_SOURCE}"
				GIT_SHALLOW TRUE
				SYSTEM
				EXCLUDE_FROM_ALL
			)
		else()
			if (NOT DEP_FILE OR NOT DEP_HASH)
				message(FATAL_ERROR "Missing 'file_name' and 'file_hash' propeerties for dependency '${DEP_NAME}'")
			endif()
			FetchContent_Declare("${DEP_NAME}"
				# This URL scheme assumes we're using GitHub repo URLs as DEP_SOURCE
				URL        "${DEP_SOURCE}/releases/download/${DEP_VERSION}/${DEP_FILE}"
				URL_HASH   "SHA256=${DEP_HASH}"
				DOWNLOAD_NO_PROGRESS TRUE
				SYSTEM
				EXCLUDE_FROM_ALL
			)
		endif()

		set(FETCHCONTENT_ARG "FETCHCONTENT_ID;${DEP_NAME}")
	endif()

	set(TARGET_ARG "")
	if (DEP_TARGET)
		set(TARGET_ARG "MAIN_TARGET;${DEP_TARGET}")
	endif()

	mumble_register_dependency("${DEP_NAME}" ${FETCHCONTENT_ARG} ${TARGET_ARG})
endforeach()

