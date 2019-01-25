# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(pkgconfig.pri)

# Include this file to portably link
# against protobuf.
#
# It requires compiler.pri to be included
# for $$PROTOBUF_PATH, etc. to work on win32.
# This file can't include compiler.pri: it can
# only be included once per .pro file.

win32-msvc* {
	# Legacy win32-static paths (protobuf 2.x)
	INCLUDEPATH *= "$$PROTOBUF_PATH/vsprojects/include" "$$PROTOBUF_PATH/src" protobuf
	CONFIG(debug, debug|release) {
		QMAKE_LIBDIR *= "$$PROTOBUF_PATH/vsprojects/Debug"
	} else {
		QMAKE_LIBDIR *= "$$PROTOBUF_PATH/vsprojects/Release"
	}

	# New win32-static paths (protobuf 3.2+)
	INCLUDEPATH *= "$$PROTOBUF_PATH/include"
	QMAKE_LIBDIR *= "$$PROTOBUF_PATH/lib"

	LIBS *= -llibprotobuf
}

win32-g++ {
	LIBS *= -lprotobuf
}

unix {
	CONFIG *= link_pkgconfig
	must_pkgconfig(protobuf)
}
