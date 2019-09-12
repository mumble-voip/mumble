# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../test.pri)
include(../../../qmake/qt.pri)

QT *= network

TARGET = TestServerResolver
SOURCES *= TestServerResolver.cpp HostAddress.cpp ServerResolver_qt5.cpp ServerResolverRecord.cpp
HEADERS *= HostAddress.h ServerResolver.h ServerResolverRecord.h

isEqual(QT_MAJOR_VERSION, 4) {
  CONFIG *= no-srv
}

CONFIG(no-srv) {
  DEFINES += USE_NO_SRV
  SOURCES -= ServerResolver_qt5.cpp
  SOURCES *= ServerResolver_nosrv.cpp
}

win32:LIBS *= -lws2_32
