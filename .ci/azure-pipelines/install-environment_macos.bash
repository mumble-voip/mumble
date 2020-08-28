#!/bin/bash -ex
#
# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

cd $AGENT_TEMPDIRECTORY

brew install ninja

if [ -d $MUMBLE_ENVIRONMENT_PATH ]; then
	exit 0
fi

# We use axel to download the environment
brew install axel

echo "Environment not cached. Downloading..."

environmentArchive="$MUMBLE_ENVIRONMENT_VERSION.tar.xz"

axel -n 10 --output="$environmentArchive" "$MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.tar.xz"

echo "Extracting build environment to $MUMBLE_ENVIRONMENT_STORE..."

mkdir -p $MUMBLE_ENVIRONMENT_STORE

tar xf "$environmentArchive" -C $MUMBLE_ENVIRONMENT_STORE

chmod +x "$MUMBLE_ENVIRONMENT_PATH/installed/x64-osx/tools/Ice/slice2cpp"

ls -l $MUMBLE_ENVIRONMENT_STORE
