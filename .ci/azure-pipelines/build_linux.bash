#!/bin/bash -ex
#
# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Below is a list of configuration variables used from environment.
#
# Predefined variables:
#
#  BUILD_BINARIESDIRECTORY - The local path on the agent that can be used
#                            as an output folder for compiled binaries.
#  BUILD_SOURCESDIRECTORY  - The local path on the agent where the
#                            repository is downloaded.
#
# Defined on Azure Pipelines:
#
#  MUMBLE_BUILD_NUMBER_TOKEN      - Access token for the build number page on our server.
#

if [[ -n "$MUMBLE_BUILD_NUMBER_TOKEN" ]]; then
	VERSION=$(python "scripts/mumble-version.py" --format version)
	BUILD_NUMBER=$(curl "https://mumble.info/get-build-number?commit=${BUILD_SOURCEVERSION}&version=${VERSION}&token=${MUMBLE_BUILD_NUMBER_TOKEN}")
else
	BUILD_NUMBER=0
fi

RELEASE_ID=$(python "scripts/mumble-version.py")

cd $BUILD_BINARIESDIRECTORY

cmake -G Ninja -DCMAKE_INSTALL_PREFIX=appdir/usr -DCMAKE_UNITY_BUILD=ON \
      -DCMAKE_BUILD_TYPE=Release -DRELEASE_ID=$RELEASE_ID -DBUILD_NUMBER=$BUILD_NUMBER \
      -Dtests=ON -Dsymbols=ON -Dgrpc=ON \
      -Ddisplay-install-paths=ON $BUILD_SOURCESDIRECTORY

cmake --build .

