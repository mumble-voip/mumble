# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Common include file for fake games executables.

include(../../../qmake/compiler.pri)

TEMPLATE = app

INCLUDEPATH += $$PWD

HEADERS += games_linux.h games_win32.h

DESTDIR = $$DESTDIR/tests/games
OBJECTS_DIR = $$OBJECTS_ROOT/games/$$TARGET
