#!/usr/bin/env bash

set -e
set -x

os=$1
build_type=$2
arch=$3
workspace=$4


if [[ "$os" == "" || "$build_type" == "" || "$arch" == "" || "$workspace" == "" ]]; then
	echo "Invalid parameters"
	exit 1
fi

# Turn variables into lowercase
os="${os,,}"
# only consider name up to the hyphen
os=$(echo "$os" | sed 's/-.*//')
build_type="${build_type,,}"
arch="${arch,,}"


MUMBLE_ENVIRONMENT_DIR="$workspace/mumble-build-environment"
MUMBLE_BUILD_ENV_PATH=""
MUMBLE_ENVIRONMENT_VERSION=""
ADDITIONAL_CMAKE_OPTIONS=""
VCPKG_CMAKE_OPTIONS=""

case "$os" in
	"ubuntu")
		# We have to use the version without debug symbols in order for the size of the
		# uncompressed archive to not exceed CI size limits
		MUMBLE_ENVIRONMENT_VERSION="linux-static-1.4.x-2020-08-24-f65cd5d-1168-no-debug"
		;;
	"windows")
		if [[ "$arch" == "64bit" ]]; then
			MUMBLE_ENVIRONMENT_VERSION="win64-static-1.4.x-2020-07-22-dbd6271-1162"
		else
			MUMBLE_ENVIRONMENT_VERSION="win64-static-1.4.x-2020-07-22-dbd6271-1162"
		fi
		;;
	"macos")
		MUMBLE_ENVIRONMENT_VERSION="macos-static-1.4.x-2020-07-22-dbd6271-1162"
		;;
	*)
		echo "OS $os is not supported"
		exit 1
		;;
esac

MUMBLE_BUILD_ENV_PATH="$MUMBLE_ENVIRONMENT_DIR/$MUMBLE_ENVIRONMENT_VERSION"

if [[ "$build_type" == "static" ]]; then
	ADDITIONAL_CMAKE_OPTIONS="$ADDITIONAL_CMAKE_OPTIONS -Dstatic=ON"

	VCPKG_TARGET_TRIPLET=""
	case "$os" in
		"ubuntu")
			VCPKG_TARGET_TRIPLET="linux"
			;;
		"windows")
			VCPKG_TARGET_TRIPLET="windows-static-md"
			ADDITIONAL_CMAKE_OPTIONS="$ADDITIONAL_CMAKE_OPTIONS -Dpackaging=ON"
			;;
		"macos")
			VCPKG_TARGET_TRIPLET="osx"
			;;
	esac

	if [[ "$arch" == "64bit" ]]; then
		VCPKG_TARGET_TRIPLET="x64-$VCPKG_TARGET_TRIPLET"
	else
		VCPKG_TARGET_TRIPLET="x32-$VCPKG_TARGET_TRIPLET"
	fi

	VCPKG_CMAKE_OPTIONS="-DCMAKE_TOOLCHAIN_FILE='$MUMBLE_BUILD_ENV_PATH/scripts/buildsystems/vcpkg.cmake' 
		-DVCPKG_TARGET_TRIPLET='$VCPKG_TARGET_TRIPLET'
		-DIce_HOME='$MUMBLE_BUILD_ENV_PATH/installed/$VCPKG_TARGET_TRIPLET'"
fi

# set environment variables in a way that GitHub Actions understands and preserves
echo "MUMBLE_ENVIRONMENT_DIR=$MUMBLE_ENVIRONMENT_DIR" >> "$GITHUB_ENV"
echo "MUMBLE_BUILD_ENV_PATH=$MUMBLE_BUILD_ENV_PATH" >> "$GITHUB_ENV"
echo "MUMBLE_ENVIRONMENT_VERSION=$MUMBLE_ENVIRONMENT_VERSION" >> "$GITHUB_ENV"
echo "ADDITIONAL_CMAKE_OPTIONS=$ADDITIONAL_CMAKE_OPTIONS" >> "$GITHUB_ENV"
echo "VCPKG_CMAKE_OPTIONS=$VCPKG_CMAKE_OPTIONS" >> "$GITHUB_ENV"
