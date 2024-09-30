#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright The Mumble Developers. All rights reserved.
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
import codecs
import argparse

# Once a .qrc file's content exceeds this size, the
# file will be considered full.
MAX_SIZE = 1024*1024

def main():
    parser = argparse.ArgumentParser("This script generates a QRC file for embedding the flag SVG icons")
    parser.add_argument("--flag-dir", help="Path to the directory in which the flag SVGs reside in", metavar="PATH", required=True)
    parser.add_argument("--output", help="Path to which the generated QRC file shall be written", metavar="PATH", required=True)

    args = parser.parse_args()

    # Get a list of all flag SVGs
    flags = []

    for fn in sorted(os.listdir(args.flag_dir)):
        if not fn.lower().endswith('svg'):
            continue
        
        flags.append(fn)

    output_directory = os.path.dirname(args.output)

    # Generate output file.
    with codecs.open(args.output, "w", "utf-8") as f:
        f.write('<!DOCTYPE RCC>\n')
        f.write('<RCC version="1.0">\n')
        f.write('<qresource>\n')

        for fn in flags:
            f.write('<file alias="{0}">{1}</file>\n'.format('flags/' + fn, os.path.relpath(os.path.join(args.flag_dir, fn), output_directory)))

        f.write('</qresource>\n')
        f.write('</RCC>\n')

if __name__ == '__main__':
    main()
