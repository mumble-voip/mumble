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
		if [ "${MUMBLE_NO_PCH}" == "1" ] && [ "${MASTER_BRANCH}" = "1" ]; then
			# Wraps the compilation with the Build Wrapper to generate the configuration used later by the SonarQube Scanner.
			mkdir build
			build-wrapper-linux-x86-64 --out-dir build/sonar make -j 2
			make check

			# Run the SonarQube analysis
			sonar-scanner
		else
			make -j2 && make check
		fi
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
		make check TESTRUNNER="wine"
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
		make check TESTRUNNER="wine"
	else
		exit 1
	fi
elif [ "${TRAVIS_OS_NAME}" == "osx" ]; then
	export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
	export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opt/openssl/lib/pkgconfig
	export PATH=$PATH:/usr/local/opt/qt5/bin:/usr/local/bin
	export MUMBLE_PREFIX=/usr/local
	export MUMBLE_ICE_PREFIX=/usr/local/opt/ice
	qmake CONFIG+="release tests warnings-as-errors" && make -j2 && make check
	./macx/scripts/osxdist.py
else
	exit 1
fi
