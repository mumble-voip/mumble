# Copyright 2005-2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../plugins.pri)

TARGET		= manual
CONFIG          += qt
isEqual(QT_MAJOR_VERSION, 5) {
    QT *= widgets
}
HEADERS		= manual.h
SOURCES		= manual.cpp
FORMS		+= manual.ui

# Link against mumble_app.dll when
# building for win32-static.
win32:CONFIG(qt_dynamic_lookup) {
    CONFIG(debug, debug|release) {
        QMAKE_LIBDIR *= ../../debug
    }

    CONFIG(release, debug|release) {
        QMAKE_LIBDIR *= ../../release
    }

    LIBS *= -lmumble_app
}
