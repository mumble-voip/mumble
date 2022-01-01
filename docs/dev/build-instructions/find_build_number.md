# Finding the correct BUILD_NUMBER

When invoking CMake, you can specify a build number as e.g. `-DBUILD_NUMBER=42`. The build number will be used as the third component of the Mumble
version.

First off: This is only important if you are planning to distribute/package Mumble (it makes sure that Mumble can be upgraded properly). For
self-compiled versions this doesn't really matter and you can simply omit the build number.

The build number is automatically increased with every commit that is made to Mumble. All build numbers for all commits can be found on
https://mumble.info/build-number.json. Simply look up the current commit hash (topmost entry in the result of `git log`) in the list and use the
corresponding build number.

