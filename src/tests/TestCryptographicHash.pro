# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../../compiler.pri)
include(../../qmake/openssl.pri)

TEMPLATE = app
CONFIG += qt warn_on qtestlib
CONFIG -= app_bundle
LANGUAGE = C++
TARGET = TestCryptographicHash
SOURCES = TestCryptographicHash.cpp CryptographicHash.cpp
HEADERS = CryptographicHash.h
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble
