# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# The runtime directory and fork-based isolation tests exercise the POSIX implementation.
if(UNIX)
	add_executable(TestIPCUtils TestIPCUtils.cpp TestIPCUtilsC.c)
	target_link_libraries(TestIPCUtils PRIVATE ipcutils)
	add_test(NAME TestIPCUtils COMMAND $<TARGET_FILE:TestIPCUtils>)
endif()
