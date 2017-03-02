# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../../../compiler.pri)
include(../../../qmake/openssl.pri)

TEMPLATE = app
QT = core testlib
CONFIG += testcase
CONFIG += warn_on
CONFIG -= app_bundle

# We build this test without 'gui' in QT,
# but we include the QtGui headers from
# murmur_pch.h. This causes an error when Qt
# tries to find the OpenGL headers to use, since
# we're missing the include paths from QtGui's
# .pri files. Define QT_NO_OPENGL to avoid these
# build errors.
DEFINES += QT_NO_OPENGL

LANGUAGE = C++
TARGET = TestPasswordGenerator
SOURCES = TestPasswordGenerator.cpp PasswordGenerator.cpp CryptographicRandom.cpp arc4random_uniform.cpp
HEADERS = PasswordGenerator.h CryptographicHash.h
VPATH += ../..
VPATH += ../../../3rdparty/arc4random-src
INCLUDEPATH += ../.. ../../murmur ../../mumble ../../../3rdparty/arc4random-src
