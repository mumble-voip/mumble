#!/bin/bash -ex
#
# Copyright 2019-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

currentDir=$(pwd)
cd $AGENT_TEMPDIRECTORY

sudo apt-get update

sudo apt-get -y install build-essential g++-multilib ninja-build pkg-config \
                        qt5-default qttools5-dev qttools5-dev-tools libqt5svg5-dev \
                        libboost-dev libssl-dev libprotobuf-dev protobuf-compiler libprotoc-dev \
                        libcap-dev libxi-dev \
                        libasound2-dev libasound2-plugins libasound2-plugins-extra\
                        libogg-dev libsndfile1-dev libspeechd-dev \
                        libavahi-compat-libdnssd-dev libzeroc-ice-dev \
                        zsync appstream libgrpc++-dev protobuf-compiler-grpc

if [ -d $MUMBLE_ENVIRONMENT_PATH ]; then
	exit 0
fi

# We use axel to download the environment
sudo apt-get -y install axel

echo "Environment not cached. Downloading..."

environmentArchive="$MUMBLE_ENVIRONMENT_VERSION.tar.xz"

axel -n 10 --output="$environmentArchive" "$MUMBLE_ENVIRONMENT_SOURCE/$MUMBLE_ENVIRONMENT_VERSION.tar.xz"

echo "Extracting build environment to $MUMBLE_ENVIRONMENT_STORE..."

mkdir -p $MUMBLE_ENVIRONMENT_STORE

"$currentDir"/.ci/azure-pipelines/extractWithProgress.bash "$environmentArchive" $MUMBLE_ENVIRONMENT_STORE

ls -l $MUMBLE_ENVIRONMENT_STORE
