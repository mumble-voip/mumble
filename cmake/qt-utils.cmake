# Copyright 2020-2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

find_package(Python3 COMPONENTS Interpreter REQUIRED)

function(include_qt_plugin TARGET SCOPE PLUGIN)
	set(PATH "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_plugin_import.cpp")
	if(NOT EXISTS ${PATH})
		file(WRITE ${PATH} "#include <QtPlugin>\n")
		set_property(SOURCE ${PATH} PROPERTY GENERATED TRUE)
		set_property(SOURCE ${PATH} PROPERTY SKIP_AUTOGEN TRUE)
	else()
		file(READ ${PATH} CONTENT)
		string(FIND ${CONTENT} ${PLUGIN} INDEX)
		if(NOT ${INDEX} EQUAL -1)
			set(FOUND TRUE)
		endif()
	endif()

	# We have to exclude these files from unity builds since we have one of that for the client
	# and one for the server. If they happen to get included in the same build or just with some
	# other code that happens to include one of the plugins statically, then there'll be errors.
	# We set the property on existing files as well in order to ensure that it is set even
	# if the file existed already (and setting the property multiple times should not hurt).
	set_property(SOURCE ${PATH} PROPERTY SKIP_UNITY_BUILD_INCLUSION TRUE)

	if(NOT FOUND)
		file(APPEND ${PATH} "Q_IMPORT_PLUGIN(${PLUGIN})\n")
	endif()

	get_property(TARGET_SOURCES TARGET ${TARGET} PROPERTY SOURCES)
	if(NOT ${PATH} IN_LIST TARGET_SOURCES)
		target_sources(${TARGET} ${SCOPE} ${PATH})
	endif()
endfunction()

function(compile_translations OUT_VAR OUT_DIR TS_FILES)
	find_pkg(Qt5 COMPONENTS LinguistTools REQUIRED)

	# Create output directory
	file(MAKE_DIRECTORY "${OUT_DIR}")

	set(COMPILED_FILES "")

	# Compile the given .ts files into .qm files into the output directory
	foreach(CURRENT_TS IN LISTS TS_FILES)
		set_source_files_properties("${CURRENT_TS}" PROPERTIES OUTPUT_LOCATION "${OUT_DIR}")
		qt5_add_translation(COMPILED_FILES "${CURRENT_TS}")
	endforeach()

	# return the list of compiled .qm files
	set("${OUT_VAR}" "${COMPILED_FILES}" PARENT_SCOPE)
endfunction()


function(include_translations OUT_VAR OUT_DIR TS_FILES)
	compile_translations(QM_FILES "${OUT_DIR}" "${TS_FILES}")

	set(QRC_PATH "${OUT_DIR}/mumble_translations.qrc")

	# Create a resource file (.qrc) containing the name of the compiled translation files (.qm).
	# This is required in order to embed those files into the built executable.
	# NOTE: We write the files' name instead of their path because they are in the same directory as the resource file.
	file(WRITE ${QRC_PATH} "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n")

	foreach(QM_FILE ${QM_FILES})
		get_filename_component(FILENAME ${QM_FILE} NAME)
		file(APPEND ${QRC_PATH} "\t<file>${FILENAME}</file>\n")
	endforeach()

	file(APPEND ${QRC_PATH} "</qresource>\n</RCC>\n")

	set("${OUT_VAR}" "${QRC_PATH}" PARENT_SCOPE)
endfunction()

function(query_qmake OUT_VAR PROP)
	# Get the path to the qmake executable
	get_target_property(QT_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)

	# Query qmake for the location of the installed Qt translations
	execute_process(
		COMMAND ${QT_QMAKE_EXECUTABLE} -query "${PROP}"
		OUTPUT_VARIABLE QUERY_RESULT
	)

	string(STRIP "${QUERY_RESULT}" QUERY_RESULT)

	set("${OUT_VAR}" "${QUERY_RESULT}" PARENT_SCOPE)
endfunction()

function(bundle_qt_translations TARGET)
	query_qmake(QT_TRANSLATIONS_DIRECTORY "QT_INSTALL_TRANSLATIONS")
	string(STRIP "${QT_TRANSLATIONS_DIRECTORY}" QT_TRANSLATIONS_DIRECTORY)

	message(STATUS "Bundling Qt translations from \"${QT_TRANSLATIONS_DIRECTORY}\"")

	# Compile our version of Qt translations
	set(QT_TRANSLATION_OVERWRITE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src/mumble/qttranslations")
	set(QT_TRANSLATION_OVERWRITE_DIR "${CMAKE_CURRENT_BINARY_DIR}/qttranslations")
	file(GLOB TS_FILES "${QT_TRANSLATION_OVERWRITE_SOURCE_DIR}/*.ts")
	compile_translations(QM_FILES "${QT_TRANSLATION_OVERWRITE_DIR}" "${TS_FILES}")

	set(GENERATED_QRC_FILE "${CMAKE_CURRENT_BINARY_DIR}/mumble_qt_translations.qrc")

	# Copy conf file to build dir for the Python script to find it
	file(COPY "${QT_TRANSLATION_OVERWRITE_SOURCE_DIR}/translations.conf"
		DESTINATION "${QT_TRANSLATION_OVERWRITE_DIR}")

	# Generate the QRC file that contains the Qt translations and potentially our overwrites of them
	execute_process(
		COMMAND "${Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/scripts/generate-mumble_qt-qrc.py"
			"${GENERATED_QRC_FILE}" "${QT_TRANSLATIONS_DIRECTORY}" "${QT_TRANSLATION_OVERWRITE_DIR}"
		RESULT_VARIABLE GENERATOR_EXIT_CODE
	)

	if(NOT GENERATOR_EXIT_CODE EQUAL 0)
		message(FATAL_ERROR "generate-mumble_qt-qrc.py returned exit code ${GENERATOR_EXIT_CODE}")
	endif()

	# Add the generated QRC file to the given target
	target_sources(${TARGET} PRIVATE "${GENERATED_QRC_FILE}")
endfunction()
