#!/usr/bin/env python3

# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Requires to install cairosvg: pip3 install cairosvg
import cairosvg

import argparse
import pathlib
import os
import sys
import tempfile
import subprocess
from distutils.spawn import find_executable


def main():
    repoRoot = pathlib.Path(__file__).absolute().parent.parent

    parser = argparse.ArgumentParser(description="This program generates the mumble.ico icon")
    parser.add_argument("--small-svg", help="Path to the SVG that is to be used for the very small icon sizes", metavar="PATH",
            default=pathlib.Path.joinpath(repoRoot, "icons/mumble_small.svg"))
    parser.add_argument("--regular-svg", help="Path to the SVG that is to be used for the normal and large icon sizes", metavar="PATH",
            default=pathlib.Path.joinpath(repoRoot, "icons/mumble.svg"))
    parser.add_argument("--output", help="Path to where the resulting ico file should be written", metavar="PATH",
            default=pathlib.Path.joinpath(repoRoot, "icons/mumble.ico"))

    args = parser.parse_args()

    convert_bin = find_executable("convert")
    if convert_bin is None:
        print("[ERROR]: The \"convert\" CLI program does not seem to be installed or is not in PATH")
        print("(convert is part of the ImageMagick tools)")
        sys.exit(1)

    try:
        subprocess.run([convert_bin, "--help"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    except Exception as e:
        print("[ERROR]: %s can't be executed: %s" % (convert_bin, e))
        sys.exit(1)

    for current in [args.small_svg, args.regular_svg]:
        if not os.path.isfile(current):
            print("[ERROR]: \"%s\" does not exist or is not a file" % current)
            sys.exit(1)

    small_svg = open(args.small_svg, "r").read()
    regular_svg = open(args.regular_svg, "r").read()

    # First, generate the PNGs in the necessary sizes
    images = []
    for currentSize in [16, 24, 32, 48, 64, 96, 128, 256]:
        outputPath = os.path.join(tempfile.gettempdir(), "mumble_%dx%d.png" % (currentSize, currentSize))
        cairosvg.svg2png(bytestring = small_svg if currentSize < 32 else regular_svg, output_width = currentSize, output_height = currentSize,
                write_to = outputPath)
        images.append(outputPath)

    # Now use the convert CLI utility to combine these PNGs into a .ico file
    subprocess.run([convert_bin, *images, args.output])

    print("Wrote %s" % args.output)


if __name__ == "__main__":
    main()
