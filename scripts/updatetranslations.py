#!/usr/bin/env python3
#
# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Updates the translation files src/mumble/mumble_*.ts from source and commits the changes.
#
# The main actions this script performs:
# * lupdate to update translation strings
# * Commit the resulting translation file
#
# Requires qt5 ; sudo apt-get install libqt5-dev

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

def CheckForGitHasTsFileChanges(tsfiles: list) -> bool:
    res = subprocess.run(['git', 'status', '--porcelain', '--'] + tsfiles, capture_output=True)
    return res.returncode == 0 and len(res.stdout) > 0

def Commit(tsfiles: list) -> None:
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
    res = subprocess.run(['git', 'commit', '-m', 'TRANSLATION: Update translation files'], capture_output=True)
    if res.returncode != 0:
        logging.error('The git commit call returned an error status code %d', res.returncode)
        logging.debug('stdout: %s', res.stdout)
        exit(1)

def Update(lupdatebin, tsfile: str, debuglupdate: bool) -> (int, int, int):
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
        , '-ts', tsfile
        ], capture_output=True)
    if debuglupdate:
        logging.debug(res.stdout)
    if res.returncode != 0:
        logging.error('lupdate failed with error code %d', res.returncode)
        logging.debug('stdout: ' + res.stdout)
        exit(1)
    p = re.compile('Found (?P<nsrc>[0-9]+) source text\(s\) \((?P<nnew>[0-9]+) new and (?P<nsame>[0-9]+) already existing\)')
    m = p.search(res.stdout.decode('ascii'))
    logging.debug('Found %s texts where %s new and %s same', m.group('nsrc'), m.group('nnew'), m.group('nsame'))
    return (m.group('nsrc'), m.group('nnew'), m.group('nsame'))

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--vcpkg-triplet', type=str, required=False, help='the vcpkg triplet to use the lupdate tool from, e.g. "x64-windows-static-md"')
    parser.add_argument('--debug', dest='debug', action='store_true')
    parser.add_argument('--debug-lupdate', dest='debuglupdate', action='store_true')
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

    logging.info('Ensuring ts files have no local changes…')
    if CheckForGitHasTsFileChanges(tsfiles):
        logging.error('There are ts files with local changes. Cancelling execution.')
        exit(1)

    logging.info('Updating ts files…')
    nsrc: Optional[int] = None
    nnew: Optional[int] = None
    nsame: Optional[int] = None
    mismatch = False
    for tsfile in tsfiles:
        logging.debug('Updating ts file ' + tsfile + '…')
        (resnsrc, resnnew, resnsame) = Update(lupdatebin, tsfile, args.debuglupdate)
        if nsrc is None:
            nsrc = resnsrc
            nnew = resnnew
            nsame = resnsame
        else:
            if nsrc != resnsrc or nnew != resnnew or nsame != resnsame:
                logging.warn('Mismatching counts of updating changes between ts files: %s %s %s vs %s %s %s', nsrc, nnew, nsame, resnsrc, resnnew, resnsame)
                mismatch = True

    if nsrc is not None:
        logging.info('Found %s texts where %s new and %s same', nsrc, nnew, nsame)

    if CheckForGitHasTsFileChanges(tsfiles):
        logging.info('Committing changes…')
        Commit(tsfiles)
    else:
        logging.info('No changes to commit.')

    logging.info('The updating of the translation files successfully completed.')
