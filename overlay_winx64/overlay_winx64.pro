# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

CONFIG += force-x86_64-toolchain
VPATH = ../overlay
TARGET = mumble_ol_x64

include(../overlay/overlay-shared.pro)
