#!/usr/bin/env python3
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# This script is meant to be used for backporting translations from a target branch to a different branch
# Only translations that already exist in the base branch will be updated to match the corresponding
# translations in the target branch


import xml.etree.ElementTree as xml
import argparse
import os
import subprocess
import platform
from io import BytesIO
import re

htmlEscapeCodeEscape = "__(-ThisOnceWasAnAmpersand-)__"


def cmd(args):
    shell = platform.system() == 'Windows'
    p = subprocess.Popen(args, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        raise Exception('cmd(): {0} failed with status {1}: {2}'.format(args, p.returncode, stderr))
    return stdout.decode('utf-8')


def assertNoPendingChanges(filePath):
    if len(cmd(["git", "status", "--porcelain", "--", filePath])) > 0:
        raise RuntimeError("File \"%s\" has pending changes" % filePath)


def findTranslationFor(source, context, comment, translationRoot):
    for currentCtx in translationRoot.findall("context"):
        if currentCtx.find("name").text == context:
            for currentMsg in currentCtx.findall("message"):
                if currentMsg.find("source").text == source:
                    currentComment = ""
                    if not currentMsg.find("comment") is None:
                        currentComment = currentMsg.find("comment").text

                    if comment == currentComment:
                        return currentMsg.find("translation").text
            
            # Translation not found
            return ""

    return ""


def parseFile(filePath):
    with open(filePath, "r") as openFile:
        contents = openFile.read()

        # Replace the ampersand in all HTML codes in order to prevent automatic un-escaping
        pattern = re.compile(r"&((?:#(?:\d|\w)+|\w+);)")
        contents = re.sub(pattern, r"%s\1" % htmlEscapeCodeEscape, contents)

        return xml.fromstring(contents)


def main():
    parser = argparse.ArgumentParser("Backports translation changes from the given branch to the current one")
    parser.add_argument("FROM_BRANCH", help="Branch to fetch the translations from")
    parser.add_argument("--translation-dir", help="The directory in which the translation files are located", metavar="PATH", default="")

    args = parser.parse_args()

    # Get the name of the branch we're currently on in order to print a helpful message
    currentBranchName = cmd(["git", "rev-parse", "--abbrev-ref", "HEAD"]).strip()
    print("Backporting translations from \"%s\" to \"%s\"" % (args.FROM_BRANCH, currentBranchName))


    if not args.translation_dir:
        # Assume translation files are located at ../src/mumble relative to this script's location
        scriptDir = os.path.dirname(os.path.realpath(__file__))
        args.translation_dir = os.path.join(scriptDir, "..", "src", "mumble")


    # Get all translation files in the source directory
    tsFiles = [os.path.join(args.translation_dir, current) for current in os.listdir(args.translation_dir) if current.endswith(".ts")]

    for currentTranslationFile in tsFiles:
        # First make sure there are no pending changes for these files
        assertNoPendingChanges(currentTranslationFile)

        # Parse the current state of the file
        currentTranslationRoot = parseFile(currentTranslationFile)

        if not "language" in currentTranslationRoot.attrib:
            # This is the source translation file that only contains source strings
            continue

        language = currentTranslationRoot.attrib["language"]
        print("Processing \"{}\"...".format(language))


        try:
            # Checkout the file in its state in the target branch and parse the state from there
            cmd(["git", "checkout", args.FROM_BRANCH, "--", currentTranslationFile])
            updatedTranslationRoot = parseFile(currentTranslationFile) 
        finally:
            pass
            # Always restore to the current branch's state afterwards
            cmd(["git", "reset", "--", currentTranslationFile])
            cmd(["git", "restore", "--", currentTranslationFile])

        # Go through all translations in the current state, locate the translation for any particular source string
        # in the state from the target branch and of the translations differ, use the one from the target branch
        changedTranslations = 0
        for currentCtx in currentTranslationRoot.findall("context"):
            context = currentCtx.find("name").text

            for currentMsg in currentCtx.findall("message"):
                source = currentMsg.find("source").text
                translationElement = currentMsg.find("translation")
                translation = translationElement.text

                comment = ""
                if not currentMsg.find("comment") is None:
                    comment = currentMsg.find("comment").text

                newTranslation = findTranslationFor(source, context, comment, updatedTranslationRoot)

                if not newTranslation == "" and translation != newTranslation:
                    # Overtake new translation
                    translationElement.text = newTranslation
                    translationElement.attrib = {}

                    changedTranslations += 1


        if changedTranslations > 0:
            # The translations have changed and thus we need to update our TS file
            contents = xml.tostring(currentTranslationRoot, method="xml", encoding="utf-8", xml_declaration=True, short_empty_elements=False)

            contents = contents.decode("UTF-8")
            # Use double quotes for the XML header as Qt Linguist does (but ElementTree does not)
            # Also add the "<!DOCTYPE TS>" comment after that (again just as Qt Linguist does)
            contents = contents.replace("<?xml version='1.0' encoding='utf-8'?>", "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<!DOCTYPE TS>")

            # We have previously replaced the ampersand in all HTML escape codes in order to prevent Python from unescaping these for us. This is
            # because ElementTree unescapes them all but only re-escapes the ones that are strictly necessary. Thus that would create a huge diff
            # where nothing actually changed.
            # By having replaced the escape code's ampersands before we can now undo that and be sure to only replace at the appropriate
            # places.
            contents = contents.replace(htmlEscapeCodeEscape, "&")

            # Make sure the translation files end with a newline (as Qt Linguist does)
            if not contents.endswith("\n"):
                contents += "\n"

            # Actually write the file's new content to disk
            with open(currentTranslationFile, "w") as targetFile:
                targetFile.write(contents)


            print("  -> {} translation(s) changed".format(changedTranslations))


if __name__ == "__main__":
    main()
