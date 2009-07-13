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
HEADERS	*= ../ACL.h ../Group.h ../Channel.h ../Connection.h ../User.h ../Net.h
SOURCES *= ../ACL.cpp ../Group.cpp ../Channel.cpp ../Connection.cpp ../User.cpp ../Timer.cpp ../CryptState.cpp ../OSInfo.cpp ../Net.cpp

DIST = DBus.h ServerDB.h ../../icons/murmur.ico Murmur.ice MurmurI.h MurmurIceWrapper.cpp
PRECOMPILED_HEADER = murmur_pch.h

!CONFIG(no-ice) {
	CONFIG *= ice
}

!CONFIG(no-dbus) {
	CONFIG *= dbus
}


win32 {
  RC_FILE = murmur.rc
  CONFIG *= gui
  QT *= gui
  LIBS	*= -lws2_32
  RESOURCES	*= murmur.qrc
  SOURCES *= Tray.cpp
  HEADERS *= Tray.h
  INCLUDEPATH *= /dev/OpenSSL/include
  LIBPATH *= /dev/OpenSSL/lib
  LIBS *= -llibeay32 -luser32
  LIBS *= -ldelayimp -lQwave -delayload:Qwave.DLL
}

unix {
  UNAME=$$system(uname -s)
  CONFIG *= link_pkgconfig
  contains(UNAME, Linux) {
    LIBS *= -lcap
  }

  contains(UNAME, FreeBSD) {
    LIBS *= -lcrypto
  } else {
    PKGCONFIG *= openssl
  }
  HEADERS *= UnixMurmur.h
  SOURCES *= UnixMurmur.cpp
  TARGET = murmurd
}

macx {
  CONFIG -= app_bundle
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
		INCLUDEPATH *= /opt/mumble/ice-3.3.0/include/
		LIBPATH *= /opt/mumble/ice-3.3.0/lib/
	}

	unix:CONFIG(static) {
		INCLUDEPATH *= /opt/Ice-3.3/include
		LIBPATH *= /opt/Ice-3.3/lib
		LIBS *= -lbz2
		QMAKE_CXXFLAGS *= -fPIC
		slice.commands = /opt/Ice-3.3/bin/slice2cpp Murmur.ice
	}
}
