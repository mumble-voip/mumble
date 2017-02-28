# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../../../compiler.pri)

TEMPLATE = app
QT = core testlib
CONFIG += testcase
CONFIG += warn_on
CONFIG -= app_bundle

# We build this test with 'gui' in QT,
# but we include the QtGui headers from
# murmur_pch.h. Define QT_NO_OPENGL to
# avoid build errors about a missing GLES/gles2.h
# header.
DEFINES += QT_NO_OPENGL

LANGUAGE = C++
TARGET = TestTimer
SOURCES = TestTimer.cpp Timer.cpp
HEADERS = Timer.h
VPATH += ../..
INCLUDEPATH += ../.. ../../murmur ../../mumble
