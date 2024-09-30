#!/bin/bash -ex
#
# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

currentDir=$(pwd)
cd $AGENT_TEMPDIRECTORY

brew install ninja

if [ -d $MUMBLE_ENVIRONMENT_PATH ]; then
	exit 0
fi

# We use axel to download the environment
brew install axel
# We use gtar instead of tar as the default tar on macOS doesn't support the --record-size option
brew install gnu-tar

echo "Environment not cached. Downloading..."

environmentArchive="$MUMBLE_ENVIRONMENT_VERSION.tar.xz"

iteration=0
maxIterations=3

while [ $iteration -lt $maxIterations ]; do
	# By using && we avoid Bash exiting if the command fails even if the -e flag is set
	axel -n 10 --output="$environmentArchive" "$MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.tar.xz" && break

	iteration=$(( $iteration + 1 ))

	sleep 5
done

if [ $iteration -ge $maxIterations ]; then
	echo "Failed at downloading the build environment"
	exit 1
fi

echo "Extracting build environment to $MUMBLE_ENVIRONMENT_STORE..."

mkdir -p $MUMBLE_ENVIRONMENT_STORE

"$currentDir"/.ci/azure-pipelines/extractWithProgress.bash "$environmentArchive" $MUMBLE_ENVIRONMENT_STORE

chmod +x "$MUMBLE_ENVIRONMENT_PATH/installed/$MUMBLE_ENVIRONMENT_TRIPLET/tools/Ice/slice2cpp"

ls -l $MUMBLE_ENVIRONMENT_STORE
