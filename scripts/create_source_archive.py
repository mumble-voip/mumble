#!/usr/bin/env python3

# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

from typing import List
from typing import Optional

import tarfile
import zipfile
import os
import argparse
import platform
import subprocess
import tempfile


def git(args: List[str]) -> str:
    """Executes git with the given arguments"""
    shell: bool = platform.system() == "Windows"
    args.insert(0, "git")
    p = subprocess.Popen(
        args, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        raise Exception(
            "cmd(): {0} failed with status {1}: {2}".format(args, p.returncode, stderr)
        )
    return stdout.decode("utf-8").strip()


def get_file_paths(base_dir: str) -> List[str]:
    """Gets a list of all files within the file tree rooted at the given directory. Hidden files are excluded"""
    files: List[str] = []

    for current_file in os.listdir(base_dir):
        if current_file.startswith("."):
            # Skip hidden files
            continue

        full_path: str = os.path.join(base_dir, current_file)

        if os.path.isdir(full_path):
            files += get_file_paths(full_path)
        else:
            files.append(full_path)

    return files


def create_zip_archive(
    files: List[str],
    archive_name: str,
    rel_to: str,
    compression: int = zipfile.ZIP_DEFLATED,
) -> str:
    """Writes the given set of files into a zip archive (using compression, if desired)"""
    top_level_dir: str = os.path.basename(archive_name)
    archive_name += ".zip"

    with zipfile.ZipFile(archive_name, mode="x", compression=compression) as archive:
        for current_file in files:
            archive.write(
                current_file,
                arcname=os.path.join(
                    top_level_dir, os.path.relpath(current_file, rel_to)
                ),
            )

    return archive_name


def create_tar_archive(
    files: List[str], archive_name: str, rel_to: str, compression: Optional[str] = "gz"
) -> str:
    """Writes the given set of files into a tar archive (using compression, if desired)"""

    def tar_filter(tarinfo):
        tarinfo.uid = 0
        tarinfo.uname = ''
        tarinfo.gid = 0
        tarinfo.gname = ''
        return tarinfo

    top_level_dir: str = os.path.basename(archive_name)
    archive_name += ".tar"
    if compression:
        archive_name += "." + compression

    with tarfile.open(
        archive_name, mode="x:" + (compression if compression else "")
    ) as archive:
        for current_file in files:
            archive.add(
                current_file,
                arcname=os.path.join(
                    top_level_dir, os.path.relpath(current_file, rel_to)
                ),
                recursive=False,
                filter=tar_filter,
            )

    return archive_name


def main() -> None:
    parser = argparse.ArgumentParser(
        "Creates a source archive of a git repository (including submodules)"
    )
    parser.add_argument(
        "--format",
        choices=["tar", "zip"],
        default="zip",
        help="What kind of archive to create",
    )
    parser.add_argument(
        "--remote",
        help="The path/URL to the remote git repository. If not given, the current git repo is used",
    )
    parser.add_argument(
        "--origin-name", default="origin", help="Name of the origin remote"
    )
    parser.add_argument(
        "--revision",
        default="master",
        help="Name of the revision (tag or branch) for which an archive shall be created",
    )
    parser.add_argument("--name", help="Name of the archive to be produced")

    args = parser.parse_args()

    remote: str = args.remote
    if not remote:
        remote: str = git(["remote", "get-url", args.origin_name])

    archive_name: str = args.name if args.name else "mumble-" + args.revision

    # Clone the repo into a temp directory
    with tempfile.TemporaryDirectory() as tmp_repo:
        print("Creating temporary clone at '%s'..." % tmp_repo)
        git(
            [
                "clone",
                "--depth",
                "1",
                "--recurse-submodules",
                "--shallow-submodules",
                "--branch",
                args.revision,
                remote,
                tmp_repo,
            ]
        )

        files: List[str] = get_file_paths(tmp_repo)

        print("Archiving repo (%d files & folders)..." % len(files))

        if args.format == "zip":
            archive_name = create_zip_archive(files, archive_name, rel_to=tmp_repo)
        elif args.format == "tar":
            archive_name = create_tar_archive(files, archive_name, rel_to=tmp_repo)
        else:
            raise RuntimeError("Unsupported archive format '%s'" % args.format)

        print("Archive created at '%s'" % archive_name)


if __name__ == "__main__":
    main()
