#!/usr/bin/env python3
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from typing import Tuple
from typing import List
from typing import Dict

import argparse
import sys


def parse_key_value_pair(pair: str) -> Tuple[str, str]:
    assert "=" in pair

    idx = pair.find("=")

    key = pair[: idx].strip()
    value = pair[idx + 1:].strip()

    if value[0] == value[-1] and value[0] in ["\"", "'"]:
        # Value is enclosed in quotes -> Remove those
        value = value[1: -1]

    return (key, value)


def parse_key_value_pairs(pairs: List[str]) -> Dict[str, str]:
    key_value_pairs = {}

    for currentPair in pairs:
        key, value = parse_key_value_pair(currentPair)

        key_value_pairs[key] = value

    return key_value_pairs


def main():
    parser = argparse.ArgumentParser(description="This script generates a cmake script whose sole purpose is to invoke configure_file" +
                                     " on a given set of files.")
    parser.add_argument(
        "--variables", help="Specify a number of variables and their content as key-value-pairs", metavar="NAME=VALUE", nargs="+")
    parser.add_argument("--files", help="Specify the to-be-configured files and their output path as key-value-pairs", metavar="IN-FILE=OUT-FILE",
                        nargs="+", required=True)
    parser.add_argument("--output", help="The path to the file to which to write the generated content. If not given, the content is" +
                        " written to stdout")
    parser.add_argument("--at-only", help="Passes the @ONLY option to configure_file", action="store_true", default=False)

    args = parser.parse_args()

    variables = parse_key_value_pairs(args.variables) if args.variables else {}
    files = parse_key_value_pairs(args.files) if args.files else {}

    cmake_content = "# Defining variables\n"
    for currentVar in variables.keys():
        if " " in currentVar:
            print("[ERROR]: Variable name with whitespace \"%s\"" % currentVar)
            sys.exit(1)

        cmake_content += "set(%s \"%s\")\n" % (currentVar, variables[currentVar])

    cmake_content += "\n"
    cmake_content += "# Configuring files\n"
    for currentInFile in files.keys():
        cmake_content += "configure_file(\"%s\" \"%s\" %s)\n" % (currentInFile, files[currentInFile], "@ONLY" if args.at_only else "")

    if args.output:
        with open(args.output, "w") as outFile:
            outFile.write(cmake_content)
    else:
        print(cmake_content)


if __name__ == "__main__":
    main()
