# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../test.pri)

QT += network

# Check for QSslDiffieHellmanParameters availability, and define
# USE_QSSLDIFFIEHELLMANPARAMETERS preprocessor if available.
#
# Can be disabled with no-qssldiffiehellmanparameters.
!CONFIG(no-qssldiffiehellmanparameters):exists($$[QT_INSTALL_HEADERS]/QtNetwork/QSslDiffieHellmanParameters) {
  DEFINES += USE_QSSLDIFFIEHELLMANPARAMETERS
}

TARGET = TestFFDHE
SOURCES *= SSL.cpp SSLLocks.cpp FFDHE.cpp TestFFDHE.cpp
HEADERS *= SSL.h SSLLocks.h FFDHE.h FFDHETable.h
