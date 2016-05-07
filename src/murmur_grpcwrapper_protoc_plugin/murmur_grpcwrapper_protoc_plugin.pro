# Copyright 2015-2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../mumble.pri)

TEMPLATE = app
TARGET = protoc-gen-murmur-grpcwrapper
LANGUAGE = C++
SOURCES = main.cpp
LIBS = -lprotoc
CONFIG -= qt
CONFIG += c++11
PKGCONFIG += protobuf

include(../../symbols.pri)
