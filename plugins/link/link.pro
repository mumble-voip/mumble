# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../plugins.pri)

TARGET		= link

win32 {
	SOURCES		= link.cpp
	LIBS		+= -luser32
}

unix {
	SOURCES		= link-posix.cpp

        linux* {
		LIBS          += -lrt
	}
}
