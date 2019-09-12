# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../../qmake/compiler.pri)

TEMPLATE = app
CONFIG -= qt
CONFIG += debug_and_release
TARGET = mumble

INCLUDEPATH *= $$PWD/..

win32 {
  DEFINES += WIN32 _WIN32
  RC_FILE = ../mumble/mumble.rc
  LIBS *= -luser32 -lshlwapi

  win32-g++ {
    QMAKE_LFLAGS *= -municode
    DEFINES *= _UNICODE
  }
  
  win32-msvc* {
    CONFIG(release, debug|release) {
      QMAKE_CXXFLAGS_RELEASE -= -MD
      QMAKE_CXXFLAGS += -MT
    }
    CONFIG(debug, debug|release) {
      QMAKE_CXXFLAGS_DEBUG -= -MDd
      QMAKE_CXXFLAGS += -MTd
    }
  }
}

SOURCES *= mumble_exe.cpp Overlay.cpp

!CONFIG(no-elevation) {
  CONFIG(release, debug|release) {
    QMAKE_LFLAGS *= /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\"
  }
}

win32-msvc* {
  QMAKE_POST_LINK = $$QMAKE_POST_LINK$$escape_expand(\\n\\t)$$quote(mt.exe -nologo -updateresource:$(DESTDIR_TARGET);1 -manifest ../mumble/mumble.appcompat.manifest)
}

CONFIG(debug, debug|release) {
  CONFIG += console
  DEFINES *= DEBUG
}

CONFIG(release, debug|release) {
  DEFINES *= NDEBUG
}

include(../../qmake/symbols.pri)
