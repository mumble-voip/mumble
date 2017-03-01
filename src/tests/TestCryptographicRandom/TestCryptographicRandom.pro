# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../../../compiler.pri)
include(../../../qmake/openssl.pri)

TEMPLATE = app
QT = core testlib
CONFIG += testcase
CONFIG += qt warn_on
CONFIG -= app_bundle
LANGUAGE = C++

# We build this test with 'gui' in QT,
# but we include the QtGui headers from
# murmur_pch.h. Define QT_NO_OPENGL to
# avoid build errors about a missing GLES/gles2.h
# header.
DEFINES += QT_NO_OPENGL

TARGET = TestCryptographicRandom
SOURCES = TestCryptographicRandom.cpp CryptographicRandom.cpp arc4random_uniform.cpp
HEADERS = CryptographicHash.h
VPATH += ../..
VPATH += ../../../3rdparty/arc4random-src
INCLUDEPATH += ../.. ../../murmur ../../mumble ../../../3rdparty/arc4random-src
