# Copyright 2005-2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

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

    # For MumbleSSL::qsslSanityCheck()
    LIBS *= -ldl
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
	include(murmur_ice/ice_config.pri)

	SOURCES *= MurmurIce.cpp
	HEADERS *= MurmurIce.h

	DEFINES *= USE_ICE

	LIBS *= -lmurmur_ice
	INCLUDEPATH *= murmur_ice

	unix {
		QMAKE_CFLAGS *= "-isystem murmur_ice"
		QMAKE_CXXFLAGS *= "-isystem murmur_ice"
	}
}

grpc {
	isEqual(QT_MAJOR_VERSION, 4) {
		error("Murmur's gRPC support requires Qt 5")
	}

	DEFINES *= USE_GRPC
	INCLUDEPATH *= murmur_grpc
	LIBS *= -lmurmur_grpc

	HEADERS *= MurmurGRPCImpl.h
	SOURCES *= MurmurGRPCImpl.cpp

	GRPC_WRAPPER = MurmurRPC.proto
	grpc_wrapper.output = MurmurRPC.proto.Wrapper.cpp
	grpc_wrapper.commands = protoc --plugin=${DESTDIR}protoc-gen-murmur-grpcwrapper -I. --murmur-grpcwrapper_out=. MurmurRPC.proto
	grpc_wrapper.input = GRPC_WRAPPER
	grpc_wrapper.variable_out =
	QMAKE_EXTRA_COMPILERS += grpc_wrapper

	unix {
		QMAKE_CXXFLAGS *= -std=c++11
		must_pkgconfig(grpc)
		must_pkgconfig(grpc++)
	}
}

bonjour {
	DEFINES *= USE_BONJOUR

	HEADERS *= \
		../../3rdparty/qqbonjour-src/BonjourRecord.h \
		../../3rdparty/qqbonjour-src/BonjourServiceRegister.h \
		BonjourServer.h
	SOURCES *= \
		../../3rdparty/qqbonjour-src/BonjourServiceRegister.cpp \
		BonjourServer.cpp
	INCLUDEPATH *= ../../3rdparty/qqbonjour-src
	win32 {
		INCLUDEPATH *= "$$BONJOUR_PATH/include"
		QMAKE_LIBDIR *= "$$BONJOUR_PATH/lib/win32"
		LIBS *= -lDNSSD
	}
	unix:!macx {
		system(pkg-config --exists avahi-compat-libdns_sd avahi-client) {
			must_pkgconfig(avahi-compat-libdns_sd)
			must_pkgconfig(avahi-client)
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
