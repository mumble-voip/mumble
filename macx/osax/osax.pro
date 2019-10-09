# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Mumble Overlay scripting addition
# (for injection into running processes)

include(../../qmake/compiler.pri)

CONFIG += x86_64 debug_and_release
CONFIG(universal) {
  CONFIG += ppc
}

TEMPLATE = lib
CONFIG += plugin plugin_bundle
CONFIG -= gui qt

CONFIG(static) {
  CONFIG -= static
}

TARGET = MumbleOverlay
QMAKE_INFO_PLIST = osax.plist

QMAKE_LFLAGS = -framework Foundation -framework Cocoa
QMAKE_BUNDLE_EXTENSION = .osax
QMAKE_LFLAGS_PLUGIN = -bundle

SDEF.files = MumbleOverlay.sdef
SDEF.path = Contents/Resources
QMAKE_BUNDLE_DATA += SDEF

OBJECTIVE_SOURCES = osax.m

include(../../qmake/symbols.pri)
