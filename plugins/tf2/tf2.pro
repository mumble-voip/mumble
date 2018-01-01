# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../plugins.pri)

DEFINES += "NULL_DESC=\"L\\\"Team Fortress 2 (Retracted, now using link)\\\"\""
TARGET = tf2
SOURCES = ../null_plugin.cpp
LIBS += -luser32
