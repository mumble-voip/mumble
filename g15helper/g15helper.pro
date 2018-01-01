# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Build rules for G15 helper daemon

win32:!CONFIG(g15-emulator) {
  CONFIG += force-x86-toolchain
}
include (../qmake/compiler.pri)

TEMPLATE = app
CONFIG *= debug_and_release warn_on
TARGET = mumble-g15-helper

HEADERS = g15helper.h

# The g15helper utility can be built in two modes:
#
#  - Native. This is what will be built if you're
#    on Windows or macOS by default.
#
#  - Emulator. This can be enabled by using the
#    CONFIG(g15-emulator) when invoking qmake.
#    Instead of building a g15helper that talks
#    to the G15 LCD using the Logitech provided
#    library, we build a small program that
#    emulates the LCD display in an on-screen
#    window. This is useful for debugging
#    Mumble's LCD/G15 support.
CONFIG(g15-emulator) {
  QT *= core gui
  isEqual(QT_MAJOR_VERSION, 5) {
    QT *= widgets
  }

  SOURCES *= g15helper_emu.cpp
  HEADERS *= g15helper_emu.h

  win32 {
    RC_FILE = g15helper.rc

    win32-msvc* {
      QMAKE_POST_LINK = $$QMAKE_POST_LINK$$escape_expand(\\n\\t)$$quote(mt.exe -nologo -updateresource:$(DESTDIR_TARGET);1 -manifest ../src/mumble/mumble.appcompat.manifest)
    }
  }
} else {
  CONFIG -= qt

  win32 {
    SOURCES = g15helper.c
    RC_FILE = g15helper.rc
    LIBS *= -llgLcd -ladvapi32 -lshell32 -luser32
    QMAKE_LIBDIR *= "$$G15SDK_PATH/Lib/x86"
    INCLUDEPATH *= "$$G15SDK_PATH/Src"
    DEFINES *= WIN32

    win32-msvc* {
      QMAKE_POST_LINK = $$QMAKE_POST_LINK$$escape_expand(\\n\\t)$$quote(mt.exe -nologo -updateresource:$(DESTDIR_TARGET);1 -manifest ../src/mumble/mumble.appcompat.manifest)
    }

    CONFIG(release, debug|release) {
      QMAKE_CFLAGS_RELEASE -= -MD
      QMAKE_CFLAGS_RELEASE *= -MT
      QMAKE_CXXFLAGS_RELEASE -= -MD
      QMAKE_CXXFLAGS_RELEASE *= -MT
    }
    CONFIG(debug, debug|release) {
      QMAKE_CFLAGS_DEBUG -= -MDd
      QMAKE_CFLAGS_DEBUG *= -MTd
      QMAKE_CXXFLAGS_DEBUG -= -MDd
      QMAKE_CXXFLAGS_DEBUG *= -MTd
    }
  }

  macx {
    CONFIG -= app_bundle
    isEqual(QT_MAJOR_VERSION, 5) {
      QMAKE_CFLAGS += -arch i386
      QMAKE_OBJECTIVE_CFLAGS += -arch i386
      QMAKE_CXXFLAGS += -arch i386
      QMAKE_LFLAGS += -arch i386
    } else {
      CONFIG -= x86_64
      CONFIG += x86
    }
    SOURCES = g15helper_macx.c
    LIBS *= -llgLcd
    QMAKE_LIBDIR *= $$(MUMBLE_PREFIX)/../LCDSDK/lib/
    INCLUDEPATH *= $$(MUMBLE_PREFIX)/../LCDSDK/Src/
    QMAKE_LFLAGS += -framework CoreFoundation -sectcreate __TEXT __info_plist g15helper.plist
    DEFINES *= APPLE
  }
}

CONFIG(release, debug|release) {
  DESTDIR = ../release
}

CONFIG(debug, debug|release) {
  DEFINES *= USE_LOGFILE
  DESTDIR = ../debug/
}

include(../qmake/symbols.pri)
