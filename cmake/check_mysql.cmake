# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

function(check_mysql_connectivity)
	set(options REQUIRED)
	set(oneValueArgs USERNAME PASSWORD)
	set(multiValueArgs "")
	cmake_parse_arguments(MYSQL_CONNECTIVITY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	find_program(MYSQL_CLI "mysql" MYSQL_CONNECTIVITY_REQUIRED)
endfunction()
