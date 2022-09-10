# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

function(try_compile_snippet)
	set(options)
	set(oneValueArgs RESULT_VAR LANG)
	set(multiValueArgs INCLUDES INCLUDE_HEADERS STATEMENTS COMPILE_DEFINITIONS LINK_OPTIONS LINK_LIBRARIES OUTPUT_VARIABLE CMAKE_FLAGS)

	cmake_parse_arguments(COMPILE_SNIPPET "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if(COMPILE_SNIPPET_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "try_compile_snippet: Unrecognized arguments: ${COMPILE_SNIPPET_UNPARSED_ARGUMENTS}")
	endif()

	set(REQUIRED_ARGS ${oneValueArgs} STATEMENTS)
	foreach(CURRENT IN LISTS REQUIRED_ARGS)
		if(NOT COMPILE_SNIPPET_${CURRENT})
			message(FATAL_ERROR "try_compile_snippet: Missing argument ${CURRENT}")
		endif()
	endforeach()

	list(JOIN COMPILE_SNIPPET_INCLUDES "\n" INCLUDE_STATEMENTS)
	string(APPEND INCLUDE_STATEMENTS "\n")
	foreach(CURRENT IN LISTS COMPILE_SNIPPET_INCLUDE_HEADERS)
		string(APPEND INCLUDE_STATEMENTS "\n#include <${CURRENT}>")
	endforeach()

	list(JOIN COMPILE_SNIPPET_STATEMENTS ";\n" JOINED_STATEMENTS)
	set(SOURCE_CODE "\
		${INCLUDE_STATEMENTS}\n\
		\n\
		int main() {\n\
			${JOINED_STATEMENTS};\n\
			\n\
			return 0;\n\
		}"
	)

	string(TOLOWER "${COMPILE_SNIPPET_LANG}" COMPILE_SNIPPET_LANG)

	if("${COMPILE_SNIPPET_LANG}" MATCHES "^(cxx|cpp|c\\+\\+)$")
		set(FILE_EXTENSION "cpp")
	elseif("${COMPILE_SNIPPET_LANG}" MATCHES "^(c)$")
		set(FILE_EXTENSION "c")
	else()
		message(FATAL_ERROR "try_compile_snippet: Unsupported language ${COMPILE_SNIPPET_LANG}")
	endif()
	
	set(FILENAME "${CMAKE_CURRENT_BINARY_DIR}/try_compile_snippet.${FILE_EXTENSION}")

	file(WRITE "${FILENAME}" "${SOURCE_CODE}")

	try_compile(COMPILE_SUCCESS "${CMAKE_CURRENT_BINARY_DIR}"
		SOURCES "${FILENAME}"
		CMAKE_FLAGS ${COMPILE_SNIPPET_CMAKE_FLAGS}
		COMPILE_DEFINITIONS ${COMPILE_SNIPPET_COMPILE_DEFINITIONS}
		LINK_OPTIONS ${COMPILE_SNIPPET_LINK_OPTIONS}
		LINK_LIBRARIES ${COMPILE_SNIPPET_LINK_LIBRARIES}
		OUTPUT_VARIABLE BUILD_OUTPUT
	)

	set(${COMPILE_SNIPPET_RESULT_VAR} ${COMPILE_SUCCESS} PARENT_SCOPE)
	if(COMPILE_SNIPPET_OUTPUT_VARIABLE)
		set(${COMPILE_SNIPPET_OUTPUT_VARIABLE} ${BUILD_OUTPUT} PARENT_SCOPE)
	endif()
endfunction()
