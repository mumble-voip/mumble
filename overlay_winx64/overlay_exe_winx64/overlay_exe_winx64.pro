# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

CONFIG += force-x86_64-toolchain
VPATH = ../../overlay/overlay_exe
TARGET = mumble_ol_helper_x64

include(../../overlay/overlay_exe/overlay_exe-shared.pro)
