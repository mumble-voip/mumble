# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../qmake/compiler.pri)

TEMPLATE	= lib
CONFIG		+= plugin debug_and_release warn_on
CONFIG		-= qt
DIST		*= mumble_plugin.h

CONFIG(static) {
	CONFIG -= static
	CONFIG += shared
	CONFIG += qt_dynamic_lookup
}

DESTDIR = $$DESTDIR/plugins
OBJECTS_DIR = $$OBJECTS_ROOT/plugins/$$TARGET

include(../qmake/symbols.pri)
