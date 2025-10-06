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


MUMBLE_ENVIRONMENT_DIR="$workspace/build_env"
MUMBLE_ENVIRONMENT_SOURCE="https://github.com/mumble-voip/vcpkg/releases/download/2025-08"
# Note: We currently overwrite this for Linux
MUMBLE_ENVIRONMENT_COMMIT="17f4c23245"
MUMBLE_ENVIRONMENT_VERSION=""
ADDITIONAL_CMAKE_OPTIONS=""
VCPKG_CMAKE_OPTIONS=""

VCPKG_TARGET_TRIPLET=""
if [[ "$arch" == "x86_64" ]]; then
	VCPKG_TARGET_TRIPLET="x64"
else
	echo "Unknown architecture '$arch'"
	exit 1
fi


case "$os" in
	"ubuntu")
		VCPKG_TARGET_TRIPLET="$VCPKG_TARGET_TRIPLET-linux"
		MUMBLE_ENVIRONMENT_COMMIT="8834a23a8e"
		;;
	"windows")
		VCPKG_TARGET_TRIPLET="$VCPKG_TARGET_TRIPLET-windows-static-md"
		;;
	"macos")
		VCPKG_TARGET_TRIPLET="$VCPKG_TARGET_TRIPLET-osx"
		;;
	*)
		echo "OS $os is not supported"
		exit 1
		;;
esac

MUMBLE_ENVIRONMENT_VERSION="mumble_env.${VCPKG_TARGET_TRIPLET}.${MUMBLE_ENVIRONMENT_COMMIT}"

if [[ "$build_type" == "static" ]]; then
	ADDITIONAL_CMAKE_OPTIONS="$ADDITIONAL_CMAKE_OPTIONS -Dstatic=ON"

	VCPKG_CMAKE_OPTIONS="-DCMAKE_TOOLCHAIN_FILE='$MUMBLE_ENVIRONMENT_DIR/scripts/buildsystems/vcpkg.cmake'" 
	VCPKG_CMAKE_OPTIONS="$VCPKG_CMAKE_OPTIONS -DVCPKG_TARGET_TRIPLET='$VCPKG_TARGET_TRIPLET'"
	VCPKG_CMAKE_OPTIONS="$VCPKG_CMAKE_OPTIONS -DIce_HOME='$MUMBLE_ENVIRONMENT_DIR/installed/$VCPKG_TARGET_TRIPLET'"
fi

# set environment variables in a way that GitHub Actions understands and preserves
echo "MUMBLE_ENVIRONMENT_SOURCE=$MUMBLE_ENVIRONMENT_SOURCE" >> "$GITHUB_ENV"
echo "MUMBLE_ENVIRONMENT_DIR=$MUMBLE_ENVIRONMENT_DIR" >> "$GITHUB_ENV"
echo "MUMBLE_ENVIRONMENT_VERSION=$MUMBLE_ENVIRONMENT_VERSION" >> "$GITHUB_ENV"
echo "ADDITIONAL_CMAKE_OPTIONS=$ADDITIONAL_CMAKE_OPTIONS" >> "$GITHUB_ENV"
echo "VCPKG_CMAKE_OPTIONS=$VCPKG_CMAKE_OPTIONS" >> "$GITHUB_ENV"
echo "MUMBLE_VCPKG_TRIPLET=$VCPKG_TARGET_TRIPLET" >> "$GITHUB_ENV"
echo "QT_DEBUG_PLUGINS=1" >> "$GITHUB_ENV"

if [[ "$os" = "ubuntu" ]]; then
	# Setting this is necessary in order to be able to run tests on the CLI
	echo "QT_QPA_PLATFORM=offscreen" >> "$GITHUB_ENV"
fi
