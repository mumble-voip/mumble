# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(mumble_server_database STATIC
	"ServerDatabase.cpp"
	"DBChannel.cpp"
)

set(TABLE_IMPLEMENTATIONS
	"ServerTable.cpp"
	"LogTable.cpp"
	"ConfigTable.cpp"
	"ChannelTable.cpp"
)

foreach(CURRENT_TABLE IN LISTS TABLE_IMPLEMENTATIONS)
	# As there are many duplicates in the used column name variables that all live in the
	# same namespace in the cpp files, we have to avoid them being batched together.
	set_source_files_properties("${CURRENT_TABLE}" PROPERTIES SKIP_UNITY_BUILD_INCLUSION TRUE)

	target_sources(mumble_server_database PRIVATE "${CURRENT_TABLE}")
endforeach()

set_property(TARGET mumble_server_database PROPERTY INTERPROCEDURAL_OPTIMIZATION ${lto})

target_link_libraries(mumble_server_database PUBLIC mumble_database)

target_include_directories(mumble_server_database
	PUBLIC
		"${SHARED_SOURCE_DIR}"
		"${CMAKE_SOURCE_DIR}/src/murmur"
)
