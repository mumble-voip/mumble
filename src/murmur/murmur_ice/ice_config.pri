# Copyright 2005-2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

win32:CONFIG(debug, debug|release) {
	LIBS *= -lIceD -lIceUtilD
} else {
	LIBS *= -lIce -lIceUtil
}

win32 {
	INCLUDEPATH *= "$$ICE_PATH/include"
	!CONFIG(static) {
		QMAKE_LIBDIR *= "$$ICE_PATH/lib/vc100"
	} else {
		DEFINES *= ICE_STATIC_LIBS
		QMAKE_LIBDIR *= $$BZIP2_PATH/lib
		equals(MUMBLE_ARCH, x86) {
			QMAKE_LIBDIR *= $$ICE_PATH/lib
		}
		equals(MUMBLE_ARCH, x86_64) {
			QMAKE_LIBDIR *= $$ICE_PATH/lib/x64
		}
		CONFIG(release, debug|release): LIBS *= -llibbz2
		CONFIG(debug, debug|release):   LIBS *= -llibbz2d
		LIBS *= -ldbghelp -liphlpapi -lrpcrt4
	}
}

macx {
	INCLUDEPATH *= $$(MUMBLE_PREFIX)/Ice-3.4.2/include/
	QMAKE_LIBDIR *= $$(MUMBLE_PREFIX)/Ice-3.4.2/lib/
}

CONFIG(ermine) {
	INCLUDEPATH *= $$(MUMBLE_ICE_PREFIX)/include/
	QMAKE_LIBDIR *= $$(MUMBLE_ICE_PREFIX)/lib/
}

unix:!macx:CONFIG(static) {
	INCLUDEPATH *= /opt/Ice-3.3/include
	QMAKE_LIBDIR *= /opt/Ice-3.3/lib
	LIBS *= -lbz2
	QMAKE_CXXFLAGS *= -fPIC
}

macx:CONFIG(static) {
	LIBS *= -lbz2 -liconv
	QMAKE_CXXFLAGS *= -fPIC
}
