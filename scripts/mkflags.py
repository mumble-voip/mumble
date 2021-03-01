#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2016-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# mkflags.py generates .pri and .qrc files from Mumble's
# flag SVGs, such that the flags can be included as Qt
# resources.
#
# The script splits the flags into multiple .qrc files once
# a single .qrc file exceeds a given threshold.
#
# This is because older compilers (and old hardware, too!)
# can have problems with large source files. When Qt embeds
# a .qrc file, it generates a .cpp file and compiles it. Some
# of our flag SVGs can be quite large, and if we only use a
# single .qrc file, it is (as of this writing) 32MB+ large.

from __future__ import (unicode_literals, print_function, division)

import os
import shutil
import codecs
import collections

# Container for an on-disk flag SVG. Contains size and filename.
OnDiskFlag = collections.namedtuple('OnDiskFlag', ['size', 'filename'])

# Once a .qrc file's content exceeds this size, the
# file will be considered full.
MAX_SIZE = 1024*1024

def main():
	# Get a list of all flag SVGs, and sort them by size.
	flags = []

	flagsDir = os.path.join('icons', 'flags')
	flagFns = os.listdir(flagsDir)
	for fn in flagFns:
		if not fn.lower().endswith('svg'):
			continue

		with open(os.path.join(flagsDir, fn), 'r') as f:
			buf = f.read()
			sz = len(buf)
			flags.append(OnDiskFlag(size=sz, filename=fn))

	flags = sorted(flags) # Sort by first tuple index (size).

	# Figure out the .qrc target of the individual
	# SVG files. Once a .qrc target exceeds MAX_SIZE,
	# we add a new file.
	flagsOut = []

	curFileContent = []
	curFileSz = 0

	for flag in flags:
		sz = flag.size
		fn = flag.filename

		curFileSz += sz

		curFileContent.append(fn)

		if curFileSz > MAX_SIZE:
			flagsOut.append(curFileContent)

			curFileContent = []
			curFileSz = 0

	if len(curFileContent) > 0:
		flagsOut.append(curFileContent)

	# Remove old flags qrc file.
	oldFlagsQrc = os.path.join('src', 'mumble', 'mumble_flags.qrc')
	if os.path.exists(oldFlagsQrc):
		os.remove(oldFlagsQrc)

	# Remove existing flags dir in src/mumble.
	flagsOutDir = os.path.join('src', 'mumble', 'flags')
	if os.path.exists(flagsOutDir):
		shutil.rmtree(flagsOutDir)
	os.mkdir(flagsOutDir)

	# Generate output files.
	for idx, content in enumerate(flagsOut):
		fn = 'mumble_flags_{0}.qrc'.format(idx)

		with codecs.open(os.path.join(flagsOutDir, fn), "w", "utf-8") as f:
			f.write('<!DOCTYPE RCC>\n')
			f.write('<RCC version="1.0">\n')
			f.write('<qresource>\n')

			for fn in content:
				f.write('<file alias="{0}">{1}</file>\n'.format('flags/' + fn, '../../../icons/flags/' + fn))

			f.write('</qresource>\n')
			f.write('</RCC>\n')

	# Generate .pri file for flags.
	with codecs.open(os.path.join(flagsOutDir, 'mumble_flags.pri'), "w", "utf-8") as f:
		for idx, _ in enumerate(flagsOut):
			fn = 'mumble_flags_{0}.qrc'.format(idx)
			f.write('RESOURCES *= flags/{0}\n'.format(fn))

if __name__ == '__main__':
	main()
