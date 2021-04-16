#!/bin/bash -ex
#
# Copyright 2020-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

sudo apt-get update

sudo apt-get -y install build-essential g++-multilib ninja-build pkg-config \
                        qt5-default qttools5-dev qttools5-dev-tools libqt5svg5-dev \
                        libboost-dev libssl-dev libprotobuf-dev protobuf-compiler libprotoc-dev \
                        libcap-dev libxi-dev \
                        libasound2-dev libasound2-plugins libasound2-plugins-extra\
                        libogg-dev libsndfile1-dev libspeechd-dev \
                        libavahi-compat-libdnssd-dev libzeroc-ice-dev \
                        zsync appstream libgrpc++-dev protobuf-compiler-grpc \
                        libpoco-dev
