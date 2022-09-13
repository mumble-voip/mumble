# Copyright 2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

add_library(mumble_server_database STATIC
	"ServerDatabase.cpp"
	"DBChannel.cpp"
	"DBUser.cpp"
	"DBUserData.cpp"

	# Table implementations
	"ServerTable.cpp"
	"LogTable.cpp"
	"ConfigTable.cpp"
	"ChannelTable.cpp"
	"ChannelPropertyTable.cpp"
	"UserTable.cpp"
)

set_property(TARGET mumble_server_database PROPERTY INTERPROCEDURAL_OPTIMIZATION ${lto})

# We require boost::optional (until we can move to CXX17 and switch to std::optional), which was
# introduced in Boost v1.30.0
find_pkg(Boost 1.30.0 REQUIRED)

target_link_libraries(mumble_server_database PUBLIC mumble_database GSL Boost::headers)

target_include_directories(mumble_server_database
	PUBLIC
		"${SHARED_SOURCE_DIR}"
)
