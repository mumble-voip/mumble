#!/usr/bin/env python
#
# Copyright 2019-2021 The Mumble Developers. All rights reserved.
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
# The version is of form 1.3.0~2020-07-02~g4f336a2~snapshot.
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
# was referred to as "1.3.0" in several places, but the actual version
# number said 1.2.6-234-gf552ag1.
#
# This script is the replacement for `git describe`. It outputs the
# *actual* base version of the Mumble tree, rather than the latest tag.
# This means that snapshots for Mumble 1.3.0 now have the base version
# '1.3.0'.
#
# It also changes the version string: Instead of using dashes
# as a separator in the version string, it now uses tildes. This allows
# Debian's dpkg version comparer to correctly sort snapshot versions
# before release versions. The new string also includes 'snapshot' in the
# version string to denote to users that the given version is a pre-release
# snapshot.
# Furthermore the version number does use the latest commit's date rather
# than therevision number since the last tag.
# A full new-style version string looks like this:
# 1.3.0~2020-07-02~g4f336a2~snapshot.

from __future__ import (unicode_literals, print_function, division)

import argparse
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
        raise Exception('cmd(): {0} failed with status {1}: {2}'.format(args, p.returncode, stderr))
    return stdout.decode('utf-8')

# Reads the version from CMakeLists.txt
def readProjectVersion():
    sourceTreeRoot = strip(cmd(['git', 'rev-parse', '--show-toplevel']))

    version = None
    with open(os.path.join(sourceTreeRoot, 'CMakeLists.txt'), 'r') as f:
        for line in f:
            # The version is specified as e.g. VERSION "1.4.0.${BUILD_NUMBER}"
            if 'VERSION "' in line and '.${BUILD_NUMBER}"' in line:
                line = line.replace('VERSION "', '')
                line = line[0 : line.find('.${BUILD_NUMBER}"')].strip()
                version = line
                break

    if version is None:
        raise Exception('Unable to read version from CMakeLists.txt')

    if len(version) == 0 or not '.' in version:
            raise Exception('Bad version: "{0}"'.format(version))

    return version

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--format', choices = ['full', 'version', 'suffix'], default='full', help = 'Output format')
    parser.add_argument('-n', '--newline', action = "store_true", help = 'Break line after printing version')
    parser.add_argument('-r', '--revision', type = int, default = 1, help = 'Revision (only used for type \'beta\' and \'rc\')')
    parser.add_argument('-t', '--type', choices = ['snapshot', 'beta', 'rc', 'stable'], default = 'snapshot', help = 'Release type - determines the suffix')
    args = parser.parse_args()

    if args.newline:
        end = None
    else:
        end = ''

    version = readProjectVersion()

    if args.format == 'version':
        print(version, end = end)
        return

    suffix = ''

    if args.type == 'rc' or args.type == 'beta':
        suffix = '-{0}{1}'.format(args.type, args.revision)
    elif args.type == 'snapshot':
        # Get the date of the most recent commit
        latestCommitDate = cmd(['git', 'log', '-1', '--format=%cd', '--date=short']).strip()

        # Get the hash of the most recent commit (shortened)
        latestCommitHash = cmd(['git', 'rev-parse', '--short', 'HEAD']).strip()

        suffix = '~{0}~g{1}~snapshot'.format(latestCommitDate, latestCommitHash)

    if args.format == 'suffix':
        print(suffix, end = end)
    else:
        print(version + suffix, end = end)

if __name__ == '__main__':
    main()
