# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../qmake/compiler.pri)
include(../qmake/qt.pri)
include(../qmake/rcc.pri)
include(../qmake/pkgconfig.pri)

VERSION		= 1.4.0
DIST		= mumble.pri Message.h PacketDataStream.h CryptState.h Timer.h Version.h OSInfo.h SSL.h
CONFIG		+= qt thread debug_and_release warn_on
DEFINES		*= MUMBLE_VERSION_STRING=$$VERSION
INCLUDEPATH	+= $$PWD . ../mumble_proto
VPATH		+= $$PWD

HEADERS *= \
  ACL.h \
  Channel.h \
  CryptState.h \
  Connection.h \
  Group.h \
  HTMLFilter.h \
  User.h \
  Net.h \
  OSInfo.h \
  Timer.h \
  SSL.h \
  Version.h \
  SSLCipherInfo.h \
  SSLCipherInfoTable.h \
  licenses.h \
  License.h \
  LogEmitter.h \
  CryptographicHash.h \
  CryptographicRandom.h \
  PasswordGenerator.h \
  ByteSwap.h \
  HostAddress.cpp \
  Ban.h \
  EnvUtils.h \
  UnresolvedServerAddress.h \
  ServerAddress.h \
  ServerResolver.h \
  ServerResolverRecord.h \
  SelfSignedCertificate.h \
  SSLLocks.h \
  FFDHETable.h \
  FFDHE.h

SOURCES *= \
  ACL.cpp \
  Group.cpp \
  Channel.cpp \
  Connection.cpp \
  HTMLFilter.cpp \
  User.cpp \
  Timer.cpp \
  CryptState.cpp \
  OSInfo.cpp \
  SSL.cpp \
  Version.cpp \
  SSLCipherInfo.cpp \
  License.cpp \
  LogEmitter.cpp \
  CryptographicHash.cpp \
  CryptographicRandom.cpp \
  PasswordGenerator.cpp \
  HostAddress.cpp \
  Ban.cpp \
  EnvUtils.cpp \
  UnresolvedServerAddress.cpp \
  ServerAddress.cpp \
  ServerResolver_qt5.cpp \
  ServerResolverRecord.cpp \
  SelfSignedCertificate.cpp \
  SSLLocks.cpp \
  FFDHE.cpp

LIBS		*= -lmumble_proto

equals(QT_MAJOR_VERSION, 4) {
	CONFIG *= no-srv
}

CONFIG(no-srv) {
	DEFINES += USE_NO_SRV
	SOURCES -= ServerResolver_qt5.cpp
	SOURCES *= ServerResolver_nosrv.cpp
}

# Add arc4random_uniform
INCLUDEPATH *= ../../3rdparty/arc4random-src
SOURCES *= ../../3rdparty/arc4random-src/arc4random_uniform.cpp

# Note: Protobuf generates into its own directory so we can mark it as a
#       system include folder for unix. Otherwise the generated code creates
#       a lot of spurious warnings in ours.

CONFIG(packaged) {
	MUMDEFVER = $$find(DEFINES, "MUMBLE_VERSION=")
	count(MUMDEFVER, 0) {
		DEFINES	*= MUMBLE_VERSION=$$VERSION
	}
}

# Add OpenSSL dependency
include(../qmake/openssl.pri)

# Add protobuf dependency
include(../qmake/protobuf.pri)

win32-msvc* {
	LIBS *= -lcrypt32 -lws2_32
	LIBS *= -ldelayimp -lQwave -delayload:Qwave.DLL
}

win32-g++ {
	LIBS *= -lprotobuf -lcrypt32 -lws2_32
	LIBS *= -ldelayimp -lqwave -delayload:qwave.dll
}

unix {
	CONFIG(static) {
		PKG_CONFIG += --static
	}

	QMAKE_CFLAGS *= "-I../mumble_proto" "-isystem ../mumble_proto"
	QMAKE_CXXFLAGS *= "-I../mumble_proto" "-isystem ../mumble_proto"
}

# Make Q_DECL_OVERRIDE and Q_DECL_FINAL no-ops
# for Qt 4.
isEqual(QT_MAJOR_VERSION, 4) {
	DEFINES *= Q_DECL_OVERRIDE=
	DEFINES *= Q_DECL_FINAL=
}

CONFIG(debug, debug|release) {
  CONFIG += console
}

QMAKE_LIBDIR = $$DESTDIR $$QMAKE_LIBDIR
