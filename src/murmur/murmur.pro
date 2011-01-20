include(../mumble.pri)

DEFINES *= MURMUR
TEMPLATE	=app
CONFIG  *= network
CONFIG(static) {
	QMAKE_LFLAGS *= -static
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

!CONFIG(no-dbus):!win32 {
	CONFIG *= dbus
}

!CONFIG(no-bonjour) {
	CONFIG *= bonjour
}

win32 {
  RC_FILE = murmur.rc
  CONFIG *= gui
  QT *= gui
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
	slice.target = Murmur.cpp
	win32 {
		slice.commands = slice2cpp --checksum -I\"$$ICE_PATH/slice\" Murmur.ice
	} else {
		slice.commands = slice2cpp --checksum -I/usr/local/share/Ice -I/usr/share/Ice/slice -I/usr/share/slice -I/usr/share/Ice-3.4.1/slice/ -I/usr/share/Ice-3.3.1/slice/ Murmur.ice
	}

	slice.depends = Murmur.ice
	QMAKE_EXTRA_TARGETS *= slice
	PRE_TARGETDEPS *= Murmur.cpp

	SOURCES *= Murmur.cpp MurmurIce.cpp
	HEADERS *= MurmurIce.h
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

bonjour {
	DEFINES *= USE_BONJOUR

	HEADERS *= ../bonjour/bonjourrecord.h ../bonjour/bonjourserviceregister.h BonjourServer.h
	SOURCES *= ../bonjour/bonjourserviceregister.cpp BonjourServer.cpp
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
