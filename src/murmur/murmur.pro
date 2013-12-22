include(../mumble.pri)

DEFINES *= MURMUR
TEMPLATE	=app
CONFIG  *= network
CONFIG(static):!macx {
	QMAKE_LFLAGS *= -static
}
CONFIG(ermine) {
	QMAKE_LFLAGS *= -Wl,-rpath,$$(MUMBLE_PREFIX)/lib:$$(MUMBLE_ICE_PREFIX)/lib
}
CONFIG	-= gui
QT *= network sql xml
QT -= gui
TARGET = murmur
DBFILE  = murmur.db
LANGUAGE	= C++
FORMS =
HEADERS *= Server.h ServerUser.h Meta.h
SOURCES *= main.cpp Server.cpp ServerUser.cpp ServerDB.cpp Register.cpp Cert.cpp Messages.cpp Meta.cpp RPC.cpp

DIST = DBus.h ServerDB.h ../../icons/murmur.ico Murmur.ice MurmurI.h MurmurIceWrapper.cpp murmur.plist
PRECOMPILED_HEADER = murmur_pch.h

!CONFIG(no-ice) {
	CONFIG *= ice
}

!CONFIG(no-dbus):!win32:!macx {
	CONFIG *= dbus
}

!CONFIG(no-bonjour) {
	CONFIG *= bonjour
}

win32 {
  RC_FILE = murmur.rc
  CONFIG *= gui
  QT *= gui
  isEqual(QT_MAJOR_VERSION, 5) {
    QT *= widgets
  }
  RESOURCES	*= murmur.qrc
  SOURCES *= Tray.cpp
  HEADERS *= Tray.h
  LIBS *= -luser32
}

unix {
  contains(UNAME, Linux) {
    LIBS *= -lcap
  }

  HEADERS *= UnixMurmur.h
  SOURCES *= UnixMurmur.cpp
  TARGET = murmurd
}

macx {
  CONFIG -= app_bundle
  LIBS *= -framework Security
  QMAKE_LFLAGS += -sectcreate __TEXT __info_plist murmur.plist
}

dbus {
	DEFINES *= USE_DBUS
	CONFIG *= qdbus
	HEADERS *= DBus.h
	SOURCES *= DBus.cpp
}

ice {
	SLICEFILES = Murmur.ice

	slice.output = ${QMAKE_FILE_BASE}.cpp
	win32 {
		slice.commands = slice2cpp --checksum -I\"$$ICE_PATH/slice\" ${QMAKE_FILE_NAME}
	} else {
		slice.commands = slice2cpp --checksum -I/usr/local/share/Ice -I/usr/share/Ice/slice -I/usr/share/slice -I/usr/share/Ice-3.4.1/slice/ -I/usr/share/Ice-3.3.1/slice/ -I/usr/share/Ice-3.4.2/slice/ -I/usr/share/Ice-3.5.0/slice/ -I/usr/share/Ice-3.5.1/slice/ ${QMAKE_FILE_NAME}
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

	SOURCES *= MurmurIce.cpp
	HEADERS *= MurmurIce.h
	win32:CONFIG(debug, debug|release) {
		LIBS *= -lIceD -lIceUtilD
	} else {
		LIBS *= -lIce -lIceUtil
	}
	DEFINES *= USE_ICE

	win32 {
		INCLUDEPATH *= "$$ICE_PATH/include"
		!CONFIG(static) {
			QMAKE_LIBDIR *= "$$ICE_PATH/lib/vc100"
		} else {
			DEFINES *= ICE_STATIC_LIBS
			QMAKE_LIBDIR *= $$ICE_PATH/lib $$BZIP2_PATH/lib
			LIBS *= -llibbz2 -ldbghelp -liphlpapi -lrpcrt4
		}
	}

	macx {
		INCLUDEPATH *= $$(MUMBLE_PREFIX)/Ice-3.4.2/include/
		QMAKE_LIBDIR *= $$(MUMBLE_PREFIX)/Ice-3.4.2/lib/
		slice.commands = $$(MUMBLE_PREFIX)/Ice-3.4.2/bin/slice2cpp --checksum -I$$(MUMBLE_PREFIX)/Ice-3.4.2/slice/ Murmur.ice
	}

	CONFIG(ermine) {
		INCLUDEPATH *= $$(MUMBLE_ICE_PREFIX)/include/
		QMAKE_LIBDIR *= $$(MUMBLE_ICE_PREFIX)/lib/
		slice.commands = $$(MUMBLE_ICE_PREFIX)/bin/slice2cpp --checksum -I$$(MUMBLE_ICE_PREFIX)/slice/ Murmur.ice
	}

	unix:!macx:CONFIG(static) {
		INCLUDEPATH *= /opt/Ice-3.3/include
		QMAKE_LIBDIR *= /opt/Ice-3.3/lib
		LIBS *= -lbz2
		QMAKE_CXXFLAGS *= -fPIC
		slice.commands = /opt/Ice-3.3/bin/slice2cpp --checksum -I/opt/Ice-3.3/slice Murmur.ice
	}

	macx:CONFIG(static) {
		LIBS *= -lbz2 -liconv
		QMAKE_CXXFLAGS *= -fPIC
	}
}

bonjour {
	DEFINES *= USE_BONJOUR

	HEADERS *= ../bonjour/BonjourRecord.h ../bonjour/BonjourServiceRegister.h BonjourServer.h
	SOURCES *= ../bonjour/BonjourServiceRegister.cpp BonjourServer.cpp
	INCLUDEPATH *= ../bonjour
	win32 {
		INCLUDEPATH *= "$$BONJOUR_PATH/include"
		QMAKE_LIBDIR *= "$$BONJOUR_PATH/lib/win32"
		LIBS *= -lDNSSD
	}
	unix:!macx {
		system(pkg-config --exists avahi-compat-libdns_sd avahi-client) {
			PKGCONFIG *= avahi-compat-libdns_sd avahi-client
		} else {
			LIBS *= -ldns_sd
		}
	}
}

include(../../symbols.pri)
