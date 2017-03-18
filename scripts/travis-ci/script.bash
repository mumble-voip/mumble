#!/bin/bash -ex
#
# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

if [ "${TRAVIS_OS_NAME}" == "linux" ]; then
	if [ "${MUMBLE_QT}" == "qt4" ] && [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		qmake-qt4 CONFIG+="release tests g15-emulator qt4-legacy-compat" -recursive && make -j2 && make check
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "x86_64-linux-gnu" ]; then
		qmake CONFIG+="release tests g15-emulator" -recursive && make -j2 && make check
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "i686-w64-mingw32" ]; then
		wget http://www.steinberg.net/sdk_downloads/asiosdk2.3.zip -P ../
		unzip ../asiosdk2.3.zip -d ../
		mv ../ASIOSDK2.3 3rdparty/asio
		PATH=$PATH:/usr/lib/mxe/usr/bin
		export MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-unknown-linux-gnu/bin/protoc
		${MUMBLE_HOST}.static-qmake-qt5 -recursive -Wall CONFIG+="release tests warnings-as-errors g15-emulator no-overlay no-bonjour no-elevation no-ice"
		make -j2
		make check TESTRUNNER="wine"
	elif [ "${MUMBLE_QT}" == "qt5" ] && [ "${MUMBLE_HOST}" == "x86_64-w64-mingw32" ]; then
		wget http://www.steinberg.net/sdk_downloads/asiosdk2.3.zip -P ../
		unzip ../asiosdk2.3.zip -d ../
		mv ../ASIOSDK2.3 3rdparty/asio
		PATH=$PATH:/usr/lib/mxe/usr/bin
		export MUMBLE_PROTOC=/usr/lib/mxe/usr/x86_64-unknown-linux-gnu/bin/protoc
		${MUMBLE_HOST}.static-qmake-qt5 -recursive -Wall CONFIG+="release tests warnings-as-errors g15-emulator no-overlay no-bonjour no-elevation no-ice"
		make -j2
		make check TESTRUNNER="wine"
	else
		exit 1
	fi
else
	exit 1
fi
