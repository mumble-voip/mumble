#!/bin/bash -ex
#
# Copyright 2017-2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if [ "${TRAVIS_REPO_SLUG}" == "mumble-voip/mumble" ] && [ "${TRAVIS_PULL_REQUEST}" == "false" ]; then
		if [ "${TRAVIS_BRANCH}" == "master" ]; then
			MASTER_BRANCH=1
		fi
fi

RELEASE_ID=$(python scripts/mumble-version.py)

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	if [ "${MUMBLE_HOST}" == "aarch64-linux-gnu" ]; then
		mkdir build && cd build

		cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -Dtests=ON -DRELEASE_ID=$RELEASE_ID -Dsymbols=ON -Donline-tests=ON -Dgrpc=ON -Ddisplay-install-paths=ON ..

		cmake --build .
		# We don't execute tests on ARM64 because TestPacketDataStream fails.
		# See https://github.com/mumble-voip/mumble/issues/3845.
		#ctest
		sudo cmake --install .
	elif [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		mkdir build && cd build

		# We specify the absolute path because otherwise Travis CI's CMake is used.
		/usr/bin/cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -Dtests=ON -DRELEASE_ID=$RELEASE_ID -Dsymbols=ON -Donline-tests=ON -Dgrpc=ON \
		-Ddisplay-install-paths=ON ..

		/usr/bin/cmake --build .
		/usr/bin/ctest --verbose
		sudo /usr/bin/cmake --install .
	elif [ "${MUMBLE_HOST}" == "i686-w64-mingw32" ] || [ "${MUMBLE_HOST}" == "x86_64-w64-mingw32" ]; then
		wget https://dl.mumble.info/build/extra/asio_sdk.zip -P /tmp/
		unzip /tmp/asio_sdk.zip
		mv asiosdk_2.3.3_2019-06-14 3rdparty/asio

		mkdir build && cd build

		PATH=$PATH:/usr/lib/mxe/usr/bin

		${MUMBLE_HOST}.static-cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -Dtests=ON -DRELEASE_ID=$RELEASE_ID -Dstatic=ON -Dsymbols=ON -Dasio=ON \
			-Dzeroconf=OFF -Dice=OFF -Doverlay=OFF -Donline-tests=ON -Ddisplay-install-paths=ON ..
		cmake --build .
		# TODO: investigate why tests fail.
		#ctest
	else
		exit 1
	fi
else
	exit 1
fi
