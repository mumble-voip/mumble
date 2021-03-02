#!/usr/bin/env python
#
# Copyright 2015-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
#
# extract-emojione-flags.py
# Extract SVG flags from the Emoji One emoji collection.
#
# This script walks the Emoji One SVG collection and extracts
# flag emojis.
#
# Emoji One's emojis all use the Unicode code points (base 16) as
# their on-disk names. If a character consists of multiple code points,
# they are separated by a dash ('-').
#
# This script walks all the Emoji One SVGs, and finds the ones which
# only contain symbols in the Regional Indicator Symbol range.
# These are flag emojis.
#
# The flags are then copied to the destination directory, and named
# as the region's lower case ISO_3166-1_alpha-2 code with the ".svg"
# extension.

from __future__ import (unicode_literals, print_function, division)

import os
import shutil
import sys

# Beginning and end of the Regional Indicator Symbol range,
# as defined by Unicode 6.0's emoji support.
#
# See https://en.wikipedia.org/wiki/Regional_Indicator_Symbol
# for more information.
REGIONAL_INDICATOR_SYMBOL_LETTER_A = 0x1F1E6
REGIONAL_INDICATOR_SYMBOL_LETTER_Z = 0x1F1FF

def is_region(points):
	for point in points:
		if point > REGIONAL_INDICATOR_SYMBOL_LETTER_Z \
		   or point < REGIONAL_INDICATOR_SYMBOL_LETTER_A:
			return False	
	return True

def region_name(points):
	name = ''
	for point in points:
		name += chr(point - REGIONAL_INDICATOR_SYMBOL_LETTER_A + ord('a'))
	return name

def main():
	if len(sys.argv) < 3:
		print('''Usage: extract-emojione-flags.py [emojione-svg-dir] [dst-dir]

emojione-svg-dir: point this to the directory containing Emoji One SVGs, such as
                  $EMOJIONE_SRC_TREE/assets/svg.

dst-dir: point this to the directory you want the resulting SVGs to be written to.

''')
		sys.exit(1)

	emojiOneSvgDir = sys.argv[1]
	dstDir = sys.argv[2]

	files = os.listdir(emojiOneSvgDir)
	for fn in files:
		base, ext = os.path.splitext(fn.lower())
		if not ext == '.svg':
			continue
		
		pointsStr = base.split('-')
		points = [int(p, 16) for p in pointsStr]
		
		if is_region(points):
			name = region_name(points)
			svgName = name + '.svg'
			shutil.copy(os.path.join(emojiOneSvgDir, fn),
			            os.path.join(dstDir, svgName))

if __name__ == '__main__':
	main()
