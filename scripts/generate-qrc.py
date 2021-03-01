#!/usr/bin/env python
# Copyright 2015-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

#
# Improved version of rcc --project
#
import sys
import os
import re
import argparse

parser = argparse.ArgumentParser(description='Improved version of rcc --project')
parser.add_argument('theme')
parser.add_argument('--cwd', default='.')
parser.add_argument('--prefix', default='')
parser.add_argument('--output', '-o')
parser.add_argument('--include', '-i', default='.*', help='Default inclusion regex')
parser.add_argument('--exclude', '-e', default='a^', help='Default exclusion regex')

args = parser.parse_args()

out = sys.stdout
if args.output:
    out = open(args.output, 'w')

print>>out, '<!DOCTYPE RCC>'
print>>out, '<RCC version=\"1.0\">'
print>>out, '<qresource prefix="%s">' % args.prefix

include = re.compile(args.include)
exclude = re.compile(args.exclude)

os.chdir(args.cwd)
for (dirpath, dirnames, filenames) in os.walk(args.theme):
    for f in filenames:
        path = os.path.join(dirpath, f)
        relpath = os.path.relpath(path, args.theme)

        if not include.search(relpath):
            continue
            
        if exclude.search(relpath):
            continue

        print>>out, '    <file alias="%s">%s</file>' % (relpath, path)
print>>out, '</qresource>'
print>>out, '</RCC>'
