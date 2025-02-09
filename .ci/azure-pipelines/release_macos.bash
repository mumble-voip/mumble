#!/usr/bin/env bash -ex
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Below is a list of configuration variables used from environment.
#
# Predefined variables:
#
#  BUILD_SOURCESDIRECTORY       - The local path on the agent where the
#                                 repository is downloaded.
#

VERSION=$(python3 "scripts/mumble-version.py")

cd $BUILD_BINARIESDIRECTORY

$BUILD_SOURCESDIRECTORY/macx/scripts/osxdist.py --version=$VERSION --source-dir=$BUILD_SOURCESDIRECTORY --binary-dir=.

mv *.dmg $BUILD_ARTIFACTSTAGINGDIRECTORY
