#!/usr/bin/env python
#
# Copyright 2015-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from __future__ import (unicode_literals, print_function, division)

import os
import platform
import sys

allowed_prefixes = ('qt', 'qtbase')
override_qt = []

def parseTranslationsConfig(configFile):
	configHandle = open(configFile, "r")

	for currentLine in configHandle.readlines():
		currentLine = currentLine.strip()
		# Skip comments and empty lines
		if currentLine.startswith("#") or not currentLine:
			continue

		# A config entry is supposed to be in the format <operator> <fileName>
		splitParts = currentLine.split(" ", 1)
		if len(splitParts) != 2:
			raise RuntimeError("Invalid line in translation config file: %s" % currentLine)

		operator = splitParts[0].lower().strip()
		translationFileName = splitParts[1].strip()

		if not translationFileName:
			raise RuntimeError("Empty filename in translation config: %s" % currentLine)

		if not translationFileName.endswith(".ts"):
			raise RuntimeError("Expected translation file to have a '*.ts' name but got %s" % translationFileName)

		# Replace the trailing .ts with .qm as this is what lrelease will turn it into
		translationFileName = translationFileName[:-3] + ".qm"

		if operator == "fallback":
			# fallback files are the default, so no special action has to be taken
			pass
		# be programmer friendly and allow "override" as well
		elif operator == "overwrite" or operator == "override":
			override_qt.append(translationFileName)


def dirToQrc(of, dirName, alreadyProcessedLangs, localTranslationDir = False):
	processedLangs = []
	absDirName = os.path.abspath(dirName)
	fns = os.listdir(dirName)
	for fn in fns:
		isOverride = False
		if fn in override_qt and localTranslationDir:
			# This translation should be used to overwrite an existing Qt-translation.
			isOverride = True

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
		if lang in alreadyProcessedLangs and not isOverride:
			continue
		if not prefix in allowed_prefixes:
			continue
		absFn = os.path.join(absDirName, fn)
		if platform.system() == 'Windows':
			absFn = absFn.replace('\\', '/')
		
		if not isOverride:
			of.write(' <file alias="{0}">{1}</file>\n'.format(fn, absFn))
			processedLangs.append(lang)
		else:
			# In order to recognize translation-overrides, we have to prefix them
			of.write(' <file alias="{0}">{1}</file>\n'.format("mumble_overwrite_" + fn, absFn))

	return processedLangs

def main():
	# python generate-mumble_qt-qrc.py <output-fn> [inputs...] localDir
	output = sys.argv[1]
	inputs = sys.argv[2:-1]
	localDir = sys.argv[-1]

	# parse config file
	if localDir.endswith("/") or localDir.endswith("\\"):
		localDir = localDir[:-1]

	configFile = os.path.join(localDir, "translations.conf")
	if os.path.isfile(configFile):
		parseTranslationsConfig(configFile)

	of = open(output, 'w')
	of.write('<!DOCTYPE RCC><RCC version="1.0">\n')
	of.write('<qresource>\n')
	processedLangs = []
	for dirName in inputs:
		newlyProcssedLangs = dirToQrc(of, dirName, processedLangs)
		processedLangs.extend(newlyProcssedLangs)
	# Process translations provided by Mumble itself (aka local translations)
	dirToQrc(of, localDir, processedLangs, True)
	of.write('</qresource>\n')
	of.write('</RCC>\n')
	of.close()

if __name__ == '__main__':
	main()
