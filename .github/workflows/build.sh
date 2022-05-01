#!/usr/bin/env bash

set -e
set -x

buildDir="${GITHUB_WORKSPACE}/build"

mkdir "$buildDir"

cd "$buildDir"

VERSION=$("${GITHUB_WORKSPACE}/scripts/mumble-version.py")
BUILD_NUMBER=$("${GITHUB_WORKSPACE}/scripts/mumble-build-number.py" --commit "${GITHUB_SHA}" --version "${VERSION}" \
	--password "${MUMBLE_BUILD_NUMBER_TOKEN}" --default 0)

# Run cmake with all necessary options
cmake -G Ninja \
	  -S "$GITHUB_WORKSPACE" \
	  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	  -DBUILD_NUMBER=$BUILD_NUMBER \
	  $CMAKE_OPTIONS \
      -DCMAKE_UNITY_BUILD=ON \
	  $ADDITIONAL_CMAKE_OPTIONS \
	  $VCPKG_CMAKE_OPTIONS

# Actually build
cmake --build . --config $BUILD_TYPE

