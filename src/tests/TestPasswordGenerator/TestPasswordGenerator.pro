# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../test.pri)

QT += network

TARGET = TestPasswordGenerator
SOURCES *= SSL.cpp SSLLocks.cpp TestPasswordGenerator.cpp PasswordGenerator.cpp CryptographicRandom.cpp arc4random_uniform.cpp
HEADERS *= SSL.h SSLLocks.h PasswordGenerator.h CryptographicHash.h

VPATH *= ../../../3rdparty/arc4random-src
INCLUDEPATH *=  ../../../3rdparty/arc4random-src
