# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Common include file for tests.

include(../../qmake/compiler.pri)

TEMPLATE = app
QT = core testlib
CONFIG += testcase
CONFIG += thread warn_on
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

VPATH *= ../.. ../../murmur ../../mumble
INCLUDEPATH *= ../.. ../../murmur ../../mumble

# We have to depend on OpenSSL in all tests
# for no-pch builds to work. Our PCH headers
# include OpenSSL, and if the headers aren't
# in the include path, the build will break.
include(../../qmake/openssl.pri)
