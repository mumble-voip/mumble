# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../test.pri)

TARGET = TestPasswordGenerator
SOURCES = TestPasswordGenerator.cpp PasswordGenerator.cpp CryptographicRandom.cpp arc4random_uniform.cpp
HEADERS = PasswordGenerator.h CryptographicHash.h

VPATH *= ../../../3rdparty/arc4random-src
INCLUDEPATH *=  ../../../3rdparty/arc4random-src
