#!/usr/bin/env python3
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from typing import List
from typing import Set
from typing import Optional

import argparse
import platform
import subprocess
import re
import sys
from functools import cmp_to_key

from commitMessage.CommitMessage import CommitMessage, CommitFormatError

release_branch_pattern = re.compile(r"(\d+)\.(\d+)\.x")
pr_number_pattern = re.compile(
    r"PR\s*#(\d+)|Pull\s*Request\s*#(\d+)|\(#(\d+)\)", re.IGNORECASE
)


def cmd(args: List[str]) -> str:
    shell: bool = platform.system() == "Windows"
    p = subprocess.Popen(
        args, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        raise Exception(
            "cmd(): {0} failed with status {1}: {2}".format(args, p.returncode, stderr)
        )
    return stdout.decode("utf-8")


def is_merge_commit(commit: str) -> bool:
    """Checks whether the given commit is a merge commit"""
    # See https://stackoverflow.com/a/59425446
    try:
        cmd(["git", "rev-parse", "{}^2".format(commit)])
        return True
    except:
        return False


def get_subject(commit: str) -> str:
    """Gets the subject line from the given commit's commit message"""
    return cmd(["git", "log", "--format=%s", "-n", "1", commit]).strip()


def get_message_body(commit: str) -> str:
    """Gets the body (everything except for the subject) from the given commit's commit message"""
    return cmd(["git", "log", "--format=%b", "-n", "1", commit]).strip()


def is_branch(branch: str) -> bool:
    """Checks whether the given name actually represents an existing branch"""
    try:
        cmd(["git", "show-ref", "--quiet", "--heads", branch])
        return True
    except:
        return False


def is_ancestor(first: str, second: str) -> bool:
    """Check if first is an ancestor of second"""
    try:
        cmd(["git", "merge-base", "--is-ancestor", first, second])
        return True
    except:
        return False


def get_branch_of(ref: str, include_remote_branches: bool = False) -> str:
    """Gets the branch that contains the given reference (could be a tag or a commit hash)"""
    args = ["git", "branch"]
    if include_remote_branches:
        args.append("-a")
    args.append("--contains")
    args.append(ref)

    branches: List[str] = cmd(args).split("\n")

    assert len(branches) > 0, 'Couldn\'t find a branch containing "%s"' % ref

    # remove leading star indicating currently checked out branch
    branches = [x[1:].strip() if x.startswith("*") else x.strip() for x in branches]

    # We prefer the master branch, then release branches (newest release series first) and after that we don't care
    def branch_compare(left: str, right: str) -> int:
        if left == right:
            return 0
        if left in ["master", "main"]:
            return -1
        if right in ["master", "main"]:
            return +1

        left_match = release_branch_pattern.match(left)
        right_match = release_branch_pattern.match(right)

        if left_match and right_match:
            left_major = int(left_match.group(1))
            left_minor = int(left_match.group(2))
            right_major = int(right_match.group(1))
            right_minor = int(right_match.group(2))

            return (right_major - left_major) * int(1e5) + (right_minor - left_minor)

        if left_match:
            return -1
        if right_match:
            return +1

        return 0

    sorted(branches, key=cmp_to_key(branch_compare))

    return branches[0]


def get_commits_between(from_ref: str, to_ref: str) -> List[str]:
    """Gets a list of commit hashs of commits that lie between the two given references"""
    assert is_ancestor(from_ref, to_ref)

    commits = cmd(["git", "rev-list", "{}..{}".format(from_ref, to_ref)]).split("\n")

    commits = [x for x in reversed(commits) if x]

    return commits


def get_commits_in_merge(merge_commit: str) -> List[str]:
    """Gets the commits that were introduced by the given merge commit"""
    return (
        cmd(["git", "rev-list", "{}^1..{}^2".format(merge_commit, merge_commit)])
        .strip()
        .split("\n")
    )


def get_merge_commit_introducing(commit: str, branch: str) -> Optional[str]:
    """Gets the merge commit (if any) that introduced the given commit into the given branch"""
    # See https://stackoverflow.com/a/57509010
    candidates: List[str] = (
        cmd(
            [
                "git",
                "rev-list",
                "--topo-order",
                "--merges",
                "--ancestry-path",
                "--reverse",
                "{}..{}".format(commit, branch),
            ]
        )
        .strip()
        .split("\n")
    )

    assert len(candidates) > 0

    merge_commit: str = candidates[0]

    if merge_commit == "":
        # There is no merge commit
        return None

    # Due to the way we are searching for the merge commits, we have to verify that the found merge commit actually introduces
    # the searched for commit
    return merge_commit if commit in get_commits_in_merge(merge_commit) else None


def get_new_commits(
    reference_branch: str, target_branch: str, since_ref: Optional[str] = None
) -> List[str]:
    """Gets the commits that are new in the target branch compared with the reference branch (aka: that have not between
    backported to the reference branch. If given, only commits newer than since_ref are returned"""
    assert reference_branch != target_branch
    assert is_branch(reference_branch)
    assert is_branch(target_branch)

    args = ["git", "cherry", reference_branch, target_branch]
    if since_ref:
        args.append(since_ref)

    commits = cmd(args).split("\n")

    # git cherry returns a list of SHA1 hashes prefixed with either "-" or "+" indicating that an equivalent commit
    # (in terms of code changes) is already contained in the old branch ("-") or that this is not the case ("+")
    commits = [x[1:].strip() for x in commits if x and x.startswith("+")]

    return commits


def format_change_line(line: str, pr_number: Optional[int], target_format: str) -> str:
    """Formats a line for a changelog entry according to the given format"""
    changeline: str = "- " + line

    if pr_number:
        if target_format == "github":
            changeline += " (#{})".format(str(pr_number))
        elif target_format == "website":
            changeline += " ({{{{< issue {} >}}}})".format(str(pr_number))
        else:
            changeline += " (https://github.com/mumble-voip/mumble/pull/{})".format(
                str(pr_number)
            )

    return changeline


def generate_changelog_from(commits: List[str], branch: str, target_format: str) -> str:
    """Generates the changelog for the given list of commits in the given format"""
    server_changes: List[str] = []
    client_changes: List[str] = []
    shared_changes: List[str] = []
    positional_audio_changes: List[str] = []
    misc_changes: List[str] = []

    skipTypes: Set[str] = set(
        ["FORMAT", "DOCS", "TEST", "MAINT", "CI", "REFAC", "BUILD", "TRANSLATION"]
    )

    for commit_hash in commits:
        if is_merge_commit(commit_hash):
            continue

        merge_commit_hash: Optional[str] = get_merge_commit_introducing(
            commit_hash, branch
        )

        pr_number: Optional[int] = None
        if merge_commit_hash:
            merge_subject: str = get_subject(merge_commit_hash)
            mod_merge_subject = merge_subject.replace(":", " ")

            # Fix for some merge commits with do not follow the convention
            if "  Backport " in mod_merge_subject:
                mod_merge_subject = mod_merge_subject.split("  Backport ")[1]

            match = pr_number_pattern.search(mod_merge_subject)

            if match:
                # Find the first pr_number across the different regex groups.
                # Regex groups are enumerated based on the original regex not
                # on the number of matched parts of the regex...
                for group in range(pr_number_pattern.groups):
                    m = match.group(group)

                    if not m:
                        continue
                    if not m.isnumeric():
                        continue

                    pr_number = int(m)
                    break
            else:
                print(
                    '[WARNING]: Non-conforming merge commit subject "{}"'.format(
                        merge_subject
                    )
                )

        try:
            commit = CommitMessage(get_subject(commit_hash))

            if len(set(commit.m_types) & skipTypes) > 0:
                # We don't list these changes in the changelog
                continue

            targetGroups = []
            if "client" in commit.m_scopes:
                targetGroups.append(client_changes)
            if "server" in commit.m_scopes:
                targetGroups.append(server_changes)
            if "shared" in commit.m_scopes:
                targetGroups.append(shared_changes)
            if "positional-audio" in commit.m_scopes:
                targetGroups.append(positional_audio_changes)
            if "ice" in commit.m_scopes and not "server" in commit.m_scopes:
                targetGroups.append(server_changes)
                commit.m_summary = "Ice: " + commit.m_summary
            if "grpc" in commit.m_scopes and not "server" in commit.m_scopes:
                targetGroups.append(server_changes)
                commit.m_summary = "gRPC: " + commit.m_summary
            if "audio" in commit.m_scopes and not "client" in commit.m_scopes:
                targetGroups.append(client_changes)
            if len(targetGroups) == 0:
                targetGroups.append(misc_changes)

            if "FEAT" in commit.m_types:
                prefix: str = "Added: "
            elif "FIX" in commit.m_types:
                prefix: str = "Fixed: "
            elif "CHANGE" in commit.m_types:
                prefix: str = "Changed: "
            elif "REVERT" in commit.m_types:
                prefix: str = "Reverted: "
            else:
                prefix: str = "Unknown: "

            for currentTarget in targetGroups:
                currentTarget.append(
                    format_change_line(
                        prefix + commit.m_summary, pr_number, target_format
                    )
                )
        except CommitFormatError:
            # We can't classify the change -> assume misc
            misc_changes.append(
                format_change_line(
                    "Unknown: " + get_subject(commit_hash), pr_number, target_format
                )
            )

    changelog: str = ""
    if len(client_changes) > 0:
        changelog += "### Client\n\n"
        changelog += "\n".join(sorted(client_changes)) + "\n\n"

    if len(server_changes) > 0:
        changelog += "### Server\n\n"
        changelog += "\n".join(sorted(server_changes)) + "\n\n"

    if len(shared_changes) > 0:
        changelog += "### Both\n\n"
        changelog += "\n".join(sorted(shared_changes)) + "\n\n"

    if len(positional_audio_changes) > 0:
        changelog += "### Positional audio plugins\n\n"
        changelog += "\n".join(sorted(positional_audio_changes)) + "\n\n"

    if len(misc_changes):
        changelog += "### Miscellaneous\n\n"
        changelog += "\n".join(sorted(misc_changes)) + "\n\n"

    return changelog


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("FROM_REF", help="Tag or commit to start the diff from")
    parser.add_argument("TO_REF", help="Tag or commit to end the diff at")
    parser.add_argument(
        "--target",
        choices=["github", "website", "other"],
        help="Generate the Changelog in a format suitable for this target",
        default="other",
    )
    args = parser.parse_args()

    from_branch: str = (
        get_branch_of(args.FROM_REF) if not is_branch(args.FROM_REF) else args.FROM_REF
    )
    to_branch: str = (
        get_branch_of(args.TO_REF) if not is_branch(args.TO_REF) else args.TO_REF
    )

    if from_branch == to_branch:
        # We perform a diff within a single branch
        if not is_ancestor(args.FROM_REF, args.TO_REF):
            print(
                "[ERROR]: FROM_REF is not an ancestor of TO_REF - maybe you switched the arguments?"
            )
            sys.exit(1)
        commits = get_commits_between(args.FROM_REF, args.TO_REF)
    else:
        # We perform a diff across different branches
        since_ref = args.TO_REF if args.TO_REF != to_branch else None
        commits = get_new_commits(
            reference_branch=from_branch, target_branch=to_branch, since_ref=since_ref
        )

    print(generate_changelog_from(commits, to_branch, args.target))


if __name__ == "__main__":
    main()
