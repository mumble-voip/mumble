include(../mumble.pri)

DEFINES *= MURMUR
TEMPLATE	=app
CONFIG  *= network
CONFIG	-= gui
QT *= network sql xml
QT -= gui
TARGET = murmur
DBFILE  = murmur.db
LANGUAGE	= C++
FORMS =
HEADERS *= Server.h ServerUser.h Meta.h PBKDF2.h
SOURCES *= main.cpp Server.cpp ServerUser.cpp ServerDB.cpp Register.cpp Cert.cpp Messages.cpp Meta.cpp RPC.cpp PBKDF2.cpp

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
  QMAKE_POST_LINK = $$QMAKE_POST_LINK$$escape_expand(\\n\\t)$$quote(mt.exe -nologo -updateresource:$(DESTDIR_TARGET);1 -manifest ../mumble/mumble.appcompat.manifest)
}

unix {
  contains(UNAME, Linux) {
    LIBS *= -lcap
  }

  CONFIG(static):!macx {
    QMAKE_LFLAGS *= -static
  }

  CONFIG(ermine) {
    QMAKE_LFLAGS *= -Wl,-rpath,$$(MUMBLE_PREFIX)/lib:$$(MUMBLE_ICE_PREFIX)/lib
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
	QT *= dbus
	HEADERS *= DBus.h
	SOURCES *= DBus.cpp
}

ice {
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
			QMAKE_LIBDIR *= $$BZIP2_PATH/lib
			equals(QMAKE_TARGET.arch, x86) {
				QMAKE_LIBDIR *= $$ICE_PATH/lib
			}
			equals(QMAKE_TARGET.arch, x86_64) {
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

	LIBS *= -lmurmur_ice
	INCLUDEPATH *= murmur_ice

	unix {
		QMAKE_CFLAGS *= "-isystem murmur_ice"
		QMAKE_CXXFLAGS *= "-isystem murmur_ice"
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

# Check for QSslDiffieHellmanParameters availability, and define
# USE_QSSLDIFFIEHELLMANPARAMETERS preprocessor if available.
#
# Can be disabled with no-qssldiffiehellmanparameters.
!CONFIG(no-qssldiffiehellmanparameters):exists($$[QT_INSTALL_HEADERS]/QtNetwork/QSslDiffieHellmanParameters) {
	# ...but only if we're inside a Mumble build environment for now.
	# If someone decides to put a Mumble snapshot into a distro, this
	# could break the build in the future, with newer versions of Qt,
	# if the API of QSslDiffieHellmanParameters changes when it is
	# upstreamed.
	MUMBLE_PREFIX=$$(MUMBLE_PREFIX)
	!isEmpty(MUMBLE_PREFIX) {
		DEFINES += USE_QSSLDIFFIEHELLMANPARAMETERS
	}
}

include(../../symbols.pri)
