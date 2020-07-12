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

echo "Environment not cached. Downloading..."

wget "$MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.tar.xz"

echo "Extracting build environment to $MUMBLE_ENVIRONMENT_STORE..."

mkdir -p $MUMBLE_ENVIRONMENT_STORE

tar xf "$MUMBLE_ENVIRONMENT_VERSION.tar.xz" -C $MUMBLE_ENVIRONMENT_STORE

chmod +x "$MUMBLE_ENVIRONMENT_PATH/installed/x64-osx/tools/Ice/slice2cpp"

ls -l $MUMBLE_ENVIRONMENT_STORE
