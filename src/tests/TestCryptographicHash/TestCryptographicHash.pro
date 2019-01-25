# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../test.pri)

QT += network

TARGET = TestCryptographicHash
SOURCES *= SSL.cpp SSLLocks.cpp TestCryptographicHash.cpp CryptographicHash.cpp
HEADERS *= SSL.h SSLLocks.h CryptographicHash.h
