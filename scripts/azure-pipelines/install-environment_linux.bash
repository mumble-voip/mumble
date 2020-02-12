#!/bin/bash -ex
#
# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

sudo add-apt-repository ppa:beineri/opt-qt-5.12.7-xenial -y

sudo apt-get update

sudo apt-get -y install build-essential pkg-config qt512base qt512svg qt512tools qt512translations \
                        libboost-dev libssl-dev libprotobuf-dev protobuf-compiler \
                        libcap-dev libxi-dev libgl1-mesa-dev \
                        libasound2-dev libpulse-dev \
                        libogg-dev libsndfile1-dev libspeechd-dev \
                        libavahi-compat-libdnssd-dev libzeroc-ice-dev libg15daemon-client-dev \
                        zsync
