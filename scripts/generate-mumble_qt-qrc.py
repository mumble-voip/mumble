#!/usr/bin/env python
#
# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from __future__ import (unicode_literals, print_function, division)

import os
import platform
import sys

allowed_prefixes = ('qt', 'qtbase')

def dirToQrc(of, dirName, alreadyProcessedLangs):
	processedLangs = []
	absDirName = os.path.abspath(dirName)
	fns = os.listdir(dirName)
	for fn in fns:
		fnRoot, fnExt = os.path.splitext(fn)
		if fnExt.lower() != '.qm':
			continue
		lastUnderscoreIdx = fnRoot.rfind('_')
		if lastUnderscoreIdx == -1:
			continue
		prefix = fnRoot[:lastUnderscoreIdx]
		lang = fnRoot[lastUnderscoreIdx+1:]
		# Handle en_US-style locale names
		if lang.upper() == lang:
			nextToLastUnderscoreIdx = prefix.rfind('_')
			prefix = fnRoot[:nextToLastUnderscoreIdx]
			lang = fnRoot[nextToLastUnderscoreIdx+1:]
		if lang in alreadyProcessedLangs:
			continue
		if not prefix in allowed_prefixes:
			continue
		absFn = os.path.join(absDirName, fn)
		if platform.system() == 'Windows':
			absFn = absFn.replace('\\', '/')
		of.write(' <file alias="{0}">{1}</file>\n'.format(fn, absFn))
		processedLangs.append(lang)
	return processedLangs

def main():
	# python generate-mumble_qt-qrc.py <output-fn> [inputs...]
	output = sys.argv[1]
	inputs = sys.argv[2:]

	of = open(output, 'w')
	of.write('<!DOCTYPE RCC><RCC version="1.0">\n')
	of.write('<qresource>\n')
	processedLangs = []
	for dirName in inputs:
		newlyProcssedLangs = dirToQrc(of, dirName, processedLangs)
		processedLangs.extend(newlyProcssedLangs)
	of.write('</qresource>\n')
	of.write('</RCC>\n')
	of.close()

if __name__ == '__main__':
	main()
