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

FetchContent_Declare(CLI11
	GIT_TAG                 "v2.6.2"
	GIT_REPOSITORY          "https://github.com/CLIUtils/CLI11.git"
	GIT_SHALLOW
	SYSTEM
	EXCLUDE_FROM_ALL
)

mumble_register_dependency(CLI11
	MAIN_TARGET     "CLI11::CLI11"
	FETCHCONTENT_ID "CLI11"
)

mumble_register_dependency(sndfile
	MAIN_TARGET "sndfile::sndfile"
)

mumble_register_dependency(Opus
	MAIN_TARGET "Opus::opus"
)
