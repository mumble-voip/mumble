#!/bin/bash -ex
#
# Copyright 2019-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.
#
# Below is a list of configuration variables used from environment.
#
#  BUILD_BINARIESDIRECTORY - The local path on the agent that can be used
#                            as an output folder for compiled binaries.
#  BUILD_SOURCESDIRECTORY  - The local path on the agent where the
#                            repository is downloaded.
#

VER=$(python scripts/mumble-version.py)

cd $BUILD_BINARIESDIRECTORY

# QSslDiffieHellmanParameters was introduced in Qt 5.8, Ubuntu 16.04 has 5.5.
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=appdir/usr -Dtests=ON Donline-tests=ON -Dversion=$VER -Dsymbols=ON \
	-Dqssldiffiehellmanparameters=OFF -Donline-tests=ON $BUILD_SOURCESDIRECTORY

cmake --build .
ctest --verbose
cmake --install .

