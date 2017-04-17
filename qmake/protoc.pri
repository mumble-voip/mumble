# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# This .pri files finds a suitable protoc binary on the system.
#
# It exposes the found protoc binary as the 'PROTOC' qmake variable.
# This variable contains the absolute path to a working protoc binary.
#
# Why do we need to do this?
#
# The protobuf compiler's binary name in MinGW packages for Linux
# cross-compiling is never 'protoc', but something like
# 'x86_64-w64-mingw32.static-protoc'.
#
# This file tries to use the right binary, by searching in $PATH
# using the 'which' tool.
#
# If no binary is found, an error is shown to the user.
#
# This script also supports setting a MUMBLE_PROTOC environment
# variable to specify an absolute path to a protoc binary to use.
# Setting this environment variable overrides any querying.

include(qt.pri)

PROTOC = $$(MUMBLE_PROTOC)

# If the MUMBLE_PROTOC environment variable isn't set
isEmpty(PROTOC) {
	# If we're on Qt 4 on win32, or we're building from Windows, use "protoc"
	equals(QMAKE_HOST.os, "Windows")|equals(QMAKE_VERSION_INT_MAJOR, 4):win32 {
		PROTOC=protoc
	} else {
		# Determine if the 'which' tool is available. We need it
		# to determine which PROTOC binary on the system to use.
		# If 'which' isn't available, error out.
		# Calling 'which which' is intentional.
		# If which is available, it will print the path to it.
		# If it isn't available, it will not print anything.
		WHICH=$$system(which which 2>/dev/null)
		isEmpty(WHICH) {
			message("The 'which' command is not available on the system. Unable to search for protoc installations. Assuming 'protoc'... If this is not correct, please point the MUMBLE_PROTOC environment variable at a working protoc binary.")
			PROTOC=protoc
		} else {
			TARGET_TRIPLE = $$system($${QMAKE_CC} -dumpmachine 2>/dev/null)
			!isEmpty(TARGET_TRIPLE) {
				isEmpty(PROTOC) {
					PROTOC=$$system(which $$TARGET_TRIPLE"-protoc" 2>/dev/null)
				}
				isEmpty(PROTOC) {
					PROTOC=$$system(which $$TARGET_TRIPLE".static-protoc" 2>/dev/null)
				}
				isEmpty(PROTOC) {
					PROTOC=$$system(which $$TARGET_TRIPLE".shared-protoc" 2>/dev/null)
				}
			}
			isEmpty(PROTOC) {
				PROTOC=$$system(which protoc 2>/dev/null)
			}
			isEmpty(PROTOC) {
				error("Unable to find system's protoc binary. Some scripts invoked during the Mumble build use the protobuf compiler. You can manually specify it via the MUMBLE_PROTOC environment variable.")
			}
		}
	}
}
