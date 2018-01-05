# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../test.pri)

QT += network

TARGET = TestCryptographicRandom
SOURCES *= SSL.cpp SSLLocks.cpp TestCryptographicRandom.cpp CryptographicRandom.cpp arc4random_uniform.cpp
HEADERS *= SSL.h SSLLocks.h CryptographicHash.h

VPATH *= ../../../3rdparty/arc4random-src
INCLUDEPATH *= ../../../3rdparty/arc4random-src
