# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(pkgconfig.pri)

# Include this file to portably link
# against OpenSSL.
#
# It requires compiler.pri to be included
# for $$OPENSSL_PATH, etc. to work on win32.
# This file can't include compiler.pri: it can
# only be included once per .pro file.

win32-msvc* {
	INCLUDEPATH *= "$$OPENSSL_PATH/include"
	QMAKE_LIBDIR *= "$$OPENSSL_PATH/lib"
	LIBS *= -lgdi32 -llibeay32
}

win32-g++ {
	LIBS *= -lssl -lcrypto -lgdi32
}

unix {
	contains(UNAME, FreeBSD) {
		LIBS *= -lcrypto -lssl
	} else {
		CONFIG *= link_pkgconfig
		must_pkgconfig(openssl)
	}
}
