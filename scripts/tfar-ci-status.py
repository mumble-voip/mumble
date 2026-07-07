#!/usr/bin/env python3
# MUMBLE-TFAR
# Polls the public GitHub API for the latest workflow run of a commit/branch
# and prints per-job status + all failure annotations (compiler errors show up
# here thanks to .github/msvc-problem-matcher.json). Works without any token.
#
# Usage:
#   scripts/tfar-ci-status.py [--repo StormofTheGalaxy/mumble-tfar] [--ref master] [--wait]

import argparse
import json
import sys
import time
import urllib.request

API = "https://api.github.com"


def get(url):
    req = urllib.request.Request(url, headers={"Accept": "application/vnd.github+json",
                                               "User-Agent": "tfar-ci-status"})
    with urllib.request.urlopen(req) as response:
        return json.load(response)


def latest_run(repo, ref):
    runs = get(f"{API}/repos/{repo}/actions/runs?per_page=20")["workflow_runs"]
    for run in runs:
        if ref in (run["head_branch"], run["head_sha"][:len(ref)]):
            return run
    return runs[0] if runs else None


def print_run(repo, run):
    print(f"run {run['id']}  [{run['name']}]  branch={run['head_branch']}  "
          f"sha={run['head_sha'][:8]}  status={run['status']}  conclusion={run['conclusion']}")
    jobs = get(f"{API}/repos/{repo}/actions/runs/{run['id']}/jobs?per_page=30")["jobs"]
    failed = False
    for job in jobs:
        marker = {"success": "OK  ", "failure": "FAIL", None: "... "}.get(job["conclusion"], "??? ")
        print(f"  {marker} {job['name']}")
        if job["conclusion"] == "failure":
            failed = True
            annotations = get(f"{API}/repos/{repo}/check-runs/{job['id']}/annotations")
            for a in annotations:
                if a["annotation_level"] in ("failure", "error"):
                    print(f"       {a['path']}:{a['start_line']}: {a['message'][:400]}")
    return failed


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--repo", default="StormofTheGalaxy/mumble-tfar")
    parser.add_argument("--ref", default="master", help="branch name or sha prefix")
    parser.add_argument("--wait", action="store_true", help="poll until the run completes")
    args = parser.parse_args()

    run = latest_run(args.repo, args.ref)
    if not run:
        print("no runs found")
        return 2

    while args.wait and run["status"] != "completed":
        print(f"waiting... status={run['status']}")
        time.sleep(60)
        run = get(f"{API}/repos/{args.repo}/actions/runs/{run['id']}")

    failed = print_run(args.repo, run)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
