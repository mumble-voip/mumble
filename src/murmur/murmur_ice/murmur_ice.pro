# Copyright 2005-2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../../../compiler.pri)

# Ice 3.6 C++11 check
!CONFIG(no-ice-cpp11-check):unix {
	include(ice_config.pri)

	# TODO:
	# strip -std=c++XX from QMAKE_CXXFLAGS
	# include INCLUDEPATHS and QMAKE_LIBS in the compiler flags
	# ... this is pretty tough to do inside a .pro file, since
	# qmake has not expanded everything yet :(
	OUTPUT=$$system($${QMAKE_CXX} ${QMAKE_CXXFLAGS} ${QMAKE_LFLAGS} -lIce -lIceUtil -std=c++11 Ice36Cpp11Test.cpp -o /dev/null 2>&1)
	!contains($$OUTPUT, "error:") {
		CONFIG += ice-cpp11
	}
	CONFIG(ice-cpp11) {
		# XXX: correct?
		!CONFIG(c++11)|!CONFIG(c++14)|!CONFIG(c++1z) {
			error(Ice libraries on the system require C++11)
		}
	}
}

SLICEFILES = ../Murmur.ice

slice.output = ${QMAKE_FILE_BASE}.cpp
win32 {
	slice.commands = slice2cpp --checksum -I\"$$ICE_PATH/slice\" ${QMAKE_FILE_NAME}
} else {
	ICE_VERSION = $$system(slice2cpp --version 2>&1)
	EXTRA_SLICEFLAGS =
	!isEmpty(ICE_VERSION) {
		EXTRA_SLICEFLAGS = -I/usr/share/Ice-$$ICE_VERSION/slice/
	}
	slice.commands = slice2cpp --checksum -I/usr/local/share/Ice -I/usr/share/Ice/slice -I/usr/share/slice $$EXTRA_SLICEFLAGS ${QMAKE_FILE_NAME}
}
slice.input = SLICEFILES
slice.CONFIG *= no_link explicit_dependencies
slice.variable_out = SOURCES

sliceh.output = ${QMAKE_FILE_BASE}.h
sliceh.depends = ${QMAKE_FILE_BASE}.cpp
sliceh.commands = $$escape_expand(\\n)
sliceh.input = SLICEFILES
sliceh.CONFIG *= no_link explicit_dependencies target_predeps

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
CONFIG += staticlib

QMAKE_EXTRA_COMPILERS *= slice sliceh

win32 {
	INCLUDEPATH *= "$$ICE_PATH/include"
	CONFIG(static) {
		DEFINES *= ICE_STATIC_LIBS
	}
}

macx {
	INCLUDEPATH *= $$(MUMBLE_PREFIX)/Ice-3.4.2/include/
	slice.commands = $$(MUMBLE_PREFIX)/Ice-3.4.2/bin/slice2cpp --checksum -I$$(MUMBLE_PREFIX)/Ice-3.4.2/slice/ ../Murmur.ice
}

CONFIG(ermine) {
	INCLUDEPATH *= $$(MUMBLE_ICE_PREFIX)/include/
	slice.commands = $$(MUMBLE_ICE_PREFIX)/bin/slice2cpp --checksum -I$$(MUMBLE_ICE_PREFIX)/slice/ ../Murmur.ice
}

unix:!macx:CONFIG(static) {
	INCLUDEPATH *= /opt/Ice-3.3/include
	slice.commands = /opt/Ice-3.3/bin/slice2cpp --checksum -I/opt/Ice-3.3/slice ../Murmur.ice
}

macx:CONFIG(static) {
	LIBS *= -lbz2 -liconv
	QMAKE_CXXFLAGS *= -fPIC
}

!CONFIG(third-party-warnings) {
    # We ignore warnings in third party builds. We won't actually look
    # at them and they clutter out our warnings.
    CONFIG -= warn_on
    CONFIG += warn_off
}

CONFIG(debug, debug|release) {
	DESTDIR = ../../../debug
}

CONFIG(release, debug|release) {
	DESTDIR = ../../../release
}

include(../../../symbols.pri)
