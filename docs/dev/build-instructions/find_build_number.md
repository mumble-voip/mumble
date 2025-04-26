# Finding the correct BUILD_NUMBER

When invoking CMake, you can specify a build number as e.g. `-DBUILD_NUMBER=42`. The build number will be used as the third component of the Mumble
version.

First off: This is only important if you are planning to distribute/package Mumble (it makes sure that Mumble can be upgraded properly). For
self-compiled versions this doesn't really matter and you can simply omit the build number.

The build number is automatically increased with every commit that is made to Mumble. All build numbers for all commits can be found on
https://mumble.info/build-number.json. Simply look up the current commit hash (topmost entry in the result of `git log`) in the list and use the
corresponding build number.

Instead of manually looking up the commit of interest yourself, you could instead use the script `scripts/mumble-build-number.py`. You have to
pass the commit hash and the Mumble version (as specified in the main `CMakeLists.txt` file) to it and it will fetch the corresponding build number
for you. For instance:
```bash
python3 mumble-build-number.py --commit 53ddee77364482a8385ce7244a233bf10412d76a --version 1.5
```

Note that not all commits will have a build number associated with them (due to how our CI works). As a general rule of thumbs, only merge-commits will
yield a build number. However, if you did not explicitly switch to a specific commit, the current HEAD (can be obtained via `git rev-parse HEAD`) will
most likely have a build number associated with it.

