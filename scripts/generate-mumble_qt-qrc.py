#!/usr/bin/env python
#
# Copyright (C) 2015 Mikkel Krautz <mikkel@krautz.dk>
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# - Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
# - Neither the name of the Mumble Developers nor the names of its
#   contributors may be used to endorse or promote products derived from this
#   software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


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
