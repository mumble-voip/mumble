# Copyright 2005-2016 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

TEMPLATE = subdirs

CONFIG += debug_and_release
SUBDIRS = link
DIST = plugins.pri

win32 {
	SUBDIRS += aoc arma2 bf1942 bf2 bf3 bf2142 bfbc2 bfheroes bf4 bf4_x86 blacklight borderlands borderlands2 breach cod2 cod4 cod5 codmw2 codmw2so cs css dods dys etqw tf2 gmod gtaiv gtav gw hl2dm insurgency jc2 l4d l4d2 lol lotro ql rl sr sto ut2004 ut3 ut99 wolfet wow
}
