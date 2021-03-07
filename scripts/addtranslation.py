#!/usr/bin/env python3
#
# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Adds the translation file src/mumble/mumble_*.ts from source and commits the changes.
# The parameter --lang specifies the language locale to use.
#
# Requires the qt5 tool lupdate ; sudo apt-get install libqt5-dev

import os, glob, logging, sys, subprocess, re
import argparse
from shutil import which
from typing import Optional

def FindLupdate(vcpkg_triplet: Optional[str] = None) -> Optional[str]:
    logging.debug('Looking for lupdate…')
    if which('lupdate') is not None:
        return 'lupdate'
    if vcpkg_triplet is not None:
        vcpkgbin = os.path.join(os.path.expanduser('~'), 'vcpkg', 'installed', vcpkg_triplet, 'tools', 'qt5', 'bin')
        logging.debug('Looking for lupdate in %s…', vcpkgbin)
        return which('lupdate', path=vcpkgbin)
    return None

def Commit(tsfiles: list, locale: str) -> None:
    res = subprocess.run(['git', 'reset', '--mixed'], capture_output=True)
    if res.returncode != 0:
        logging.error('The git reset call returned an error status code %d', res.returncode)
        logging.debug('stdout: %s', res.stdout)
        exit(1)
    res = subprocess.run(['git', 'add'] + tsfiles, capture_output=True)
    if res.returncode != 0:
        logging.error('The git add call returned an error status code %d', res.returncode)
        logging.debug('stdout: %s', res.stdout)
        exit(1)
    res = subprocess.run(['git', 'commit', '-m', 'TRANSLATION: Add new language %s' % locale], capture_output=True)
    if res.returncode != 0:
        logging.error('The git commit call returned an error status code %d', res.returncode)
        logging.debug('stdout: %s', res.stdout)
        exit(1)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--vcpkg-triplet', type=str, required=False, help='the vcpkg triplet to use the lupdate tool from, e.g. "x64-windows-static-md"')
    parser.add_argument('--debug', dest='debug', action='store_true')
    parser.add_argument('--debug-lupdate', dest='debuglupdate', action='store_true')
    parser.add_argument('--lang', type=str, required=True, help='Language code (2 characters) to add')
    parser.set_defaults(debug=False, debuglupdate=False)
    args = parser.parse_args()

    loglevel = logging.DEBUG if args.debug else logging.INFO
    logging.basicConfig(stream=sys.stdout, level=loglevel, format='%(levelname)s: %(message)s')

    lupdatebin = FindLupdate(args.vcpkg_triplet)
    if lupdatebin is None:
        logging.error('Could not find `lupdate` executable')
        exit(1)

    # cd into repository root directory
    os.chdir(os.path.dirname(os.path.abspath(__file__)) + '/..')
    logging.info('Working in directory %s', os.getcwd())

    tsfiles = glob.glob(os.path.join('src', 'mumble', 'mumble_*.ts'))
    logging.debug('Identified these ts files: %s', tsfiles)

    logging.info('Ensuring ts file does not exist yet…')
    targetPath = os.path.join('src', 'mumble', f'mumble_{args.lang}.ts')
    if os.path.exists(targetPath):
        logging.error(f'A translation file for the specified locale {args.lang} already exists at {targetPath}')
        exit(1)

    logging.info(f'Creating ts file {targetPath}…')
    res = subprocess.run([
        lupdatebin
        , '-no-ui-lines'
        # {sametext|similartext|number}
        , '-disable-heuristic', 'similartext'
        # {absolute|relative|none}
        , '-locations', 'none'
        , '-no-obsolete'
        , '-no-recursive'
        , '-extensions', 'ui,c,cpp,h,mm'
        # sources
        , './src', './src/mumble'
        # target
        , '-ts', targetPath
        ], capture_output=True)
    if args.debuglupdate:
        logging.debug(res.stdout)
    if res.returncode > 0:
        logging.error('lupdate returned with error status code %d', res.returncode)
        logging.debug('stdout: %s', res.stdout)
        exit(1)

    logging.info('Committing changes…')
    Commit([targetPath], args.lang)

    logging.info('Adding the translation file successfully completed.')
