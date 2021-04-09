#!/usr/bin/env python3

# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.


import argparse
import os
import platform
import subprocess
import datetime
from string import Template
import re

def cmd(args):
    shell = platform.system() == 'Windows'
    p = subprocess.Popen(args, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        raise Exception('cmd(): {0} failed with status {1}: {2}'.format(args, p.returncode, stderr))
    return stdout.decode('utf-8')


licenseHeaderTemplate = Template(
"""${commentChar} Copyright ${copyright} The Mumble Developers. All rights reserved.
${commentChar} Use of this source code is governed by a BSD-style license
${commentChar} that can be found in the LICENSE file at the root of the
${commentChar} Mumble source tree or at <https://www.mumble.info/LICENSE>.""")

blackListFiles = []

class InvalidFileError(Exception):
    pass

class LicenseNotFoundError(Exception):
    pass

def generateHeaderForFile(filePath):
    if filePath in blackListFiles:
        raise InvalidFileError("File is blacklisted: \"" + filePath + "\"")

    fileName = os.path.basename(filePath)
    extension = os.path.splitext(fileName)[-1]

    commentChar = None

    if extension in [".cpp", ".c", ".h", ".hpp", ".cs", ".ice", ".proto", ".mm", ".vs", ".m", ".ps", ".hlsl", ".fx"]:
        commentChar = "//"
    elif extension in [".bash", ".sh", ".py", ".yaml", ".yml", ".ps1", ".pl", ".cmake", ".rb", ".init"] or fileName == "CMakeLists.txt":
        commentChar = "#"
    elif extension in [".bat", ".cmd"]:
        commentChar = "::"

    if commentChar is None:
        raise InvalidFileError("Can't handle extension: \"" + extension + "\"")

    # Get file's creation date
    creationDateStr = cmd(["git", "log", "--diff-filter=A", "--format=%ci", "-1", "--", filePath]).strip()

    if not creationDateStr:
        raise InvalidFileError("File not in git index: \"" + filePath + "\"")

    # We only need the day, month and year
    creationDate = datetime.datetime.fromisoformat(creationDateStr.split()[0])

    copyright = None
    if creationDate.year == datetime.datetime.today().year:
        copyright = str(creationDate.year)
    else:
        copyright = str(creationDate.year) + "-" + str(datetime.datetime.today().year)

    return licenseHeaderTemplate.substitute(commentChar=commentChar, copyright=copyright)


def findCopyright(content, pattern):
    match = re.search(pattern, content)
    
    if match:
        return (match.start(), match.end())
    else:
        raise LicenseNotFoundError()


def exclude(path, excludes):
    for current in excludes:
        if path.startswith(current):
            return True
    
    return False


def main():
    parser = argparse.ArgumentParser(description="Updates (and adds) license headers of the Mumble source files")
    parser.add_argument("-a", "--add-new", action="store_true", default=False, help="Also add license headers to files that did not contain one before")

    args = parser.parse_args()

    scriptDir = os.path.dirname(os.path.realpath(__file__))
    rootDir = os.path.realpath(os.path.join(scriptDir, ".."))

    excludePaths = ["build", "docs", "3rdparty", "3rdPartylLicenses", "themes", ".git", "man"]
    for i in range(len(excludePaths)):
        excludePaths[i] = os.path.join(rootDir, excludePaths[i])

    for i in range(len(blackListFiles)):
        blackListFiles[i] = os.path.join(rootDir, blackListFiles[i])

    # Convert the license template into a pattern that matches the license declaration inside the source files
    licensePattern = re.compile(licenseHeaderTemplate.substitute(copyright="\d+(-\d+)?", commentChar=".+?").strip().replace("\n", "\\r?\\n").replace(" ",
        "\\s*"), re.IGNORECASE)

    modifiedFiles = 0
    for root, dirs, files in os.walk(rootDir, topdown = True):
        for currentFile in files:
            fullPath = os.path.join(root, currentFile)

            if exclude(fullPath, excludePaths):
                continue

            try:
                generatedCopyright = generateHeaderForFile(fullPath)
                
                fileHandle = open(fullPath, "r")
                fileContent = fileHandle.read()

                try:
                    start,end = findCopyright(fileContent, licensePattern)
                except LicenseNotFoundError:
                    # no license yet
                    if not args.add_new:
                        print("No license header found for", fullPath)
                        continue
                    else:
                        print("Adding license header to", fullPath)
                        if not fileContent.startswith("#!"):
                            # File does not start with shebang -> write license to the top
                            start = 0
                            end = 0

                            generatedCopyright += "\n\n"
                        else:
                            # File starts with shebang -> append license after the first line (and add
                            # an extra blank line)
                            start = fileContent.find("\n") + 1
                            assert start > 0
                            end = start

                            generatedCopyright = "\n" + generatedCopyright + "\n\n"

                modifiedContent = fileContent[ : start] + generatedCopyright + fileContent[end : ]

                if modifiedContent != fileContent:
                    fileHandle.close()
                    # Open for writing
                    fileHandle = open(fullPath, "w")

                    modifiedFiles += 1

                    fileHandle.write(modifiedContent)
            except InvalidFileError:
                continue

    print("Modified {} files".format(modifiedFiles))


if __name__ == "__main__":
    main()
