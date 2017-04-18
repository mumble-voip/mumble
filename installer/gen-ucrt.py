#!/usr/bin/env python
# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from __future__ import (unicode_literals, print_function, division)

import os
import codecs

def mangleComponentId(fn):
	# Component IDs in WiX can't contain dashes. Mangle them with _ instead.
	return fn.replace('-', '_')

def fileHeader(f):
	f.write('<?xml version="1.0" encoding="utf-8"?>\r\n')
	f.write('<Include>\r\n')
	f.write('\r\n')
	f.write('<!-- This file is auto-generated via gen-ucrt.py. Please don\'t touch by hand -->\r\n')
	f.write('\r\n')

def fileFooter(f):
	f.write('\r\n')
	f.write('</Include>\r\n')

def gencomponents(f, files, prefix=''):
	for fn in files:
		f.write('<Component Id="{0}">\r\n'.format(mangleComponentId(prefix+fn)))
		f.write('  <File Id="{0}" Source="$(var.RedistDirUCRT)\{1}" KeyPath="yes" />\r\n'.format(mangleComponentId(prefix+fn), fn))
		f.write('</Component>\r\n')

def gencomponentrefs(f, files, prefix=''):
	for fn in files:
		f.write('''<ComponentRef Id="{0}" />\r\n'''.format(mangleComponentId(prefix+fn)))

def main():
	ucrtx64 = 'C:\\Program Files (x86)\\Windows Kits\\10\\Redist\\ucrt\\DLLs\\x64'
	ucrtx86 = 'C:\\Program Files (x86)\\Windows Kits\\10\\Redist\\ucrt\\DLLs\\x86'

	filesx86 = os.listdir(ucrtx86)
	filesx64 = os.listdir(ucrtx64)

	# Perform a quick sanity test to ensure that both x86 and x64 UCRT variants
	# use the same filenames.
	if set(filesx86) != set(filesx64):
		raise Exception('Fatal error: x86 UCRT files are not equivalent to x64 UCRT files')

	# ...Since they're the same, let's just use the
	# filenames from x86.
	files = filesx86

	with codecs.open('MumbleUCRTComponents.wxi', 'wb', 'utf-8') as f:
		fileHeader(f)
		gencomponents(f, files)
		fileFooter(f)

	with codecs.open('MurmurUCRTComponents.wxi', 'wb', 'utf-8') as f:
		fileHeader(f)
		gencomponents(f, files, 'Murmur_')
		fileFooter(f)

	with codecs.open('MumbleUCRTComponentRefs.wxi', 'wb', 'utf-8') as f:
		fileHeader(f)
		gencomponentrefs(f, files)
		fileFooter(f)

	with codecs.open('MurmurUCRTComponentRefs.wxi', 'wb', 'utf-8') as f:
		fileHeader(f)
		gencomponentrefs(f, files, 'Murmur_')
		fileFooter(f)

if __name__ == '__main__':
	main()
