#!/usr/bin/env python3
#
# Copyright 2021-2022 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

import argparse
import platform
import subprocess
import re

from commitMessage.CommitMessage import CommitMessage, CommitFormatError

def cmd(args):
    shell = platform.system() == 'Windows'
    p = subprocess.Popen(args, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        raise Exception('cmd(): {0} failed with status {1}: {2}'.format(args, p.returncode, stderr))
    return stdout.decode('utf-8')

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

    mergeCommitPattern = re.compile("^([0-9a-f]+)\s*[Mm]erge\s.+?#(\d+):?\s*(.+)$", re.MULTILINE)
    backportCommitPattern = re.compile("^[Bb]ackport\s*\"(.*)\".*$")
    mergePrefixPattern = re.compile("^Merge PR #\d+:")

    commits = cmd(["git", "log" ,"--format=oneline",  "--date=short", "{}..{}".format(args.FROM_TAG, args.TO_TAG)]).split("\n")

    serverChanges          = []
    clientChanges          = []
    sharedChanges          = []
    positionalAudioChanges = []
    miscChanges            = []

    skipTypes = set(["FORMAT", "DOCS", "TEST", "MAINT", "CI", "REFAC", "BUILD", "TRANSLATION"])

    for commitLine in commits:
        match = re.match(mergeCommitPattern, commitLine)

        if not match:
            # Commit doesn't match the expected pattern and is thus ignored
            continue

        commitHash = match.group(1)
        prNumber = match.group(2)
        commitTitle = match.group(3)

        backportMatch = re.match(backportCommitPattern, commitTitle)
        if backportMatch:
            # This commit is a backport commit where the actual commit title is the bit in the quotes
            commitTitle = backportMatch.group(1)

            if re.match(mergePrefixPattern, commitTitle):
                commitTitle = commitTitle[ commitTitle.find(":") + 1 : ]

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
            if "positional-audio" in commit.m_scopes:
                targetGroups.append(positionalAudioChanges)
            if "ice" in commit.m_scopes and not "server" in commit.m_scopes:
                targetGroups.append(serverChanges)
                commit.m_summary = "Ice: " + commit.m_summary
            if "grpc" in commit.m_scopes and not "server" in commit.m_scopes:
                targetGroups.append(serverChanges)
                commit.m_summary = "gRPC: " + commit.m_summary
            if "audio" in commit.m_scopes and not "client" in commit.m_scopes:
                targetGroups.append(clientChanges)
            if len(targetGroups) == 0:
                targetGroups.append(miscChanges)

            prefix = ""
            if "FEAT" in commit.m_types:
                prefix = "Added: "
            elif "FIX" in commit.m_types:
                prefix = "Fixed: "
            elif "CHANGE" in commit.m_types:
                prefix = "Changed: "
            elif "REVERT" in commit.m_types:
                prefix = "Reverted: "
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

    if len(positionalAudioChanges) > 0:
        print("### Positional audio plugins")
        print()
        print("\n".join(sorted(positionalAudioChanges)))
        print()
        print()

    if len(miscChanges):
        print("### Miscellaneous")
        print()
        print("\n".join(sorted(miscChanges)))
        print()



if __name__ == "__main__":
    main()
