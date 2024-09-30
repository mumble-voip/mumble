#!/bin/bash -ex
#
# Copyright The Mumble Developers. All rights reserved.
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

VERSION=$("./scripts/mumble-version.py")
BUILD_NUMBER=$("./scripts/mumble-build-number.py" --commit "${BUILD_SOURCEVERSION}" --version "${VERSION}" \
	--password "${MUMBLE_BUILD_NUMBER_TOKEN}" --default 0)

cd $BUILD_BINARIESDIRECTORY

cmake -G Ninja -DCMAKE_INSTALL_PREFIX=appdir/usr -DCMAKE_UNITY_BUILD=ON \
      -DCMAKE_BUILD_TYPE=Release -DBUILD_NUMBER=$BUILD_NUMBER \
      -Dtests=ON -Dsymbols=ON \
      -Ddisplay-install-paths=ON $BUILD_SOURCESDIRECTORY

cmake --build .

