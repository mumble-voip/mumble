#!/bin/bash -ex
#
# Copyright 2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opt/openssl/lib/pkgconfig
export PATH=$PATH:/usr/local/opt/qt5/bin:/usr/local/bin
export MUMBLE_PREFIX=/usr/local
export MUMBLE_ICE_PREFIX=/usr/local/opt/ice

qmake -recursive CONFIG+="release tests warnings-as-errors" && make -j $(sysctl -n hw.ncpu) && make check
