# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../test.pri)

QT *= network

TARGET = TestCrypt
HEADERS *= SSL.h SSLLocks.h Timer.h CryptState.h
SOURCES *= SSL.cpp SSLLocks.cpp TestCrypt.cpp CryptState.cpp Timer.cpp

win32:LIBS *= -lws2_32
