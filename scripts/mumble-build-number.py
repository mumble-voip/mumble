#!/usr/bin/env python3
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# This script fetches the build number for a given commit

import argparse
import urllib.request
import urllib.parse
import sys

def fetch_build_number(commit = None, version = None, password = None):
    if commit is None or version is None:
        return None

    parameter = {
        "commit": commit,
        "version": version
    }

    if not password is None:
        parameter["token"] = password

    query = "https://mumble.info/get-build-number?" + urllib.parse.urlencode(parameter)

    try:
        request = urllib.request.urlopen(query)
        answer = request.read().decode("utf-8")

        return int(answer)
    except urllib.error.HTTPError:
        # The request failed
        return None

def main():
    parser = argparse.ArgumentParser("This script will fetch the build number for the given commit hash")
    parser.add_argument("--commit", help="The hash of the commit to fetch the build number for", metavar="HASH", required=True)
    parser.add_argument("--version", help="The Mumble version the given commit belongs to (e.g. 1.4)", metavar="VERSION", required=True)
    parser.add_argument("--password", help="The password to use in order to gain write access on the server")
    parser.add_argument("--default", help="The default build number to use, in case none can be fetched. Note that this will cause this script to\
            always succeed.", type=int)
    args = parser.parse_args()

    if not args.password is None and args.password.strip() == "":
        # An empty password is considered to be no password at all
        args.password = None

    buildNumber = fetch_build_number(commit = args.commit, version = args.version, password = args.password)

    if buildNumber is None and not args.default is None:
        buildNumber = args.default

    if buildNumber is None:
        print("[ERROR]: Failed to fetch the build number for commit " + args.commit + " in Mumble v" + args.version, file=sys.stderr)

        sys.exit(1)
    else:
        print(buildNumber)
        sys.exit(0)


if __name__ == "__main__":
    main()
