# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

option(database-mysql-test "Whether to include the MySQL database tests (requires special setup)" OFF)
option(database-postgresql-tests "Whether to include the PostgreSQL database tests (requires special setup)" OFF)
 
add_executable(TestDatabase
	DatabaseTest.cpp
	DefaultTable.cpp
	TestUtils.cpp
	KeyValueTable.cpp
	ConstraintTable.cpp
)

if(database-mysql-tests)
	target_compile_definitions(TestDatabase PRIVATE MUMBLE_TEST_MYSQL)
endif()
if(database-postgresql-tests)
	target_compile_definitions(TestDatabase PRIVATE MUMBLE_TEST_POSTGRESQL)
endif()

set_target_properties(TestDatabase PROPERTIES AUTOMOC ON)

target_link_libraries(TestDatabase PRIVATE Qt5::Test mumble_database)

add_test(NAME TestDatabase COMMAND $<TARGET_FILE:TestDatabase>)
