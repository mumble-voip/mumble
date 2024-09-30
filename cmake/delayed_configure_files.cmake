# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# This function allows to delay calls to configure_file to the post-build phase. This can become necessary, if one wants to
# use the result of generator expressions for the substitutions performed via configure_file. Generator expressions are only
# evaluated at build time, while configure_file runs at configure time. To work around this issue, this function will create
# an auxiliary target that generates a new cmake script at build time and this cmake script in turn will call configure_file.
# The variables set in that cmake script are passed as arguments to this function. Since the auxiliary script is generated
# at build time, generator expressions can be used in the variable definitions.
#
# Parameters:
# PYTHON_INTERPRETER <path>       Path to a working Python interpreter. Mandatory argument.
# FILES <inFile=outFile> ...      Set of files to configure as key-value-pairs (in-file=out-file). Mandatory argument.
# TARGET <target>                 If given, it is made sure that <target> is fully built, before the configure_file action happens
# SCRIPT_NAME <name>              The name of the auxiliary cmake script. If not provided, a default name is generated
# PASSTHROUGH_VARIABLES <var> ... Set of existing variables to pass-through to the generated script so that they can be used
#                                 in the configured file(s).
# VARIABLES <name=value> ...      Set of additional variables defined as key-value-pairs. The definition of <value> may make
#                                 use of generator expressions.
# @ONLY                           Passes the @ONLY parameter to the generated configure_file invocations
function(delayed_configure_files)
	set(options "@ONLY")
	set(oneValueArgs TARGET SCRIPT_NAME PYTHON_INTERPRETER)
	set(multiValueArgs FILES PASSTHROUGH_VARIABLES VARIABLES)

	cmake_parse_arguments(DELAYED_CONFIGURE_FILES "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	if(DELAYED_CONFIGURE_FILES_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unrecognized arguments to delayed_configure_file: \"${DELAYED_CONFIGURE_FILES_UNPARSED_ARGUMENTS}\"")
	endif()
	# Check required args
	if(NOT DELAYED_CONFIGURE_FILES_FILES)
		message(FATAL_ERROR "Called delayed_configure_files without the FILES parameter")
	endif()
	if(DELAYED_CONFIGURE_FILES_TARGET)
		if(NOT TARGET ${DELAYED_CONFIGURE_FILES_TARGET})
			message(FATAL_ERROR "delayed_configure_files: Provided target \"${DELAYED_CONFIGURE_FILES_TARGET}\" is not actually a (valid) target")
		endif()
	endif()
	if(NOT DELAYED_CONFIGURE_FILES_PYTHON_INTERPRETER)
		message(FATAL_ERROR "Called delayed_configure_files without the PYTHON_INTERPRETER parameter")
	endif()


	# Generate a free target name
	set(AUX_TARGET_NUM 1)
	set(AUX_TARGET_BASENAME "delayed_configure_files_target_")
	while(TARGET "${AUX_TARGET_BASENAME}${AUX_TARGET_NUM}")
		math(EXPR AUX_TARGET_NUM "${AUX_TARGET_NUM} + 1")
	endwhile()
	set(AUX_TARGET_NAME "${AUX_TARGET_BASENAME}${AUX_TARGET_NUM}")


	# Handle defaults
	if(NOT DELAYED_CONFIGURE_FILES_SCRIPT_NAME)
		set(DELAYED_CONFIGURE_FILES_SCRIPT_NAME "configure_files_${AUX_TARGET_NUM}.cmake")
	endif()

	if(DELAYED_CONFIGURE_FILES_@ONLY)
		set(AT_ONLY "--at-only")
	else()
		set(AT_ONLY "")
	endif()

	
	if(DELAYED_CONFIGURE_FILES_VARIABLES)
		set(VARIABLES ${DELAYED_CONFIGURE_FILES_VARIABLES})
	else()
		set(VARIABLES "")
	endif()
	if(DELAYED_CONFIGURE_FILES_PASSTHROUGH_VARIABLES)
		foreach(CURRENT_VAR IN LISTS DELAYED_CONFIGURE_FILES_PASSTHROUGH_VARIABLES)
			if(NOT DEFINED ${CURRENT_VAR})
				message(FATAL_ERROR "delayed_configure_files: Can't passthrough non-existing variable \"${CURRENT_VAR}\"")
			endif()

			list(APPEND VARIABLES "${CURRENT_VAR}=${${CURRENT_VAR}}")
		endforeach()
	endif()

	if(NOT VARIABLES STREQUAL "")
		set(VARIABLES --variables ${VARIABLES})
	endif()



	# Create a command to generate an intermediary cmake script that will take care of configuring the respective
	# files, once executed
	add_custom_command(
		OUTPUT "${DELAYED_CONFIGURE_FILES_SCRIPT_NAME}"
		COMMAND "${DELAYED_CONFIGURE_FILES_PYTHON_INTERPRETER}"
		ARGS "${CMAKE_SOURCE_DIR}/scripts/generate_configure_cmake_script.py"
			--files ${DELAYED_CONFIGURE_FILES_FILES} ${VARIABLES}
			--output "${DELAYED_CONFIGURE_FILES_SCRIPT_NAME}"
			${AT_ONLY}
			COMMENT "Generating delayed configure script ${DELAYED_CONFIGURE_FILES_SCRIPT_NAME}"
	)

	# Create a target that will execute the generated cmake script. Make sure that this target will be
	# added to the default build target, ensuring that it is always executed.
	add_custom_target(
		${AUX_TARGET_NAME} ALL
		DEPENDS "${DELAYED_CONFIGURE_FILES_SCRIPT_NAME}"
		COMMAND "${CMAKE_COMMAND}" -P "${DELAYED_CONFIGURE_FILES_SCRIPT_NAME}"
		COMMENT "Executing delayed configure script ${DELAYED_CONFIGURE_FILES_SCRIPT_NAME}"
	)

	if(DELAYED_CONFIGURE_FILES_TARGET)
		# Make our auxiliary target depend on the provided main target to ensure that the main target is always built
		# before our auxiliary target executes.
		add_dependencies(${AUX_TARGET_NAME} ${DELAYED_CONFIGURE_FILES_TARGET})
	endif()
endfunction()
