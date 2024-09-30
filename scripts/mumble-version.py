#!/usr/bin/env python3
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# This script reads and prints the current Mumble version

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
    parser.add_argument('-n', '--newline', action = "store_true", help = 'Break line after printing version')
    args = parser.parse_args()

    if args.newline:
        end = None
    else:
        end = ''

    version = readProjectVersion()

    print(version, end = end)



if __name__ == '__main__':
    main()
