# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_executable(TestServerDatabase
	ServerDatabaseTest.cpp
	JSONAssembler.cpp

	"${CMAKE_CURRENT_SOURCE_DIR}/../TestUtils.cpp"
)

target_compile_definitions(TestServerDatabase PRIVATE ${MUMBLE_DB_TEST_DEFINES})

set_target_properties(TestServerDatabase PROPERTIES AUTOMOC ON)

target_link_libraries(TestServerDatabase PRIVATE Qt6::Test mumble_server_object_lib shared)

# Add parent directory to include path to be able to share helper functions with general DB tests
target_include_directories(TestServerDatabase PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/..")

add_test(NAME TestServerDatabase COMMAND $<TARGET_FILE:TestServerDatabase>)

set(input_table_data_paths)
set(migrated_table_data_paths)

macro(add_table_pair input migrated)
	set(input_path "${CMAKE_CURRENT_SOURCE_DIR}/table_data/${input}")
	set(migrated_path "${CMAKE_CURRENT_SOURCE_DIR}/table_data/${migrated}")

	if(NOT EXISTS "${input_path}")
		message(FATAL_ERROR "Can't find JSON table definition at '${input_path}'")
	endif()
	if(NOT EXISTS "${migrated_path}")
		message(FATAL_ERROR "Can't find JSON table definition at '${migrated_path}'")
	endif()

	list(APPEND input_table_data_paths "${input_path}")
	list(APPEND migrated_table_data_paths "${migrated_path}")
endmacro()

add_table_pair("meta_table_input.json" "meta_table_migrated.json")
add_table_pair("server_table_input.json" "server_table_migrated.json")
add_table_pair("log_table_input.json" "log_table_migrated.json")
add_table_pair("config_table_input.json" "config_table_migrated.json")
add_table_pair("channel_table_input.json" "channel_table_migrated.json")
add_table_pair("channel_property_table_input.json" "channel_property_table_migrated.json")
add_table_pair("user_table_input.json" "user_table_migrated.json")
add_table_pair("user_property_table_input.json" "user_property_table_migrated.json")
add_table_pair("group_table_input.json" "group_table_migrated.json")
add_table_pair("group_member_table_input.json" "group_member_table_migrated.json")
add_table_pair("acl_table_input.json" "acl_table_migrated.json")
add_table_pair("channel_link_table_input.json" "channel_link_table_migrated.json")
add_table_pair("ban_table_input.json" "ban_table_migrated.json")
add_table_pair("channel_listener_table_input.json" "channel_listener_table_migrated.json")

list(LENGTH input_table_data_paths max_idx)
math(EXPR max_idx "${max_idx} - 1")

set(JSON_TABLE_PAIRS "")
foreach(current_idx RANGE ${max_idx})
	list(GET input_table_data_paths ${current_idx} current_input_path)
	list(GET migrated_table_data_paths ${current_idx} current_migrated_path)

	set(JSON_TABLE_PAIRS "${JSON_TABLE_PAIRS}{ \"${current_input_path}\", \"${current_migrated_path}\" },\n")
endforeach()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/JSONLocator.cpp.in" "${CMAKE_CURRENT_BINARY_DIR}/JSONLocator.cpp")
target_sources(TestServerDatabase PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/JSONLocator.cpp")
target_include_directories(TestServerDatabase PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
