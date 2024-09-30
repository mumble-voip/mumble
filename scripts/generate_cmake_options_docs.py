#!/usr/bin/env python3
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

import os
import re
from functools import total_ordering

@total_ordering
class CmakeOption:
    def __init__(self, name, description, default):
        self.m_name = name
        self.m_description = description
        self.m_defaultValue = default

    def __eq__(self, other):
        return self.m_name.lower() == other.m_name.lower()

    def __lt__(self, other):
        return self.m_name.lower() < other.m_name.lower()


def main():
    scriptPath = os.path.realpath(__file__)
    rootDir = os.path.dirname(os.path.dirname(scriptPath))

    cmakeFiles = []

    for root, dirs, files in os.walk(rootDir):
        for currentFile in files:
            if currentFile.lower().endswith(".cmake") or currentFile.lower().endswith("cmakelists.txt"):
                path = os.path.join(root, currentFile)

                if path.startswith(os.path.join(rootDir, "build")):
                    # Exclude all files in the build dir
                    continue

                if path.startswith(os.path.join(rootDir, "3rdparty")):
                    # Exclude all files in the 3rdParty dir
                    continue

                cmakeFiles.append(os.path.join(root, currentFile));


    options = []
    for currentFile in cmakeFiles:
        content = open(currentFile, "r").read()

        pattern = re.compile(r"option\(([a-zA-Z_\-0-9]+)\s+\"(.*?)\"\s+(.*?)\)")
        pos = 0
        match = pattern.search(content, pos)

        while not match is None:
            optionName = match.group(1)
            optionDescription = match.group(2)
            optionDefault = match.group(3)

            options.append(CmakeOption(optionName, optionDescription, optionDefault))

            pos = match.end()

            match = pattern.search(content, pos)


    # sort options
    options.sort()


    generatedContent = "# CMake options\n"
    generatedContent += "\n"
    generatedContent += "Using CMake the build can be customized in a number of ways. The most prominent examples for this is the usage of different\n"
    generatedContent += "options (flags). These can be set by using `-D<optionName>=<value>` where `<optionName>` is the name of the respective option\n"
    generatedContent += "as listed below and `<value>` is either `ON` or `OFF` depending on whether the option shall be activated or inactivated.\n"
    generatedContent += "\n"
    generatedContent += "An example would be `cmake -Dtests=ON ..`.\n"
    generatedContent += "\n"
    generatedContent += "\n"
    generatedContent += "## Available options\n"
    generatedContent += "\n"

    for currentOption in options:
        generatedContent += "### " + currentOption.m_name + "\n"
        generatedContent += "\n"
        desc = currentOption.m_description

        if desc == "":
            desc = "(No description available)"

        generatedContent += desc + "\n"
        generatedContent += "(Default: " + currentOption.m_defaultValue + ")\n"
        generatedContent += "\n"


    print(generatedContent)



if __name__ == "__main__":
    main()
