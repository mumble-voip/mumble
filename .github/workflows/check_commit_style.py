#!/usr/bin/env python3

import sys
import os
import platform
import subprocess

sys.path.append(os.path.realpath(__file__ + "/../../../scripts/commitMessage"))

from CommitMessage import CommitMessage, CommitFormatError

def cmd(args):
    shell = platform.system() == 'Windows'
    p = subprocess.Popen(args, shell=shell, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = p.communicate()
    if p.returncode != 0:
        raise Exception('cmd(): {0} failed with status {1}: {2}'.format(args, p.returncode, stderr))
    return stdout.decode('utf-8')


def main():
    targetBranch = ""
    if len(sys.argv) > 1:
        targetBranch = sys.argv[1]
    else:
        raise RuntimeError("Target branch not specified!")

    print("Checking commit styles - target branch: \"{}\"".format(targetBranch))

    try:
        # Set up remote 
        remoteName = "mumble-upstream"
        cmd(["git", "remote", "add", remoteName, "https://github.com/mumble-voip/mumble.git"])

        # Fetch remote
        cmd(["git", "fetch", "--no-recurse-submodules", remoteName])

        # get new commits
        commitHashes = [x for x in cmd(["git", "rev-list", "{}/{}..HEAD".format(remoteName, targetBranch)]).split("\n") if x]
        # Reverse the order of the commits so that oldest comes first
        commitHashes.reverse()

        encounteredErrors = False

        for currentHash in commitHashes:
            description = cmd(["git", "show", "-s", "--format=%B", currentHash])
            
            try:
                # If we can create a commit message object, this means that the commit message does follow all our
                # requirements
                CommitMessage(description)

                print("Commit {} passed all checks".format(currentHash))
            except CommitFormatError as e:
                encounteredErrors = True
                
                print()
                print("Commit {} does NOT follow the required commit style:".format(currentHash))
                print("  " + str(e))
                print("    The commit message was:")
                print("    \"" + description.replace("\n", "\n    ").strip() + "\"")
                print()


        if encounteredErrors:
            print("[ERROR]: There were commits in invalid format!")
            sys.exit(1)

    finally:
        # remove remote again
        cmd(["git", "remote", "remove", remoteName])

if __name__ == "__main__":
    main()
