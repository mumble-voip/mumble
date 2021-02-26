#!/usr/bin/env python3
#
# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

import argparse
import platform
import subprocess

from commitMessage.CommitMessage import CommitMessage, CommitFormatError

def cmd(args):
    shell = platform.system() == 'Windows'
    p = subprocess.Popen(args, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        raise Exception('cmd(): {0} failed with status {1}: {2}'.format(args, p.returncode, stderr))
    return stdout.decode('utf-8')

def isRelevantCommit(commitLine):
    if commitLine.strip() == "":
        return False
    
    components = commitLine.split()

    if len(components) < 2:
        return False

    # First component is commit hash
    # We only consider merge commits to be relevant
    return components[1].lower() == "merge"

def formatChangeLine(line, prNumber, target):
    if target == "github":
        return "- {} (#{})".format(line, prNumber)
    elif target == "website":
        return "- {} ({{{{< issue {} >}}}})".format(line, prNumber)
    else:
        return "- {} (https://github.com/mumble-voip/mumble/pull/{})".format(line, prNumber)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("FROM_TAG", help="Tag or commit to start the diff from")
    parser.add_argument("TO_TAG", help="Tag or commit to end the diff at")
    parser.add_argument("--target", choices=["github", "website", "other"], help="Generate the Changelog in a format suitable for this target",
            default="other")
    args = parser.parse_args()

    commits = cmd(["git", "log" ,"--format=oneline",  "--date=short", "{}..{}".format(args.FROM_TAG, args.TO_TAG)]).split("\n")
    commits = list(filter(isRelevantCommit, commits))

    serverChanges = []
    clientChanges = []
    sharedChanges = []
    miscChanges   = []

    skipTypes = set(["FORMAT", "DOCS", "TEST", "MAINT", "CI", "REFAC", "BUILD", "TRANSLATION"])

    for commitLine in commits:
        parts = commitLine.split(maxsplit=1)
        commitHash = parts[0]
        commitTitle = parts[1]

        assert ":" in commitTitle
        assert "#" in commitTitle

        prTagStart = commitTitle.find("#")
        prTagEnd = commitTitle.find(":")
        assert prTagStart + 1 < prTagEnd

        # Extract PR number
        prNumber = commitTitle[prTagStart + 1 : prTagEnd]
        # Cut out PR information from commit title
        commitTitle = commitTitle[prTagEnd + 1 : ].strip()

        try:
            commit = CommitMessage(commitTitle)

            if len(set(commit.m_types) & skipTypes) > 0:
                # We don't list these changes in the changelog
                continue

            targetGroups = []
            if "client" in commit.m_scopes:
                targetGroups.append(clientChanges)
            if "server" in commit.m_scopes:
                targetGroups.append(serverChanges)
            if "shared" in commit.m_scopes:
                targetGroups.append(sharedChanges)
            if len(targetGroups) == 0:
                targetGroups.append(miscChanges)

            prefix = ""
            if "FEAT" in commit.m_types:
                prefix = "Added: "
            elif "FIX" in commit.m_types:
                prefix = "Fixed: "
            else:
                prefix = "Unknown: "

            for currentTarget in targetGroups:
                currentTarget.append(formatChangeLine(prefix + commit.m_summary, prNumber, args.target))
        except CommitFormatError:
            # We can't classify the change -> assume misc
            miscChanges.append(formatChangeLine("Unknown: " + commitTitle, prNumber, args.target))


    if len(clientChanges) > 0:
        print("### Client")
        print()
        print("\n".join(sorted(clientChanges)))
        print()
        print()

    if len(serverChanges) > 0:
        print("### Server")
        print()
        print("\n".join(sorted(serverChanges)))
        print()
        print()

    if len(sharedChanges) > 0:
        print("### Both")
        print()
        print("\n".join(sorted(sharedChanges)))
        print()
        print()

    if len(miscChanges):
        print("### Miscellaneous")
        print()
        print("\n".join(sorted(miscChanges)))
        print()



if __name__ == "__main__":
    main()
