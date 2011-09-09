include(../mumble.pri)

DEFINES *= MUTTER
CONFIG  *= network ice
CONFIG(static) {
	QMAKE_LFLAGS *= -static
}
CONFIG	-= gui
QT *= network sql xml
QT -= gui
TARGET = mutter
LANGUAGE	= C++
FORMS =
HEADERS = 
SOURCES = main.cpp
PROTOBUF = 

DIST = ../murmur/Murmur.ice

ice {
	SLICEFILES = ../murmur/Murmur.ice

	slice.output = ${QMAKE_FILE_BASE}.cpp
	win32 {
		slice.commands = slice2cpp --checksum -I\"$$ICE_PATH/slice\" ${QMAKE_FILE_NAME}
	} else {
		slice.commands = slice2cpp --checksum -I/usr/local/share/Ice -I/usr/share/Ice/slice -I/usr/share/slice -I/usr/share/Ice-3.4.1/slice/ -I/usr/share/Ice-3.3.1/slice/ ${QMAKE_FILE_NAME}
	}
	slice.input = SLICEFILES
	slice.CONFIG *= no_link explicit_dependencies
	slice.variable_out = SOURCES

	sliceh.output = ${QMAKE_FILE_BASE}.h
	sliceh.depends = ${QMAKE_FILE_BASE}.cpp
	sliceh.commands = $$escape_expand(\\n)
	sliceh.input = SLICEFILES
	sliceh.CONFIG *= no_link explicit_dependencies target_predeps

	QMAKE_EXTRA_COMPILERS *= slice sliceh

#	SOURCES *= Murmur.cpp
#	HEADERS *= Murmur.h
	win32:CONFIG(debug, debug|release) {
		LIBS *= -lIceD -lIceUtilD
	} else {
		LIBS *= -lIce -lIceUtil
	}
	DEFINES *= USE_ICE

	win32 {
		INCLUDEPATH *= "$$ICE_PATH/include"
		QMAKE_LIBDIR *= "$$ICE_PATH/lib/vc100"
	}

	macx {
		INCLUDEPATH *= $$(MUMBLE_PREFIX)/ice-3.4.1/include/
		QMAKE_LIBDIR *= $$(MUMBLE_PREFIX)/ice-3.4.1/lib/
		slice.commands = $$(MUMBLE_PREFIX)/ice-3.4.1/bin/slice2cpp --checksum -I$$(MUMBLE_PREFIX)/ice-3.4.1/slice/ Murmur.ice
	}

	unix:CONFIG(static) {
		INCLUDEPATH *= /opt/Ice-3.3/include
		QMAKE_LIBDIR *= /opt/Ice-3.3/lib
		LIBS *= -lbz2
		QMAKE_CXXFLAGS *= -fPIC
		slice.commands = /opt/Ice-3.3/bin/slice2cpp --checksum -I/opt/Ice-3.3/slice Murmur.ice
	}
}

include(../../symbols.pri)
