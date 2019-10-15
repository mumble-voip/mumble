#!/bin/bash -ex
#
# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if [ "${TRAVIS_REPO_SLUG}" == "mumble-voip/mumble" ] && [ "${TRAVIS_PULL_REQUEST}" == "false" ]; then
		if [ "${TRAVIS_BRANCH}" == "master" ]; then
			MASTER_BRANCH=1
		fi
fi

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	if [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		EXTRA_CONFIG=PREFIX=/usr
		if [ "${MUMBLE_NO_PCH}" == "1" ]; then
			EXTRA_CONFIG="no-pch ${EXTRA_CONFIG}"
		fi
		qmake CONFIG+="release tests g15-emulator ${EXTRA_CONFIG}" DEFINES+="MUMBLE_VERSION=${TRAVIS_COMMIT:0:7}" -recursive
		make -j2 && make check
	elif [ "${MUMBLE_HOST}" == "aarch64-linux-gnu" ]; then
		qmake CONFIG+="release tests warnings-as-errors ${EXTRA_CONFIG}" -recursive
		make -j $(nproc)
	elif [ "${MUMBLE_HOST}" == "i686-w64-mingw32" ]; then
		wget http://www.steinberg.net/sdk_downloads/asiosdk2.3.zip -P ../
		unzip ../asiosdk2.3.zip -d ../
		mv ../ASIOSDK2.3 3rdparty/asio
		PATH=$PATH:/usr/lib/mxe/usr/bin
		export MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-pc-linux-gnu/bin/protoc
		EXTRA_CONFIG=
		if [ "${MUMBLE_NO_PCH}" == "1" ]; then
			EXTRA_CONFIG="no-pch ${EXTRA_CONFIG}"
		fi
		${MUMBLE_HOST}.static-qmake-qt5 -recursive -Wall CONFIG+="release tests warnings-as-errors g15-emulator no-overlay no-bonjour no-elevation no-ice ${EXTRA_CONFIG}"
		make -j2
		make check TESTRUNNER="wine-development"
	elif [ "${MUMBLE_HOST}" == "x86_64-w64-mingw32" ]; then
		wget http://www.steinberg.net/sdk_downloads/asiosdk2.3.zip -P ../
		unzip ../asiosdk2.3.zip -d ../
		mv ../ASIOSDK2.3 3rdparty/asio
		PATH=$PATH:/usr/lib/mxe/usr/bin
		export MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-pc-linux-gnu/bin/protoc
		EXTRA_CONFIG=
		if [ "${MUMBLE_NO_PCH}" == "1" ]; then
			EXTRA_CONFIG="no-pch ${EXTRA_CONFIG}"
		fi
		${MUMBLE_HOST}.static-qmake-qt5 -recursive -Wall CONFIG+="release tests warnings-as-errors g15-emulator no-overlay no-bonjour no-elevation no-ice ${EXTRA_CONFIG}"
		make -j2
		make check TESTRUNNER="wine-development"
	else
		exit 1
	fi
else
	exit 1
fi
