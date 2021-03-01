#!/usr/bin/env python
#
# Copyright 2015-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# rcc-depends.py is a small tool that can be used as a drop-in
# replacement for 'rcc -list' as a command to find dependencies
# for .qrc files.

from __future__ import (unicode_literals, print_function, division)

import os
import platform
import sys
from xml.dom import minidom

def main():
	inputs = sys.argv[1:]
	for fn in inputs:
		with open(fn, 'r') as f:
			absFn = os.path.abspath(fn)
			fnDir = os.path.dirname(absFn)
			s = f.read()
			dom = minidom.parseString(s)
			fileTags = dom.getElementsByTagName('file')
			for fileTag in fileTags:
				textNode = fileTag.childNodes[0].wholeText
				absPath = os.path.normpath(os.path.join(fnDir, textNode))
				if platform.system() == 'Windows':
					try:
						output = os.path.relpath(absPath)
					except ValueError:
						# In some cases, Qt lives on another drive than Mumble.
						# This means that we can't create relative path from
						# our absolute path.
						# In those cases, use the absolute path instead.
						output = absPath
					output = output.replace('\\', '/')
				else:
					output = os.path.relpath(absPath)
				print(output)

if __name__ == '__main__':
	main()
