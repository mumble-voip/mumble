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
HEADERS *= Server.h Meta.h
SOURCES *= main.cpp Server.cpp ServerDB.cpp Register.cpp Cert.cpp Messages.cpp Meta.cpp RPC.cpp

DIST = DBus.h ServerDB.h ../../icons/murmur.ico Murmur.ice MurmurI.h MurmurIceWrapper.cpp
PRECOMPILED_HEADER = murmur_pch.h

!CONFIG(no-ice) {
	CONFIG *= ice
}

!CONFIG(no-dbus) {
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
}

dbus {
	DEFINES *= USE_DBUS
	CONFIG *= qdbus
	HEADERS *= DBus.h
	SOURCES *= DBus.cpp
}

ice {
	slice.target = Murmur.cpp
	slice.commands = slice2cpp Murmur.ice
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
		INCLUDEPATH *= /dev/Ice/include
		LIBPATH *= /dev/Ice/lib
	}

	macx {
		INCLUDEPATH *= /opt/mumble-1.2/ice-3.3.1/include/
		LIBPATH *= /opt/mumble-1.2/ice-3.3.1/lib/
	}

	unix:CONFIG(static) {
		INCLUDEPATH *= /opt/Ice-3.3/include
		LIBPATH *= /opt/Ice-3.3/lib
		LIBS *= -lbz2
		QMAKE_CXXFLAGS *= -fPIC
		slice.commands = /opt/Ice-3.3/bin/slice2cpp Murmur.ice
	}
}

bonjour {
	DEFINES *= USE_BONJOUR

	HEADERS *= ../bonjour/bonjourrecord.h ../bonjour/bonjourserviceregister.h BonjourServer.h
	SOURCES *= ../bonjour/bonjourserviceregister.cpp BonjourServer.cpp
	INCLUDEPATH *= ../bonjour
	win32 {
		INCLUDEPATH *= /dev/Bonjour/include
		LIBPATH *= /dev/Bonjour/lib/win32
		LIBS *= -ldelayimp -lDNSSD -delayload:DNSSD.DLL
	}
	unix:!macx {
		PKGCONFIG *= avahi-compat-libdns_sd avahi-client
	}
}
