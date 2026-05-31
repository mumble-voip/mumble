# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

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
target_set_mumble_compiler_flags(mumble_database)

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

# In case find_package gets resolved via FetchContent, set some SOCI build options
set(SOCI_DB2 OFF)
set(SOCI_EMPTY OFF)
set(SOCI_FIREBIRD OFF)
set(SOCI_MYSQL ${enable-mysql})
set(SOCI_ODBC OFF)
set(SOCI_ORACLE OFF)
set(SOCI_POSTGRESQL ${enable-postgresql})
set(SOCI_SQLITE3 ${enable-sqlite})

find_package("Soci" 4.2.0 COMPONENTS ${components} CONFIG REQUIRED)

target_link_libraries(mumble_database PUBLIC SOCI::SOCI)


find_package("nlohmann_json" REQUIRED)

target_link_libraries(mumble_database PUBLIC nlohmann_json)
