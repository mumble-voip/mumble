# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../../qmake/compiler.pri)
include(../../qmake/protoc.pri)

PROTOBUF *= ../Mumble.proto

pbh.output = ${QMAKE_FILE_BASE}.pb.h
pbh.depends = ${QMAKE_FILE_BASE}.pb.cc
pbh.commands = $$escape_expand(\\n)
pbh.input = PROTOBUF
pbh.CONFIG *= no_link explicit_dependencies target_predeps
pbh.variable_out = HEADERS

pb.output = ${QMAKE_FILE_BASE}.pb.cc
pb.commands = $${PROTOC} --cpp_out=. -I. -I.. -I${QMAKE_FILE_IN_PATH} ${QMAKE_FILE_NAME}
pb.input = PROTOBUF
pb.CONFIG *= no_link explicit_dependencies
pb.variable_out = SOURCES

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
CONFIG += staticlib

# Add protobuf dependency
include(../../qmake/protobuf.pri)

QMAKE_EXTRA_COMPILERS *= pb pbh

!CONFIG(third-party-warnings) {
  # We ignore warnings in third party builds. We won't actually look
  # at them and they clutter out our warnings.
  CONFIG -= warn_on
  CONFIG += warn_off
}

include(../../qmake/symbols.pri)
