# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../test.pri)
include(../../../qmake/qt.pri)

QT *= network

TARGET = TestServerResolver
SOURCES *= TestServerResolver.cpp HostAddress.cpp ServerResolver.cpp ServerResolverRecord.cpp
HEADERS *= HostAddress.h ServerResolver.h ServerResolverRecord.h

win32:LIBS *= -lws2_32
