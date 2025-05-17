# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

option(bundled-soci "Build the included version of SOCI instead of looking for one on the system" ON)

option(enable-sqlite "Whether or not to enable the SQLite database backend" ON)
option(enable-mysql "Whether or not to enable the MySQL database backend" ${server})
option(enable-postgresql "Whether or not to enable the PostgreSQL database backend" ${server})

add_library(mumble_database STATIC
	"Backend.cpp"
	"Column.cpp"
	"Constraint.cpp"
	"ConversionUtils.cpp"
	"DataType.cpp"
	"ForeignKey.cpp"
	"Database.cpp"
	"Index.cpp"
	"MetaTable.cpp"
	"MySQLConnectionParameter.cpp"
	"PostgreSQLConnectionParameter.cpp"
	"PrimaryKey.cpp"
	"Savepoint.cpp"
	"SQLiteConnectionParameter.cpp"
	"Table.cpp"
	"TransactionHolder.cpp"
	"Trigger.cpp"
	"Utils.cpp"
	"Version.cpp"
)

target_include_directories(mumble_database PUBLIC "${CMAKE_SOURCE_DIR}/src")

if (NOT enable-sqlite)
	target_compile_definitions(mumble_database PUBLIC MUMBLE_DISABLE_SQLITE)
endif()
if (NOT enable-mysql)
	target_compile_definitions(mumble_database PUBLIC MUMBLE_DISABLE_MYSQL)
endif()
if (NOT enable-postgresql)
	target_compile_definitions(mumble_database PUBLIC MUMBLE_DISABLE_POSTGRESQL)
endif()

if (bundled-soci)
	# Include SOCI, but hard-code a few options
	set(SOCI_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/soci)

	set(SOCI_SHARED OFF CACHE BOOL "" FORCE)
	set(SOCI_LTO ${lto} CACHE BOOL "" FORCE)
	set(SOCI_DB2 OFF CACHE BOOL "" FORCE)
	set(SOCI_FIREBIRD OFF CACHE BOOL "" FORCE)
	set(SOCI_ODBC OFF CACHE BOOL "" FORCE)
	set(SOCI_ORACLE OFF CACHE BOOL "" FORCE)
	set(SOCI_EMPTY OFF CACHE BOOL "" FORCE)
	set(SOCI_MYSQL ${enable-mysql} CACHE BOOL "" FORCE)
	set(SOCI_POSTGRESQL ${enable-postgresql} CACHE BOOL "" FORCE)
	set(SOCI_SQLITE3 ${enable-sqlite} CACHE BOOL "" FORCE)

	add_subdirectory("${3RDPARTY_DIR}/soci" "${SOCI_BINARY_DIR}" EXCLUDE_FROM_ALL)

	disable_warnings_for_all_targets_in("${3RDPARTY_DIR}/soci")
else()
	# Only Soci 4.1 introduces the general BLOB operations that we require
	set(components "")
	if (enable-sqlite)
		list(APPEND components "SQLite3")
	endif()
	if (enable-mysql)
		list(APPEND components "MySQL")
	endif()
	if (enable-postgresql)
		list(APPEND components "PostgreSQL")
	endif()

	find_pkg("Soci" 4.1.0 COMPONENTS ${components} REQUIRED)
endif()

target_link_libraries(mumble_database PUBLIC SOCI::soci)


if (NOT TARGET nlohmann_json)
	if(bundled-json)
		set(JSON_BuildTests OFF CACHE INTERNAL "")
		set(JSON_ImplicitConversions OFF CACHE INTERNAL "")
		add_subdirectory("${3RDPARTY_DIR}/nlohmann_json/" "${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json/" EXCLUDE_FROM_ALL)
	else()
		find_pkg("nlohmann_json" REQUIRED)
	endif()
endif()

target_link_libraries(mumble_database PUBLIC nlohmann_json)
