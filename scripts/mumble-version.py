#!/usr/bin/env python
#
# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# This script returns the Mumble version string for a Mumble Git
# repository. The script must be run from within a Mumble Git
# repository.
# This is a replacement for `git describe` to make snapshots
# use the future, untagged version number rather than the previous.
#
# The version is of form 1.3.0~154~g4f336a2~snapshot.
# It includes the target release version rather than the previous
# release (as git describe does).
#
# Detailed description:
#
# Once upon a time, Mumble used the output of `git describe` as
# its version string.
#
# If a commit was tagged, it was a "release", and got a simple
# string which was the name of the tag.
#
# If a commit wasn't tagged, it got '1.2.6-234-gf552ag1', which
# consists of the number of commits since the latest tag, and
# the commit hash of the latest commit.
#
# However, the output of `git describe` was found to be confusing
# in practice. This is because the base version of the `git describe`
# output is the latest tag, which is to say: the *previous*
# version of Mumble.
#
# So, a user running a snapshot that would become 1.3.0 would be
# running a version like 1.2.6-234-gf552ag1. This is confusing
# simply by looking at the version numbers, but the way versioning
# works inside the Mumble client made it worse: the client's version
# was refered to as "1.3.0" in several places, but the actual version
# number said 1.2.6-234-gf552ag1.
#
# This script is the replacement for `git describe`. It outputs the
# *actual* base version of the Mumble tree, rather than the latest tag.
# This means that snapshots for Mumble 1.3.0 now have the base version
# '1.3.0'.
#
# It also changes the version string slightly. Instead of using dashes
# as a separator in the version string, it now uses tildes. This allows
# Debian's dpkg version comparer to correctly sort snapshot versions
# before release versions. The new string also includes 'snapshot' in the
# version string to denote to users that the given version is a pre-release
# snapshot. A full new-style version string looks like this:
# 1.3.0~154~g4f336a2~snapshot.

from __future__ import (unicode_literals, print_function, division)

import os
import platform
import subprocess
import sys

def strip(s):
	s = s.replace('\r', '')
	s = s.replace('\n', '')
	return s

def cmd(args):
	shell = platform.system() == 'Windows'
	p = subprocess.Popen(args, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	stdout, stderr = p.communicate()
	if p.returncode != 0:
		raise Exception('cmd: {0} failed with status {1}: {2}'.format(args, p.returncode, stderr))
	return stdout.decode('utf-8')

# Reads the version from src/mumble.pri
def readMumblePriVersion():
	sourceTreeRoot = strip(cmd(['git', 'rev-parse', '--show-toplevel']))

	version = None
	with open(os.path.join(sourceTreeRoot, 'src', 'mumble.pri'), 'r') as f:
		for line in f:
			if 'VERSION' in line:
				line = line.replace('VERSION', '')
				line = line.replace('=', '')
				line = line.replace('\t', '')
				line = line.replace(' ', '')
				line = strip(line)
				version = line
				break
	if version is None:
		raise Exception('unable to read version from mumble.pri')
	return version

def main():
	latestTag = cmd(['git', 'describe', '--abbrev=0', '--tags'])
	latestTag = strip(latestTag)
	if len(latestTag) == 0:
		raise Exception('empty latestTag, unable to continue')

	latestCommit = cmd(['git', 'rev-parse', 'HEAD'])
	if len(latestCommit) < 7:
		raise Exception('bad commit string: {0}'.format(latestCommit))
	latestCommit = strip(latestCommit)

	revListStr = cmd(['git', 'rev-list', '{0}..HEAD'.format(latestTag)])
	revList = revListStr.split('\n')
	nrevs = len(revList)-1 # Consider the newline at the end.
	version = ''
	if nrevs == 0:
		# The most recent tag is the latest commit. That means this must
		# be a tagged release version.
		# We verify that the tag commit is the current HEAD to make sure it is.
		revListStr = cmd(['git', 'rev-list', latestTag])
		revList = revListStr.split('\n')
		if len(revList) == 0:
			raise Exception('unable to get rev-list for potential release tag')
		latestCommitForLatestTag = revList[0]
		if latestCommitForLatestTag != latestCommit:
			raise Exception('commit-hash mismatch; aborting potential relase version string')
		version = latestTag
	else:
		mumblePriVersion = readMumblePriVersion()
		if len(mumblePriVersion) == 0 or not '.' in mumblePriVersion:
			raise Exception('bad mumblePriVersion: "{0}"'.format(mumblePriVersion))

		version = '{0}~{1}~g{2}~snapshot'.format(mumblePriVersion, nrevs, latestCommit[0:7])

	end = ''
	if '--newline' in sys.argv:
		end = None
	print(version, end=end)

if __name__ == '__main__':
	main()
