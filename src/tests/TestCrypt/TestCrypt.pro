# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../../../compiler.pri)
include(../../../qmake/openssl.pri)

TEMPLATE = app
QT = core network testlib
CONFIG += testcase
CONFIG += thread warn_on
CONFIG -= app_bundle

# We build this test with 'gui' in QT,
# but we include the QtGui headers from
# murmur_pch.h. Define QT_NO_OPENGL to
# avoid build errors about a missing GLES/gles2.h
# header.
DEFINES += QT_NO_OPENGL

LANGUAGE = C++
TARGET = TestCrypt
HEADERS = Timer.h CryptState.h
SOURCES = TestCrypt.cpp CryptState.cpp Timer.cpp
VPATH += ../..
INCLUDEPATH += ../.. ../../murmur ../../mumble
