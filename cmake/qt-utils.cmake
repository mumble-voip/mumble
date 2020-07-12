# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

function(include_qt_plugin TARGET SCOPE PLUGIN)
	set(PATH "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}_plugin_import.cpp")
	if(NOT EXISTS ${PATH})
		file(WRITE ${PATH} "#include <QtPlugin>\n")
		set_property(SOURCE ${PATH} PROPERTY GENERATED TRUE SKIP_AUTOGEN TRUE)
	else()
		file(READ ${PATH} CONTENT)
		string(FIND ${CONTENT} ${PLUGIN} INDEX)
		if(NOT ${INDEX} EQUAL -1)
			set(FOUND TRUE)
		endif()
	endif()

	if(NOT FOUND)
		file(APPEND ${PATH} "Q_IMPORT_PLUGIN(${PLUGIN})\n")
	endif()

	get_property(TARGET_SOURCES TARGET ${TARGET} PROPERTY SOURCES)
	if(NOT ${PATH} IN_LIST TARGET_SOURCES)
		target_sources(${TARGET} ${SCOPE} ${PATH})
	endif()
endfunction()

function(create_translations_qrc TS_DIR TS_FILES QRC_FILENAME)
	find_pkg(Qt5 COMPONENTS LinguistTools REQUIRED)

	# Workaround for Qt bug: CMake deletes .ts files upon clean.
	# @ref https://bugreports.qt.io/browse/QTBUG-41736
	# @ref https://stackoverflow.com/a/24245615/1917249
	set_directory_properties(PROPERTIES CLEAN_NO_CUSTOM TRUE)

	# Update the translation files (e.g. add new strings) and compile them.
	qt5_create_translation(QM_FILES ${TS_DIR} ${TS_FILES})

	set(QRC_PATH "${CMAKE_CURRENT_BINARY_DIR}/${QRC_FILENAME}")

	# Create a resource file (.qrc) containing the name of the compiled translation files (.qm).
	# This is required in order to embed those files into the built executable.
	# NOTE: We write the files' name instead of their path because they are in the same directory as the resource file.
	file(WRITE ${QRC_PATH} "<!DOCTYPE RCC><RCC version=\"1.0\">\n<qresource>\n")

	foreach(QM_FILE ${QM_FILES})
		get_filename_component(FILENAME ${QM_FILE} NAME)
		file(APPEND ${QRC_PATH} "\t<file>${FILENAME}</file>\n")
	endforeach()

	file(APPEND ${QRC_PATH} "</qresource>\n</RCC>\n")
endfunction()
