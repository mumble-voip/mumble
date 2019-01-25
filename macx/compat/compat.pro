# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Tiny stub program that we splice together into our
# main app bundle for architectures that we don't support.

include(../../qmake/compiler.pri)
include(../common.pri)

TEMPLATE = app
CONFIG -= qt app_bundle
CONFIG += debug_and_release

isEqual(QT_MAJOR_VERSION, 5) {
  QMAKE_CFLAGS += -arch i386
  QMAKE_OBJECTIVE_CFLAGS += -arch i386
  QMAKE_CXXFLAGS += -arch i386
  QMAKE_LFLAGS += -arch i386
} else {
  CONFIG -= x86_64
  CONFIG += x86
}

CONFIG(universal) {
  CONFIG += ppc
}

TARGET = Mumble.compat
OBJECTIVE_SOURCES = compat.m
QMAKE_LFLAGS += -framework ApplicationServices -framework Cocoa -framework AppKit

include(../../qmake/symbols.pri)
