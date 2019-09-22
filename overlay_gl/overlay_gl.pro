# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Overlay payload for Unix-like systems and OS X.

include(../qmake/compiler.pri)

TEMPLATE = lib
CONFIG -= qt gui
CONFIG *= debug_and_release
VERSION = 1.4.0
SOURCES = overlay.c

CONFIG(static) {
  CONFIG -= static
}

unix:!macx {
  TARGET = mumble$(TARGET_ADD)

  DEFINES += TARGET_UNIX
  linux* {
    LIBS *= -lrt -ldl
  }
  QMAKE_CFLAGS *= -fvisibility=hidden $(CFLAGS_ADD)
  QMAKE_LFLAGS -= -Wl,--no-undefined

  QMAKE_LFLAGS -= -Wl,-z,now
  QMAKE_LFLAGS += -Wl,-z,lazy

  QMAKE_LFLAGS *= $(LFLAGS_ADD)
  equals(QMAKE_LINK,g++) {
    QMAKE_LINK = gcc
    QMAKE_LINK_SHLIB = gcc
  }
}

macx {
  CONFIG *= x86_64 x86 plugin
  CONFIG(universal) {
    CONFIG *= ppc
  }

  TARGET = mumbleoverlay$(TARGET_ADD)

  DEFINES += TARGET_MAC
  QMAKE_CFLAGS *= -x objective-c
  QMAKE_CFLAGS *= $(CFLAGS_ADD)
  QMAKE_LFLAGS_PLUGIN *= -undefined dynamic_lookup -dynamic
  QMAKE_LFLAGS += -framework CoreFoundation
  QMAKE_LFLAGS += $(LFLAGS_ADD)
  QMAKE_INFO_PLIST = overlay_gl.plist

  INCLUDEPATH *= ../3rdparty/mach-override-src
  LIBS *= -lmach-override
}


DESTDIR = $$DESTDIR$(DESTDIR_ADD)

QMAKE_LIBDIR = $$DESTDIR $$QMAKE_LIBDIR

include(../qmake/symbols.pri)
