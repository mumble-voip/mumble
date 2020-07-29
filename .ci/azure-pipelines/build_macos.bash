#!/bin/bash -ex
#
# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Below is a list of configuration variables used from environment.
#
#  BUILD_BINARIESDIRECTORY - The local path on the agent that can be used
#                            as an output folder for compiled binaries.
#  BUILD_SOURCESDIRECTORY  - The local path on the agent where the
#                            repository is downloaded.
#

VER=$(python scripts/mumble-version.py)

cd $BUILD_BINARIESDIRECTORY

cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=$MUMBLE_ENVIRONMENT_TOOLCHAIN -DIce_HOME="$MUMBLE_ENVIRONMENT_PATH/installed/x64-osx" -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -Dversion=$VER -Dstatic=ON -Dsymbols=ON $BUILD_SOURCESDIRECTORY
cmake --build .
ctest --verbose

$BUILD_SOURCESDIRECTORY/macx/scripts/osxdist.py --version=$VER --source-dir=$BUILD_SOURCESDIRECTORY --binary-dir=.

mv *.dmg $BUILD_ARTIFACTSTAGINGDIRECTORY
