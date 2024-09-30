#!/bin/bash -ex
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Builds Mumble using the specified build script.
# The path to the script is relative to the build environment's root.
# The configuration we build with is adjusted to be close to
# our release builds.
#
# Below is a list of configuration variables used from environment.
#
# Predefined variables:
#
#  BUILD_BINARIESDIRECTORY      - The local path on the agent that can be used
#                                 as an output folder for compiled binaries.
#  BUILD_SOURCESDIRECTORY       - The local path on the agent where the
#                                 repository is downloaded.
#
# Defined in .azure-pipelines.yml:
#
#  MUMBLE_ENVIRONMENT_PATH      - Path to the environment's root directory.
#  MUMBLE_ENVIRONMENT_TOOLCHAIN - Path to the vcpkg CMake toolchain, used for CMake.
#  MUMBLE_ENVIRONMENT_TRIPLET   - vcpkg triplet.
#
# Defined on Azure Pipelines:
#
#  MUMBLE_BUILD_NUMBER_TOKEN           - Access token for the build number page on our server.
#

VERSION=$("./scripts/mumble-version.py")
BUILD_NUMBER=$("./scripts/mumble-build-number.py" --commit "${BUILD_SOURCEVERSION}" --version "${VERSION}" \
	--password "${MUMBLE_BUILD_NUMBER_TOKEN}" --default 0)

cd $BUILD_BINARIESDIRECTORY

cmake -G Ninja -DVCPKG_TARGET_TRIPLET=$MUMBLE_ENVIRONMENT_TRIPLET -DCMAKE_TOOLCHAIN_FILE=$MUMBLE_ENVIRONMENT_TOOLCHAIN \
      -DIce_HOME="$MUMBLE_ENVIRONMENT_PATH/installed/$MUMBLE_ENVIRONMENT_TRIPLET" \
      -DCMAKE_BUILD_TYPE=Release -DCMAKE_UNITY_BUILD=ON -DBUILD_NUMBER=$BUILD_NUMBER \
      -Dtests=ON -Dstatic=ON -Dsymbols=ON \
      -Ddisplay-install-paths=ON $BUILD_SOURCESDIRECTORY

cmake --build .

