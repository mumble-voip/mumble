# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(../qmake/compiler.pri)

TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link

win32|linux {
  SUBDIRS += aoc arma2 bf1942 bf2 bf3 bf2142 bfbc2 bfheroes bf4_x86 blacklight borderlands borderlands2 breach cod2 cod4 cod5 codmw2 codmw2so cs dys etqw ffxiv gmod gtaiv gtasa gw insurgency jc2 l4d l4d2 lol lotro ql rl sr ut2004 ut3 ut99 wolfet wow

  equals(MUMBLE_ARCH, x86_64) {
    SUBDIRS += bf1 bf4 ffxiv_x64 gtav wow_x64
  }
}

# Plugins which are now retracted were only released for Windows
win32 {
  SUBDIRS += css dods hl2dm sto tf2
}
