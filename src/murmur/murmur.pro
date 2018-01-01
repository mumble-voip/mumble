# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../mumble.pri)
include(../../qmake/protoc.pri)

DEFINES *= MURMUR
TEMPLATE =app
CONFIG *= network
CONFIG -= gui
QT *= network sql xml
QT -= gui
TARGET = murmur
DBFILE = murmur.db
LANGUAGE = C++
FORMS =
HEADERS *= Server.h ServerUser.h Meta.h PBKDF2.h
SOURCES *= main.cpp Server.cpp ServerUser.cpp ServerDB.cpp Register.cpp Cert.cpp Messages.cpp Meta.cpp RPC.cpp PBKDF2.cpp

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
  RESOURCES *= murmur.qrc
  SOURCES *= Tray.cpp About.cpp
  HEADERS *= Tray.h About.h
  LIBS *= -luser32
  win32-msvc* {
    QMAKE_POST_LINK = $$QMAKE_POST_LINK$$escape_expand(\\n\\t)$$quote(mt.exe -nologo -updateresource:$(DESTDIR_TARGET);1 -manifest ../mumble/mumble.appcompat.manifest)
  }
}

unix {
  contains(UNAME, Linux) {
    LIBS *= -lcap
  }

  CONFIG(static):!macx {
    QMAKE_LFLAGS *= -static
  }

  !macx:CONFIG(buildenv) {
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
    # check Ice version, 3.7 merged IceUtil into Ice
    ICE_VERSION = $$system(slice2cpp --version 2>&1)
    ICE_MAJOR_VERSION = $$section(ICE_VERSION, ., 0, 0)
    ICE_MINOR_VERSION = $$section(ICE_VERSION, ., 1, 1)

    !equals(ICE_MAJOR_VERSION, 3) {
      error("Unsupported Ice version")
    }
    lessThan(ICE_MINOR_VERSION, 7) {
      # Ice < 3.7
      LIBS *= -lIce -lIceUtil
    }  else {
      # Ice 3.7+
      LIBS *= -lIce
    }
  }

  DEFINES *= USE_ICE

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

  unix:!macx:CONFIG(buildenv) {
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
  grpc_wrapper.commands = $${PROTOC} --plugin=${DESTDIR}protoc-gen-murmur-grpcwrapper -I. --murmur-grpcwrapper_out=. MurmurRPC.proto
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
  DEFINES += USE_QSSLDIFFIEHELLMANPARAMETERS
}

include(../../qmake/symbols.pri)
