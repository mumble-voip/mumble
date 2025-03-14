#!/usr/bin/env bash

set -e
set -x

buildDir="${GITHUB_WORKSPACE}/build"

mkdir "$buildDir"

cd "$buildDir"

# Run cmake with all necessary options
cmake -G Ninja \
	  -S "$GITHUB_WORKSPACE" \
	  -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
	  -DBUILD_NUMBER=$MUMBLE_BUILD_NUMBER \
	  $CMAKE_OPTIONS \
      -DCMAKE_UNITY_BUILD=ON \
	  -Ddisplay-install-paths=ON \
	  -Ddatabase-sqlite-tests=ON \
	  -Ddatabase-mysql-tests=ON \
	  -Ddatabase-postgresql-tests=ON \
	  $ADDITIONAL_CMAKE_OPTIONS \
	  $VCPKG_CMAKE_OPTIONS

# Actually build
cmake --build . --config $BUILD_TYPE

