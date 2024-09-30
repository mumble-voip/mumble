# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

find_pkg(Qt6 COMPONENTS Widgets REQUIRED)

# kpCertificate: We can't create a value for that on-the-fly, so we have to exclude it from the test
# All other: These settings are not saved
set(IGNORED_FIELDS
	"kpCertificate,bSuppressIdentity,lmLoopMode,dPacketLoss,dMaxPacketDelay,requireRestartToApply,settingsLocation,createdSettingsBackup")

include(FindPythonInterpreter)

set(PYTHON_HINTS
	"C:/Python39-x64" # Path on the AppVeyor CI server
)

find_python_interpreter(
	VERSION 3
	INTERPRETER_OUT_VAR PYTHON_INTERPRETER
	HINTS ${PYTHON_HINTS}
	REQUIRED
)

set(TEST_CASE_SOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/TestSettingsJSONSerialization.cpp")
set(SETTINGS_HEADER "${CMAKE_SOURCE_DIR}/src/mumble/Settings.h")

if(NOT EXISTS "${SETTINGS_HEADER}")
	message(FATAL_ERROR "Unable to locate Settings header at \"${SETTINGS_HEADER}\"")
endif()

# Generate the test-case's source file
add_custom_command(
	OUTPUT "${TEST_CASE_SOURCE_FILE}"
	COMMAND "${PYTHON_INTERPRETER}" "${CMAKE_CURRENT_SOURCE_DIR}/generate_test_case.py" --settings-header "${SETTINGS_HEADER}"
		--settings-struct-nam Settings --ignore-fields "${IGNORED_FIELDS}" --output-file "${TEST_CASE_SOURCE_FILE}"
	COMMENT "Generating test-case for 'SettingsJSONSerialization'"
	DEPENDS "generate_test_case.py" "${CMAKE_SOURCE_DIR}/src/mumble/Settings.h"
		"${CMAKE_SOURCE_DIR}/src/mumble/JSONSerialization.h"
)

add_executable(TestSettingsJSONSerialization "${TEST_CASE_SOURCE_FILE}")
target_link_libraries(TestSettingsJSONSerialization PRIVATE mumble_client_object_lib)
target_link_libraries(TestSettingsJSONSerialization PRIVATE Qt6::Test)

set_target_properties(TestSettingsJSONSerialization PROPERTIES AUTOMOC ON)


add_test(
	NAME TestSettingsJSONSerialization
	COMMAND $<TARGET_FILE:TestSettingsJSONSerialization>
	# Specifying the working directory is necessary, to make sure the dependent DLLs are found (on Windows)
	WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
)
